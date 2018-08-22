
#include "../header.h"

int main()
{
    int fd1 = open("a.txt", O_RDONLY);
    if(fd1 == -1)
    {
        perror("open read");
        return 0;
    }

    int fd2 = open("b.txt", O_WRONLY | O_TRUNC | O_CREAT, 0777);
    if(fd2 == -1)
    {
        perror("open write");
        return 0;
    }

#if 0
    int len = lseek(fd1, 0, SEEK_END);
    char* buf = malloc(len);
    lseek(fd1, 0, SEEK_SET);

    read(fd1, buf, len);
    write(fd2, buf, len);
#endif

    char buf[1024];

    while(1)
    {
        int readlen = read(fd1, buf, sizeof(buf));
        if(readlen == 0) // 读的长度是0，则表示读到文件末尾
            break;
        write(fd2, buf, readlen);
    }

    close(fd1);
    close(fd2);
}
