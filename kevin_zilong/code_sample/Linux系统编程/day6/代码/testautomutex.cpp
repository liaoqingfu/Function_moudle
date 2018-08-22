#include "automutex.h"
#include <pthread.h>

int result = 0;
pthread_mutex_t mutex;

void *thread_func(void *)
{
	for(int i=0;i<100000;++i)
	{
		automutex am(mutex);
		result++;
	}
	return NULL;
}

int main()
{
	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, thread_func, NULL);
	pthread_create(&tid2, NULL, thread_func, NULL);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	return 0;
}

