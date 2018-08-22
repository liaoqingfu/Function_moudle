#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void *thread_func(void *p)
{
	char *str = strdup("abc");
	return str;
}

int main()
{
	pthread_t tid;
	pthread_create(&tid, NULL, thread_func, NULL);

	char *str;
	pthread_join(tid, &str);
	printf("%s\n", str);
	free(str);
	return 0;
}
