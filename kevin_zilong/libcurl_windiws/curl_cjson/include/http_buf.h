#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cjson.h"
#include "curl/curl.h"

#ifndef HTTP_BUF_H
#define HTTP_BUF_H

#pragma pack(1)

enum 
{
	zx_False = 0,
	zx_True = 1
};


enum
{
	zx_NULL = -1000,

};


#ifdef __cplusplus
extern "C"
{
#endif

typedef struct curl_slist zx_Header;


typedef struct _zx_Error 
{
	int code;
	const char* message;
} zx_Error;

typedef struct _zx_Buffer 
{
	char* buf;
	char* curr;
	char* bufEnd;
} zx_Buffer;

typedef size_t (*zx_FnWrite)(const void *buf, size_t, size_t n, void *self);
typedef struct _zx_Writer 
{
	void* self;
	zx_FnWrite Write;
} zx_Writer;

typedef struct _zx_Auth_Itbl 
{
	zx_Error (*Auth)(void* self, zx_Header** header, const char* url, const char* addition, size_t addlen);
	void (*Release)(void* self);
} zx_Auth_Itbl;

typedef struct _zx_Auth 
{
	void* self;
	zx_Auth_Itbl* itbl;
} zx_Auth;

typedef struct _zx_Client {
	void* curl;
	zx_Auth auth;
	zx_Json* root;
	zx_Buffer b;
	zx_Buffer respHeader;

	// Use the following field to specify which NIC to use for sending packets.
	const char* boundNic;

	// Use the following field to specify the average transfer speed in bytes per second (Bps)
	// that the transfer should be below during lowSpeedTime seconds for this SDK to consider
	// it to be too slow and abort.
	long lowSpeedLimit;

	// Use the following field to specify the time in number seconds that
	// the transfer speed should be below the logSpeedLimit for this SDK to consider it
	// too slow and abort.
	long lowSpeedTime;
} zx_Client;


typedef long long zx_Int64;
typedef unsigned long long zx_Uint64;
typedef unsigned int zx_Uint32;
typedef unsigned short zx_Uint16;
//typedef unsigned long size_t;
typedef int ssize_t;
typedef int zx_Bool;

void zx_Client_InitEx(zx_Client *self, zx_Auth auth, size_t bufSize);
CURL *zx_Client_reset(zx_Client *self);
void zx_Buffer_Cleanup(zx_Buffer* self);
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

#ifdef __cplusplus
}
#endif

#pragma pack(0)

#endif /* zx_OTA_INTERFACE_H */




