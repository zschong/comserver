#include "comconfig.h"
#include "varconfig.h"
#include "comserver.h"

#define MAX_COM	1

int main(void)
{
	VarConfig vars;
	ComConfig coms;
	ComServer server[MAX_COM];
	map<int,xstring> names;

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

	while(1)
	{
		sleep(1);
	}
	return 0;
}
