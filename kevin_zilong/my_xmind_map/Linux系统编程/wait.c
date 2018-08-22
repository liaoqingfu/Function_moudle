#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/wait.h>
int main()
{
    int status=0;
    pid_t pid =fork();//创建一个子进程
    if(pid==0)
    {
        printf("子进程\n");
    }
    else if(pid>0)
    {
        pid_t pid_wait=wait(&status);//将结束状态放在status里面
		//wait(NULL); //也可以这样，回收所有的子进程
        printf("status=%d\n",status);//2*256=512,根据返回值确定status值
        
        if(WIFEXITED(status))//判断子进程是否是正常结束
        {
            int exit_code=WEXITSTATUS(status);//获取子进程结束时的返回值
            printf("child exit=%d\n",exit_code);//2
        }

        printf("主进程\n");
    }
   // getchar();

    return 2;
}
