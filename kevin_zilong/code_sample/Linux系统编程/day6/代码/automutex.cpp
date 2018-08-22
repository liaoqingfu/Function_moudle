#include "automutex.h"

automutex::automutex(pthread_mutex_t &mutex):mutex(mutex)
{
	pthread_mutex_lock(&mutex);
}

automutex::~automutex()
{
	pthread_mutex_unlock(&mutex);
}
