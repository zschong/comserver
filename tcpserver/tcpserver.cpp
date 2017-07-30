#include "tcpserver.h"
#include "tcp-socket.h"

TcpServer::TcpServer(void)
{
	fd = -1;
	error = 0;
	listening = false;
	Open();
}
TcpServer::~TcpServer(void)
{
	Close();
}

bool TcpServer::Open(void)
{
	if( -1 == fd )
	{
		fd = tcp_open();
	}
	return (-1 != fd);
}
bool TcpServer::Close(void)
{
	if( -1 != fd )
	{
		tcp_close(fd);
	}
	fd = -1;
	error = 0;
	listening = false;
}
bool TcpServer::Block(bool b)
{
	if( b )
	{
		return tcp_block(fd) == 0;
	}
	else
	{
		return tcp_nonblock(fd) == 0;
	}
}
bool TcpServer::KeepAlive(int idle)
{
	return tcp_keepalive(fd, 1, idle, 5, 3);
}
bool TcpServer::ReuseAddress(bool b)
{
	return tcp_reuse_address(fd) == 0;
}
bool TcpServer::Bind(const std::string& ip, int port)
{
	if( ip.empty() )
	{
		return false;
	}
	return tcp_bind(fd, inet_addr(ip.data()), port) == 0;
}
bool TcpServer::Listen(int backlog)
{
	if( tcp_listen(fd, backlog) == 0 )
	{
		return listening = true;
	}
	return listening = false;
}
bool TcpServer::Accept(class TcpServer& worker)
{
	worker.Close();
	worker.fd = tcp_accept(fd);
	return worker.IsOpened();
}
int  TcpServer::Recv(char *buf, int len)
{
	return (error = tcp_recv(fd, buf, len));
}
int  TcpServer::Send(const char* buf, int len)
{
	return (error = tcp_send(fd, (char*)buf, len));
}
bool TcpServer::IsOpened(void)
{
	return (-1 != fd);
}
bool TcpServer::IsListening(void)
{
	return listening;
}
bool TcpServer::IsEstablished(void)
{
	switch(error)
	{
		case EBADF:
		case ENOTCONN:
			return false;
	}
	return tcp_state(fd) == TCP_ESTABLISHED;
}
