
#ifndef __BOX_H__
#define __BOX_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/types.h>

typedef void(*box_notify)(int fd);

#define BOX_EPOLL 1
#define BOX_SELECT 2

void box_init(int type = BOX_EPOLL);
void box_run();
int box_create_server(uint16_t port, const char* ip = "0.0.0.0");

void box_set_notify(int fd, box_notify cbk);
#define box_set_read_notify box_set_notify

void box_close(int fd);
void box_set_nonblock(int fd);
void box_set_write_notify(int fd, box_notify cbk);



#endif
