#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
extern char **environ; //声明全局
void test()
{
    char *path = getenv("PATH");
    printf("%s\n",path);
}

int main()
{
    test();
    return 0;
}
