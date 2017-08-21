#ifndef __DEBUG_SERVER_H__
#define __DEBUG_SERVER_H__
#include <map>
#include <vector>
#include "thread.h"
#include "tcpserver.h"
using namespace std;

class DebugServer : public Thread
{
	TcpServer server;
	TcpServer worker;
	class ComServer *comserver;

public:
	DebugServer(void):comserver(0){}

public:
	void SetComServer(const ComServer& com);
	bool Start(const string& ip, int port);
	bool CheckConnection(void);
	bool RecvData(void);
	bool GetDebug(void);
private:
	void Loop(void);
};



#endif//__DEBUG_SERVER_H__
