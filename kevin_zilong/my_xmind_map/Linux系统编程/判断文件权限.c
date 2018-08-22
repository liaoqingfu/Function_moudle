#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>


void test01()
{
    struct stat buf;//定义一个文件属性结构体
    stat("./a.txt",&buf); //获取文件属性
    chmod("./a.txt",0777); //设置文件权限为全部权限
    if(S_IRUSR & buf.st_mode) //判断用户对文件是否有读权限
    {
        printf("用户对文件有读权限\n");
    }
    if(S_IWUSR & buf.st_mode) //判断用户对文件是否有写权限
    {
        printf("用户对文件有写权限\n");
    }    
    if(S_IXUSR & buf.st_mode) //判断用户对文件是否有执行权限
    {
        printf("用户对文件有执行权限\n");
    }
}

void test02()
{
    int ret=access("./a.out",F_OK); //判断文件是否存在，存在返回0，不存在返回-1
    if(ret)
    {
        printf("文件不存在\n");
    }
    else
    {
        printf("文件存在\n");
    }
    ret=access("./a.out",R_OK); //判断文件是否有读权限，有返回0，没有返回-1
    if(ret)
    {
        printf("用户没有读权限\n");
    }
    else
    {
        printf("用户有读权限\n");
    }
    ret=access("./a.out",W_OK); //判断文件是否有写权限，有返回0，没有返回-1
    if(ret)
    {
        printf("用户没有写权限\n");
    }
    else
    {
        printf("用户对文件有写权限\n");
    }
    ret=access("./a.out",X_OK); //判断文件是否有执行权限，存在返回0，不存在返回-1
    if(ret)
    {
        printf("用户没有执行权限\n");
    }
    else
    {
        printf("用户对文件有执行权限\n");
    }
}


int main()
{
   test01();
   // test02();
    return 0;
}
