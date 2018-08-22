
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

// char* foo();
//

int main()
{

    int fd = open("a.txt", O_RDONLY);
    if(fd < 0)
    {
//        printf("errno is %d, %s\n", errno, strerror(errno)); // errno 是一个全局变量
        perror("open");   // open: err string\n
    }
}
