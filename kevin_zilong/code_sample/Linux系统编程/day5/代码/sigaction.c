#include <signal.h>
#include <stdio.h>
#include <unistd.h>


void func(int a, siginfo_t *info, void *p)
{
	printf("sigint\n");
	printf("a=%d\n", a);
	printf("%d\n", (int)p);
//	printf("%d\n",info->si_value.sival_int);
}

int main()
{
	struct sigaction action;
	action.sa_handler = NULL;
	action.sa_sigaction = func; 
	sigemptyset(&action.sa_mask);
	action.sa_flags = SA_RESTART|SA_SIGINFO;

	sigaction(SIGINT, &action, NULL); 

	while(1)
	{
		sleep(1);
	}
	return 0;
}
