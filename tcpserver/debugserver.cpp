#include <stdio.h>
#include <stdlib.h>
#include "entity.h"
#include "showhex.h"
#include "modbus.h"
#include "comserver.h"
#include "debugserver.h"

void DebugServer::SetComServer(const ComServer& com)
{
	comserver = (ComServer*)&com;
}
bool DebugServer::Start(const string& ip, int port)
{
	server.ReuseAddress(1);
	if( server.Bind(ip, port) )
	{
		return server.Listen(1);
	}
	return false;
}

bool DebugServer::CheckConnection(void)
{
	if( worker.IsEstablished() )
	{
		return true;
	}
	if( comserver )
	{
		comserver->SetDebugMode(false);
		usleep(1000);
		comserver->SetDebugMode(false);
	}
	if( server.Accept(worker) )
	{
		worker.Block(0);
		worker.KeepAlive(10);
	}
	return worker.IsEstablished();
}
bool DebugServer::RecvData(void)
{
	int len = 0;
	char buf[256] = {0};

	len = worker.Recv(buf, sizeof(buf));
	if( len < 1 )
	{
		return false;
	}
	if( comserver )
	{
		ComReader& com = comserver->GetComReader();
		com.Send((unsigned char*)buf, len);
		for(int i = 0; i < 100; i++)
		{
			len = com.Recv(buf, sizeof(buf));
			if( len > 0 )
			{
				worker.Send(buf, len);
			}
			usleep(1000);
		}
	}
	return true;
}
void DebugServer::Loop(void)
{
	if( CheckConnection() )
	{
		if( comserver )
		{
			comserver->SetDebugMode(true);
			RecvData();
		}
	}
	else
	{
		if( comserver )
		{
			comserver->SetDebugMode(false);
		}
	}
	usleep(1000);
}
