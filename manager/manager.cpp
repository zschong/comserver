#include <unistd.h>
#include "manager.h"


void Manager::Start(void)
{
	sleep(1);
	InitNames();
	ReadConfigs();
	StartComServer();
	//StartJsonServer();
	StartDebugServer();
	StartModubusServer();

	while(1)
	{
		sleep(1);
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
	Manager manager;

	chdir("/root/");
	manager.Start();
}
