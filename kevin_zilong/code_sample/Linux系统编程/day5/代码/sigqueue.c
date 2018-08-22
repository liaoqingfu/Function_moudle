#include <stdio.h>
#include <signal.h>

int main()
{
	union sigval value;
	value.sival_int = 5;
	sigqueue(3931, SIGINT, value); 
	return 0;
}
