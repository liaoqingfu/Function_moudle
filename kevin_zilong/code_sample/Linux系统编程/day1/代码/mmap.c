#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

int main()
{
	int fd = open("./a.txt", O_RDWR);
	ftruncate(fd, 1024);
	void *p = mmap(NULL, 1024, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	close(fd);
	memcpy(p,"abcd", 4);
	munmap(p,1024);
	return 0;
}

