/*============================================================================
 Name        : ota_interface.h
 Author      : oceanwing.com
 Copyright   : 2017(c) Shenzhen Oceanwing Smart Innovations Co., Ltd.
 Create      : brady.xiao
 Date        : 2018-01-31 
 Description : 
 ============================================================================*/



#ifndef zx_OTA_INTERFACE_H
#define zx_OTA_INTERFACE_H

#pragma pack(1)

#ifdef __cplusplus
extern "C"
{
#endif

#define READ_OTA_DATA_SIZE  1024  
#define INFO_DATA_SIZE 		512
#define OTA_SHORT_LEN       32
#define MD5_OTA_SIZE        16

#define OTA_DEBUG_LOG 		0

#define OTA_PAKAGE           "ota_pakage"
#define HUB_MAIN_SYSTEM      "hub_7628_sys"
#define HUB_SUB1G_SYSTEM     "hub_1310_sys"
#define CAM_MAIN_SYSTEM      "cam_3518_sys"
#define CAM_MAIN_SYSTEM_720P "cam_3518_sys_720"
#define CAM_SUB1G_SYSTEM     "cam_1310_sys"
#define AI_KERNEL_SYSTEM     "ai_kernel_sys"
#define AI_ROOTFS_SYSTEM     "ai_rootfs_sys"
#define AI_MODEL_SYSTEM      "ai_model_sys"

#define VERSION_PATH         "/tmp"
#define VERSION_TF_PATH      "/media/mmcblk0p1"
#define HUB_MAIN_VER_NAME    "7628_emmc.img"
#define HUB_SEC_VER_NAME     "CC1310_C.bin"

#define DEV_MAIN_VER_NAME    "3518_emmc.bin"
#define DEV_MAIN_VER_NAME_720P    "3518_emmc_720.bin"
#define DEV_SEC_VER_NAME     "CC1310_dev.bin"

#define HUB_SUB1G_OTA_PATH   "/mnt/sub1g_upgrade/"

#define XM_CAM_1080P_HWMODEL  "HAIYI-IMX323"
#define XM_CAM_720P_HWMODEL   "HAIYI-OV9732"

#define AI_KERNEL_VER_NAME    "kernel.img"
#define AI_ROOTFS_VER_NAME    "rootfs.img"
#define AI_MODEL_VER_NAME     "model.img"

#define OTA_LIBCURL_OR_WGET_DOWNLOAD_METHOD     0    //1为wget下载方式，0为libcurl下载方式


#if 0
typedef enum upgrade_err_info{

	get_all_info = 1,
	Compute_md5,
	Assignment,
	
}upgrade_err_info;
#endif

typedef struct version_upgrade{

	int force_upgrade;         	//是否强制升级
	unsigned int upgrade_flag; 	//1:有版本可供升级;0:不需要升级
	unsigned int upgrade_type; 	//升级方式: 1全量 2差分包
	unsigned int rom_version;  	//升级的版本号
	unsigned long file_size;   	//升级包大小,单位:字节
	
	char *device_type;        	//device type的名称,实际是产品硬件版本，包含了产品信息和硬件版本，后端通过该字段来决定客户端需要的是那种固件包
	char *introduction;       	//升级包描述
	char *down_url;           	//升级包路径

	char *file_name;          	//升级包名称
	char *rom_version_name;   	//升级的版本名称
	char *file_md5;           	//升级包MD5

	struct version_upgrade *next;

}VERSION_UPGRADE;



/*
获取所有的升级版本的信息
success返回 VERSION_UPGRADE的head指针，false返回 NULL 
参数 current_version_name: 当前对应 device_type 的版本名称,
参数 device_type: device type的名称,实际是产品硬件版本，包含了产品信息和硬件版本，后端通过该字段来决定客户端需要的是那种固件包,
参数 rom_version: 设备rom的版本号,
参数 sn:设备的序列号
*/
VERSION_UPGRADE* zx_get_all_upgrade_version_info(
		const char *current_version_name, 	//当前对应 device_type 的版本名称
		const char *device_type, 			// device type的名称,实际是产品硬件版本，包含了产品信息和硬件版本，后端通过该字段来决定客户端需要的是哪种固件包
		unsigned long rom_version, 			// rom的版本号
		const char *sn 						// 序列号
		);



/*
获取所有升级版本里面单个版本的信息，必须先调用zx_get_all_upgrade_version_info获取到所有的信息，才能获取到单个版本的信息
success返回 VERSION_UPGRADE的设备信息指针，false返回 NULL
参数 device_type: device type的名称
参数 pPre: 传入参数,
*/
VERSION_UPGRADE *get_device_type_info(VERSION_UPGRADE *pPre,const char *device_type);




/*
下载需要的版本信息
success返回0，false返回-1 
参数 down_path: 下载的路径
参数 version_upgrade: 传入参数
参数 default_filename: 自己定义的文件名，如果不写，默认为服务器查询到的文件名
*/
int zx_dnload_upgrade_version(
		const char *down_path,			//下载路径
		VERSION_UPGRADE *upgrade,		//版本信息
		char * default_filename 		//自定义文件名，默认为服务器上的文件名
		);




/*
释放分配的空间
参数 head: 传入参数，VERSION_UPGRADE头节点
success返回0，false返回-1
*/
int zx_upgrade_free(VERSION_UPGRADE *head);


#ifdef __cplusplus
}
#endif

#pragma pack(0)

#endif /* zx_OTA_INTERFACE_H */




