
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>

//AF_INET相关的定义
#include <netinet/in.h>

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(20099); // host --> network short
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(fd, (struct sockaddr*)&addr, sizeof(addr));

    // read，write和管道的read write行为一致
    write(fd, "hello server", 13);

    char buf[1024];
    read(fd, buf, sizeof(buf));

    printf("%s\n", buf);

}
