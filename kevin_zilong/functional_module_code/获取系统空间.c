#include <sys/types.h>
#include <sys/sysinfo.h>
#include <fcntl.h>

unsigned long long zx_get_sys_free_size(void)
{
	struct sysinfo info;
	//unsigned long long total_len = 0;

	sysinfo(&info);

	//total_len = info.totalram;

	return info.freeram;
}