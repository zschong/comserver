#ifndef __MANAGER_H__
#define __MANAGER_H__
#include "comconfig.h"
#include "varconfig.h"
#include "comserver.h"
#include "modbustcp.h"
#include "jsonserver.h"

#define MAX_COM		8
#define TCP_PORT	1000
#define JSON_PORT	2000

class Manager
{
	VarConfig vars;
	ComConfig coms;
	ComServer server[MAX_COM];
	JsonServer jsons[MAX_COM];
	ModbusTcpServer mbtcp[MAX_COM];
	map<int,xstring> names;

public:
	void Start(void);
private:
	void InitNames(void);
	void ReadConfigs(void);
	void StartComServer(void);
	void StartJsonServer(void);
	void StartModubusServer(void);
};

#endif//__MANAGER_H__
