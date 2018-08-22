#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>

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

	//创建epoll集合
	int epollfd = epoll_create(512);
	//定义与描述符对应的结构体
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = micefd;

	//把鼠标描述符加入到集合中
	epoll_ctl(epollfd, EPOLL_CTL_ADD, micefd, &ev);

	//修改结构体与键盘匹配
	ev.data.fd = eventfd;
	//把键盘描述符加入到集合中
	epoll_ctl(epollfd, EPOLL_CTL_ADD, eventfd, &ev);

	struct epoll_event out_ev[2];

	while(1)
	{
		int ret = epoll_wait(epollfd, out_ev, 2, 2);
		if (ret < 0)
		{
			if (errno == EINTR)
			{
				continue;
			}
			return 0;
		}
		if (ret > 0)
		{
			for (int i=0; i<ret; ++i)
			{
				if (out_ev[i].data.fd == micefd)
				{
					char buf[4096];
					read(micefd, buf, sizeof(buf));
					printf("mice\n");
				}
				else if (out_ev[i].data.fd == eventfd)
				{
					char buf[4096];
					read(eventfd, buf, sizeof(buf));
					printf("event\n");
				}
			}
		}
	}
	

	close(micefd);
	close(eventfd);
	return 0;
}
