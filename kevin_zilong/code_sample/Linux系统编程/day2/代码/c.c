#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main()
{
	/*
	int fd = open("./a.txt", O_WRONLY);
	if(fd < 0)
	{
		perror("open");
		return 0;
	}
    */	
	unlink("./a.txt");
	//getchar();
	//close(fd);
	return 0;
}
