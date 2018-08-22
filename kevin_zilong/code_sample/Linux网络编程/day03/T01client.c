
#include "../header.h"
int main1()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9988);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(fd, (struct sockaddr*)&addr, sizeof(addr));

    write(fd, "hello", 6);
}

int main()
{
    int i;
    int n = 10000;
    for(i=0; i<n; ++i)
    {
        pid_t pid = fork();
        if(pid == 0)
        {
            main1();
            exit(1);
        }
    }
    for(i=0; i<n; ++i)
    {
        wait(NULL);
    }
}
