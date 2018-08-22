#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>
#include<utime.h>
#include<sys/time.h>

void test()
{
    struct utimbuf buf; //定义一个时间结构体
    struct stat sta; //定义一个文件属性结构体
    buf.actime=1000; //修改文件的访问时间 ：access
    buf.modtime=100000; //修改文件的改变时间：modification,时间都会从系统开始时间算

    utime("./uuu.txt",&buf); //设置修改的时间，改变了文件的访问时间和修改时间，不能改变文件的创建时间

    stat("./uuu.txt",&sta);//获取文件的属性
    printf("%d\n",(int)sta.st_atime); //打印文件的访问时间
    printf("%d\n",(int)sta.st_mtime); //打印文件的修改时间
    printf("%d\n",(int)sta.st_ctime); //打印文件的创建时间

}
void test01()
{
    struct timeval tv[2]; //定义一个时间结构体
    struct stat sta; //定义一个文件属性结构体
    tv[0].tv_sec=10000; //修改文件的访问时间,秒 ：access
    tv[0].tv_usec=1000; //修改文件的访问时间，微秒 必须比sec设置的小
    
    tv[1].tv_sec=10000; //修改文件的改变时间，秒 modification
    tv[1].tv_usec=1000; //修改文件的改变时间，微妙 必须比sec设置的小

    utimes("./uuu.txt",tv); //设置修改的时间，改变了文件的访问时间和修改时间，不能改变文件的创建时间

    stat("./uuu.txt",&sta);//获取文件的属性
    printf("%d\n",(int)sta.st_atime); //打印文件的访问时间
    printf("%d\n",(int)sta.st_mtime); //打印文件的修改时间
    printf("%d\n",(int)sta.st_ctime); //打印文件的创建时间

}

int main()
{
   // test();
    test01();
    return 0;
}
