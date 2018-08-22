
#include <stdlib.h>
#include <stdio.h>

// C库中已经定义这个变量，所以直接extern出来即可
extern char** environ;

// 打印所有的环境变量
int main()
{
    unsetenv("HOME");
    int i;
    for(i=0; ; ++i)
    {
        if(environ[i] == NULL) break;

//        printf("%s\n", environ[i]);
    }

    char* home = getenv("HOME");
    printf("%s\n", home);

}
