#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <openssl/err.h>
#include <openssl/ssl.h>

#define	TRACE()		do { fprintf(stderr, "%s ****** %s()****** <%d> \n", __FILE__,__func__, __LINE__ ); }while(0)


#define POSTURL "https://www.baidu.com"
//#define POSTFIELDS "email=myemail@163.com&password=mypassword&autologin=1&submit=登 录&type="
#define FILENAME "google.com"
//#define PROXY	   "192.168.2.56:8080"
//#define USERPASS   "wang-yunpeng:WYpeng656-2009"
size_t write_data(void* buffer,size_t size,size_t nmemb,void *stream)
{
	FILE *fptr = (FILE*)stream;
	fwrite(buffer,size,nmemb,fptr);
	return size*nmemb;
}

int main(int argc,char *argv[])
{
	CURL *curl;
	CURLcode res;
	FILE* fptr;
	//struct curl_slist *http_header = NULL;

	if ((fptr = fopen(FILENAME,"w")) == NULL)
	{
		fprintf(stderr,"fopen file error:%s\n",FILENAME);
		return -1;
	}

	curl = curl_easy_init();
	if (!curl)
	{
		fprintf(stderr,"curl init failed\n");
		return -1;
	}
	TRACE();
	//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);  
	//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);  
	//curl_easy_setopt(curl, CURLOPT_PROXY, PROXY);
	//curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, USERPASS);//公司上网的账号密码，
	
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);  //打开debug日志
	curl_easy_setopt(curl,CURLOPT_URL,POSTURL); //url地址
	//curl_easy_setopt(curl,CURLOPT_POSTFIELDS,POSTFIELDS); //post参数
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write_data); //对返回的数据进行操作的函数地址
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,fptr); //这是write_data的第四个参数值
	//curl_easy_setopt(curl,CURLOPT_POST,1); //设置问非0表示本次操作为post
	curl_easy_setopt(curl,CURLOPT_VERBOSE,1); //打印调试信息
	
	//curl_easy_setopt(curl,CURLOPT_HEADER,1); //将响应头信息和相应体一起传给write_data
	//curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1); //设置为非0,响应头信息location
	//curl_easy_setopt(curl,CURLOPT_COOKIEFILE,"/Users/zhu/CProjects/curlposttest.cookie");

	res = curl_easy_perform(curl);

	if (res != CURLE_OK)
	{
		switch(res)
		{
		case CURLE_UNSUPPORTED_PROTOCOL:
			fprintf(stderr,"不支持的协议,由URL的头部指定\n");
		case CURLE_COULDNT_CONNECT:
			fprintf(stderr,"不能连接到remote主机或者代理\n");
		case CURLE_HTTP_RETURNED_ERROR:
			fprintf(stderr,"http返回错误\n");
		case CURLE_READ_ERROR:
			fprintf(stderr,"读本地文件错误\n");
		default:
			fprintf(stderr,"返回值:%d\n",res);
		}
		return -1;
	}

	curl_easy_cleanup(curl);
}