#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "keymngclientop.h"
#include "poolsocket.h"
#include "keymng_msg.h"
#include "keymnglog.h"
#include "myipc_shm.h"
#include "keymng_shmop.h"




//初始化客户端 全局变量
int MngClient_InitInfo(MngClient_Info *pCltInfo)
{
    int ret = 0;
    strcpy(pCltInfo->clientId,"1111");
    strcpy(pCltInfo->AuthCode,"key");
    strcpy(pCltInfo->serverId,"0001");
    strcpy(pCltInfo->serverip,"127.0.0.1");
    pCltInfo->serverport = 8000;
    pCltInfo->maxnode = 1;
    pCltInfo->shmkey = 0x1111;
    pCltInfo->shmhdl = -1;
    ret =KeyMng_ShmInit(pCltInfo->shmkey, pCltInfo->maxnode, &pCltInfo->shmhdl);
    if(ret != 0)
	{
	     KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "KeyMng_ShmInit err\n");
	     return ret;
	}
	printf("---------客户端 创建/打开共享内存完成\n");
    return 0;
}

//密钥协商
int MngClient_Agree(MngClient_Info *pCltInfo)
{
    srand((unsigned int)time(NULL));
    MsgKey_Req msgKey_Req;
    //存储密钥请求的报文
    unsigned char *pRed_outdata = NULL;
    int pRed_outlen = -1;
    //存储密钥应答的报文
    unsigned char *pRes_outdata = NULL;
    int pRes_outlen = -1;
    MsgKey_Res * msgKey_Res = NULL;
	int type=0;
    
    int timeout = 3;
    int connfd = -1;
    int i = 0;
    
    int ret = 0;
LABLE1:
    //生成随机数 r1  srand(),调试阶段先不弄随机数
    for(i=0; i<64; ++i)
    {
        msgKey_Req.r1[i] = '1'+ rand() % 20;  //abcdef......
    }
    //编码密钥请求结构体 --> TLV
    msgKey_Req.cmdType = KeyMng_NEWorUPDATE;
    strcpy(msgKey_Req.clientId,pCltInfo->clientId);
    strcpy(msgKey_Req.AuthCode,pCltInfo->AuthCode);
    strcpy(msgKey_Req.serverId,pCltInfo->serverId);
    
    ret = MsgEncode((void *)&msgKey_Req,ID_MsgKey_Req,&pRed_outdata,&pRed_outlen);
	if(ret != 0)
	{
	     KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "MsgEncode err\n");
	     goto END;
	}
 
    //初始化连接 init
    ret =sckClient_init();
    if(ret != 0)
	{
	    KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "sckClient_init err\n");
	    goto END;
	}
		
	//建立连接 connect
	ret = sckClient_connect(pCltInfo->serverip, pCltInfo->serverport, timeout, &connfd);
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4], ret, "sckClient_connect err\n");
		goto END;	
	}
    //发送请求报文。
    ret = sckClient_send(connfd, timeout, pRed_outdata, pRed_outlen);
    if(ret != 0)
	{
	     KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "sckClient_send err\n");
	     goto END;
	}
    // --------- 等待服务器 操作，回发 应答报文。

	//接收应答报文。  TLV
	ret = sckClient_rev(connfd, timeout, &pRes_outdata, &pRes_outlen);
	if(ret != 0)
	{
	     KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "sckClient_rev err\n");
	     goto END;
	}

	//解码密钥应答报文 ---> Res {  }
	ret = MsgDecode(pRes_outdata,pRes_outlen,(void **)&msgKey_Res,&type);
	if(ret != 0)
	{
	     KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "MsgDecode err\n");
	     goto END;
	}

	//判断 rv 结果 ，进行后续操作。  成功：0 失败：1
	if(msgKey_Res->rv != 0)
	{
	     KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[2],ret, "密钥协商失败，重新协商\n");
	     goto LABLE1;
	}
	printf("--------新密钥编号：%d\n",msgKey_Res->seckeyid);
	//--取出Res->r2 结合 r1 生成密钥
	NodeSHMInfo nodeSHMInfo;
	for(i=0;i<64;++i)
	{
	    nodeSHMInfo.seckey[2*i] = msgKey_Req.r1[i];
	    nodeSHMInfo.seckey[2*i + 1] = msgKey_Res->r2[i];
	}
	
	//--写共享内存。
	nodeSHMInfo.status = 0;
	strcpy(nodeSHMInfo.clientId,msgKey_Req.clientId);
	strcpy(nodeSHMInfo.serverId,msgKey_Res->serverId);
	nodeSHMInfo.seckeyid = msgKey_Res->seckeyid;
	ret =KeyMng_ShmWrite(pCltInfo->shmhdl, pCltInfo->maxnode, &nodeSHMInfo);
    if(ret != 0)
	{
	     KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "KeyMng_ShmWrite err\n");
	     goto END;
	}
	printf("-------客户端 密钥存储共享内存完成\n");
	printf("-------密钥协商成功！！!\n");
	
END:  //释放内存
    if(pRed_outdata != NULL)
    {
        MsgMemFree((void **)&pRed_outdata,0);
    }
    if(msgKey_Res != NULL)
    {
        MsgMemFree((void **)&msgKey_Res,type);
    }
    if(pRes_outdata != NULL)
    {
       sck_FreeMem((void **)&pRes_outdata);
    }
	return ret;
}

//2 密钥校验
int MngClient_Check(MngClient_Info *pCltInfo)
{
    printf("-------客户端进行密钥校验\n");
    NodeSHMInfo pNodeInfo;
    int i=0;
    int ret = -1;
    char buf[10]={0};
    MsgKey_Req msgKey_Req;
    MsgKey_Res *msgKey_Res= NULL;
    int type = -1;
    unsigned char *pRed_outdata = NULL;
    int pRed_outlen = -1;
    //存储密钥应答的报文
    unsigned char *pRes_outdata = NULL;
    int pRes_outlen = -1;
    
    int timeout = 3;
    int connfd = -1;
    memset(&msgKey_Req,0,sizeof(MsgKey_Req));
    memset(buf,0,10);
    //1. 读共享内存， 取出密钥前10字节。	KeyMng_ShmRead();
    
    ret = KeyMng_ShmRead(pCltInfo->shmhdl, pCltInfo->clientId, pCltInfo->serverId,  pCltInfo->maxnode, &pNodeInfo);
    if(ret != 0)
	{
	     KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "KeyMng_ShmRead err\n");
	     goto END;
	}
    printf("-------读共享内存成功\n"); 
	//2. 组织请求报文 { cmdType = KeyMng_Check， r1=前10字节密钥}
	for(i=0;i<10;++i)
	{
	    buf[i] = pNodeInfo.seckey[i];
	}
	
	msgKey_Req.cmdType = KeyMng_Check;
	strcpy(msgKey_Req.clientId,pNodeInfo.clientId);
	strcpy(msgKey_Req.serverId,pNodeInfo.serverId);
	strcpy(msgKey_Req.AuthCode,pCltInfo->AuthCode);
	memset(&msgKey_Req.r1,0,sizeof(msgKey_Req.r1));
	strncpy(msgKey_Req.r1,buf,10);
	//strncpy(msgKey_Req.r1,(char *)pNodeInfo.seckey,10);	
    printf("-------组织请求报文成功\n"); 
    //printf("msgKey_Req.r1=%s\n",msgKey_Req.r1);
    
	//3. 编码请求报文  --- Req TLV
	ret = MsgEncode((void *)&msgKey_Req,ID_MsgKey_Req,&pRed_outdata,&pRed_outlen);
    if(ret != 0)
	{
	     KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "MsgEncode err\n");
	     goto END;
	}
    printf("-------编码请求报文成功\n");
	//4. 初始化、建立连接、发送请求报文。
	ret = sckClient_init();
    if(ret != 0)
	{
	     KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "sckClient_init err\n");
	     goto END;
	}
	printf("-------初始化连接成功\n");
	//客户端 连接服务器
    ret =sckClient_connect(pCltInfo->serverip,pCltInfo->serverport, timeout, &connfd);
    if(ret != 0)
	{
	     KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "sckClient_connect err\n");
	     goto END;
	}
	printf("-------连接服务器成功\n");
    //客户端 发送报文
    ret = sckClient_send(connfd, timeout, pRed_outdata, pRed_outlen);
    if(ret != 0)
	{
	     KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "sckClient_send err\n");
	     goto END;
	}
    printf("-------客户端 发送报文成功\n");
	//5. 接收应答报文 --- Res TLV
	//客户端 接受报文
    ret = sckClient_rev(connfd, timeout, &pRes_outdata, &pRes_outlen); 
    if(ret != 0)
	{
	     KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "sckClient_rev err\n");
	     goto END;
	}
    printf("-------客户端 接受报文成功\n");
	//6. 解码应答报文
	ret = MsgDecode(pRes_outdata,pRes_outlen,(void **)&msgKey_Res,&type);
	printf("-------ret = %d\n",ret);
    if(ret != 0)
	{
	     KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "sckClient_rev err\n");
	     goto END;
	}
	printf("-------客户端 解码应答报文成功\n");
	//7. 根据rv 显示校验结果。
	if(msgKey_Res->rv == 0)
	{
	    printf("密钥校验OK，密钥一致！\n");
	}
END:  //释放内存
    if(pRed_outdata != NULL)
    {
        MsgMemFree((void **)&pRed_outdata,0);
    }
    if(msgKey_Res != NULL)
    {
        MsgMemFree((void **)&msgKey_Res,type);
    }
    if(pRes_outdata != NULL)
    {
       sck_FreeMem((void **)&pRes_outdata);
    }
    return ret;
}


//密钥注销
int MngClient_Revoke(MngClient_Info *pCltInfo)
{
    printf("-------客户端进行密钥注销\n");
    NodeSHMInfo pNodeInfo;
    int ret = -1;
    MsgKey_Req msgKey_Req;
    MsgKey_Res *msgKey_Res= NULL;
    int type = -1;
    unsigned char *pRed_outdata = NULL;
    int pRed_outlen = -1;
    //存储密钥应答的报文
    unsigned char *pRes_outdata = NULL;
    int pRes_outlen = -1;
    int keyid = 0;
    
    int timeout = 3;
    int connfd = -1;
    memset(&msgKey_Req,0,sizeof(MsgKey_Req));
    memset(&msgKey_Req.r1,0,sizeof(msgKey_Req.r1));
    
    //1. 读共享内存， 取出密钥编号	KeyMng_ShmRead();
    ret = KeyMng_ShmRead(pCltInfo->shmhdl, pCltInfo->clientId, pCltInfo->serverId,  pCltInfo->maxnode, &pNodeInfo);
    if(ret != 0)
	{
	     KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "KeyMng_ShmRead err\n");
	     goto END;
	}
    printf("-------读共享内存成功\n"); 
	//2. 组织请求报文 { cmdType = KeyMng_Revoke， r1 = 旧的seckeyid }
	msgKey_Req.cmdType = KeyMng_Revoke;
	keyid = pNodeInfo.seckeyid;
	memcpy(msgKey_Req.clientId,pNodeInfo.clientId,sizeof(pNodeInfo.clientId));
	memcpy(msgKey_Req.serverId,pNodeInfo.serverId,sizeof(pNodeInfo.serverId));
	memcpy(msgKey_Req.AuthCode,pCltInfo->AuthCode,sizeof(pCltInfo->AuthCode));
	sprintf(msgKey_Req.r1,"%d",keyid); //注销当前的密钥  seckeyid
	//sprintf(msgKey_Req.r1,"%d",keyid);  //注销1号keyid 并更新注销时间
	
	printf("msgKey_Req.r1=%s,pNodeInfo.seckeyid = %d\n",msgKey_Req.r1,pNodeInfo.seckeyid);

    printf("-------组织请求报文成功\n"); 
	//3. 编码请求报文  --- Req TLV
	ret = MsgEncode((void *)&msgKey_Req,ID_MsgKey_Req,&pRed_outdata,&pRed_outlen);
	if(ret != 0)
	{
	     KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "MsgEncode err\n");
	     goto END;
	}
    printf("-------编码请求报文成功\n");

	//4. 初始化、建立连接、发送请求报文。
	
	ret = sckClient_init();
    if(ret != 0)
	{
	     KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "sckClient_init err\n");
	     goto END;
	}
	printf("-------初始化连接成功\n");
	//客户端 连接服务器
    ret =sckClient_connect(pCltInfo->serverip,pCltInfo->serverport, timeout, &connfd);
    if(ret != 0)
	{
	     KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "sckClient_connect err\n");
	     goto END;
	}
	printf("-------连接服务器成功\n");
    //客户端 发送报文
    ret = sckClient_send(connfd, timeout, pRed_outdata, pRed_outlen);
    if(ret != 0)
	{
	     KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "sckClient_send err\n");
	     goto END;
	}
    printf("-------客户端 发送报文成功\n");

	//5. 接收应答报文 --- Res TLV
	ret = sckClient_rev(connfd, timeout, &pRes_outdata, &pRes_outlen); 
    if(ret != 0)
	{
	     KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "sckClient_rev err\n");
	     goto END;
	}
    printf("-------客户端 接受报文成功\n");
	//6. 解码应答报文
	ret = MsgDecode(pRes_outdata,pRes_outlen,(void **)&msgKey_Res,&type);
	printf("-------ret = %d\n",ret);
    if(ret != 0)
	{
	     KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "sckClient_rev err\n");
	     goto END;
	}
	printf("-------客户端 解码应答报文成功\n");
	//7. 根据rv 显示校验结果。
	if(msgKey_Res->rv == 0)
	{
	    printf("客户端：%s,keyid = %d密钥注销成功,数据库更新了注销时间！\n",msgKey_Req.clientId,keyid);
	}
END:  //释放内存
    if(pRed_outdata != NULL)
    {
        MsgMemFree((void **)&pRed_outdata,0);
    }
    if(msgKey_Res != NULL)
    {
        MsgMemFree((void **)&msgKey_Res,type);
    }
    if(pRes_outdata != NULL)
    {
       sck_FreeMem((void **)&pRes_outdata);
    }
    return ret;
}

//密钥查看
int MngClient_view(MngClient_Info *pCltInfo)
{
    int ret = 0;
    printf("-------客户端进行密钥查看\n");
    NodeSHMInfo pNodeInfo;
    
    //1. 读共享内存， 取出密钥编号	KeyMng_ShmRead();
    ret = KeyMng_ShmRead(pCltInfo->shmhdl, pCltInfo->clientId, pCltInfo->serverId,  pCltInfo->maxnode, &pNodeInfo);
    if(ret != 0)
	{
	     KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "KeyMng_ShmRead err\n");
	     return ret;
	}
	printf("当前客户端：%s  密钥是：%s\n",pCltInfo->clientId,pNodeInfo.seckey);
    return 0;
}



//退出
int MngClient_Quit(MngClient_Info *pCltInfo)
{
    printf("-------客户端已退出\n");
    return 0;
}