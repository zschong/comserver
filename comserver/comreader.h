#ifndef __COM_READER_H__
#define __COM_READER_H__
#include <vector>
#include "comport.h"
using namespace std;

class ComReader : public ComPort
{
private:
	vector<char> queue;
	unsigned char data[256];

public:
	int Send(unsigned char *data, int len);
	unsigned char* ReadData(void);
	unsigned char* Data(void);
	unsigned int Length(void);
	int Cleanup(void);
};

#endif//__COM_READER_H__
