#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

pthread_mutex_t *mutex;

int main()
{
	int fd = shm_open("/sz005", O_RDWR|O_CREAT, 0777);
	ftruncate(fd, 1024);
	char *p = (char *)mmap(NULL, 1024, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
	close(fd);

	mutex = (pthread_mutex_t *)p;

	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);

	pthread_mutex_init(mutex, &attr);
	pid_t pid = fork();
	if (pid == 0)
	{
		pthread_mutex_lock(mutex);
		getchar();
		pthread_mutex_unlock(mutex);
	}
	else if(pid > 0)
	{
		sleep(1);
		pthread_mutex_lock(mutex);
		printf("lock\n");
		pthread_mutex_unlock(mutex);
	}
	return 0;
}

