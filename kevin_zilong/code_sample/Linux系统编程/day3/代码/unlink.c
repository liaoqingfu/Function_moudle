#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd = open("./b.c", O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return -1;
	}

	unlink("./b.c");
	char buf[4096] = {0};
	read(fd, buf, sizeof(buf));
	printf("%s\n", buf);
	close(fd);
	return 0;
}
