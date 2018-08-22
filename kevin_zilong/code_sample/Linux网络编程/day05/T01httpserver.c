#include "../header.h"

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9988);
    addr.sin_addr.s_addr = INADDR_ANY;

    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        perror("bind");
        return 0;
    }

    listen(fd, 250);

    int newfd = accept(fd, NULL, NULL);

    char buf[4096];

    while(1)
    {
        memset(buf, 0, sizeof(buf));
        int ret = read(newfd, buf, sizeof(buf));
        if(ret > 0)
        {
            printf("%s", buf);
            fflush(stdout);
        }
    }
}
