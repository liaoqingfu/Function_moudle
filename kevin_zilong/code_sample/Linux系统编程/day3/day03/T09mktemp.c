
#include "../header.h"
int main()
{
    char buf[] = "/tmp/XXXXXXworld";
//    char* buf = "/tmp/XXXXXX";
    int fd = mkostemps(buf, 5, O_APPEND|O_WRONLY|O_CREAT);
    if(fd < 0)
        perror("temp");

    printf("%s\n", buf);
}
