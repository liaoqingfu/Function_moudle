#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include<fcntl.h>


void test01()
{
    struct stat buf; //定义一个文件属性的结构体
    stat("./a.txt",&buf); //获取文件属性
    int size=buf.st_size; //获取文件大小
    int inode=buf.st_ino; //获取文件编号
    int dev=buf.st_dev; //获取设备号
    int blocks=buf.st_blocks; //获取文件块数量 blocks
    printf("size=%d,inode=%d\n",size,inode);
    printf("dev=%d,blocks=%d\n",dev,blocks);//文件存储是遵循4K对齐的，所以最小是8块
    if(S_ISREG(buf.st_mode)) //判断文件类型是否是文件
    {
        printf("regular file\n");
        if(S_IRUSR & buf.st_mode)//判断用户对文件是否有读权限
        {
            printf("user read\n");
        }
        if(S_IXUSR & buf.st_mode) //判断用户对文件是否有执行权限
        {
            printf("user exe\n");
        }
        if(S_IWUSR & buf.st_mode)//判断用户对文件是否有写权限
        {
            printf("user write\n");  
        }
    }
    else if(S_ISDIR(buf.st_mode)) //判断文件类型是否是目录
    {
        printf("dir file\n");
    }
}

void test02()
{
    int ret=access("./a.txt",F_OK);//判断文件是否存在，存在返回0，不存在返回-1
    if(ret)
    {
        printf("a.txt not exist\n");
    }
    else
    {
        printf("a.txt is exist\n");
    }

    ret = access("./a.txt",R_OK);//判断实际用户是否有读的权限,成功返回0，不成功返回-1
    if(ret)
    {
        printf("not read\n"); 
    }
    else
    {
        printf("read\n");
    }
   int ret1 = access("./a.txt",W_OK);//判断实际用户是否有写的权限,成功返回0，不成功返回-1
    if(ret1)
    {
        printf("not write\n"); 
    }
    else
    {
        printf("write\n");
    }

   int ret2 = access("./a.txt",X_OK);//判断实际用户是否有可执行的权限,成功返回0，不成功返回-1
    if(ret2)
    {
        printf("not exe\n"); 
    }
    else
    {
        printf("exe\n");
    }
    int fd=open("./ui.txt",O_WRONLY|O_CREAT,0777);
    if(fd<0)
    {
        perror("open false:");
    }
    close(fd);
}

int main()
{
   // test01();
    test02();
    return 0;
}
