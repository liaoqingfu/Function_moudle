#include <stdio.h>
#include <pthread.h>
#include <errno.h>

void *thread_func(void *p)
{

	return NULL;
}

int main()
{
	//第二种分离方式
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_t tid1, tid2;
	pthread_create(&tid1, &attr, thread_func, NULL);
	pthread_create(&tid2, &attr, thread_func, NULL);
	
	//第一种分离方式
	//pthread_detach(tid1);
	//pthread_detach(tid2);

	int ret = pthread_join(tid1, NULL);
	printf("errno = %d\n EINVAL=%d\n", ret, EINVAL);
	return 0;
}
