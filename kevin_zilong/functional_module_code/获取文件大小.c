unsigned long get_file_size(const char *path)  
{ 
    unsigned long filesize = -1;  
    FILE *fp = NULL;  
	
	if(!path)
	{
		dzlog_warn("The function parameter  is NULL");
		return filesize;
	}
	
    fp = fopen(path, "r"); 
	
    if( !fp )
    {
		dzlog_error(" open path error:%s",path);  
        return filesize;
    }  
	
    fseek(fp, 0L, SEEK_END); 
	
    filesize = ftell(fp);  
	
    fclose(fp); 
	
    return filesize;  
} 


方法一：
unsigned long get_file_size(const char *path)  
{  
    unsigned long filesize = -1;  
    FILE *fp;  
    fp = fopen(path, "r");  
    if(fp == NULL)  
        return filesize;  
    fseek(fp, 0L, SEEK_END);  
    filesize = ftell(fp);  
    fclose(fp);  
    return filesize;  
} 

方法二：
#include <sys/stat.h>  //文件大小不能超过2G
  
unsigned long get_file_size(const char *path)  
{  
    unsigned long filesize = -1;      
    struct stat statbuff;  
    if(stat(path, &statbuff) < 0){  
        return filesize;  
    }else{  
        filesize = statbuff.st_size;  
    }  
    return filesize;  
} 