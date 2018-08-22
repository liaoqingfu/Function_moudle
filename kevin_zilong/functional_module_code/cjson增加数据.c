#ifndef bool
#define bool	char
#endif

#ifndef true
#define true	1
#endif

#ifndef false
#define false	0
#endif


	param_root = cJSON_CreateObject();	//创建json

	if(!param_root)
	{
		dzlog_error("cJSON CreateObject is error");
		goto exit;
	}

	cJSON_AddStringToObject(param_root,"account",account);		//增加字符串
	cJSON_AddStringToObject(param_root,"file_name",file_name);
	cJSON_AddNumberToObject(param_root,"file_size",file_size);	//增加数字
	cJSON_AddStringToObject(param_root,"device_sn",device_sn);
	cJSON_AddNumberToObject(param_root,"type",type);
	
	if(disable_ssl)
		cJSON_AddTrueToObject(param_root,"disable_ssl");  //disable_ssl	//增加bool类型  true
	else
		cJSON_AddFalseToObject(param_root,"disable_ssl");				//增加bool类型  false

	s_json_param = cJSON_PrintUnformatted(param_root);