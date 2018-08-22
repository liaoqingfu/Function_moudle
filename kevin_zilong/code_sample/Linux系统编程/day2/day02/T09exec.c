
#include "../header.h"
int main()
{
    char* env[] = {"aaa=bbb", "ccc=ddd", NULL};
    execle("./t08", "./t08", NULL, env);
}
