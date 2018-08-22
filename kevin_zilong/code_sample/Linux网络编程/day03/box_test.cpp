
#include "box2.h"
thread_local int i=0;
void doRead(int fd)
{
    char buf[1024];
    printf("do read\n");
    int ret = read(fd, buf, sizeof(buf));
    printf("do read ok ret=%d\n", ret);
    if(ret <= 0)
    {
        box_close(fd);
        return;
    }
    printf("buf=%s i=%d\n", buf, i++);
    box_setnotify(fd, doRead);
}

void doAccept(int fd)
{
    printf("doAccept begin\n");
    while(1)
    {
        int newfd = accept(fd, NULL, NULL);
        if(newfd < 0 && errno == EAGAIN)
            break;

        box_set_nonblock(newfd);
        box_setnotify(newfd, doRead);
    }

    printf("do accept end\n");
    box_setnotify(fd, doAccept);
}

int main()
{
    box_init();

    int fd = box_create_server(9988);
    box_setnotify(fd, doAccept);

    box_run();
}
