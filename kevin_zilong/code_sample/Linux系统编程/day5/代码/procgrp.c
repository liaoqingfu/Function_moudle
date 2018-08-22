#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handle_child(int a)
{
	printf("child\n");
}

void handle_main(int a)
{
	printf("main\n");
}

int main()
{
	pid_t pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, handle_child);
	}
	else if (pid > 0)
	{
		signal(SIGINT, handle_main);
	}
	while(1)
	{
		sleep(1);
	}
	return 0;
}
