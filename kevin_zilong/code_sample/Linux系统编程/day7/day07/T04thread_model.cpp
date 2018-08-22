
#include <stdio.h>
#include <pthread.h>
#include <list>
using namespace std;

// 以下程序问题很多，看看就好了
// 1. 没有加锁
// 2. 浪费CPU

list<int> datas; // 仓库是全局变量，当线程访问全局变量时，应该加锁

void* thread_func(void* ptr)
{
    while(1) // datas没有数据时，子线程一直耗费CPU
    {
        // 消费
        if(datas.size() > 0)
        {
            int data = *datas.begin();
            datas.pop_front();

            printf("%c\n", data);
        }
    }
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);

    while(1)
    {
        // 生产
        int ret = getchar();
        datas.push_back(ret);
    }
}
