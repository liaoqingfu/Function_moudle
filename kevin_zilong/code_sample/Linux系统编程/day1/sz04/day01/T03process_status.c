
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>


// main函数是进程的入口
int main(int argc, char* argv[])
{
    // 获取进程的进程号，类型是pid_t，在Linux中其实就是一个整数
//    pid_t pid = getpid();

//    printf("pid is %d\n", (int)pid);

    execl("./helloworld", "./helloworld", NULL);
    printf("hello world2\n");
}
