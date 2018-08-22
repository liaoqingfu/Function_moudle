#include "../header.h"
int main()
{
    if(chroot("/home/xueguoliang") == -1)
        perror("chroot");


    int fd = open("/a.txt", O_WRONLY);
    write(fd, "hello", 5);
    close(fd);

}
