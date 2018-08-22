#include "../header.h"

int main()
{
    struct stat stat_buf;
    stat("a.out", &stat_buf); // 将a.txt的所有文件属性放入stat_buf

    printf("%d, %d, %d\n", (int)stat_buf.st_size/1024,
    (int)stat_buf.st_atime,
    (int)stat_buf.st_blksize);

    if(S_IRUSR & stat_buf.st_mode)
    {
        printf("user can read\n");
    }

    if(S_ISREG( stat_buf.st_mode))
    {
        printf("this is regular file\n");
    }
}
