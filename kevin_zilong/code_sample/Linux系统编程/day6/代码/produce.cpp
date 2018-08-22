#include <list>
#include <stdio.h>
#include <iostream>
#include <pthread.h>

using namespace std;

struct _task
{
	int task;
};

pthread_mutex_t mutex;
//定义条件变量
pthread_cond_t cond;

//定义消息队列
list<_task*> que;

void *thread_func(void *p)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		//等待条件触发
		pthread_cond_wait(&cond, &mutex); //默认进行一次解锁，然后开始等待，条件触发默认再加一次锁
		while(1)
		{
			if (que.size() == 0)
			{
				pthread_mutex_unlock(&mutex);
				continue;
			}
			//处理消息
			_task *task = *que.begin();
			que.pop_front();

			pthread_mutex_unlock(&mutex);
			printf("%d\n", task->task);
		}
	}
	return NULL;
}


int main()
{
	//初始化条件变量
	pthread_cond_init(&cond, NULL);
	pthread_mutex_init(&mutex, NULL);
	pthread_t tid;
	pthread_create(&tid, NULL, thread_func, NULL);

	while(1)
	{
		int i;
		cin >> i;
		_task *task = new _task;
		task->task = i;
		pthread_mutex_lock(&mutex);
		que.push_back(task);
		pthread_mutex_unlock(&mutex);
		//唤醒条件变量
		pthread_cond_signal(&cond);
	}
	pthread_join(tid, NULL);
	return 0;
}
