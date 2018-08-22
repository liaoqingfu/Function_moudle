#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *thread_func(void *p)
{
	while(1)
	{
		sleep(1);
		printf("hello\n");
		printf("tid=%ld\n", pthread_self());
		//pthread_exit(0); //关闭线程
		//exit(0);//关闭进程
	}

	return NULL;
}

int main()
{
	pthread_t tid1,tid2;
	pthread_create(&tid1, NULL, thread_func, NULL);
	pthread_create(&tid2, NULL, thread_func, NULL);
	//printf("%ld\n", tid);	
	//if (tid1 == tid2) 错误
	//pthread_equal(tid1,tid2);
	/*
	while (1)
	{
		sleep(1);
		printf("world\n");
	}
	*/
	//pthread_exit(0);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	return 0;
}
