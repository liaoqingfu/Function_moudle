
#include "zx_upload_info.h"




zx_Error zx_device_Info_base(
	int device_channel,
	char *device_model,
	char *device_name,
	char *device_sn,
	int device_type,
	int event_num,
	char *main_hw_version,
	char *main_sw_version,
	char *schedule,
	char *schedulex,
	char *sec_hw_version,
	char *sec_sw_version,
	int sector_id,
	char *station_sn,
	char *sub1g_mac,
	char *wifi_mac
)
{
	zx_Error err;
	zx_Client client;
	char *s_json_param = NULL;
	//char *s_result = NULL;
	char* http_url = NULL;
	cJSON *res_root = NULL, *param_root = NULL;
	int retry_count = 0;

	zx_Auth zx_NoAuth = 
	{
        NULL,
        NULL
	};

	zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);

	http_url = zx_String_Concat2(AS_HOST, ZX_UPLOAD_DEVS_INFO_URL);
	err.code = -1;
	if (http_url)
	{
    	param_root = cJSON_CreateObject();
		if (param_root)
		{
			cJSON_AddNumberToObject(param_root,"device_channel",device_channel);
			cJSON_AddStringToObject(param_root,"device_model",device_model);
			cJSON_AddStringToObject(param_root,"device_name",device_name);
			cJSON_AddStringToObject(param_root,"device_sn",device_sn);
			cJSON_AddNumberToObject(param_root,"device_type",device_type);
			cJSON_AddNumberToObject(param_root,"event_num",event_num);
			cJSON_AddStringToObject(param_root,"main_hw_version",main_hw_version);
			cJSON_AddStringToObject(param_root,"main_sw_version",main_sw_version);
			cJSON_AddStringToObject(param_root,"schedule",schedule);
			cJSON_AddStringToObject(param_root,"schedulex",schedulex);
			cJSON_AddStringToObject(param_root,"sec_hw_version",sec_hw_version);
			cJSON_AddStringToObject(param_root,"sec_sw_version",sec_sw_version);
			cJSON_AddNumberToObject(param_root,"sector_id",sector_id);
			cJSON_AddStringToObject(param_root,"station_sn",station_sn);
			cJSON_AddStringToObject(param_root,"sub1g_mac",sub1g_mac);
			cJSON_AddStringToObject(param_root,"wifi_mac",wifi_mac);

       		s_json_param = cJSON_PrintUnformatted(param_root);
        	if(s_json_param)
			{
				//printf("%s\n",s_json_param);
		RETRY_AGAIN:
				err = zx_Client_CallWithBuffer(&client, &res_root, http_url, s_json_param, strlen(s_json_param), "application/json", "POST");
				
				if (err.code == 200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					//printf("s_result = %s\n",s_result);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		printf("code=%d msg=%s\n",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
				}else{
					printf("err.code=%d  msg=%s\n",err.code , err.message);

					if( CURLE_COULDNT_CONNECT == err.code || CURLE_COULDNT_RESOLVE_HOST == err.code )		
					{
						++retry_count;
						if(retry_count <= 3)	//重试三次
						{
							Sleep(1000);		//延时1秒后重试
							printf("device Info base request retry %d count\n",retry_count);
							goto RETRY_AGAIN;
				
						}else{
							printf("device Info base request %d count,please check network. err code = %d\n",retry_count,err.code);
						}
					}
				}

            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(http_url);
	}
	zx_Client_Cleanup(&client);
	curl_global_cleanup();
	return err;
}


zx_Error zx_base_station_base(
	char *ip_addr,
	char *main_hw_version,
	char *main_sw_version,
	char *sec_hw_version,
	char *sec_sw_version,
	char *station_model,
	char *station_name,
	char *station_sn,
	char *sub1g_mac,
	char *time_zone,
	char *wifi_mac,
	char *wifi_ssid 
)
{
	zx_Error err;
	zx_Client client;
	char *s_json_param = NULL;
	//char *s_result = NULL;
	char* http_url = NULL;
	cJSON *res_root = NULL, *param_root = NULL;
	int retry_count = 0;

	zx_Auth NoAuth = 
	{
        NULL,
        NULL
	};

	zx_Global_Init(-1);

	zx_Client_InitEx(&client, NoAuth, RESP_BODY_SIZE);

	http_url = zx_String_Concat2(AS_HOST, ZX_UPLOAD_HUB_INFO_URL);
	err.code = -1;
	if (http_url)
	{
    	param_root = cJSON_CreateObject();
		if (param_root)
		{
			cJSON_AddStringToObject(param_root,"ip_addr",ip_addr);
			cJSON_AddStringToObject(param_root,"main_hw_version",main_hw_version);
			cJSON_AddStringToObject(param_root,"main_sw_version",main_sw_version);
			cJSON_AddStringToObject(param_root,"sec_hw_version",sec_hw_version);
			cJSON_AddStringToObject(param_root,"sec_sw_version",sec_sw_version);
			cJSON_AddStringToObject(param_root,"station_model",station_model);
			cJSON_AddStringToObject(param_root,"station_name",station_name);
			cJSON_AddStringToObject(param_root,"station_sn",station_sn);
			cJSON_AddStringToObject(param_root,"sub1g_mac",sub1g_mac);
			cJSON_AddStringToObject(param_root,"time_zone",time_zone);
			cJSON_AddStringToObject(param_root,"wifi_mac",wifi_mac);
			cJSON_AddStringToObject(param_root,"wifi_ssid",wifi_ssid);
			

       		s_json_param = cJSON_PrintUnformatted(param_root);
        	if(s_json_param)
			{
				//printf("s_json_param  = %s\n",s_json_param);
	RETRY_AGAIN:
				err = zx_Client_CallWithBuffer(&client, &res_root, http_url, s_json_param, strlen(s_json_param), "application/json", "POST");
				if (err.code == 200) 
				{
					//s_result = cJSON_PrintUnformatted(res_root);
					//printf("s_result = %s\n",s_result);
					//err.code = zx_Json_GetInt(res_root, "code", 0);
            		printf("code=%d msg=%s\n",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
					
				}else{
					printf("err.code=%d  msg=%s\n",err.code , err.message);

						//无法连接至主机或代理服务器或者域名解析失败 重试
					if( CURLE_COULDNT_CONNECT == err.code || CURLE_COULDNT_RESOLVE_HOST == err.code )		
					{
						++retry_count;
						if(retry_count <= 3)	//重试三次
						{
							Sleep(1000);		//延时1秒后重试
							printf("base station base request retry %d count\n",retry_count);
							goto RETRY_AGAIN;
				
						}else{
							printf("base station base request %d count,please check network. err code = %d\n",retry_count,err.code);
						}
					}
				}

            	free(s_json_param);
        	}
        	cJSON_Delete(param_root);
    	}
		free(http_url);
	}
	zx_Client_Cleanup(&client);
	curl_global_cleanup();
	return err;
}
