#include <sys/times.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
int main()
{
    int a = 5;
    int b = 6;
    struct tms t1;
    struct tms t2;

    times(&t1);

    // 得到现在进程花了多少时间
    int c = a + b;
    
    for(int i=0; i<1000*1000; ++i)
    {
     //   c = a+b;
        int fd = open("a.txt", O_WRONLY);
        close(fd);
    } 
    // 1000行代码

    // 得到进程花了多少时间
    times(&t2);

    printf("utime=%d, stime=%d\n", (int)(t2.tms_utime - t1.tms_utime),
            (int)(t2.tms_stime - t1.tms_stime));

}
