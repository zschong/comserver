#ifndef __MODBUS_TCP__H__
#define __MODBUS_TCP__H__
#include <map>
#include <vector>
#include "thread.h"
#include "tcpserver.h"
using namespace std;

class ModbusTcpServer : public Thread
{
	TcpServer server;
	TcpServer worker;
	vector<char> data;
	map<char,char> cmd;
	class ComServer *comserver;

public:
	ModbusTcpServer(void):comserver(0){}

public:
	void SetSlave(char slave);
	void SetComServer(class ComServer& com);
	bool Start(const string& ip, int port);
	bool RecvData(void);
	bool FindHeader(void);
	bool CheckData(void);
	bool CheckConnection(void);
private:
	void Loop(void);
};

#endif//__MODBUS_TCP__H__
