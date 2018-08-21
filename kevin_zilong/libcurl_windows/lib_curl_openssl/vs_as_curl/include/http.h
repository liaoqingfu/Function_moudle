/*
 ============================================================================
 Name        : http.h
 Author      : Oceanwing.com
 Copyright   : 2017(c) Shenzhen Oceanwing Smart Innovations Co., Ltd.
 Create      : lucien.liu
 Date        : 2017-10-30 
 Description : 
 ============================================================================
 */

#ifndef zx_HTTP_H
#define zx_HTTP_H

#include "base.h"
#include "cjson.h"

/*============================================================================*/
/* Global */

#ifdef __cplusplus
extern "C"
{
#endif

void zx_Global_Init(long flags);
void zx_Global_Cleanup();

void zx_MacAuth_Init();
void zx_MacAuth_Cleanup();

void zx_Servend_Init(long flags);
void zx_Servend_Cleanup();

#ifdef __cplusplus
}
#endif

/*============================================================================*/
/* type zx_Mutex */

#if defined(_WIN32)
#include <windows.h>
typedef CRITICAL_SECTION zx_Mutex;
#else
#include <pthread.h>
typedef pthread_mutex_t zx_Mutex;
#endif

#ifdef __cplusplus
extern "C"
{
#endif

void zx_Mutex_Init(zx_Mutex* self);
void zx_Mutex_Cleanup(zx_Mutex* self);

void zx_Mutex_Lock(zx_Mutex* self);
void zx_Mutex_Unlock(zx_Mutex* self);

/*============================================================================*/
/* type zx_Json */

typedef struct cJSON zx_Json;

const char* zx_Json_GetString(zx_Json* self, const char* key, const char* defval);
int zx_Json_GetArraySize(zx_Json *self, const char* key, zx_Int64 defval);
const char* zx_Json_GetStringAt(zx_Json* self, int n, const char* defval);
zx_Uint32 zx_Json_GetUInt32(zx_Json *self, const char *key, zx_Uint32 defval);
zx_Int64 zx_Json_GetInt64(zx_Json* self, const char* key, zx_Int64 defval);
zx_Uint64 zx_Json_GetUInt64(zx_Json* self, const char* key, zx_Uint64 defval);
int zx_Json_GetInt(zx_Json *self, const char *key, int defval);
int zx_Json_GetBoolean(zx_Json* self, const char* key, int defval);
zx_Json* zx_Json_GetObjectItem(zx_Json* self, const char* key, zx_Json* defval);
zx_Json* zx_Json_GetArrayItem(zx_Json* self, int n, zx_Json* defval);
void zx_Json_Destroy(zx_Json* self);

/*============================================================================*/
/* type zx_Auth */

#pragma pack(1)

typedef struct curl_slist zx_Header;

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

zx_Auth zx_NoAuth;

/*============================================================================*/
/* type zx_Client */

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

void zx_Client_InitEx(zx_Client* self, zx_Auth auth, size_t bufSize);
void zx_Client_Cleanup(zx_Client* self);
void zx_Client_BindNic(zx_Client* self, const char* nic);
void zx_Client_SetLowSpeedLimit(zx_Client* self, long lowSpeedLimit, long lowSpeedTime);

zx_Error zx_Client_Call(zx_Client* self, zx_Json** ret, const char* url, const char *method);
zx_Error zx_Client_CallNoRet(zx_Client* self, const char* url, const char *method);
zx_Error zx_Client_CallWithBinary(zx_Client* self, zx_Json** ret, const char* url,
												zx_Reader body, zx_Int64 bodyLen, const char* mimeType, const char *method);
zx_Error zx_Client_CallWithBuffer(zx_Client* self, zx_Json** ret, const char* url,
												const char* body, size_t bodyLen, const char* mimeType, const char *method);zx_Error zx_Client_CallWithBuffer2(zx_Client* self, zx_Json** ret, const char* url,
								const char* body, size_t bodyLen, const char* mimeType, const char *method);

/*============================================================================*/
/* func zx_Client_InitNoAuth/InitMacAuth  */

typedef struct _zx_Mac 
{
	const char* accessKey;
	const char* secretKey;
} zx_Mac;

zx_Auth zx_MacAuth(zx_Mac* mac);

char* zx_Mac_Sign(zx_Mac* self, char* data);
char* zx_Mac_SignToken(zx_Mac* self, char* data);

void zx_Client_InitNoAuth(zx_Client* self, size_t bufSize);
void zx_Client_InitMacAuth(zx_Client* self, size_t bufSize, zx_Mac* mac);

/*============================================================================*/

#pragma pack()

#ifdef __cplusplus
}
#endif

#endif /* zx_HTTP_H */

