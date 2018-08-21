/*============================================================================
 Name        : cloud_storage_interface.c
 Author      : oceanwing.com
 Copyright   : 2017(c) Shenzhen Oceanwing Smart Innovations Co., Ltd.
 Create      : brady.xiao
 Date        : 2017-12-05 
 Description : 
 ============================================================================*/

#include <stdio.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <string.h>  
#include <netinet/in.h>  
#include <netdb.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <sys/stat.h>

#include "cloud_storage_interface.h"
#include "base.h"
#include "http.h"
#include "cjson.h"
#include "as_interface.h"

char* zx_MyJson_GetString(void *self, const char *key, const char *defval)
{
	if( !key || !defval || !self)
	{
		dzlog_warn("The function parameter  is NULL");
		return NULL;
	}
	zx_Json *json_self = (zx_Json *)self;
#if CLOUD_DEBUG_LOG
	char *s_result = cJSON_PrintUnformatted(json_self); 
	
	if( !s_result )
	{
		dzlog_warn("The function parameter json is NULL");
		return NULL;
	}
	dzlog_info("MyJson GetString json is %s",s_result);
	if(s_result) {free(s_result); s_result = NULL;}
#endif

	const char *dst = zx_Json_GetString(json_self,key,defval);
	
	if(!dst)
	{
		dzlog_error(" Json GetString is NULL");
		return NULL;
	}
	
	char *buff = (char *)malloc(strlen(dst) + 2);
	
	if( !buff )
	{
		dzlog_error("malloc buff error");
		return NULL;
	}
	memset(buff,0,strlen(dst) + 2 );
	strncpy(buff,dst,strlen(dst));
		
	return buff;
}



/*url解码*/
void zx_urldecode(char *p)  
{  
	register int i=0;  
	if(!p )
	{
		dzlog_warn("The function parameter  is NULL");
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


size_t zx_aws_write_data(void* buffer,size_t size,size_t nmemb,void *stream)
{
	FILE *fptr = (FILE*)stream;
	fwrite(buffer,size,nmemb,fptr);
	return size*nmemb;
}

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


int zx_dnload_data(const char *url_path,const char *filename)
{
	CURL *curl = NULL;
	CURLcode res;
	FILE* fptr = NULL;
	int ret = 0;
	int i = 0;

	if(!url_path || !filename)
	{
		dzlog_warn("The function parameter  is NULL");
		return -1;
	}

	dzlog_info("******dnload start ******");

	if ( !(fptr = fopen(filename,"w")) )
	{
		dzlog_error("fopen file error:%s",filename);
		return -1;
	}
	
	curl = curl_easy_init();
	
	if (!curl)
	{
		dzlog_error("curl init failed");
		return -1;
	}
	res = curl_easy_setopt(curl,CURLOPT_URL,url_path); //url地址
	
	if(res != CURLE_OK)
	{
		dzlog_error("curl_easy_setopt url_path = %s====== res = %d\n",curl_easy_strerror(res),res);
		ret = -1;
		goto END;
	}

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);  //不检查证书
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);  //不检查证书

	//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);  //设置默认值：严格证书检查
	//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);  //设置默认值：严格证书检查

	/*如果在60秒内低于30字节/秒则中止*/
  	curl_easy_setopt(curl,CURLOPT_LOW_SPEED_TIME,60L);
  	curl_easy_setopt(curl,CURLOPT_LOW_SPEED_LIMIT,30L);
	
	curl_easy_setopt( curl, CURLOPT_VERBOSE, 1L ); 		//在屏幕打印请求连接过程和返回http数据
	curl_easy_setopt(curl,CURLOPT_NOPROGRESS,0L);  		//显示下载进度，启用进度表


	res = curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,zx_aws_write_data); //对返回的数据进行操作的函数地址

	if(res != CURLE_OK)
	{
		dzlog_error(" write data = %s====== res = %d\n",curl_easy_strerror(res),res);
		ret = -1;
		goto END;
	}

	curl_easy_setopt(curl,CURLOPT_WRITEDATA,fptr); 	//write_data的第四个参数

	dzlog_info("****** Start to perform CURL ******");

http_retry:	
	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		switch(res)
		{
		case CURLE_UNSUPPORTED_PROTOCOL:
			dzlog_error("Unsupported protocols are specified by the header of the URL:%d",res);
			break;
		case CURLE_COULDNT_CONNECT:
			dzlog_error("Cannot connect to a remote host or agent:%d",res);
			break;
		case CURLE_HTTP_RETURNED_ERROR:
			dzlog_error("HTTP return error:%d",res);
			break;
		case CURLE_READ_ERROR:
			dzlog_error("Error reading the local file:%d",res);
			break;
		default:
			dzlog_error("curl_easy_perform curl = %s======res = %d",curl_easy_strerror(res),res);
		}

		if(CURLE_COULDNT_RESOLVE_HOST == res )  //主机名无法解析，重试三次
		{
			++i;
			if(i <= 3)
			{
				dzlog_info("repeat curl %d count",i);
				goto http_retry;
			}else{
				dzlog_error("repeat curl %d count %s====== res = %d",i,curl_easy_strerror(res),res);
			}
				
		}
		ret = -1;
	}
	
	dzlog_info("****** perform CURL end******");
	
END:
	curl_easy_cleanup(curl);
	fclose(fptr);
	
	dzlog_info("****** dnload data end ******");
	return ret;
}



// 上传文件数据, 成功返回key_prefix，失败返回NULL
char* zx_putdata_aws_s3(
	const char *account,           //用户ID 
	const char *file_path_name ,   // 文件路径和名字
	const char *device_sn ,        //设备SN
	unsigned int type	           //文件类型 1-基站异常 2-移动端异常 3-基站监控缩略图 4-设备监控视频 5-人脸缩略图 6-客户反馈图 
)
{
	char *uptoken_put_url = NULL;
	zx_Json *res_root = NULL;
	char *filename_path = NULL, *tmp = NULL;
	char *filename = NULL;
	CURLcode res;
	char *s_result = NULL;
	unsigned long filesize = 0;
	FILE * fp = NULL;
	char *file_key_prefix = NULL;	
	zx_Client client;
	CURL *curl = NULL;
	int i = 0;
	
	if(!account || !file_path_name || !device_sn)
	{
		dzlog_warn("The function parameter is NULL");
		return NULL;
	}

	dzlog_info("****** putdata aws s3 start ******");
	
	//zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);

	filesize = get_file_size(file_path_name);
	
	if(filesize == 0)
	{
		dzlog_error("****** putdata aws s3 get file size error******");
		return NULL;
	}

	filename_path = (char *)malloc(strlen(file_path_name)+2);
	
	if(!filename_path)
	{
		dzlog_error("putdata aws s3 malloc filename error");
		return NULL;
	}
	memset(filename_path,0,strlen(file_path_name)+2);
	strncpy(filename_path,file_path_name,strlen(file_path_name));
	
#if CLOUD_DEBUG_LOG
	dzlog_info("filename_path = %s",filename_path);
#endif

	tmp = strrchr( filename_path, '/' );  

	if( tmp )
	{
		tmp = tmp + 1;  // +1 去掉右斜杆,取后面的名字
		filename = (char *)malloc( strlen( tmp ) + 2 );
		if( !filename )
		{
			dzlog_error("malloc filename error");
			
			free( filename_path ); 
			filename_path = NULL;
			goto END;
		}

		memset( filename, 0, strlen( tmp ) + 2  );
		strncpy( filename, tmp, strlen( tmp ) );
		
		free( filename_path ); 
		filename_path = NULL;

	}
	else
	{
		filename = filename_path;
		tmp = NULL;
	}

	fp = fopen(file_path_name, "r");
	
	if (!fp)  
    {  
        dzlog_error(" fopen file_path_name error:%s",file_path_name);  
		goto END;  
    } 
	
#if CLOUD_DEBUG_LOG
	dzlog_info("hub uptoken request === %s, %s, %lu, %s, %d ",account,filename,filesize,device_sn,type);
#endif

	res_root = zx_hub_uptoken_request(account,filename,filesize,device_sn,type,false,client);
	
	if(!res_root || zx_Json_GetInt(res_root, "code", 0) != 0)
	{
		
		dzlog_error("****** hub uptoken request cjson is NULL******");
		goto END;
	}
	
#if CLOUD_DEBUG_LOG
	s_result = cJSON_PrintUnformatted(res_root);
	
	if(!s_result)
	{
		dzlog_error("****** putdata aws s3 hub uptoken request cjson is NULL******");
		goto END;
	}
	dzlog_info("****** putdata aws s3 cjson s_result is %s******",s_result);
	
	if(s_result) {free(s_result); s_result = NULL;}
#endif

	res_root = cJSON_GetObjectItem(res_root, "data");

	if(!res_root)
	{
		
		dzlog_error("****** hub uptoken request data is NULL******");
		goto END;
	}
	
#if CLOUD_DEBUG_LOG
	s_result = cJSON_PrintUnformatted(res_root);
	
	if(!s_result)
	{
		dzlog_error("******putdata aws s3 data is NULL******");
		goto END;
	}
	dzlog_info("****** putdata aws s3 cjson data is %s******",s_result);
	
	if(s_result) {free(s_result); s_result = NULL;}
#endif

	uptoken_put_url = zx_MyJson_GetString(res_root, "uptoken", "");
	
	if(!uptoken_put_url)
	{
		dzlog_error("****** uptoken is NULL******");
		goto END;
	}
	
#if CLOUD_DEBUG_LOG
	dzlog_info("******putdata aws s3 uptoken =  %s******",uptoken_put_url);
#endif

	zx_urldecode(uptoken_put_url);  //url解码
	
#if CLOUD_DEBUG_LOG
	dzlog_info("******putdata aws s3 urldecode uptoken =  %s******",uptoken_put_url);
#endif
	
	file_key_prefix = zx_MyJson_GetString(res_root, "key_prefix", "");

	if(!file_key_prefix)
	{
		dzlog_error("****** hub uptoken request file_key_prefix is NULL******");
		goto END;
	}
	
#if CLOUD_DEBUG_LOG
	dzlog_info("******  urldecode file key prefix = %s******",file_key_prefix);
#endif

	curl = curl_easy_init(); 

	if (!curl)
	{
		if(file_key_prefix) {free(file_key_prefix); file_key_prefix = NULL;}
		dzlog_error("curl init failed");
		goto END;
	}

	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
	curl_easy_setopt(curl, CURLOPT_PUT, 1L);
	
	curl_easy_setopt(curl, CURLOPT_URL, uptoken_put_url);

	// 下面两个为验证对方和验证主机名，若为0，则跳过验证，规避ssl的证书检查
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

	//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);  //设置默认值：严格证书检查
	//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);  //设置默认值：严格证书检查

	 /*如果在60秒内低于30字节/秒则中止*/
  	curl_easy_setopt(curl,CURLOPT_LOW_SPEED_TIME,60L);
  	curl_easy_setopt(curl,CURLOPT_LOW_SPEED_LIMIT,30L);

	curl_easy_setopt( curl, CURLOPT_VERBOSE, 1L ); //在屏幕打印请求连接过程和返回http数据
	curl_easy_setopt(curl,CURLOPT_NOPROGRESS,0L);  //显示上传进度，启用进度表
	

	curl_easy_setopt(curl, CURLOPT_READDATA, fp);
	                  
	curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,(curl_off_t)filesize);

	dzlog_info("****** Start to perform CURL ******");


http_retry:

	res = curl_easy_perform(curl);
	
	if(res != CURLE_OK)
	{
		switch(res)
		{
		case CURLE_UNSUPPORTED_PROTOCOL:
			dzlog_error("Unsupported protocols are specified by the header of the URL:%d",res);
			break;
		case CURLE_COULDNT_CONNECT:
			dzlog_error("Cannot connect to a remote host or agent:%d",res);
			break;
		case CURLE_HTTP_RETURNED_ERROR:
			dzlog_error("HTTP return error:%d",res);
			break;
		case CURLE_READ_ERROR:
			dzlog_error("Error reading the local file:%d",res);
			break;
		default:
			dzlog_error("%s====== res = %d",curl_easy_strerror(res),res);	
		}

		if(CURLE_COULDNT_RESOLVE_HOST == res )  //主机名无法解析，重试三次
		{
			++i;
			if(i <= 3)
			{
				dzlog_info("repeat curl %d count",i);
				goto http_retry;
			}else{
				dzlog_error("repeat curl %d count %s====== res = %d",i,curl_easy_strerror(res),res);
			}	
		}
		if(file_key_prefix) {free(file_key_prefix); file_key_prefix = NULL;}
	}
	
	dzlog_info("******perform CURL end******");
	
END:
	if(s_result) 
	{
		free(s_result); 
		s_result = NULL;
	}

	if(uptoken_put_url) 
	{
		free(uptoken_put_url); 
		uptoken_put_url = NULL;
	}

	if(filename) 
	{
		free(filename); 
		filename = NULL;
	}
	
	if(curl) 
	{
		curl_easy_cleanup(curl);
		curl = NULL;
	}

	if( fp ) 
	{
		fclose(fp);
		fp = NULL;	
	}

	zx_Client_Cleanup(&client);
	//curl_global_cleanup();

	dzlog_info("****** put data end and filesize :%lu******",filesize);
    	
    return file_key_prefix;  
}


/*
从S3服务器上获取数据
1.从filename里面下载文件到指定路径下
*/
int zx_getdata_aws_s3(
	const char *account,         //用户ID
	const char *station_sn,      //基站SN
	const char *file_key_prefix, //文件key值
	const char *filename         //存储的路径和文件名
)
{
	char *get_down_url = NULL;
	zx_Json *res_root = NULL;
	
#if CLOUD_DEBUG_LOG
	char *s_result = NULL;
#endif

	int ret = -1;
	zx_Client client;
	unsigned long filesize = 0;

	if(!account || !station_sn || !filename)
	{
		dzlog_warn("the function parameter is NULL");
		return -1;
	}
	
	dzlog_info("****** get data start******");

	//zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	
	res_root = zx_hub_dntoken_request(account,file_key_prefix,station_sn,client);

	if(!res_root || zx_Json_GetInt(res_root, "code", 0) != 0)
	{
		
		dzlog_error("****** hub dntoken request cjson is NULL******");
		goto END;
	}
#if CLOUD_DEBUG_LOG
	s_result = cJSON_PrintUnformatted(res_root);
	
	if(!s_result)
	{
		dzlog_error("****** cjson is NULL******");
		return -1;
	}
	dzlog_info("****** cjson s_result is: %s******",s_result);

	if(s_result) {free(s_result); s_result = NULL;}
#endif

	res_root = cJSON_GetObjectItem(res_root, "data");

	if(!res_root)
	{
		dzlog_error("****** data is NULL******");
		return -1;
	}
	
#if CLOUD_DEBUG_LOG

	s_result = cJSON_PrintUnformatted(res_root);
	
	if(!s_result)
	{
		dzlog_error("****** data cjson is NULL******");
		return -1;
	}
	dzlog_info("****** cjson data is: %s******",s_result);

	if(s_result) 
	{
		free(s_result); 
		s_result = NULL;
	}
#endif

	get_down_url = zx_MyJson_GetString(res_root, "storage_url", "");

	if(!get_down_url)
	{
		dzlog_error("****** get down url is NULL******");
		ret = -1;
		goto END;
	}
	
#if CLOUD_DEBUG_LOG
	dzlog_info("******get down url = %s******",get_down_url);
#endif

	zx_urldecode(get_down_url);  //url解码
	
#if CLOUD_DEBUG_LOG
	dzlog_info("******urldecode get_down_url = %s******",get_down_url);
#endif

	ret = zx_dnload_data(get_down_url,filename);
	if(ret == -1)
	{
		dzlog_error("****** dnload data  is error******");
		goto END;
	}

	filesize = get_file_size(filename);

	if(filesize == -1)
	{
		dzlog_error("******get file size  is error******");
	}

END:	
	if(get_down_url) 
	{
		free(get_down_url); 
		get_down_url = NULL;
	}

	zx_Client_Cleanup(&client);
	//curl_global_cleanup();
	
	dzlog_info("****** get data end and filesize is: %lu******",filesize);

	return ret;
}













