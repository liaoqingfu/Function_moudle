#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
void you()
{
	int fd = open("./a.txt", O_WRONLY);
	test(fd);
}

void test(int fd)
{
	int flag = fcntl(fd, F_GETFL);
	flag |= O_APPEND;
	flag = flag|O_APPEND;
	fcntl(fd, F_SETFL, flag);
}
int main()
{
	printf("stdout  abc\n");
	dup2(1,3);
	close(1);
	int fd = open("./a.txt", O_WRONLY);
	//printf("fd=%d\n", fd);
    char buf[1024] = {0};
	read(1,buf,5);
	write(3, buf, 5);
	close(1);
	dup2(3,1);
	close(3);
	printf("stdout aaa\n");
	return 0;
}
