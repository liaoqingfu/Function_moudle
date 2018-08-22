
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main()
{
	int fd = open("./a.txt", O_RDONLY);
	if (fd < 0)
	{
		perror("open");
	}
	char buf[5];
	read(fd, buf, 5);
	printf("fd=%s\n", buf); //hello
	//int fd1 = dup(fd);
	int fd1 = open("./a.txt", O_RDONLY);
	read(fd1, buf, 5);
	printf("fd1=%s\n", buf); //hello
	read(fd, buf, 5);
	printf("fd=%s\n", buf); //world
	return 0;
}
