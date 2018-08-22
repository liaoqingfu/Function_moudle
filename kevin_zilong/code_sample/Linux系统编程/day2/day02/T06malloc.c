
#include "../header.h"

void foo()
{
    exit(10); // 
}

int main()
{
    int fd = open("a.txt", O_RDONLY);
    char* p = malloc(100);

    foo();


    return 99;
}
