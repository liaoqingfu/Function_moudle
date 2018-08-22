#include<stdio.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
int main()
{
    int fd=open("a.txt",O_RDWR);//以读写的方式打开文件，打开成功返回0，打开失败小于0；
    if(fd<0)
    {
       // printf("errno is %d,error string=%s\n",errno,strerror(errno)); 
        // 两种抒写方式，可以用strerror将errno放入，也可以用perror函数，下面更好用点
        perror("open file:");
    }
    return 0;
}
