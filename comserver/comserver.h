#ifndef __COM_SERVER_H__
#define __COM_SERVER_H__
#include <map>
#include "thread.h"
#include "entity.h"
#include "modbus.h"
#include "storage.h"
#include "comreader.h"


class ComServer : public Thread
{
private:
	xstring name;
	Storage storage;
	ComNode comconfig;
	ComReader reader;
	Mutex comlock;
	Mutex cachelock;
	Mutex configlockA;
	bool debugmode;
	bool comchange;
	bool configchange;
	map<int,ValueNode> cache;
	map<int,ConfigNode> configA;
	map<int,ConfigNode> configB;
	map<int,ConfigNode>::iterator A,B;

public:
	ComServer(void):configchange(false),comchange(false),debugmode(false){}
public:
	bool SetCom(ComNode& com);
	bool SetName(const xstring& comname);
	bool SetConfig(map<int, ConfigNode>&);
	bool GetValue(const ConfigNode&, map<int, unsigned short>&);
	bool GetJson(xstring& s);
	bool SetDebugMode(bool);
	ComReader& GetComReader(void);
private:
	void Loop(void);	
	void RecvData(void);
	void CheckCom(void);
	void CheckConfig(void);
	void Request(ConfigNode& node);
	void Response(ModbusBase&, ModbusBase&);
};

#endif//__COM_SERVER_H__
