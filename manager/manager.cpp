#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include "manager.h"

#define RESTART_TIMEOUT		(2 * 60 * 60)

void Manager::Start(void)
{
	sleep(1);
	InitNames();
	ReadConfigs();
	StartComServer();
	//StartJsonServer();
	StartDebugServer();
	StartModubusServer();

	time_t timeout = time(0);

	while(1)
	{
		sleep(1);
		if( abs(time(0) - timeout) > RESTART_TIMEOUT )
		{
			exit(0);
		}
	}
}
void Manager::InitNames(void)
{
	for(int i = 0; i < MAX_COM; i++)
	{
		names[i].format("com%d", i+1);
	}
}
void Manager::ReadConfigs(void)
{
	vars.ReadConfig("varconfig.txt");
	coms.ReadConfig("comconfig.txt");
}
void Manager::StartComServer(void)
{
	for(int i = 0; i < MAX_COM; i++)
	{
		ComNode node;
		map<int,ConfigNode> config;

		server[i].SetName(names[i]);
		coms.GetConfig(names[i], node);
		server[i].SetCom(node);
		vars.GetConfig(names[i], config);
		server[i].SetConfig(config);
	}

}
void Manager::StartJsonServer(void)
{
	for(int i = 0; i< MAX_COM; i++)
	{
		jsons[i].SetComServer(server[i]);
		jsons[i].Start("0.0.0.0", JSON_PORT+i+1);
	}
}
void Manager::StartDebugServer(void)
{
	for(int i = 0; i< MAX_COM; i++)
	{
		debugs[i].SetComServer(server[i]);
		debugs[i].Start("0.0.0.0", DEBUG_PORT+i+1);
	}
}
void Manager::StartModubusServer(void)
{
	for(int i = 0; i< MAX_COM; i++)
	{
		mbtcp[i].SetComServer(server[i]);
		mbtcp[i].Start("0.0.0.0", TCP_PORT+i+1);
	}
}

int main(void)
{
	int child = -1;
	
begin:
	child = fork();
	if( -1 == child )
	{
		return -1;
	}
	if( 0 == child )
	{
		Manager manager;
		chdir("/root/");
		manager.Start();
	}
	else
	{
		int status = 0;
		int w = waitpid(child, &status, WUNTRACED | WCONTINUED);
		if( -1 == w )
		{
			return -1;
		}
		if( WIFEXITED(status) )
		{
			printf("child(%d) exited\n", child);
			goto begin;
		}
		if( WIFSIGNALED(status) )
		{
			printf("child(%d) signaled\n", child);
			goto begin;
		}
		kill(child, SIGKILL);
		printf("child(%d) signaled\n", child);
		sleep(1);
		goto begin;
	}

	return 0;
}
