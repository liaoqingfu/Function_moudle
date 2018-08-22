#include<unistd.h>
#include<signal.h>
#include<stdio.h>

//实现信号函数
void handle_int(int a)
{
    if(a==SIGINT)//判断传入的信号是否是SIGINT的
    {
        printf("SIGINT信号 %d\n",SIGINT); //2
    }
    else if(a==SIGRTMIN)
    {
        printf("SIGRTMIN信号 %d\n",SIGRTMIN); //34
    }
}

void handle_hup(int b)
{
    if(b==SIGHUP)
    {
        printf("SIGHUP信号 %d\n",SIGHUP);//1
    }
}


int main()
{
    signal(SIGINT,handle_int);//重新定义信号SIGINT，信号原来的功能就会失效
    signal(SIGHUP,handle_hup);//重新定义信号SIGHUP,信号原来的功能就会失效
    signal(SIGRTMIN,handle_int);//重新定义信号SIGTMIN,信号原来的功能就会失效
    //有信号发送，才会触发信号，才会打印
   // sleep(10);//10秒后发送信号
    while(1)
    {
        getchar(); //使进程停在这里，然后通过另一个进程发信号
    }
    return 0; //用另外一个进程，用kill发送信号 kill(pid,SIGINT) 即 kill(pid ,信号类型）；
}
