#include <signal.h>
#include <unistd.h>

int main()
{
	//signal(SIGINT, handle);
	//sigset_t set;
	//sigaddset(&set,SIGINT);
	//sigprocmask(SIG_BLOCK, &set, NULL);

	signal(SIGINT, SIG_IGN);
	while(1)
	{
		sleep(1);
	}
	return 0;
}
