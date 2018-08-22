#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <alloca.h>
#include <sys/time.h>

int  main(){
	struct timeval tv;

	struct timezone tz;
	struct tm *p;
	time_t timep,timeq;

	int ret;
#if 0 
	ret = gettimeofday(&tv,&tz);
	if(ret == -1)
	{
	qDebug("Gettimeofday   error. ret = %d", ret);
	return ;   }
	ret = time(&timep);
	if(ret == (time_t) - 1)
	{
		qDebug("Time   error. ret = %d", ret);
		return;
	}
#endif
	/*get   local   current   time   and   date*/
	p = localtime(&timep);
	printf("%d\n",p->tm_year);
	printf("%d\n",p->tm_mon);
	printf("%d\n",p->tm_mday);
	printf("%d\n",p->tm_hour);
	printf("%d\n",p->tm_min);
	p = (struct tm *)alloca(sizeof(struct tm));
	p->tm_year = 2012 - 1900;
	p->tm_mon = 12 -1;
	p->tm_mday = 12;
	p->tm_hour = 12;
	p->tm_min = 12;
	p->tm_sec = 12;
	timeq = mktime(p);
	tv.tv_sec = (long)timeq;
	ret = settimeofday(&tv, &tz);
	if(ret == -1)
	{
		printf("settimeofday error.");
	}else  
		system("hwclock -w");
	printf("%d\n",p->tm_year);
	printf("%d\n",p->tm_mon);
	printf("%d\n",p->tm_mday);
	printf("%d\n",p->tm_hour);
	printf("%d\n",p->tm_min);
}
