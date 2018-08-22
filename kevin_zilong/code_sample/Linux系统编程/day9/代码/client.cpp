#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>

using namespace std;

pid_t get_server_pid()
{
	int fd = open("./server.conf", O_RDONLY);
	if (fd < 0)
		exit(0);
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	read(fd, buf, sizeof(buf));
	close(fd);
	return (pid_t)atoi(buf);
}

void *thread_recv(void *p)
{
	int fd_read = (int)(intptr_t)p;
	while(1)
	{
		char buf[1024];
		memset(buf, 0, sizeof(buf));
		int ret = read(fd_read, buf, sizeof(buf));
		if (ret == 0)
			exit(0);
		printf("%s\n", buf);
	}
	return NULL;
}

int main()
{
	char path1[1024];
	sprintf(path1, "%d-1", (int)getpid());
	mkfifo(path1, 0777);

	char path2[1024];
	sprintf(path2, "%d-2", (int)getpid());
	mkfifo(path2, 0777);

	pid_t server_pid = get_server_pid();
	//给服务器发信号
	union sigval val;
	sigqueue(server_pid, 34, val); 

	int fd_write = open(path1, O_WRONLY);
	int fd_read = open(path2, O_RDONLY);

	pthread_t tid;
	pthread_create(&tid, NULL, thread_recv, (void *)(intptr_t)fd_read);

	while(1)
	{
		char buf[1024];
		memset(buf, 0, sizeof(buf));
		fgets(buf, sizeof(buf), stdin);
		//去掉\n
		buf[strlen(buf)-1] = '\0';

		if (strlen(buf) == 0)
			continue;

		write(fd_write, buf, strlen(buf)+1);
	}
	return 0;
}
