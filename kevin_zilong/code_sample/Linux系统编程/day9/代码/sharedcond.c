#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

pthread_mutex_t *mutex;
pthread_cond_t *cond;

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

	pthread_condattr_t condattr;
	pthread_condattr_init(&condattr);
	pthread_condattr_setpshared(&condattr, PTHREAD_PROCESS_SHARED);

	cond = (pthread_cond_t *)(p+sizeof(pthread_mutex_t));
	pthread_cond_init(cond, &condattr);

	pid_t pid = fork();
	if (pid == 0)
	{
		getchar();
		pthread_cond_signal(cond);
	}
	else if(pid > 0)
	{
		sleep(1);
		pthread_mutex_lock(mutex);
		pthread_cond_wait(cond, mutex);
		pthread_mutex_unlock(mutex);
		printf("lock\n");
	}
	return 0;
}

