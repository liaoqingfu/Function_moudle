#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd = open("./a.txt", O_RDWR);
	if (fd < 0)
	{
		printf("open error\n");
		return -1;
	}
	ftruncate(fd, 1024);
	//内存映射
	char *ptr = (void *)mmap(NULL, 1024, PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0);
	close(fd);

	strcpy(ptr, "abcdef");
	return 0;
}
