#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

long long zx_LocalTime_ms(void)		//获取当前时间，秒
{
    struct timeval tv;
    long long time_s;
    struct tm * t = NULL;
    time_t tt;
    
    gettimeofday(&tv, NULL);
      
    time_s = (long long)tv.tv_sec  + (long long)tv.tv_usec;
    
    tt = (time_t)time_s;
    
    t = localtime(&tt);
    
    printf("%s\n", ctime(&tt));		//打印出时间

    printf("%s\n", asctime(t));
    
    return time_s;
}

long long zx_LocalTime_ms1(void)		//获取当前时间，毫秒
{
    struct timeval tv;
    long long ms;

    gettimeofday(&tv, NULL);

    ms = (long long)tv.tv_sec * (long long)1000 + (long long)tv.tv_usec / (long long)1000;
    return ms;
}

int main()
{
	long long ls = 0;
	ls = zx_LocalTime_ms();
	printf("ls = %lld\n",ls);
	return 0;
}