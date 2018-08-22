#include "../header.h"

int a;
pthread_t tid1;
pthread_t tid2;

//  进程 = 线程 + 数据
//  进程 = 多个线程 + 数据
//
//  进程是分配资源的单位
//  线程是系统调度的单位
//
//  线程在内核中，叫任务(task) struct task_struct

// 子线程
// 函数形式是固定的
void foo()
{
    pthread_t tid = pthread_self();
    if(pthread_equal(tid, tid1))
    {
        printf("in thread1\n");        
    }
    else if(pthread_equal(tid, tid2))
    {
        printf("in thread2\n");
    }
    else
    {
        printf("unknown id=%u\n", (unsigned int) tid);
    }
}

void* thread_func(void* ptr)
{
    foo();

    while(1)
    {
        // exit(0); // 整个进程推出，所有线程退出

        pthread_exit(0); // 调用该函数的线程退出
 //       printf("in thread a=%d\n", a);
        usleep(100*1000);
    }
    return NULL;
}

// 主线程
int main()
{

    // 创建一个线程
    pthread_create(&tid1, // 线程id，由系统分配，保存到tid
            NULL,        // 线程的额外属性，如果为NULL，那么使用默认的属性
            thread_func, // 线程的入口函数，理解成线程的main函数
            NULL);       // 传递给线程的参数


    pthread_create(&tid2, NULL, thread_func, NULL);

    foo();

    while(1)
    {
        a++;
 //       printf("in main\n");
        usleep(100*1000); // 100毫秒
    }


    pthread_exit(0);
    return 0; // 进程执行到这里时，所有线程退出
}
