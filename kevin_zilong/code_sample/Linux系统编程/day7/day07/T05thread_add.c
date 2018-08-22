#include "../header.h"

// 锁的粒度
//
pthread_mutex_t mutex; // 锁
pthread_mutex_t mutexB; // 

int a = 0;
int b = 0;

#if 0
void foo()
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        if(datas.size() == 0)
        {
            continue;
        }

        int data = *datas.begin();
        datas.pop_front();
        pthread_mutex_unlock(&mutex);
    }
}
#endif

#if 0
void access_global()
{
    pthread_mutex_lock(&mutex);
    a = 100;
    pthread_mutex_unlock(&mutex);
}
#endif


void* thread_func(void* ptr)
{
#if 0
    pthread_mutex_lock(&mutex);
    access_global();
    pthread_mutex_unlock(&mutex);
#endif

    int i=0;
    for(i=0; i<1000000; i++)
    {
        // 原子操作
        pthread_mutex_lock(&mutex);
        pthread_mutex_lock(&mutex); // 加锁
        a++;
        pthread_mutex_unlock(&mutex); // 解锁

        pthread_mutex_lock(&mutexB);
        b++;
        pthread_mutex_unlock(&mutexB);

    }
}

int main()
{
    // 初始化锁
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutexB, NULL);

    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);


    int i=0;
    for(i=0; i<1000000; i++)
    {
        pthread_mutex_lock(&mutex); // 加锁
        a++;
        pthread_mutex_unlock(&mutex); // 解锁

        pthread_mutex_lock(&mutex);
        b++;
        pthread_mutex_unlock(&mutex);
    }

    pthread_join(tid, NULL);

    pthread_mutex_destory(&mutex);
    printf("a is %d\n", a);
}
