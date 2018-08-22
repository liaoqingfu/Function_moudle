#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <utime.h>
//cp a.c b.c
int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("args error\n");
		return -1;
	}

	const char *srcfile = argv[1];
	const char *dstfile = argv[2];

	FILE *srcfp = fopen(srcfile, "r");
	if (srcfp == NULL)
	{
		printf("open src file error\n");
		return -1;
	}
	FILE *dstfp = fopen(dstfile, "w");
	if (dstfp == NULL)
	{
		printf("open dst file error\n");
		return -1;
	}
	
	char buf[4096];
	while(1)
	{
		int ret = fread(buf, 1, sizeof(buf), srcfp);
		if (ret <= 0)
		{
			break;
		}
		fwrite(buf, 1, ret, dstfp);
	}

	fclose(srcfp);
	fclose(dstfp);

	struct stat src_stat;
	stat(srcfile, &src_stat);
	//让目标文件的时间和源文件保持一致
	struct utimbuf tim;
	tim.actime = src_stat.st_atime;
	tim.modtime = src_stat.st_mtime;
	utime(dstfile, &tim);
	
	//修改文件的访问权限
	chmod(dstfile, src_stat.st_mode & ~S_IRUSR);
	//修改文件的所有者
	chown(dstfile, 0, src_stat.st_gid);
	return 0;
}
