#include <stdio.h>
#include <stdlib.h>
#include "entity.h"
#include "showhex.h"
#include "modbus.h"
#include "comserver.h"
#include "jsonserver.h"

void JsonServer::SetComServer(const ComServer& com)
{
	comserver = (ComServer*)&com;
}
bool JsonServer::Start(const string& ip, int port)
{
	server.ReuseAddress(1);
	if( server.Bind(ip, port) )
	{
		return server.Listen(1);
	}
	return false;
}

bool JsonServer::CheckConnection(void)
{
	if( worker.IsEstablished() )
	{
		return true;
	}
	if( server.Accept(worker) )
	{
		worker.Block(0);
		worker.KeepAlive(10);
	}
	return worker.IsEstablished();
}
bool JsonServer::RecvData(void)
{
	int len = 0;
	char buf[32] = {0};

	len = worker.Recv(buf, sizeof(buf));
	if( len < 1 )
	{
		return false;
	}
	return GetJson();
}
bool JsonServer::GetJson(void)
{
	if( comserver )
	{
		xstring json;
		if(	comserver->GetJson(json) )
		{
			worker.Send(json.data(), json.length());
		}
	}
	return true;
}

void JsonServer::Loop(void)
{
	if( CheckConnection() )
	{
		RecvData();
	}
	usleep(1000);
}
