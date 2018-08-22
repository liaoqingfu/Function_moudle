#include <stdio.h>
#include <pthread.h>

pthread_rwlock_t mutex;

void *thread_func(void *p)
{
	int fd = open("...", O_RDWR);
	char buf[4096];
	while(1)
	{
		pthread_rwlock_rdlock(&mutex);
		read(fd, buf, sizeof(buf));
		pthread_rwlock_unlock(&mutex);

		pthread_rwlock_wrlock(&mutex);
		write(fd, buf, sizeof(buf));
		pthread_rwlock_unlock(&mutex);

	}
	return NULL;
}



int main()
{
	//初始化读写锁
	pthread_rwlock_init(&mutex, NULL);
	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, thread_func, NULL);
	pthread_create(&tid2, NULL, thread_func, NULL);
	
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	return 0;
}
