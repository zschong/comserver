#ifndef __FILED_BUFFER_H__
#define __FILED_BUFFER_H__

class FieldBuffer
{
	unsigned char data[sizeof(int)];
public:
	unsigned char GetChar(void)
	{
		return data[0];
	}
	void SetChar(unsigned char value)
	{
		data[0] = value;
	}

	unsigned short GetShortAB(void)
	{
		return (data[0] << 8) | data[1];
	}
	void SetShortAB(unsigned short value)
	{
		data[1] = 0xff & value >> 8;
		data[0] = 0xff & value >> 0;
	}

	unsigned short GetShortBA(void)
	{
		return (data[1] << 8) | data[0];
	}
	void SetShortBA(unsigned short value)
	{
		data[1] = 0xff & value >> 8;
		data[0] = 0xff & value >> 0;
	}

	unsigned int GetIntABCD(void)
	{
		return  (data[0] << 24) 
		|		(data[1] << 16) 
		|		(data[2] <<  8) 
		|		(data[3] <<  0);
	}
	void SetIntABCD(unsigned int value)
	{
		data[0] = 0xff & (value >> 24);
		data[1] = 0xff & (value >> 16);
		data[2] = 0xff & (value >>  8);
		data[3] = 0xff & (value >>  0);
	}

	unsigned int GetIntDCBA(void)
	{
		return  (data[3] << 24) 
		|		(data[2] << 16) 
		|		(data[1] <<  8) 
		|		(data[0] <<  0);
	}
	void SetIntDCBA(unsigned int value)
	{
		data[3] = 0xff & (value >> 24);
		data[2] = 0xff & (value >> 16);
		data[1] = 0xff & (value >>  8);
		data[0] = 0xff & (value >>  0);
	}
};

#endif//__FILED_BUFFER_H__
