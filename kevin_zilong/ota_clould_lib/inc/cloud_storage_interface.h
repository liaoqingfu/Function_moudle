/*============================================================================
 Name        : cloud_storage_interface.h
 Author      : oceanwing.com
 Copyright   : 2017(c) Shenzhen Oceanwing Smart Innovations Co., Ltd.
 Create      : brady.xiao
 Date        : 2017-12-05 
 Description : 
 ============================================================================*/

#ifndef zx_CLOUD_STORAGE_INTERFACE_H
#define zx_CLOUD_STORAGE_INTERFACE_H

#pragma pack(1)

#ifdef __cplusplus
extern "C"
{
#endif

#define GET_INFO_LEN_SIZE 1024
#define CLOUD_DEBUG_LOG   0

typedef enum {

	STATION_ABNORMAL = 1,			//基站异常
	MOBLIE_ABNORMAL,				//移动端异常
	STATION_MONITOR_THUMBNAIL,		//基站监控缩略图
	DEVICE_MONITORING_VIDEO,		//设备监控视频
	FACE_THUMBNAIL,					//人脸缩略图
	CUSTOMER_FEEDBACK_DIAGRAM		//客户反馈图
	
}CLOUD_FILE_TYPE;


/*
上传文件数据, 成功返回key_prefix，失败返回NULL
*/
char * zx_putdata_aws_s3(
	const char *account,          //用户ID 
	const char *file_path_name ,  // 文件名称,需包含路径
	const char *device_sn ,       //设备SN 
	unsigned int type	          //文件类型 1-基站异常 2-移动端异常 3-基站监控缩略图 4-设备监控视频 5-人脸缩略图 6-客户反馈图
);


/*
Get the data from AWS_S3.
*/
int zx_getdata_aws_s3(
	const char *account,         //用户ID
	const char *station_sn,      //基站SN
	const char *file_key_prefix, //文件key值
	const char *filename         //存储的路径和文件名
);


/*url解码*/
void zx_urldecode(char *p);

/*
下载数据
参数 url_path : 下载的url
参数 filename : 下载内容存放的路径和文件名
成功返回 0 失败返回 -1
*/
int zx_dnload_data(const char *url_path,const char *filename);


/*
获取json里面key中的data数据
成功返回 带空间的数据信息，失败返回 NULL
需要手动清理返回的内存空间
*/
char* zx_MyJson_GetString(void *self, const char *key, const char *defval);

/*
获取文件大小
参数 path : 文件路径，传入参数
成功返回文件大小，失败返回 -1
*/
unsigned long get_file_size(const char *path);




#ifdef __cplusplus
}
#endif

#pragma pack(0)





#endif /* zx_CLOUD_STORAGE_INTERFACE_H */



