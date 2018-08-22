
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>

#include <netinet/in.h>

int main()
{
    // 在Linux系统中，socket使用文件描述符来表示
    // AF_INET: ipv4互联网协议族
    // SOCK_STREAM: 流类型的SOCKET，TCP协议
    // 0：TCP/IP协议
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(20099);
    addr.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY的值是0

    // fd socket在端口20099上绑定
    bind(fd, (struct sockaddr*)&addr, sizeof(addr));

    // 监听
    listen(fd, 25);

    // 接收连接，阻塞
    int newfd = accept(fd, NULL, NULL);


    write(newfd, "hello client", 13);


    char buf[1024];
    // 阻塞
    read(newfd, buf, sizeof(buf));

    printf("%s\n", buf);

}
