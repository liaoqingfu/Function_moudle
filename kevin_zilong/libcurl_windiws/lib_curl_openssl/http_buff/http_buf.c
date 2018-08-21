
#include "http_buf.h"


void zx_Client_InitEx(zx_Client *self, zx_Auth auth, size_t bufSize);
CURL *zx_Client_reset(zx_Client *self);
void zx_Buffer_Cleanup(zx_Buffer* self);
const char* zx_Buffer_CStr(zx_Buffer* self);
zx_Error zx_Client_CallWithBuffer(zx_Client* self, zx_Json** ret, const char* url,const char* body, size_t bodyLen, const char* mimeType, const char *method);
const char *zx_Json_GetString(zx_Json *self, const char *key, const char *defval);
zx_Uint32 zx_Json_GetUInt32(zx_Json *self, const char *key, zx_Uint32 defval);
zx_Uint64 zx_Json_GetUInt64(zx_Json *self, const char *key, zx_Uint64 defval);
zx_Int64 zx_Json_GetInt64(zx_Json *self, const char *key, zx_Int64 defval);
int zx_Json_GetArraySize(zx_Json *self, const char *key, zx_Int64 defval);
int zx_Json_GetInt(zx_Json *self, const char *key, int defval);
void zx_Global_Init(long flags);
void zx_Client_Cleanup(zx_Client *self);
void zx_Buffer_Init(zx_Buffer* self, size_t initSize);
size_t zx_Buffer_Len(zx_Buffer* self);
static void zx_Buffer_expand(zx_Buffer* self, size_t expandSize);
size_t zx_Buffer_Write(zx_Buffer* self, const void* buf, size_t n);
size_t zx_Buffer_Fwrite(const void *buf, size_t size, size_t nmemb, void *self);
zx_Writer zx_BufWriter(zx_Buffer* self);
void zx_Buffer_Reset(zx_Buffer* self);

static const char g_statusCodeError[] = "http status code is not OK";
#define snprintf _snprintf

CURL *zx_Client_reset(zx_Client *self) 
{
    CURL *curl = (CURL *) self->curl;

    curl_easy_reset(curl);
	
#if 1
    zx_Buffer_Reset(&self->b);
    zx_Buffer_Reset(&self->respHeader);
    if (self->root != NULL) 
	{
        cJSON_Delete(self->root);
        self->root = NULL;
    }
#endif

    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

    return curl;
}

void zx_Client_InitEx(zx_Client *self, zx_Auth auth, size_t bufSize) 
{
	printf("helo");
    self->curl = curl_easy_init();
    self->root = NULL;
    self->auth = auth;

    zx_Buffer_Init(&self->b, bufSize);
    zx_Buffer_Init(&self->respHeader, bufSize);

    self->boundNic = NULL;

    self->lowSpeedLimit = 0;
    self->lowSpeedTime = 0;
}

void zx_Buffer_Cleanup(zx_Buffer* self)
{
	if (self->buf != NULL) 
	{
		free(self->buf);
		self->buf = NULL;
	}
}

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

void zx_Global_Init(long flags) 
{
    curl_global_init(CURL_GLOBAL_ALL);
}


zx_Error zx_callex(CURL *curl, zx_Buffer *resp, zx_Json **ret, zx_Bool simpleError,zx_Buffer *resph) 
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

static zx_Error zx_Client_callWithBody(zx_Client *self, zx_Json **ret, const char *url,const char *body, zx_Int64 bodyLen, const char *mimeType, const char *method) 
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
	snprintf(c_random, 22, "X-Random-Number: %04d", rand());
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

        if (err.code != 200) 
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

zx_Error zx_Client_CallWithBuffer(zx_Client *self, zx_Json **ret, const char *url,const char *body, size_t bodyLen, const char *mimeType, const char *method) 
{
    CURL *curl = zx_Client_initcall(self, url);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, bodyLen);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);

    return zx_Client_callWithBody(self, ret, url, body, bodyLen, mimeType, method);
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

void zx_Buffer_Init(zx_Buffer* self, size_t initSize)
{
	self->buf = self->curr = (char*)malloc(initSize);
	self->bufEnd = self->buf + initSize;
}

size_t zx_Buffer_Len(zx_Buffer* self)
{
	return self->curr - self->buf;
}

static void zx_Buffer_expand(zx_Buffer* self, size_t expandSize)
{
	size_t oldSize = self->curr - self->buf;
	size_t newSize = (self->bufEnd - self->buf) << 1;
	expandSize += oldSize;
	while (newSize < expandSize) 
	{
		newSize <<= 1;
	}
	self->buf = realloc(self->buf, newSize);
	self->curr = self->buf + oldSize;
	self->bufEnd = self->buf + newSize;
}

const char* zx_Buffer_CStr(zx_Buffer* self)
{
	if (self->curr >= self->bufEnd) 
	{
		zx_Buffer_expand(self, 1);
	}
	*self->curr = '\0';
	return self->buf;
}

size_t zx_Buffer_Write(zx_Buffer* self, const void* buf, size_t n)
{
	if (self->curr + n > self->bufEnd) 
	{
		zx_Buffer_expand(self, n);
	}
	memcpy(self->curr, buf, n);
	self->curr += n;
	return n;
}

#if 1
void zx_Buffer_Reset(zx_Buffer* self)
{
	self->curr = self->buf;
}
#endif

size_t zx_Buffer_Fwrite(const void *buf, size_t size, size_t nmemb, void *self)
{
	//assert(size == 1);
	return zx_Buffer_Write((zx_Buffer*)self, buf, nmemb);
}

