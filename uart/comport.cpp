#include <sys/ioctl.h>
#include <fcntl.h>
#include "comport.h"

bool ComPort::IsOpen(void)
{
	return (-1 != fd);
}
bool ComPort::Open(const string& path)
{
	if( -1 == fd )
	{
		fd = uart_open(path.data());
	}
	return (-1 != fd);
}
bool ComPort::Set(int baud, char parity, int bsize, int stop)
{
	switch(parity)
	{
	case 0:
		parity = 'N';
		break;
	case 1:
		parity = 'O';
		break;
	case 2:
		parity = 'E';
		break;
	}
	return (uart_config(fd, baud, parity, bsize, stop) == 0);
}
int ComPort::Recv(char *buf, int len)
{
	return uart_read(fd, buf, len);
}
int  ComPort::Send(unsigned char *data, int len)
{
	return uart_write(fd, (const char*)data, len);
}
bool ComPort::Block(bool b)
{
	if( b )
	{
		return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) & ~O_NONBLOCK);
	}
	else
	{
		return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
	}
}
bool ComPort::Close(void)
{
	uart_close(fd);
	fd = -1;
}
bool ComPort::SetMode(int mode)
{
	switch(mode)
	{
	case Mode_422:
	case Mode_485:
	case Mode_232:
		break;
	default:
		mode = Mode_485;
	}
	return ioctl(fd, TIOCSERMODE, Mode_485) == 0;
}
