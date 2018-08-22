void zx_get_time_str(char *time_str, size_t time_str_size)
{
    time_t tt;
    struct tm local_time;

    time(&tt);
	
    localtime_r(&tt, &local_time);

    strftime(time_str, time_str_size, "%Y%m%d%H%M%S", &local_time);
}