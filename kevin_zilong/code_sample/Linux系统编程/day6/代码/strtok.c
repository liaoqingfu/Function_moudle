#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

void *thread_func(void *p)
{
	char buf[] = {"1 2 3 4 5 6 7 8 9 0"};
	char *save;
	char *ptr = strtok_r(buf, " ", &save);
	//strtok  static char* p;
	while(ptr)
	{
		usleep(100);
		printf("%s\n", ptr);
		ptr = strtok_r(NULL, " ", &save);
	}
	return NULL;
}

int main()
{
	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, thread_func, NULL);
	pthread_create(&tid2, NULL, thread_func, NULL);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	return 0;
}
