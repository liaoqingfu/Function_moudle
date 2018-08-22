#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

void handle(int a)
{
	wait(NULL);
	exit(0);
}

//./a.out 1
//./a.out 2
int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("args error\n");
		return -1;
	}
	
	//输入用户名字
	char name[1024] = {0};
	printf("please input your name: ");
	fgets(name, sizeof(name), stdin);
	name[strlen(name)-1] = ':';

	int fd1, fd2;
	if (argv[1][0] == '1')
	{
		fd1 = open("./fifo1", O_RDONLY);
		fd2 = open("./fifo2", O_WRONLY);
	}
	else if(argv[1][0] == '2')
	{
		fd2 = open("./fifo1", O_WRONLY);
		fd1 = open("./fifo2", O_RDONLY);
	}

	pid_t pid = fork();
	if (pid == 0)
	{
		close(fd2);
		while(1)
		{
			char readbuf[1024];
			if(read(fd1, readbuf, sizeof(readbuf)) == 0)
				break;
			printf("%s", readbuf);
		}
		exit(0);
	}
	signal(SIGCHLD, handle);

	close(fd1);
	while(1)
	{
		char buf[1024] = {0};
		fgets(buf, sizeof(buf), stdin);

		write(fd2, name, strlen(name));
		write(fd2, buf, strlen(buf)+1);

	}
	
	close(fd1);
	close(fd2);
	return 0;
}
	
	
