#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
int main()
{
    pid_t pid=getpid();
    pid_t ppid=getppid();
    printf("%d\n",(int)pid);
    printf("%d,%d\n",(int)pid,(int)ppid);
    return 0;
}
