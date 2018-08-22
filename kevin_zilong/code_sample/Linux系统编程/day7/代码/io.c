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
		printf("open mice error\n");
		return -1;
	}
	int eventfd = open("/dev/input/event1", O_RDONLY);
	if (eventfd < 0)
	{
		close(micefd);
		printf("open event error\n");
		return -1;
	}
	int flags = fcntl(micefd, F_GETFL);
	flags = flags | O_NONBLOCK;
	fcntl(micefd, F_SETFL, flags);

	flags = fcntl(eventfd, F_GETFL);
	flags = flags | O_NONBLOCK;
	fcntl(eventfd, F_SETFL, flags);
	
	char buf[4096];
	while(1)
	{
		int ret = read(micefd, buf, sizeof(buf));
		if (ret > 0)
		{
			printf("mice\n");
		}
		ret = read(eventfd, buf, sizeof(buf));
		if (ret > 0)
		{
			printf("event\n");
		}
	}
	close(micefd);
	close(eventfd);
	return 0;
}
