#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long 	UINT64;

//用select来定时，可以精确到1us
void setTimer(int seconds, int mseconds)
{
        struct timeval temp;

        temp.tv_sec = seconds;
        temp.tv_usec = mseconds;

        select(0, NULL, NULL, NULL, &temp);
        printf("timer\n");

        return ;
}

long long zx_LocalTime_ms(void)   //获取当前时间，ms
{
    struct timeval tv;
    long long ms;

    gettimeofday(&tv, NULL);

    ms = (long long)tv.tv_sec * (long long)1000 + (long long)tv.tv_usec / (long long)1000;
    return ms;
}

UINT64 zx_mgw_CurTime_s(void)			//获取当前时间，秒
{
    struct timeval tv;
    UINT64 time_s; 
    gettimeofday(&tv, NULL);     
    time_s = (UINT64)tv.tv_sec  + (UINT64)tv.tv_usec;

    return time_s;
}