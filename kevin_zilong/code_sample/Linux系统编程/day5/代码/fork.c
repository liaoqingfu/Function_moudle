#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>

void handle(int a)
{
	//	wait(NULL);
	while(1)
	{
		int ret= waitpid(-1,NULL,WNOHANG);
		if (ret == 0 || ret == -1)
			break;
		printf("huishou\n");
	}
}

int main()
{
	for(int i = 0; i < 5; ++i)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			printf("child\n");
			return 0;
		}
	}
	printf("main\n");
	signal(SIGCHLD, handle);

	printf("abc\n");
	while(1)
	{
		sleep(1);
	}
		return 0;
}

