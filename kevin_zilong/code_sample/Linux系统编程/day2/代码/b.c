#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	struct stat buf1;
	stat("./b.txt", &buf1);
	struct stat buf2;
	stat("./b.slink", &buf2);
	//获取的都是源文件的属性
	printf("srcsize=%d,slinksize=%d\n", buf1.st_size, buf2.st_size);
	//获取链接文件本身的属性，不追踪源文件
	struct stat buf3;
	lstat("./b.slink", &buf3);
	printf("slinksize=%d\n", buf3.st_size);
	return 0;
}
