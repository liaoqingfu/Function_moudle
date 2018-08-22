#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
	int fdr = open("./a.txt", O_RDONLY);
	if (fdr < 0)
	{
		perror("open src");
		return 0;
	}
	int fdw = open("./b.txt", O_WRONLY | O_CREAT, 0777);
	if (fdw < 0)
	{
		perror("open dst");
		return 0;
	}

	/*
	int filelen = lseek(fdr, 0, SEEK_END);
	lseek(fdr, 0, SEEK_SET);

	char *buffer = malloc(filelen);
	read(fdr, buffer, filelen);
	write(fdw, buffer, filelen);
	

	char buffer;
	while(1)
	{
	int ret = read(fdr, &buffer, 1);
	if (ret <= 0)
	{
		break;
	}
	write(fdw, &buffer, 1);
	}
	*/

	close(fdr);
	close(fdw);
	//free(buffer);
	return 0;
}
