#include "fileserver.h"


bool FileServer::SetConfig(map<xstring,xstring>& m)
{
	if( configlock.Lock() )
	{
		config.clear();
		config = m;
		configchange = true;
		configlock.Unlock();
		return true;
	}
	return false;
}
bool FileServer::SetValue(map<int,ValueNode>& m)
{
	if( cachelock.Lock() )
	{
		cache.clear();
		cache = m;
		cachechange = true;
		cachelock.Unlock();
		return true;
	}
	return false;
}
void FileServer::Loop(void)
{
		
}

