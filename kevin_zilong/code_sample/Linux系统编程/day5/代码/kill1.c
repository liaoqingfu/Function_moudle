#include <signal.h>

int main()
{
	kill(3150, SIGINT);
	kill(3150, SIGHUP);
	return 0;
}
