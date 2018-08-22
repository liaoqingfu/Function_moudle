#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

thread_local int result = 0;
pthread_t tid1, tid2;

void *thread_func(void *p)
{
	if(pthread_equal(tid1, pthread_self()))
	{
		result=100;
	}
	else
	{
		sleep(1);
		printf("%d\n", result);
	}
	return NULL;
}

int main()
{
	pthread_create(&tid1, NULL, thread_func, NULL);
	pthread_create(&tid2, NULL, thread_func, NULL);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	return 0;
}
