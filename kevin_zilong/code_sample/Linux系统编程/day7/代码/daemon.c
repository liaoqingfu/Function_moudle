#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	pid_t pid = fork();
	if (pid == 0)
	{
		//子进程中执行
		//把掩码设置为0
		umask(0);
		//创建新会话，与终端脱离
		setsid();
		//重新设置工作目录
		chdir("/");
		//把所有的文件描述符关闭
		for (int i=0; i<1024; ++i)
		{
			close(i);
		}
		//重定向标准输出/标准输入/标准错误到/dev/null
		open("/dev/null", O_RDONLY); //系统分配0号描述符，实际就是标准输入绑定到null上
		open("./log.txt", O_WRONLY|O_CREAT, 0777);
		open("/dev/null", O_WRONLY);

		//确保守护进程是单例的
		int fd = open("./daemon.txt", O_WRONLY|O_CREAT|O_EXCL, 0777);
		if (fd < 0)
			exit(0);
		/*....*/
		while(1)
			sleep(1);
		return 0;


	}
	else if (pid > 0)
	{
		//主进程执行
		while(1)
			sleep(1);
	}
	return 0;
}
