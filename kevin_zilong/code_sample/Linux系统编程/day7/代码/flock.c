#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>

int main()
{
	int fd = open("./a.txt", O_RDWR);
	if (fd < 0)
	{
		perror("open");
		return -1;
	}
	//给文件加锁
	flock(fd, LOCK_SH);
	sleep(10);
	flock(fd, LOCK_UN);
	return 0;
}
