#include "../header.h"
int main()
{
    pid_t pid = fork();
    if(pid > 0) // parent
    {
        printf("wait child exit\n");
        // 等待子进程结束
        // 等待任意一个子进程退出
        // 返回值是退出的那个子进程ID
        // wait得到了子进程的运行结果之后，那么子进程的PCB没用了，就释放了
        int status;
        pid = wait(&status); // status得到的是子进程退出的状态 
        int retcode = WEXITSTATUS(status);
        printf("child exit, status=%d\n", retcode);
    }
    else if(pid == 0) // child
    {
        sleep(1);
        return 99;
    }
}
