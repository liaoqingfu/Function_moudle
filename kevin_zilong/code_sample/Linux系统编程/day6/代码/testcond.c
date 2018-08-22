#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

void *thread_func(void *p)
{
	while(1)
	{
	pthread_mutex_lock(&mutex);
	pthread_cond_wait(&cond, &mutex);
	printf("thread\n");
	pthread_mutex_unlock(&mutex);
	}
}
int main()
{
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);

	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, thread_func, NULL);
	pthread_create(&tid2, NULL, thread_func, NULL);
	
	sleep(2);
	//for(int i=0; i<5; ++i)
	//pthread_cond_signal(&cond);
	//广播唤醒所有等待条件变量的线程
	pthread_cond_broadcast(&cond);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	return 0;
}
