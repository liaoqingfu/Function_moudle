#include "../header.h"


int a;
int main()
{
    a = 10;
    pid_t pid = fork();
    if(pid > 0)
    {
        a = 20;
        // pid is child process id
        // in parent
    }
    else if(pid == 0)
    {
        sleep(1);
        printf("a=%d\n", a); // 10 or 20, is 10
        // in child
        pid_t parentid = getppid();
    }
    else
    {
        // fork failure
    }
}
