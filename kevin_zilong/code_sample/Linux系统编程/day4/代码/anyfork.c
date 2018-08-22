#include <stdio.h>
#include <unistd.h>

int main()
{
	for (int i = 0; i < 5; ++i)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			printf("child\n");
			break;
		}
	}
	return 0;
}
