
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void myfunc(int ret)
{

    time_t tim;
    time(&tim);//获取系统时间
    char buf[4096]={0};
    ctime_r(&tim,buf);//转换时间格式
    int fd= open("/home/zilong/log.txt",O_WRONLY|O_CREAT|O_APPEND,0777);
    write(fd,buf,strlen(buf));
    close(fd);
}

int main()
{
    pid_t pid=fork();//产生一个子进程
    if(pid>0)
    {
        exit(1);
        //kill(getpid(),SIGKILL);//结束父进程

    }
    else if(pid == 0)
    {
        setsid();//子进程创建新的会话
        chdir("/home/zilong"); //改变工作目录
        umask(0);//设置权限
        close(STDIN_FILENO);//关闭标准输入输出和标准错误文件，回收资源
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        struct sigaction act;//设置信号
        act.sa_flags=0;
        act.sa_handler=myfunc;//设置信号函数
        sigemptyset(&act.sa_mask);//清空信号集
        sigaction(SIGALRM,&act,NULL);//发送信号

        struct itimerval tt;//定义定时器
        tt.it_interval.tv_sec=2;//每两秒循环一次
        tt.it_interval.tv_usec=0;
        tt.it_value.tv_sec=1; //定时一秒，1秒后开始执行
        tt.it_value.tv_usec=0;
        setitimer(ITIMER_REAL,&tt,NULL); //设置定时器

        while(1)//不结束子进程，守护进程后台一直运行
        {
            continue;
        }
    }
    return 0;
}
