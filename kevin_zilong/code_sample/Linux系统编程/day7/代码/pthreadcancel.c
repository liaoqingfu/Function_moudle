#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *thread_func(void *p)
{
	while(1)
	{
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
		printf("1\n");
		usleep(1000);
		printf("2\n");
		usleep(1000);
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
		printf("3\n");
		pthread_testcancel();
		
	}
	return NULL;
}

int main()
{
	pthread_t tid;
	pthread_create(&tid, NULL, thread_func, NULL);
	sleep(5);
	pthread_cancel(tid); //结束子线程
	pthread_join(tid, NULL);
	return 0;
}
