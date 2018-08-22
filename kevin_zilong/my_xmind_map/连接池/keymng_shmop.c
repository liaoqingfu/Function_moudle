#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>


#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "keymnglog.h"
#include "keymng_shmop.h"
#include "myipc_shm.h"

#include "keymngclientop.h"


//打开共享内存 共享内存存在则使用 不存在则创建
int KeyMng_ShmInit(int key, int maxnodenum, int *shmhdl)
{
    int ret = 0;
    ret = IPC_OpenShm(key, maxnodenum*sizeof(NodeSHMInfo), shmhdl);
    if(ret == MYIPC_NotEXISTErr)
    {
        printf("keymng 检测到共享内存不存在，正在创建共享内存.....\n");
        ret = IPC_CreatShm(key, maxnodenum*sizeof(NodeSHMInfo), shmhdl);
        if(ret != 0)
        {
            printf("keymng 创建共享内存 err : %d\n",ret);
            return ret;
        }
        else
        {
            void *mapaddr = NULL;
            printf("keymng 创建共享内存OK ...\n");
            ret = IPC_MapShm((int)*shmhdl, (void **)&mapaddr);  //关联共享内存
            if(ret != 0)
            {
                printf("共享内存关联失败 IPC_MapShm err:%d\n",ret);
                return ret;
            }
            memset(mapaddr,0,maxnodenum*sizeof(NodeSHMInfo));
            printf("keymng 清空共享内存OK\n");
        }
    }
    else if(ret == 0)
    {
        printf("keymng 检测到共享内存存在，使用旧的共享内存...\n");
    }
    else 
    {
        printf("IPC_OpenShm err\n");
    }
    return ret;
}

int KeyMng_ShmWrite(int shmhdl, int maxnodenum, NodeSHMInfo *pNodeInfo)
{
    int ret = 0,i = 0;
    NodeSHMInfo tmpNodeSHMInfo;
    NodeSHMInfo *pNode = NULL;
    void *mapaddr = NULL;
    memset(&tmpNodeSHMInfo,0,sizeof(tmpNodeSHMInfo));
    ret = IPC_MapShm(shmhdl, &mapaddr);  //关联共享内存
    if(ret != 0)
    {
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "IPC_MapShm err\n");
	    goto END;
    }
    //判断传入的网点密钥是否已经存在
    for(i=0;i<maxnodenum;++i)  //存在
    {
        pNode = mapaddr + sizeof(NodeSHMInfo)*i;
        if(strcmp(pNode->clientId,pNodeInfo->clientId)==0 && strcmp(pNode->serverId,pNodeInfo->serverId)==0)
        {
            KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[3],ret, "系统检测到共享内存中已经存在网点信息  clientId:%s,serverId:%s\n",pNode->clientId,pNode->serverId);
            memcpy(pNode,pNodeInfo,sizeof(NodeSHMInfo));
	        goto END;
        }
    }
    
    for(i=0;i<maxnodenum;++i)  //不存在
    {
        pNode = mapaddr + sizeof(NodeSHMInfo)*i;
        if(memcmp(&tmpNodeSHMInfo,pNode,sizeof(NodeSHMInfo)) ==0)
        {
            KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[3],ret, "系统检测到共享内存中有一个空位置\n");
            memcpy(pNode,pNodeInfo,sizeof(NodeSHMInfo));
	        goto END;
        }
    }
    if(i == maxnodenum)
    {
        ret = 1111;
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "系统检测到共享内存已满\n");
        goto END;
          
    }
END:
    IPC_UnMapShm(mapaddr);  //取消关联内存
    return ret;
}


int KeyMng_ShmRead(int shmhdl, char *clientId, char *serverId,  int maxnodenum, NodeSHMInfo *pNodeInfo)
{
    int ret = 0,i = 0;
    NodeSHMInfo tmpNodeSHMInfo;
    NodeSHMInfo *pNode = NULL;
    void *mapaddr = NULL;
    memset(&tmpNodeSHMInfo,0,sizeof(tmpNodeSHMInfo));
    ret = IPC_MapShm(shmhdl, &mapaddr);  //关联共享内存
    if(ret != 0)
    {
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "IPC_MapShm err\n");
	    goto END;
    }
    
    for(i=0;i<maxnodenum;++i)  //取出共享内存数据
    {
        pNode = mapaddr + sizeof(NodeSHMInfo)*i;
        if(strcmp(pNode->clientId,clientId)==0 && strcmp(pNode->serverId,serverId)==0)
        {
            KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[3],ret, "系统检测到共享内存有一个空位置\n");
            memcpy(pNodeInfo,pNode,sizeof(NodeSHMInfo));
	        goto END;
        }
    }
    if(i == maxnodenum)
    {
        ret = 1111;
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "系统检测到共享内存已满\n");
        goto END;
          
    }
END:
    IPC_UnMapShm(mapaddr);  //取消关联内存
    return ret;
}
