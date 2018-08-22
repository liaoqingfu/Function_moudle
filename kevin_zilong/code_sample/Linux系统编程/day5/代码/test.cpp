
#include <list>
	
list<int> l;

struct A
{
	int a;
	int b;
}
	A a;

void handle(int b)
{
	a.a=33;
}

char *strtok(char *buf, char *str)
{
	static char* p;
	if (buf) p = buf;
	..
}

char *strtok_r(char *buf, char *str, char**save)
{
	if (buf) *save = buf;
	...
}
char buf[1024];
int main()
{
	signal(SIGINT, handle);
	for(int i = 0; i < 50; ++i)
	{
		l.push_back(i);
	}

	strcpy(buf, "abacadae");
	char *p = strtok(buf,"a");
	while(p)
	{
		strtok(NULL, "a");
	}
	a.a=10;
	a.b = 10;
	return 0;
}
