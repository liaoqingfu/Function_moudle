#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main1()
{
	int fd[2];
	pipe(fd); //创建匿名管道，fd[0]代表read,fd[1]代表写

	char buf[] = "abcderf";
	write(fd[1],buf, sizeof(buf));
	char readbuf[1024] = {0};
	read(fd[0], readbuf, sizeof(readbuf));
	printf("%s\n", readbuf);
	return 0;
}

int main2()
{
	int fd[2];
	pipe(fd);
	pid_t pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		char buf[] = "child process";
		write(fd[1], buf, sizeof(buf));
		exit(0);
	}
	else if (pid > 0)
	{
		close(fd[1]);
		sleep(1);
		char buf[1024] = {0};
		read(fd[0], buf, sizeof(buf));
		printf("%s\n", buf);
	}
	return 0;
}


int main3()
{
	int fd[2];
	pipe(fd);
	for(int i=0;i<100000;++i)
	{
		char buf;
		write(fd[1],&buf,1);
		printf("%d\n", i);
	}
	return 0;
}


int main()
{
	int fd[2];
	pipe(fd);
	pid_t pid = fork();
	if (pid == 0)
	{
		sleep(1);
		close(fd[1]); //子进程把写端关闭
		dup2(fd[0], 0); //把子进程的标准输入重定向到管道上
		close(fd[0]); //子进程把读端关闭
		char buf[1024] = {0};
		fgets(buf, sizeof(buf), stdin);
		printf("%s\n", buf);
		exit(0);
	}
	else if (pid > 0)
	{
		close(fd[0]); //把读的端口关闭
		dup2(fd[1], 1); //把主进程的标准输出重定向到管道上
		close(fd[1]); //把写的端口关闭
		printf("%s\n","main process write");
		fflush(stdout);
		wait(NULL);
	}
	return 0;
}
