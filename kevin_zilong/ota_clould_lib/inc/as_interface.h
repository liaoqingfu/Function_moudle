/*
 ============================================================================
 Name        : as_interface.h
 Author      : Oceanwing.com
 Copyright   : 2017(c) Shenzhen Oceanwing Smart Innovations Co., Ltd.
 Create      : lucien.liu
 date        : 2017-10-30 
 Description :
 ============================================================================
 */
#ifndef zx_AS_INTERFACE_H
#define zx_AS_INTERFACE_H


#ifndef bool
#define bool	char
#endif

#ifndef true
#define true	1
#endif

#ifndef false
#define false	0
#endif


#include "base.h"
#include "http.h"
#include "cjson.h"
#include <curl/curl.h>
//#include "sys_interface.h"
//#include "comm_protocol_define.h"
//#include "ai_interface.h"

#pragma pack(1)

#ifdef __cplusplus
extern "C"
{
#endif


//#define AS_HOST "http://security-moat-qa.goroav.com"
//#define AS_HOST            "http://54.191.193.72:7075"

#define AS_HOST            "https://security-app.eufylife.com"

//#define AS_ROM_UPGRADE     "http://security-qa.goroav.com:80"
//#define AS_ROM_UPGRADE     "http://54.191.193.72:9094"    //后期这个服务器不会在用
#define AS_ROM_UPGRADE     "https://security-app.eufylife.com"  


#define RESP_BODY_SIZE  2048


void zx_init_as_interface(void);

/*============================================================================
 func zx_hub_bind_devs 
 AS: post /hub/bind_devs  绑定设备  
 
 response JSON:
 {
  "code": 20003,
  "msg": "base station not binded"
 }  
============================================================================*/
zx_Error zx_hub_bind_devs(const char* station_sn, const char* device_sn, const char* device_name,
	              	      const int device_channel, const int dev_type, const char *account_id);


/*============================================================================
 func zx_hub_unbind_devs 
 AS: post /hub/unbind_devs  绑定设备  
{
  "account": "string",
  "device_sn": "string",
  "station_sn": "string"
} 
 response JSON:
 {
  "code": 20003,
  "msg": "base station not binded"
 }  
============================================================================*/
zx_Error zx_hub_unbind_devs(const char* station_sn, const char* device_sn, const char *account_id);

/*============================================================================
 func zx_hub_check_devs
 AS: post /hub/check_device 设备鉴权 
 param JSON:
 {
	"device_sn": "string",
 } 
 resopnse JSON:
 {
  "code": 0,   //code list define?
  "msg": "成功"
 }   
============================================================================*/
zx_Error zx_hub_check_devs(const char* device_sn, const char* station_sn);


/*============================================================================
 func zx_hub_edit_devs_info 
 AS: post /hub/update_devs_info 设备更名 
 param JSON:
 {
	"device_name": "string",
    "device_sn": "string",
    "time_zone": "string",
 }  
 response JSON:
 {
  "code": 20003,
  "msg": "base station not binded"
 }    
============================================================================*/
//zx_Error zx_hub_edit_devs_info(const char* device_sn, const char* station_sn, 
//                     const char* device_name, const char* schedule, int device_channel);
zx_Error zx_hub_edit_devs_intinfo_by_paramname(const char* device_sn, const char* station_sn, const char *account, 
                     const char* param_name, int value);
zx_Error zx_hub_edit_devs_strinfo_by_paramname(const char* device_sn, const char* station_sn, const char *account, 
                     const char* param_name, const char* value);
zx_Error zx_update_devs_version(const char* device_sn, const char* station_sn, const char *account, 
                     const char* main_sw_version, const char *sec_sw_version, const char *hw_version);

/*============================================================================
 func zx_hub_edit_hub_info 
 AS: post /hub/update_hub_info  修改基站名称
 param JSON:
 {
	"station_name": "string",
    "station_sn": "string",
    "time_zone": "string"
 }  
 response JSON:
 {
  "code": 20003,
  "msg": "base station not binded"
 }    
============================================================================*/
zx_Error zx_hub_edit_hub_info(const char* station_sn, const char* station_name, const char* time_zone,
                                            const char* ip_addr, const char* wifi_ssid, const char* account);
zx_Error zx_hub_update_info_by_infoname(const char* station_sn, const char* account, const char* infoname, const char *value);
zx_Error zx_hub_update_version(const char* station_sn, const char* account, const char* main_soft_version, const char *sec_soft_version);

/*============================================================================
 func zx_hub_get_devs_info 
 AS: post /hub/get_dev_info  查询设备信息 
 param JSON:
 {
    "station_sn": "string",
    "device_sn":string
 } 
 response JSON:
 {
  "code": 0,
  "data": {
    "create_time": 0,
    "device_channel": 0,
    "device_id": 0,
    "device_model": "string",
    "device_name": "string",
    "device_sn": "string",
    "device_type": 0,
    "station_sn": "string",
    "device_wifi_mac":"string",   //ADD
    "device_sub1g_mac":"string",  //ADD
    "status": 1
  },
  "msg": "成功"
 }
============================================================================*/
zx_Error zx_hub_get_devs_info(const char* station_sn, const char* device_sn);


/*============================================================================
 func zx_hub_get_devs_params
 AS: POST /hub/get_devs_params  基站查询设备参数列表
 param JSON:
 {
  "device_sn": "string",
  "station_sn": "string"
 }
 resopnse JSON:
 {
  "code": 0,
  "data": [
    {
      "create_time": 0,
      "device_sn": "string",
      "param_id": 0,
      "param_type": 0,
      "param_value": "string",
      "status": 1
    }
  ],
  "msg": "成功"
}
============================================================================*/
zx_Error zx_hub_get_devs_params(const char* station_sn, const char* device_sn);

/*============================================================================
 func zx_hub_get_hub_params
 AS: get /hub/get_hub_params 基站查询基站参数列表
 param JSON:
 {
    "station_sn": "string",
 } 
============================================================================*/
zx_Error zx_hub_get_hub_params(const char* station_sn, const char* account);

/*============================================================================
方法：post /hub/get_p2p_connectinfo 获取p2p连接信息 DID\initstr\lisence
入参：{
  		"station_sn": "string"             //基站SN
	}
返回：{
  		"code": 0,                     
  		"data": 
		{
    		"create_time": 0,            //创建时间
    		"did_id": 0,                 //序列号
    		"p2p_did": "string",          //p2p did
    		"push_did": "string",         //push did
    		"station_sn": "string",        //sn
    		"status": 1,                 //状态
    		"p2p_conn ": "string",       //P2P连接串
    		"p2p_license": "string"       //p2p license
            "push_license": "string"      //push license
  		},
  		"msg": "string"
}
============================================================================*/

//zx_Error zx_hub_get_p2p_params(const char* station_sn, P2P_BASE_INFO *p2p_info);//const char* station_sn, char *did, char *init_str, char *p2p_license, char *push_license

/*============================================================================
方法：post /hub/get_rtsp_connectinfo 设备分配rtsp
	入参：{
  			"device_sn": "string",      //设备SN
            “station_sn”:”string”      //基站SN
}
	返回：{
  	"code": 0,
  		"data": {
    		"create_time": 0,
    		"device_sn": "string",
    		"rtsp_id": 0,
    		"status": 1,
    		"svr_domain": "string",
    		"svr_port": 0
  			},
  		"msg": "成功"
	}
============================================================================*/
zx_Error zx_get_rtsp_connectinfo(const char* station_sn, const char* decvice_sn);

/*============================================================================
方法：post /hub/reset 基站恢复出厂设置通知
入参：{
  		"station_sn": "string",     //基站sn
        “account”:”string”          //账号信息
}
返回：{
          	"code": 0,                
"msg": "xxxxxxxxxxx"
}
============================================================================*/
zx_Error zx_hub_reset(const char* station_sn, const char* account);


/*============================================================================
方法：post /hub/upload_devs_params 修改设备参数
	入参：{
  		"device_sn": "string",
		"station_sn": "string",            //基站sn
 		 "params": [
    		{
      			"param_type": 0,
      			"param_value": "string"
    		}
  		]
		}
	返回：{
          	"code": 0,                
			"msg": "xxxxxxxxxxx"
		}
============================================================================*/
zx_Error zx_upload_devs_params(const char* account_id, const char* station_sn, const char* device_sn, int param_type, const char* param_value);
zx_Error zx_upload_devs_int_params(const char* account_id, const char* station_sn, const char* device_sn, int param_type, const int param_value);

//    post /hub/upload_devs_info  设备上报

//    post /hub/upload_hub_baseInfo  基站上报

//    post /hub/upload_hub_params  修改基站参数
/*============================================================================
方法：post /hub/upload_hub_params 修改基站参数
	入参：{
  		"account": "string",
		"station_sn": "string",            //基站sn
 		 "params": [
    		{
      			"param_type": 0,
      			"param_value": "string"
    		}
  		]
		}
	返回：{
          	"code": 0,                
			"msg": "xxxxxxxxxxx"
		}
============================================================================*/
zx_Error zx_upload_hub_params(const char* account_id, const char* station_sn, int param_type, const char* param_value);

//    post /event/hub/upload_history_record  单个监控事件上报

//    post /event/hub/upload_history_records  监控事件上报

//	get /cloud/uptoken 生成上传凭证(基站调用)

//    get /hub/running  查询基站运行状态

//    post /hub/running  添加基站运行状态

//	post /files/hub/crash 上传基站异常日志

zx_Error zx_curl_net_detection(void);


/*============================================================================
方法：post hub/bind_hub app绑定基站
入参：{
  "account": "string",
  "name": "string",
  "sn": "string",
  "time_zone": "string"
}
============================================================================*/
zx_Error zx_app_bind_hub(const char* station_sn, const char* account_id, const char *hub_name, const char* time_zone);

/*============================================================================
方法：post /app/unbind_hub app解除绑定基站
入参：{
  "account": "string",
  "sn": "string",
}
============================================================================*/
zx_Error zx_app_unbind_hub(const char* station_sn, const char* account_id);
zx_Error zx_active_curl_for_thread(void);

/*============================================================================
方法：POST /cloud/hub/get_uptoken 
	入参：{
  "account": "string",   //用户ID
  "file_name": "string",  //文件名称
  "file_size": 0,     //文件大小
  "device_sn": "string",   //设备SN
  "type": 0  //文件类型 1-基站异常 2-移动端异常 3-基站监控缩略图 4-基站监控视频
}
	返回：{
  "code": 0,
  "msg": "xxxxxx",
  "data": {
    "uptoken": "url"
    "key_prefix": "key_prefix"
  }
}
============================================================================*/
zx_Json *zx_hub_uptoken_request (
const char *account,  		//用户ID 
const char *file_name ,   	// 文件名称
unsigned long file_size ,  	// 文件大小
const char *device_sn ,   	//设备SN 
unsigned int type,  		//文件类型 1-基站异常 2-移动端异常 3-基站监控缩略图 4-基站监控视频
bool disable_ssl,   		//是否禁用ssl
zx_Client client
);

/*============================================================================
方法：POST /cloud/hub/get_dntoken 
	入参：{
  "account": "string",  //用户ID
  "file_key": "string",  //文件keywfhg
  "station_sn": "string"   //基站SN
}
	返回：{
  "code": 0,
  "msg": "xxxxxx",
  "data": {
    "storage_url": "url"}
}
}
============================================================================*/
zx_Json *zx_hub_dntoken_request (
const char *account ,   	//用户ID
const char *file_key,   	//文件keywfhg
const char *station_sn,  	//基站SN
zx_Client client
);

/*============================================================================
方法：POST /security/upgrade/fetch-rom-version
	入参：{
  "current_version_name": "",
  "device_type": "Station",
  "rom_version": 1,
  "sn": "sn1"
}
	返回：{
  "res_code": 1,
  "message": "",
  "data": [
    {
      "device_type": "Station",
      "rom_version": 1,
      "rom_version_name": "v1",
      "introduction": "station v1",
      "upgrade_flag": 1,
      "upgrade_type": 1,
      "force_upgrade": false,
      "full_package": {
        "file_path": "http://security-ci.s3-us-west-2.amazonaws.com/security/d339c3a3-068d-45c0-802a-00614cbb1eb8_logo.png",
        "file_size": 5087,
        "file_name": "d339c3a3-068d-45c0-802a-00614cbb1eb8_logo.png",
        "file_md5": "8d2b7f3d00f50b8aebb7d1c002c64ca1"
      }
    },
    {
      "device_type": "Camera",
      "rom_version": 1,
      "rom_version_name": "v1",
      "introduction": "camera v1",
      "upgrade_flag": 1,
      "upgrade_type": 2,
      "force_upgrade": false,
      "full_package": {
        "file_path": "http://security-ci.s3-us-west-2.amazonaws.com/security/7fe0f613-a877-48a7-acb8-e77cb91dbe57_filename.png",
        "file_size": 7668,
        "file_name": "7fe0f613-a877-48a7-acb8-e77cb91dbe57_filename.png",
        "file_md5": "c2e75bf4c8417f57b7d81bd813a3ce76"
      }
    }
  ]
}
============================================================================*/
zx_Json *zx_rom_upgrade_request (
const char *current_version_name, //当前对应device_type 的版本名称
const char *device_type,          // device type的名称,实际是产品硬件版本，包含了产品信息和硬件版本，后端通过该字段来决定客户端需要的是那种固件包
unsigned long rom_version,        // rom的版本号
const char *sn,                   // 序列号
zx_Client client
);

int zx_get_josn_string(const char *sn, const short channel, const char *filepath, const short storge_type, char *out_str);

zx_Error zx_get_hub_bind_status(const char* account_id, const char *station_sn);

zx_Error zx_update_favorite_history_record(const char* account_id, const char* station_sn, int id, int mode);

zx_Error zx_delete_hub_history_record(const char* account_id, const char* station_sn, int id);

//zx_Error zx_delete_hub_history_records(const char* account_id, const char* station_sn, int num, const RECORD_INFO* pRecords);

zx_Error zx_delete_hub_history_record_by_file(const char* account_id,
                                                        const char* station_sn,
                                                        const char* local_path);
#if 0
zx_Error zx_hub_add_face_features(const char* account_id,
                                         const char* station_sn,
                                         const char* pfacefeature,
                                         const char* pimage,
                                         AI_FACE_FEATURE * pfaceinfo);

zx_Error zx_hub_update_face_features(const char* account_id,
                                             const char* station_sn,
                                             const char* pfacefeature,  /* base64 */
                                             const char* pimage,        /* jep->base64 */                                             
                                             AI_FACE_FEATURE * pfaceinfo);
#endif
zx_Error zx_hub_delete_faces_features(const char* account_id, const char* station_sn, int user_id);

zx_Error zx_upload_hub_history_record(     const char* account_id,
                                              const char* station_sn,
                                              const char* device_sn,
                                              const char* local_path,
                                              zx_Uint64 start_time,
                                              zx_Uint64 end_time,
                                              zx_Uint32 framenum,
                                              const char* cloud_path,
                                              int storagetype,
                                              const char* thumb_data,
                                              const int* pface_id);

zx_Error zx_get_cloud_storage_flag(const char* account_id, const char* device_sn);

void zx_deinit_as_interface(void);

#ifdef __cplusplus
}
#endif

#pragma pack()

#endif /* zx_AS_INTERFACE_H */
