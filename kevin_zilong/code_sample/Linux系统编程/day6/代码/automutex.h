#include <pthread.h>

class automutex
{
	public:
		automutex(pthread_mutex_t &mutex);
		~automutex();
	public:
		pthread_mutex_t &mutex;
};
