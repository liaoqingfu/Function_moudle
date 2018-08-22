#include <unistd.h>
#include <signal.h>
#include <stdio.h>
void handle_int(int a)
{
	if(a == SIGRTMIN)
	{

	printf("singRTMIN\n");
	}
	if (a == SIGHUP)
	{
		printf("sighup\n");
	}
}
void handle_hup(int a)
{
	printf("sighup\n");
}
int main()
{
	signal(SIGRTMIN,handle_int);
	signal(SIGHUP, handle_int);
	

	
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGRTMIN);
	sigaddset(&set, SIGHUP);
	sigprocmask(SIG_BLOCK, &set, NULL);
	sleep(20);
	sigprocmask(SIG_UNBLOCK, &set, NULL);
	//sigdelset(&set, SIGINT);
	//sigprocmask(SIG_UNBLOCK, &set, NULL);
	
//	signal(SIGINT, SIG_IGN);

//	sleep(10);
	//	signal(SIGINT, SIG_DFL);
//	signal(SIGINT, handle_int);
	while(1)
	{
		sleep(1);
	}
	return 0;
}
