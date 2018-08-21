/*
 ============================================================================
 Name        : http.c
 Author      : Oceanwing.com
 Copyright   : 2017(c) Shenzhen Oceanwing Smart Innovations Co., Ltd.
 Create      : lucien.liu
 date        : 2017-10-30 
 Description :
 ============================================================================
 */

#include "http.h"
//#include "cjson.h"
#include "base.h"
#include <curl/curl.h>
#include "PPCS/PPCS_Error.h"

#if defined(_WIN32)
#pragma comment(lib, "curllib.lib")
#endif

/*============================================================================*/
/* type zx_Mutex */

#if defined(_WIN32)

void zx_Mutex_Init(zx_Mutex* self)
{
	InitializeCriticalSection(self);
}

void zx_Mutex_Cleanup(zx_Mutex* self)
{
	DeleteCriticalSection(self);
}

void zx_Mutex_Lock(zx_Mutex* self)
{
	EnterCriticalSection(self);
}

void zx_Mutex_Unlock(zx_Mutex* self)
{
	LeaveCriticalSection(self);
}

#else

void zx_Mutex_Init(zx_Mutex *self) 
{
    pthread_mutex_init(self, NULL);
}

void zx_Mutex_Cleanup(zx_Mutex *self) 
{
    pthread_mutex_destroy(self);
}

void zx_Mutex_Lock(zx_Mutex *self) 
{
    pthread_mutex_lock(self);
}

void zx_Mutex_Unlock(zx_Mutex *self) 
{
    pthread_mutex_unlock(self);
}

#endif

/*============================================================================*/
/* Global */

void zx_Buffer_formatInit();

void zx_Global_Init(long flags) 
{
    //zx_Buffer_formatInit();
    curl_global_init(CURL_GLOBAL_ALL);
}

void zx_Global_Cleanup() 
{
    curl_global_cleanup();
}

/*============================================================================*/
/* func zx_call */

static const char g_statusCodeError[] = "http status code is not OK";

zx_Error zx_callex(CURL *curl, zx_Buffer *resp, zx_Json **ret, zx_Bool simpleError,
                         zx_Buffer *resph) 
{
    zx_Error err;
    CURLcode curlCode;
    long httpCode;
    zx_Json *root;

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, zx_Buffer_Fwrite);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, resp);
    if (resph != NULL) 
	{
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, zx_Buffer_Fwrite);
        curl_easy_setopt(curl, CURLOPT_WRITEHEADER, resph);
    }
    curlCode = curl_easy_perform(curl);
    if (curlCode == 0) 
	{
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
        if (zx_Buffer_Len(resp) != 0) 
		{
            root = cJSON_Parse(zx_Buffer_CStr(resp));
        } 
		else 
		{
            root = NULL;
        }
        *ret = root;
        err.code = (int) httpCode;
        if (httpCode / 100 != 2) 
		{
            if (simpleError) 
			{
                err.message = g_statusCodeError;
            } 
			else 
			{
                err.message = zx_Json_GetString(root, "error", g_statusCodeError);
            }
        } 
		else 
		{
            err.message = "OK";
        }
    } 
	else 
	{
        *ret = NULL;
        err.code = curlCode;
        err.message = "curl_easy_perform error";
    }

    return err;
}

/*============================================================================*/
/* type zx_Json */

const char *zx_Json_GetString(zx_Json *self, const char *key, const char *defval) 
{
    zx_Json *sub;
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

const char *zx_Json_GetStringAt(zx_Json *self, int n, const char *defval) 
{
    zx_Json *sub;
    if (self == NULL) 
	{
        return defval;
    }
    sub = cJSON_GetArrayItem(self, n);
    if (sub != NULL && sub->type == cJSON_String) 
	{
        return sub->valuestring;
    } 
	else 
	{
        return defval;
    }
}

int zx_Json_GetInt(zx_Json *self, const char *key, int defval) 
{
    zx_Json *sub;
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

zx_Int64 zx_Json_GetInt64(zx_Json *self, const char *key, zx_Int64 defval) 
{
    zx_Json *sub;
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

zx_Uint64 zx_Json_GetUInt64(zx_Json *self, const char *key, zx_Uint64 defval) 
{
    zx_Json *sub;
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

int zx_Json_GetBoolean(zx_Json *self, const char *key, int defval) 
{
    zx_Json *sub;
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

zx_Json *zx_Json_GetObjectItem(zx_Json *self, const char *key, zx_Json *defval) 
{
    zx_Json *sub;
    if (self == NULL) 
	{
        return defval;
    }
    sub = cJSON_GetObjectItem(self, key);
    if (sub != NULL) 
	{
        return sub;
    } 
	else 
	{
        return defval;
    }
} // zx_Json_GetObjectItem

int zx_Json_GetArraySize(zx_Json *self, const char *key, zx_Int64 defval) 
{
    zx_Json *sub;
    int size;
    if (self == NULL) 
	{
        return defval;
    }
    sub = cJSON_GetObjectItem(self, key);
    if (sub != NULL) 
	{
        size = cJSON_GetArraySize(sub);
    } 
	else 
	{
        size = defval;
    }
    return size;
} // zx_Json_GetArraySize

zx_Json *zx_Json_GetArrayItem(zx_Json *self, int n, zx_Json *defval) 
{
    zx_Json *sub;
    if (self == NULL) 
	{
        return defval;
    }
    sub = cJSON_GetArrayItem(self, n);
    if (sub != NULL) 
	{
        return sub;
    } else {
        return defval;
    }
} // zx_Json_GetArrayItem

void zx_Json_Destroy(zx_Json *self) 
{
    cJSON_Delete(self);
} // zx_Json_Destroy

zx_Uint32 zx_Json_GetUInt32(zx_Json *self, const char *key, zx_Uint32 defval) 
{
    zx_Json *sub;
    if (self == NULL) 
	{
        return defval;
    }
    sub = cJSON_GetObjectItem(self, key);
    if (sub != NULL && sub->type == cJSON_Number) 
	{
        return (zx_Uint32) sub->valueint;
    } 
	else 
	{
        return defval;
    }
}

/*============================================================================*/
/* type zx_Client */

zx_Auth zx_NoAuth = 
{
        NULL,
        NULL
};

void zx_Client_InitEx(zx_Client *self, zx_Auth auth, size_t bufSize) 
{
    self->curl = curl_easy_init();
    self->root = NULL;
    self->auth = auth;

    zx_Buffer_Init(&self->b, bufSize);
    zx_Buffer_Init(&self->respHeader, bufSize);

    self->boundNic = NULL;

    self->lowSpeedLimit = 0;
    self->lowSpeedTime = 0;
}

void zx_Client_InitNoAuth(zx_Client *self, size_t bufSize) 
{
    zx_Client_InitEx(self, zx_NoAuth, bufSize);
}

void zx_Client_Cleanup(zx_Client *self) 
{
    if (self->auth.itbl != NULL) 
	{
        self->auth.itbl->Release(self->auth.self);
        self->auth.itbl = NULL;
    }
    if (self->curl != NULL) 
	{
        curl_easy_cleanup((CURL *) self->curl);
        self->curl = NULL;
    }
    if (self->root != NULL) 
	{
        cJSON_Delete(self->root);
        self->root = NULL;
    }
    zx_Buffer_Cleanup(&self->b);
    zx_Buffer_Cleanup(&self->respHeader);
}

void zx_Client_BindNic(zx_Client *self, const char *nic) 
{
    self->boundNic = nic;
} // zx_Client_BindNic

void zx_Client_SetLowSpeedLimit(zx_Client *self, long lowSpeedLimit, long lowSpeedTime) 
{
    self->lowSpeedLimit = lowSpeedLimit;
    self->lowSpeedTime = lowSpeedTime;
} // zx_Client_SetLowSpeedLimit

CURL *zx_Client_reset(zx_Client *self) 
{
    CURL *curl = (CURL *) self->curl;

    curl_easy_reset(curl);
    zx_Buffer_Reset(&self->b);
    zx_Buffer_Reset(&self->respHeader);
    if (self->root != NULL) 
	{
        cJSON_Delete(self->root);
        self->root = NULL;
    }

    // Set this option to allow multi-threaded application to get signals, etc
    // Setting CURLOPT_NOSIGNAL to 1 makes libcurl NOT ask the system to ignore SIGPIPE signals
    // See also https://curl.haxx.se/libcurl/c/CURLOPT_NOSIGNAL.html

    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

    return curl;
}

static CURL *zx_Client_initcall(zx_Client *self, const char *url) 
{
    CURL *curl = zx_Client_reset(self);

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_URL, url);

    return curl;
}

static zx_Error zx_Client_callWithBody(
        zx_Client *self, zx_Json **ret, const char *url,
        const char *body, zx_Int64 bodyLen, const char *mimeType, const char *method) 
{
    int retCode = 0;
    zx_Error err;
    const char *ctxType;
	const char *crxType;
    //char ctxLength[64];
	char c_random[32];
    zx_Header *headers = NULL;
    CURL *curl = (CURL *) self->curl;

    // Bind the NIC for sending packets.
    if (self->boundNic != NULL) 
	{
        retCode = curl_easy_setopt(curl, CURLOPT_INTERFACE, self->boundNic);
        if (retCode == CURLE_INTERFACE_FAILED) 
		{
            err.code = 9994;
            err.message = "Can not bind the given NIC";
            return err;
        }
    }

	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
    //curl_easy_setopt(curl, CURLOPT_POST, 1);

    if (mimeType == NULL) 
	{
        ctxType = "Content-Type: application/octet-stream";
		crxType = "Accept: application/octet-stream";
    } 
	else 
	{
        ctxType = zx_String_Concat2("Content-Type: ", mimeType);
		crxType = zx_String_Concat2("Accept: ", mimeType);
    }
	srand((unsigned)time(NULL));

    //zx_snprintf(ctxLength, 64, "Content-Length: %lld", bodyLen);
	zx_snprintf(c_random, 22, "X-Random-Number: %04d", rand());
    //headers = curl_slist_append(NULL, ctxLength);
	headers = curl_slist_append(NULL, c_random);
    headers = curl_slist_append(headers, ctxType);
	headers = curl_slist_append(headers, crxType);
    //headers = curl_slist_append(headers, "Expect:");

    if (self->auth.itbl != NULL) 
	{
        if (body == NULL) 
		{
            err = self->auth.itbl->Auth(self->auth.self, &headers, url, NULL, 0);
        } 
		else 
		{
            err = self->auth.itbl->Auth(self->auth.self, &headers, url, body, (size_t) bodyLen);
        }

        if (err.code != AS_HTTP_200) 
		{
            return err;
        }
    }
	
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    err = zx_callex(curl, &self->b, &self->root, zx_False, &self->respHeader);

    curl_slist_free_all(headers);
    if (mimeType != NULL) 
	{
        free((void *) ctxType);
    }

    *ret = self->root;
    return err;
}

zx_Error zx_Client_CallWithBinary(zx_Client *self, zx_Json **ret, const char *url,
                                                zx_Reader body, zx_Int64 bodyLen, const char *mimeType, 
                                                const char *method) 
{
	zx_Error err;
	if (self->curl)
	{
    	CURL *curl = zx_Client_initcall(self, url);

    	curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, bodyLen);
    	curl_easy_setopt(curl, CURLOPT_READFUNCTION, body.Read);
    	curl_easy_setopt(curl, CURLOPT_READDATA, body.self);

    	return zx_Client_callWithBody(self, ret, url, NULL, bodyLen, mimeType, method);
	}
	else
	{
		err.code = ERROR_NULL_POINT;		
	}
	return err;
}

zx_Error zx_Client_CallWithBuffer(zx_Client *self, zx_Json **ret, const char *url,
                                                const char *body, size_t bodyLen, const char *mimeType, const char *method) 
{
	zx_Error err;
	if (self->curl)
	{
    	CURL *curl = zx_Client_initcall(self, url);

    	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, bodyLen);
    	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
		return zx_Client_callWithBody(self, ret, url, body, bodyLen, mimeType, method);
	}
	else
	{
		err.code = ERROR_NULL_POINT;		
	}
	return err;
}

zx_Error zx_Client_CallWithBuffer2(zx_Client *self, zx_Json **ret, const char *url,
                                                 const char *body, size_t bodyLen, const char *mimeType, const char *method) 
{
	zx_Error err;
	if (self->curl)
	{
    	CURL *curl = zx_Client_initcall(self, url);

    	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, bodyLen);
    	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);

    	return zx_Client_callWithBody(self, ret, url, NULL, bodyLen, mimeType, method);
	}
	else
	{
		err.code = ERROR_NULL_POINT;		
	}
	return err;
}

zx_Error zx_Client_Call(zx_Client *self, zx_Json **ret, const char *url, const char *method) 
{
    zx_Error err;
    zx_Header *headers = NULL;
    CURL *curl = zx_Client_initcall(self, url);

    if (self->auth.itbl != NULL) 
	{
        err = self->auth.itbl->Auth(self->auth.self, &headers, url, NULL, 0);
        if (err.code != AS_HTTP_200) 
		{
            return err;
        }
    }

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);

    err = zx_callex(curl, &self->b, &self->root, zx_False, &self->respHeader);
    curl_slist_free_all(headers);
    *ret = self->root;
    return err;
}

zx_Error zx_Client_CallNoRet(zx_Client *self, const char *url, const char *method) 
{
    zx_Error err;
    zx_Header *headers = NULL;
    CURL *curl = zx_Client_initcall(self, url);

    if (self->auth.itbl != NULL) 
	{
        err = self->auth.itbl->Auth(self->auth.self, &headers, url, NULL, 0);
        if (err.code != AS_HTTP_200) 
		{
            return err;
        }
    }

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);

    err = zx_callex(curl, &self->b, &self->root, zx_False, &self->respHeader);
    curl_slist_free_all(headers);
    return err;
}

/*
int create_js(void)
{
    cJSON *root, *js_body, *js_list;
	zx_Client client;
	zx_Global_Init(-1);
	zx_Client_InitEx(&client, zx_NoAuth, 1024);
	char* url = zx_String_Concat2("http://security-moat-qa.goroav.com", "/v1/hub/bind_devs");///hub/bind_devs
	//CURL *curl = zx_Client_initcall(client, url);
    root = cJSON_CreateObject();
    //cJSON_AddItemToObject(root,"body", js_body = cJSON_CreateArray());
    //cJSON_AddItemToArray(js_body, js_list = cJSON_CreateObject());
    cJSON_AddNumberToObject(root,"device_channel",0);
	cJSON_AddStringToObject(root,"device_model","123");
	cJSON_AddStringToObject(root,"device_name","test");
	cJSON_AddNumberToObject(root,"device_type",0);
	cJSON_AddStringToObject(root,"station_sn","12345567788");

    {
        char *s = cJSON_PrintUnformatted(root);
        if(s)
		{
			
			zx_Error error;
			cJSON* res_root;
			char *result;
			
			error = zx_Client_CallWithBuffer(&client, &res_root, url, s, strlen(s), "application/json", "POST");
			printf("call result=%d\n", error);
			free(url);
			result = cJSON_PrintUnformatted(res_root);
            printf(" %s \n",s);
			printf(" %s \n",result);
            free(s);
        }
    }
    if(root)
        cJSON_Delete(root);

	zx_Client_Cleanup(&client);
	curl_global_cleanup();
    return 0;
}

*/




