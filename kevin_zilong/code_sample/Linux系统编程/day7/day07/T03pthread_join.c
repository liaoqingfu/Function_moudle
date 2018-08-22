#include "../header.h"
#include <pthread.h>
#include <stdio.h>

void* thread_func(void* ptr)
{
//    sleep(2);
    void* p = 0;
    *p = 100; // 段错误：导致整个进程退出？
    // 进程隔离，线程融合
    return (void*)(intptr_t)99;
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);

    void* result;
    pthread_join(tid, &result);
    
    printf("%d\n", (int)(intptr_t)result);
    
//    while(1)
//        sleep(1);

    printf("join end\n");
}
