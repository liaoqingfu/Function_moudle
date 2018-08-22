#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<string.h>
void test01()
{
    int fd=open("./b.txt",O_RDWR); //以读写的方式打开文件
    //mmap(内存地址(不写的话，系统会自动分配，所以一般写NULL)，映射长度(不能大于文件大小，一般就是写文件长度)，打开文件时的方式(即上面打开文件用的是读写方式，这里打开映射就要是读和写,标记，文件描述符，偏移量)
    ftruncate(fd,1024);   //设置文件的大小为1024，跟truncate(文件路径，文件长度) 文件多了的话，就会自动截断
   void *p= mmap(NULL,1024,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0); //映射文件到内存中,用void *来接，mmap返回一个void * 指针
    close(fd);//映射好了，就可以关闭文件了的
    printf("%s\n",(char *)p); //这样就可以直接操作那块内存地址的了，将文件内容都打印出来了的，大小为1024，不要越界
    memcpy(p,"kevin",5); //将kevin字符直接放到了文件里面，因为映射了的，即使关闭了文件，也可以操作文件的，除非解除映射
    printf("p=%s\n",(char *)p); // 文件内容增加了kevin,但是不是追加的方式，这样文件前面5个字符会被kevin覆盖
    munmap(p,1024);//解除映射

    
}

int main()
{
    test01();
    return 0;

}
