
#include "../header.h"

extern char** environ;
int main()
{
    int i=0;
    for(i=0; ;++i)
    {
        if(environ[i] == NULL) break;
        printf("%s\n", environ[i]);
    } 
    return 0;
}
