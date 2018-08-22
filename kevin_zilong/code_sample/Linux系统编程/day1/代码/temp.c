#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main()
{
	char buf[] = "/tmp/hello-XXXXXX";
	char *p = mktemp(buf);
	printf("%s\n", p);
	int fd = open(p, O_WRONLY|O_CREAT, 0777);
	write(fd, "abcd", 4);
	close(fd);
	return 0;
}
