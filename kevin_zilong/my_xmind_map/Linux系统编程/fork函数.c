#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

void test01()
{
    pid_t pid=fork(); //产生一个子进程
    if(pid==0)
    {
        printf("子进程: %d\n",pid);
    }
    else if(pid>0)
    {
        wait(NULL);  //等待子进程结束后才执行主进程
        printf("主进程 ：%d\n",pid);
    }
}
void test02()//子进程0 ，1，2，3，4，主进程0，1，2，3，4
{
    pid_t pid=fork();
    int i=0;
    for(i=0;i<5;++i)
    {
        if(pid==0)
        {
            printf("子进程 ：%d\n",i);
        }
        else if(pid>0)
        {
            wait(NULL);
            printf("主进程 ：%d\n",i);
        }
    }
}

void test03()//子进程 0 主进程 0 ；子进程 1 主进程 1 ；子进程 2 主进程 2 ；子进程 3 主进程3；子进程 4 主进程 4；
{
    int i=0;
    for(i=0;i<5;++i)
    {
        pid_t pid=fork();
        if(pid==0)
        {
            printf("子进程 ：%d\n",i);
          //  break;
            return;
        }
        else if(pid>0)
        {
            wait(NULL);
            printf("主进程 ：%d\n",i);
        }
    }
}


void test04()
{
    int i=0;
    for(i=0;i<5;++i)
    {
        pid_t pid=fork();
        if(pid==0)
        {
            printf("子进程 ：%d\n",i);
          //  break;
            return;
        }
    }
    int j=0;
    for(j=0;j<5;++j)
    {
        wait(NULL);
        printf("主进程 ：%d\n",j);
    }

        printf("主oooo进程 ：%d\n",j);
}
int main()
{
   // test01();
   // test02();
   // test03();
    test04();
    return 0;
}
