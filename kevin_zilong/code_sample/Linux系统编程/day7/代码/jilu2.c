#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd = open("./a.txt", O_RDWR);
	if (fd < 0)
	{
		perror("open");
		return -1;
	}
	//设置记录锁的结构体
	struct flock l;
	l.l_type = F_WRLCK; //写锁
	l.l_whence = SEEK_SET;
	l.l_start = 513;
	l.l_len = 1024;

	//给文件加锁
	fcntl(fd, F_SETLKW, &l);
	printf("lock\n");
	sleep(10);
	l.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &l);
	close(fd);
	return 0;
}
