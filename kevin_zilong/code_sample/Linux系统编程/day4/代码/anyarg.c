#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void sum(int count, ...)
{
	int temp=0;
	va_list ap;
	va_start(ap, count);
	for (int i = 0; i < count; ++i)
	{
	 temp += va_arg(ap,int);
	}
	printf("%d\n", temp);
	va_end(ap);
}

#define mylog(fmt,...) myprint(__FILE__,__LINE__,fmt, __VA_ARGS__) 
void printStr(const char *p, ...)
{
	va_list ap;
	va_start(ap, p);
	while(1)
	{
		p = va_arg(ap, char*);
		if (p == NULL)
			break;
		printf("%s\n", p);
	}
	va_end(ap);
		
}

void myprint(const char *filename,int fileline,const char *fmt, ...)
{
	printf("%s:%d\n", filename,fileline);
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
}
int main()
{
//	sum(3,1,2,3);
//	printStr(NULL,"abc","def","gh",NULL);
//	myprint(__FILE__,__LINE__,"%d,%d\n", 3,4);
	mylog("%d,%d\n",3,4);
	return 0;
}
