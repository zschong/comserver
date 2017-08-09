#ifndef __HOSWHEX_H__
#define __HOSWHEX_H__
#include <stdio.h>

#define showhex(m,n)  \
	do{\
		if((n)>0)printf("%s:", __func__);\
		Show::Showhex((unsigned char*)m, n);\
	}while(0)

class Show
{
public:
	static void Showhex(unsigned char* buf, int len)
	{
		for(int i = 0; i < len && buf; i++)
		{
			printf("%02X ", buf[i]);
			if( (i+1)%16 == 0 )
			{
				printf("\n");
			}
		}
		if(len>0)printf("\n");
	}
};

#endif//__HOSWHEX_H__
