#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void handle_cmd(char *buf);

//处理简单命令 ls -a -l
void handle_simple_cmd(char *buf, int *fd)
{
	//定义参数数组，用于执行exec函数
	char *args[1024];
	char *p = strtok(buf, " ");
	int index = 0;
	while(1)
	{
		args[index++] = p;
		if (p == NULL)
			break;
		p = strtok(NULL, " ");
	}

	//判断命令是否是cd
	if (strcmp(args[0], "cd") == 0)
	{
		chdir(args[1]);
		return;
	}
	
	//创建子进程执行exec
	pid_t pid = fork();
	if (pid == 0)
	{
		if (fd)
		{
		close(fd[0]); //把管道的读端关闭
		//把子进程的标准输出重定向到管道中
		dup2(fd[1], 1); 
		close(fd[1]); //把管道的写端关闭
		}

		execvp(args[0],args);
		printf("command error\n");
		exit(0);
	}
	else if(pid > 0)
	{
		wait(NULL);
	}
}

//处理复杂命令 cat a.txt | grep a | wc -l
void handle_pipe_cmd(char *buf)
{
	//把复杂命令分成两段
	char *cmd1 = strtok(buf, "|"); //截取第一段简单命令
	char *cmd2 = strtok(NULL, "\0"); //截取剩余命令
	//开启匿名管道
	int fd[2];
	pipe(fd);
	//管道命令，后面的程序执行时会自动从标准输入获取数据
	//处理简单命令
	handle_simple_cmd(cmd1, fd);
	//处理剩余命令
	pid_t pid = fork();
	if (pid == 0)
	{
		close(fd[1]);
		dup2(fd[0], 0);
		close(fd[0]);

		handle_cmd(cmd2);
		exit(0);
	}
	close(fd[0]);
	close(fd[1]);
	wait(NULL);
}

//判断命令是简单命令还是复杂命令
void handle_cmd(char *buf)
{
	if (index(buf, '|') == NULL)
		handle_simple_cmd(buf, NULL); //处理简单命令
	else
	{
		handle_pipe_cmd(buf); //处理复杂命令
	}
}


int main()
{
	while(1)
	{
		//输出当前路径
		char path[1024] = {0};
		getcwd(path, sizeof(path));
		printf("%s > ", path);

		//获取用户输入的命令
		char buf[1024] = {0};
		fgets(buf, sizeof(buf), stdin);
		//去掉命令中的\n
		buf[strlen(buf)-1] = '\0';
		
		//判断是否是空命令
		if (strlen(buf) == 0)
			continue;
		//判断是否是退出命令
		if (strcmp(buf, "exit") == 0)
			break;
		//如果是其他命令，调用处理函数
		handle_cmd(buf);
	}
	return 0;
}

