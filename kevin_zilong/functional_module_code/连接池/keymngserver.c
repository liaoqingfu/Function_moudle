#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

#include "keymngserverop.h"
#include "keymnglog.h"
#include "poolsocket.h"

MngServer_Info svrInfo;
int flg = 0;

#define CREATE_DAEMON {if(fork()>0){exit(0);}setsid();}  //创建守护进程

void sigcallback(int signum)
{
    flg = 1;
    return;
}

void *subThread_callback(void *arg)
{
    int ret = -1,type = -1;
    int timeout = 3;
    int connfd = (int)(intptr_t)arg;
    //存储req TLV 报文数据
    unsigned char *pReqOutData = NULL;
    int reqOutlen = 0;
    //存储res TLV 报文数据
    unsigned char *pResOutData = NULL;
    int resOutlen = 0;
    
    MsgKey_Req *pMsgKey_Req = NULL;
    printf("---线程中---\n");
    while(1)
    {
        if(flg == 1)
        {
            break;
        }
        //服务器端端接受报文
        ret = sckServer_rev(connfd, timeout, &pReqOutData, &reqOutlen); //1
        if(ret == Sck_ErrPeerClosed)
        {
            printf("---服务器检测到客户端已经关闭---\n");
            break;
        }
        else if(ret == Sck_ErrTimeOut)
        {
            printf("服务器端 sckServer_rev 超时\n");
            continue;
        }
        else if(ret != 0)
        {
            printf("sckServer_rev error:%d\n",ret);
            break;
        }
        printf("---服务器检测接收到报文---\n");
        //解码请求报文
        ret = MsgDecode( pReqOutData,reqOutlen,(void **)&pMsgKey_Req,&type);
	    if(ret != 0)
        {
            KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "MngServer_InitInfo err\n");
    	    break;
        }
        printf("---服务器解码报文成功---\n");
        switch(pMsgKey_Req->cmdType)
        {
            case KeyMng_NEWorUPDATE: //密钥协商
                ret = MngServer_Agree(&svrInfo, pMsgKey_Req, &pResOutData, &resOutlen);
                break;
            case KeyMng_Check:  //密钥校验
                ret = MngServer_Check(&svrInfo, pMsgKey_Req, &pResOutData, &resOutlen);
                break;
            case KeyMng_Revoke: //密钥注销
                ret = MngServer_Revoke(&svrInfo,pMsgKey_Req, &pResOutData, &resOutlen);
                break;
            case 0:  //退出
                return 0;
            case 4:  //密钥查看
                break;
             default:
                printf("选项不支持，请选择正确的选项\n");
                break;     
        }
        if(ret != 0)
        {
            KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "MngServer_Agree err\n");
            break;
        }
LABLE: 
        //服务器端发送报文
        ret = sckServer_send(connfd, timeout, pResOutData, resOutlen);
        if(ret == Sck_ErrPeerClosed)
        {
            printf("---服务器检测到客户端已经关闭---\n");
            break;
        }
        else if(ret == Sck_ErrTimeOut)
        {
            printf("服务器端 sckServer_send 超时\n");
            goto LABLE;
        }
        else if(ret != 0)
        {
            printf("sckServer_send error:%d\n",ret);
            break;
        }

    }
    sckServer_close(connfd); //关闭连接
    
    //释放内存
    if(pReqOutData != NULL)
    {
        sck_FreeMem((void **)&pReqOutData);
    }
    if(pMsgKey_Req != NULL)
    {
        MsgMemFree((void **)&pMsgKey_Req,type);
    }
    if(pResOutData != NULL)
    {
        MsgMemFree((void **)&pResOutData,0);
    }
    return NULL;
}


int main()
{
    int ret = -1;
    int timeout = 3;
    int listenfd = -1;
    int connfd = -1;
    pthread_t tid;
    CREATE_DAEMON
    
    signal(SIGUSR1,sigcallback); //注册一个信号，用来接收守护进程结束，收到信号，守护进程不能马上结束，需要去做一些资源回收
    bzero(&svrInfo,sizeof(MngServer_Info));
    
    //初始化服务器 全局变量
    ret = MngServer_InitInfo(&svrInfo);
    if(ret != 0)
    {
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "MngServer_InitInfo err\n");
	    return ret;
    }
    printf("------服务器基础信息初始化完成\n");
    
    //服务器端 socket 初始化
    ret = sckServer_init(svrInfo.serverport, &listenfd);
    if(ret != 0)
    {
        printf("sckServer_init error :%d\n",ret);
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "sckServer_init err\n");
	    return ret;
    }
    printf("-----服务器端 socket 初始化完成\n");
    
    while(1)
    {
        if(flg == 1)
        {
            break;
        }
        printf("=====服务器端接收连接=====\n");
        //服务器端接收连接
        ret = sckServer_accept(listenfd, timeout, &connfd);
        
        if(ret == Sck_ErrTimeOut)
        {
            printf("=====客户端连接超时=====\n");
            continue;
        }
        else if(ret != 0)
        {
            printf("sckServer_accept error :%d\n",ret);
            KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "sckServer_accept err\n");
            break;
        }
        printf("服务器接收到连接成功\n");
        ret = pthread_create(&tid,NULL,subThread_callback,(void *)(intptr_t)connfd);
        if(ret != 0)
        {
            printf("pthread_create error: %d is %s\n",ret,strerror(ret));
            KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "pthread_create err\n");
            break;
        }
        printf("线程创建成功\n");
    }
    //服务器端环境释放 
    sckServer_destroy();
    printf("=====服务器自己销毁=====\n");
    return 0;
}