#include<unistd.h>


int main()
{


    pid_t pid=fork();//产生一个子进程

    if(pid==0)//子进程执行以下命令
    {
        setsid();  //设置一个新的会话界面，脱离原来的程序，子进程完全独立出来 了，结束掉父进程，子进程还是在运行，产生了孤儿进程，孤儿进程由init进程接收
    }

    while(1)
    {
        sleep(1);
    }
    return 0;
}
