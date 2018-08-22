#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAMEMAX 128

unsigned long get_file_size(const char *path)  
{ 
    unsigned long filesize = 0;  
    FILE *fp = NULL;  
	
	if(!path)
	{
		printf("The function parameter  is NULL");
		return filesize;
	}
	
	printf("path = %s\n",path);
	
    fp = fopen(path, "r"); 
	
    if( !fp )
    {
		printf(" open path error:%s",path);  
        return filesize;
    }  
	
    fseek(fp, 0L, SEEK_END); 
	
    filesize = ftell(fp);  
	
	printf("filesize = %lu\n",filesize);
	
    fclose(fp); 
	
    return filesize;  
} 

char * hex2str(const char *ssrc, int nlen)  
{  
    int  i = 0;  
	if (ssrc && nlen > 0)
	{
		char *sztmp = (char *)malloc(3*nlen); 	 
  		if (sztmp)
		{
    		for( i = 0; i < nlen; i++ )  
    		{  
        		sprintf(&sztmp[i*3], "%02X ", (unsigned char)ssrc[i]);  
    		}  
			return sztmp;
		}
	}
    return NULL; 
} 

void printf_hex(const char *ssrc, int nlen)
{
	char *hex = hex2str(ssrc, nlen);
	if (hex)
	{
		printf("%s\n", hex);
		free(hex);
	}
}

int zx_check_new_log(const char *log_key, char *outfile)
{
    char cmd_buf[64] = {0};
    sprintf(cmd_buf, "%s%s%s", "ls ", log_key, " | tail -n 1");
    printf("%s\n", cmd_buf);
    FILE *fp = popen(cmd_buf, "r");
    if (fp)
    {
        memset(outfile, 0, FILENAMEMAX);
        fgets(outfile, FILENAMEMAX, fp);
        pclose(fp);
        int ilen = strlen(outfile);        
        printf_hex(outfile, ilen);
        if ( ilen > 6)
        {
            if (outfile[ilen-1] < 0x20)
            {
                outfile[ilen-1] = 0;
            }
            ilen = strlen(outfile);
            printf("log: %s, len=%d\n", outfile, ilen);
            return ilen;
        }
    }
    return -1;
}

int main(int argc,char **argv)
{
	unsigned long filesize = 0;
	char charpath[128] = {0};
	
	printf("%d     %s\n",argc,argv[1]);
	
	printf("len = %d\n",strlen(argv[1]));
	
	filesize = get_file_size(argv[1]);
	
	printf("%lu\n",filesize);
	
	zx_check_new_log("zx*",charpath);
	
	printf("charpath len= %d\n",strlen(charpath));

	printf("charpath = %s\n",charpath);
	
	filesize = get_file_size(charpath);
	
	printf("%lu\n",filesize);

	return 0;
}