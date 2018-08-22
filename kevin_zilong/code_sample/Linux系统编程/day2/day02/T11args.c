#include "../header.h"
#include <stdarg.h>
#if 0
int add(int a, int b)
{
    return a+b;
}
#endif

int add(int a, ...)
{
    int sum = a;
    va_list ap; // 可以看作是一个指针
    va_start(ap, a); // 得到首地址

    while(1)
    {
        int arg = va_arg(ap, int); // 在ap的当前位置得到参数，并把ap指针往后移动
        if(arg == 0)
            break;
        sum += arg;
    }

    va_end(ap); // 释放

    return sum;
}

void __xLog(const char* filename, int line, const char* fmt, ...)
{
    char buf[4096];
    va_list ap;
    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);

    printf("[%s, %d] %s", filename, line, buf);
}

#define xLog(fmt, ...) __xLog(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

int main()
{
    // 用0来做结束标记，结束标记在计算机中叫哨兵
    int ret = add(10, 20, 30, 40, 50, 0);
    printf("%d\n", ret);

    xLog("%d\n", ret); 
    xLog("hello world\n");
}
