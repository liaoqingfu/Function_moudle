


typedef unsigned short  	UINT16;
typedef unsigned int 		UINT32;
typedef unsigned long  		ULONG;
typedef unsigned long long 	UINT64;

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