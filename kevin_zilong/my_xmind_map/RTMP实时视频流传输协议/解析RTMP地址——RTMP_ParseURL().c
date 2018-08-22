RTMP的URL的格式：

rtmp://localhost/vod/mp4:sample1_1500kbps.f4v

    “://”之前的是使用的协议类型，可以是rtmp，rtmpt，rtmps等

    之后是服务器地址

    再之后是端口号（可以没有，默认1935）

    在之后是application的名字，在这里是“vod”

    最后是流媒体文件路径。
	


    /* 
     * 本文件主要包含了对输入URL的解析 
     */  
    #include "stdafx.h"  
    #include <stdlib.h>  
    #include <string.h>  
      
    #include <assert.h>  
    #include <ctype.h>  
      
    #include "rtmp_sys.h"  
    #include "log.h"  
      
    /*解析URL，得到协议名称(protocol)，主机名称(host)，应用程序名称(app) 
     * 
     */  
    int RTMP_ParseURL(const char *url, int *protocol, AVal *host, unsigned int *port,  
        AVal *playpath, AVal *app)  
    {  
        char *p, *end, *col, *ques, *slash;  
      
        RTMP_Log(RTMP_LOGDEBUG, "Parsing...");  
      
        *protocol = RTMP_PROTOCOL_RTMP;  
        *port = 0;  
        playpath->av_len = 0;  
        playpath->av_val = NULL;  
        app->av_len = 0;  
        app->av_val = NULL;  
      
        /* 字符串解析 */  
        /* 查找“://”  */  
        //函数原型：char *strstr(char *str1, char *str2);  
        //功能：找出str2字符串在str1字符串中第一次出现的位置（不包括str2的串结束符）。  
        //返回值：返回该位置的指针，如找不到，返回空指针。  
        p = strstr((char *)url, "://");  
        if(!p) {  
            RTMP_Log(RTMP_LOGERROR, "RTMP URL: No :// in url!");  
            return FALSE;  
        }  
        {  
        //指针相减，返回“://”之前字符串长度len  
        int len = (int)(p-url);  
        //获取使用的协议  
        //通过比较字符串的方法  
        if(len == 4 && strncasecmp(url, "rtmp", 4)==0)  
            *protocol = RTMP_PROTOCOL_RTMP;  
        else if(len == 5 && strncasecmp(url, "rtmpt", 5)==0)  
            *protocol = RTMP_PROTOCOL_RTMPT;  
        else if(len == 5 && strncasecmp(url, "rtmps", 5)==0)  
                *protocol = RTMP_PROTOCOL_RTMPS;  
        else if(len == 5 && strncasecmp(url, "rtmpe", 5)==0)  
                *protocol = RTMP_PROTOCOL_RTMPE;  
        else if(len == 5 && strncasecmp(url, "rtmfp", 5)==0)  
                *protocol = RTMP_PROTOCOL_RTMFP;  
        else if(len == 6 && strncasecmp(url, "rtmpte", 6)==0)  
                *protocol = RTMP_PROTOCOL_RTMPTE;  
        else if(len == 6 && strncasecmp(url, "rtmpts", 6)==0)  
                *protocol = RTMP_PROTOCOL_RTMPTS;  
        else {  
            RTMP_Log(RTMP_LOGWARNING, "Unknown protocol!\n");  
            goto parsehost;  
        }  
        }  
      
        RTMP_Log(RTMP_LOGDEBUG, "Parsed protocol: %d", *protocol);  
      
    parsehost:  
        //获取主机名称  
        //跳过“://”  
        p+=3;  
      
        /* 检查一下主机名 */  
        if(*p==0) {  
            RTMP_Log(RTMP_LOGWARNING, "No hostname in URL!");  
            return FALSE;  
        }  
        //原型：char *strchr(const char *s,char c);   
        //功能：查找字符串s中首次出现字符c的位置  
        //说明：返回首次出现c的位置的指针，如果s中不存在c则返回NULL。   
        end   = p + strlen(p);//指向结尾的指针  
        col   = strchr(p, ':');//指向冒号（第一个）的指针  
        ques  = strchr(p, '?');//指向问号（第一个）的指针  
        slash = strchr(p, '/');//指向斜杠（第一个）的指针  
      
        {  
        int hostlen;  
        if(slash)  
            hostlen = slash - p;  
        else  
            hostlen = end - p;  
        if(col && col -p < hostlen)  
            hostlen = col - p;  
      
        if(hostlen < 256) {  
            host->av_val = p;  
            host->av_len = hostlen;  
            RTMP_Log(RTMP_LOGDEBUG, "Parsed host    : %.*s", hostlen, host->av_val);  
        } else {  
            RTMP_Log(RTMP_LOGWARNING, "Hostname exceeds 255 characters!");  
        }  
      
        p+=hostlen;  
        }  
      
        /* 获取端口号 */  
        if(*p == ':') {  
            unsigned int p2;  
            p++;  
            p2 = atoi(p);  
            if(p2 > 65535) {  
                RTMP_Log(RTMP_LOGWARNING, "Invalid port number!");  
            } else {  
                *port = p2;  
            }  
        }  
      
        if(!slash) {  
            RTMP_Log(RTMP_LOGWARNING, "No application or playpath in URL!");  
            return TRUE;  
        }  
        p = slash+1;  
      
        {  
        /* 获取应用程序(application) 
         * 
         * rtmp://host[:port]/app[/appinstance][/...] 
         * application = app[/appinstance] 
         */  
      
        char *slash2, *slash3 = NULL;//指向第二个斜杠，第三个斜杠的指针  
        int applen, appnamelen;  
      
        slash2 = strchr(p, '/');//指向第二个斜杠  
        if(slash2)  
            slash3 = strchr(slash2+1, '/');//指向第三个斜杠，注意slash2之所以+1是因为让其后移一位  
      
        applen = end-p; /* ondemand, pass all parameters as app */  
        appnamelen = applen; /* ondemand length */  
      
        if(ques && strstr(p, "slist=")) { /* whatever it is, the '?' and slist= means we need to use everything as app and parse plapath from slist= */  
            appnamelen = ques-p;  
        }  
        else if(strncmp(p, "ondemand/", 9)==0) {  
                    /* app = ondemand/foobar, only pass app=ondemand */  
                    applen = 8;  
                    appnamelen = 8;  
            }  
        else { /* app!=ondemand, so app is app[/appinstance] */  
            if(slash3)  
                appnamelen = slash3-p;  
            else if(slash2)  
                appnamelen = slash2-p;  
      
            applen = appnamelen;  
        }  
      
        app->av_val = p;  
        app->av_len = applen;  
        RTMP_Log(RTMP_LOGDEBUG, "Parsed app     : %.*s", applen, p);  
      
        p += appnamelen;  
        }  
      
        if (*p == '/')  
            p++;  
      
        if (end-p) {  
            AVal av = {p, end-p};  
            RTMP_ParsePlaypath(&av, playpath);  
        }  
      
        return TRUE;  
    }  
      
    /* 
     * 从URL中获取播放路径（playpath）。播放路径是URL中“rtmp://host:port/app/”后面的部分 
     * 
     * 获取FMS能够识别的播放路径 
     * mp4 流: 前面添加 "mp4:", 删除扩展名 
     * mp3 流: 前面添加 "mp3:", 删除扩展名 
     * flv 流: 删除扩展名 
     */  
    void RTMP_ParsePlaypath(AVal *in, AVal *out) {  
        int addMP4 = 0;  
        int addMP3 = 0;  
        int subExt = 0;  
        const char *playpath = in->av_val;  
        const char *temp, *q, *ext = NULL;  
        const char *ppstart = playpath;  
        char *streamname, *destptr, *p;  
      
        int pplen = in->av_len;  
      
        out->av_val = NULL;  
        out->av_len = 0;  
      
        if ((*ppstart == '?') &&  
            (temp=strstr(ppstart, "slist=")) != 0) {  
            ppstart = temp+6;  
            pplen = strlen(ppstart);  
      
            temp = strchr(ppstart, '&');  
            if (temp) {  
                pplen = temp-ppstart;  
            }  
        }  
      
        q = strchr(ppstart, '?');  
        if (pplen >= 4) {  
            if (q)  
                ext = q-4;  
            else  
                ext = &ppstart[pplen-4];  
            if ((strncmp(ext, ".f4v", 4) == 0) ||  
                (strncmp(ext, ".mp4", 4) == 0)) {  
                addMP4 = 1;  
                subExt = 1;  
            /* Only remove .flv from rtmp URL, not slist params */  
            } else if ((ppstart == playpath) &&  
                (strncmp(ext, ".flv", 4) == 0)) {  
                subExt = 1;  
            } else if (strncmp(ext, ".mp3", 4) == 0) {  
                addMP3 = 1;  
                subExt = 1;  
            }  
        }  
      
        streamname = (char *)malloc((pplen+4+1)*sizeof(char));  
        if (!streamname)  
            return;  
      
        destptr = streamname;  
        if (addMP4) {  
            if (strncmp(ppstart, "mp4:", 4)) {  
                strcpy(destptr, "mp4:");  
                destptr += 4;  
            } else {  
                subExt = 0;  
            }  
        } else if (addMP3) {  
            if (strncmp(ppstart, "mp3:", 4)) {  
                strcpy(destptr, "mp3:");  
                destptr += 4;  
            } else {  
                subExt = 0;  
            }  
        }  
      
        for (p=(char *)ppstart; pplen >0;) {  
            /* skip extension */  
            if (subExt && p == ext) {  
                p += 4;  
                pplen -= 4;  
                continue;  
            }  
            if (*p == '%') {  
                unsigned int c;  
                sscanf(p+1, "%02x", &c);  
                *destptr++ = c;  
                pplen -= 3;  
                p += 3;  
            } else {  
                *destptr++ = *p++;  
                pplen--;  
            }  
        }  
        *destptr = '\0';  
      
        out->av_val = streamname;  
        out->av_len = destptr - streamname;  
    }  