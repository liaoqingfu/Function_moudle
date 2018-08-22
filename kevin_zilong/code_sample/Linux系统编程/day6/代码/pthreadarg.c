#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <inttypes.h>

// 整型 32 64
// 指针 跟机器位长保持一致
void *thread_func(void *p)
{
	printf("%d\n", (int)(intptr_t)p);
}

int main()
{
	char buf[] = {"1 2 3 4 5 6"};
	int a = 100;
	pthread_t tid;
	pthread_create(&tid, NULL, thread_func, (void *)(intptr_t)a);
	//NULL (void *)0

	pthread_join(tid, NULL);
	return 0;
}
