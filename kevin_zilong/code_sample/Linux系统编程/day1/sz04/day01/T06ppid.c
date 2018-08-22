
#include <stdio.h>
#include <unistd.h>
int main()
{
    pid_t parentid = getpid();

    pid_t pid = fork();
    if(pid == 0) // child 
    {
        pid_t ppid = getppid(); // ppid == parentid
        printf("%d, %d\n", (int)ppid, (int)parentid);
        return 1;
    }

    sleep(1);

    return 0;
}
