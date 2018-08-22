#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "keymngserverop.h"
#include "keymng_msg.h"
#include "keymnglog.h"
#include "poolsocket.h"
#include "myipc_shm.h"
#include "keymng_shmop.h"
#include "icdbapi.h"
#include "keymng_dbop.h"



//初始化服务器 全局变量
int MngServer_InitInfo(MngServer_Info *svrInfo)
{
    int ret = 0;
    strcpy(svrInfo->serverId,"0001");
    strcpy(svrInfo->dbuse,"SECMNG");
    strcpy(svrInfo->dbpasswd,"SECMNG");
    strcpy(svrInfo->dbsid,"orcl");
    strcpy(svrInfo->serverip,"127.0.0.1");
    svrInfo->dbpoolnum = 10;
    svrInfo->serverport = 8000;
    svrInfo->maxnode = 1;
    svrInfo->shmkey = 0x0001;
    svrInfo->shmhdl = -1;
    
    //初始化共享内存
    ret = KeyMng_ShmInit(svrInfo->shmkey, svrInfo->maxnode, &svrInfo->shmhdl);
    if(ret != 0)
    {
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "KeyMng_ShmInit err\n");
	    return ret;
    }
    printf("--------服务器 创建/打开共享内存完成\n");
    //初始化数据库连接池
    ret = IC_DBApi_PoolInit(svrInfo->dbpoolnum, svrInfo->dbsid, svrInfo->dbuse, svrInfo->dbpasswd);
    if(ret != 0)
    {
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "IC_DBApi_PoolInit err\n");
	    return ret;
    }
    printf("------服务器 连接池创建完成\n");
    return 0;
}

int MngServer_Agree(MngServer_Info *svrInfo, MsgKey_Req *msgkeyReq, unsigned char **outData, int *datalen)
{
    srand((unsigned int)time(NULL));
    int i = 0,ret = -1;
    MsgKey_Res msgKey_Res;
    NodeSHMInfo nodeSHMInfo;
    int seckeyid = 0;
    ICDBHandle handle = NULL;
    memset(&msgKey_Res,0,sizeof(MsgKey_Res));
    memset(&nodeSHMInfo,0,sizeof(NodeSHMInfo));
    printf("---进入MngServer_Agree---\n");
    printf("svrInfo->serverId=%s,msgkeyReq->serverId=%s\n",svrInfo->serverId,msgkeyReq->serverId);
    if(strcmp(svrInfo->serverId,msgkeyReq->serverId) != 0)
    {
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "客户端访问了错误的服务器\n");
	    return -1;
    }
    //生成随机值 Res- > r2 
    for(i=0;i<64;++i)
    {
        msgKey_Res.r2[i] = '1'+ rand() % 30;
    }
    printf("---生成随机值成功---\n");
    //组织应答结构体 Res
    msgKey_Res.rv = 0;  //0:成功  1:失败
    strcpy(msgKey_Res.clientId,msgkeyReq->clientId);
    strcpy(msgKey_Res.serverId,msgkeyReq->serverId);
     //msgKey_Res.seckeyid 来自于数据库
    
    //获取连接池中的一条连接
    ret = IC_DBApi_ConnGet(&handle, 0, 0);
    if(ret != 0)
    {
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "IC_DBApi_ConnGet err\n");
	    return ret;
    }
    printf("---获取一条连接IC_DBApi_ConnGet---\n");    
    
    //开启事务
    ret = IC_DBApi_BeginTran(handle);
    if(ret != 0)
    {
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "IC_DBApi_BeginTran err\n");
	    return ret;
    }
    printf("---开启事务成功---\n");
    //获取seckeyid
    ret = KeyMngsvr_DBOp_GenKeyID(handle, &seckeyid);
   // printf("---生成密钥结构体信息---%d\n",ret);
    if(ret != 0)
    {
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "KeyMngsvr_DBOp_GenKeyID err\n");
	    return ret;
    }
    msgKey_Res.seckeyid = seckeyid;
    //生成密钥结构体信息
    printf("---生成密钥结构体信息---\n");
    
    for(i=0;i<64;++i)
    {
        nodeSHMInfo.seckey[2*i] = msgkeyReq->r1[i];
        nodeSHMInfo.seckey[2*i+1] = msgKey_Res.r2[i];
    }
    
    //组织密钥信息结构体
    nodeSHMInfo.status = 0;
    nodeSHMInfo.seckeyid = msgKey_Res.seckeyid;
    strcpy(nodeSHMInfo.clientId,msgkeyReq->clientId);
    strcpy(nodeSHMInfo.serverId,svrInfo->serverId);
    printf("---生成密钥结构体信息成功---\n");
    //写共享内存  --- 使用当前密钥
    ret = KeyMng_ShmWrite(svrInfo->shmhdl, svrInfo->maxnode, &nodeSHMInfo);
   if(ret != 0)
    {
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "KeyMng_ShmWrite err\n");
	    return ret;
    }
    printf("-------服务器 密钥存储共享内存完成\n");
    
    //写数据库
    ret = KeyMngsvr_DBOp_WriteSecKey(handle, &nodeSHMInfo); 
    if(ret != 0)
    {
        IC_DBApi_Rollback(handle);  //插入失败，回滚
		if(ret==IC_DB_CONNECT_ERR)
		{
			IC_DBApi_ConnFree(handle, 0);
		}
		else
		{
			IC_DBApi_ConnFree(handle, 1);
		}
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "KeyMngsvr_DBOp_WriteSecKey err\n");
	    return ret;
    }
    
    //插入成功，提交，放回连接
    IC_DBApi_Commit(handle);
    IC_DBApi_ConnFree(handle, 1);
    

    //编码应答报文 传出
    ret = MsgEncode((void *)&msgKey_Res,ID_MsgKey_Res,outData,datalen);
	if(ret != 0)
    {
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "MsgEncode err\n");
	    return ret;
    }
    return 0;
}

//密钥校验
int MngServer_Check(MngServer_Info *svrInfo, MsgKey_Req *msgkeyReq, unsigned char **outData, int *datalen)
{
    int ret = -1;
    int i = 0;
    //1. 取出 Req 中的clientID/serverID
    char buf[10]={0};
	MsgKey_Res msgKey_Res;
	unsigned char *pRes_outdata = NULL;
    int pRes_outlen = -1;
    NodeSHMInfo pNodeInfo;
    memset(&msgKey_Res,0,sizeof(MsgKey_Res));
    memset(&pNodeInfo,0,sizeof(NodeSHMInfo));
    memset(buf,0,10);
    printf("-------服务器进行密钥校验\n");
	//2. 根据 clientID/serverID 读共享内存， 取出相应密钥前 10 字节。
	ret = KeyMng_ShmRead(svrInfo->shmhdl, msgkeyReq->clientId, msgkeyReq->serverId, svrInfo->maxnode, &pNodeInfo);
    if(ret != 0)
    {
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "KeyMng_ShmRead err\n");
	    return ret;
    }
    printf("-------服务器读共享内存成功\n");
    for(i=0;i<10;++i)
	{
	    buf[i] = pNodeInfo.seckey[i];
	}

	//3. 比对 --> 应答报文 Res.rv = 0/1
	if(strcmp(msgkeyReq->r1,buf)==0)
	{
	    msgKey_Res.rv = 0;
	}
	else
	{
	     msgKey_Res.rv = 1;
	}
	printf("-------服务器读比对报文成功\n");
	//4. 组织应答报文结构体 Res		r2不使用，清空。 seckeyid
	strcpy(msgKey_Res.clientId,msgkeyReq->clientId);
	strcpy(msgKey_Res.serverId,pNodeInfo.serverId);
	memset(msgKey_Res.r2,0,sizeof(msgKey_Res.r2));
    msgKey_Res.seckeyid = 0;
    printf("-------服务器组织应答报文成功\n");
	//5. 编码应答报文
	ret = MsgEncode((void *)&msgKey_Res,ID_MsgKey_Res,&pRes_outdata,&pRes_outlen);
	if(ret != 0)
    {
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "MsgEncode err\n");
	    return ret;
    }
    printf("-------服务器编码应答报文成功\n");
	//6. 传出。
	*outData = pRes_outdata;
	*datalen = pRes_outlen;
    return 0;
}


int KeyMngsvr_DBOp_UpdateSecKeyState(void *dbhdl, int seckeyid)
{
    int ret = 0;
    char mysql[2048] = {0};
    char optime[24] = {0};
    printf("进入KeyMngsvr_DBOp_UpdateSecKeyState函数成功\n");
    
    ret = IC_DBApi_GetDBTime(dbhdl, optime); //获取当前操作时间
    if(ret != 0)
    {
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "IC_DBApi_GetDBTime err\n");
	    goto END;
    }
    printf("获取当前操作时间成功\n");
    //开启事务
    ret = IC_DBApi_BeginTran(dbhdl);
    if(ret != 0)
    {
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "IC_DBApi_BeginTran err\n");
	    goto END;
    }
    printf("KeyMngsvr_DBOp_UpdateSecKeyState 开启事务成功\n");
    //组织oracle语句
    sprintf(mysql,"update SECMNG.seckyeinfo set state=1,createtime=to_date('%s','yyyy-mm-ddhh24:mi:ss') where keyid=%d",optime,seckeyid);
    //sprintf(mysql,"update SECMNG.seckyeinfo set state=1 where keyid=%d",seckeyid);
    printf("%s\n",mysql);
    ret = IC_DBApi_ExecNSelSql(dbhdl, mysql);
    printf("ret = %d",ret);
    if(ret != 0)
    {
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "IC_DBApi_ExecNSelSql err\n");
	    goto END;
    }
    printf("更新keyid成功\n"); 
END:
    return ret;
}

//密钥注销
int MngServer_Revoke(MngServer_Info *svrInfo, MsgKey_Req *msgkeyReq, unsigned char **outData, int *datalen)
{
    int ret = -1;
    //1. 取出 Req 中的clientID/serverID
	MsgKey_Res msgKey_Res;
	unsigned char *pRes_outdata = NULL;
    int pRes_outlen = -1;
    NodeSHMInfo pNodeInfo;
    ICDBHandle handle = NULL;
    int seckeyid = 0;
    memset(&msgKey_Res,0,sizeof(MsgKey_Res));
    memset(&pNodeInfo,0,sizeof(NodeSHMInfo));
    printf("-------服务器进行密钥注销\n");
    
    //1. 取出 Req 中的 clientID/serverID | 取出 r1 获取旧有密钥的编号。
    	
	//2. 根据 clientID/serverID 读共享内存， 取出 seckeyid。
	ret = KeyMng_ShmRead(svrInfo->shmhdl, msgkeyReq->clientId, msgkeyReq->serverId, svrInfo->maxnode, &pNodeInfo);
    if(ret != 0)
    {
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "KeyMng_ShmRead err\n");
	    return ret;
    }
 
    printf("-------密钥注销---服务器读共享内存成功\n");
    sscanf(msgkeyReq->r1,"%d",&seckeyid);
    //判断是否是旧密钥
    if(seckeyid != pNodeInfo.seckeyid)
    {
        printf("是旧密钥\n");
    }
 

	//3. server_init() 初始化连接池， 获取数据库连接， 
	//ret =  IC_DBApi_PoolInit(int bounds, char* dbName, char* dbUser, char* dbPswd);  //在MngServer_InitInfo 服务器初始化中已经初始化好了
	
	//获取连接池中的一条连接
    ret = IC_DBApi_ConnGet(&handle, 0, 0);
    if(ret != 0)
    {
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "IC_DBApi_ConnGet err\n");
	    return ret;
    }
    printf("---密钥注销---获取一条连接IC_DBApi_ConnGet---\n");    
    
    //开启事务
    ret = IC_DBApi_BeginTran(handle);
    if(ret != 0)
    {
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "IC_DBApi_BeginTran err\n");
	    return ret;
    }
    printf("---密钥注销---开启事务成功---\n");
    //获取seckeyid
  
	//4. 执行 update SECMNG.seckeyinfo set state=1 where keyid = 【seckeyid】

		//实现一个函数， 并且调用起来： KeyMngsvr_DBOp_UpdateSecKeyState(void *handle, int seckeyid);
    //5. 将调用结果 Res.rv = 0/1	注销成功与否
    ret = KeyMngsvr_DBOp_UpdateSecKeyState(handle, seckeyid);
    if(ret != 0)
    {
        IC_DBApi_Rollback(handle);  //更新失败，回滚
		if(ret==IC_DB_CONNECT_ERR)
		{
			IC_DBApi_ConnFree(handle, 0);
		}
		else
		{
			IC_DBApi_ConnFree(handle, 1);
		}
        msgKey_Res.rv = 1; //注销失败
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "KeyMngsvr_DBOp_UpdateSecKeyState err\n");
	    return ret;
    }
    else
    {
        IC_DBApi_Commit(handle);
        IC_DBApi_ConnFree(handle, 1);  //放回连接到连接池
    }
    printf("---密钥注销---更新数据库keyid成功---\n");
    
    //6. 组织应答报文结构体 Res		r2不使用，清空。 seckeyid
	//msgKey_Res.seckeyid = seckeyid;
	msgKey_Res.rv = 0;
	strcpy(msgKey_Res.clientId,pNodeInfo.clientId);
	strcpy(msgKey_Res.serverId,pNodeInfo.serverId);
	memset(msgKey_Res.r2,0,sizeof(msgKey_Res.r2));
	msgKey_Res.seckeyid = seckeyid;
	
    printf("---密钥注销---组织应答报文成功---\n");
	//7. 编码应答报文
	ret = MsgEncode((void *)&msgKey_Res,ID_MsgKey_Res,&pRes_outdata,&pRes_outlen);
    if(ret != 0)
    {
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "IC_DBApi_BeginTran err\n");
	    return ret;
    }
    printf("---密钥注销---编码应答成功---\n");
    
	//8. 传出。
	*outData = pRes_outdata;
	*datalen = pRes_outlen;
    return 0;
}

