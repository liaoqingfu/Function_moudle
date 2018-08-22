
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "keymnglog.h"
#include "keymngclientop.h"
#include "keymng_msg.h"


int main1()
{
    KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[1],1, "%S", "1====IC_DEBUG_LEVEL");
    KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[2],2, "%S", "1====IC_DEBUG_LEVEL");
    KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[3],3, "%S", "1====IC_DEBUG_LEVEL");
    KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],4, "%S", "1====IC_DEBUG_LEVEL");
    return 0;
}

int Usage() //显示菜单
{
    int nSel = -1;
    system("clear");
    printf("\n  /*************************************************************/");
    printf("\n  /*************************************************************/");
    printf("\n  /*     1.密钥协商                                            */");
    printf("\n  /*     2.密钥校验                                            */");
    printf("\n  /*     3.密钥注销                                            */");
    printf("\n  /*     4.密钥查看                                            */");
    printf("\n  /*     0.退出系统                                            */");
    printf("\n  /*************************************************************/");
    printf("\n  /*************************************************************/");
    printf("\n\n 请选择：");
    scanf("%d",&nSel);
    while(getchar() != '\n');   //把应用程序io缓冲器的所有的数据 都读走,避免影响下一次 输入
    return nSel;
}

int main()
{
    int ret = 0;
    int  nSel = 0;
    MngClient_Info mngClient_Info;
    memset(&mngClient_Info,0,sizeof(mngClient_Info));
    //初始化客户端 全局变量
    ret = MngClient_InitInfo(&mngClient_Info);
    if(ret != 0)
    {
        KeyMng_Log(__FILE__, __LINE__, KeyMngLevel[4],ret, "MngClient_InitInfo err\n");
	    return ret;
    }
    while(1)
    {
        //显示菜单，接收用户选择
        nSel = Usage();
        switch(nSel)
        {
            case KeyMng_NEWorUPDATE: //密钥协商
                ret = MngClient_Agree(&mngClient_Info);
                break;
            case KeyMng_Check:  //密钥校验
                ret = MngClient_Check(&mngClient_Info);
                break;
            case KeyMng_Revoke: //密钥注销
                ret = MngClient_Revoke(&mngClient_Info);
                break;
            case 0:  //退出
                ret = MngClient_Quit(&mngClient_Info);
                return 0;
            case 4:  //密钥查看
                ret = MngClient_view(&mngClient_Info);
                break;
             default:
                printf("选项不支持，请选择正确的选项\n");
                break;     
        }
        if(ret)
        {
            printf("\n!!!!!!!!!!ERROR!!!!!!!!!!!!!");
            printf("\n错误码是:%x\n",ret);
        }
        else
        {
            printf("\n!!!!!!!!!!!!SUCCESS!!!!!!!!!!!!!!!!1\n");
        }         
      printf("请按 Enter 键进行下一次操作: ");  
      getchar();       
    }
    return 0;
}

