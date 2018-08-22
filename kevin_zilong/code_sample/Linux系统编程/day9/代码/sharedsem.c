#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>


sem_t *sem;

int main()
{
	int fd = shm_open("/sz005", O_RDWR|O_CREAT, 0777);
	ftruncate(fd, 1024);
	char *p = (char *)mmap(NULL, 1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	sem = (sem_t *)p;
	sem_init(sem, 1, 0);

	pid_t pid = fork();
	if (pid == 0)
	{
		sem_wait(sem);
		printf("sem\n");
	}
	else if (pid > 0)
	{
		getchar();
		sem_post(sem);
		wait(NULL);
	}
	return 0;
}

