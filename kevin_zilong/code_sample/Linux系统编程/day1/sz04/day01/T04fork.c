
#include <stdio.h>
int a = 0;
int main()
{
    a = 2 + 3;
    fork();
    fork();
    printf("hello world\n");
    return 0;
}
