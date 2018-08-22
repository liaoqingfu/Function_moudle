
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <semaphore.h>


sem_t producer_sem;//定义信号量
sem_t customer_sem;//定义信号量

typedef struct node{  //用一个链表去存储数据
    struct node *next;
    int data;
}node;   

node *head=NULL;

void *producer(void *set)
{
    srand((unsigned int)time(NULL)); //时间种子
    while(1)
    {
        sem_wait(&producer_sem); //资源全部用完就会等待，producer_sem --操作,相当于给信号量加锁
        node *pnew = (node *)malloc(sizeof(node));
        pnew->next =head; //因为head为NULL，就相当于 pnew->next=head->next
        pnew->data=rand()%1000;//给数据域赋随机值
        head=pnew;//head->next=pnew
        printf("+++++++生产者：%ld,%d\n",pthread_self(),pnew->data);
        sem_post(&customer_sem); //customer_sem ++,相当于给信号量解锁，消费者就可以去消费了
        sleep(rand()%3);//休眠0-2秒

    }
    return NULL;
}

void *customer(void *set)
{
    while(1)
    {
        sem_wait(&customer_sem);//一开始customer_sem为0，阻塞线程等待，相当于给信号量加锁
       // if(head==NULL) //信号量就不需要判断了，没有资源就会阻塞，等待生产者生成
        node *pel = head;
        head = head->next;
        printf("+++++++消费者 :%ld,%d\n",pthread_self(),pel->data);
        free(pel);
        sem_post(&producer_sem); //producer_sem ++ 做++操作，相当于给信号量解锁，告诉生产者有资源了，可以生成了
        sleep(rand()%3);//休眠0-2秒

    }
    return NULL;
}



int main()
{
    pthread_t p[2];//定义线程

    sem_init(&producer_sem,0,4);//初始化信号量，生产者信号量为5个，可以同时使用5个资源，中间的0为线程，不为0的话就是进程
    sem_init(&customer_sem,0,0);//初始化信号量，消费者一开始是0，没有分配资源，线程阻塞状态


    pthread_create(&p[0],NULL,producer,NULL);//创建线程
    pthread_create(&p[1],NULL,customer,NULL);//创建线程

    int i=0;
    for(i=0;i<2;++i)
    {
        pthread_join(p[i],NULL);//回收子线程
    }
    sem_destroy(&producer_sem);//销毁信号量，回收资源
    sem_destroy(&customer_sem);

    return 0;
    
}
