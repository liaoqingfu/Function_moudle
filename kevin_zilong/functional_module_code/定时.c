#include <stdio.h>  
#include <signal.h>  
#include <sys/time.h>  
  
void signalHandler(int signo)  
{  
    switch (signo){  
        case SIGALRM:  
            printf("Caught the SIGALRM signal!\n");  
            break;  
   }  
}  
  
int main(int argc, char *argv[])  
{  
    signal(SIGALRM, signalHandler);  
  
    struct itimerval new_value, old_value;  
    new_value.it_value.tv_sec = 0;  
    new_value.it_value.tv_usec = 1;  
    new_value.it_interval.tv_sec = 0;  
    new_value.it_interval.tv_usec = 200000;  
    setitimer(ITIMER_REAL, &new_value, &old_value);  
      
    for(;;);  
       
    return 0;  
} 









#include <stdio.h>  
#include <time.h>  
#include <sys/time.h>  
#include <stdlib.h>  
#include <signal.h>  
  
static int count = 0;  
static struct itimerval oldtv;  
  
void set_timer()  
{  
    struct itimerval itv;  
    itv.it_interval.tv_sec = 1;  
    itv.it_interval.tv_usec = 0;  
    itv.it_value.tv_sec = 1;  
    itv.it_value.tv_usec = 0;  
    setitimer(ITIMER_REAL, &itv, &oldtv);  
}  
  
void signal_handler(int m)  
{  
    count ++;  
    printf("%d\n", count);  
}  
  
int main()  
{  
    signal(SIGALRM, signal_handler);  
    set_timer();  
    while(count < 10000);  
    exit(0);  
    return 1;  
}