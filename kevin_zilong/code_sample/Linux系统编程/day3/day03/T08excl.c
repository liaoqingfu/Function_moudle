#include "../header.h"


int main()
{
    int fd = open("a.txt", O_CREAT|O_WRONLY, 0777);
    if(fd < 0)
    {
        perror("open");
        return 0;
    }

    printf("open ok\n");

    getchar();

    printf("write\n");

    write(fd, "hello", 5);
    close(fd);
}
