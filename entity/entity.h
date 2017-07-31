#ifndef __ENTITY_H__
#define __ENTITY_H__
#include "xstring.h"

class ValueNode
{
public:
	unsigned char slave;
	unsigned short offset;
	unsigned short value;
public:
	ValueNode(void):slave(0),offset(0),value(0){}
};

class ConfigNode 
{
public:
	unsigned char slave;
	unsigned char fcode;
	unsigned short offset;
	unsigned short count;
	unsigned short timeout;
public:
	ConfigNode(void):slave(0),fcode(0),offset(0),count(0),timeout(100){}
};

class ComNode
{
public:
	xstring name;
	int baud;
	int parity;
	int bsize;
	int stop;
public:
	ComNode(void):baud(9600),parity(0),bsize(8),stop(1){}
};

class FieldNode
{
public:
	xstring field1;
	xstring field2;
	xstring field3;
	xstring field4;
	xstring field5;
	xstring field6;
	xstring field7;
	xstring field8;
	xstring field9;
};


#endif//__ENTITY_H__
