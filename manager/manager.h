#ifndef __MANAGER_H__
#define __MANAGER_H__
#include "comconfig.h"
#include "varconfig.h"
#include "comserver.h"
#include "modbustcp.h"
#include "jsonserver.h"
#include "debugserver.h"

#define MAX_COM		16	
#define TCP_PORT	1000
#define JSON_PORT	2000
#define DEBUG_PORT	3000

class Manager
{
	VarConfig vars;
	ComConfig coms;
	ComServer server[MAX_COM];
	JsonServer jsons[MAX_COM];
	DebugServer debugs[MAX_COM];
	ModbusTcpServer mbtcp[MAX_COM];
	map<int,xstring> names;

public:
	void Start(void);
private:
	void InitNames(void);
	void ReadConfigs(void);
	void StartComServer(void);
	void StartJsonServer(void);
	void StartDebugServer(void);
	void StartModubusServer(void);
};

#endif//__MANAGER_H__
