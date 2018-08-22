
// /usr/local
#include <event2/event.h>
#include <event2/listener.h>
#include <netinet/in.h>
#include <event2/bufferevent.h>
#include <string.h>
#include <arpa/inet.h>
#include <event2/buffer.h>

#include <string>
#include <list>
using namespace std;

typedef struct user_t
{
    bufferevent* bev;
    string ip;
    uint32_t header;
} user_t;

list<user_t*> users;


void readheader(struct bufferevent *bev, void *ctx);
void readdata(struct bufferevent *bev, void *ctx);
void eventcb(struct bufferevent *bev, short what, void *ctx);

void del_user(struct bufferevent* bev)
{
    for(auto it = users.begin(); it!=users.end(); ++it)
    {
        user_t* user = *it;
        if(user->bev == bev)
        {
            users.erase(it);
            break;
        }
    }
}

user_t* find_user(struct bufferevent* bev)
{
    for(auto it = users.begin(); it != users.end(); ++it)
    {
        user_t* user = *it;
        if(user->bev == bev)
            return user;
    }

    return NULL;
}
user_t* find_user(string ip)
{
    for(auto it = users.begin(); it != users.end(); ++it)
    {
        user_t* user = *it;
        if(user->ip == ip)
            return user;
    }

    return NULL;
}

void readdata(struct bufferevent *bev, void *ctx)
{
    user_t* user = find_user(bev);
    char* data = new char[user->header+1];
    data[user->header] = 0;
    bufferevent_read(bev, data, user->header);

    // ip:msg
    char* ip = strtok(data, ":");
    char* msg = strtok(NULL, "\0");

    user_t* other = find_user(ip);

    uint32_t length = user->ip.size() + 1 + strlen(msg);
    length = htonl(length);
    bufferevent_write(other->bev, &length, 4);
    bufferevent_write(other->bev, user->ip.c_str(), user->ip.size());
    bufferevent_write(other->bev, ":", 1);
    bufferevent_write(other->bev, msg, strlen(msg));

    delete data;

    struct evbuffer* input =  bufferevent_get_input(bev);
    length = evbuffer_get_length(input);
    if(length >= 4)
    {
        readheader(bev, ctx);
    }
    else
    {
        bufferevent_setwatermark(bev, EV_READ, 4, 0);
        bufferevent_setcb(bev, readheader, NULL, eventcb, ctx);
        bufferevent_enable(bev, EV_READ|EV_WRITE);
    }
}


void readheader(struct bufferevent *bev, void *ctx)
{
    user_t* user = find_user(bev);

    uint32_t header;
    bufferevent_read(bev, &header, sizeof(header));
    header = ntohl(header);
    user->header = header;
    printf("user->header=%d\n", (int)header);

    struct evbuffer* input =  bufferevent_get_input(bev);
    int length = evbuffer_get_length(input);
    if(length >= header)
    {
        readdata(bev, ctx);
    }
    else
    {
        bufferevent_setwatermark(bev, EV_READ, header, 0);
        bufferevent_setcb(bev, readdata, NULL, eventcb, ctx);
        bufferevent_enable(bev, EV_READ|EV_WRITE);
    }
}

void eventcb(struct bufferevent *bev, short what, void *ctx)
{
    printf("error %d\n", (int)what);
    del_user(bev);
    bufferevent_free(bev);
}

void doAccept(struct evconnlistener * server,
              evutil_socket_t newfd,   // libevent已经自动调用accept，得到连接的socket
              struct sockaddr *addr,
              int socklen,
              void *ptr)
{
    printf("accepting...\n");
    struct event_base* base = (struct event_base*)ptr;

    // 将newfd放入集合，相当与epoll_add
    // yichushihua
    struct bufferevent * bev = bufferevent_socket_new(base,
                                                      newfd,
                                                      BEV_OPT_CLOSE_ON_FREE);

    struct sockaddr_in* addr_in = (struct sockaddr_in*)addr;
    user_t* user = new user_t;
    user->bev = bev;
    user->ip = inet_ntoa(addr_in->sin_addr);
    users.push_back(user);


    bufferevent_setwatermark(bev, EV_READ, 4, 0);

    // 设置回调函数，可以设置三个回调函数
    bufferevent_setcb(bev, readheader, NULL, eventcb, base);
    // 设置这个socket能做什么，设置了read和write
    bufferevent_enable(bev, EV_READ|EV_WRITE);


    printf("accept ...\n");
}


int main()
{
    // 类似box_init，初始化一个集合叫base
  //  event_config_avoid_method("epoll");
    struct event_base* base = event_base_new();

   // event_set_mem_functions(mymalloc, myrealloc, myfree);

    int flags =  LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9988);
    addr.sin_addr.s_addr = INADDR_ANY;

    // 创建一个监听的服务器，并且加入集合base，有连接回调doAccept
    struct evconnlistener * server =  // yi chushihua --> pending
            evconnlistener_new_bind(
                base, // 要加入的集合
                doAccept,
                base, // 回调参数
                flags,
                250,
                (struct sockaddr*)&addr,
                sizeof(addr));

    // 类似box_run，进入死循环
    event_base_dispatch(base);
}
