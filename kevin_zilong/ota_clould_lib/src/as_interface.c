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
#if 0
zx_Error zx_hub_get_p2p_params(const char* station_sn, P2P_BASE_INFO *p2p_info)
{
	zx_Error err;
	err.code = -1; //did && init_str && p2p_license && push_license
	if (station_sn && p2p_info)
	{
		zx_Client client;
		char *s_json_param;
		cJSON *res_root, *param_root;
	
		//zx_Global_Init(-1);
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
					dzlog_info("err.code=%d  msg=%s",err.code , err.message);
					//dzlog_info("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    				//dzlog_info("response body:%s", zx_Buffer_CStr(&client.b));
					if (err.code == AS_HTTP_200) 
					{
						err.code = zx_Json_GetInt(res_root, "code", 0);
						//int ires = zx_Json_GetInt(res_root, "code", 0);
						//char *s_result = cJSON_PrintUnformatted(res_root);
						if (err.code == 0)
						{
                        	cJSON *js_list = cJSON_GetObjectItem(res_root, "data");
							char *s_result = cJSON_PrintUnformatted(js_list);
							dzlog_info("%s", s_result);
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
							
							const char * wipn_enc_dec_key =  zx_Json_GetString(js_list, "wipn_enc_dec_key", NULL);
							memcpy(p2p_info->push_encdec_key, wipn_enc_dec_key, strlen(wipn_enc_dec_key));
							const char * wipn_ndt_aes128key =  zx_Json_GetString(js_list, "wipn_ndt_aes128key", NULL);
							memcpy(p2p_info->push_aes128_key, wipn_ndt_aes128key, strlen(wipn_ndt_aes128key));
							const char * query_server_did = zx_Json_GetString(js_list, "query_server_did", NULL);
							memcpy(p2p_info->push_query_ser_list, query_server_did, strlen(query_server_did));
							//dzlog_info("init_str=%s p2p_uid=%s p2p_license=%s...", init_str, did, p2p_license);
							free(s_result);	
						}
					}
            		free(s_json_param);
        		}
        		cJSON_Delete(param_root);
    		}
			free(url);	
		}
		zx_Client_Cleanup(&client);
		//curl_global_cleanup();
	}
	
	return err;
}
#endif

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
	zx_Client client;
	char *s_json_param;
	//char *s_result;
	cJSON *res_root, *param_root;

	//zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/bind_devs");
	err.code = -1;
	if (url)
	{
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
				//dzlog_info("******call start******");
				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				//dzlog_info("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    			//dzlog_info("response body:\n%s", zx_Buffer_CStr(&client.b));
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
				}

            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
	}
	zx_Client_Cleanup(&client);
	//curl_global_cleanup();
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
	char *s_json_param;
	cJSON *res_root, *param_root;
	zx_Client client;
	//zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/unbind_devs");
	err.code = -1;
	if (url)
	{
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
				dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
				}
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
	}
	zx_Client_Cleanup(&client);
	//curl_global_cleanup();
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
	zx_Client client;
	char *s_json_param;
	//char *s_result;
	cJSON *res_root, *param_root;

	//zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/check_device");
	err.code = -1;
	if (url)
	{
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
				dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				//dzlog_info("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    			//dzlog_info("response body:\n%s", zx_Buffer_CStr(&client.b));
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
				}
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
	}
	zx_Client_Cleanup(&client);
	//curl_global_cleanup();
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
	zx_Client client;
	char *s_json_param;
	//char *s_result;
	cJSON *res_root, *param_root;

	//zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/update_devs_info");
	err.code = -1;
	if (url)
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
				dzlog_info("%s", s_json_param);
				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				//dzlog_info("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    			//dzlog_info("response body:\n%s", zx_Buffer_CStr(&client.b));
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
				}
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
	}
	zx_Client_Cleanup(&client);
	//curl_global_cleanup();
	return err;
}


zx_Error zx_hub_edit_devs_intinfo_by_paramname(const char* device_sn, const char* station_sn, const char *account, 
                     const char* param_name, int value)
{
	zx_Error err;
	zx_Client client;
	char *s_json_param;
	//char *s_result;
	cJSON *res_root, *param_root;

	//zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/update_devs_info");
	err.code = -1;
	if (url)
	{
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
				dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				//dzlog_info("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    			//dzlog_info("response body:\n%s", zx_Buffer_CStr(&client.b));
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
				}
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
	}
	zx_Client_Cleanup(&client);
	//curl_global_cleanup();
	return err;
}


zx_Error zx_update_devs_version(const char* device_sn, const char* station_sn, const char *account, 
                     const char* main_sw_version, const char *sec_sw_version, const char *hw_version)
{
	zx_Error err;
	zx_Client client;
	char *s_json_param;
	//char *s_result;
	cJSON *res_root, *param_root;

	//zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/update_devs_info");
	err.code = -1;
	if (url)
	{
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
				dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				//dzlog_info("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    			//dzlog_info("response body:\n%s", zx_Buffer_CStr(&client.b));
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
				}
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
	}
	zx_Client_Cleanup(&client);
	//curl_global_cleanup();
	return err;
}


zx_Error zx_hub_edit_devs_name(const char* device_sn, const char* station_sn, const char* device_name, const char *account)
{
	zx_Error err;
	char *s_json_param;
	cJSON *res_root, *param_root;
	zx_Client client;
	//zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/update_devs_info");
	err.code = -1;
	if (url)
	{
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
				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
				}
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
	}
	zx_Client_Cleanup(&client);
	//curl_global_cleanup();
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
	zx_Client client;
	char *s_json_param;
	//char *s_result;
	cJSON *res_root, *param_root;

	//zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/update_hub_info");
	err.code = -1;
	if (url)
	{
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
				dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				//dzlog_info("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    			//dzlog_info("response body:\n%s", zx_Buffer_CStr(&client.b));
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
				}
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
	}
	zx_Client_Cleanup(&client);
	//curl_global_cleanup();
	return err;
}


zx_Error zx_hub_update_info_by_infoname(const char* station_sn, const char* account, const char* infoname, const char *value)
{
	zx_Error err;
	char *s_json_param;
	cJSON *res_root, *param_root;
	zx_Client client;
	//zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/update_hub_info");
	err.code = -1;
	if (url)
	{
    	param_root = cJSON_CreateObject();
		if (param_root)
		{
			cJSON_AddStringToObject(param_root,"station_sn",station_sn);
			cJSON_AddStringToObject(param_root,"account",account);
			cJSON_AddStringToObject(param_root, infoname, value);

       		s_json_param = cJSON_PrintUnformatted(param_root);
        	if(s_json_param)
			{
				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
				}
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
	}
	zx_Client_Cleanup(&client);
	//curl_global_cleanup();
	return err;
}


zx_Error zx_hub_update_version(const char* station_sn, const char* account, const char* main_soft_version, const char *sec_soft_version)
{
	zx_Error err;
	char *s_json_param;
	cJSON *res_root, *param_root;
	zx_Client client;
	//zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/update_hub_info");
	err.code = -1;
	if (url)
	{
    	param_root = cJSON_CreateObject();
		if (param_root)
		{
			cJSON_AddStringToObject(param_root,"station_sn",station_sn);
			cJSON_AddStringToObject(param_root,"account",account);
			cJSON_AddStringToObject(param_root, "main_sw_version", main_soft_version);
			cJSON_AddStringToObject(param_root, "sec_sw_version", sec_soft_version);

       		s_json_param = cJSON_PrintUnformatted(param_root);
        	if(s_json_param)
			{
				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
				}
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
	}
	zx_Client_Cleanup(&client);
	//curl_global_cleanup();
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
	zx_Client client;
	char *s_json_param;
	//char *s_result;
	cJSON *res_root, *param_root;

	//zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/get_dev_info");
	err.code = -1;
	if (url)
	{
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
				dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				//dzlog_info("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    			//dzlog_info("response body:\n%s", zx_Buffer_CStr(&client.b));
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
				}
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
	}
	zx_Client_Cleanup(&client);
	//curl_global_cleanup();
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
	zx_Client client;
	char *s_json_param;
	//char *s_result;
	cJSON *res_root, *param_root;

	//zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/get_devs_params");
	err.code = -1;
	if (url)
	{
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
				dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				//dzlog_info("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    			//dzlog_info("response body:\n%s", zx_Buffer_CStr(&client.b));
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
				}
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
	}
	zx_Client_Cleanup(&client);
	//curl_global_cleanup();
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
	zx_Client client;
	char *s_json_param;
	//char *s_result;
	cJSON *res_root, *param_root;

	//zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/get_hub_params");
	err.code = -1;
	if (url)
	{
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
				dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				//dzlog_warn("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    			//dzlog_warn("response body:\n%s", zx_Buffer_CStr(&client.b));
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
				}
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
	}
	zx_Client_Cleanup(&client);
	//curl_global_cleanup();
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
	zx_Client client;
	char *s_json_param;
	//char *s_result;
	cJSON *res_root, *param_root;

	//zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/get_rtsp_connectinfo");
	err.code = -1;
	if (url)
	{
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
				dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				//dzlog_warn("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    			//dzlog_warn("response body:\n%s", zx_Buffer_CStr(&client.b));
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
				}
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
	}
	zx_Client_Cleanup(&client);
	//curl_global_cleanup();
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
	zx_Client client;
	char *s_json_param;
	//char *s_result;
	cJSON *res_root, *param_root;

	//zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/reset");
	err.code = -1;
	if (url)
	{
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
				dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				//dzlog_warn("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    			//dzlog_warn("response body:\n%s", zx_Buffer_CStr(&client.b));
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
				}
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
	}
	zx_Client_Cleanup(&client);
	//curl_global_cleanup();
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
zx_Error zx_upload_devs_params(const char* account_id, const char* station_sn, const char* device_sn, int param_type, const char* param_value)
{
	zx_Error err;
	char *s_json_param;
	cJSON *res_root, *param_root, *param_list, *param_heard;
	zx_Client client;
	//zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/upload_devs_params");
	err.code = -1;
	if (url && account_id && station_sn && device_sn && param_value)
	{
    	param_root = cJSON_CreateObject();
		param_heard = cJSON_CreateArray();
		param_list = cJSON_CreateObject();
				
		if (param_root && param_list)
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
				dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
				}

            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
	}
	zx_Client_Cleanup(&client);
	//curl_global_cleanup();
	return err;
}

zx_Error zx_upload_devs_int_params(const char* account_id, const char* station_sn, const char* device_sn, int param_type, const int param_value)
{
	zx_Error err;
	char *s_json_param;
	cJSON *res_root, *param_root, *param_list, *param_heard;
	zx_Client client;
	//zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/upload_devs_params");
	err.code = -1;
	if (url && account_id && station_sn && device_sn && param_value)
	{
    	param_root = cJSON_CreateObject();
		param_heard = cJSON_CreateArray();
		param_list = cJSON_CreateObject();
				
		if (param_root && param_list)
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
				dzlog_info("%s", s_json_param);

				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
				}

            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
	}
	zx_Client_Cleanup(&client);
	//curl_global_cleanup();
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
	zx_Client client;
	char *s_json_param;
	//char *s_result;
	cJSON *res_root, *param_root, *param_list, *param_heard;

	//zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/upload_hub_params");
	err.code = -1;
	if (url && account_id && station_sn && param_value)
	{
    	param_root = cJSON_CreateObject();
		param_heard = cJSON_CreateArray();
		param_list = cJSON_CreateObject();
				
		if (param_root && param_list)
		{
			cJSON_AddStringToObject(param_root,"station_sn",station_sn);
			cJSON_AddStringToObject(param_root,"account",account_id);

			cJSON_AddItemToObject(param_heard, "", param_list);
			cJSON_AddItemToObject(param_root,"params", param_heard);
			
			cJSON_AddNumberToObject(param_list, "param_type", param_type);
			cJSON_AddStringToObject(param_list, "param_value", param_value); 			
       		s_json_param = cJSON_PrintUnformatted(param_root);
        	if(s_json_param)
			{
				dzlog_info("%s", s_json_param);

				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				//dzlog_warn("response header:\n%s", zx_Buffer_CStr(&client.respHeader));
    			//dzlog_warn("response body:\n%s", zx_Buffer_CStr(&client.b));
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
				}

            	free(s_json_param);
        	}
			//cJSON_Delete(param_list);
        	cJSON_Delete(param_root);
    	}
		free(url);
	}
	zx_Client_Cleanup(&client);
	//curl_global_cleanup();
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
	
	char *s_json_param;
	cJSON *res_root, *param_root;	
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/bind_hub");
	zx_Client client;
	//zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	err.code = -1;
	if (url)
	{
    	param_root = cJSON_CreateObject();
		dzlog_warn("start %s %s %s %s******", account_id, station_sn, hub_name, time_zone);
		if (param_root)
		{
			cJSON_AddStringToObject(param_root,"account",account_id);
			cJSON_AddStringToObject(param_root,"sn",station_sn);
			//cJSON_AddStringToObject(param_root,"name",hub_name);
			//cJSON_AddStringToObject(param_root,"time_zone",time_zone);

       		s_json_param = cJSON_PrintUnformatted(param_root);
        	if(s_json_param)
			{
				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				//dzlog_warn("call start %s %s******", url, s_json_param);
				dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				if (err.code == AS_HTTP_200) 
				{
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
				}
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
	}
	zx_Client_Cleanup(&client);
	//curl_global_cleanup();
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
	
	char *s_json_param;
	cJSON *res_root, *param_root;	
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/unbind_hub");
	zx_Client client;
	//zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	err.code = -1;
	if (url)
	{
    	param_root = cJSON_CreateObject();
		dzlog_info("start %s %s ******", account_id, station_sn);
		if (param_root)
		{
			cJSON_AddStringToObject(param_root,"account",account_id);
			cJSON_AddStringToObject(param_root,"station_sn",station_sn);

       		s_json_param = cJSON_PrintUnformatted(param_root);
        	if(s_json_param)
			{
				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				dzlog_info("call start %s %s******", url, s_json_param);
				dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				if (err.code == AS_HTTP_200) 
				{
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
				}
            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
	}
	zx_Client_Cleanup(&client);
	//curl_global_cleanup();
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
#if 0
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
                                              const int* pface_id)
{
    zx_Error err;
    char *s_json_param;
    cJSON *res_root, *param_root, *param_list;
    zx_Client client;
    //zx_Global_Init(-1);
    zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
    char* url = zx_String_Concat2(AS_HOST, "/v1/event/hub/upload_history_record");
    err.code = -1;
    if (url && account_id && station_sn && device_sn)
    {
        param_root = cJSON_CreateObject();
        param_list = cJSON_CreateArray();
        if (param_root && param_list)
        {
            int i;
            cJSON_AddStringToObject(param_root,"account",account_id);
            cJSON_AddStringToObject(param_root,"station_sn",station_sn);
            cJSON_AddStringToObject(param_root,"device_sn",device_sn);

            cJSON_AddNumberToObject(param_root,"start_time", start_time);
            cJSON_AddNumberToObject(param_root,"end_time", end_time);
            cJSON_AddNumberToObject(param_root,"frame_num",framenum);
            cJSON_AddNumberToObject(param_root,"storage_type", storagetype);  // 1: 本地 3:本地+云
            if (pface_id != NULL && *pface_id > 0)
            {
                cJSON_AddItemToObject(param_root,"ai_face_ids", param_list);
                for (i=0; i<FACE_NUM_MAX; i++)
                {
                    if (*pface_id)
                    {
                        cJSON_AddNumberToArry(param_list, *pface_id);
                        pface_id++;
                    }
                    else
                    {
                        break;
                    }
                }
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
                dzlog_info("err.code=%d  msg=%s",err.code , err.message);
                if (err.code == AS_HTTP_200) 
                {
                    err.code = zx_Json_GetInt(res_root, "code", 0);
                    dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), zx_Json_GetString(res_root, "msg", 0));
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
    }
    zx_Client_Cleanup(&client);
    //curl_global_cleanup();

    return err;
}
#endif
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
    char *s_json_param;
    cJSON *res_root, *param_root;// *param_list, *param_heard;
    zx_Client client;
    //zx_Global_Init(-1);
    zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
    char* url = zx_String_Concat2(AS_HOST, "/v1/event/hub/delete_record_by_path");
    err.code = -1;
    if (url && account_id && station_sn && local_path)
    {
      param_root = cJSON_CreateObject();

      if (param_root)
      {
        cJSON_AddStringToObject(param_root,"account",account_id);
        cJSON_AddStringToObject(param_root,"station_sn",station_sn);
        cJSON_AddStringToObject(param_root,"storage_path",local_path);

        s_json_param = cJSON_PrintUnformatted(param_root);
        if(s_json_param)
        {
          dzlog_info("%s", s_json_param);

          err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "DELETE");
          dzlog_info("err.code=%d  msg=%s",err.code , err.message);
          if (err.code == AS_HTTP_200)
          {
            err.code = zx_Json_GetInt(res_root, "code", 0);
            dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), zx_Json_GetString(res_root, "msg", 0));
          }

          free(s_json_param);
        }
        cJSON_Delete(param_root);
      }
      free(url);
    }
    zx_Client_Cleanup(&client);
    //curl_global_cleanup();
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
											  const char* station_sn,
											  int id)
{
	zx_Error err;
	char *s_json_param;
	cJSON *res_root, *param_root;// *param_list, *param_heard;
	zx_Client client;
	//zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	char* url = zx_String_Concat2(AS_HOST, "/v1/event/hub/delete_record");
	err.code = -1;
	if (url && account_id && station_sn)
	{
		param_root = cJSON_CreateObject();

		if (param_root)
		{
			cJSON_AddStringToObject(param_root,"account",account_id);
			cJSON_AddStringToObject(param_root,"station_sn",station_sn);
			cJSON_AddNumberToObject(param_root,"monitor_id",id);

			s_json_param = cJSON_PrintUnformatted(param_root);
			if(s_json_param)
			{
				dzlog_info("%s", s_json_param);

				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "DELETE");
				dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				if (err.code == AS_HTTP_200)
				{
					err.code = zx_Json_GetInt(res_root, "code", 0);
					dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), zx_Json_GetString(res_root, "msg", 0));
				}

				free(s_json_param);
			}
			cJSON_Delete(param_root);
		}
		free(url);
	}
	zx_Client_Cleanup(&client);
	//curl_global_cleanup();

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
#if 0
zx_Error zx_delete_hub_history_records(const char* account_id,
                                               const char* station_sn,
                                               int num, const RECORD_INFO* pRecords)
{
  zx_Error err;
  char *s_json_param;
  cJSON *res_root, *param_root, *param_list;
  zx_Client client;
  //zx_Global_Init(-1);
  zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
  char* url = zx_String_Concat2(AS_HOST, "/v1/event/hub/delete_records");
  err.code = -1;
  if (url && account_id && station_sn)
  {
      param_root = cJSON_CreateObject();      
      param_list = cJSON_CreateArray();

      if (param_root && param_list && pRecords)
      {
          int i;
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
              dzlog_info("%s", s_json_param);

              err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "DELETE");
              dzlog_info("err.code=%d  msg=%s",err.code , err.message);
              if (err.code == AS_HTTP_200)
              {
                  err.code = zx_Json_GetInt(res_root, "code", 0);
                  dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), zx_Json_GetString(res_root, "msg", 0));
              }

              free(s_json_param);
          }
          cJSON_Delete(param_root);
      }
      free(url);
  }
  zx_Client_Cleanup(&client);
  //curl_global_cleanup();

  return err;
}
#endif
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
	char *s_json_param;
	cJSON *res_root, *param_root;// *param_list, *param_heard;
	zx_Client client;
	char* url;

	//zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);

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
	  param_root = cJSON_CreateObject();

	  if (param_root)
	  {
		  cJSON_AddStringToObject(param_root,"account",account_id);
		  cJSON_AddStringToObject(param_root,"station_sn",station_sn);
		  cJSON_AddNumberToObject(param_root,"monitor_id",id);

		  s_json_param = cJSON_PrintUnformatted(param_root);
		  if(s_json_param)
		  {
			  dzlog_info("%s", s_json_param);

			  err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
			  dzlog_info("err.code=%d  msg=%s",err.code , err.message);
			  if (err.code == AS_HTTP_200)
			  {
				  err.code = zx_Json_GetInt(res_root, "code", 0);
				  dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), zx_Json_GetString(res_root, "msg", 0));
			  }

			  free(s_json_param);
		  }
		  cJSON_Delete(param_root);
	  }
	  free(url);
	}
	zx_Client_Cleanup(&client);
	//curl_global_cleanup();

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
	//zx_Global_Init(-1);
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
				dzlog_info("%s", s_json_param);

				err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
				dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				if (err.code == AS_HTTP_200)
				{
					err.code = zx_Json_GetInt(res_root, "code", 0);
					dzlog_info("cloud_flag=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), zx_Json_GetString(res_root, "msg", 0));
				}

				free(s_json_param);
			}
			cJSON_Delete(param_root);
		}
		free(url);
	}
	zx_Client_Cleanup(&client);
	//curl_global_cleanup();

	return err;
}


/*============================================================================
方法：post /event/hub/add_face 上传人脸特征点和图片
入参：
  {
    "account": "string",
    "station_sn": "string",    
    "features": [
      {
        "feature": "string",
        "path": "string",
        "from_path": 0,        
        "position": 0
      }
    ],
    "files": [
      "string"
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
  char *s_json_param;
  cJSON *res_root, *param_root, *param_list, *param_heard, *param_image;
  zx_Client client;
  //zx_Global_Init(-1);
  zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
  char* url = zx_String_Concat2(AS_HOST, "/v1/hub/aiassis/add_face");
  err.code = -1;
  if (url && account_id && station_sn && pfacefeature && pfaceinfo)
  {
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

              err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
              dzlog_info("err.code=%d  msg=%s",err.code , err.message);
              if (err.code == AS_HTTP_200)
              {
                  err.code = zx_Json_GetInt(res_root, "code", 0);
                  dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), zx_Json_GetString(res_root, "msg", 0));
                  if (err.code == 0)
                  {
                      cJSON *js_list = cJSON_GetObjectItem(res_root, "data");
                      if (js_list)
                      {
                          char *s_result = cJSON_PrintUnformatted(js_list);
                          dzlog_info("%s", s_result);

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

              free(s_json_param);
          }
          cJSON_Delete(param_root);
      }
      free(url);
  }
  zx_Client_Cleanup(&client);
  //curl_global_cleanup();

  return err;
}
#endif

/*============================================================================
方法：post /event/hub/update_face 更新人脸特征点和图片
入参：
 {
   "account": "string",
   "station_sn": "string",
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
   "sync": 0
 }

返回：
{
 "code": 0,
 "msg": "成功"
}
============================================================================*/
#if 0
zx_Error zx_hub_update_face_features(const char* account_id,
                                             const char* station_sn,
                                             const char* pfacefeature,  /* base64 */
                                             const char* pimage,        /* jep->base64 */                                             
                                             AI_FACE_FEATURE * pfaceinfo)
{
    zx_Error err;
    char *s_json_param;
    cJSON *res_root, *param_root, *param_list, *param_heard, *param_image;
    zx_Client client;
    //zx_Global_Init(-1);
    zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
    char* url = zx_String_Concat2(AS_HOST, "/v1/hub/aiassis/update_face");
    err.code = -1;
    if (url && account_id && station_sn && pfacefeature && pfaceinfo)
    {
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
           cJSON_AddNumberToObject(param_root,"user_id", pfaceinfo->user_id);
           cJSON_AddStringToObject(param_root,"nick_name", pfaceinfo->nick_name);
           cJSON_AddNumberToObject(param_root,"notification", pfaceinfo->notify);
           cJSON_AddNumberToObject(param_root,"sync", 0);
           
           s_json_param = cJSON_PrintUnformatted(param_root);
           if(s_json_param)
           {
               //dzlog_info("%s", s_json_param);

               err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
               dzlog_info("err.code=%d  msg=%s",err.code , err.message);
               if (err.code == AS_HTTP_200)
               {
                   err.code = zx_Json_GetInt(res_root, "code", 0);
                   dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), zx_Json_GetString(res_root, "msg", 0));
                   if (err.code == 0)
                   {
                       cJSON *js_list = cJSON_GetObjectItem(res_root, "data");
                       if (js_list)
                       {
                           char *s_result = cJSON_PrintUnformatted(js_list);
                           dzlog_info("%s", s_result);

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

               free(s_json_param);
           }
           cJSON_Delete(param_root);
       }
       free(url);
    }
    zx_Client_Cleanup(&client);
    //curl_global_cleanup();

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
    char *s_json_param;
    cJSON *res_root, *param_root, *param_list;
    zx_Client client;
    //zx_Global_Init(-1);
    zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
    char* url = zx_String_Concat2(AS_HOST, "/v1/hub/aiassis/delete_faces");
    err.code = -1;
    if (url && account_id && station_sn )
    {
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
              dzlog_info("%s", s_json_param);

              err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "DELETE");
              dzlog_info("err.code=%d  msg=%s",err.code , err.message);
              if (err.code == AS_HTTP_200)
              {
                  err.code = zx_Json_GetInt(res_root, "code", 0);
                  dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), zx_Json_GetString(res_root, "msg", 0));
              }

              free(s_json_param);
          }
          cJSON_Delete(param_root);
      }
      free(url);
    }
    zx_Client_Cleanup(&client);
    //curl_global_cleanup();
    
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
zx_Client client
)
{
	zx_Error err;
	zx_Json *res_root = NULL, *param_root = NULL;
	char *s_json_param = NULL;
	char *url = NULL;
	char *s_result = NULL;
	
	if(!account || !file_name || !device_sn)
	{
		dzlog_warn("parameter is NULL,Please check the parameters");
		return NULL;
	}
		
	url = zx_String_Concat2(AS_HOST, "/v1/cloud/hub/get_uptoken");
	
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
	cJSON_AddStringToObject(param_root,"device_sn",device_sn);
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
		dzlog_info("res_code=%d message=%s",zx_Json_GetInt(res_root, "res_code", 0),  zx_Json_GetString(res_root, "message", "")); 
	}
	else
	{
		dzlog_error("err code=%d message=%s", err.code, err.message);
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





zx_Error zx_active_curl_for_thread(void)
{
	zx_Error err;
	//cJSON *res_root;	
	char* url = "www.baidu.com";//zx_String_Concat2(AS_HOST, "/ping");
	zx_Client client;
	//zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	if (url)
	{
   		//err = zx_Client_Call(&gclient, &res_root, url, "GET");
		err = zx_Client_CallNoRet(&client, url, "GET");
		dzlog_info("err.code=%d  msg=%s",err.code , err.message);
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
	char *s_json_param;
	cJSON *res_root, *param_root;// *param_list, *param_heard;
	zx_Client client;
	zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	char* url = zx_String_Concat2(AS_HOST, "/v1/hub/check_hub");
	err.code = -1;
	if (url && account_id && station_sn )
	{
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
				dzlog_info("err.code=%d  msg=%s",err.code , err.message);
				if (err.code == AS_HTTP_200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		dzlog_info("code=%d msg=%s",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
				}

            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(url);
	}
	zx_Client_Cleanup(&client);
	curl_global_cleanup();
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

#if 0
int main_as(int argc, char **argv)
{
	//zx_Buffer_formatInit();
	zx_init_as_interface();
	//for (int i=0;i<500;i++)
    //zx_hub_bind_devs("12345", "23456", 0);
	zx_active_curl_for_thread();
	P2P_BASE_INFO p2p_info;
	zx_hub_get_p2p_params("T8001G031748001B", &p2p_info);
	zx_app_bind_hub("T8001G031748001B", "7269dbbe202527ab65e4da4e28ddb44c59da08d4", " ", " ");
	//char* base64 = zx_String_Encode(COMPANY_NAME);
	//dzlog_warn("%s ",base64);
	//free(base64);
    return 0;
}
#endif


