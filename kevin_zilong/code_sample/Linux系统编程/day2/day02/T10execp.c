
#include "../header.h"
int main()
{
    // execl 要求使用绝对路径或者相对路径
    // execlp p表示环境变量的PATH，有了p，指定的程序会去环境变量中查找
    execlp("./t08", "./t08", NULL);

    // execlp也可以使用相对路径和绝对路径，这个效果和execl是一样的
}
