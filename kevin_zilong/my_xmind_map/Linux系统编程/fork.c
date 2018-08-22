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
            printf("子进程 ：%d\n",i); //调用的时候由cpu来调度
           // break; //是break的话，后面的主进程还是会运行5次，因为break是跳出循环，后面的打印还是会执行到底
            return;//是return的话，后面的主进程只运行一次，因为return是返回，后面的打印就不会执行了的，所以用return好
        }
    }
    int j=0;
   // for(j=0;j<5;++j)
   // {
     //   wait(NULL);
      //  printf("主进程 ：%d\n",j);
   // }

        printf("主oooo进程 ：%d\n",j);//最后来掉用
}


void test05()
{
    int i=0;
    pid_t pid=fork();
    if(pid==0)
    {
         printf("子进程\n");
    }
   
    printf("111111\n");

    if(pid>0)
    {
        wait(NULL);
        printf("主进程\n");
    }
    printf("222222\n");
}

void test06()
{
    int i=0;
    pid_t pid=fork();
    printf("pid=%d\n",pid);//会打印两次 一次是父进程的，一次是子进程的 子进程的一定是0，
    if(pid==0)
    {
       // pid_t pid2=fork();//也会产生两个，一次是子进程的，另一次是子子进程的，子子进程pid2一定是等于0
        pid=fork();//会产生两个pid，子进程的和父进程的，跟上面是一样的
        printf("pid=%d\n",pid);
        if(pid==0)
        {
            printf("子子进程\n");
        }
        else if(pid>0)
        {
            wait(NULL);//回收子子进程的
            printf("子进程\n");
        }
       // printf("子进程\n");
    }
    else if(pid>0)
    {
        wait(NULL);//回收子进程的
        printf("父进程\n");
    }
}

int main()
{
   // test01();
   // test02();
   // test03();
   // test04();
   // test05();
    test06();
    return 0;
}
