#include "box2.h"
#include <map>
#include <list>
using namespace std;

static int g_thread_count ;
static int g_epollfd;

static map<int, box_notify> read_cbks;
static map<int, box_notify> write_cbks;

typedef struct box_ev
{
    int fd;
    int events;
} box_ev;

static list<box_ev*> evs;
static pthread_mutex_t mutex;
static sem_t sem;

static void box_callback(int fd, int events)
{
    printf("events=%x, EPOLLIN=%x, EPOLLOUT=%x\n", events, EPOLLIN, EPOLLOUT);
    if(events & EPOLLIN)
    {
        printf("read callback\n");
        read_cbks[fd](fd);
    }
    if(events & EPOLLOUT)
    {
        write_cbks[fd](fd);
    }
}
static void* thread_func(void* ptr)
{
    while(1)
    {
        int ret = sem_wait(&sem);
        if(ret == 0)
        {
            box_ev* ev ;
            pthread_mutex_lock(&mutex);
            if(evs.size() > 0)
            {
                ev = *evs.begin();
                evs.pop_front();
            }
            else
            {
                pthread_mutex_unlock(&mutex);
                continue;
            }
            pthread_mutex_unlock(&mutex);

            box_callback(ev->fd, ev->events);
            delete ev;
        }
        else if(ret < 0 && errno ==EINTR)
        {
            continue;
        }
        else
        {}
    }
}

void box_init(int thread_count)
{
        g_thread_count = thread_count;
        g_epollfd = epoll_create(1024);
        if(thread_count > 0)
        {
            sem_init(&sem, 0, 0);
            pthread_mutex_init(&mutex, NULL);
            for(int i=0; i<thread_count; ++i)
            {
                pthread_t tid;
                pthread_create(&tid, NULL, thread_func, NULL);
            }
        }
}

int box_create_server(uint16_t port, const char* ip)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    box_set_nonblock(fd);
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

void box_setnotify(int fd, box_notify read_cbk, box_notify write_cbk)
{
    int events = EPOLLONESHOT;
    if(read_cbk)
    {
        read_cbks[fd] = read_cbk;
        events |= EPOLLIN;
    }

    if(write_cbk)
    {
        write_cbks[fd] = write_cbk;
        events |= EPOLLOUT;
    }

    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = events;
    if(epoll_ctl(g_epollfd, EPOLL_CTL_MOD, fd, &ev) < 0) // mod failure
    {
        epoll_ctl(g_epollfd, EPOLL_CTL_ADD, fd, &ev);
    }
}

void box_run()
{
    struct epoll_event ev[8];
    while(1)
    {
        int ret = epoll_wait(g_epollfd, ev, 8, 5000);
        printf("ret is %d\n", ret);
        if(ret > 0)
        {
            for(int i=0; i<ret; ++i)
            {
                int events = ev[i].events;
                int fd = ev[i].data.fd;

                if(g_thread_count == 0) // single thread
                {
                    printf("main thread callback\n");
                    box_callback(fd, events);
                }
                else // multi threads
                {
                    printf("threads callback\n");
                    box_ev* bev = new box_ev;
                    bev->events = events;
                    bev->fd = fd;

                    pthread_mutex_lock(&mutex);
                    evs.push_back(bev);
                    pthread_mutex_unlock(&mutex);
                    sem_post(&sem);
                }
            }
        }
    }
}

void box_set_nonblock(int fd)
{
    int flags;
    flags = fcntl(fd, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flags);
}

void box_close(int fd)
{
    {
        auto it = read_cbks.find(fd);
        if(it != read_cbks.end())
            read_cbks.erase(it);
    }

    {
        auto it = write_cbks.find(fd);
        if(it != write_cbks.end())
            write_cbks.erase(it);
    }

    close(fd);
}


