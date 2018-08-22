#define _MUL_TIMER_MAIN


#ifdef _MUL_TIMER_MAIN


#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <time.h> 
#include "mul_timer.h"

timer_handle_t hdl[3], call_cnt = 0;
int timer_proc1(void *arg, int len)
{
    char tstr[200];
    static int i, ret;
    
    get_format_time(tstr);
    printf("hello %s: timer_proc1 is here./n", tstr);
    if(i >= 5)
    {
        get_format_time(tstr);
        ret = del_a_timer(hdl[0]);
        printf("timer_proc1: %s del_a_timer::ret=%d/n", tstr, ret);
    }
    i++;
    call_cnt++;
    
    
    return (1);
}

int timer_proc2(void * arg, int len)
{
    char tstr[200];
    static int i, ret;
    
    get_format_time(tstr);
    printf("hello %s: timer_proc2 is here./n", tstr);
    if(i >= 5)
    {
        get_format_time(tstr);
        ret = del_a_timer(hdl[2]);
        printf("timer_proc2: %s del_a_timer::ret=%d/n", tstr, ret);
    }
    i++;
    call_cnt++;
    
    return (1);
}


int main(void)
{
    char arg[50];
    char tstr[200];
    int ret;
    
    init_mul_timer();
    hdl[0] = set_a_timer(2, timer_proc1,1, NULL, 0);
    printf("hdl[0]=%d/n", hdl[0]);
    hdl[1] = set_a_timer(3, timer_proc2,2, arg, 50);
    printf("hdl[1]=%d/n", hdl[1]);
    hdl[2] = set_a_timer(3, timer_proc2,3, arg, 101);
    printf("hdl[1]=%d/n", hdl[2]);
    while(1)
    {
        if(call_cnt >= 12)
        {
            get_format_time(tstr);
            ret = destroy_mul_timer();
            printf("main: %s destroy_mul_timer, ret=%d/n", tstr, ret);
            call_cnt++;
        }
        if(call_cnt >= 20)
        {
            break;
        }
    }
    
    return 0;
}

#endif