
#include "../header.h"

int main()
{
    int fd = open("a.txt", O_WRONLY);

    pid_t pid = fork();
    if(pid == 0)
    {
        write(fd, "hello", 5);
    }
    else if(pid > 0)
    {
        write(fd, "world", 5);
        wait(NULL);
    }
}

// hello A
// world B
// helloworld C
// worldhello D
