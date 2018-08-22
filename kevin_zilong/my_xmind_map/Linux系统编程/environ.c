#include<stdio.h>
#include<stdlib.h>
#include<sys/times.h>

extern char **environ;  //需要声明为全局变量
int main()
{
    int index=0;
    struct tms buf;//定义一个时间结构体
    times(&buf);//得到程序运行到这所需时间
    while(1)
    {
        if(environ[index]==NULL)
        {
            break;
        }
        printf("%s\n",environ[index++]);//打印环境变量上的内容，直到为空
    }

    int i=0,j=0;
    for(i=0;i<100000;++i)
    {
        for(j=0;j<1000;++j)
        {
            continue;//消耗点时间，不然后面看不出来时间变化
        }
    }
    
    struct tms buf1; //定义一个时间结构体
    times(&buf1); //得到程序运行到这所需时间
    printf("%ld\n",buf1.tms_utime-buf.tms_utime); //打印程序运行消耗时间
    
    return 0;
}
