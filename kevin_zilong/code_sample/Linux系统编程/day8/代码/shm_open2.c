#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

int main()
{
	//创建一个内存文件
	int fd = shm_open("/sz005", O_RDWR|O_CREAT, 0777);
	if(fd < 0)
	{
		printf("open shm error\n");
		return -1;
	}
	ftruncate(fd, 1024);
	//内存映射
	char *ptr = (char *)mmap(NULL, 1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	//strcpy(ptr, "1232141");
	printf("%s\n", ptr);
	return 0;
}
