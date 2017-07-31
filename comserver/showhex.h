#ifndef __HOSWHEX_H__
#define __HOSWHEX_H__
#include <stdio.h>

#define showhex(b,l) \
	do{\
		if( l > 0 )\
		{\
			for(int _i = 0; _i < l; _i++)\
			{\
				printf("%02X ", b[_i]);\
				if( (_i+1)%16 == 0 )\
				{\
					printf("\n");\
				}\
			}\
			printf("\n");\
		}\
	}while(0)

#endif//__HOSWHEX_H__
