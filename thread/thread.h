#ifndef __THREAD_H__
#define __THREAD_H__
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "mutex.h"

class Thread
{
private:
	bool run;
	Mutex locker;
	pthread_t tid;
	void(*loop)(void *p);
public:
	Thread(void);
	~Thread(void);
public:
	bool Lock(void);
	bool TryLock(void);
	bool Unlock(void);
private:
	virtual void Loop(void){assert(0);}
private:
	static void* ThreadEntry(void *p);
};
#endif//__THREAD_H__
