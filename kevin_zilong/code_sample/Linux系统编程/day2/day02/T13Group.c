#include "../header.h"
int main()
{
    pid_t pid = fork();
    if(pid > 0)
        return 0; // parent exit

    if(pid == 0)
    {
        // 闹独立
//        setpgid(getpid(), getpid());
    }

    while(1)
    {
        sleep(1);
    }
}

