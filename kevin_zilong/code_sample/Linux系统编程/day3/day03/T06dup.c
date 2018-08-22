
#include "../header.h"
int main()
{

    printf("hello world\n"); // stdout // write(1, "hello world", 11);
    fflush(stdout);

    int stdout_backup = dup(1); // 文件1拷贝到某个位置保存  3

    int fd = open("a.txt", O_TRUNC|O_CREAT|O_WRONLY, 0777);  // 4
    dup2(fd, 1);  // 将标准输出重定向到a.txt
    close(fd); // 关闭4
    printf("hello world1"); // a.txt

    // 在切换之间，要刷新缓冲区
    fflush(stdout);
    
    dup2(stdout_backup, 1);
    printf("hello world2\n"); // stdout
}
