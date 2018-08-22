1．建立socket
        2．将该socket设置为非阻塞模式
        3．调用connect()
        4．使用select()检查该socket描述符是否可写
        5．根据select()返回的结果判断connect()结果
        6．将socket设置为阻塞模式（如果你的程序不需要用阻塞模式，这步就省了，不过一般情况都是用阻塞模式，这样容易管理）
那么根据上面的6个步骤，我们写一个简单的模块程序来调试看下：
{
                int sockfd = socket(AF_INET, SOCK_STREAM, 0);
                if(sockfd < 0) exit(1);
                struct sockaddr_in serv_addr;
                ………//以服务器地址填充结构serv_addr
                int error=-1, len;
                len = sizeof(int);
                timeval tm;
                fd_set set;
                unsigned long ul = 1;
                ioctl(sockfd, FIONBIO, &ul); //设置为非阻塞模式
                bool ret = false;
                if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
                {
                        tm.tv_set = TIME_OUT_TIME;
                        tm.tv_uset = 0;
                        FD_ZERO(&set);
                        FD_SET(sockfd, &set);
                        if( select(sockfd+1, NULL, &set, NULL, &tm) > 0)
                        {
                                getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
                                if(error == 0) ret = true;
                                else ret = false;
                        } else ret = false;
                }
                else ret = true;
                ul = 0;
                ioctl(sockfd, FIONBIO, &ul); //设置为阻塞模式
                //下面还可以进行发包收包操作
                ……………
        }