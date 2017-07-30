#include <stdio.h>
#include <unistd.h>
#include "tcpserver.h"
#include "comserver.h"
#include "modbustcp.h"

int main(void)
{
	ComServer comserver;
	ModbusTcpServer server;

	server.SetSlave(2);
	server.SetComServer(comserver);
	server.Start("0.0.0.0", 8888);

	while(1)
	{
		getchar();
	}
}
