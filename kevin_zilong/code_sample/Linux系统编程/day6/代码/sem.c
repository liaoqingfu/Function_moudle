#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t sem;

void *thread_func(void *p)
{
	while(1)
	{
		sleep(1);
		sem_wait(&sem); //如果信号量的值是0，则阻塞，如果非0，则把信号量减1，继续执行
		printf("sem, tid=%ld\n", pthread_self());
	}
}

int main()
{
	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, thread_func, NULL);
	pthread_create(&tid2, NULL, thread_func, NULL);

	//初始化信号量
	sem_init(&sem,0,10);
	//发送信号量，实际上是把sem的值加1
	//for(int i=0; i<5;++i)
	//sem_post(&sem);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	return 0;
}
