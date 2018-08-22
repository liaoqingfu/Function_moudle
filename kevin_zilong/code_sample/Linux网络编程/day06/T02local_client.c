#include "../header.h"
#include <sys/un.h>
int main()
{
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "sock_file");

    connect(fd, (struct sockaddr*)&addr, sizeof(addr));

    write(fd, "hello", 6);
}
