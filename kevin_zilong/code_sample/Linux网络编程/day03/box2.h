#ifndef BOX2_H
#define BOX2_H

#include "../header.h"

typedef void(*box_notify)(int fd);

void box_init(int thread_count = 0);
void box_run();

int box_create_server(uint16_t port, const char* ip = "0.0.0.0");
void box_setnotify(int fd, box_notify read_cbk, box_notify write_cbk = NULL);

void box_close(int fd);
void box_set_nonblock(int fd);


#endif // BOX2_H
