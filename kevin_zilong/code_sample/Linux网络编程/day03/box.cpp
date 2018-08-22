
#include <map>
#include <unistd.h>
#include <string.h>
#include "box.h"
#include <sys/epoll.h>
#include <fcntl.h>
using namespace std;

static int epollfd = 0;
static int maxfd = 0;
static fd_set set;
static fd_set wset;

static map<int, box_notify> socks;
static int type;


void box_init(int t)
{
    type = t;
    if(type == BOX_EPOLL)
        epollfd = epoll_create(1024); // create epoll
    else if(type == BOX_SELECT)
    {
        FD_ZERO(&set);
        FD_ZERO(&wset);
    }
}

int box_create_server(uint16_t port, const char* ip)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        printf("bind error\n");
        exit(1);
    }
    listen(fd, 250);

    return fd;
}

void box_set_notify(int fd, box_notify notify)
{
    socks[fd] = notify; 
    box_set_nonblock(fd);

    if(type == BOX_SELECT)
    {
        if(maxfd < fd) maxfd = fd;
        FD_SET(fd, &set);
    }
    else if(type==BOX_EPOLL)
    {
        struct epoll_event ev;
        ev.data.fd = fd;
        ev.events = EPOLLIN;
        epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    }
}

void box_close(int fd)
{
    auto it = socks.find(fd);
    if(it == socks.end())
        return;

    if(type == BOX_SELECT)
    {
        FD_CLR(fd, &set);
        if(fd == maxfd) maxfd --;
    }

    close(it->first);
    socks.erase(it);
}


void box_select_run()
{
    while(1)
    {
        fd_set tmp_set;
        memcpy(&tmp_set, &set, sizeof(set)); // bitmap
        struct timeval tv;
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        int ret = select(maxfd+1, &tmp_set, NULL, NULL, &tv);
        printf("ret is %d\n", ret);
        if(ret > 0)
        {
            for(auto it=socks.begin(); it!=socks.end(); ++it)
            {
                int fd = it->first;
                if(FD_ISSET(fd, &tmp_set))
                {
                    printf("fd is %d\n", fd);
                    it->second(fd);
                }
            }
        }
        else if(ret < 0 && errno != EINTR)
        {
            printf("errot select\n");
            break;
        }
    }
}

void box_epoll_run()
{
    struct epoll_event ev[8];
    while(1)
    {
        int ret = epoll_wait(epollfd, ev, 8, 5000);
        if(ret > 0)
        {
            for(int i=0; i<ret ;++i)
            {
                int fd = ev[i].data.fd;
                box_notify notify = socks[fd];
                notify(fd);
            }
        }
        else if(ret == 0 || (ret < 0 && errno == EINTR))
        {}
        else
        {
            printf("epoll_wait error\n");
            exit(1);
        }
    }
}


void box_run()
{
    if(type == BOX_EPOLL)
        box_epoll_run();
    else if(type == BOX_SELECT)
        box_select_run();
}


void box_set_nonblock(int fd)
{
    int flags;
    flags = fcntl(fd, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flags);
}

void box_set_write_notify(int fd, box_notify cbk)
{
    socks[fd] = cbk;

    if(type == BOX_EPOLL)
    {
        struct epoll_event ev;
        ev.data.fd = fd;
        ev.events = EPOLLOUT;
        if(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) < 0)
        {
            epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
        }
    }
}









