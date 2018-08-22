#include "../header.h"
int main()
{
    pid_t pid = fork();
    if(pid == 0)
    {
        setsid(); // 闹独立，组长不能闹独立
        while(1)
        {
            sleep(1);
        }
    }
}
