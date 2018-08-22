#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

int main()
{
	DIR *dir = opendir(".");
	struct dirent *de = readdir(dir);
	long pos;
	while(de)
	{
		if(de->d_name[0] == '.')
		{
			de = readdir(dir);
			continue;
		}

		struct stat buf_stat;
		stat(de->d_name, &buf_stat);

		//先打印文件类型
		if (S_ISREG(buf_stat.st_mode))
		{
			printf("-");
		}
		else if(S_ISDIR(buf_stat.st_mode))
		{
			printf("d");
		}
		//打印文件权限
		if (buf_stat.st_mode & S_IRUSR)
		{
			printf("r");
		}
		else
		{
			printf("-");
		}
		if (buf_stat.st_mode & S_IWUSR)
		{
			printf("w");
		}
		else
		{
			printf("-");
		}
		
		if (buf_stat.st_mode & S_IXUSR)
		{
			printf("x");
		}
		else
		{
			printf("-");
		}

		//打印文件大小
		printf("\t%d\t", (int)buf_stat.st_size);
		//打印文件所有者
		printf("%d\t", (int)buf_stat.st_uid);
		//判断：如果文件名等于ls.c，那么记录下ls.c的位置
		//if (strcmp(de->d_name, "ls.c") == 0)
		//pos = 	telldir(dir);
		//打印文件名
		printf("%s\n", de->d_name);
		de = readdir(dir);
	}
	closedir(dir);
	/*
	printf("-------------------\n");
	//从ls.c的下一个位置开始打印
	dir = opendir(".");
	seekdir(dir, pos);
	de = readdir(dir);
	while(de)
	{
		if (de->d_name[0] != '.')
			printf("%s\n", de->d_name);
		de = readdir(dir);
	}
	printf("-----------------------\n");
	rewinddir(dir); //把光标复位到初始位置
	de = readdir(dir);
	while(de)
	{
		if (de->d_name[0] != '.')
			printf("%s\n", de->d_name);
		de = readdir(dir);
	}

	closedir(dir);
	*/
	return 0;
}
