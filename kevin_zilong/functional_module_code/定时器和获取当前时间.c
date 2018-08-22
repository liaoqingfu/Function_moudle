void killpid_func()
{ 
	dzlog_warn("Download timeout and end the process %d",getppid());
	//kill(getppid(), SIGALRM); 
}
 
void init_sigaction()
{
    struct sigaction act;
    act.sa_handler = killpid_func; //设置处理信号的函数
    act.sa_flags  = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGALRM, &act, NULL);//时间到发送SIGROF信号
}
 
void init_time()
{
    struct itimerval val;
          
    val.it_value.tv_sec = 20; //20秒后启用定时器
    val.it_value.tv_usec = 0;
    val.it_interval = val.it_value; //定时器间隔为20s
    setitimer(ITIMER_REAL, &val, NULL);  //20秒后发送 SIGALRM 信号，循环发送
}



//用select来定时，可以精确到1us
void setTimer(int seconds, int mseconds)
{
        struct timeval temp;

        temp.tv_sec = seconds;
        temp.tv_usec = mseconds;

        select(0, NULL, NULL, NULL, &temp);
        printf("timer\n");

        return ;
}

long long zx_LocalTime_ms(void)   //获取当前时间，ms
{
    struct timeval tv;
    long long ms;

    gettimeofday(&tv, NULL);

    ms = (long long)tv.tv_sec * (long long)1000 + (long long)tv.tv_usec / (long long)1000;
    return ms;
}

UINT64 zx_mgw_CurTime_s(void)			//获取当前时间，秒
{
    struct timeval tv;
    UINT64 time_s; 
    gettimeofday(&tv, NULL);     
    time_s = (UINT64)tv.tv_sec  + (UINT64)tv.tv_usec;

    return time_s;
}


