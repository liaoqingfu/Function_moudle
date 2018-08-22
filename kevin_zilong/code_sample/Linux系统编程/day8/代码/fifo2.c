#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd = open("./fifo", O_RDONLY);
	if (fd < 0)
	{
		printf("open fifo error\n");
		return -1;
	}
	printf("open fifo ok\n");
	sleep(1);
	while(1)
	{
	char buf[1024];
	read(fd, buf ,sizeof(buf));
	printf("%s\n", buf);
	}

	close(fd);
	return 0;
}
