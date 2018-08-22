#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/select.h>


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


	//返回值
	//小于0，失败：被中断打断，错误
	//等于0，超时，没监听到数据
	//大于0, 监听到事件，并且返回产生事件的描述符数量
	while(1)
	{
		//定义fd_set集合
		fd_set set;
		//初始化集合
		FD_ZERO(&set);
		//把鼠标和键盘描述符加入到集合中
		FD_SET(micefd, &set);
		FD_SET(eventfd, &set);

		//第一个参数，打开的文件描述符中最大的值加1
		//第二个，监听读取的集合
		//第三个，监听写入的集合,NULL
		//第四个，要排除的集合,NULL
		//第五个，超时时间
		int maxfd = micefd > eventfd ? micefd : eventfd;
		struct timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		//set既是传入参数，又是传出参数，如果有事件产生，select会把没有事件产生的描述从集合中删除掉
		int ret = select(maxfd + 1, &set, NULL, NULL, &tv);
		//为什么第一个参数是打开的最大文件描述符加1
		//fd_set, unsigned int data[32]; 32*32=1024
		if (ret < 0)
		{
			if (errno == EINTR) //被中断打断
			{
				continue;
			}
			return 0;
		}
		if (ret > 0)//有事件产生
		{
			if (FD_ISSET(micefd, &set))
			{
				char buf[4096];
				read(micefd, buf, sizeof(buf));
				printf("mice\n");
			}
			if (FD_ISSET(eventfd, &set))
			{
				char buf[4096];
				read(eventfd, buf, sizeof(buf));
				printf("event\n");
			}
		}
	}	
	return 0;
}


