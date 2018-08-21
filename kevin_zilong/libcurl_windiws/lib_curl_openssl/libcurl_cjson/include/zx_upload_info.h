#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "http_buf.h"

#ifndef ZX_UPLOAD_INFO_H
#define ZX_UPLOAD_INFO_H


#ifdef __cplusplus
extern "C"
{
#endif

#define AS_HOST						"https://security-app.eufylife.com"
#define ZX_UPLOAD_DEVS_INFO_URL		"/v1/factory/upload_devs_info"
#define ZX_UPLOAD_HUB_INFO_URL		"/v1/factory/upload_devs_info"
#define RESP_BODY_SIZE				2048

zx_Error zx_device_Info_base(
	int device_channel,			//设备通道号
	char *device_model,			//设备型号
	char *device_name,			//设备名
	char *device_sn,			//设备SN 
	int device_type,			//设备类型(1-摄像头)
	int event_num,				//事件数量 
	char *main_hw_version,		//主系统硬件版本号 
	char *main_sw_version,		//主系统软件版本号
	char *schedule,				//设备定时计划 
	char *schedulex,			//设备定时计划(基站用)
	char *sec_hw_version,		//辅助系统sub1g硬件版本号 
	char *sec_sw_version,		//辅助系统sub1g软件版本号 
	int sector_id,				//防区ID
	char *station_sn,			//基站SN 
	char *sub1g_mac,			//sub1g的mac地址
	char *wifi_mac				//wifi的mac地址
	);

zx_Error zx_base_station_base(
	char *ip_addr,				// ip地址
	char *main_hw_version,		//主系统硬件版本号
	char *main_sw_version,		//主系统软件版本号 
	char *sec_hw_version,		//辅助系统sub1g硬件版本号
	char *sec_sw_version,		//辅助系统sub1g软件版本号
	char *station_model,		//基站型号
	char *station_name,			//基站名 
	char *station_sn,			//基站SN
	char *sub1g_mac,			//sub1g的mac地址
	char *time_zone,			//基站时区
	char *wifi_mac,				//wifi的mac地址
	char *wifi_ssid				//wifi名称
	);

#ifdef __cplusplus
}
#endif


#endif 




