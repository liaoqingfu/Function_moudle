
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "../header.h"

int a;

int main()
{
    int fd = open("a.txt", O_RDWR);
    a = 10;
    int b = 10;
    char* p = malloc(100);
    strcpy(p, "hello");
    
    void* ptr = mmap(NULL, // 映射目标地址，如果是NULL，就让系统决定
            4096, // 映射长度，要求被映射的文件有这个长度内容
            PROT_READ|PROT_WRITE, // 映射权限，有读、写、执行
            MAP_SHARED, // 将数据的改变，反映得到对文件
            fd, // 对应的文件描述符
            0); // 映射偏移

    pid_t pid = fork();
    if(pid == 0)
    {
        strcpy((char*)ptr, "yyyyyyyyyyyyyyyyy");
        a = 20;
        b = 20;
        strcpy(p, "world");
    }
    else if (pid > 0)
    {
        sleep(1);
        printf("%s\n", (char*)ptr); // yyyyyyyyyyyyyyyy
        printf("a is %d\n", a); // 10
        printf("a is %d\n", b); // 10
        printf("p is %s\n", p); // hello
    }

    close(fd);
}
