#ifndef __COMPORT_H__
#define __COMPORT_H__ 
#include "uart.h"
#include "xstring.h"

#ifdef cplusplus
extern "C"
#endif//cplusplus

class ComPort
{
private:
	int fd;
public:
	ComPort(void):fd(-1){}
public:
	bool Open(const string& path);
	bool Set(int baud, char parity, int bsize, int stop);
	virtual int  Recv(char *buf, int len);
	virtual int  Send(unsigned char *data, int len);
	bool Block(bool b);
	bool Close(void);
	bool IsOpen(void);
	bool SetMode(int);
};

#endif//__COMPORT_H__
