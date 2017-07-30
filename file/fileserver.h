#ifndef __FILE_SERVER_H__
#define __FILE_SERVER_H__
#include "entity.h"
#include "thread.h"

class FileServer : public Thread 
{
private:
	Mutex cachelock;
	Mutex configlock;
	map<int,ValueNode> cache;
	map<xstring,xstring> config;
	bool cachechange;
	bool configchange;
	time_t laststore;

public:
	FileServer(void):cachechange(false),configchange(false),laststore(time(0){}
public:
	bool SetConfig(map<xstring,xstring>& m);
	bool SetValue(map<int,ValueNode>& m);
private:
	void Loop(void);
};


#endif//__FILE_SERVER_H__
