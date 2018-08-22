#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

int main()
{
	signal(SIGPIPE, SIG_IGN); //忽略信号
	int fd = open("./fifo", O_WRONLY);
	if (fd < 0)
	{
		printf("open fifo error\n");
		return -1;
	}
	printf("open fifo ok\n");
	while(1)
	{
		sleep(1);
	char buf[] = "write by fifo1.c";
	int ret = write(fd, buf, sizeof(buf));
	if (ret == -1)
	{
		printf("errno = %d, EPIPE=%d\n", errno,EPIPE);
		break;
	}
	}
	close(fd);
	return 0;
}
