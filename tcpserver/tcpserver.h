#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__
#include <string>

class TcpServer
{
private:
	int fd;
	int error;
	bool listening;

public:
	TcpServer(void);
	~TcpServer(void);

public:
	bool Open(void);
	bool Close(void);
	bool Block(bool b);
	bool KeepAlive(int idle);
	bool ReuseAddress(bool b);
	bool Bind(const std::string&, int port);
	bool Listen(int backlog);
	bool Accept(class TcpServer&);
	bool IsOpened(void);
	bool IsListening(void);
	bool IsEstablished(void);
	int  Recv(char *buf, int len);
	int  Send(const char* buf, int len);
	int  Send(const unsigned char* buf, int len){Send((const char*)buf, len);}
};

#endif//__TCP_SERVER_H__
