#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

typedef struct _Args
{
	char srcpath[1024];
	char dstpath[1024];
	int start;
	int fraglen;
}_Args;

void *copyfrag(void *p)
{
	_Args *arg = (_Args *)p;
	int srcfd = open(arg->srcpath, O_RDONLY);
	if (srcfd < 0)
	{
		printf("open srcpath error\n");
		return NULL;
	}
	int dstfd = open(arg->dstpath, O_WRONLY | O_CREAT, 0777);
	if (dstfd < 0)
	{
		close(srcfd);
		printf("open dstpath error\n");
		return NULL;
	}
	lseek(srcfd, arg->start, SEEK_SET);
	lseek(dstfd, arg->start, SEEK_SET);
	
	char buf[4096] = {0};
	while(1)
	{
		memset(buf, 0, sizeof(buf));
		int ret = read(srcfd, buf, arg->fraglen < sizeof(buf) ? arg->fraglen : sizeof(buf));
		if (ret <= 0)
			break;
		write(dstfd, buf, ret);
		arg->fraglen -= ret;
	}
	close(srcfd);
	close(dstfd);
	free(p);
}

void copyfile(const int jobs, const char *srcpath, const char *dstpath)
{
	struct stat src_stat;
	stat(srcpath, &src_stat);
	int filelen = (int)src_stat.st_size;
	int fraglen = filelen / jobs;
	int lastlen = filelen % jobs;

	//保存子线程的tid
	//pthread_t *tids = malloc(sizeof(pthread_t) * jobs);
	for (int i=0; i<jobs; ++i)
	{
		_Args *arg = malloc(sizeof(_Args));
		strcpy(arg->srcpath, srcpath);
		strcpy(arg->dstpath, dstpath);
		arg->start = fraglen * i;
		arg->fraglen = fraglen;
		//pthread_create(tids + i, NULL, copyfrag, (void *)arg);
		pthread_t tid;
		pthread_create(&tid, NULL, copyfrag, (void *)arg);
		pthread_detach(tid);
	}
	//在主线程中拷贝文件剩余部分
	_Args *arg = malloc(sizeof(_Args));
	strcpy(arg->srcpath, srcpath);
	strcpy(arg->dstpath, dstpath);
	arg->start = fraglen * jobs;
	arg->fraglen = lastlen;

	copyfrag((void *)arg);
	

	/*
	for (int i=0; i<jobs; ++i)
	{
		pthread_join(tids[i], NULL);
	}
	*/
	//free(tids);
	pthread_exit(0);
}
//./a.out 4 srcfile dstfile
int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		printf("args error\n");
		return -1;
	}
	const int jobs = atoi(argv[1]);
	const char *srcpath = argv[2];
	const char *dstpath = argv[3];
	copyfile(jobs, srcpath, dstpath);
	return 0;
}
