/*
 ============================================================================
 Name        : as_interface.c
 Author      : Oceanwing.com
 Copyright   : 2017(c) Shenzhen Oceanwing Smart Innovations Co., Ltd.
 Create      : lucien.liu
 date        : 2017-10-30 
 Description :
 ============================================================================
 */

#include "as_interface.h"
#include "PPCS/PPCS_Error.h"
#include "mgw_interface.h"
#include "local_storage_interface.h"
#include "ai_interface.h"
//#include "xm_interface.h"
#include "ppcs_interface.h"
//#include "nvram.h"

/*
应用服务器分布
    security-app.eufylife.com     // USA 线上
    security-app-qa.eufylife.com  // USA 测试用
    security-app-eu.eufylife.com  // EU
*/
//char *AS_HOST = "https://security-app.eufylife.com";
char *AS_HOST = "https://security-app-qa.eufylife.com";

extern HUB_BASE_PARAM *base_param;
extern char as_server_index;
extern char g_hub_sn[DEVICE_SN_LEN + 1];



char* zx_String_Concat2(const char* s1, const char* s2)
{
	size_t len1 = strlen(s1);
	size_t len2 = strlen(s2);
	char* p = (char*)malloc(len1 + len2 + 1);
	memcpy(p, s1, len1);
	memcpy(p + len1, s2, len2);
	p[len1 + len2] = '\0';
	return p;
}

//zx_Client gclient;

/*============================================================================
方法：post /hub/get_p2p_connectinfo
入参：{
  		"station_sn": "string"             //基站SN
	}
返回：
{
  "code": 0,
  "msg": "succeed",
  "data": {
    "did_id": 5,
    "station_sn": "T8001G0317480001",
    "p2p_did": "SECCAMA-000102-GGSUD",
    "push_did": "SECCAMA-000102-GGSUD",
    "p2p_license": "DSVJYI",
    "push_license": "DWEZWV",
    "server_id": 1,
    "create_time": 1511524430,
    "status": 1,
    "p2p_conn": "EFGBFFBJKAJPGDJMENGMFOECHEMHHJLBANEFBFFLDLPEIFJBFMDLEENBBFICPFJJHIKCIDEGMJPLFPGPLLOONOBBJNLBAKGNACGBDFEONNLPEOHAAJBIPNNL",
    "app_conn": "EBGCFGBNKNIJGJJCEDHJFAEPGHNEHCMEHIFBBJDGAPJMLHKDDDAFCKPCGNLJIJLDAFNPKEDJPFNKBKCJIB",
    "wipn_enc_dec_key": "ZXSecurity17Cam@",
    "wipn_ndt_aes128key": "ZXSecurity17Cam@",
    "query_server_did": "SECCAMA-000003-WJTKZ;SECCAMA-000004-DJKCF;SECCAMA-000005-CUDDD;SECCAMA-000006-GJTJZ;SECCAMA-000007-FZFXK;SECCAMA-000008-YDKKL",
    "query_servers": [
      "SECCAMA-000003-WJTKZ",
      "SECCAMA-000004-DJKCF",
      "SECCAMA-000005-CUDDD",
      "SECCAMA-000006-GJTJZ",
      "SECCAMA-000007-FZFXK",
      "SECCAMA-000008-YDKKL"
    ]
  }
}
}const char* station_sn, char *did, char *init_str, char *p2p_license, char *push_license
============================================================================*/
zx_Error zx_hub_get_p2p_params(const char* station_sn, P2P_BASE_INFO *p2p_info)
{
	zx_Error err;
	if (station_sn && p2p_info)
	{
		zx_Client client;
		char *s_json_param = NULL;
		cJSON *res_root = NULL, *param_root = NULL;

		err.code = -1; 

		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
		char* url = zx_String_Concat2(AS_HOST, "/v1/hub/get_p2p_connectinfo");
		if (url)
		{		
    		param_root = cJSON_CreateObject();
			if (param_root)
			{			
				cJSON_AddStringToObject(param_root,"station_sn",station_sn);			
       			s_json_param = cJSON_PrintUnformatted(param_root);
        		if(s_json_param)
				{
					err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
					//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
					//dzlog_info("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    				//dzlog_info("response body:%s", zx_Buffer_CStr(&client.b));
					if (err.code == AS_HTTP_200) 
					{
						err.code = zx_Json_GetInt(res_root, "code", 0);
						dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
						if (err.code == 0)
						{
                        	cJSON *js_list = cJSON_GetObjectItem(res_root, "data");
							char *s_result = cJSON_PrintUnformatted(js_list);
							//dzlog_info("%s", s_result);
							const char *sdid = zx_Json_GetString(js_list, "p2p_did", NULL);
							memcpy(p2p_info->p2p_uid, sdid, strlen(sdid));
#if USE_DOMAIN_NAME
							const char *sinit_str = zx_Json_GetString(js_list, "p2p_conn", NULL);
#else
							const char *sinit_str = zx_Json_GetString(js_list, "app_conn", NULL);
#endif
							memcpy(p2p_info->p2p_initstr, sinit_str, strlen(sinit_str));
							const char *sp2p_license = zx_Json_GetString(js_list, "p2p_license", NULL);
							memcpy(p2p_info->p2p_license, sp2p_license, strlen(sp2p_license));
							const char *spush_license = zx_Json_GetString(js_list, "push_license", NULL);
							memcpy(p2p_info->push_license, spush_license, strlen(spush_license));
							const char *sNDT_license = zx_Json_GetString(js_list, "ndt_license", NULL);
							memcpy(p2p_info->NDT_license, sNDT_license, strlen(sNDT_license));

							const char * wipn_enc_dec_key =  zx_Json_GetString(js_list, "wipn_enc_dec_key", NULL);
							memcpy(p2p_info->push_encdec_key, wipn_enc_dec_key, strlen(wipn_enc_dec_key));
							const char * wipn_ndt_aes128key =  zx_Json_GetString(js_list, "wipn_ndt_aes128key", NULL);
							memcpy(p2p_info->push_aes128_key, wipn_ndt_aes128key, strlen(wipn_ndt_aes128key));
							const char * query_server_did = zx_Json_GetString(js_list, "query_server_did", NULL);
							memcpy(p2p_info->push_query_ser_list, query_server_did, strlen(query_server_did));
							dzlog_info("init_str=%s p2p_uid=%s push_query_ser_list=%s...", sinit_str, sdid, p2p_info->push_query_ser_list);
							free(s_result);	
						}
					}
					else
				  		dzlog_error("err.code=%d  msg=%s",err.code , err.message);
            		free(s_json_param);
        		}
        		cJSON_Delete(param_root);
    		}
			free(url);	
		}
		zx_Client_Cleanup(&client);
	}
	
	return err;
}

/*============================================================================
方法：post /hub/bind_devs 
	入参：{
  			"device_channel": 0,        //基站wifi信道
  			"device_sn": "string",       //设备SN号
  			"station_sn": "string"       //基站SN号 
}
	返回：{
          	"code": 0,                
"msg": "xxxxxxxxxxx"
}
============================================================================*/
zx_Error zx_hub_bind_devs(const char* station_sn, const char* device_sn, const char* device_name,
	              	      const int device_channel, const int dev_type, const char *account_id)
{
	zx_Error err;
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/bind_devs");
	if (url)
	{		
		zx_Client client;
		char *s_json_param = NULL;
		cJSON *res_root = NULL, *param_root = NULL;
		err.code = -1;
		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
    	param_root = cJSON_CreateObject();
		if (param_root)
		{
			cJSON_AddStringToObject(param_root,"station_sn",station_sn);
			cJSON_AddStringToObject(param_root,"device_sn",device_sn);
			cJSON_AddStringToObject(param_root,"account",account_id);
			cJSON_AddStringToObject(param_root,"device_name",device_name);
			cJSON_AddNumberToObject(param_root,"device_type",dev_type);
    		cJSON_AddNumberToObject(param_root,"device_channel",device_channel);
			//cJSON_AddStringToObject(param_root,"device_wifi_mac",device_wifi_mac);
			//cJSON_AddStringToObject(param_root,"device_sub1g_mac",device_sub1g_mac);

			s_json_param = cJSON_PrintUnformatted(param_root);
			if(s_json_param)
			{
				dzlog_info("%s", s_json_param);

				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				//dzlog_info("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    			//dzlog_info("response body:\n%s", zx_Buffer_CStr(&client.b));
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",err.code , zx_Json_GetString(res_root, "msg", 0));
				}
				else
				  	dzlog_error("err.code=%d  msg=%s",err.code , err.message);
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
		zx_Client_Cleanup(&client);
	}
	return err;
}

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
zx_Error zx_hub_unbind_devs(const char* station_sn, const char* device_sn, const char *account_id)
{
	zx_Error err;
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/unbind_devs");
	
	if (url)
	{		
		char *s_json_param = NULL;
		cJSON *res_root = NULL, *param_root = NULL;
		zx_Client client;
		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
		err.code = -1;

    	param_root = cJSON_CreateObject();
		if (param_root)
		{
			cJSON_AddStringToObject(param_root,"station_sn",station_sn);
			cJSON_AddStringToObject(param_root,"device_sn",device_sn);
			cJSON_AddStringToObject(param_root,"account",account_id);

       		s_json_param = cJSON_PrintUnformatted(param_root);
        	if(s_json_param)
			{
				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
				}
				else
				  	dzlog_error("err.code=%d  msg=%s",err.code , err.message);
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
		zx_Client_Cleanup(&client);
	}

	return err;
}

/*============================================================================
方法：post /hub/check_device 
	入参：{
  			"device_sn": "string",      //设备SN
            “station_sn”:”string”      //基站SN
		  }
	返回：{
          	"code": 0,        //20006：已经绑定别的基站  20005：已绑定给定基站  0、没有绑定
			"msg": "xxxxxxxxxxx"
		  }
============================================================================*/
zx_Error zx_hub_check_devs(const char* device_sn, const char* station_sn)
{
	zx_Error err;
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/check_device");
	if (url)
	{		
		zx_Client client;
		char *s_json_param = NULL;
		cJSON *res_root = NULL, *param_root = NULL;
		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
		err.code = -1;

    	param_root = cJSON_CreateObject();
		if (param_root)
		{
			cJSON_AddStringToObject(param_root,"station_sn",station_sn);
			cJSON_AddStringToObject(param_root,"device_sn",device_sn);
			
       		s_json_param = cJSON_PrintUnformatted(param_root);
        	if(s_json_param)
			{
				//dzlog_info("******call start******");
				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				//dzlog_info("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    			//dzlog_info("response body:\n%s", zx_Buffer_CStr(&client.b));
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
				}
				else
				  	dzlog_error("err.code=%d  msg=%s",err.code , err.message);
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
		zx_Client_Cleanup(&client);
	}

	return err;
}

/*============================================================================
	方法：post /hub/update_devs_info 
	入参：{
  "account": "string",
  "device_channel": 0,
  "device_model": "string",
  "device_name": "string",
  "device_sn": "string",
  "device_type": 0,
  "main_hw_version": "string",
  "main_sw_version": "string",
  "schedule": "string",
  "schedulex": "string",
  "sec_hw_version": "string",
  "sec_sw_version": "string",
  "station_sn": "string",
  "sub1g_mac": "string",
  "wifi_mac": "string"
}
	返回：{
          	"code": 0,                
			"msg": "xxxxxxxxxxx"
          }
============================================================================*/

zx_Error zx_hub_edit_devs_strinfo_by_paramname(const char* device_sn, const char* station_sn, const char *account, 
                     const char* param_name, const char* value)
{
	zx_Error err;
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/update_devs_info");
	if (url && device_sn && station_sn)
	{		
		zx_Client client;
		char *s_json_param = NULL;
		cJSON *res_root = NULL, *param_root = NULL;
		err.code = -1;

		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
      	if (strlen(device_sn) && strlen(station_sn))
      	{
    		param_root = cJSON_CreateObject();
			if (param_root)
			{
				cJSON_AddStringToObject(param_root, "station_sn", station_sn);
				cJSON_AddStringToObject(param_root, "device_sn", device_sn);
				cJSON_AddStringToObject(param_root, "account", account);
				cJSON_AddStringToObject(param_root, param_name, value);

       			s_json_param = cJSON_PrintUnformatted(param_root);
        		if(s_json_param)
				{
					//dzlog_info("******call start******");
					//dzlog_info("%s", s_json_param);
					err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
					//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
					//dzlog_info("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    				//dzlog_info("response body:\n%s", zx_Buffer_CStr(&client.b));
					if (err.code == AS_HTTP_200) 
					{
						//s_result = cJSON_PrintUnformatted(res_root);
						err.code = zx_Json_GetInt(res_root, "code", 0);
            			dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
					}
					else
				  		dzlog_error("err.code=%d  msg=%s",err.code , err.message);
            		free(s_json_param);
        		}
        		cJSON_Delete(param_root);
        	}
      	}
      	free(url);
		zx_Client_Cleanup(&client);
	}
	
	return err;
}


zx_Error zx_hub_edit_devs_intinfo_by_paramname(const char* device_sn, const char* station_sn, const char *account, 
                     const char* param_name, int value)
{
	zx_Error err;
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/update_devs_info");
	
	if (url)
	{		
		zx_Client client;
		char *s_json_param = NULL;
		cJSON *res_root = NULL, *param_root = NULL;
		err.code = -1;
		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);

    	param_root = cJSON_CreateObject();
		if (param_root)
		{
			cJSON_AddStringToObject(param_root, "station_sn", station_sn);
			cJSON_AddStringToObject(param_root, "device_sn", device_sn);
			cJSON_AddStringToObject(param_root, "account", account);
    		cJSON_AddNumberToObject(param_root, param_name, value);

       		s_json_param = cJSON_PrintUnformatted(param_root);
        	if(s_json_param)
			{
				//dzlog_info("******call start******");
				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				//dzlog_info("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    			//dzlog_info("response body:\n%s", zx_Buffer_CStr(&client.b));
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
				}
				else
				  	dzlog_error("err.code=%d  msg=%s",err.code , err.message);
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
		zx_Client_Cleanup(&client);
	}
	
	return err;
}


zx_Error zx_update_devs_version(const char* device_sn, const char* station_sn, const char *account, 
                     const char* main_sw_version, const char *sec_sw_version, const char *hw_version)
{
	zx_Error err;
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/update_devs_info");
	
	if (url)
	{		
		zx_Client client;
		char *s_json_param = NULL;
		cJSON *res_root = NULL, *param_root = NULL;
		err.code = -1;
		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);

    	param_root = cJSON_CreateObject();
		if (param_root)
		{
			cJSON_AddStringToObject(param_root, "station_sn", station_sn);
			cJSON_AddStringToObject(param_root, "device_sn", device_sn);
			cJSON_AddStringToObject(param_root, "account", account);
    		cJSON_AddStringToObject(param_root, "main_sw_version", main_sw_version);
			cJSON_AddStringToObject(param_root, "sec_sw_version", sec_sw_version);
			cJSON_AddStringToObject(param_root, "main_hw_version", hw_version);

       		s_json_param = cJSON_PrintUnformatted(param_root);
        	if(s_json_param)
			{
				//dzlog_info("******call start******");
				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				//dzlog_info("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    			//dzlog_info("response body:\n%s", zx_Buffer_CStr(&client.b));
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
				}
				else
				  	dzlog_error("err.code=%d  msg=%s",err.code , err.message);
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
		zx_Client_Cleanup(&client);
	}
	
	return err;
}


zx_Error zx_hub_edit_devs_name(const char* device_sn, const char* station_sn, const char* device_name, const char *account)
{
	zx_Error err;
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/update_devs_info");
	
	if (url)
	{
		char *s_json_param = NULL;
		cJSON *res_root = NULL, *param_root = NULL;
		zx_Client client;
		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
		err.code = -1;

    	param_root = cJSON_CreateObject();
		if (param_root)
		{
			cJSON_AddStringToObject(param_root,"station_sn",station_sn);
			cJSON_AddStringToObject(param_root,"device_sn",device_sn);
			cJSON_AddStringToObject(param_root,"device_name",device_name);
			cJSON_AddStringToObject(param_root,"account",account);

       		s_json_param = cJSON_PrintUnformatted(param_root);
        	if(s_json_param)
			{
				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json;charset=utf-8", "POST");
				//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
				}
				else
				  	dzlog_error("err.code=%d  msg=%s",err.code , err.message);
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
		zx_Client_Cleanup(&client);
	}
	
	return err;
}

/*============================================================================
方法：post /hub/update_hub_info 
入参：{
  "account": "string",
  "ip_addr": "string",
  "main_hw_version": "string",
  "main_sw_version": "string",
  "sec_hw_version": "string",
  "sec_sw_version": "string",
  "station_model": "string",
  "station_name": "string",
  "station_sn": "string",
  "sub1g_mac": "string",
  "time_zone": "string",
  "wifi_mac": "string",
  "wifi_ssid": "string"
}
返回：{
          	"code": 0,                
"msg": "xxxxxxxxxxx"
}
============================================================================*/

zx_Error zx_hub_edit_hub_info(const char* station_sn, const char* station_name, const char* time_zone,
                                            const char* ip_addr, const char* wifi_ssid, const char* account)
{
	zx_Error err;
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/update_hub_info");
	if (url)
	{
		char *s_json_param = NULL;
		cJSON *res_root = NULL, *param_root = NULL;
		zx_Client client;
		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
		err.code = -1;

    	param_root = cJSON_CreateObject();
		if (param_root)
		{
			cJSON_AddStringToObject(param_root,"station_sn",station_sn);
			cJSON_AddStringToObject(param_root,"account",account);
			cJSON_AddStringToObject(param_root,"time_zone",time_zone);
			cJSON_AddStringToObject(param_root,"station_name",station_name);
			//cJSON_AddNumberToObject(param_root,"device_type",device_type);
    		//cJSON_AddNumberToObject(param_root,"device_channel",device_channel);
			cJSON_AddStringToObject(param_root,"ip_addr",ip_addr);
			cJSON_AddStringToObject(param_root,"wifi_ssid",wifi_ssid);

       		s_json_param = cJSON_PrintUnformatted(param_root);
        	if(s_json_param)
			{
				//dzlog_info("******call start******");
				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				//dzlog_info("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    			//dzlog_info("response body:\n%s", zx_Buffer_CStr(&client.b));
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
				}
				else
				  	dzlog_error("err.code=%d  msg=%s",err.code , err.message);
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
		zx_Client_Cleanup(&client);
	}
	
	return err;
}


zx_Error zx_hub_update_info_by_infoname(const char* station_sn, const char* account, const char* infoname, const char *value)
{
	zx_Error err;
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/update_hub_info");
	if (url)
	{
		char *s_json_param = NULL;
		cJSON *res_root = NULL, *param_root = NULL;
		zx_Client client;
		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
		err.code = -1;

		param_root = cJSON_CreateObject();
		if (param_root && infoname && station_sn && account && strlen(account)>0)
		{
			cJSON_AddStringToObject(param_root,"station_sn",station_sn);
			cJSON_AddStringToObject(param_root,"account",account);
			cJSON_AddStringToObject(param_root, infoname, value);

			s_json_param = cJSON_PrintUnformatted(param_root);
			if(s_json_param)
			{
				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
					dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
				}
				else
				  	dzlog_error("err.code=%d  msg=%s",err.code , err.message);
				free(s_json_param);
			}
			cJSON_Delete(param_root);
		}
		free(url);
		zx_Client_Cleanup(&client);
	}
	
	return err;
}

/*
  {
    "account": "string",
    "ai_algor_version": "string",
    "ai_kernel_version": "string",
    "ai_rootfs_version": "string",
    "ip_addr": "string",
    "main_hw_version": "string",
    "main_sw_version": "string",
    "sec_hw_version": "string",
    "sec_sw_version": "string",
    "station_model": "string",
    "station_name": "string",
    "station_sn": "string",
    "sub1g_mac": "string",
    "time_zone": "string",
    "volume": "string",
    "wifi_mac": "string",
    "wifi_ssid": "string"
  }

*/
zx_Error zx_hub_update_all_info(void)
{
	zx_Error err;	
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/update_hub_info");
	err.code = -1;

	if (url)
	{
		char *s_json_param = NULL;
		cJSON *res_root = NULL, *param_root = NULL;
		zx_Client client;
		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);

    	param_root = cJSON_CreateObject();
		if (param_root)
		{
			cJSON_AddStringToObject(param_root,"station_sn", base_param->hub_info.hub_sn);
			cJSON_AddStringToObject(param_root,"account", base_param->hub_info.account_id);
			cJSON_AddStringToObject(param_root, "main_sw_version", base_param->hub_info.hub_software_main_ver);
			cJSON_AddStringToObject(param_root, "sec_sw_version", base_param->hub_info.hub_hardware_sec_ver);   // sub1g
			cJSON_AddStringToObject(param_root, "volume", base_param->hub_info.tf_card_label);
			cJSON_AddStringToObject(param_root, "ip_addr", base_param->hub_info.hub_wlan_ip_addr);
			cJSON_AddStringToObject(param_root, "ai_algor_version", base_param->hub_info.ai_algorversion);
			cJSON_AddStringToObject(param_root, "ai_kernel_version", base_param->hub_info.ai_kernelversion);
			cJSON_AddStringToObject(param_root, "ai_rootfs_version", base_param->hub_info.ai_rootfsversion);
			s_json_param = cJSON_PrintUnformatted(param_root);
			if(s_json_param)
			{
				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
				}
				else
				  	dzlog_error("err.code=%d  msg=%s",err.code , err.message);
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
		zx_Client_Cleanup(&client);
	}
	
	return err;
}

/*============================================================================
	方法：post /hub/get_dev_info 
	入参：{
  			"device_sn": "string",      //设备SN
            “station_sn”:”string”      //基站SN
		  }
	返回：{
  			"code": 0,
  			"data": {
    				"create_time": 0,            //创建时间
    				"device_channel": 0,         //与基站的通信信道
    				"device_id": 0,              //序列号
    				"device_model": "string",     //设备型号
    				"device_name": "string",      //设备名称
    				"device_sn": "string",         //设备sn
    				"device_type": 0,            //设备类型
    				"station_sn": "string",        //基站sn
					"wifi_mac": "string",         //wifi mac
					"sub1g_mac": "string",       //sub1g mac
    				"status": 1                 //状态
  					},
  			"msg": "成功"
		}
============================================================================*/
zx_Error zx_hub_get_devs_info(const char* station_sn, const char* device_sn)
{
	zx_Error err;	
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/get_dev_info");
	err.code = -1;
	if (url)
	{
		zx_Client client;
		char *s_json_param = NULL;
		cJSON *res_root = NULL, *param_root = NULL;
		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);

    	param_root = cJSON_CreateObject();
		if (param_root)
		{
			cJSON_AddStringToObject(param_root,"station_sn",station_sn);
			cJSON_AddStringToObject(param_root,"device_sn",device_sn);

       		s_json_param = cJSON_PrintUnformatted(param_root);
        	if(s_json_param)
			{
				//dzlog_info("******call start******");
				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				//dzlog_info("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    			//dzlog_info("response body:\n%s", zx_Buffer_CStr(&client.b));
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
				}
				else
				  	dzlog_error("err.code=%d  msg=%s",err.code , err.message);
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
		zx_Client_Cleanup(&client);
	}
	
	return err;
}

/*============================================================================
	方法：post /hub/get_devs_params 
	入参：{
  			"device_sn": "string",      //设备SN
            “station_sn”:”string”      //基站SN
          }
	返回：{
  			"code": 0,
  			"data": [
    		{
     			"device_sn": "string",         //设备sn
      			"param_id": 0,              //参数ID
      			"param_type": 0,            //???
      			"param_value": "string",      //参数值
      			"status": 1
    		 } ],
  			"msg": "成功"
		 }
============================================================================*/
zx_Error zx_hub_get_devs_params(const char* station_sn, const char* device_sn)
{
	zx_Error err;
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/get_devs_params");
	err.code = -1;
	if (url)
	{
		zx_Client client;
		char *s_json_param = NULL;
		cJSON *res_root = NULL, *param_root = NULL;
		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);

    	param_root = cJSON_CreateObject();
		if (param_root)
		{
			cJSON_AddStringToObject(param_root,"station_sn",station_sn);
			cJSON_AddStringToObject(param_root,"device_sn",device_sn);

       		s_json_param = cJSON_PrintUnformatted(param_root);
        	if(s_json_param)
			{
				//dzlog_info("******call start******");
				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				//dzlog_info("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    			//dzlog_info("response body:\n%s", zx_Buffer_CStr(&client.b));
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
				}
				else
				  	dzlog_error("err.code=%d  msg=%s",err.code , err.message);
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
		zx_Client_Cleanup(&client);
	}
	
	return err;
}

/*============================================================================
	方法：post /hub/get_hub_params 
	入参：{
  			"station_sn": "string",     //基站SN
        	“account”:”string”          //账号信息
		 }
	返回：{
  			"code": 0,
  			"data": [
    		{
      			"device_sn": "string",         //设备sn
      			"param_id": 0,              //参数ID
      			"param_type": 0,            //???
      			"param_value": "string",      //参数值
      			"status": 1
    		 } ],
  			"msg": "成功"
		 }
============================================================================*/
zx_Error zx_hub_get_hub_params(const char* station_sn, const char* account)
{
	zx_Error err;
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/get_hub_params");
	err.code = -1;
	if (url)
	{
		zx_Client client;
		char *s_json_param = NULL;
		cJSON *res_root = NULL, *param_root = NULL;
		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
    	param_root = cJSON_CreateObject();
		if (param_root)
		{
			cJSON_AddStringToObject(param_root,"station_sn",station_sn);
			cJSON_AddStringToObject(param_root,"account",account);

       		s_json_param = cJSON_PrintUnformatted(param_root);
        	if(s_json_param)
			{
				//dzlog_warn("******call start******");
				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				//dzlog_warn("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    			//dzlog_warn("response body:\n%s", zx_Buffer_CStr(&client.b));
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
				}
				else
				  	dzlog_error("err.code=%d  msg=%s",err.code , err.message);
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
		zx_Client_Cleanup(&client);
	}
	
	return err;
}

/*============================================================================
方法：post /hub/get_rtsp_connectinfo 
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
zx_Error zx_get_rtsp_connectinfo(const char* station_sn, const char* decvice_sn)
{
	zx_Error err;
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/get_rtsp_connectinfo");
	err.code = -1;
	if (url)
	{
		zx_Client client;
		char *s_json_param = NULL;
		cJSON *res_root = NULL, *param_root = NULL;
		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
    	param_root = cJSON_CreateObject();
		if (param_root)
		{
			cJSON_AddStringToObject(param_root,"station_sn",station_sn);
			cJSON_AddStringToObject(param_root,"device_sn",decvice_sn);

       		s_json_param = cJSON_PrintUnformatted(param_root);
        	if(s_json_param)
			{
				//dzlog_warn("******call start******");
				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				//dzlog_warn("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    			//dzlog_warn("response body:\n%s", zx_Buffer_CStr(&client.b));
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
				}
				else
				  	dzlog_error("err.code=%d  msg=%s",err.code , err.message);
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
		zx_Client_Cleanup(&client);
	}
	
	return err;
}

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
zx_Error zx_hub_reset(const char* station_sn, const char* account)
{
	zx_Error err;
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/reset");
	err.code = -1;
	if (url)
	{
		zx_Client client;
		char *s_json_param = NULL;
		cJSON *res_root = NULL, *param_root = NULL;
		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
    	param_root = cJSON_CreateObject();
		if (param_root)
		{
			cJSON_AddStringToObject(param_root,"station_sn",station_sn);
			cJSON_AddStringToObject(param_root,"account",account);

       		s_json_param = cJSON_PrintUnformatted(param_root);
        	if(s_json_param)
			{
				//dzlog_warn("******call start******");
				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				//dzlog_warn("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    			//dzlog_warn("response body:\n%s", zx_Buffer_CStr(&client.b));
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
				}
				else
				  	dzlog_error("err.code=%d  msg=%s",err.code , err.message);
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
		zx_Client_Cleanup(&client);
	}
	
	return err;
}

/*============================================================================
方法：post /hub/upload_door_sensor_info 修改设备参数
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
zx_Error zx_upload_door_sensor_info(const char* account_id, const char* station_sn,DEV_BASE_PARAM *dev_info)
{
	zx_Error err;
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/upload_devs_params");
	err.code = -1;
	if (url && account_id && station_sn  && dev_info && dev_info->dev_sn)
	{
		char *s_json_param = NULL;
		cJSON *res_root = NULL, *param_root = NULL, *param_list = NULL, *param_bat_state = NULL, *param_alarm_enable = NULL, *param_sensor_status = NULL, *param_time = NULL,*param_dev_status = NULL;
		zx_Client client;
		char param_value_string[11] = {0};

		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
      	if (strlen(station_sn)==DEVICE_SN_LEN && strlen(dev_info->dev_sn)==DEVICE_SN_LEN) //SN合法
      	{
			param_root = cJSON_CreateObject();
			param_list = cJSON_CreateArray();
			param_bat_state = cJSON_CreateObject();
			param_alarm_enable = cJSON_CreateObject();
			param_sensor_status = cJSON_CreateObject();
			param_time = cJSON_CreateObject();
			param_dev_status = cJSON_CreateObject();
				
			if (param_root && param_list && param_bat_state && param_alarm_enable && param_sensor_status && param_time)
			{
				cJSON_AddStringToObject(param_root,"station_sn",station_sn);
				cJSON_AddStringToObject(param_root,"device_sn",dev_info->dev_sn);
				cJSON_AddStringToObject(param_root,"account",account_id);

                cJSON_AddItemToArray(param_list, param_bat_state);
                cJSON_AddItemToArray(param_list, param_alarm_enable);
                cJSON_AddItemToArray(param_list, param_sensor_status);
                cJSON_AddItemToArray(param_list, param_time);
                cJSON_AddItemToArray(param_list, param_dev_status);	
				cJSON_AddItemToObject(param_root,"params", param_list);
			
			       cJSON_AddNumberToObject(param_dev_status, "param_type", APP_CMD_GET_DEV_STATUS);
				cJSON_AddStringToObject(param_dev_status, "param_value", "1");
				cJSON_AddNumberToObject(param_bat_state, "param_type", APP_CMD_ENTRY_SENSOR_BAT_STATE);
				cJSON_AddStringToObject(param_bat_state, "param_value", (dev_info->dev_door_sensor_is_battery_low == 0)?"0":"1");
				cJSON_AddNumberToObject(param_alarm_enable, "param_type", APP_CMD_DOOR_SENSOR_ALARM_ENABLE);
				cJSON_AddStringToObject(param_alarm_enable, "param_value", "1");
				cJSON_AddNumberToObject(param_sensor_status, "param_type", APP_CMD_ENTRY_SENSOR_STATUS);
				cJSON_AddStringToObject(param_sensor_status, "param_value", (dev_info->dev_door_sensor_state == 0)?"0":"1");
				sprintf(param_value_string,"%ld",time(0));
				cJSON_AddNumberToObject(param_time, "param_type", APP_CMD_ENTRY_SENSOR_CHANGE_TIME);
				cJSON_AddStringToObject(param_time, "param_value", param_value_string);
            	s_json_param = cJSON_PrintUnformatted(param_root);
				dzlog_info("json string:%s",s_json_param);
            	if(s_json_param)
            	{
					//dzlog_info("%s", s_json_param);
					err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
					//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
					if (err.code == AS_HTTP_200) 
					{
						//s_result = cJSON_PrintUnformatted(res_root);
						err.code = zx_Json_GetInt(res_root, "code", 0);
						dzlog_info("door sensor info uploade: code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
					}
					else
					{
						dzlog_error("err.code=%d  msg=%s",err.code , err.message);
					}
                	free(s_json_param);
            	}
            	cJSON_Delete(param_root);
        	}
      	}
      	free(url);
		zx_Client_Cleanup(&client);
	}
	
	return err;
}


zx_Error zx_upload_door_sensor_evt(const char* account_id, const char* station_sn,DEV_BASE_PARAM *dev_info)
{
	zx_Error err;
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/upload_devs_params");
	err.code = -1;
	dzlog_info("enter zx_upload_door_sensor_evt.");
	if (url && account_id && station_sn  && dev_info && dev_info->dev_sn)
	{
		char *s_json_param = NULL;
		cJSON *res_root = NULL, *param_root = NULL, *param_list = NULL,*param_sensor_status = NULL, *param_time = NULL,*param_dev_status = NULL;
		zx_Client client;
		char param_value_string[11] = {0};

             //dzlog_info("enter zx_upload_door_sensor_evt 2.");
		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
		dzlog_info("station_sn = %s,dev_sn = %s",station_sn,dev_info->dev_sn);
      	if (strlen(station_sn)==DEVICE_SN_LEN && strlen(dev_info->dev_sn)==DEVICE_SN_LEN) //SN合法
      	{
      	       //dzlog_info("enter zx_upload_door_sensor_evt 3.");
			param_root = cJSON_CreateObject();
			param_list = cJSON_CreateArray();
			param_sensor_status = cJSON_CreateObject();
			param_time = cJSON_CreateObject();
			param_dev_status = cJSON_CreateObject();
				
			if (param_root && param_list && param_sensor_status && param_time && param_dev_status)
			{
			       //dzlog_info("enter zx_upload_door_sensor_evt 4.");
				cJSON_AddStringToObject(param_root,"station_sn",station_sn);
				cJSON_AddStringToObject(param_root,"device_sn",dev_info->dev_sn);
				cJSON_AddStringToObject(param_root,"account",account_id);


                           cJSON_AddItemToArray(param_list,param_sensor_status);
                           cJSON_AddItemToArray(param_list,param_time);
                           cJSON_AddItemToArray(param_list,param_dev_status);
				cJSON_AddItemToObject(param_root,"params", param_list);
			
			
				cJSON_AddNumberToObject(param_dev_status, "param_type", APP_CMD_GET_DEV_STATUS);
				cJSON_AddStringToObject(param_dev_status, "param_value", "1");
				cJSON_AddNumberToObject(param_sensor_status, "param_type", APP_CMD_ENTRY_SENSOR_STATUS);
				cJSON_AddStringToObject(param_sensor_status, "param_value", (dev_info->dev_door_sensor_state == 0)?"0":"1");
				sprintf(param_value_string,"%ld",time(0));
				cJSON_AddNumberToObject(param_time, "param_type", APP_CMD_ENTRY_SENSOR_CHANGE_TIME);
				cJSON_AddStringToObject(param_time, "param_value", param_value_string);
            	s_json_param = cJSON_PrintUnformatted(param_root);
				dzlog_info("json string:%s",s_json_param);
            	if(s_json_param)
            	{
					//dzlog_info("%s", s_json_param);
					err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
					//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
					if (err.code == AS_HTTP_200) 
					{
						//s_result = cJSON_PrintUnformatted(res_root);
						err.code = zx_Json_GetInt(res_root, "code", 0);
						dzlog_info("door sensor evt uploade: code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
						if (err.code == AS_HTTP_DEV_NON_ADMIN) 
						{
							dzlog_info("unbind this device.");
							zx_unbind_dev_by_sn(dev_info->dev_sn);
						}
					}
					else
					{
						dzlog_error("err.code=%d  msg=%s",err.code , err.message);
					}
                	free(s_json_param);
            	}
            	cJSON_Delete(param_root);
        	}
      	}
      	free(url);
		zx_Client_Cleanup(&client);
	}
	
	return err;
}

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
zx_Error zx_upload_devs_params(const char* account_id, const char* station_sn, const char* device_sn, 
								int param_type, const char* param_value)
{
	zx_Error err;
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/upload_devs_params");
	err.code = -1;
	if (url && account_id && station_sn && device_sn && param_value)
	{
		char *s_json_param;
		cJSON *res_root = NULL, *param_root = NULL, *param_list = NULL, *param_heard = NULL;
		zx_Client client;
		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
      	if (strlen(station_sn)==DEVICE_SN_LEN && strlen(device_sn)==DEVICE_SN_LEN) //SN合法
      	{
			param_root = cJSON_CreateObject();
			param_heard = cJSON_CreateArray();
			param_list = cJSON_CreateObject();
				
			if (param_root && param_list && param_heard)
			{
				cJSON_AddStringToObject(param_root,"station_sn",station_sn);
				cJSON_AddStringToObject(param_root,"device_sn",device_sn);
				cJSON_AddStringToObject(param_root,"account",account_id);

				cJSON_AddItemToObject(param_heard, "", param_list);
				cJSON_AddItemToObject(param_root,"params", param_heard);
			
				cJSON_AddNumberToObject(param_list, "param_type", param_type);
				cJSON_AddStringToObject(param_list, "param_value", param_value); 			
            	s_json_param = cJSON_PrintUnformatted(param_root);
            	if(s_json_param)
            	{
					//dzlog_info("%s", s_json_param);
					err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
					//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
					if (err.code == AS_HTTP_200) 
					{
						//s_result = cJSON_PrintUnformatted(res_root);
						err.code = zx_Json_GetInt(res_root, "code", 0);
						dzlog_info("code=%d msg=%s, param_type= %d",err.code, zx_Json_GetString(res_root, "msg", 0), param_type);
                        if (err.code == AS_HTTP_DEV_NON_ADMIN)
                        {
                            zx_unbind_dev_by_sn(device_sn);
                        }
                    }
					else
					{
						dzlog_error("err.code=%d  msg=%s",err.code , err.message);
					}
                	free(s_json_param);
            	}
            	cJSON_Delete(param_root);
        	}
      	}
      	free(url);
		zx_Client_Cleanup(&client);
	}
	
	return err;
}

/*============================================================================
方法：post /hub/zx_upload_devs_default_params 设备参数
入参：
{
  "account": "string",
  "device_sn": "string",
  "params": [
    {
      "param_type": 0,
      "param_value": "string"
    }
  ],
  "station_sn": "string"
}

返回：
{
    "code": 0,
    "msg": "xxxxxxxxxxx"
}
============================================================================*/
zx_Error zx_upload_devs_default_params(char channel)
{
    zx_Error err;
    err.code = -1;

    char* url = zx_String_Concat2(AS_HOST, "/v1/hub/upload_devs_params");    
    if (url)
    {
        char *s_json_param;
        cJSON *res_root = NULL, *param_root = NULL, *param_list = NULL, *param_heard = NULL;
        zx_Client client;
        zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);

        int i;
        DEV_BASE_PARAM *pdev_param = NULL;
        for (i = 0; i < MAX_SUB1G_CONNECT; i++)
        {
            if (base_param->dev_param[i].channle_id == channel)
            {
                pdev_param = &base_param->dev_param[i];
                break;
            }
        }
        if (pdev_param != NULL) //SN合法
        {
            param_root = cJSON_CreateObject();
            param_heard = cJSON_CreateArray();
                
            if (param_root && param_heard  && strlen(pdev_param->dev_sn) == DEVICE_SN_LEN)
            {
                char str_value[4] = {0};

                cJSON_AddStringToObject(param_root,"station_sn", base_param->hub_info.hub_sn);
                cJSON_AddStringToObject(param_root,"device_sn", pdev_param->dev_sn);
                cJSON_AddStringToObject(param_root,"account", base_param->hub_info.account_id);

                param_list = cJSON_CreateObject();
                if (param_list)
                {
                    cJSON_AddNumberToObject(param_list, "param_type", APP_CMD_PIR_SWITCH);
                    memset(str_value, 0, sizeof(str_value));
                    snprintf(str_value, sizeof(str_value), "%d", pdev_param->dev_pir_status);
                    cJSON_AddStringToObject(param_list, "param_value", str_value);
                    cJSON_AddItemToObject(param_heard, "", param_list);
                }

                param_list = cJSON_CreateObject();
                if (param_list)
                {
                    cJSON_AddNumberToObject(param_list, "param_type", APP_CMD_SET_PIRSENSITIVITY);
                    memset(str_value, 0, sizeof(str_value));
                    snprintf(str_value, sizeof(str_value), "%d", pdev_param->dev_pir_sensitivity);
                    cJSON_AddStringToObject(param_list, "param_value", str_value);
                    cJSON_AddItemToObject(param_heard, "", param_list);
                }

                param_list = cJSON_CreateObject();
                if (param_list)
                {
                    cJSON_AddNumberToObject(param_list, "param_type", APP_CMD_EAS_SWITCH);
                    memset(str_value, 0, sizeof(str_value));
                    snprintf(str_value, sizeof(str_value), "%d", pdev_param->dev_gsession_status);
                    cJSON_AddStringToObject(param_list, "param_value", str_value);
                    cJSON_AddItemToObject(param_heard, "", param_list);
                }

                param_list = cJSON_CreateObject();
                if (param_list)
                {
                    cJSON_AddNumberToObject(param_list, "param_type", APP_CMD_DEV_LED_SWITCH);
                    memset(str_value, 0, sizeof(str_value));
                    snprintf(str_value, sizeof(str_value), "%d", pdev_param->dev_led_status);
                    cJSON_AddStringToObject(param_list, "param_value", str_value);
                    cJSON_AddItemToObject(param_heard, "", param_list);
                }

                param_list = cJSON_CreateObject();
                if (param_list)
                {
                    cJSON_AddNumberToObject(param_list, "param_type", APP_CMD_IRCUT_SWITCH);
                    memset(str_value, 0, sizeof(str_value));
                    snprintf(str_value, sizeof(str_value), "%d", pdev_param->dev_ir_cut_status);
                    cJSON_AddStringToObject(param_list, "param_value", str_value);
                    cJSON_AddItemToObject(param_heard, "", param_list);
                }

                param_list = cJSON_CreateObject();
                if (param_list)
                {
                    cJSON_AddNumberToObject(param_list, "param_type", APP_CMD_SET_DEV_MIC_VOLUME);
                    memset(str_value, 0, sizeof(str_value));
                    snprintf(str_value, sizeof(str_value), "%d", pdev_param->dev_mic_volume);
                    cJSON_AddStringToObject(param_list, "param_value", str_value);
                    cJSON_AddItemToObject(param_heard, "", param_list);
                }

                param_list = cJSON_CreateObject();
                if (param_list)
                {
                    cJSON_AddNumberToObject(param_list, "param_type", APP_CMD_SET_DEV_SPEAKER_VOLUME);
                    memset(str_value, 0, sizeof(str_value));
                    snprintf(str_value, sizeof(str_value), "%d", pdev_param->dev_speaker_volume);
                    cJSON_AddStringToObject(param_list, "param_value", str_value);
                    cJSON_AddItemToObject(param_heard, "", param_list);
                }                

                param_list = cJSON_CreateObject();
                if (param_list)
                {
                    cJSON_AddNumberToObject(param_list, "param_type", APP_CMD_GET_AWAY_ACTION); 
                    memset(str_value, 0, sizeof(str_value));
                    snprintf(str_value, sizeof(str_value), "%d", pdev_param->away_action);
                    cJSON_AddStringToObject(param_list, "param_value", str_value);
                    cJSON_AddItemToObject(param_heard, "", param_list);
                }
                param_list = cJSON_CreateObject();
                if (param_list)
                {
                    cJSON_AddNumberToObject(param_list, "param_type", APP_CMD_GET_HOME_ACTION); 
                    memset(str_value, 0, sizeof(str_value));
                    snprintf(str_value, sizeof(str_value), "%d", pdev_param->home_action);
                    cJSON_AddStringToObject(param_list, "param_value", str_value);
                    cJSON_AddItemToObject(param_heard, "", param_list);
                }

                param_list = cJSON_CreateObject();
                if (param_list)
                {
                    cJSON_AddNumberToObject(param_list, "param_type", APP_CMD_GET_DEV_STATUS);
                    pdev_param->dev_online_states = 1;
                    cJSON_AddStringToObject(param_list, "param_value", "1");
                    cJSON_AddItemToObject(param_heard, "", param_list);
                }

                param_list = cJSON_CreateObject();
                if (param_list)
                {
                    cJSON_AddNumberToObject(param_list, "param_type", APP_CMD_GET_BATTERY);
                    memset(str_value, 0, sizeof(str_value));
                    snprintf(str_value, sizeof(str_value), "%d", pdev_param->dev_electric_quantity);
                    cJSON_AddStringToObject(param_list, "param_value", str_value);
                    cJSON_AddItemToObject(param_heard, "", param_list);
                }

                cJSON_AddItemToObject(param_root,"params", param_heard);
                s_json_param = cJSON_PrintUnformatted(param_root);
                if(s_json_param)
                {
                    //dzlog_info("%s", s_json_param);
                    err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
                    //dzlog_info("err.code=%d  msg=%s",err.code , err.message);
                    if (err.code == AS_HTTP_200) 
                    {
                        //s_result = cJSON_PrintUnformatted(res_root);
                        err.code = zx_Json_GetInt(res_root, "code", 0);
                        dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
                    }
                    else
                    {
                        dzlog_error("err.code=%d  msg=%s",err.code , err.message);
                    }
                    free(s_json_param);
                }
                cJSON_Delete(param_root);
            }
        }
        free(url);
        zx_Client_Cleanup(&client);
    }

    return err;
}

zx_Error zx_upload_devs_int_params(const char* account_id, const char* station_sn, const char* device_sn, 
									int param_type, const int param_value)
{
	zx_Error err;
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/upload_devs_params");
	err.code = -1;
	if (url && account_id && station_sn && device_sn && param_value)
	{
		char *s_json_param = NULL;
		cJSON *res_root = NULL, *param_root = NULL, *param_list, *param_heard = NULL;
		zx_Client client;
		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
    	param_root = cJSON_CreateObject();
		param_heard = cJSON_CreateArray();
		param_list = cJSON_CreateObject();
				
		if (param_root && param_list && param_heard)
		{
			cJSON_AddStringToObject(param_root,"station_sn",station_sn);
			cJSON_AddStringToObject(param_root,"device_sn",device_sn);
			cJSON_AddStringToObject(param_root,"account",account_id);

			cJSON_AddItemToObject(param_heard, "", param_list);
			cJSON_AddItemToObject(param_root,"params", param_heard);
			
			cJSON_AddNumberToObject(param_list, "param_type", param_type);
			cJSON_AddNumberToObject(param_list, "param_value", param_value);
       		s_json_param = cJSON_PrintUnformatted(param_root);
        	if(s_json_param)
			{
				//dzlog_info("%s", s_json_param);

				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
				}
				else
				  	dzlog_error("err.code=%d  msg=%s",err.code , err.message);
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
		zx_Client_Cleanup(&client);
	}
	
	return err;
}


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
zx_Error zx_upload_hub_params(const char* account_id, const char* station_sn, int param_type, const char* param_value)
{
	zx_Error err;
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/upload_hub_params");
	err.code = -1;
	if (url && account_id && station_sn && param_value)
	{
		zx_Client client;
		char *s_json_param = NULL;
		cJSON *res_root = NULL, *param_root = NULL, *param_list = NULL, *param_heard = NULL;

		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
    	param_root = cJSON_CreateObject();
		param_heard = cJSON_CreateArray();
		param_list = cJSON_CreateObject();

		if (param_root && param_list && strlen(account_id)>0)
		{
			cJSON_AddStringToObject(param_root,"station_sn",station_sn);
			cJSON_AddStringToObject(param_root,"account",account_id);

            cJSON_AddNumberToObject(param_list, "param_type", param_type);
            cJSON_AddStringToObject(param_list, "param_value", param_value);
            cJSON_AddItemToObject(param_heard, "", param_list); // 把参数组JSON加入数组     
            
            cJSON_AddItemToObject(param_root,"params", param_heard); // 把数组加入

            s_json_param = cJSON_PrintUnformatted(param_root);
            if(s_json_param)
			{
				//dzlog_info("%s", s_json_param);
				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				//dzlog_warn("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    			//dzlog_warn("response body:\n%s", zx_Buffer_CStr(&client.b));
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
					dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
                    if (err.code == AS_HTTP_HUB_NON_ADMIN)
                    {
                        zx_unbind_hub_by_server();
                    }
				}
				else
				{
				  	dzlog_error("err.code=%d  msg=%s, param_type=%d",err.code , err.message, param_type);
				}
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
		zx_Client_Cleanup(&client);
	}
	
	return err;
}


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
zx_Error zx_upload_hub_default_params(void)
{
    zx_Error err;
    err.code = -1;
    char* url = zx_String_Concat2(AS_HOST, "/v1/hub/upload_hub_params");
    if (url)
    {
        zx_Client client;
        char *s_json_param = NULL;
        cJSON *res_root = NULL, *param_root = NULL, *param_list = NULL, *param_heard = NULL;

        zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
        param_root = cJSON_CreateObject();
        param_heard = cJSON_CreateArray();        

        if (param_root && param_heard)
        {
            char str_value[4] = {0};

            cJSON_AddStringToObject(param_root,"station_sn",base_param->hub_info.hub_sn);
            cJSON_AddStringToObject(param_root,"account",base_param->hub_info.account_id);

            param_list = cJSON_CreateObject();
            {
                cJSON_AddNumberToObject(param_list, "param_type", APP_CMD_SET_ALARM_MODE);  //布防模式
                memset(str_value, 0, sizeof(str_value));
                snprintf(str_value, sizeof(str_value), "%d", base_param->hub_info.alarm_mode);
                cJSON_AddStringToObject(param_list, "param_value", str_value);
                cJSON_AddItemToObject(param_heard, "", param_list); // 把参数组JSON加入数组
            } 

            param_list = cJSON_CreateObject();
            {
                cJSON_AddNumberToObject(param_list, "param_type", APP_CMD_SET_AI_SWITCH);  //AI开关
                memset(str_value, 0, sizeof(str_value));
                snprintf(str_value, sizeof(str_value), "%d", base_param->hub_info.ai_status);
                cJSON_AddStringToObject(param_list, "param_value", str_value);
                cJSON_AddItemToObject(param_heard, "", param_list); // 把参数组JSON加入数组
            }

            param_list = cJSON_CreateObject();
            {
                cJSON_AddNumberToObject(param_list, "param_type", APP_CMD_SET_DEV_SPEAKER_VOLUME); //音量
                memset(str_value, 0, sizeof(str_value));
                snprintf(str_value, sizeof(str_value), "%d", base_param->hub_info.spk_volume);
                cJSON_AddStringToObject(param_list, "param_value", str_value);
                cJSON_AddItemToObject(param_heard, "", param_list); // 把参数组JSON加入数组
            }            

            cJSON_AddItemToObject(param_root,"params", param_heard); // 把数组加入
            s_json_param = cJSON_PrintUnformatted(param_root);
            if(s_json_param)
            {
                //dzlog_info("%s", s_json_param);
                err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
                if (err.code == AS_HTTP_200) 
                {
                    //s_result = cJSON_PrintUnformatted(res_root);
                    err.code = zx_Json_GetInt(res_root, "code", 0);
                    dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
                }
                else
                {
                    dzlog_error("err.code=%d  msg=%s",err.code , err.message);
                }
                free(s_json_param);
            }
            cJSON_Delete(param_root);
        }
        free(url);
        zx_Client_Cleanup(&client);
    }
    return err;
}

/*============================================================================
方法：post hub/bind_hub app绑定基站
入参：{
  "account": "string",
  "name": "string",
  "sn": "string",
  "time_zone": "string"
}
============================================================================*/
zx_Error zx_app_bind_hub(const char* station_sn, const char* account_id, const char *hub_name, const char* time_zone)
{
	zx_Error err;	
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/bind_hub");
	err.code = -1;
	if (url)
	{			
		char *s_json_param = NULL;
		cJSON *res_root = NULL, *param_root = NULL;
    	param_root = cJSON_CreateObject();
		zx_Client client;
		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
		//dzlog_warn("start %s %s %s %s******", account_id, station_sn, hub_name, time_zone);
		if (param_root)
		{
			cJSON_AddStringToObject(param_root,"account",account_id);
			cJSON_AddStringToObject(param_root,"sn",station_sn);
			//cJSON_AddStringToObject(param_root,"name",hub_name);
			//cJSON_AddStringToObject(param_root,"time_zone",time_zone);

			s_json_param = cJSON_PrintUnformatted(param_root);
			if(s_json_param)
			{
                dzlog_info("%s", s_json_param);

				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				//dzlog_warn("call start %s %s******", url, s_json_param);
				//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				if (err.code == AS_HTTP_200) 
				{
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
				}
				else
				  	dzlog_error("err.code=%d  msg=%s",err.code , err.message);
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
		zx_Client_Cleanup(&client);
	}
	
	return err;
}

/*============================================================================
方法：post /app/unbind_hub app解除绑定基站
入参：{
  "account": "string",
  "sn": "string",
}
============================================================================*/
zx_Error zx_app_unbind_hub(const char* station_sn, const char* account_id)
{
	zx_Error err;	
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/unbind_hub");
	err.code = -1;
	if (url)
	{
		char *s_json_param = NULL;
		cJSON *res_root = NULL, *param_root = NULL;	
		zx_Client client;
		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);

    	param_root = cJSON_CreateObject();
		//dzlog_info("start %s %s ******", account_id, station_sn);
		if (param_root)
		{
			cJSON_AddStringToObject(param_root,"account",account_id);
			cJSON_AddStringToObject(param_root,"station_sn",station_sn);

       		s_json_param = cJSON_PrintUnformatted(param_root);
        	if(s_json_param)
			{
				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				//dzlog_info("call start %s %s******", url, s_json_param);
				//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				if (err.code == AS_HTTP_200) 
				{
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
				}
				else
				  	dzlog_error("err.code=%d  msg=%s",err.code , err.message);
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
		zx_Client_Cleanup(&client);
	}
	
	return err;
}

/*============================================================================
方法：post /event/hub/zx_upload_hub_log 单个监控事件上报
    入参：
    {
      "account": "string",          //账号id 
      "description": "string",      //log日志内容描述
      "device_sn": "string",        //设备唯一标识
      "device_type": "string",      //设备类型
      "error_code": "string",
      "language": "string",
      "level": " warn",             //日志级别
      "logging_time": 0,            //日志记录时间(int)
      "model": "string",            //设备型号:Battery Camera
      "module": "string",           //日志对应模块
      "os": "string",               //操作系统，如:ios
      "pairs_key": "string",        //APP 要求上报log时给出的标记 
      "product_name": "string",     //产品名
      "serialization_data": "string",
      "station_sn": "string",
      "url": "string",              //Log文件上传地址
      "version": "string"           //系统版本
    }
    返回：
    {
        "code": 0,
        "msg": "Succeed",
        "data": 0
    }
============================================================================*/
zx_Error zx_upload_hub_log(const char* account_id, const char* station_sn,
                           const char* version, const char* device_sn,
                           const char* level, const char* module,
                           const char* log_url, const char* pairs_key,
                           const char* err_str, const char* devtype)
{
    zx_Error err;    
    err.code = -1;
    if (account_id == NULL || station_sn == NULL  || log_url == NULL )
    {
        return err;
    }
    char* url = zx_String_Concat2(AS_HOST, "/v1/hub/logging/upload_log");
    if (url)
    {
		char *s_json_param = NULL;
   		cJSON *res_root = NULL, *param_root = NULL, *param_list = NULL;
    	zx_Client client;
    	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
        param_root = cJSON_CreateObject();
        param_list = cJSON_CreateArray();
        if (param_root && param_list)
        {
            cJSON_AddStringToObject(param_root,"account",account_id);
            cJSON_AddStringToObject(param_root,"station_sn",station_sn);
            if (device_sn)
            {
                cJSON_AddStringToObject(param_root,"device_sn",device_sn);
            }
            else
            {
                cJSON_AddStringToObject(param_root,"device_sn","");
            }

            long long curtime = zx_LocalTime_ms();
            cJSON_AddNumberToObject(param_root, "logging_time", curtime);
            cJSON_AddStringToObject(param_root, "error_code", err_str);
            
            cJSON_AddStringToObject(param_root,"version", version);
            cJSON_AddStringToObject(param_root,"level",level);
            cJSON_AddStringToObject(param_root,"model","EverCam 1080P");
            cJSON_AddStringToObject(param_root,"module",module);
            cJSON_AddStringToObject(param_root,"product_name","Eufy Security");
            cJSON_AddStringToObject(param_root,"os","Linux");
            cJSON_AddStringToObject(param_root,"url",log_url);
            cJSON_AddStringToObject(param_root,"pairs_key",pairs_key);
            cJSON_AddStringToObject(param_root,"device_type",devtype);
            cJSON_AddStringToObject(param_root,"description","bug");
            
            s_json_param = cJSON_PrintUnformatted(param_root);
            if(s_json_param)
            {
                //dzlog_info("%s", s_json_param);
                
                err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
                //dzlog_info("err.code=%d  msg=%s",err.code , err.message);
                if (err.code == AS_HTTP_200) 
                {
                    err.code = zx_Json_GetInt(res_root, "code", 0);
                    dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
                }
                else
                {
                    dzlog_error("err.code=%d  msg=%s",err.code , err.message);
                }
                free(s_json_param);
            }
            else
            {
                dzlog_error("s_json_param is NULL");
            }
            cJSON_Delete(param_root);
        }
        else
        {
            dzlog_error("cJSON_CreateObject Fail");
        }
        free(url);
		zx_Client_Cleanup(&client);
    }   

    return err;
}

/*============================================================================
方法：post /event/hub/upload_history_record 单个监控事件上报
	入参：
		{
		  "account": "string",
		  "device_sn": "string",
		  "station_sn": "string",
		  "start_time": 0,
		  "end_time": 0,
		  "cloud_path": "string",
		  "storage_path": "string",
		  "frame_num": 0,
		  "storage_type": 0,
		  "thumb_data": "string",
		  "thumb_path": "string"
		}
	返回：
		{
			"code": 0,
			"msg": "成功"
		}
============================================================================*/
zx_Error zx_upload_hub_history_record(        const char* device_sn,
                                              const char* local_path,
                                              zx_Uint64 start_time,
                                              zx_Uint64 end_time,
                                              zx_Uint32 framenum,
                                              const char* cloud_path,
                                              int storagetype,
                                              const char* thumb_data,
                                              const unsigned int* pface_id,
                                              unsigned char has_human)
{
    zx_Error err;
    err.code = -1;

    if (device_sn == NULL || framenum == 0)
    {
        return err;
    }
    
    char* url = zx_String_Concat2(AS_HOST, "/v1/event/hub/upload_history_record");    
    if (url)
    {
        char *s_json_param = NULL;
        cJSON *res_root = NULL, *param_root = NULL, *param_list = NULL;
        zx_Client client;
        zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
        param_root = cJSON_CreateObject();
        param_list = cJSON_CreateArray();
        if (param_root && param_list)
        {
            int i = 0;
            int face_num = 0;
            cJSON_AddStringToObject(param_root,"account", base_param->hub_info.account_id);
            cJSON_AddStringToObject(param_root,"station_sn", base_param->hub_info.hub_sn);
            cJSON_AddStringToObject(param_root,"device_sn", device_sn);
            cJSON_AddNumberToObject(param_root,"start_time", start_time);
            cJSON_AddNumberToObject(param_root,"end_time", end_time);
            cJSON_AddNumberToObject(param_root,"frame_num", framenum);
            cJSON_AddNumberToObject(param_root,"storage_type", storagetype);  // 1: 本地 3:本地+云
            cJSON_AddNumberToObject(param_root,"cipher_id", base_param->hub_info.rsa_pub_id);
            cJSON_AddStringToObject(param_root,"volume", base_param->hub_info.tf_card_label);
            if (pface_id != NULL)
            {
                if (*pface_id > 0)
                {
                    for (i=0; i<FACE_NUM_MAX; i++)
                    {
                        if (*pface_id)
                        {
                            cJSON_AddNumberToArry(param_list, *pface_id);
                            dzlog_info("history_record, face_num[%d] = %d", face_num, *pface_id);
                            pface_id++;
                            face_num += 1;
                        }
                        else
                        {
                            break;
                        }
                    }
                    cJSON_AddItemToObject(param_root,"ai_face_ids", param_list);
                }
            }

            if (has_human)
            {
                cJSON_AddNumberToObject(param_root,"has_human", 1); // 1-有人形
            }
            else
            {
                cJSON_AddNumberToObject(param_root,"has_human", 0);
            }

            if (cloud_path == NULL)
            {
                cJSON_AddStringToObject(param_root,"cloud_path","");
            }
            else
            {
                cJSON_AddStringToObject(param_root,"cloud_path",cloud_path);
            }
            if (local_path == NULL)
            {
                cJSON_AddStringToObject(param_root,"storage_path","");
            }
            else
            {
                cJSON_AddStringToObject(param_root,"storage_path",local_path);
            }
            if (thumb_data == NULL)
            {
                cJSON_AddStringToObject(param_root,"thumb_data","");
            }
            else
            {
                cJSON_AddStringToObject(param_root,"thumb_data",thumb_data);
            }

            s_json_param = cJSON_PrintUnformatted(param_root);
            if(s_json_param)
            {
                //dzlog_info("%s", s_json_param);
                
                err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
                //dzlog_info("err.code=%d  msg=%s", err.code, err.message);
                if (err.code == AS_HTTP_200) 
                {
                    err.code = zx_Json_GetInt(res_root, "code", 0);
                    dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
                    if (err.code == AS_HTTP_DEV_NON_ADMIN)
                    {
                        zx_unbind_dev_by_sn(device_sn);
                    }
                }
                else
                {
                    dzlog_error("err.code=%d  msg=%s",err.code , err.message);
                }
                free(s_json_param);
            }
            else
            {
                dzlog_error("s_json_param is NULL");
            }
            cJSON_Delete(param_root);
        }
        else
        {
            dzlog_error("cJSON_CreateObject Fail");
        }
        free(url);
		zx_Client_Cleanup(&client);
    }
    else
    {
        dzlog_error("Parm error, framenum: %d", framenum);
    }   

    return err;
}

/*============================================================================
方法：post /event/hub/delete_record_by_path 基站空间满时删除老的监控视频记录
入参：
	{
		"account": "string",
		"file_path": "string",
		"station_sn": "string"
	}
返回：
	{
		"code": 0,
		"msg": "成功"
	}
============================================================================*/
zx_Error zx_delete_hub_history_record_by_file(const char* account_id,
                                              const char* station_sn,
                                              const char* local_path)
{
    zx_Error err;    
    err.code = -1;
    if (account_id == NULL || station_sn == NULL  || local_path == NULL )
    {
        return err;
    }
    
    char* url = zx_String_Concat2(AS_HOST, "/v1/event/hub/delete_record_by_path");
    if (url )
    {
		char *s_json_param = NULL;
    	cJSON *res_root = NULL, *param_root = NULL;// *param_list, *param_heard;
    	zx_Client client;
    	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
      	param_root = cJSON_CreateObject();

      	if (param_root)
      	{
        	cJSON_AddStringToObject(param_root,"account",account_id);
        	cJSON_AddStringToObject(param_root,"station_sn",station_sn);
        	cJSON_AddStringToObject(param_root,"file_path",local_path);

        	s_json_param = cJSON_PrintUnformatted(param_root);
        	if(s_json_param)
        	{
          		//dzlog_info("%s", s_json_param);

          		err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "DELETE");
          		//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
          		if (err.code == AS_HTTP_200)
          		{
            		err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
          		}
          		else
			  		dzlog_error("err.code=%d  msg=%s",err.code , err.message);
          		free(s_json_param);
        	}
        	cJSON_Delete(param_root);
      	}
      	free(url);
		zx_Client_Cleanup(&client);
	}
    
    return err;
}

/*============================================================================
方法：post /event/hub/delete_record 删除APP指定的单个监控视频记录
入参：
	{
	  "account": "string",
	  "station_sn": "string"
	  "monitor_id": 0,       // 数据库地索引id
	}
返回：
	{
	  "code": 0,
	  "msg": "成功"
	}
============================================================================*/
zx_Error zx_delete_hub_history_record(const char* account_id,
									  const char* station_sn, int id)
{
	zx_Error err;
	char* url = zx_String_Concat2(AS_HOST, "/v1/event/hub/delete_record");
	err.code = -1;
	if (url && account_id && station_sn)
	{
		char *s_json_param = NULL;
		cJSON *res_root = NULL, *param_root = NULL;// *param_list, *param_heard;
		zx_Client client;
		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
		param_root = cJSON_CreateObject();

		if (param_root)
		{
			cJSON_AddStringToObject(param_root,"account",account_id);
			cJSON_AddStringToObject(param_root,"station_sn",station_sn);
			cJSON_AddNumberToObject(param_root,"monitor_id",id);

			s_json_param = cJSON_PrintUnformatted(param_root);
			if(s_json_param)
			{
				//dzlog_info("%s", s_json_param);

				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "DELETE");
				//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				if (err.code == AS_HTTP_200)
				{
					err.code = zx_Json_GetInt(res_root, "code", 0);
					dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
				}
	            else
				  dzlog_error("err.code=%d  msg=%s",err.code , err.message);
				free(s_json_param);
			}
			cJSON_Delete(param_root);
		}
		free(url);
		zx_Client_Cleanup(&client);
	}
	return err;
}

/*============================================================================
方法：post /event/hub/delete_records 删除APP指定的多个监控视频记录
入参：
  {
    "account": "string",
    "station_sn": "string"
    "monitor_ids": [
      0
    ],    
  }
返回：
  {
    "code": 0,
    "msg": "成功"
  }
============================================================================*/
zx_Error zx_delete_hub_history_records(const char* account_id,
                                               const char* station_sn,
                                               int num, const RECORD_INFO* pRecords)
{
  	zx_Error err;
  	char* url = zx_String_Concat2(AS_HOST, "/v1/event/hub/delete_records");
  	err.code = -1;
  	if (url && account_id && station_sn)
  	{
		char *s_json_param = NULL;
  		cJSON *res_root = NULL, *param_root = NULL, *param_list = NULL;
  		zx_Client client;
  		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
      	param_root = cJSON_CreateObject();      
      	param_list = cJSON_CreateArray();

      	if (param_root && param_list && pRecords)
      	{
          	int i = 0;
          	cJSON_AddStringToObject(param_root,"station_sn", station_sn);
          	cJSON_AddStringToObject(param_root,"account", account_id);      
          
          	cJSON_AddItemToObject(param_root,"monitor_ids", param_list);
          	for (i=0; i<num; i++)
          	{
              	cJSON_AddNumberToArry(param_list, pRecords->record_id);
              	pRecords++;
          	}
          
          	s_json_param = cJSON_PrintUnformatted(param_root);
          	if(s_json_param)
          	{
              	//dzlog_info("%s", s_json_param);

              	err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "DELETE");
              	//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
              	if (err.code == AS_HTTP_200)
              	{
                  	err.code = zx_Json_GetInt(res_root, "code", 0);
                  	dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
              	}
              	else
				  	dzlog_error("err.code=%d  msg=%s",err.code , err.message);
              	free(s_json_param);
          	}
          	cJSON_Delete(param_root);
      	}
      	free(url);
		zx_Client_Cleanup(&client);
  	} 

  	return err;
}

/*============================================================================
方法：post event/hub/favorite 收藏/取消 历史记录
入参：
  {
	"account": "string",
	"station_sn": "string"
	"monitor_id": 0,	   // 数据库地索引id
  }
返回：
  {
	"code": 0,
	"msg": "成功"
  }
============================================================================*/
zx_Error zx_update_favorite_history_record(const char* account_id,
													const char* station_sn,
													int id, int mode)
{
	zx_Error err;
	char* url;
	if (mode == 1)
	{
		url = zx_String_Concat2(AS_HOST, "/v1/event/hub/favorite");
	}
	else
	{
		url = zx_String_Concat2(AS_HOST, "/v1/event/hub/unfavorite");
	}
	err.code = -1;
	if (url && account_id && station_sn)
	{
		char *s_json_param = NULL;
		cJSON *res_root = NULL, *param_root = NULL;// *param_list, *param_heard;
		zx_Client client;		

		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	  	param_root = cJSON_CreateObject();

	  	if (param_root)
	  	{
		  	cJSON_AddStringToObject(param_root,"account",account_id);
		  	cJSON_AddStringToObject(param_root,"station_sn",station_sn);
		  	cJSON_AddNumberToObject(param_root,"monitor_id",id);

		  	s_json_param = cJSON_PrintUnformatted(param_root);
		  	if(s_json_param)
		  	{
			  	//dzlog_info("%s", s_json_param);

			  	err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
			  	//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
			  	if (err.code == AS_HTTP_200)
			  	{
				  	err.code = zx_Json_GetInt(res_root, "code", 0);
				  	dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
			  	}
			  	else
				  	dzlog_error("err.code=%d  msg=%s",err.code , err.message);
			  	free(s_json_param);
		  	}
		  	cJSON_Delete(param_root);
	  	}
	  	free(url);
		zx_Client_Cleanup(&client);
	}
	return err;
}

/*============================================================================
方法：post /v1/cloud/hub/get_uptoken 查询云存储是否开通
入参：
	{
	  "account": "string",
	  "file_path": "string",
	  "station_sn": "string"
	}
返回：
	{
	  "code": 0,
	  "msg": "成功"
	}
============================================================================*/
zx_Error zx_get_cloud_storage_flag(const char* account_id, const char* device_sn)
{
	zx_Error err;
	char *s_json_param;
	cJSON *res_root, *param_root;// *param_list, *param_heard;
	zx_Client client;

	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	char* url = zx_String_Concat2(AS_HOST, "/v1/cloud/hub/get_uptoken");
	err.code = -1;
	if (url && account_id && device_sn)
	{
		param_root = cJSON_CreateObject();

		if (param_root)
		{
			cJSON_AddStringToObject(param_root,	"account", account_id);
			cJSON_AddStringToObject(param_root,	"device_sn", device_sn);
			cJSON_AddStringToObject(param_root,	"file_name", "check_cloud_flag");
			cJSON_AddNumberToObject(param_root,	"file_size", 10);
			cJSON_AddNumberToObject(param_root,	"type", 4);
			cJSON_AddFalseToObject(param_root,	"disable_ssl");

			s_json_param = cJSON_PrintUnformatted(param_root);
			if(s_json_param)
			{
				//dzlog_info("%s", s_json_param);

				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				if (err.code == AS_HTTP_200)
				{
					err.code = zx_Json_GetInt(res_root, "code", 0);
					dzlog_info("code=%d msg=%s, dev_sn:%s",err.code, zx_Json_GetString(res_root, "msg", 0), device_sn);
                    if (err.code == AS_HTTP_DEV_NON_ADMIN)
                    {
                        zx_unbind_dev_by_sn(device_sn);
                    }
                }
                else
                {
				  dzlog_error("err.code=%d  msg=%s",err.code , err.message);
                }
				free(s_json_param);
			}
			cJSON_Delete(param_root);
		}
		free(url);
	}
	zx_Client_Cleanup(&client);

	return err;
}

/*============================================================================
方法：post /hub/aiassis/add_stranger   上传陌生人脸特征点和图片
入参：
  {
    "account": "string",
    "station_sn": "string",
    "features": [
      {
        "feature": "string",
        "from_path": 0,
        "path": "string",
        "position": 0
      }
    ],
    "files": [
      "string"
    ],    
    "sync": 0
  }

返回：
  {
    "code": 0,
    "data": [
      {
        "Identifier": "string",
        "ai_face_id": 0,
        "create_time": 0,
        "face_path": "string",
        "face_url": "string",
        "feature": "string",
        "is_stranger": 0,
        "status": 1
      }
    ],
    "msg": "成功"
  }
============================================================================*/

#if 0
zx_Error zx_hub_add_stranger_features(const char* account_id,
                                              const char* station_sn,
                                              const AI_FACECHECK_RESULT* pfacejpg,
                                              unsigned int * face_id)
{
	zx_Error err;
  	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/aiassis/add_stranger");
  	err.code = -1;
  	if (url && account_id && station_sn && pfacejpg && face_id)
  	{
		char *s_json_param = NULL;
  		cJSON *res_root = NULL, *param_root = NULL, *param_list = NULL, *param_feature = NULL, *param_image = NULL;
  		zx_Client client;
  		int i = 0;

  		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
    	param_root = cJSON_CreateObject();
      	param_feature = cJSON_CreateArray(); // 特征点数组
      	param_image = cJSON_CreateArray();   // 头像数组
      
      	if (param_root && param_feature && param_image)
      	{ 
          	char* pfacebase64 = NULL;
          	char* pimagebase64 = NULL;
          	char* pfeauture = NULL;
          	int filter_index = -1;

          	RS_FACE_RECOGNITION_RESULT *face = (RS_FACE_RECOGNITION_RESULT *)pfacejpg->data;

          	cJSON_AddStringToObject(param_root,"station_sn", station_sn);
          	cJSON_AddStringToObject(param_root,"account", account_id);          

          	for (i=0; i<pfacejpg->face_cnt; i++)
          	{
              	pfeauture = (char *)face->fr_feature;
              	if (face->fr_available_flag == 2) // 过滤熟人
              	{
                	filter_index = i;
                	dzlog_info("filter familiar image[%d]", i);
                	continue;
              	}
              	else
              	{
                	memcpy(pfeauture, HEAD_MAGIC, MAGIC_SIZE);
                	pfeauture[MAGIC_SIZE] = (i | 0x30);
                	rand_str(&pfeauture[MAGIC_SIZE+1], 10);
                	pfeauture[20] = '\0';  // 当前特征点无效，只传20个Byte
              	}

              	param_list = cJSON_CreateObject(); // json
              	pfacebase64 = zx_Memory_Encode( pfeauture, 20); // 只传20个Byte, 4*FACE_FEATURE_SAMPLE
              	if (pfacebase64)
              	{
                  	cJSON_AddStringToObject(param_list, "feature", pfacebase64);
                  	free(pfacebase64);
              	}
              	else
              	{
                  	pfeauture[20] = '\0';
                  	cJSON_AddStringToObject(param_list, "feature", pfeauture);
              	}
              	cJSON_AddStringToObject(param_list, "path", "");
              	cJSON_AddNumberToObject(param_list, "from_path", 0);
              	cJSON_AddNumberToObject(param_list, "position", 0);
              	cJSON_AddItemToArray(param_feature, param_list);  // 加入一个json到数组param_heard              
              	face +=1;
          	}
          	cJSON_AddItemToObject(param_root,"features", param_feature);

          	int*  pimg_len = (int *)face;
          	char* pimg_data = NULL;
          	int jpg_size = 0;
          	int img_max = 0;
          	int img_num = 0;
          	int big_index = 0;
          	for (i=0; i<pfacejpg->face_cnt; i++)
          	{
              	jpg_size = *pimg_len;
              	pimg_data = (char *)(pimg_len+1);
              	if (jpg_size >= JPG_BUF_SIZE)
              	{
                	dzlog_info("image[%d] jpg_size:%d", i, jpg_size);
                	break;
              	}
              	if (filter_index != i) // 不是熟人
              	{
                  	pimagebase64 = zx_Memory_Encode(pimg_data, jpg_size);
                  	if (pimagebase64)
                  	{
                      	cJSON_AddItemToArray(param_image, cJSON_CreateString(pimagebase64));
                      	free(pimagebase64);
                  	}
                  	else
                  	{
                    	cJSON_AddItemToArray(param_image, cJSON_CreateString("img base64 error"));
                    	dzlog_warn("upimagebase64 encode error!");
                  	}                  
                  	if (jpg_size > img_max)
                  	{
                    	img_max = jpg_size;
                    	big_index = img_num;
                  	}
                  	img_num += 1;
              	}
              	pimg_len = (int *)(pimg_data+jpg_size); // 长度的指针
          	}
          	dzlog_info("upload image num=%d", img_num);
          	cJSON_AddItemToObject(param_root,"files", param_image);
          	cJSON_AddNumberToObject(param_root,"sync", 0);
          
          	s_json_param = cJSON_PrintUnformatted(param_root);
          	if(s_json_param)
          	{
              	//dzlog_info("%s", s_json_param);

              	err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");              
              	if (err.code == AS_HTTP_200)
              	{
                  	err.code = zx_Json_GetInt(res_root, "code", 0);
                  	dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
                  	if (err.code == 0)
                  	{
                      	cJSON *js_list = cJSON_GetObjectItem(res_root, "data");
                      	if (js_list)
                      	{
                            //char *s_result = cJSON_PrintUnformatted(js_list);
                            //dzlog_info("%s", s_result);
                            //free(s_result);

                            int list_size = cJSON_GetArraySize(js_list);
                            dzlog_info("list_size:%d, %d", list_size, img_num);
                            if (list_size > img_num)
                            {
                                list_size = img_num;
                            }
                            cJSON *js_face = NULL;
                            int j = 1;
                            for (i=0; i<list_size; i++)
                            {
                                js_face = cJSON_GetArrayItem(js_list, i); //拿中括号里面的第i个json
                                if (js_face)
                                {
                                    //char *s_result = cJSON_PrintUnformatted(js_face);
                                    //dzlog_info("%s", s_result);
                                    //free(s_result);

                                    if (i == big_index) // 最大头像放到最前面
                                    {
                                        face_id[0] = zx_Json_GetInt(js_face, "ai_face_id", 0);
                                    }
                                    else
                                    {
                                        face_id[j] = zx_Json_GetInt(js_face, "ai_face_id", 0);
                                        j += 1;
                                    }
                                }
                                else
                                {
                                    dzlog_error("can't get facelist[%d]", i);
                                }
                            }
                        }
                    }
                    else if (err.code == AS_HTTP_HUB_NON_ADMIN)
                    {
                        zx_unbind_hub_by_server();
                    }
                }
                else
                {
                    dzlog_error("err.code=%d  msg=%s",err.code , err.message);
                }              
                free(s_json_param);
            }
            cJSON_Delete(param_root);
        }
        free(url);
        zx_Client_Cleanup(&client);
    }
    return err;
}
#endif

/*============================================================================
方法：post /event/hub/add_face 上传人脸特征点和图片
入参：
  {
    "account": "string",
    "station_sn": "string",    
    "features": [
      {
        "feature": "string", // 人脸特征
        "path": "string",
        "from_path": 0,        
        "position": 0
      }
    ],
    "files": [
      "string"               // jpeg图片
    ],
    "ai_group_id": 0,
    "user_id": "string"
    "nick_name": "string",
    "notification": 0,    
    "sync": 0,    
  }

返回：
  {
    "code": 0,
    "msg": "成功"
  }
============================================================================*/

#if 0
zx_Error zx_hub_add_face_features(const char* account_id,
                                         const char* station_sn,
                                         const char* pfacefeature,  /* base64 */
                                         const char* pimage,        /* jep->base64 */
                                         AI_FACE_FEATURE * pfaceinfo)
{
  	zx_Error err;
  	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/aiassis/add_face");
  	err.code = -1;
  	if (url && account_id && station_sn && pfacefeature && pfaceinfo)
 	{
		char *s_json_param = NULL;
  		cJSON *res_root = NULL, *param_root = NULL, *param_list = NULL, *param_heard = NULL, *param_image = NULL;
  		zx_Client client;
  		//zx_Global_Init(-1);
  		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
      	param_root = cJSON_CreateObject();
      	param_heard = cJSON_CreateArray();
      	param_list = cJSON_CreateObject();
      	param_image = cJSON_CreateArray();
      
      	if (param_root && param_heard && param_list)
      	{
		  	//int i;
          	cJSON_AddStringToObject(param_root,"station_sn", station_sn);
          	cJSON_AddStringToObject(param_root,"account", account_id);
          	cJSON_AddItemToObject(param_root,"features", param_heard);
          
          	cJSON_AddItemToObject(param_heard, "", param_list);          
          	cJSON_AddStringToObject(param_list, "feature", pfacefeature);         
          	cJSON_AddStringToObject(param_list, "path", "");
          	cJSON_AddNumberToObject(param_list, "from_path", 0);
          	cJSON_AddNumberToObject(param_list, "position", 0);

          	cJSON_AddItemToObject(param_root,"files", param_image);
          	if (pimage)
          	{
            	cJSON_AddItemToArray(param_image, cJSON_CreateString(pimage));
          	}
          	else
          	{
            	cJSON_AddItemToArray(param_image, cJSON_CreateString(""));
          	}

          	cJSON_AddNumberToObject(param_root,"ai_group_id", pfaceinfo->group_id);
          	cJSON_AddStringToObject(param_root,"user_id", ""); // 新增用户
          	cJSON_AddStringToObject(param_root,"nick_name", pfaceinfo->nick_name);
          	cJSON_AddNumberToObject(param_root,"notification", pfaceinfo->notify);
          	cJSON_AddNumberToObject(param_root,"sync", 0);
          
          	s_json_param = cJSON_PrintUnformatted(param_root);
          	if(s_json_param)
          	{
              //dzlog_info("%s", s_json_param);

              	err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json;charset=utf-8", "POST");
              	//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
              	if (err.code == AS_HTTP_200)
              	{
                  	err.code = zx_Json_GetInt(res_root, "code", 0);
                  	dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
                  	if (err.code == 0)
                  	{
                      	cJSON *js_list = cJSON_GetObjectItem(res_root, "data");
                      	if (js_list)
                      	{
                         	 //char *s_result = cJSON_PrintUnformatted(js_list);
                          	//dzlog_info("%s", s_result);
                          	//free(s_result);
                          	cJSON *js_faces = cJSON_GetObjectItem(js_list, "faces");
                          	if (js_faces)
                          	{
                              	js_faces =cJSON_GetArrayItem(js_faces, 0); //拿中括号里面的第一个json
                              	if (js_faces)
                              	{
                                  	pfaceinfo->face_id = zx_Json_GetInt(js_faces, "ai_face_id", 0);
                                  	pfaceinfo->user_id = zx_Json_GetInt(js_faces, "ai_user_id", 0);
                              	}
                          	}
                      	}
                  	}
              	}
			  	else
				  	dzlog_error("err.code=%d  msg=%s",err.code , err.message);
              	free(s_json_param);
          	}
          	cJSON_Delete(param_root);
      	}
      	free(url);
		zx_Client_Cleanup(&client);
  	}
  	return err;
}

/*============================================================================
方法：post /event/hub/update_face 更新人脸特征点和图片
入参：
 {
   "account": "string",
   "ai_group_id": 0,
   "ai_user_id": 0,
   "features": [
     {
       "feature": "string",
       "from_path": 0,
       "path": "string",
       "position": 0
     }
   ],
   "files": [
     "string"
   ],
   "nick_name": "string",
   "notification": 0,
   "station_sn": "string",
   "sync": 0
 }

返回：
{
  "code": 0,
  "data": {
    "ai_group_id": 0,
    "ai_user_id": 0,
    "faces": [
      {
        "Identifier": "string",
        "ai_face_id": 0,
        "ai_user_id": 0,
        "create_time": 0,
        "face_path": "string",
        "face_url": "string",
        "feature": "string",
        "is_stranger": 0,
        "owner_id": "string",
        "position": 0,
        "status": 1
      }
    ],
    "nick_name": "string",
    "notification": 0,
    "user_id": "string"
  },
  "msg": "成功"
}

============================================================================*/
zx_Error zx_hub_update_face_features(const char* account_id,
                                             const char* station_sn,
                                             const char* pfacefeature,  /* base64 */
                                             const char* pimage,        /* jep->base64 */                                             
                                             AI_FACE_FEATURE * pfaceinfo)
{
    zx_Error err;
    char* url = zx_String_Concat2(AS_HOST, "/v1/hub/aiassis/update_face");
    err.code = -1;
    if (url && account_id && station_sn && pfacefeature && pfaceinfo)
    {
		char *s_json_param = NULL;
    	cJSON *res_root = NULL, *param_root = NULL, *param_list = NULL, *param_heard = NULL, *param_image = NULL;
    	zx_Client client;
    	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
       	param_root = cJSON_CreateObject();
       	param_heard = cJSON_CreateArray();
       	param_list = cJSON_CreateObject();
       	param_image = cJSON_CreateArray();
       
       	if (param_root && param_heard && param_list)
       	{
           //int i;
           cJSON_AddStringToObject(param_root,"station_sn", station_sn);
           cJSON_AddStringToObject(param_root,"account", account_id);
           cJSON_AddItemToObject(param_root,"features", param_heard);
           
           cJSON_AddItemToObject(param_heard, "", param_list);
           
           cJSON_AddStringToObject(param_list, "feature", pfacefeature);      
           cJSON_AddStringToObject(param_list, "path", "");
           cJSON_AddNumberToObject(param_list, "from_path", 0);
           cJSON_AddNumberToObject(param_list, "position", 0);

           cJSON_AddItemToObject(param_root,"files", param_image);
           if (pimage)
           {
             cJSON_AddItemToArray(param_image, cJSON_CreateString(pimage));
           }
           else
           {
             cJSON_AddItemToArray(param_image, cJSON_CreateString(""));
           }

           cJSON_AddNumberToObject(param_root,"ai_group_id", pfaceinfo->group_id);
           cJSON_AddNumberToObject(param_root,"ai_user_id", pfaceinfo->user_id);
           cJSON_AddStringToObject(param_root,"nick_name", pfaceinfo->nick_name);
           cJSON_AddNumberToObject(param_root,"notification", pfaceinfo->notify);
           cJSON_AddNumberToObject(param_root,"sync", 0);
           
           s_json_param = cJSON_PrintUnformatted(param_root);
           if(s_json_param)
           {
               //dzlog_info("%s", s_json_param);

               err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json;charset=utf-8", "POST");
               //dzlog_info("err.code=%d  msg=%s",err.code , err.message);
               if (err.code == AS_HTTP_200)
               {
                   err.code = zx_Json_GetInt(res_root, "code", 0);
                   dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
                   if (err.code == 0)
                   {
                       cJSON *js_list = cJSON_GetObjectItem(res_root, "data");
                       if (js_list)
                       {
                           //char *s_result = cJSON_PrintUnformatted(js_list);
                           //dzlog_info("%s", s_result);
                           //free(s_result);
                           cJSON *js_faces = cJSON_GetObjectItem(js_list, "faces");
                           if (js_faces)
                           {
                               js_faces =cJSON_GetArrayItem(js_faces, 0); //拿中括号里面的第一个json
                               if (js_faces)
                               {
                                   pfaceinfo->face_id = zx_Json_GetInt(js_faces, "ai_face_id", 0);
                                   pfaceinfo->user_id = zx_Json_GetInt(js_faces, "ai_user_id", 0);
                               }
                           }
                       }
                   }
               }
               else
               {
                    dzlog_error("err.code=%d  msg=%s",err.code , err.message);
               }
               free(s_json_param);
           }
           cJSON_Delete(param_root);
       	}
       	free(url);
		zx_Client_Cleanup(&client);
    }
    return err;
}
#endif

/*============================================================================
 方法：post /event/hub/zx_hub_delete_faces_features 删除用户的人脸数据
 入参：
    {
        {
            "account": "string",
            "station_sn": "string",
            "ai_user_ids": [
            0
            ]
        }
    } 
返回：
   {
     "code": 0,
     "msg": "成功"
   }
============================================================================*/
zx_Error zx_hub_delete_faces_features(const char* account_id,
                                      const char* station_sn,
                                      int user_id)
{
    zx_Error err;
    char* url = zx_String_Concat2(AS_HOST, "/v1/hub/aiassis/delete_faces");
    err.code = -1;
    if (url && account_id && station_sn )
    {
		char *s_json_param = NULL;
    	cJSON *res_root = NULL, *param_root = NULL, *param_list = NULL;
    	zx_Client client;
    	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
      	param_root = cJSON_CreateObject();
      	param_list = cJSON_CreateArray();

      	if (param_root && param_list)
      	{          
          	cJSON_AddStringToObject(param_root,"station_sn", station_sn);
          	cJSON_AddStringToObject(param_root,"account", account_id);
          
          	cJSON_AddItemToObject(param_root,"ai_user_ids", param_list);          
          	cJSON_AddNumberToArry(param_list, user_id);   // 用户ID

          	s_json_param = cJSON_PrintUnformatted(param_root);
          	if(s_json_param)
          	{
              	//dzlog_info("%s", s_json_param);

              	err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "DELETE");
              	//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
              	if (err.code == AS_HTTP_200)
              	{
                  	err.code = zx_Json_GetInt(res_root, "code", 0);
                  	dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
              	}
			  	else
				  dzlog_error("err.code=%d  msg=%s",err.code , err.message);
              	free(s_json_param);
          	}
          	cJSON_Delete(param_root);
      	}
      	free(url);
		zx_Client_Cleanup(&client);

    }
    
    return err;    
}


/*============================================================================
入参：
  {
    "account": "string",
    "data_list": [
      {
        "channel_time": 0,
        "face_num": 0,
        "family_num": 0,
        "family_time": 0,
        "first_face_time": 0,
        "first_frame_time": 0,
        "first_human_time": 0,
        "human_num": 0,
        "mode": 0,
        "pir_time": 0,
        "stream_time": 0
      }
    ],
    "device_sn": "string",
    "device_type": "string",
    "language": "string",
    "model": "string",
    "os": "string",
    "product_name": "string",
    "station_sn": "string",
    "version": "string"
  }
返回：
  {
  }
============================================================================*/
#if 0
zx_Error zx_upload_ai_performance(char channel)
{
  zx_Error err;
  AI_FACECHECK_RESULT* pFace = zx_get_ai_face_point(channel);
  LOCAL_FP_INFO* plocal = get_local_filerecord_by_channel(channel);
  DEV_BASE_PARAM* devParam = zx_get_dev_by_channel(channel);
  err.code = -1;
  if (pFace && plocal && devParam)
  {
    int open_time   = 0;
    int stream_time = 0;
    int detect_time = 0;
    int body_time   = 0;
    int face_time   = 0;
    int family_time = 0;

    if (plocal->open_time > 0)
    {
        open_time   = plocal->open_time  - plocal->pir_time;
    }
    if (plocal->start_time > 0)
    {
        stream_time  = plocal->start_time - plocal->pir_time;
    }
    if (pFace->detect_time > 0)
    {
        detect_time = pFace->detect_time - plocal->pir_time;
    }
    if (pFace->body_time > 0)
    {
        body_time   = pFace->body_time   - plocal->pir_time;
    }
    if (pFace->face_time > 0)
    {
        face_time   = pFace->face_time   - plocal->pir_time;
    }
    if (pFace->family_time > 0)
    {
        family_time = pFace->family_time  - plocal->pir_time;
    }

    dzlog_info("camera:%d, ai_performance: time = %dms, %dms, %dms, %dms, %dms, %dms, bady_cnt:%d, face_cnt:%d",
               channel, open_time, stream_time, detect_time, body_time, face_time, family_time,
               pFace->bady_cnt, pFace->face_cnt);
#if 1
    char* url = zx_String_Concat2(AS_HOST, "/v1/hub/logging/upload_ai_event");
    if (url)
    {
        zx_Client client;
        char *s_json_param = NULL;
        cJSON *res_root = NULL, *param_root = NULL, *param_list = NULL, *param_heard = NULL;

        zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
        param_root = cJSON_CreateObject();
        param_heard = cJSON_CreateArray();

        if (param_root && param_heard)
        {
            cJSON_AddStringToObject(param_root,"account",base_param->hub_info.account_id);
            cJSON_AddStringToObject(param_root,"station_sn",base_param->hub_info.hub_sn);
            cJSON_AddStringToObject(param_root,"device_sn",devParam->dev_sn);

            param_list = cJSON_CreateObject();
            {
                cJSON_AddNumberToObject(param_list, "mode", plocal->ir_mode);
                cJSON_AddNumberToObject(param_list, "pir_time", plocal->pir_time);
                cJSON_AddNumberToObject(param_list, "channel_time", open_time);
                cJSON_AddNumberToObject(param_list, "stream_time", stream_time);
                cJSON_AddNumberToObject(param_list, "first_frame_time", detect_time);
                cJSON_AddNumberToObject(param_list, "first_human_time", body_time);
                cJSON_AddNumberToObject(param_list, "first_face_time", face_time);
                cJSON_AddNumberToObject(param_list, "family_time", family_time);
                cJSON_AddNumberToObject(param_list, "human_num", pFace->bady_cnt);
                cJSON_AddNumberToObject(param_list, "face_num", pFace->face_cnt);
                cJSON_AddNumberToObject(param_list, "family_num", pFace->family_cnt);

                cJSON_AddItemToObject(param_heard, "", param_list); // 把参数组JSON加入数组
            }
            cJSON_AddItemToObject(param_root,"data_list", param_heard); // 把数组加入

            cJSON_AddStringToObject(param_root,"version", base_param->hub_info.hub_hardware_main_ver);
            cJSON_AddStringToObject(param_root,"model","EverCam 1080P");
            cJSON_AddStringToObject(param_root,"product_name","Eufy Security");
            cJSON_AddStringToObject(param_root,"os","Linux");

            s_json_param = cJSON_PrintUnformatted(param_root);
            if(s_json_param)
            {
                //dzlog_info("%s", s_json_param);
                err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
                if (err.code == AS_HTTP_200) 
                {
                    //s_result = cJSON_PrintUnformatted(res_root);
                    err.code = zx_Json_GetInt(res_root, "code", 0);
                    dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
                }
                else
                {
                    dzlog_error("err.code=%d  msg=%s",err.code , err.message);
                }
                free(s_json_param);
            }
            cJSON_Delete(param_root);
        }
        free(url);
        zx_Client_Cleanup(&client);
    }
#endif
  }
  return err;
}
#endif

/*============================================================================
方法
入参：
  {
    "account": "string",
    "data_list": [
      {
        "battery": 0,
        "charging": 0,
        "mode": 0,
        "open_time": 0,
        "sub1g": 0,
        "tone_num": 0,
        "wifi": 0
      }
    ],
    "device_sn": "string",
    "device_type": "string",
    "language": "string",
    "model": "string",
    "os": "string",
    "product_name": "string",
    "station_sn": "string",
    "version": "string"
  }
返回：
  {
  }
============================================================================*/
zx_Error zx_upload_power_performance(int channel)
{

  zx_Error err;
 #if 0
  XM_CAMERA_INFO* pcamera = get_camera_info(channel);
  DEV_BASE_PARAM* devParam = zx_get_dev_by_channel(channel);
  err.code = -1;
  if (pcamera && devParam)
  {
    int open_time = pcamera->end_time - pcamera->start_time;

    dzlog_info("camera:%d, power_performance, opentime:%d, ir_mode:%d, playtone:%d, wifi:%d, sub1g:%d, charge:%d, power:%d",
               channel, open_time, pcamera->ir_mode, pcamera->tone_num,
               devParam->dev_wifi_rssi, devParam->dev_sub1g_rssi, devParam->dev_charge_status, devParam->dev_electric_quantity);

    char* url = zx_String_Concat2(AS_HOST, "/v1/hub/logging/upload_power_event");    
    if (url)
    {
        zx_Client client;
        char *s_json_param = NULL;
        cJSON *res_root = NULL, *param_root = NULL, *param_list = NULL, *param_heard = NULL;

        zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
        param_root = cJSON_CreateObject();
        param_heard = cJSON_CreateArray();

        if (param_root && param_heard)
        {
            cJSON_AddStringToObject(param_root,"account",base_param->hub_info.account_id);
            cJSON_AddStringToObject(param_root,"station_sn",base_param->hub_info.hub_sn);
            cJSON_AddStringToObject(param_root,"device_sn",devParam->dev_sn);

            param_list = cJSON_CreateObject();
            {
                cJSON_AddNumberToObject(param_list, "mode", pcamera->ir_mode);
                cJSON_AddNumberToObject(param_list, "battery", devParam->dev_electric_quantity);
                cJSON_AddNumberToObject(param_list, "charging", devParam->dev_charge_status);
                cJSON_AddNumberToObject(param_list, "open_time", open_time);
                cJSON_AddNumberToObject(param_list, "sub1g", devParam->dev_sub1g_rssi);
                cJSON_AddNumberToObject(param_list, "wifi", devParam->dev_wifi_rssi);
                cJSON_AddNumberToObject(param_list, "tone_num", pcamera->tone_num);


                cJSON_AddItemToObject(param_heard, "", param_list); // 把参数组JSON加入数组
            }
            cJSON_AddItemToObject(param_root,"data_list", param_heard); // 把数组加入

            cJSON_AddStringToObject(param_root,"version", base_param->hub_info.hub_hardware_main_ver);
            cJSON_AddStringToObject(param_root,"model","EverCam 1080P");
            cJSON_AddStringToObject(param_root,"product_name","Eufy Security");
            cJSON_AddStringToObject(param_root,"os","Linux");

            s_json_param = cJSON_PrintUnformatted(param_root);
            if(s_json_param)
            {
                //dzlog_info("%s", s_json_param);
                err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
                if (err.code == AS_HTTP_200) 
                {
                    //s_result = cJSON_PrintUnformatted(res_root);
                    err.code = zx_Json_GetInt(res_root, "code", 0);
                    dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
                }
                else
                {
                    dzlog_error("err.code=%d  msg=%s",err.code , err.message);
                }
                free(s_json_param);
            }
            cJSON_Delete(param_root);
        }
        free(url);
        zx_Client_Cleanup(&client);
    }
  }
  #endif
  return err;
}


/*============================================================================
方法：post /event/hub/cipher/generate
入参：
  {
    "account": "string",
    "station_sn": "string"
  }

返回：
  {
    "code": 0,
    "data": {
      "cipher_id": 0,       // 公钥号
      "modulus": "string"   // 公钥的n模, '0'~'F'
    },
    "msg": "成功"
  }
============================================================================*/
zx_Error zx_get_RSA_pubkey(const char* account_id,
                                const char* station_sn,
                                unsigned char* pub_key, unsigned char* pub_id)
{
  	zx_Error err;
  	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/cipher/generate");
  	err.code = -1;
  	if (url && account_id && station_sn && pub_key && pub_id)
  	{
		char *s_json_param = NULL;
  		cJSON *res_root = NULL, *param_root = NULL;
  		zx_Client client;
  		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
      	param_root = cJSON_CreateObject();

      	if (param_root)
      	{
          	cJSON_AddStringToObject(param_root,"account",account_id);
          	cJSON_AddStringToObject(param_root,"station_sn",station_sn);          

          	s_json_param = cJSON_PrintUnformatted(param_root);
          	if(s_json_param)
          	{
              	//dzlog_info("%s", s_json_param);

              	err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
              	//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
              	if (err.code == AS_HTTP_200)
              	{
                  	err.code = zx_Json_GetInt(res_root, "code", 0);
                  	dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
                  	if (err.code == 0)
                  	{
                      	cJSON *js_list = cJSON_GetObjectItem(res_root, "data");
                      	if (js_list)
                      	{
                        	//char *s_result = cJSON_PrintUnformatted(js_list);
                        	//dzlog_info("%s", s_result);
                        	//free(s_result);
                        	const char *key = zx_Json_GetString(js_list, "modulus", NULL);
                        	int ilen = strlen(key);
                        	if (ilen == 256)
                        	{
                            	memcpy(pub_key, key, 256);
                            	*pub_id = zx_Json_GetInt(js_list, "cipher_id", 0); // 如果取不到,不加密
                            	dzlog_info("get cipher_id: %d", *pub_id);
                        	}
                        	else
                        	{
                            	dzlog_info("pubkey len:%d err!", ilen);
                            	err.code = -1;
                        	}
                      	}
                  	}
              	}
			  	else
				  	dzlog_error("err.code=%d  msg=%s",err.code , err.message);
              	free(s_json_param);
          	}
          	cJSON_Delete(param_root);
      	}
      	free(url);
		zx_Client_Cleanup(&client);
  	}
  	return err;
}


/*============================================================================
方法：POST /cloud/hub/get_uptoken 
入参：{
"account": "string",	 //用户ID
"file_name": "string",  //文件名称
"file_size": 0,	  //文件大小
"station_sn": "string",	//基站sn
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
		const char *account,       //用户ID 
		const char *file_name ,    // 文件名称
		unsigned long file_size ,  // 文件大小
		const char *device_sn ,    //设备SN 
		unsigned int type,         //文件类型 1-基站异常 2-移动端异常 3-基站监控缩略图 4-基站监控视频
		bool disable_ssl,	       //是否禁用ssl
		zx_Client client)
{
	zx_Error err;
	zx_Json *res_root = NULL, *param_root = NULL;
	char *s_json_param = NULL;
	char *url = NULL;
	char *s_result = NULL;
	int retry_count = 0;
	
	if(!account || !file_name || !device_sn)
	{
		dzlog_warn("parameter is NULL,Please check the parameters");
		return NULL;
	}

    if (type == STATION_ABNORMAL)
    {
        url = zx_String_Concat2(AS_HOST, "/v1/hub/logging/get_up_token");
    }
    else
    {
        url = zx_String_Concat2(AS_HOST, "/v1/cloud/hub/get_uptoken");
    }    

	if( !url )
	{
		dzlog_error("******Aws String Concat2 error******");
		goto exit;
	}
	param_root = cJSON_CreateObject();

	if(!param_root)
	{
		dzlog_error("cJSON CreateObject is error");
		goto exit;
	}

	cJSON_AddStringToObject(param_root,"account",account);
	cJSON_AddStringToObject(param_root,"file_name",file_name);
	cJSON_AddNumberToObject(param_root,"file_size",file_size);
    if (type == STATION_ABNORMAL)
    {
        cJSON_AddStringToObject(param_root,"station_sn",device_sn);
    }
    else
    {
        cJSON_AddStringToObject(param_root,"device_sn",device_sn);
    }
	cJSON_AddNumberToObject(param_root,"type",type);
	
	if(disable_ssl)
		cJSON_AddTrueToObject(param_root,"disable_ssl");  //disable_ssl
	else
		cJSON_AddFalseToObject(param_root,"disable_ssl");

	s_json_param = cJSON_PrintUnformatted(param_root);
	
	if(!s_json_param)
	{
		dzlog_error("cJSON PrintUnformatted is error");
		goto exit;
	}
	
RETRY_AGAIN:	
	err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
	
	if (err.code == AS_HTTP_200) 
	{
		s_result = cJSON_PrintUnformatted(res_root);
		
		if(!s_result)
		{
			dzlog_error("cJSON PrintUnformatted is error");
			goto exit;
		}
		//dzlog_info("cJSON = %s",s_result);
		dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), zx_Json_GetString(res_root, "msg", ""));
	}
	else
	{
		dzlog_error("err code=%d msg=%s", err.code, err.message);

		//无法连接至主机或代理服务器或者域名解析失败 重试
		if( CURLE_COULDNT_CONNECT == err.code  || CURLE_COULDNT_RESOLVE_HOST == err.code )		
		{
			++retry_count;
			if(retry_count <= AS_TRY_COUNT)  //重试三次
			{
				zx_mgw_setTimer(1,0);		//延时1秒后重试
				dzlog_info("Aws uptoken request retry %d count",retry_count);
				goto RETRY_AGAIN;
				
			}
			else
			{
				dzlog_error("Aws uptoken request %d count,please check network. err code = %d",retry_count, err.code);
			}
		}
	}
exit:
	if(url) 
	{
		free(url); 
		url = NULL;
	}

	if(s_result) 
	{
		free(s_result); 
		s_result = NULL;
	}

	if(s_json_param) 
	{
		free(s_json_param); 
		s_json_param = NULL;
	}

	if(param_root) 
	{
		cJSON_Delete(param_root); 
		param_root = NULL;
	}
	return res_root;
}

/*============================================================================
方法：POST /cloud/hub/get_dntoken 
入参：{
"account": "string",	//用户ID
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
	const char *account  , 		//用户ID
	const char *file_key , 		//文件keywfhg
	const char *station_sn,		//基站SN
	zx_Client client
)
{
	zx_Error err;
	zx_Json *res_root = NULL, *param_root = NULL;
	char *url = NULL;
	char *s_json_param = NULL;
	char *s_result = NULL;
	int retry_count = 0;
	
	if(!account || !file_key || !station_sn)
	{
		dzlog_warn("parameter is NULL,Please check the parameters");
		return NULL;
	}
		
	url = zx_String_Concat2(AS_HOST, "/v1/cloud/hub/get_dntoken");
	
	if( !url )
	{
		dzlog_error("******Aws String Concat2 error******");
		goto exit;
	}

	param_root = cJSON_CreateObject();

	if(!param_root)
	{
		dzlog_error("cJSON CreateObject is error");
		goto exit;
	}

	cJSON_AddStringToObject(param_root,"account",account);
	cJSON_AddStringToObject(param_root,"file_key",file_key);
	cJSON_AddStringToObject(param_root,"station_sn",station_sn);

	s_json_param = cJSON_PrintUnformatted(param_root);
	
	if(!s_json_param)
	{
		dzlog_error("cJSON_PrintUnformatted is error");
		goto exit;
	}
	
RETRY_AGAIN:
	err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");

	if (err.code == AS_HTTP_200) 
	{
		s_result = cJSON_PrintUnformatted(res_root);

		if(!s_result)
		{
			dzlog_error("cJSON PrintUnformatted is error");
			goto exit;
		}
		
		//dzlog_info("cJSON = %s",s_result);
		dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), zx_Json_GetString(res_root, "msg", ""));
	}
	else
	{			
		dzlog_error("err code=%d msg=%s", err.code, err.message);

		//无法连接至主机或代理服务器或者域名解析失败 重试
		if( CURLE_COULDNT_CONNECT == err.code || CURLE_COULDNT_RESOLVE_HOST == err.code )		
		{
			++retry_count;
			if(retry_count <= AS_TRY_COUNT)	//重试三次
			{
				zx_mgw_setTimer(1,0);		//延时1秒后重试
				dzlog_info("Aws dntoken request retry %d count",retry_count);
				goto RETRY_AGAIN;
				
			}
			else
			{
				dzlog_error("Aws dntoken request %d count,please check network. err code = %d",retry_count,err.code);
			}
		}
	}

exit:	
	if(url) 
	{
		free(url); 
		url = NULL;
	}

	if(s_result) 
	{
		free(s_result); 
		s_result = NULL;
	}

	if(s_json_param) 
	{
		free(s_json_param); 
		s_json_param = NULL;
	}

	if(param_root) 
	{
		cJSON_Delete(param_root); 
		param_root = NULL;
	}
	return res_root;
}


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
	const char *current_version_name, 		//当前对应 device_type 的版本名称
	const char *device_type, 				// device type的名称,实际是产品硬件版本，包含了产品信息和硬件版本，后端通过该字段来决定客户端需要的是那种固件包
	unsigned long rom_version, 				// rom的版本号
	const char *sn, 						// 序列号
	zx_Client client
)
{
	zx_Error err;
	zx_Json *res_root = NULL, *param_root = NULL;
	char *url = NULL;
	char *s_result = NULL;
	char *s_json_param = NULL;
	int retry_count = 0;
	
	if(!current_version_name || !device_type || !sn)
	{
		dzlog_warn("parameter is NULL,Please check the parameters");
		return NULL;
	}
	
	//url = zx_String_Concat2(AS_ROM_UPGRADE, "/v1/security/upgrade/fetch-rom-version");
	url = zx_String_Concat2(AS_HOST, "/v1/hub/ota/get_rom_version");  //2018-5-7 修改为域名https接口
	
	if( !url )
	{
		dzlog_error("******String Concat2 error******");
		goto exit;
	}
	param_root = cJSON_CreateObject();

	if(!param_root)
	{
		dzlog_error("cJSON CreateObject is error");
		goto exit;
	}

	cJSON_AddStringToObject(param_root,"current_version_name",current_version_name);
	cJSON_AddStringToObject(param_root,"device_type",device_type);
	cJSON_AddNumberToObject(param_root,"rom_version",rom_version);
	cJSON_AddStringToObject(param_root,"sn",sn);
	
	s_json_param = cJSON_PrintUnformatted(param_root);

	if(!s_json_param)
	{
		dzlog_error("cJSON_PrintUnformatted is error");
		goto exit;
	}
	
RETRY_AGAIN:
	err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
	
	if (err.code == AS_HTTP_200) 
	{
		s_result = cJSON_PrintUnformatted(res_root);

		if(!s_result)
		{
			dzlog_error("cJSON PrintUnformatted is error");
			goto exit;
		}
		//dzlog_info("cJSON = %s",s_result);
		dzlog_info("code=%d  msg=%s",zx_Json_GetInt(res_root, "code", 0),  zx_Json_GetString(res_root, "msg", "")); 
	}
	else
	{
		dzlog_error("err code=%d message=%s", err.code, err.message);

		//无法连接至主机或代理服务器或者域名解析失败 重试
		if( CURLE_COULDNT_CONNECT == err.code || CURLE_COULDNT_RESOLVE_HOST == err.code )		
		{
			++retry_count;
			if(retry_count <= AS_TRY_COUNT)		//重试三次
			{
				zx_mgw_setTimer(1,0);		//延时1秒后重试
				dzlog_info("rom upgrade request retry %d count",retry_count);
				goto RETRY_AGAIN;
			
			}
			else
			{
				dzlog_error("rom upgrade request %d count,please check network. err code = %d",retry_count,err.code);
			}
		}
	}
exit:
	if(url) 
	{
		free(url); 
		url = NULL;
	}

	if(s_result) 
	{
		free(s_result); 
		s_result = NULL;
	}

	if(s_json_param) 
	{
		free(s_json_param); 
		s_json_param = NULL;
	}

	if(param_root) 
	{
		cJSON_Delete(param_root); 
		param_root = NULL;
	}
	return res_root;
}

int zx_get_as_server(void)
{
    int index = 0;

#if 0
    index = getNvramIndex("2860");
    if( index == -1 )
    {
        dzlog_error("getNvramIndex failed.");
        return -1;
    }
    const char *pserver = nvram_bufget(index, "AS_SERVER");

    dzlog_info("get_as_server: %d", *pserver);
	return *pserver;
#endif

	index = zx_hisi_get_as_server_index(index,sizeof(int));

	if(index != sizeof(int))
	{
		dzlog_error("hisi get as server index error");
		return -1;
	}

	return index;
    
}

int zx_set_as_server(int as_server)
{
	
    int index = 0;

#if 0
    index = getNvramIndex("2860");
    if( index == -1 )
    {
        dzlog_error("getNvramIndex failed.");
        return -1;
    }
    nvram_set(index, "AS_SERVER", &as_server);
#endif

	index = zx_hisi_set_as_server_index(as_server,sizeof(int));

	if(index != sizeof(int))
	{
		dzlog_error("hisi set as server index error");
		return -1;
	}

    dzlog_info(" 【 set_as_server: %d 】 ", as_server);
	
    return 0;
}


/*
To receive the response in JSON format, send a GET request to
A successful request will return
{
    "status": "success",
    "country": "COUNTRY",
    "countryCode": "COUNTRY CODE",
    "region": "REGION CODE",
    "regionName": "REGION NAME",
    "city": "CITY",
    "zip": "ZIP CODE",
    "lat": LATITUDE,
    "lon": LONGITUDE,
    "timezone": "TIME ZONE",
    "isp": "ISP NAME",
    "org": "ORGANIZATION NAME",
    "as": "AS NUMBER / NAME",
    "query": "IP ADDRESS USED FOR QUERY"
}

"countryCode": "US",
"countryCode": "HK"
"countryCode": "CN"
*/
zx_Error zx_get_wlan_ip_region(void)
{
    zx_Error err;
    err.code = 0;
    as_server_index = zx_get_as_server();
    dzlog_info("【 zx_get_wlan_ip_region (%d) 】", as_server_index);

    switch (as_server_index)
    {
        case AS_SERVER_US:
        {
            AS_HOST = "https://security-app.eufylife.com";     // USA 线上
        }
        break;

        case AS_SERVER_EU:
        {
            AS_HOST = "https://security-app-eu.eufylife.com";  // EU
        }
        break;

        case AS_SERVER_QA:
        {
            AS_HOST = "https://security-app-qa.eufylife.com";  // USA 测试用
        }
        break;

        case AS_SERVER_CI:
        {
            AS_HOST = "http://security-app-ci.eufylife.com";  // 研发测试用http
        }
        break;

        default:
        {
            AS_HOST = "https://security-app-qa.eufylife.com";  // USA 测试用   
#if 0
            cJSON *res_root;
            char* url = "http://ip-api.com/json/";
            zx_Client client;

            zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
            if (url)
            {
                err = zx_Client_Call(&client, &res_root, url, "GET");
                if (err.code == AS_HTTP_200)
                {
                    char * country_code = zx_Json_GetString(res_root, "countryCode", NULL);
                    dzlog_info("countryCode=%s, timezone=%s",
                                country_code, zx_Json_GetString(res_root, "timezone", NULL));
                    if (strcmp(country_code,"US") == 0)
                    {
                        AS_HOST = "https://security-app.eufylife.com";     // USA 线上
                    }
                    else if (strcmp(country_code,"HK") == 0)
                    {
                        AS_HOST = "https://security-app-qa.eufylife.com";  // USA 测试用
                    }
                    else
                    {
                        //AS_HOST = "https://security-app-eu.eufylife.com";  // EU
                        AS_HOST = "https://security-app.eufylife.com";     // 先保持原来的
                    }
                }
                else
                {
                    dzlog_info("err.code=%d  msg=%s",err.code , err.message);
                }
            }
            zx_Client_Cleanup(&client);
#endif

        }
        break;
    }

    dzlog_info("【 AS_HOST: %s】", AS_HOST);
    return err;
}



zx_Error zx_active_curl_for_thread(void)
{
	zx_Error err;
	//cJSON *res_root;	
	char* url = "www.baidu.com";//zx_String_Concat2(AS_HOST, "/ping");
	zx_Client client;
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	if (url)
	{
   		//err = zx_Client_Call(&gclient, &res_root, url, "GET");
		err = zx_Client_CallNoRet(&client, url, "GET");
		//dzlog_info("err.code=%d  msg=%s",err.code , err.message);
		/*if (err.code == AS_HTTP_200) 
		{
			err.code = zx_Json_GetInt(res_root, "code", 0);
            dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
		}*/
		//free(url);
	}	
	zx_Client_Cleanup(&client);
	//curl_global_cleanup();
	return err;
}



void zx_init_as_interface(void)
{
	//memset(&gclient, 0, sizeof(zx_Client));
	zx_Global_Init(-1);
	//zx_Client_InitEx(&gclient, zx_NoAuth, RESP_BODY_SIZE);
}


void zx_deinit_as_interface(void)
{
	//zx_Client_Cleanup(&gclient);
	curl_global_cleanup();
}

/*
方法：POST /hub/check_hub
入参：{
  "account": "string",
  "station_sn": "string"
}
返回：0 - 基站已绑定到当前用户
20003 - 基站未绑定
20020 - 基站已绑定到其它用户
*/
zx_Error zx_get_hub_bind_status(const char* account_id, const char *station_sn)
{
	zx_Error err;
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/check_hub");
	err.code = -1;
	if (url && account_id && station_sn )
	{
		char *s_json_param = NULL;
		cJSON *res_root = NULL, *param_root = NULL;// *param_list, *param_heard;
		zx_Client client;

		zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
    	param_root = cJSON_CreateObject();

		if (param_root)
		{
			cJSON_AddStringToObject(param_root,"station_sn",station_sn);
			cJSON_AddStringToObject(param_root,"account",account_id);
		
			s_json_param = cJSON_PrintUnformatted(param_root);
			if(s_json_param)
			{
				//dzlog_info("%s", s_json_param);
				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",err.code, zx_Json_GetString(res_root, "msg", 0));
				}
				else
					dzlog_error("err.code=%d  msg=%s",err.code , err.message);

            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
		zx_Client_Cleanup(&client);
	}
	
	return err;
}


int zx_get_josn_string(const char *sn, const short channel, const char *filepath, const short storge_type, char *out_str)
{
	zx_Json *param_root = NULL;
	char *s_json_param = NULL;
	
	if (sn && filepath && out_str)
	{
		param_root = cJSON_CreateObject();
		if (param_root)
		{
			cJSON_AddStringToObject(param_root,"sn",sn);
			cJSON_AddNumberToObject(param_root,"ch",channel);
			cJSON_AddNumberToObject(param_root,"type",storge_type);		
			cJSON_AddStringToObject(param_root,"Path",filepath);
       		s_json_param = cJSON_PrintUnformatted(param_root);
        	if(s_json_param)
			{
				memcpy(out_str, s_json_param, strlen(s_json_param));
				free(s_json_param);
        	}
        	cJSON_Delete(param_root);
			return 1;
		}
	}
	return 0;
}

//http://ip-api.com/json/


