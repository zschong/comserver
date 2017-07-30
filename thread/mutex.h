#ifndef __MUTEX_H__
#define __MUTEX_H__
#include <pthread.h>

class Mutex
{
private:
	pthread_mutex_t lock;
	pthread_mutexattr_t attr;
public:
	Mutex(void)
	{
		pthread_mutexattr_init(&attr);
		pthread_mutex_init(&lock, &attr);
	}
	~Mutex(void)
	{
		pthread_mutex_destroy(&lock);
		pthread_mutexattr_destroy(&attr);
	}
public:
	bool Lock(void)
	{
		return pthread_mutex_lock(&lock) == 0;
	}
	bool TryLock(void)
	{
		return pthread_mutex_trylock(&lock) == 0;
	}
	bool Unlock(void)
	{
		return pthread_mutex_unlock(&lock) == 0;
	}
};

#endif//__MUTEX_H__
