#include <stdio.h>
#include <stdlib.h>
#include "entity.h"
#include "showhex.h"
#include "modbus.h"
#include "comserver.h"
#include "modbustcp.h"

void ModbusTcpServer::SetSlave(char slave)
{
	cmd[slave] = 3;
}
void ModbusTcpServer::SetComServer(class ComServer& com )
{
	comserver = &com;
}
bool ModbusTcpServer::Start(const string& ip, int port)
{
	server.ReuseAddress(1);
	if( server.Bind(ip, port) )
	{
		return server.Listen(1);
	}
	return false;
}

bool ModbusTcpServer::CheckConnection(void)
{
	if( worker.IsEstablished() )
	{
		return true;
	}
	if( server.Accept(worker) )
	{
		data.clear();
		worker.Block(0);
		worker.KeepAlive(10);
	}
	return worker.IsEstablished();
}
bool ModbusTcpServer::RecvData(void)
{
	int len = 0;
	char buf[1024] = {0};

	len = worker.Recv(buf, sizeof(buf));
	if( len < 1 )
	{
		return false;
	}
	for(int i = 0; i < len; i++)
	{
		data.push_back(buf[i]);
	}
	return data.size();
}
bool ModbusTcpServer::FindHeader(void)
{
	if( data.size() >= 8 )
	{
		Requestx03 request;
		for(int i = 0; i < 8; i++)
		{
			request.data[i] = data[i];
		}
		printf("data.size(%d)\n", data.size());
		showhex(request.data, 8);
		if( request.GetFcode() != 0x03 )
		{
			data.erase(data.begin(), data.begin()+1);
			return false;
		}
		return true;
	}
	return false;
}
bool ModbusTcpServer::CheckData(void)
{
	if( comserver )
	{
		char buf[8] = {0};

		for(int i = 0; i < sizeof(buf); i++)
		{
			buf[i] = data[i];
		}
		data.erase(data.begin(), data.begin()+sizeof(buf));
		Requestx03 *p = (Requestx03*)buf;
		ConfigNode node;
		node.slave = p->GetSlave();
		node.fcode = p->GetFcode();
		node.offset = p->GetOffset();
		node.count = p->GetCount();
		printf("request%02X:s=%02X f=%02X o=%04X c=%04X\n",
				node.fcode,
				node.slave,
				node.fcode,
				node.offset,
				node.count
				);
		map<int,unsigned short> values;
		if(	comserver->GetValue(node, values) )
		{
			Responsex03 response;
			response.SetSlave(node.slave);
			response.SetFcode(node.fcode);
			response.SetBcount(values.size()*2);
			unsigned char len = values.size();
			for(unsigned char i = 0; i < len; i++)
			{
				response.SetValue(i, values[i]); 
			}
			response.SetCRC(response.CalcCRC());
			worker.Send((const char*)response.data, response.CRCLen()+2);
		}
	}
	return true;
}

void ModbusTcpServer::Loop(void)
{
	if( CheckConnection() )
	{
		RecvData();
	}
	if( FindHeader() )
	{
		CheckData();
	}
	usleep(1000);
}
