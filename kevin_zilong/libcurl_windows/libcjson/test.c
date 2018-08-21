#include "curl/curl.h"
#include "curl/easy.h"



//#pragma comment(lib,"curl_as_interface/curl_cjson_https.lib")

#if 0
int main()
{
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, true);
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.baidu.com");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }
    system("pause");
    return 0;
}
#endif

#include <stdio.h>  
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "http_buf.h"
//#include "cjson.h"
//#include "base.h"
//#include "http.h"
//#include "curl/curl.h"

#define AS_HOST  "https://security-app.eufylife.com"
#define RESP_BODY_SIZE  2048






#if 1
zx_Json *zx_rom_upgrade_request (const char *current_version_name,const char *device_type,unsigned long rom_version,const char *sn,zx_Client client)
{
	zx_Error err;
	zx_Json *res_root = NULL, *param_root = NULL;
	char *url = NULL;
	char *s_result = NULL;
	char *s_json_param = NULL;
	int retry_count = 0;
	
	if(!current_version_name || !device_type || !sn)
	{
		printf("parameter is NULL,Please check the parameters");
		return NULL;
	}
	
	//url = zx_String_Concat2(AS_ROM_UPGRADE, "/v1/security/upgrade/fetch-rom-version");
	url = zx_String_Concat2(AS_HOST, "/v1/hub/ota/get_rom_version");  //2018-5-7 修改为域名https接口
	
	if( !url )
	{
		printf("******String Concat2 error******\n");
		goto exit;
	}
	printf("url = %s\n",url);
#if 1
	param_root = cJSON_CreateObject();

	if(!param_root)
	{
		printf("cJSON CreateObject is error");
		goto exit;
	}

	cJSON_AddStringToObject(param_root,"current_version_name",current_version_name);
	cJSON_AddStringToObject(param_root,"device_type",device_type);
	cJSON_AddNumberToObject(param_root,"rom_version",rom_version);
	cJSON_AddStringToObject(param_root,"sn",sn);
	
	s_json_param = cJSON_PrintUnformatted(param_root);

	if(!s_json_param)
	{
		printf("cJSON_PrintUnformatted is error\n");
		goto exit;
	}

	printf("s_json_param = %s\n",s_json_param);
	
RETRY_AGAIN:
	err = zx_Client_CallWithBuffer(&client, &res_root, url, s_json_param, strlen(s_json_param), "application/json", "POST");
	
	if(err.code == 200) 
	{
		s_result = cJSON_PrintUnformatted(res_root);

		if(!s_result)
		{
			printf("cJSON PrintUnformatted is error\n");
			goto exit;
		}
		printf("cJSON = %s\n",s_result);
		printf("code=%d  msg=%s\n",zx_Json_GetInt(res_root, "code", 0),  zx_Json_GetString(res_root, "msg", "")); 
	}
	else
	{
		printf("err code=%d message=%s\n", err.code, err.message);

		//无法连接至主机或代理服务器或者域名解析失败 重试
		if( CURLE_COULDNT_CONNECT == err.code || CURLE_COULDNT_RESOLVE_HOST == err.code )		
		{
			++retry_count;
			if(retry_count <= 3)		//重试三次
			{
				Sleep(1);		//延时1秒后重试
				printf("rom upgrade request retry %d count",retry_count);
				goto RETRY_AGAIN;
			
			}else{
				printf("rom upgrade request %d count,please check network. err code = %d\n",retry_count,err.code);
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
#if 1
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
#endif
#endif
	return res_root;
}
#endif
#if 1
zx_Error DeviceInfoBase(
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
	char *s_json_param;
	char *s_result;
	char* http_url;
	cJSON *res_root, *param_root;

	zx_Auth zx_NoAuth = 
	{
        NULL,
        NULL
	};

	zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);

	http_url = zx_String_Concat2(AS_HOST, "/v1/factory/upload_devs_info");
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
				printf("%s\n",s_json_param);
				err = zx_Client_CallWithBuffer(&client, &res_root, http_url, s_json_param, strlen(s_json_param), "application/json", "POST");
				printf("err.code=%d  msg=%s\n",err.code , err.message);
				if (err.code == 200) 
				{
					s_result = cJSON_PrintUnformatted(res_root);
					printf("s_result = %s\n",s_result);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		printf("code=%d msg=%s\n",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
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
#endif


#if 1
zx_Error BaseStationBase(
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
	char *s_json_param;
	char *s_result;
	char* http_url;
	cJSON *res_root, *param_root;

	zx_Auth NoAuth = 
	{
        NULL,
        NULL
	};

	zx_Global_Init(-1);
	zx_Client_InitEx(&client, NoAuth, RESP_BODY_SIZE);

	http_url = zx_String_Concat2(AS_HOST, "/v1/factory/upload_hub_info");
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
				printf("s_json_param  = %s\n",s_json_param);
				err = zx_Client_CallWithBuffer(&client, &res_root, http_url, s_json_param, strlen(s_json_param), "application/json", "POST");
				printf("err.code=%d  msg=%s\n",err.code , err.message);
				if (err.code == 200) 
				{
					s_result = cJSON_PrintUnformatted(res_root);
					printf("s_result = %s\n",s_result);
					err.code = zx_Json_GetInt(res_root, "code", 0);
            		printf("code=%d msg=%s\n",zx_Json_GetInt(res_root, "code", 0), 
                                              zx_Json_GetString(res_root, "msg", 0));
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
#endif

#if 1
int ota_packet()
{
#if 1
	zx_Json *rom_json = NULL;
	zx_Client client;
	char *s_result = NULL;
	//zx_Auth auth;

		zx_Auth NoAuth = 
	{
        NULL,
        NULL
	};

	zx_Global_Init(-1);
	zx_Client_InitEx(&client, NoAuth, RESP_BODY_SIZE);

	printf("zx client initex ok\n");
	rom_json = zx_rom_upgrade_request("string","ota_pakage",0,"T8001G0317480032",client);
#if 1
	if(!rom_json || zx_Json_GetInt(rom_json, "code", 0) != 0)
	{
		printf(" JSON is NULL\n");
		return -1;
	}
#endif	
	s_result = cJSON_PrintUnformatted(rom_json);
	
	if(!s_result )
	{
		printf("JSON is NULL\n");
		return -1;
	}
	printf("data is===%s\n",s_result);
	
	if(s_result) {free(s_result); s_result = NULL;}
#endif



	return 0;
}
#endif

int main()
{
	//ota_packet();
	DeviceInfoBase(0,"string","string","string",0,0,"string","string","string","string","string","string",0,"string","string","string");

	BaseStationBase("string","string","string","string","string","string","string","string","string","string","string","string");

	return 0;
}