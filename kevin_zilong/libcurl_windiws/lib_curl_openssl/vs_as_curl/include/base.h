/*============================================================================
 Name        : base.h
 Author      : oceanwing.com
 Copyright   : 2017(c) Shenzhen Oceanwing Smart Innovations Co., Ltd.
 Create      : lucien.liu
 Date        : 2017-10-30 
 Description : 
 ============================================================================*/

#ifndef zx_BASE_H
#define zx_BASE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef off_t zx_Off_T;

#pragma pack(1)

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*/
/* func zx_Zero */

#define zx_Zero(v)		memset(&v, 0, sizeof(v))

/*============================================================================*/
/* func zx_snprintf */

#define zx_snprintf		snprintf

/*============================================================================*/
/* type zx_Int64, zx_Uint32 */
typedef long long zx_Int64;
typedef unsigned long long zx_Uint64;
typedef unsigned int zx_Uint32;
typedef unsigned short zx_Uint16;

/*============================================================================*/
/* type zx_Bool */

typedef int zx_Bool;

enum 
{
	zx_False = 0,
	zx_True = 1
};


enum
{
	zx_NULL = -1000,

};

/*============================================================================*/
/* type zx_Error */

typedef struct _zx_Error 
{
	int code;
	const char* message;
} zx_Error;

zx_Error zx_OK;

const char *zx_UP_HOST;

/*============================================================================*/
/* type zx_Free */

void zx_Free(void* addr);

/*============================================================================*/
/* type zx_Count */

typedef long zx_Count;

#define COMPANY_NAME "U2hlbnpoZW4gT2NlYW53aW5nIFNtYXJ0IElubm92YXRpb25zIENvLiwgTHRk"
//"Shenzhen Oceanwing Smart Innovations Co., Ltd"
#define LONG_STR_LEN    (512)
#define NORMAL_STR_LEN  (128)
#define SHORT_STR_LEN   (32)
#define SMALL_STR_LEN   (16)
#define OTHER_STR_LEN   (20)

zx_Count zx_Count_Inc(zx_Count* self);
zx_Count zx_Count_Dec(zx_Count* self);

/*============================================================================*/
/* func zx_String_Concat */

char* zx_String_Concat2(const char* s1, const char* s2);
char* zx_String_Concat3(const char* s1, const char* s2, const char* s3);
char* zx_String_Concat(const char* s1, ...);
char* zx_String_Format(size_t initSize, const char* fmt, ...);
char* zx_String_Join(const char* deli, char* strs[], int strCount);
char* zx_String_Dup(const char* src);

/*============================================================================*/
/* func zx_String_Encode */

char* zx_Memory_Encode(const char* buf, const size_t cb);
char* zx_String_Encode(const char* s);
char* zx_String_Decode(const char* s);
char * hex2str(const char *ssrc, int nlen);
void hexstr2byte(const char* source, unsigned char* dest, int sourceLen);
int byte_mac_to_str(const char *ssrc, char *dest, int nlen);

/*============================================================================*/
/* func zx_QueryEscape */

char* zx_PathEscape(const char* s, zx_Bool* fesc);
char* zx_QueryEscape(const char* s, zx_Bool* fesc);

/*============================================================================*/
/* func zx_Seconds */
zx_Int64 zx_Seconds();

/*============================================================================*/
/* type zx_Reader */
typedef size_t (*zx_FnRead)(void *buf, size_t, size_t n, void *self);


typedef struct _zx_Reader 
{
	void* self;
	zx_FnRead Read;
} zx_Reader;

zx_Reader zx_FILE_Reader(FILE* fp);

/*============================================================================*/
/* type zx_Writer */
typedef size_t (*zx_FnWrite)(const void *buf, size_t, size_t n, void *self);
typedef struct _zx_Writer 
{
	void* self;
	zx_FnWrite Write;
} zx_Writer;

zx_Writer zx_FILE_Writer(FILE* fp);
zx_Error zx_Copy(zx_Writer w, zx_Reader r, void* buf, size_t n, zx_Int64* ret);

#define zx_Stderr zx_FILE_Writer(stderr)

/*============================================================================*/
/* type zx_ReaderAt */

typedef	ssize_t (*zx_FnReadAt)(void* self, void *buf, size_t bytes, zx_Off_T offset);

typedef struct _zx_ReaderAt 
{
	void* self;
	zx_FnReadAt ReadAt;
} zx_ReaderAt;

/*============================================================================*/
/* type zx_Buffer */

typedef struct _zx_Valist 
{
	va_list items;
} zx_Valist;

typedef struct _zx_Buffer 
{
	char* buf;
	char* curr;
	char* bufEnd;
} zx_Buffer;

void zx_Buffer_Init(zx_Buffer* self, size_t initSize);
void zx_Buffer_Reset(zx_Buffer* self);
void zx_Buffer_AppendInt(zx_Buffer* self, zx_Int64 v);
void zx_Buffer_AppendUint(zx_Buffer* self, zx_Uint64 v);
void zx_Buffer_AppendError(zx_Buffer* self, zx_Error v);
void zx_Buffer_AppendEncodedBinary(zx_Buffer* self, const char* buf, size_t cb);
void zx_Buffer_Cleanup(zx_Buffer* self);
const char* zx_Buffer_CStr(zx_Buffer* self);

void zx_Buffer_PutChar(zx_Buffer* self, char ch);
size_t zx_Buffer_Len(zx_Buffer* self);
size_t zx_Buffer_Write(zx_Buffer* self, const void* buf, size_t n);
size_t zx_Buffer_Fwrite(const void* buf, size_t, size_t n, void* self);
zx_Writer zx_BufWriter(zx_Buffer* self);
char* zx_Buffer_Expand(zx_Buffer* self, size_t n);
void zx_Buffer_Commit(zx_Buffer* self, char* p);

/*============================================================================*/
/* func zx_Null_Fwrite */
size_t zx_Null_Fwrite(const void* buf, size_t, size_t n, void* self);
zx_Writer zx_Discard;

/*============================================================================*/
/* type zx_ReadBuf */
typedef struct _zx_ReadBuf 
{
	const char* buf;
	zx_Off_T off;
	zx_Off_T limit;
} zx_ReadBuf;

zx_Reader zx_BufReader(zx_ReadBuf* self, const char* buf, size_t bytes);
zx_ReaderAt zx_BufReaderAt(zx_ReadBuf* self, const char* buf, size_t bytes);

/*============================================================================*/
/* type zx_Tee */
typedef struct _zx_Tee 
{
	zx_Reader r;
	zx_Writer w;
} zx_Tee;

zx_Reader zx_TeeReader(zx_Tee* self, zx_Reader r, zx_Writer w);

/*============================================================================*/
/* type zx_Section */

typedef struct _zx_Section 
{
	zx_ReaderAt r;
	zx_Off_T off;
	zx_Off_T limit;
} zx_Section;
zx_Reader zx_SectionReader(zx_Section* self, zx_ReaderAt r, zx_Off_T off, size_t n);

/*============================================================================*/
/* type zx_Crc32 */

unsigned long zx_Crc32_Update(unsigned long inCrc32, const void *buf, size_t bufLen);

typedef struct _zx_Crc32 
{
	unsigned long val;
} zx_Crc32;

zx_Writer zx_Crc32Writer(zx_Crc32* self, unsigned long inCrc32);

/*============================================================================*/
/* type zx_File */

typedef struct _zx_File zx_File;

#include <sys/stat.h>
typedef struct stat zx_FileInfo;

zx_Error zx_File_Open(zx_File** pp, const char* file);
zx_Error zx_File_Stat(zx_File* self, zx_FileInfo* fi);

#define zx_FileInfo_Fsize(fi) ((fi).st_size)

void zx_File_Close(void* self);
//ssize_t zx_File_ReadAt(void* self, void *buf, size_t bytes, zx_Off_T offset);
zx_ReaderAt zx_FileReaderAt(zx_File* self);

void printf_hex(const char *ssrc, int nlen);

#if 0
void get_filename();

/*============================================================================*/
/* type zx_Log */
void zx_Buffer_formatInit(void);

void zx_Buffer_AppendFormat(zx_Buffer* self, const char* fmt, ...);
void zx_Buffer_AppendFormatV(zx_Buffer* self, const char* fmt, zx_Valist* args);
const char* zx_Buffer_Format(zx_Buffer* self, const char* fmt, ...);

typedef void (*zx_FnAppender)(zx_Buffer* self, zx_Valist* ap);
void zx_Format_Register(char esc, zx_FnAppender appender);

#define zx_Ldebug	0
#define zx_Linfo    1
#define zx_Lwarn    2
#define zx_Lerror	3
#define zx_Lpanic	4
#define zx_Lfatal	5

void zx_Logv(zx_Writer w, int level, const char* fmt, zx_Valist* args);
void zx_Stderr_Info(const char* fmt, ...);
void zx_Stderr_Warn(const char* fmt, ...);
void zx_Stderr_Err(const char* fmt, ...);

void zx_Null_Log(const char* fmt, ...);

#ifndef zx_Log_Info
	#ifdef zx_DISABLE_LOG
		#define zx_Log_Info	zx_Null_Log
		#define zx_Log_Warn	zx_Null_Log
	#else
		#define zx_Log_Info	zx_Stderr_Info
		#define zx_Log_Warn	zx_Stderr_Warn
		#define zx_Log_err	zx_Stderr_Err
	#endif
#endif
#endif
/*============================================================================*/

#pragma pack()

#ifdef __cplusplus
}
#endif

#endif /* zx_BASE_H */
