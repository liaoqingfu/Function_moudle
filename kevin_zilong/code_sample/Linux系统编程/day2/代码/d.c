#include <sys/types.h>
#include <utime.h>
#include <sys/time.h>


int main()
{
/*	struct utimbuf buf;
	buf.actime=10000;
	buf.modtime = 100000;
	utime("./a.out", &buf);
	*/
	struct timeval array[2];
	array[0].tv_sec = 100000;
	array[0].tv_usec = 10000;
	array[1].tv_sec = 100000;
	array[1].tv_usec=10000;
	utimes("./bb.txt", array);
	return 0;
}
