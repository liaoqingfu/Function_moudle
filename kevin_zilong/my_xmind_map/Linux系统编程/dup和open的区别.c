#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
void test01()
{
    int fd=open("./c.txt",O_RDONLY); //以只读的方式打开文件
    char buf[512]={0};
    read(fd,buf,8);//读文件的内容，读到buf里面
    printf("%s\n",buf);  //输出文件的前面八个字符
    printf("fd=%d\n",fd); // 3
    fd= open("./c.txt",O_RDONLY); //每次打开，都会新建立一个文件描述结构
    read(fd,buf,8);//新的文件描述结构，所以只能读到前面8个字符
    printf("%s\n",buf);  //输出文件的前面八个字符
    printf("fd=%d\n",fd);//4
    close(fd); //这样会丢失掉一个文件描述符，打开文件最好就得关闭
}


void test02()
{
    int fd=open("./c.txt",O_RDONLY); //以只读的方式打开文件
    char buf[512]={0};
    read(fd,buf,8);//读文件的内容，读到buf里面
    printf("%s\n",buf);  //输出文件的前面八个字符
    printf("fd=%d\n",fd); // 3
    int flg=dup(fd);//复制文件描述符fd给flg，flg=4，但是也是指向同一个文件描述结构，操作flg就是操作文件本身
   // fd= open("./c.txt",O_RDONLY); //每次打开，都会新建立一个文件描述结构
    read(flg,buf,8);//不会重新创建新的文件描述结构，可以读到文件后面8个字符后面的8个字符
    printf("%s\n",buf);  //输出文件8个字符后面的8个字符
    printf("fd=%d\n",flg); // 4
    close(fd);
    close(flg);
}

int main()
{
   // test01();
    test02();
    return 0;
}
