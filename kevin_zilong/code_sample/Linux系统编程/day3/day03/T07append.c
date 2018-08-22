
#include "../header.h"
int main()
{
    int fd = open("a.txt", O_WRONLY|O_CREAT|O_TRUNC, 0777);
    
    // fcntl修改文件描述符的flags的   file control
    int flags = fcntl(fd, GETFL);
    flags |= O_APPEND;  //增加
   // flags &= ~O_APPEND; // 删除某种flag
    fcntl(fd, SETFL, flags);

    // 这样子是错误的 
    // fcntl(fd, SETFL, O_APPEND);

    write(fd, "hello", 5);
    lseek(fd, 0, SEEK_SET);
    write(fd, "world", 5);
}
