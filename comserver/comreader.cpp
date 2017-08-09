#include "showhex.h"
#include "comreader.h"

unsigned char* ComReader::ReadData(void)
{
	char buf[32] = {0};
	int len = Recv(buf, sizeof(buf));
	for(int i = 0; i < len; i++)
	{
		queue.push_back(buf[i]);
	}
	return Data();
}
int ComReader::Send(unsigned char *data, int len)
{
	//showhex(data, len);
	return ComPort::Send(data, len);
}
unsigned char* ComReader::Data(void)
{
	int len = queue.size() % sizeof(data);
	for(int i = 0; i < len; i++)
	{
		data[i] = queue[i];
	}
	return data;
}
unsigned int ComReader::Length(void)
{
	return queue.size();
}
int ComReader::Cleanup(void)
{
	queue.clear();
	bzero(data, sizeof(data));
}
