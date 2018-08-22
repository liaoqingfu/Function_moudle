#include "../header.h"
#include <sys/un.h>

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9988);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 

    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        perror("bind");
        return 0;
    }

    listen(fd, 10);

    int newfd = accept(fd, NULL, NULL);

    char buf[1024];
    ret = read(newfd, buf, sizeof(buf));
    printf("%d, %s\n", ret, buf);

}
