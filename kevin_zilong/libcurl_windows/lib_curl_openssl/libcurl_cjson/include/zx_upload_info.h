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
	int device_channel,			//�豸ͨ����
	char *device_model,			//�豸�ͺ�
	char *device_name,			//�豸��
	char *device_sn,			//�豸SN 
	int device_type,			//�豸����(1-����ͷ)
	int event_num,				//�¼����� 
	char *main_hw_version,		//��ϵͳӲ���汾�� 
	char *main_sw_version,		//��ϵͳ����汾��
	char *schedule,				//�豸��ʱ�ƻ� 
	char *schedulex,			//�豸��ʱ�ƻ�(��վ��)
	char *sec_hw_version,		//����ϵͳsub1gӲ���汾�� 
	char *sec_sw_version,		//����ϵͳsub1g����汾�� 
	int sector_id,				//����ID
	char *station_sn,			//��վSN 
	char *sub1g_mac,			//sub1g��mac��ַ
	char *wifi_mac				//wifi��mac��ַ
	);

zx_Error zx_base_station_base(
	char *ip_addr,				// ip��ַ
	char *main_hw_version,		//��ϵͳӲ���汾��
	char *main_sw_version,		//��ϵͳ����汾�� 
	char *sec_hw_version,		//����ϵͳsub1gӲ���汾��
	char *sec_sw_version,		//����ϵͳsub1g����汾��
	char *station_model,		//��վ�ͺ�
	char *station_name,			//��վ�� 
	char *station_sn,			//��վSN
	char *sub1g_mac,			//sub1g��mac��ַ
	char *time_zone,			//��վʱ��
	char *wifi_mac,				//wifi��mac��ַ
	char *wifi_ssid				//wifi����
	);

#ifdef __cplusplus
}
#endif


#endif 




