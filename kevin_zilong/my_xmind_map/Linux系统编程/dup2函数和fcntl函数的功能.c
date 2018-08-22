#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>

void test01()
{
    int fd=open("./b.txt",O_WRONLY); //以只写的方式打开文件
    int flg=fcntl(fd,F_GETFL);  //调用fcntl函数，通过F_GETFL得到文件打开方式
    flg =flg|O_APPEND;  //通过位或运算，将flg二进制改为 可写和追加的方式
    fcntl(fd,F_SETFL,flg); 
    //fcntl函数可以自由设置文件的打开方式，也可获取文件的已有设置方式 
    //给文件设置为可写和追加的方式，后面就可以自由操作文件的
    write(fd,"88888888",8);//如果是只写的方式的话就会把8个8给覆盖的了
    lseek(fd,0,SEEK_SET); 
    //将文件指针指向文件开头，如果文件是追加的方式的话，光标移动是不会起作用的
    write(fd,"99999999",8);  
    //如果是追加的方式8个8和8个9都写进去了的，如果不是的话，8个9会覆盖掉8个8；
    close(fd);
}

void test02()
{
    printf("stdout abc\n");//输出stdout abc，由系统文件的1描述符输出的
    dup2(1,3);  //复制1的文件描述符给3，3也指向了1的 输出文件文件描述结构
    close(1);  //关闭1指向，下次打开文件，系统会分配1描述符给文件

    int fd=open("./b.txt",O_RDWR);  //以只写的方式打开文件，文件的描述符是1
    printf("fd=%d\n",fd);//输出fd ，输出文件描述符已经改变了，无法输出了,输出的内容放在1文件描述符里面，也就是说 输出的fd=1\n写入了文件b.txt

    char buf[1024]={0};
    read(1,buf,16); 
    char tem[1024]={0};
    //读文件，b.txt现在的描述符是1，读到buf中，buf就是8个8和8个9
    write(3,buf,16); //将buf的内容写到屏幕上
    close(1); //关闭文件
    dup2(3,1); //将标准输出指回1，即复制3的描述符给1
    close(3);
   // printf("%d\n",fd);
    printf("%s\n",buf);
   // printf("stdout 999\n");
}



int main()
{
   // test01();
    test02();
 
    return 0;
}
