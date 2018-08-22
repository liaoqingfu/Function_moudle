int zx_cjson_shuzu()
{
	cJSON  *pJsonArry = NULL,*pJsonsub = NULL;
	cJSON *param_root = NULL;
	char *s_json_param = NULL;
	
	
	param_root = cJSON_CreateObject();   //创建json

	if(!param_root)
	{
		dzlog_error("cJSON CreateObject is error");
	}
	pJsonArry=cJSON_CreateArray();   /*创建数组*/
	
	cJSON_AddStringToObject(param_root,"account","1212231231231");

	cJSON_AddItemToArray(pJsonArry,cJSON_CreateString("brady"));  //数组内增加字符
	
	cJSON_AddItemToArray(pJsonArry,cJSON_CreateNumber(3));  //数组内增加数字
	cJSON_AddItemToArray(pJsonArry,cJSON_CreateNumber(4));

	pJsonsub = cJSON_CreateObject();   //创建json
	cJSON_AddNumberToObject(pJsonsub,"num",19);  //json内增加数字
	cJSON_AddStringToObject(pJsonsub,"gong","ocean");  //json内增加字符
	cJSON_AddStringToObject(pJsonsub,"si","wing");  //json内增加字符
	cJSON_AddItemToArray(pJsonArry,pJsonsub);   //将json加入数组

	cJSON_AddItemToObject(param_root,"monitor_ids",pJsonArry);  //将数组加入json
	
	cJSON_AddStringToObject(param_root,"station_sn","fffff");  //json内增加字符
	s_json_param = cJSON_PrintUnformatted(param_root);			//转换成char *

	dzlog_info("s_json_param = %s",s_json_param);
	return 0;

}



结果：

s_json_param = {"account":"1212231231231","monitor_ids":["brady",3,4,{"num":19,"gong":"ocean","si":"wing"}],"station_sn":"fffff"}


const char *zx_Json_GetString(struct cJSON *self, const char *key, const char *defval) 
{
    struct cJSON *sub = NULL;
    if (self == NULL) 
	{
        return defval;
    }
    sub = cJSON_GetObjectItem(self, key);
    if (sub != NULL && sub->type == cJSON_String) 
	{
        return sub->valuestring;
    } 
	else 
	{
        return defval;
    }
}

int zx_Json_GetInt(struct cJSON *self, const char *key, int defval) 
{
    struct cJSON *sub;
    if (self == NULL) 
	{
        return defval;
    }
    sub = cJSON_GetObjectItem(self, key);
    if (sub != NULL && sub->type == cJSON_Number) 
	{
        return (int) sub->valuedouble;
    } 
	else 
	{
        return defval;
    }
}


int zx_mgw_commun_operation_version_data(char *s_json_param)
{
	cJSON *json_data = NULL,*Array_json = NULL;
	char *s_json_param = NULL;

	if(!recv_package_data)
	{
		dzlog_error("The function parameter  is NULL");
		return -1;
	}

	dzlog_info("******mgw commun send version start******");

	json_data = cJSON_Parse(s_json_param); //字符串解析成json结构体

	if(!json_data)
	{
		dzlog_error("******cJSON Parse is error******");
		return -1;
	}
	
	char *account = zx_Json_GetString(json_data,"account","")	//拿到string
	
	json_data = cJSON_GetObjectItem(json_data,"monitor_ids");	//拿到数组
	
	Array_json = cJSON_GetArrayItem(json_data,0);			//brady
	s_json_param = cJSON_PrintUnformatted(param_root);		//转换成char *     s_json_param = brady
	
	Array_json = cJSON_GetArrayItem(json_data,1);			//3
	int num = cJSON_GetArraySize(Array_json);				//num = 3
	
	Array_json = cJSON_GetArrayItem(json_data,2);			//4
	int num = cJSON_GetArraySize(Array_json);				//num = 4
	
	Array_json = cJSON_GetArrayItem(json_data,3);

	
	return 0;
}