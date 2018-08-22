#include <stdio.h>  
#include <sys/io.h>   
#include <stdlib.h>  
#include <errno.h>  
#include <string.h>  
#include <fcntl.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <time.h>   
#include <curl/curl.h>  
#include <sys/stat.h>   
#include <dirent.h>  
#include <unistd.h>
//#include <sys/inotify.h>   

#define CLK_TCK ((clock_t)1000)



void zx_urldecode(char *p)  
{  
	register int i=0;  
	if(!p )
	{
		printf("zx_urldecode function parameter  is NULL");
		return;
	}
	while(*(p+i))  
	{  
		if ((*p=*(p+i)) == '%')  
		{  
			*p=*(p+i+1) >= 'A' ? ((*(p+i+1) & 0XDF) - 'A') + 10 : (*(p+i+1) - '0');  
			*p=(*p) * 16;  
			*p+=*(p+i+2) >= 'A' ? ((*(p+i+2) & 0XDF) - 'A') + 10 : (*(p+i+2) - '0');  
			i+=2;  
		}  
		else if (*(p+i)=='+')  
		{  
			*p=' ';  
		}  
		p++;  
	}  
	*p='\0';  
}

int main(int argc, char **argv)  
{

    // 上传文件数据	
	CURL *curl;
	CURLcode res;
	FILE * hd_src;
	struct stat file_info;
	
	clock_t start , stop;
	double s=0;  
    start=clock();  
    char path[50]={0};
    strcpy(path,getcwd(NULL,0));
    char filename[30]= "boosttest.cpp";
	//char *file = "/home/work/test/curl-7.57.0.tar.gz";
	//strcat(path,filename);
	char *url = "http://zhixin-security-qa.s3.us-west-2.amazonaws.com/monitor/device/T8001G0217530460/EwLruEm4Uo6tP2gU.boosttest.cpp?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAJGA2GR3ZQQYPFYGA/20180226/us-west-2/s3/aws4_request&X-Amz-Date=20180226T052738Z&X-Amz-Expires=86400&X-Amz-SignedHeaders=content-length;host&X-Amz-Signature=c8d3b5ff3c6516d414def6adaf904a5581f3b5d43c6f995f37497221aa5158c6";
	int n = strlen(url);
	sprintf(path,"%s/%s",path,filename);
	printf("file=%s\n",path);
	char *uptoken  = malloc(n+2);
	memset(uptoken,0,n+2);
	
	memcpy(uptoken,url,n);
	//printf("url=%s\n",uptoken);
	//if(argc < 3)
	//return 1;
	
	//file = argv[1];
	//url = argv[2];
	
	printf("uptoken  ==  %s\n",uptoken);
	
	zx_urldecode(uptoken);
	
	printf("zx_urldecode === %s\n",uptoken);
	
	/* get the file size of the local file */ 
	stat(path, &file_info);
	
	/* get a FILE * of the same file, could also be made with
	 fdopen() from the previous descriptor, but hey this is just
	 an example! */ 
	hd_src = fopen(path, "rb");
	
	/* In windows, this will init the winsock stuff */ 
	curl_global_init(CURL_GLOBAL_ALL);
	
	/* get a curl handle */ 
	curl = curl_easy_init();
	if(curl) {
	/* we want to use our own read function */ 
	//curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
	
	/* enable uploading */ 
	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
	
	/* HTTP PUT please */ 
	curl_easy_setopt(curl, CURLOPT_PUT, 1L);
	
	/* specify target URL, and note that this URL should include a file
	   name, not only a directory */ 
	curl_easy_setopt(curl, CURLOPT_URL, uptoken);
	
	curl_easy_setopt( curl, CURLOPT_VERBOSE, 1L ); //在屏幕打印请求连接过程和返回http数据
	
	/* now specify which file to upload */ 
	curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
	
	// 下面两个为验证对方和验证主机名，若为0，则跳过验证
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); 
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	
	/* provide the size of the upload, we specicially typecast the value
	   to curl_off_t since we must be sure to use the correct data size */ 
	//curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
	                 //(curl_off_t)file_info.st_size);
	                 
	curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
	                 (curl_off_t)file_info.st_size);
	
	/* Now run off and do what you've been told! */ 
	res = curl_easy_perform(curl);
	/* Check for errors */ 
	if(res != CURLE_OK)
	  fprintf(stderr, "curl_easy_perform() failed: %s\n",
	          curl_easy_strerror(res));
	
	/* always cleanup */ 
	curl_easy_cleanup(curl);
	}
	fclose(hd_src); /* close the local file */ 
	free(uptoken);
	curl_global_cleanup();
   
    stop=clock();  
    s=(stop-start)/CLK_TCK;  
    printf("run time = %lf\n",s);  
   	
    return 0;   
}
