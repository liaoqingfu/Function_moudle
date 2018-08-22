#include <stdio.h>
#include <iostream>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <signal.h>
#include <errno.h>
#include <string>

using namespace std;

//定义用户结构体
typedef struct chat_user_t
{
	string name;
	int fd_read;
	int fd_write;
} chat_user_t;

map<string,chat_user_t*> users;

//定义epoll集合
int epollfd;

//处理信号的函数
void handle_sig(int signum, siginfo_t *info, void *p)
{
	//获取客户端的进程号，根据进程号找到文件
	char path1[1024];
	char path2[1024];
	sprintf(path1,"%d-1", (int)info->si_pid);
	sprintf(path2,"%d-2", (int)info->si_pid);

	chat_user_t *user = new chat_user_t;
	user->fd_read = open(path1, O_RDONLY);
	if (user->fd_read < 0)
	{
		delete user;
		return;
	}
	user->fd_write = open(path2, O_WRONLY);
	if (user->fd_write < 0)
	{
		close(user->fd_read);
		delete user;
		return;
	}

	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.ptr = user;

	epoll_ctl(epollfd, EPOLL_CTL_ADD, user->fd_read, &ev);
	printf("someone come in\n");
}


void regist_handle_sig()
{
	//注册信号处理函数
	struct sigaction act;
	act.sa_handler = NULL;
	act.sa_sigaction = handle_sig;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART|SA_SIGINFO;
	act.sa_restorer = NULL;

	sigaction(34, &act, NULL);
}

void handle_msg(char *msg, chat_user_t *user)
{
	//用户消息格式
	//setname xxx
	//to yyy: abc
	//to all: abc
	if (strncmp(msg, "setname", 7) == 0)
	{
		strtok(msg, " ");
		char *username = strtok(NULL, " ");
		user->name = username;

		//把用户加入到map中管理
		users[username] = user;
		printf("%s setname\n", username);
	}
	else if(strncmp(msg, "to", 2) == 0)
	{
		char *header = strtok(msg, ":");
		char *content = strtok(NULL, "\0");
		
		strtok(header, " ");
		char *tousername = strtok(NULL, "\0");

		if (strcmp(tousername, "all") == 0)
		{
			//to all
			for (auto it = users.begin(); it != users.end(); ++it)
			{
				if (it->second == user)
					continue;
				char buf[4096];
				memset(buf, 0, sizeof(buf));
				sprintf(buf, "from %s : %s", user->name.c_str(), content); 
				write(it->second->fd_write, buf, strlen(buf)+1); 
			}
		}
		else
		{
			//to one
			auto it = users.find(tousername);
			if (it == users.end())
			{
				const char *buf = "user not exist";
				write(user->fd_write, buf, strlen(buf)+1); 
				return;
			}
			chat_user_t *touser = it->second;
			
			char buf[4096];
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "from %s : %s", user->name.c_str(), content); 
			write(touser->fd_write, buf, strlen(buf));
		}
	}
}

void write_pid()
{
	int fd = open("./server.conf", O_RDWR|O_CREAT, 0777);
	if (fd < 0)
		exit(0);
	char buf[1024];
	sprintf(buf, "%d", (int)getpid());
	write(fd, buf, strlen(buf));
	close(fd);
}

int main()
{
	//服务器启动把进程号写入到配置文件
	write_pid();

	epollfd = epoll_create(512);
	//注册信号处理函数
	regist_handle_sig();


	struct epoll_event out_ev[8];
	//监听客户端消息
	while(1)
	{
		int ret = epoll_wait(epollfd, out_ev, 8, 2000);
		if (ret < 0)
		{
			if (errno == EINTR)
				continue;
			return -1;
		}
		else if (ret > 0)
		{
			for (int i=0; i<ret; ++i)
			{
				//获取产生消息的用户
				chat_user_t *user = (chat_user_t *)out_ev[i].data.ptr;
				//接收用户消息
				char buf[4096];
				memset(buf, 0, sizeof(buf));
				int result = read(user->fd_read, buf, sizeof(buf));
				if (result == 0)
				{
					close(user->fd_read);
					close(user->fd_write);
					users.erase(user->name);
					delete user;
					continue;
				}
				//调用消息处理函数
				handle_msg(buf, user);
			}
		}
	}
	return 0;
}
