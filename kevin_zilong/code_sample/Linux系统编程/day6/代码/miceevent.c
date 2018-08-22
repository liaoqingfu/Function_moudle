#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int micefd = open("/dev/input/mice", O_RDONLY);
	if (micefd < 0)
	{
		perror("mice");
		return -1;
	}
	int eventfd = open("/dev/input/event1", O_RDONLY);
	if (eventfd < 0)
	{
		perror("event");
		return -1;
	}

	char buf[4096] = {0};

	pid_t pid = fork();

	if (pid == 0)
	{

		while(1)
		{
			int ret = read(micefd, buf, sizeof(buf));
			if (ret > 0)
			{
				printf("mice\n");
			}
		}
	}
	else if(pid > 0)
	{
		while(1)
		{
			int ret = read(eventfd, buf, sizeof(buf));
			if (ret > 0)
			{
				printf("event\n");
			}
		}
	
	close(micefd);
	close(eventfd);
	
	}
	return 0;
}
