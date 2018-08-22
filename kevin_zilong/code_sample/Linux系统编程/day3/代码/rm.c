#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

void rm(const char *path)
{
	//获取文件属性
	struct stat buf_stat;
	stat(path, &buf_stat);
	//判断路径是文件还是目录
	if (!S_ISDIR(buf_stat.st_mode))
	{
		unlink(path); //如果参数不是目录，直接删除
		return;
	}
	//如果参数是目录
	//打开目录
	DIR *dir = opendir(path); // mydir/adir
	while(1)
	{
		struct dirent *de = readdir(dir);
		if(de == NULL)
			break;
		//拼接完整路径
		char filepath[1024] = {0};
		sprintf(filepath, "%s/%s", path, de->d_name); // mydir/adir
		//判断目录项是文件还是子目录
		if (de->d_type != DT_DIR)
		{
			unlink(filepath);  // mydir/a.txt
		}
		else
		{
			//如果目录项是一个目录
			if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
			{
				continue;
			}
			rm(filepath);
		}
		
	}
	//删除空目录
	rmdir(path);
}
// ./rm.out mydir
int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("args error\n");
		return -1;
	}

	const char *path = argv[1];
	rm(path); //自定义的删除函数
	return 0;
}

