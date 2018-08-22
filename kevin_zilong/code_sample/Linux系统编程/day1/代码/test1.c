
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main()
{
	int fd = open("./a.txt", O_WRONLY|O_APPEND);
	printf("fd1=%d\n", fd);
	for (int i = 0; i < 10; ++i)
	{
		sleep(1);
		write(fd, "b", 1);
	}
	close(fd);
	return 0;
}
