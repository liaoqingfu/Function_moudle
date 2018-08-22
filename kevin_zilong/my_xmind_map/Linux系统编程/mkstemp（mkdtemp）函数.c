#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
void test01()
{
//临时文件只是存放临时用的，重启系统，文件就会清除的
    char buf[]="kevin-XXXXXX"; //前面是固定的，后面6个必须是6个X(大写)，系统会自动生成6个字符，保证临时文件不会重名
    char buf1[]="/tmp/kevin-XXXXXX"; //如果需要将文件写入文件夹的话，需要增加文件路径,这样在相应的tmp文件夹下就会有个临时文件的
   // char *p=mktemp(buf);//生成了一个临时文件p,文件名为：kevin-后面是系统自动分配,系统会提示告警，不安全的
    char *p=mkdtemp(buf);//这个不会告警 或者mkstemp
    char *p1=mkdtemp(buf1);//这个不会告警，或者mkstemp
    printf("p=%s\n",p);
    printf("p1=%s\n",p1);
}

int main()
{
    test01();
    return 0;
}
