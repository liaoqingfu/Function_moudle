#include "../header.h"
#include <sys/un.h>

int main()
{
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);

    unlink("sock_file");

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "sock_file");

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
