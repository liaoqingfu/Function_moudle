
#include "../header.h"
int main()
{
#if 0
    // fd就是文件描述符
    for(int i=0; ;++i)
    {
        int fd = open("a.txt", O_WRONLY);
        printf("fd=%d, i=%d\n", fd, i);
        if(fd < 0)
        {
            break;
        }
    }
#endif

    // 0 ~ stdin
    // 1 ~ stdout
    // 2 ~ stderror
    int fd = open("a.txt", O_WRONLY); // fd == 3
    int fd1 = open("a.txt", O_WRONLY); // fd == 4
    close(fd);
    int fd2 = open("a.txt", O_WRONLY); // fd == 3

}
