
#include "../header.h"
int main()
{
    int fd = open("a.txt", O_RDONLY);

    lseek(fd, -10, SEEK_END);

    char buf[6] = {0};
    read(fd, buf, 5);

    printf("%s\n", buf);
}
