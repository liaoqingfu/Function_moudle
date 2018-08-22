#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int result = 0;
int result1 = 0;
//粗粒度的锁
//定义全局锁变量
pthread_mutex_t mutex;
pthread_mutex_t mutex1;

void *thread_func(void *p)
{
	for(int i=0; i<100000; ++i)
	{
		//加锁
		pthread_mutex_lock(&mutex);
		//重复加锁
		pthread_mutex_lock(&mutex);
		result++;
		pthread_mutex_unlock(&mutex);
		pthread_mutex_unlock(&mutex);
		
	}
	return NULL;
}

void *thread_func2(void *p)
{
	for(int i=0; i<100000; ++i)
	{
		//加锁
		pthread_mutex_lock(&mutex1);
		result1++;
		pthread_mutex_unlock(&mutex1);
	}
	return NULL;
}

int main()
{
	//定义锁的高级属性
	pthread_mutexattr_t attr;
	//初始化锁的高级属性
	pthread_mutexattr_init(&attr);
	//设置高级属性为循环锁
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	//初始化锁变量
	pthread_mutex_init(&mutex, &attr);
	pthread_mutex_init(&mutex1, NULL);
	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, thread_func, NULL);
	pthread_create(&tid2, NULL, thread_func, NULL);
	//开启另外两个线程
	pthread_t tid3, tid4;
	pthread_create(&tid3, NULL, thread_func2, NULL);
	pthread_create(&tid4, NULL, thread_func2, NULL);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_join(tid3, NULL);
	pthread_join(tid4, NULL);

	//销毁
	pthread_mutex_destroy(&mutex);
	printf("%d\n", result);
	return 0;
}
