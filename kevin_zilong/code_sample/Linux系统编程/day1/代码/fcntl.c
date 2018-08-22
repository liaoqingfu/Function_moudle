#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>


int main()
{
	int fd = open("./a.txt", O_WRONLY);
	write(fd, "bbbb", 4);
	int flag = fcntl(fd, F_GETFL);
	flag |= O_APPEND;
	fcntl(fd, F_SETFL, flag);

	write(fd, "hello", 5);
	close(fd);
	return 0;
}
