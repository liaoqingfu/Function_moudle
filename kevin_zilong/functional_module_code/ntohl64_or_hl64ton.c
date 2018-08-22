UINT64 zx_mgw_CurTime_s(void)			//获取当前时间，秒
{
    struct timeval tv;
    UINT64 time_s; 
    gettimeofday(&tv, NULL);     
    time_s = (UINT64)tv.tv_sec  + (UINT64)tv.tv_usec;

    return time_s;
}


UINT64 hl64ton(UINT64 host)   
{   
	UINT64   ret = 0;   
	ULONG   high,low;
	
	low   =   host & 0xFFFFFFFF;
	high   =  (host >> 32) & 0xFFFFFFFF;
	
	low   =   htonl(low);   
	high   =   htonl(high);   
	
	ret   =   low;
	ret   <<= 32;   
	ret   |=   high;   
	return   ret;   
}

UINT64 ntohl64(UINT64 host)   
{   
	UINT64   ret = 0;   
	ULONG   high,low;
	
	low   =   host & 0xFFFFFFFF;
	high   =  (host >> 32) & 0xFFFFFFFF;
	
	low   =   ntohl(low);   
	high   =   ntohl(high);   
	
	ret   =   low;
	ret   <<= 32;   
	ret   |=   high;   
	return   ret;   
}