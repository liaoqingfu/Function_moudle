#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void docopy(const char *srcpath, const char *dstpath, int start, int frag)
{
	int srcfd = open(srcpath, O_RDONLY);
	int dstfd = open(dstpath, O_WRONLY | O_CREAT, 0777);
	
	lseek(srcfd, start, SEEK_SET);
	lseek(dstfd, start, SEEK_SET);

	char buf[4096] = {0};
	while(frag > 0)
	{
		memset(buf, 0, sizeof(buf));
		int ret = read(srcfd, buf, sizeof(buf) < frag ? sizeof(buf) : frag);
		if (ret <= 0)
			break;
		write(dstfd, buf, ret);
		frag -= ret;
	}

	close(srcfd);
	close(dstfd);
}

void copyfile(int jobs, const char *srcpath, const char *dstpath)
{
	//获取源文件属性
	struct stat src_stat;
	stat(srcpath, &src_stat);
	//获取源文件大小
	int filelen = (int)src_stat.st_size;
	//确定每个进程读的片段长度
	int frag = filelen/jobs;
	//剩下的长度由主进程去拷贝
	int lastlen = filelen%jobs;

	//根据进程数开启进程
	for (int i = 0; i < jobs; ++i)
	{
		//创建子进程
		pid_t pid = fork();
		if (pid == 0)
		{
			//子进程调用docopy拷贝文件片段
			docopy(srcpath, dstpath, frag * i, frag);
			return;
		}
	}
	//把文件剩余部分拷贝
	docopy(srcpath, dstpath, jobs * frag, lastlen);
	//回收子进程
	for (int i = 0; i < jobs; ++i)
		wait(NULL);
}

// ./a.out 4 src dst
int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		printf("args error\n");
		return -1;
	}

	int jobs = atoi(argv[1]);
	const char *srcpath = argv[2];
	const char *dstpath = argv[3];

	copyfile(jobs, srcpath, dstpath);
	return 0;
}

