#include "../header.h"
class AutoLock
{
public:
    AutoLock(pthread_mutex_t& m):mutex(m){
        pthread_mutex_lock(&mutex);
    }

    ~AutoLock(){
        pthread_mutex_unlock(&mutex);
    }
private:
    pthread_mutex_t& mutex;
};

#define MyLock(mutex) AutoLock _______lock(mutex)

int a = 0;
pthread_mutex_t mutex;

void* thread_func(void* ptr)
{
    int i;
    for(i=0; i<1000000; i++)
    {
        MyLock(mutex);
        AutoLock lock(mutex);
        a++;
    }
}

int main()
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    // 属性结构中设置循环锁属性
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

    // 锁的属性
    pthread_mutex_init(&mutex, 
            &attr); // 设置锁的属性

    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);

    int i;
    for(i=0; i<1000000; i++)
    {
        MyLock(mutex);
        a++;
    }
    pthread_join(tid, NULL);
    printf("%d\n", a);

    {
        MyLock(mutex);
        // 此处省略3行
    }
    // 此处省略10000行
}

