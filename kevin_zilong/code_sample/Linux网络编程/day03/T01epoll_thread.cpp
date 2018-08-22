#include "../header.h"
#include <list>
using namespace std;

static pthread_mutex_t mutex;
static sem_t sem;
static list<int> evfds; // 有事件的文件描述符

static int serverfd;
static int epollfd;

void set_nonblock(int fd)
{
    int flags = fcntl(fd, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, &flags);
}

void epoll_add(int epollfd, int fd, int events)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = events;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

void epoll_del(int epollfd, int fd)
{
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
}

void epoll_mod(int epollfd, int fd, int events)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = events;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}

void* thread_func(void* ptr)
{
    int i = 0;
    while(1)
    {
        // 等待主线程发送sem，告诉子线程有事件发生
        int ret = sem_wait(&sem);
        if(ret < 0 && errno == EINTR)
        {
            continue;
        }
        else if(ret < 0)
        {
            break;
        }

        // 去队列中取一个有事件的文件描述符
        pthread_mutex_lock(&mutex);
        int fd;
        if(evfds.size() > 0)
        {
            fd = *evfds.begin();
            evfds.pop_front();
        }
        else
        {
            pthread_mutex_unlock(&mutex);
            continue;
        }
        pthread_mutex_unlock(&mutex);

        // 处理socket
        if(fd != serverfd)
        {
            char buf[1024];
            int ret = read(fd, buf, sizeof(buf));
            if(ret <= 0)
            {
                close(fd); // 自动将fd从epoll中剔除
              //  epoll_del(epollfd, fd);
            }
            else
            {
                printf("%x:%s:%d\n", (int)pthread_self(), buf, i++);
                // 加回到epoll集合
                // epoll_add(epollfd, fd, EPOLLIN);
                epoll_mod(epollfd, fd, EPOLLIN | EPOLLONESHOT);
            }
        }
        else
        {
            while(1)
            {
                int newfd = accept(fd, NULL, NULL);
                if(newfd < 0 && errno == EAGAIN)
                    break;

                set_nonblock(newfd);
                epoll_add(epollfd, newfd, EPOLLIN | EPOLLONESHOT);
            }
            epoll_mod(epollfd, fd, EPOLLIN | EPOLLONESHOT); // 让epoll再监控这个服务器fd
        }

    }
}

int main()
{
    pthread_mutex_init(&mutex, NULL);
    sem_init(&sem, 0, 0);

    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);
    pthread_create(&tid, NULL, thread_func, NULL);
    pthread_create(&tid, NULL, thread_func, NULL);
    pthread_create(&tid, NULL, thread_func, NULL);


    int fd = socket(AF_INET, SOCK_STREAM, 0);
    serverfd = fd;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9988);
    addr.sin_addr.s_addr = INADDR_ANY;

    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        perror("bind");
        exit(1);
    }

    listen(fd, 250);

    set_nonblock(fd);

    epollfd = epoll_create(1024);
    epoll_add(epollfd, fd, EPOLLIN | EPOLLONESHOT);

    while(1)
    {
        struct epoll_event ev[8];
        int ret = epoll_wait(epollfd, ev, 8, 5000);
        if(ret > 0)
        {
            int i;
            for(i=0; i<ret; ++i)
            {
                int evfd = ev[i].data.fd;
                // 一旦socket有消息，那么把socket从epoll删除，避免重复通知
                // epoll_del(epollfd, evfd);

                pthread_mutex_lock(&mutex);
                // 丢到队列中去
                evfds.push_back(evfd);
                pthread_mutex_unlock(&mutex);
                sem_post(&sem);
            }
        }
    }
}

