
extern cJSON *cJSON_Parse(const char *value);			//将字符串转为cjson

extern char  *cJSON_PrintUnformatted(cJSON *item);		//将cjson转为字符串
/* Delete a cJSON entity and all subentities. */

extern cJSON *cJSON_CreateObject();						//创建一个cjson的结构

extern void   cJSON_Delete(cJSON *c);					//清理生成的空间


extern cJSON *cJSON_GetObjectItem(cJSON *object,const char *string);		//获取json中的json
extern int	  cJSON_GetArraySize(cJSON *array);								//获取json中的数量，以键值对为数，这样的算一个："timestamp":1523164269
extern cJSON *cJSON_GetArrayItem(cJSON *array,int item);					//获取json中的 中括号中的第几个json [{},{},{}] ,里面就有三个json




typedef long long zx_Int64;
typedef unsigned long long zx_Uint64;
typedef unsigned int zx_Uint32;
typedef unsigned short zx_Uint16;


int zx_Json_GetInt(cJSON *self, const char *key, int defval) 		//获取json中的数字（int）
{
    cJSON *sub;
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

zx_Int64 zx_Json_GetInt64(cJSON *self, const char *key, zx_Int64 defval) //获取json中的 64位数字
{
    cJSON *sub;
    if (self == NULL) 
	{
        return defval;
    }
    sub = cJSON_GetObjectItem(self, key);
    if (sub != NULL && sub->type == cJSON_Number) 
	{
        return (zx_Int64) sub->valuedouble;
    } else {
        return defval;
    }
}

zx_Uint64 zx_Json_GetUInt64(cJSON *self, const char *key, zx_Uint64 defval) 
{
    cJSON *sub;
    if (self == NULL) 
	{
        return defval;
    }
    sub = cJSON_GetObjectItem(self, key);
    if (sub != NULL && sub->type == cJSON_Number) 
	{
        return (zx_Uint64) sub->valuedouble;
    } 
	else 
	{
        return defval;
    }
}

int zx_Json_GetBoolean(cJSON *self, const char *key, int defval) 
{
    cJSON *sub;
    if (self == NULL) 
	{
        return defval;
    } // if
    sub = cJSON_GetObjectItem(self, key);
    if (sub != NULL) 
	{
        if (sub->type == cJSON_False) 
		{
            return 0;
        } 
		else if (sub->type == cJSON_True) 
		{
            return 1;
        } // if
    } // if
    return defval;
} // zx_Json_GetBoolean

char *zx_Json_GetString(cJSON *self, const char *key, const char *defval) 	//获取json中的 字符串
{
    cJSON *sub;
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