#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<pthread.h>
#include<time.h>
#include<sys/time.h>


typedef struct node//定义一个结构体，构成链表存放数据
{
    int data;
    struct node *next;
}Node;

//定义一个永远指向链表头部的指针
Node *head=NULL; //定义全局变量

//线程同步，互斥锁
pthread_mutex_t mutex;
//阻塞线程，条件变量类型的变量
pthread_cond_t cond;


void *producer(void *p)//实现生产者进程
{

    srand((unsigned int)time(NULL));//创建种子
    while(1)
    {
        Node *pnew=(Node *)malloc(sizeof(Node)); //申请内存空间
        if(pnew==NULL)
        {
            printf("malloc false\n");
            return;
        }
        //节点的数据初始化
        pnew->data=rand()%1000;//随机一个数0-999
        
        //使用互斥锁保护共享数据
        pthread_mutex_lock(&mutex);

        //节点的指针初始化,头插
        pnew->next=head;
        head=pnew;

        printf("=========produce: %lu,%d\n",pthread_self(),pnew->data);//打印生产者线程号和数据的值

        //解锁操作
        pthread_mutex_unlock(&mutex);

        //通知阻塞的消费者线程，解除阻塞
        pthread_cond_signal(&cond);//生产者发信号，通知消费者可以解除阻塞了

        sleep(rand()%3);//随机休眠0-2秒
    }
    return NULL;
}

void *customer(void *p)//实现消费者进程
{
    while(1)
    {
        pthread_mutex_lock(&mutex);//加互斥锁

        //判断链表是否为空
        if(head==NULL)
        {
           // continue;//不能实现线程的阻塞
            
           //实现线程的阻塞
            pthread_cond_wait(&cond,&mutex);//该函数会对互斥锁解锁，解除阻塞后，会在对互斥锁进行加锁操作

        }
        //链表不为空，需要删除一个结点，删头结点
        Node *pdel=head;
        head=head->next;
        printf("---------- customer : %lu,%d\n",pthread_self(),pdel->data);//打印消费者线程号，和数据值
        free(pdel);//删除头结点
        pthread_mutex_unlock(&mutex);//解锁
    }

    return NULL;
}

void test()
{

    pthread_t p1,p2;
    //创建生产者线程
    
    pthread_mutex_init(&mutex,NULL);//将锁初始化
    pthread_cond_init(&cond,NULL);//初始化


    pthread_create(&p1,NULL,producer,NULL);
    //创建消费者线程
    pthread_create(&p2,NULL,customer,NULL);

    pthread_join(p1,NULL);//回收子进程
    pthread_join(p2,NULL);//回收子进程

    pthread_mutex_destroy(&mutex);//销毁锁
    pthread_cond_destroy(&cond);//销毁

}

int main()
{
    test();
    return 0;
}
