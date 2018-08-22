int system(const char * cmdstring)
{
    pid_t pid;
    int status = 0;
 
if(cmdstring == NULL)
{
    return (1); //如果cmdstring为空，返回非零值，一般为1
}

if((pid = fork())<0)
{
    status = -1; //fork失败，返回-1
}
else if(pid == 0)
{
    execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
    _exit(127); // exec执行失败返回127，注意exec只在失败时才返回现在的进程，成功的话现在的进程就不存在啦~~
}
else //父进程
{
    while(waitpid(pid, &status, 0) < 0)
    {
        if(errno != EINTR)
        {
            status = -1; //如果waitpid被信号中断，则返回-1
            break;
        }
    }
}
    return status; //如果waitpid成功，则返回子进程的返回状态
}




int status;
if(NULL == cmdstring) //如果cmdstring为空趁早闪退吧，尽管system()函数也能处理空指针
{
    return;
}
status = system(cmdstring);
if(status < 0)
{
    printf("cmd: %s\t error: %s", cmdstring, strerror(errno)); // 这里务必要把errno信息输出或记入Log
    return;
}
 
if(WIFEXITED(status))
{
    printf("normal termination, exit status = %d\n", WEXITSTATUS(status)); //取得cmdstring执行结果
}
else if(WIFSIGNALED(status))
{
    printf("abnormal termination,signal number =%d\n", WTERMSIG(status)); //如果cmdstring被信号中断，取得信号值
}
else if(WIFSTOPPED(status))
{
    printf("process stopped, signal number =%d\n", WSTOPSIG(status)); //如果cmdstring被信号暂停执行，取得信号值
}