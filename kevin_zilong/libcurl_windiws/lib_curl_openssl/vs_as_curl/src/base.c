/*
 ============================================================================
 Name        : base.c
 Author      : oceanwing.com
 Copyright   : 2017(c) Shenzhen Oceanwing Smart Innovations Co., Ltd.
 Create      : lucien.liu
 Date        : 2017-10-30 
 Description :
 ============================================================================
 */

#include "base.h"
#include "urlsafe_b64.h"
#include <assert.h>
#include <time.h>
#include <errno.h>
#include <curl/curl.h>
//#include "zlog.h"

/*============================================================================*/
/* type zx_Free */

void zx_Free(void* addr)
{
	free(addr);
}

/*============================================================================*/
/* type zx_Count */

zx_Count zx_Count_Inc(zx_Count* self)
{
	return __sync_add_and_fetch(self, 1);
}

zx_Count zx_Count_Dec(zx_Count* self)
{
	return __sync_sub_and_fetch(self, 1);
}

/*============================================================================*/
/* func zx_Seconds */

zx_Int64 zx_Seconds()
{
	return (zx_Int64)time(NULL);
}

/*============================================================================*/
/* func zx_QueryEscape */

typedef enum {
	encodePath,
	encodeUserPassword,
	encodeQueryComponent,
	encodeFragment,
} escapeMode;

// Return true if the specified character should be escaped when
// appearing in a URL string, according to RFC 3986.
static int zx_shouldEscape(int c, escapeMode mode)
{
	if (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || ('0' <= c && c <= '9')) 
	{
		return 0;
	}

	switch (c) 
	{
	case '-': case '_': case '.': case '~': // §2.3 Unreserved characters (mark)
		return 0;
	case '$': case '&': case '+': case ',': case '/': case ':': case ';': case '=': case '?': case '@': // §2.2 Reserved characters (reserved)
		switch (mode) 
		{
			case encodePath: // §3.3
				return c == '?';
			case encodeUserPassword: // §3.2.2
				return c == '@' || c == '/' || c == ':';
			case encodeQueryComponent: // §3.4
				return 1;
			case encodeFragment: // §4.1
				return 0;
		}
	}

	return 1;
}

static const char zx_hexTable[] = "0123456789ABCDEF";

static char* zx_escape(const char* s, escapeMode mode, zx_Bool* fesc)
{
	int spaceCount = 0;
	int hexCount = 0;
	int i, j, len = strlen(s);
	int c;
	char* t;

	for (i = 0; i < len; i++) 
	{
		// prevent c from sign extension
		c = ((int)s[i]) & 0xFF;
		if (zx_shouldEscape(c, mode)) 
		{
			if (c == ' ' && mode == encodeQueryComponent) 
			{
				spaceCount++;
			} 
			else 
			{
				hexCount++;
			}
		}
	}

	if (spaceCount == 0 && hexCount == 0) 
	{
		*fesc = zx_False;
		return (char*)s;
	}

	t = (char*)malloc(len + 2*hexCount + 1);
	j = 0;
	for (i = 0; i < len; i++) 
	{
		// prevent c from sign extension
		c = ((int)s[i]) & 0xFF;
		if (zx_shouldEscape(c, mode)) 
		{
			if (c == ' ' && mode == encodeQueryComponent) 
			{
				t[j] = '+';
				j++;
			} 
			else 
			{
				t[j] = '%';
				t[j+1] = zx_hexTable[c>>4];
				t[j+2] = zx_hexTable[c&15];
				j += 3;
			}
		} 
		else 
		{
			t[j] = s[i];
			j++;
		}
	}
	t[j] = '\0';
	*fesc = zx_True;
	return t;
}

char* zx_PathEscape(const char* s, zx_Bool* fesc)
{
	return zx_escape(s, encodePath, fesc);
}

char* zx_QueryEscape(const char* s, zx_Bool* fesc)
{
	return zx_escape(s, encodeQueryComponent, fesc);
}

/*============================================================================*/
/* func zx_String_Concat */

char* zx_String_Concat2(const char* s1, const char* s2)
{
	size_t len1 = strlen(s1);
	size_t len2 = strlen(s2);
	char* p = (char*)malloc(len1 + len2 + 1);
	memcpy(p, s1, len1);
	memcpy(p + len1, s2, len2);
	p[len1 + len2] = '\0';
	return p;
}

char* zx_String_Concat3(const char* s1, const char* s2, const char* s3)
{
	size_t len1 = strlen(s1);
	size_t len2 = strlen(s2);
	size_t len3 = strlen(s3);
	char* p = (char*)malloc(len1 + len2 + len3 + 1);
	memcpy(p, s1, len1);
	memcpy(p + len1, s2, len2);
	memcpy(p + len1 + len2, s3, len3);
	p[len1 + len2 + len3] = '\0';
	return p;
}

char* zx_String_Concat(const char* s1, ...)
{
	va_list ap;
	char* p;
	const char* s;
	size_t len, slen, len1 = strlen(s1);

	va_start(ap, s1);
	len = len1;
	for (;;) 
	{
		s = va_arg(ap, const char*);
		if (s == NULL) 
		{
			break;
		}
		len += strlen(s);
	}

	p = (char*)malloc(len + 1);

	va_start(ap, s1);
	memcpy(p, s1, len1);
	len = len1;
	for (;;) 
	{
		s = va_arg(ap, const char*);
		if (s == NULL) 
		{
			break;
		}
		slen = strlen(s);
		memcpy(p + len, s, slen);
		len += slen;
	}
	p[len] = '\0';
	return p;
}

char* zx_String_Join(const char* deli, char* strs[], int strCount)
{
	int i = 0;
	char * ret = NULL;
	char * pos = NULL;
	//char * tmpRet = NULL;
	size_t totalLen = 0;
	size_t copyLen = 0;
	size_t deliLen = 0;

	if (strCount == 1) 
	{
		return strdup(strs[0]);
	}

	for (i = 0; i < strCount; i += 1) 
	{
		totalLen += strlen(strs[i]);
	} // for

	deliLen = strlen(deli);
	totalLen += deliLen * (strCount - 1);
	ret = (char *)malloc(totalLen + 1);
	if (ret == NULL) 
	{
		return NULL;
	}

	pos = ret;
	copyLen = strlen(strs[0]);
	memcpy(pos, strs[0], copyLen);
	pos += copyLen;

	for (i = 1; i < strCount; i += 1) 
	{
		memcpy(pos, deli, deliLen);
		pos += deliLen;

		copyLen = strlen(strs[i]);
		memcpy(pos, strs[i], copyLen);
		pos += copyLen;
	} // for

	ret[totalLen] = '\0';
	return ret;
} // zx_String_Join

char* zx_String_Dup(const char* src)
{
	return strdup(src);
} // zx_String_Dup

/*============================================================================*/
/* func zx_String_Encode */

char* zx_String_Encode(const char* buf)
{
	const size_t cb = strlen(buf);
	const size_t cbDest = urlsafe_b64_encode(buf, cb, NULL, 0);
	char* dest = (char*)malloc(cbDest + 1);
	const size_t cbReal = urlsafe_b64_encode(buf, cb, dest, cbDest);
	dest[cbReal] = '\0';
	return dest;
}

char* zx_Memory_Encode(const char* buf, const size_t cb)
{
	const size_t cbDest = urlsafe_b64_encode(buf, cb, NULL, 0);
	char* dest = (char*)malloc(cbDest + 1);
	const size_t cbReal = urlsafe_b64_encode(buf, cb, dest, cbDest);
	dest[cbReal] = '\0';
	return dest;
}

char* zx_String_Decode(const char* buf)
{
	const size_t cb = strlen(buf);
	const size_t cbDest = urlsafe_b64_decode(buf, cb, NULL, 0);
	char* dest = (char*)malloc(cbDest + 1);
	const size_t cbReal = urlsafe_b64_decode(buf, cb, dest, cbDest);
	dest[cbReal] = '\0';
	return dest;
}

/*============================================================================*/
/* type zx_Buffer */

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

void zx_Buffer_Init(zx_Buffer* self, size_t initSize)
{
	self->buf = self->curr = (char*)malloc(initSize);
	self->bufEnd = self->buf + initSize;
}

void zx_Buffer_Reset(zx_Buffer* self)
{
	self->curr = self->buf;
}

void zx_Buffer_Cleanup(zx_Buffer* self)
{
	if (self->buf != NULL) 
	{
		free(self->buf);
		self->buf = NULL;
	}
}

size_t zx_Buffer_Len(zx_Buffer* self)
{
	return self->curr - self->buf;
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

void zx_Buffer_PutChar(zx_Buffer* self, char ch)
{
	if (self->curr >= self->bufEnd) 
	{
		zx_Buffer_expand(self, 1);
	}
	*self->curr++ = ch;
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

size_t zx_Buffer_Fwrite(const void *buf, size_t size, size_t nmemb, void *self)
{
	assert(size == 1);
	return zx_Buffer_Write((zx_Buffer*)self, buf, nmemb);
}

zx_Writer zx_BufWriter(zx_Buffer* self)
{
	zx_Writer writer = {self, zx_Buffer_Fwrite};
	return writer;
}



/*============================================================================*/
/* Oceanwing Format Functions */

char* zx_Buffer_Expand(zx_Buffer* self, size_t n)
{
	if (self->curr + n > self->bufEnd) 
	{
		zx_Buffer_expand(self, n);
	}
	return self->curr;
}

void zx_Buffer_Commit(zx_Buffer* self, char* p)
{
	assert(p >= self->curr);
	assert(p <= self->bufEnd);
	self->curr = p;
}

void zx_Buffer_AppendUint(zx_Buffer* self, zx_Uint64 v)
{
	char buf[32];
	char* p = buf+32;
	for (;;) 
	{
		*--p = '0' + (char)(v % 10);
		v /= 10;
		if (v == 0) 
		{
			break;
		}
	}
	zx_Buffer_Write(self, p, buf+32-p);
}

void zx_Buffer_AppendInt(zx_Buffer* self, zx_Int64 v)
{
	if (v < 0) 
	{
		v = -v;
		zx_Buffer_PutChar(self, '-');
	}
	zx_Buffer_AppendUint(self, v);
}

void zx_Buffer_AppendError(zx_Buffer* self, zx_Error v)
{
	zx_Buffer_PutChar(self, 'E');
	zx_Buffer_AppendInt(self, v.code);
	if (v.message) 
	{
		zx_Buffer_PutChar(self, ' ');
		zx_Buffer_Write(self, v.message, strlen(v.message));
	}
}

void zx_Buffer_AppendEncodedBinary(zx_Buffer* self, const char* buf, size_t cb)
{
	const size_t cbDest = urlsafe_b64_encode(buf, cb, NULL, 0);
	char* dest = zx_Buffer_Expand(self, cbDest);
	const size_t cbReal = urlsafe_b64_encode(buf, cb, dest, cbDest);
	zx_Buffer_Commit(self, dest + cbReal);
}

void zx_Buffer_appendUint(zx_Buffer* self, zx_Valist* ap)
{
	unsigned v = va_arg(ap->items, unsigned);
	zx_Buffer_AppendUint(self, v);
}

void zx_Buffer_appendInt(zx_Buffer* self, zx_Valist* ap)
{
	int v = va_arg(ap->items, int);
	zx_Buffer_AppendInt(self, v);
}

void zx_Buffer_appendUint64(zx_Buffer* self, zx_Valist* ap)
{
	zx_Uint64 v = va_arg(ap->items, zx_Uint64);
	zx_Buffer_AppendUint(self, v);
}

void zx_Buffer_appendInt64(zx_Buffer* self, zx_Valist* ap)
{
	zx_Int64 v = va_arg(ap->items, zx_Int64);
	zx_Buffer_AppendInt(self, v);
}

void zx_Buffer_appendString(zx_Buffer* self, zx_Valist* ap)
{
	const char* v = va_arg(ap->items, const char*);
	if (v == NULL) 
	{
		v = "(null)";
	}
	zx_Buffer_Write(self, v, strlen(v));
}

void zx_Buffer_appendEncodedString(zx_Buffer* self, zx_Valist* ap)
{
	const char* v = va_arg(ap->items, const char*);
	size_t n = strlen(v);
	zx_Buffer_AppendEncodedBinary(self, v, n);
}

void zx_Buffer_appendError(zx_Buffer* self, zx_Valist* ap)
{
	zx_Error v = va_arg(ap->items, zx_Error);
	zx_Buffer_AppendError(self, v);
}

void zx_Buffer_appendPercent(zx_Buffer* self, zx_Valist* ap)
{
	zx_Buffer_PutChar(self, '%');
}


char * hex2str(const char *ssrc, int nlen)  
{  
    int  i = 0;  
	if (ssrc && nlen > 0)
	{
		char *sztmp = (char *)malloc(3*nlen); 	 
  		if (sztmp)
		{
    		for( i = 0; i < nlen; i++ )  
    		{  
        		sprintf(&sztmp[i*3], "%02X ", (unsigned char)ssrc[i]);  
    		}  
			return sztmp;
		}
	}
    return NULL; 
}  


int byte_mac_to_str(const char *ssrc, char *dest, int nlen)  
{  
    int  i = 0;  
	if (ssrc && dest && nlen > 0)
	{
   		for( i = 0; i < nlen; i++ )  
    	{  
			if (i == nlen - 1)
        		sprintf(dest[i*3], "%02X", (unsigned char)ssrc[i]);  
			else
				sprintf(dest[i*3], "%02X:", (unsigned char)ssrc[i]); 
    	}  
	}
    return 0; 
}  
  
//十六进制字符串转换为字节流  
void hexstr2byte(const char* source, unsigned char* dest, int sourceLen)  
{  
    short i;  
    unsigned char highByte, lowByte;  
      
    for (i = 0; i < sourceLen; i += 2)  
    {  
        highByte = toupper(source[i]);  
        lowByte  = toupper(source[i + 1]);  
  
        if (highByte > 0x39)  
            highByte -= 0x37;  
        else  
            highByte -= 0x30;  
  
        if (lowByte > 0x39)  
            lowByte -= 0x37;  
        else  
            lowByte -= 0x30;  
  
        dest[i / 2] = (highByte << 4) | lowByte;  
    }  
}  

/*============================================================================*/
/* func zx_FILE_Reader */

zx_Reader zx_FILE_Reader(FILE* fp)
{
	zx_Reader reader = {fp, (zx_FnRead)fread};
	return reader;
}

zx_Writer zx_FILE_Writer(FILE* fp)
{
	zx_Writer writer = {fp, (zx_FnWrite)fwrite};
	return writer;
}

/*============================================================================*/
/* func zx_Copy */

zx_Error zx_OK = {
	200, "OK"
};

zx_Error zx_Copy(zx_Writer w, zx_Reader r, void* buf, size_t n, zx_Int64* ret)
{
	zx_Int64 fsize = 0;
	size_t n1, n2;
	char* p = (char*)buf;
	if (buf == NULL) 
	{
		p = (char*)malloc(n);
	}
	for (;;) 
	{
		n1 = r.Read(p, 1, n, r.self);
		if (n1 > 0) 
		{
			n2 = w.Write(p, 1, n1, w.self);
			fsize += n2;
		} 
		else 
		{
			n2 = 0;
		}
		if (n2 != n) 
		{
			break;
		}
	}
	if (buf == NULL) 
	{
		free(p);
	}
	if (ret) 
	{
		*ret = fsize;
	}
	return zx_OK;
}

/*============================================================================*/
/* func zx_Null_Fwrite */

size_t zx_Null_Fwrite(const void *buf, size_t size, size_t nmemb, void *self)
{
	return nmemb;
}

zx_Writer zx_Discard = 
{
	NULL, zx_Null_Fwrite
};

void printf_hex(const char *ssrc, int nlen)
{
	char *hex = hex2str(ssrc, nlen);
	if (hex)
	{
		//dzlog_info("%s", hex);
		printf("%s", hex);
		free(hex);
	}
}

/*============================================================================*/
/* func zx_Null_Log */
#if 0

/*============================================================================*/
/* Oceanwing Format */

typedef struct _zx_formatProc {
	zx_FnAppender Append;
	char esc;
} zx_formatProc;

static zx_formatProc zx_formatProcs[] = {
	{ zx_Buffer_appendInt, 'd' },
	{ zx_Buffer_appendUint, 'u' },
	{ zx_Buffer_appendInt64, 'D' },
	{ zx_Buffer_appendUint64, 'U' },
	{ zx_Buffer_appendString, 's' },
	{ zx_Buffer_appendEncodedString, 'S' },
	{ zx_Buffer_appendError, 'E' },
	{ zx_Buffer_appendPercent, '%' },
};

static zx_FnAppender zx_Appenders[128] = {0};

void zx_Format_Register(char esc, zx_FnAppender appender)
{
	if ((unsigned)esc < 128) 
	{
		zx_Appenders[esc] = appender;
	}
}

void zx_Buffer_formatInit(void)
{
	zx_formatProc* p;
	zx_formatProc* pEnd = (zx_formatProc*)((char*)zx_formatProcs + sizeof(zx_formatProcs));
	for (p = zx_formatProcs; p < pEnd; p++) 
	{
		zx_Appenders[p->esc] = p->Append;
	}
}

void zx_Buffer_AppendFormatV(zx_Buffer* self, const char* fmt, zx_Valist* args)
{
	unsigned char ch;
	const char* p;
	zx_FnAppender appender;

	for (;;) 
	{
		p = strchr(fmt, '%');
		if (p == NULL) 
		{
			break;
		}
		if (p > fmt) 
		{
			zx_Buffer_Write(self, fmt, p - fmt);
		}
		p++;
		ch = *p++;
		fmt = p;
		if (ch < 128) 
		{
			appender = zx_Appenders[ch];
			if (appender != NULL) 
			{
				appender(self, args);
				continue;
			}
		}
		zx_Buffer_PutChar(self, '%');
		zx_Buffer_PutChar(self, ch);
	}
	if (*fmt) 
	{
		zx_Buffer_Write(self, fmt, strlen(fmt));
	}
}

void zx_Buffer_AppendFormat(zx_Buffer* self, const char* fmt, ...)
{
	zx_Valist args;
	va_start(args.items, fmt);
	zx_Buffer_AppendFormatV(self, fmt, &args);
}

const char* zx_Buffer_Format(zx_Buffer* self, const char* fmt, ...)
{
	zx_Valist args;
	va_start(args.items, fmt);
	zx_Buffer_Reset(self);
	zx_Buffer_AppendFormatV(self, fmt, &args);
	return zx_Buffer_CStr(self);
}

char* zx_String_Format(size_t initSize, const char* fmt, ...)
{
	zx_Valist args;
	zx_Buffer buf;
	va_start(args.items, fmt);
	zx_Buffer_Init(&buf, initSize);
	zx_Buffer_AppendFormatV(&buf, fmt, &args);
	return (char*)zx_Buffer_CStr(&buf);
}


void zx_Null_Log(const char* fmt, ...)
{
}

/*============================================================================*/
/* func zx_Stderr_Info/Warn */

static const char* zx_Levels[] = {
	"[DEBUG]",
	"[INFO]",
	"[WARN]",
	"[ERROR]",
	"[PANIC]",
	"[FATAL]"
};


void zx_Logv(zx_Writer w, int ilvl, const char* fmt, zx_Valist* args)
{
	struct timeval tv;  
    struct timezone tz;       
    struct tm *p; 
	const char* level = zx_Levels[ilvl];
    char stime[20] = {0};	

	zx_Buffer log;
	zx_Buffer_Init(&log, 512);
	
	gettimeofday(&tv, &tz);  
    p = localtime(&tv.tv_sec); 
	sprintf(stime, "%02d-%02d %02d:%02d:%02d.%03ld", 1+p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, tv.tv_usec/1000); 

	zx_Buffer_Write(&log, stime, strlen(stime));
	zx_Buffer_PutChar(&log, ' '); 
	zx_Buffer_Write(&log, level, strlen(level));
	zx_Buffer_PutChar(&log, ' ');    

	zx_Buffer_AppendFormatV(&log, fmt, args);
	zx_Buffer_PutChar(&log, '\n');
	w.Write(log.buf, 1, log.curr-log.buf, w.self);
	zx_Buffer_Cleanup(&log);
}

void zx_Stderr_Info(const char* fmt, ...)
{
	zx_Valist args;
	va_start(args.items, fmt);
	zx_Logv(zx_Stderr, zx_Linfo, fmt, &args);
	//dzlog_info(fmt, &args);
}

void zx_Stderr_Warn(const char* fmt, ...)
{
	zx_Valist args;
	va_start(args.items, fmt);
	zx_Logv(zx_Stderr, zx_Lwarn, fmt, &args);
	//dzlog_warn(fmt, &args);
}

void zx_Stderr_Err(const char* fmt, ...)
{
	zx_Valist args;
	va_start(args.items, fmt);
	zx_Logv(zx_Stderr, zx_Lerror, fmt, &args);
	//dzlog_error(fmt, &args);
}


void get_filename()
{
	char fn[30],*p;
    char pathname[80] = __FILE__;
    strcpy(fn,(p=strrchr(pathname,'\\')) ? p+1 : pathname);
    printf("%s\n",fn);
}

#endif






/*============================================================================*/

