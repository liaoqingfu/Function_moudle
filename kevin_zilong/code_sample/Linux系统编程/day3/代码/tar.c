#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <strings.h>
//                            /home/saul/Test/mydir/dir1/a.txt
void tarfile(const char *root, FILE *dstfile, const char *filepath)
{
	//把源文件名写入到目标文件
	fprintf(dstfile, "-\n%s\n", filepath + strlen(root));
	//获取源文件大小
	struct stat buf_stat;
	stat(filepath, &buf_stat);
	//把源文件大小写到目标文件
	fprintf(dstfile, "%d\n", (int)buf_stat.st_size);
	FILE *fp = fopen(filepath, "r");
	if (fp == NULL)
	{
		printf("open filepath error\n");
		return;
	}
	char buf[4096] = {0};
	while(1)
	{
		int ret = fread(buf, 1, sizeof(buf), fp);
		if (ret <= 0)
		{
			break;
		}
		fwrite(buf, 1, ret, dstfile);
	}
	fclose(fp);
}
//                             /home/saul/Test/mydir/dir1
void tardir(const char *root, FILE *dstfile, const char *srcpath)
{
	//把要打包的目录名写入到目标文件
	if (strcmp(root, srcpath) != 0)
	fprintf(dstfile, "d\n%s\n", srcpath + strlen(root));
	//打开文件夹
	DIR *dir = opendir(srcpath);
	while(1)
	{
		struct dirent *de = readdir(dir);
		if (de == NULL)
			break;

		//拼接完整路径
		char filepath[1024] = {0};
		sprintf(filepath, "%s/%s", srcpath, de->d_name);

		if (de->d_type == DT_REG)
		{
			//打包文件函数
			tarfile(root, dstfile, filepath);
		}
		else if (de->d_type == DT_DIR)
		{
			if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
			{
				continue;
			}
			tardir(root, dstfile, filepath);
		}
	}
}

// ./tar.out /home/saul/Test/mytar  /home/saul/Test/mydir
//                                   ./mydir
//                                   mydir
void tar(const char *dstpath, const char *srcpath)
{
	//打包前的准备工作
	FILE *fp = fopen(dstpath, "w");
	//在打包前先把源文件的根目录写入目标文件
	char *name = rindex(srcpath, '/');
	if (name == NULL) name = (char *)srcpath;
	else name = name + 1;
	fprintf(fp, "%s\n", name);
	//打包目录函数
	tardir(srcpath, fp, srcpath);
	//打包文件加结束标记
	fprintf(fp, "%s\n", "{--END--}");
	fclose(fp);
}
//获取一行数据的函数
char * getLine(FILE *fp)
{
    static char buf[1024] = {0};
	fgets(buf, sizeof(buf), fp);
	buf[strlen(buf) - 1] = '\0'; //mydir
	return buf;
}

int untarfile(const char *root, FILE *fp)
{
	char *type = getLine(fp);
	if (strcmp(type, "{--END--}") == 0)
		return -1;
	if (strcmp(type, "d") == 0)
	{
		char *dirname = getLine(fp); //读取出目录名
	char filepath[1024] = {0};
	sprintf(filepath, "%s%s", root, dirname); // mydir/dir1
	mkdir(filepath, 0777);
	return 0;
	}
	else if (strcmp(type, "-") == 0)
	{
	char *filename = getLine(fp); // /b.txt
	char filepath[1024] = {0};
	sprintf(filepath, "%s%s", root, filename); // mydir/b.txt
	FILE *dstfile = fopen(filepath, "w");
	if (dstfile == NULL)
	{
		printf("open untar dstfile error\n");
		return -2;
	}
	int filelen = atoi(getLine(fp));
	char buf[1024] = {0};
	while(filelen > 0)
	{
		memset(buf, 0, sizeof(buf));
		int ret = fread(buf, 1, filelen < sizeof(buf) ? filelen : sizeof(buf), fp);
		if (ret <= 0)
			break;
		fwrite(buf, 1, ret, dstfile);
		filelen -= ret;
	}
	fclose(dstfile);
	}
	return 0;	
}
//解包函数
void untar(const char *path)
{
	FILE *fp = fopen(path, "r");
	char *root = strdup(getLine(fp));
	mkdir(root, 0777); //创建相对根目录
	while(1)
	{
	//解包文件函数
		int ret = untarfile(root, fp);
		if (ret == -1)
			break;
	}
	free(root);
	fclose(fp);
}

// ./tar.out ./mytar ./mydir 打包
// ./tar.out ./mytar 解包
int main(int argc, char *argv[])
{
	if (argc == 2)
	{
		untar(argv[1]); //自定义解包函数
	}
	else if (argc == 3)
	{
		tar(argv[1], argv[2]); //自定义打包函数
	}
	else
	{
		printf("args error\n");
	}
	return 0;
}
