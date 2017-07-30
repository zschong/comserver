#include "thread.h"


Thread::Thread(void)
{
	run = true;
	assert( pthread_create(&tid, 0, ThreadEntry, this) == 0 );
}
Thread::~Thread(void)
{
	run = false;
	pthread_join(tid, NULL);
}

bool Thread::Lock(void)
{
	return locker.Lock();
}
bool Thread::TryLock(void)
{
	return locker.TryLock();
}
bool Thread::Unlock(void)
{
	return locker.Unlock();
}
void* Thread::ThreadEntry(void *p)
{
	Thread *t = (Thread*)p;

	while(t->run)
	{
		t->Loop();
	}
	return NULL;
}
