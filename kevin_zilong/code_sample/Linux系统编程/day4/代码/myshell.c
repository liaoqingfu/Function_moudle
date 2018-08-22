
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

void handle_cmd(char *buf)
{
	//此数组用于给execvp传参
	char *arg[1024];
	char *p = strtok(buf, " ");
	int index = 0;
	while(1)
	{
		arg[index++] = p;
		if (p == NULL) //后判断NULL是为了把NULL加入到arg数组
			break;
		p = strtok(NULL, " ");
	}

	if (strcmp(arg[0],"cd") == 0)
	{
		chdir(arg[1]);
	}
	else
	{

	pid_t pid = fork();
	if (pid == 0)
	{
		execvp(arg[0],arg);
		printf("command error\n");
		exit(0);
	}

	wait(NULL);
	}
}

int main()
{
	//ls -a -l
	while(1)
	{
		char path[1024] = {0};
		//获取当前所在路径
		getcwd(path, sizeof(path));
		printf("%s$ ", path);
		//获取用户输入的命令
		char buf[1024];
		fgets(buf, sizeof(buf), stdin);
		//去掉\n
		buf[strlen(buf)-1] = '\0';
		//判断用户退出
		if(strncmp(buf,"exit",4) == 0)
		{
			break;
		}
		//处理用户空命令
		if (strlen(buf) == 0)
			continue;
		//处理用户命令
		handle_cmd(buf);
	}
	return 0;
}
