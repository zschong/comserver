#ifndef __STORAGE_H__
#define __STORAGE_H__
#include <map>
#include <list>
#include "entity.h"

#ifndef u8
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
#endif//u8

class Storage
{
private:
	string prefix;
	string storedir;
	time_t laststore;
	time_t lastremove;
	time_t storetimeout;
	time_t removetimeout;

public:
	Storage(void);
public:
	bool StoreTimeout(void);
	bool RemoveTimeout(void);
	void SetName(const string& name);
	void Store(map<int,ValueNode>& m);
private:
	void Store(FILE *fp, map<u8,map<u8,map<u16,u16> > >& mmm);
	void Store(FILE *fp, u8 slave, u8 fcode, u16 offset, list<u16> &l);
	void Remove(void);
};

#endif//__STORAGE_H__
