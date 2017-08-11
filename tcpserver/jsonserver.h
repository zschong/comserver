#ifndef __JSON_SERVER_H__
#define __JSON_SERVER_H__
#include <map>
#include <vector>
#include "thread.h"
#include "tcpserver.h"
using namespace std;

class JsonServer : public Thread
{
	TcpServer server;
	TcpServer worker;
	class ComServer *comserver;

public:
	JsonServer(void):comserver(0){}

public:
	void SetComServer(const ComServer& com);
	bool Start(const string& ip, int port);
	bool CheckConnection(void);
	bool RecvData(void);
	bool GetJson(void);
private:
	void Loop(void);
};



#endif//__JSON_SERVER_H__
