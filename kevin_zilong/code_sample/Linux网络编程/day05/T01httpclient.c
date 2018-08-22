
#include "../header.h"
const char* request = 
"POST /cgi-bin/yue.cgi HTTP/1.1\r\n"
"Host: www.xueguoliang.cn\r\n"
"Connection: keep-alive\r\n"
"Upgrade-Insecure-Requests: 1\r\n"
"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/59.0.3071.115 Safari/537.36\r\n"
"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n"
"DNT: 1\r\n"
"Accept-Encoding: gzip, deflate\r\n"
"Accept-Language: zh-CN,zh;q=0.8\r\n"
"\r\n"
"POS=113.923|23.921&name=xxx&sex=male&age=18";





int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    addr.sin_addr.s_addr = inet_addr("121.40.21.165");

    connect(fd, (struct sockaddr*)&addr, sizeof(addr));

    write(fd, request, strlen(request));


    char buf[8192];

    while(1)
    {
        int ret = read(fd, buf, sizeof(buf));
        if(ret > 0)
        {
            printf("%s", buf);
            fflush(stdout);
        }
        else
            break;
    }
}
