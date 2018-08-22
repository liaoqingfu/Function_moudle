#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/select.h>
#include <errno.h>

long long zx_LocalTime_ms(void)
{
    struct timeval tv;
    long long ms;

    gettimeofday(&tv, NULL);

    ms = (long long)tv.tv_sec * (long long)1000 + (long long)tv.tv_usec / (long long)1000;
    return ms;
}