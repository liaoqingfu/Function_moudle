#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>

extern char **environ;
int main()
{
	int index = 0;
	struct tms buf;
	times(&buf);
	while(1)
	{
		if (environ[index] == NULL)
			break;
		printf("%s\n", environ[index++]);
	}
	struct tms buf1;
	times(&buf1);
	printf("%ld\n", buf1.tms_utime - buf.tms_utime);
	return 0;
}
