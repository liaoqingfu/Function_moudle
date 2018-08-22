#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc,char *argv[])
{
    printf("hello\n"); //打印hello
  // execl("/home/zilong/c_code/a.out","/home/zilong/c_code/a.out",NULL);//最后需要加NULL，才能判断结束，后面的程序都将被覆盖，执行./a.out程序
   // execl("/bin/ls","/bin/ls",NULL);//后面的程序都将被覆盖，执行ls命令程序
   // execl("/bin/ls","/bin/ls","-a","-l",NULL);

   // execlp("./a.out","./a.out",NULL);//这种更好一点，不需要路径，直接用执行程序替代了后面的程序
   // char *buf[]={"/home/zilong/c_code/a.out",NULL};
   // execv("/home/zilong/c_code/a.out",buf);

    char *buf1[]={"./a.out",NULL};
    execvp("./a.out",buf1);
    printf("wrold\n");
    return 0;
}
