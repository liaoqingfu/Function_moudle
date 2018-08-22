#include<stdio.h>

#include<stdlib.h>

#include<unistd.h>

#include<sys/types.h>

#include<sys/stat.h>

#include<fcntl.h>

#include<ctype.h>

#include<string.h>

#include<sys/time.h>

#include<signal.h>

 

struct itimerval one_timer;

struct timers

{

	int interval; //定时时间

	void(*handler)(); //处理函数

};

struct timers two_timer;

struct timers five_timer;

struct timers ten_timer;

 

void multi_timer_manage()

{

	printf("\n---");

	two_timer.interval--;

	if(two_timer.interval==0)

	{

		two_timer.interval=2;

		two_timer.handler();

	}

	five_timer.interval--;

	if(five_timer.interval==0)

	{

		five_timer.interval=5;

		five_timer.handler();

	}

	ten_timer.interval--;

	if(ten_timer.interval==0)

	{

		ten_timer.interval=10;

		ten_timer.handler();

	}

}

 

void two_output()

{

	printf("2 ");

}

 

void five_output()

{

	printf("5 ");

}

 

void ten_output()

{

	printf("10 ");

}

 

void init()

{

	one_timer.it_interval.tv_sec=1; //设置单位定时器定时时间

	one_timer.it_value.tv_sec=1; //设置单位定时器初始值

	setitimer(ITIMER_REAL,&one_timer,NULL); //初始化单位定时器

	signal(SIGALRM,multi_timer_manage); //指定单位定时器定时时间到时执行的函数


	two_timer.interval=2;

	two_timer.handler=two_output;

	five_timer.interval=5;

	five_timer.handler=five_output;

	ten_timer.interval=10;

	ten_timer.handler=ten_output;

}

 

int main()

{

	init();

	while(1);

}

 

执行结果如下：

---

---2 

---

---2 

---5 

---2 

---

---2 

---

---2 5 10 

---

---2 

---

---2 

---5 

---2 

---

---2 

---

---2 5 10 

......

可见，每秒都输出---，每隔2秒输出2，每隔5秒输出5，每隔10秒输出10。