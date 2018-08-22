#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>


void test01()
{
    struct stat buf;//定义一个属性结构体
    stat("./a.txt",&buf);//获取文件属性
    if(S_ISREG(buf.st_mode))//判断文件是否是普通文件
    {
        printf("这是普通文件\n"); //普通文件
    }
    struct stat buf1;//定义一个文件属性结构体
    stat("./llong/",&buf1);//获取文件属性
    if(S_ISDIR(buf1.st_mode))//判断文件是否是目录
    {
        printf("这是文件目录\n");//文件目录
    }
}

int main()
{
    test01();
    return 0;
}
