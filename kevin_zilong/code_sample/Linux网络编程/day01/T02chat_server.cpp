
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>

#include <netinet/in.h>
#include <pthread.h>

#include <map>
#include <string>
using namespace std;

typedef struct user_t
{
    string ip;
    int fd;
} user_t;

pthread_mutex_t mutex;
map<string, int> users;

// 一定读到size个字节
// 如果返回值==size表示成功，如果不是则失败
int doRead(int fd, char* buf, int size)
{
    int readlen = 0;
    while(readlen != size)
    {
        int ret = read(fd, buf + readlen, size-readlen);
        if(ret > 0) 
        {
            readlen += ret;
        }
        else if(ret < 0 && errno == EINTR)
        {
            continue;
        }
        else
        {
            break;
        }
#if 0
        {
            if(ret == 0)
                break;
            else  // ret < 0 
            {
                if(errno==EINTR) continue;
                break;
            }
        }
#endif
    }
    return readlen;
}

void* read_thread(void* ptr)
{
    user_t* user = (user_t*)ptr;
    int newfd = user->fd;
    while(1)
    {
        // 接收报文
        uint32_t header;

        int ret = doRead(newfd, (char*)&header, sizeof(header));
        if(ret != sizeof(header))
            break;

        header = ntohl(header); // 转主机序
        printf("%s: send buf len=%d\n", user->ip.c_str(), (int)header);

        char* buf = new char[header+1];
        buf[header] = 0;

        ret = doRead(newfd, buf, header);
        if(ret != header)
            break;

        printf("%s: send buf: %s\n", user->ip.c_str(), buf);

        // buf: ip地址\n消息内容
        char* saveptr;
        char* ip = strtok_r(buf, ":", &saveptr);
        char* msg = strtok_r(NULL, "\0", &saveptr);

        int other_fd;

        pthread_mutex_lock(&mutex);
        if(users.find(ip) != users.end())
        {
            // 转发
            other_fd = users[ip];
        }
        else
        {
            printf("这家伙乱搞\n");
            pthread_mutex_unlock(&mutex);
            delete []buf;
            continue;
        }
        pthread_mutex_unlock(&mutex);
        printf("other fd is %d\n", other_fd);

        printf("%s send %s: %s\n", user->ip.c_str(), ip, msg);

        // [消息长度]ip地址\n消息内容
        header = user->ip.size() + 1 + strlen(msg);
        printf("header is %d\n", (int)header);
        header = htonl(header);

        write(other_fd, &header, 4);
        write(other_fd, user->ip.c_str(), user->ip.size());
        write(other_fd, ":", 1);
        write(other_fd, msg, strlen(msg));

        delete []buf;
    }

    printf("%s offline ....\n", user->ip.c_str());

    close(user->fd);
    pthread_mutex_lock(&mutex);
    users.erase(users.find(user->ip));
    pthread_mutex_unlock(&mutex);
    delete user;
}

int main()
{
    pthread_mutex_init(&mutex, NULL);
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
    if(bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(1);
    }

    // 监听
    listen(fd, 25);

    while(1)
    {
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        // 接收连接，阻塞
        int newfd = accept(fd, NULL, NULL);

        // 得到newfd的ip地址和端口号
        // 得到newfd的对方的ip地址和端口号
        getsockname(newfd, (struct sockaddr*)&addr, &len);
        printf("sockname ip = %s, port=%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

        getpeername(newfd, (struct sockaddr*)&addr, &len);
        printf("peername ip = %s, port=%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

        char* ip = inet_ntoa(addr.sin_addr);
        printf("%s online ....newfd=%d\n", ip, newfd);

        pthread_mutex_lock(&mutex);
        users[ip] = newfd;
        pthread_mutex_unlock(&mutex);

        user_t* user = new user_t;
        user->fd = newfd;
        user->ip = ip;

        // 给users增加一个节点
        // users["连接服务器的客户端的ip地址"] = newfd;

        pthread_t tid;
        pthread_create(&tid, NULL, read_thread, user);
        pthread_detach(tid);
    }
}

