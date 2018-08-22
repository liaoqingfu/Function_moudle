#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>

void test01()
{
    int fd=open("./a.txt",O_RDWR); //以读写的方式打开文件
    if(fd<0) //判断文件是否打开成功
    {
        perror("open false:");
    }
    unlink("./a.txt"); //删除文件，但是不会立马删除，编译器等到close文件后才会删除文件
    close(fd); //这样后面就读不到文件的内容的内容了
    char buf[1024]={0};
    read(fd,buf,sizeof(buf)); //还是可以读文件的，文件没关闭，并没有删除
    printf("%s\n",buf); //文件未结束，可以读到文件内容
   // close(fd);
}
int main()
{
    test01();
    return 0;
}
