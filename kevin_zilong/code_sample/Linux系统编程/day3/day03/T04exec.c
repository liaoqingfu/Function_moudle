#include "../header.h"
int main()
{
    int fd = open("b.txt", O_WRONLY | O_CREAT | O_CLOEXEC, 0777); // fd == 3
    execl("./write", "./write", NULL);
}
