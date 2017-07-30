#ifndef __MANAGER_H__
#define __MANAGER_H__
#include "comconfig.h"
#include "varconfig.h"
#include "comserver.h"
#include "modbustcp.h"

#define MAX_COM	1

class Manager
{
	VarConfig vars;
	ComConfig coms;
	ComServer server[MAX_COM];
	ModbusTcpServer mbtcp[MAX_COM];
	map<int,xstring> names;

public:
	void Start(void);
};

#endif//__MANAGER_H__
