#include <signal.h>
#include <unistd.h>

int main()
{
	sigset_t set;
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGHUP);
	sigset_t oldset;
	//sigprocmask(SIG_BLOCK, &set, NULL);
	sigprocmask(SIG_BLOCK, &set, &oldset);//将set集合里面的参数也传给了oldset里面了的
	
	sigaddset(&set, SIGUSR1);
	sleep(5);
	//sigdelset(&set, SIGINT);
	//sigprocmask(SIG_UNBLOCK, &set, NULL);
	sigdelset(&set, SIGINT);
	sigprocmask(SIG_SETMASK, &set, NULL);
	while(1)
	{
		sleep(1);
	}
	return 0;
}
