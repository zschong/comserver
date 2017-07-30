#include "manager.h"


void Manager::Start(void)
{
	for(int i = 0; i < MAX_COM; i++)
	{
		names[i].format("com%d", i+1);
	}
	for(int i = 0; i < MAX_COM; i++)
	{
		server[i].SetName(names[i]);
	}
	vars.ReadConfig("varconfig.txt");
	coms.ReadConfig("comconfig.txt");

	for(int i = 0; i < MAX_COM; i++)
	{
		ComNode node;
		map<int,ConfigNode> config;

		coms.GetConfig(names[i], node);
		server[i].SetCom(node);
		vars.GetConfig(names[i], config);
		server[i].SetConfig(config);
	}
	for(int i = 0; i< MAX_COM; i++)
	{
		mbtcp[i].SetComServer(server[i]);
		mbtcp[i].Start("0.0.0.0", 10000+i);
	}
	while(1)
	{
		sleep(1);
	}
}

int main(void)
{
	Manager manager;

	manager.Start();
}
