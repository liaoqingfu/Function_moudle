#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

pthread_key_t key;
pthread_t tid1, tid2;

void foo()
{
	//获取key绑定的数据
	char *p = (char *)pthread_getspecific(key);
	printf("%s\n", p);
}

void *thread_func(void *p)
{
	if (pthread_equal(tid1, pthread_self()))
	{
	//为key绑定数据
	pthread_setspecific(key, strdup("abc"));
	sleep(1);
	}
	else
	{
		pthread_setspecific(key, strdup("def"));
		sleep(1);
	}
	foo();
	return NULL;
}

void free_key(void *p)
{
	free(p);
}

int main()
{
	//为key指定线程结束时的释放函数
	pthread_key_create(&key, free_key);
	pthread_create(&tid1, NULL, thread_func, NULL);
	pthread_create(&tid2, NULL, thread_func, NULL);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	pthread_key_delete(key);
	return 0;
}
