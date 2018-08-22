
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

//AF_INET相关的定义
#include <netinet/in.h>

int fd;

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

void* thread_func(void* ptr)
{
    while(1)
    {
        uint32_t header;
        int ret = doRead(fd, (char*)&header, sizeof(header));        
        if(ret != sizeof(header))
        {
            exit(1);
        }
        header = ntohl(header);

        char* buf = malloc(header+1);
        buf[header] = 0;

        ret = doRead(fd, buf, header);
        if(ret != header)
        {
            exit(1);
        }

        printf("%s\n", buf); // ip:xxxx
    }
}
int main()
{
    fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(20099); // host --> network short
    addr.sin_addr.s_addr = inet_addr("192.168.22.72");
    connect(fd, (struct sockaddr*)&addr, sizeof(addr));

    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);

    while(1)
    {
        char buf[4096];
        fgets(buf, sizeof(buf), stdin);
        buf[strlen(buf)-1] = 0;
        if(strlen(buf) == 0)
            continue;

        // 192.168.11.72:xx
        uint32_t header = strlen(buf);
        header = htonl(header);

        write(fd, &header, 4);
        write(fd, buf, strlen(buf));
    }

    pthread_join(tid, NULL);
}
