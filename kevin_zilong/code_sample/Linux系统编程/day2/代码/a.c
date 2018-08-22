#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

int main()
{
	//获取文件属性
	struct stat buf;
	stat("./a.txt", &buf);
	//获取文件大小
	int size = buf.st_size;
	printf("size=%d\n", size);
	//判断文件类型
	if(S_ISREG(buf.st_mode))
	{
		printf("regular file\n");
		//判断文件权限
		if(S_IRUSR & buf.st_mode)
		{
			printf("user read\n");
		}
		else if(S_IXUSR & buf.st_mode)
		{
			printf("user exe\n");
		}
	}
	else if(S_ISDIR(buf.st_mode))
	{
		printf("dir file\n");
		return 0;
	}

	//判断文件是否存在
	int ret = access("./a.txt", F_OK);
	if (ret)
	{
		printf("a.txt not exist\n");
	}
	else
	{
		printf("a.txt is exist\n");
	}

	//判断实际用户是否有读的权限
	ret = access("./a.txt", R_OK);
	if (ret)
	{
		printf("not read\n");
	}
	else
	{
		printf("read\n");
	}

	int fd = open("./c.txt", O_WRONLY | O_CREAT, 0777);
	if(fd < 0)
	{
		perror("open");
	}
	close(fd);
	return 0;
}
