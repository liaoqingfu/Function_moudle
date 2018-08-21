/*============================================================================
 Name        : ota_interface.c
 Author      : oceanwing.com
 Copyright   : 2018(c) Shenzhen Oceanwing Smart Innovations Co., Ltd.
 Create      : brady.xiao
 Date        : 2018-01-31 
 Description : 
 ============================================================================*/

#include <stdio.h>  
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>  
#include <string.h>  
#include <netinet/in.h>  
#include <netdb.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <signal.h>
#include <sys/time.h>
#include <pthread.h>
#include <errno.h>

#include "ota_interface.h"
#include "http.h"
#include "cjson.h"
#include "cloud_storage_interface.h"
#include "md5.h"
#include "as_interface.h"
//#include "comm_protocol_define.h"
//#include "sys_interface.h"



int Assignment_value(zx_Json *res_root,VERSION_UPGRADE *upgrade)
{
	char * tmp = NULL;
	char *s_result = NULL;
	zx_Json *upgrade_data = res_root;
	
#if OTA_DEBUG_LOG
	dzlog_info("****** Assignment value start******");
#endif
	if(!res_root || !upgrade)
	{
		dzlog_warn("The function parameter is NULL");
	}

	upgrade->force_upgrade = zx_Json_GetInt(upgrade_data, "force_upgrade", 0);
	upgrade->upgrade_flag = zx_Json_GetInt(upgrade_data, "upgrade_flag", 0);
	upgrade->upgrade_type = zx_Json_GetInt(upgrade_data, "upgrade_type", 0);
	upgrade->rom_version = zx_Json_GetInt(upgrade_data, "rom_version", 0);
	upgrade->device_type = zx_MyJson_GetString(upgrade_data, "device_type", "");
	
	if(!upgrade->device_type )
	{
		dzlog_warn("upgrade->device_type is NULL");
		return -1;
	}
	
	upgrade->introduction = zx_MyJson_GetString(upgrade_data, "introduction", "");
	
	if(!upgrade->introduction )
	{
		dzlog_warn("upgrade->introduction is NULL");
		return -1;
	}
	
	upgrade->rom_version_name = zx_MyJson_GetString(upgrade_data, "rom_version_name", "");
	
	if(!upgrade->rom_version_name )
	{
		dzlog_warn("upgrade->rom_version_name is NULL");
		return -1;
	}
	
	upgrade_data = cJSON_GetObjectItem(upgrade_data, "full_package");

	if( strcmp(cJSON_PrintUnformatted(upgrade_data),"null") == 0 || !upgrade_data )
	{
		if(cJSON_GetObjectItem(res_root,"children"))
		{
			dzlog_info("cJSON_GetObjectItem full_package is empty buf can Parsing the child");
			return 0;
		}
		
		dzlog_error("cJSON_GetObjectItem full_package is NULL");
		return -1;
	}
	
#if OTA_DEBUG_LOG
	s_result = cJSON_PrintUnformatted(upgrade_data);
	
	if(strcmp(s_result,"null") == 0 || !s_result)
	{
		if(cJSON_GetObjectItem(res_root,"children"))
		{
			dzlog_info("cJSON_GetObjectItem full_package is empty buf can Parsing the child");
			return 0;
		}
		
		dzlog_error("cJSON_PrintUnformatted s_result is NULL");
		return -1;
	}

	dzlog_info("****** s_result = %s******",s_result);

	if(s_result) 
	{
		free(s_result);
		s_result = NULL;
	}
	
#endif

	upgrade->down_url = zx_MyJson_GetString(upgrade_data, "file_path", "");
	
	if(!upgrade->down_url )
	{
		dzlog_warn("upgrade->down_url is NULL");
		return -1;
	}
	
#if OTA_DEBUG_LOG
	dzlog_info("****** version_upgrade->down_url = %s******",upgrade->down_url);
#endif

	zx_urldecode(upgrade->down_url); //url解码
	
#if OTA_DEBUG_LOG
	dzlog_info("****** version_upgrade->down_url = %s******",upgrade->down_url);
#endif
	
	s_result = zx_MyJson_GetString(upgrade_data, "file_name", "");
	
	if(!s_result )
	{
		dzlog_warn("upgrade->file_name is NULL");
		return -1;
	}
	
#if OTA_DEBUG_LOG
	dzlog_info("****** version_upgrade->file_name = %s******",s_result );
#endif

	zx_urldecode(s_result ); //url解码
	
#if OTA_DEBUG_LOG
	dzlog_info("****** file name decode:%s******",s_result );
#endif
	
	tmp = strrchr( s_result, '/' ); 
	
	if( tmp )
	{
		tmp = tmp + 1;  // +1 去掉左斜杆,取后面的名字
		char *filename = (char *)malloc( strlen( tmp ) + 2 );
		if( !filename )
		{
			dzlog_error(" out of memory for filename. ");
			free( s_result ); 
			s_result = NULL;
			return -1;
		}

		memset( filename, 0, strlen( tmp ) + 2  );
		strncpy( filename, tmp, strlen( tmp ) );
		upgrade->file_name = filename;

		free( s_result ); 
		s_result = NULL;

	}
	else
	{
		upgrade->file_name = s_result;
		s_result = NULL;
	}
	
#if OTA_DEBUG_LOG
	dzlog_info("****** version_upgrade->file_name = %s******",upgrade->file_name);
#endif

	upgrade->file_md5 = zx_MyJson_GetString(upgrade_data, "file_md5", "");

	if(!upgrade->file_md5 )
	{
		dzlog_error("upgrade->file_md5 is NULL");
		return -1;
	}
#if OTA_DEBUG_LOG
	dzlog_info("****** file md5 = %s******",upgrade->file_md5);
#endif

	upgrade->file_size = zx_Json_GetInt(upgrade_data, "file_size", 0);

#if OTA_DEBUG_LOG
	dzlog_info("****** file size = %lu******",upgrade->file_size);
#endif

#if OTA_DEBUG_LOG
	dzlog_info("******Assignment value end******");
#endif
	return 0;
}


int Compare_file_md5(const char *file_path, char *md5_str)  
{  
	int i = 0;  
    FILE* fd = NULL;
    int ret = -1;  
    unsigned char data[READ_OTA_DATA_SIZE + 2]={0};  
    unsigned char md5_value[MD5_OTA_SIZE + 2]={0};  
    MD5_CTX md5; 
	
	if( !file_path || !md5_str )
	{
		dzlog_warn("The function parameter  is NULL");
		return -1;
	}
	
    fd = fopen(file_path, "r"); 
	if( !fd )
	{
		dzlog_error(" open path error:%s",file_path);  
        return -1;
	}

    MD5Init(&md5);  // init md5
    while (1)  
    {  
        ret = fread(data, 1,READ_OTA_DATA_SIZE,fd); 
		
        if (-1 == ret)  
        {  
            dzlog_error(" read error "); 
            return ret;  
        }  

		if (0 == ret)  
        {  
			//dzlog_info(" read file is zero. "); 
            break;  
        } 
		
        MD5Update(&md5, data, ret); 
		
 #if 0
        if (0 == ret || ret < READ_OTA_DATA_SIZE)  
        {  
            break;  
        } 
#endif
    }  
  
    fclose(fd); 

    MD5Final(&md5, md5_value); 
  
    for(i = 0; i < MD5_OTA_SIZE; i++)  
    {  
        snprintf(md5_str + i*2, 2+1, "%02x", md5_value[i]);  
    }  
    md5_str[MD5_OTA_SIZE*2] = '\0'; // add end  
    return 0;
}



int zx_ota_system(const char * cmdstring)
{
    pid_t pid;
    int status = 0;
 
	if(!cmdstring )
	{
		dzlog_warn("The function parameter  is NULL");
	    return -1; //如果cmdstring为空，返回非零值
	}
	
	if((pid = fork())<0)
	{
	    status = -1; //fork失败，返回-1
	}
	else if(pid == 0)
	{
	    execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
	    _exit(127); // exec执行失败返回127
	}
	else //父进程
	{
	    while(waitpid(pid, &status, 0) < 0)
	    {
	        if(errno != EINTR)
	        {
	            status = -1; //如果waitpid被信号中断，则返回-1
	            break;
	        }
	    }
	}
    return status; //如果waitpid成功，则返回子进程的返回状态
}





VERSION_UPGRADE *Upgrade_Info_ListCreat()
{
	VERSION_UPGRADE *head = NULL;
	head = (VERSION_UPGRADE *)malloc(sizeof(VERSION_UPGRADE));
	if ( !head )
	{
		dzlog_error("Upgrade_Info_ListCreat malloc is error");
		return NULL;
	}
	memset(head,0,sizeof(VERSION_UPGRADE));

	//给成员变量赋初值
	head->device_type = NULL;
	head->down_url = NULL;
	head->file_md5 = NULL;
	head->file_name = NULL;
	head->file_size = 0;
	head->force_upgrade = -1;
	head->introduction = NULL;
	head->rom_version = 0;
	head->rom_version_name =NULL;
	head->upgrade_flag = 0;
	head->upgrade_type = 0;
	head->next = NULL;

	return head;
}


int Upgrade_Info_ListInsert(VERSION_UPGRADE * head, cJSON *upgrade_data)
{
	int ret = -1;
	if ( !head || !upgrade_data)
	{
		dzlog_warn("The function parameter  is NULL");
		return ret;
	}

	VERSION_UPGRADE *pPre = head;
	
	while(pPre->next) //查找可以插入的节点
	{
		pPre = pPre->next;
	}

	VERSION_UPGRADE *pNew = (VERSION_UPGRADE *)malloc(sizeof(VERSION_UPGRADE));  //给新结点分配新空间
	if ( !pNew )
	{
		dzlog_error("malloc error");
		return ret;
	}
	memset(pNew,0,sizeof(VERSION_UPGRADE));

	ret = Assignment_value(upgrade_data,pNew); //给pNew的成员变量赋值
	
	if(ret == -1)
	{
		dzlog_error("Assignment value error");
		return ret;
	}

	pPre->next = pNew; //尾插
	pNew->next = NULL; 

	return ret;
}

VERSION_UPGRADE *get_device_type_info(VERSION_UPGRADE *pPre,const char *device_type)
{
	VERSION_UPGRADE *pCur = pPre;
	
	if(!pPre || !device_type)
	{
		dzlog_warn("The function parameter  is NULL");
		return NULL;
	}
	
	while(pCur)
	{
		if(strcmp(device_type,pCur->device_type) == 0)
		{
			break;
		}
		
		pCur = pCur->next;
	}
	
	if(!pCur)
	{
		dzlog_warn("no could device_type: %s", device_type);
		return NULL;
	}

	return pCur;
}

int Upgrade_Info_ListPrint(VERSION_UPGRADE * pPre, const char *device_type)
{
	VERSION_UPGRADE *pCur = pPre;
	
	if ( !pPre )
	{
		dzlog_warn("The function parameter is NULL");
		return -1;
	}

	if( strlen(device_type) > 0 )
	{
		while(strcmp(pCur->device_type,device_type) != 0) //查找需要打印的设备版本信息
		{
			pCur = pCur->next;

			if(!pCur)
			{
				dzlog_error("There is no device_type you want.");
				return -1;
			}
		}
	}

	while (1)
	{
		dzlog_info("******Upgrade Info ListPrint data ******");
		
		dzlog_info("pCur->force_upgrade = %d  ", pCur->force_upgrade);
		dzlog_info("pCur->upgrade_flag = %d  ", pCur->upgrade_flag);
		dzlog_info("pCur->upgrade_type = %d  ", pCur->upgrade_type);
		dzlog_info("pCur->rom_version = %d  ", pCur->rom_version);
		dzlog_info("pCur->file_size = %lu  ", pCur->file_size);
		
		if(pCur->device_type)
			dzlog_info("pCur->device_type = %s  ", pCur->device_type);
		else
			dzlog_info("pCur->device_type  = empty");
		
		if(pCur->introduction)
			dzlog_info("pCur->introduction = %s  ", pCur->introduction);
		else
			dzlog_info("pCur->introduction  = empty");
		
		if(pCur->down_url)
			dzlog_info("pCur->down_url = %s  ", pCur->down_url);
		else
			dzlog_info("pCur->down_url  = empty");
		
		if(pCur->file_name)
			dzlog_info("pCur->file_name = %s  ", pCur->file_name);
		else
			dzlog_info("pCur->file_name  = empty");
		
		if(pCur->rom_version_name)
			dzlog_info("pCur->rom_version_name = %s  ", pCur->rom_version_name);
		else
			dzlog_info("pCur->rom_version_name  = empty");
		
		if(pCur->file_md5)
			dzlog_info("pCur->file_md5 = %s  ", pCur->file_md5);
		else
			dzlog_info("pCur->file_md5  = empty");
		
		if(strlen(device_type) > 0)
		{
			if(pCur->down_url)  //判断是否打印当前一个还是全部的信息
			{
				break;
			}
		}

		pCur = pCur->next;

		if(!pCur)
		{
			dzlog_info("is end ");
			break;
		}
		
	}

	return 0;
}



VERSION_UPGRADE* zx_get_all_upgrade_version_info(
	const char *current_version_name,   //当前对应 device_type 的版本名称
	const char *device_type, 			// device type的名称,实际是产品硬件版本，包含了产品信息和硬件版本，后端通过该字段来决定客户端需要的是哪种固件包
	unsigned long rom_version, 			// rom的版本号
	const char *sn 						// 序列号
	) 
{
	cJSON *res_root = NULL,*upgrade_data = NULL;
	char *s_result = NULL;
	int n = 0, ret = -1;
	VERSION_UPGRADE *upgrade = NULL;
	zx_Client client;
	
	dzlog_info(" get all upgrade version info start");
	
	if(!current_version_name  || !device_type  || !sn )
	{
		dzlog_warn("The function parameter  is NULL");
		return upgrade;
	}
	
	//zx_Global_Init(-1);

	zx_Client_InitEx(&client, zx_NoAuth, RESP_BODY_SIZE);
	
	res_root = zx_rom_upgrade_request(current_version_name,device_type,rom_version,sn,client);
	
	if(!res_root || zx_Json_GetInt(res_root, "code", 0) != 0)
	{
		dzlog_error(" JSON is NULL");
		goto END;
	}
	
#if OTA_DEBUG_LOG
	s_result = cJSON_PrintUnformatted(res_root);
	
	if(!s_result )
	{
		dzlog_error("JSON is NULL");
		return NULL;
	}
	dzlog_info("data is %s",s_result);
	
	if(s_result) {free(s_result); s_result = NULL;}
#endif

	if(zx_Json_GetInt(res_root,"code",0) != 0)
	{
		dzlog_error("msg = %s",zx_Json_GetString(res_root,"msg",""));
		goto END;
	}
	
	upgrade = Upgrade_Info_ListCreat();  //创建链表头节点

	res_root = cJSON_GetObjectItem(res_root,"data");

	if(!res_root )
	{
		dzlog_error(" data JSON is NULL");
		goto END;
	}

#if OTA_DEBUG_LOG
	s_result = cJSON_PrintUnformatted(res_root);
	
	if(!s_result )
	{
		dzlog_warn("children is NULL");
		
		if(upgrade) 
		{
			zx_upgrade_free(upgrade); 
			upgrade = NULL;
		}

		goto END;
	}
	dzlog_info("children is %s",s_result);

	if(s_result) {free(s_result); s_result = NULL;}
#endif	
	
	ret = Assignment_value(res_root,upgrade); //赋值
	
	if(ret == -1)
	{
		dzlog_error("error");
		
		if(upgrade) 
			
		{
			zx_upgrade_free(upgrade); 
			upgrade = NULL;
		}
		goto END;
	}

	res_root = cJSON_GetObjectItem(res_root,"children");  //json children

	if(!res_root )
	{
		dzlog_error("children is NULL");
		
		if(upgrade) 

		{
			zx_upgrade_free(upgrade); 
			upgrade = NULL;
		}
		goto END;
	}
#if OTA_DEBUG_LOG
	s_result = cJSON_PrintUnformatted(res_root);
	
	if(!s_result )
	{
		dzlog_warn("children is NULL");
		
		if(upgrade) 
		{
			zx_upgrade_free(upgrade); 
			upgrade = NULL;
		}

		goto END;
	}
	dzlog_info("children is %s",s_result);

	if(s_result) {free(s_result); s_result = NULL;}
#endif	
	while(1)
	{
		upgrade_data = cJSON_GetArrayItem(res_root,n);

		if(!upgrade_data )
		{
			dzlog_info("json data is Parsing ends ");
			break;
		}
	#if OTA_DEBUG_LOG
		s_result = cJSON_PrintUnformatted(upgrade_data);
		
		if(!s_result )
		{
			dzlog_info("json data is Parsing ends ");
			break;
		}
		dzlog_info("data = %d json = %s",n,s_result);
		
		if(s_result) {free(s_result); s_result = NULL;}
	#endif
	
		ret = Upgrade_Info_ListInsert(upgrade,upgrade_data);  //增加children数据在链表尾节点
		
		if(ret == -1)
		{
			dzlog_error("Upgrade_Info_ListInsert is error");
			
			if(upgrade) 
			{
				zx_upgrade_free(upgrade); 
				upgrade = NULL;
			}
			goto END;
		}
	
		++n;
	}
END:
	if(s_result) 
	{
		free(s_result); 
		s_result = NULL;	
	}
	
	zx_Client_Cleanup(&client);
	//curl_global_cleanup();
	
	dzlog_info("get all upgrade version info end");
	
	return upgrade;	
}


/*
success返回0，false返回-1 
参数 down_path: 下载路径
参数 version_upgrade: 传入参数
参数 default_filename: 自己定义的文件名，如果不写，默认为服务器查询到的文件名
*/
int zx_dnload_upgrade_version(
	const char *down_path,              //下载文件存放路径
	VERSION_UPGRADE *upgrade,   		//版本信息
	char * default_filename             //自定义文件名，默认为服务器上的文件名
)
{
	char md5_str[ MD5_OTA_SIZE * 2 + 2 ]={0};
	char *ota_buf = NULL;
	char *filename_path = NULL;

	unsigned long dev_filesize = 0;
	int ret = -1 , i =0;
	int path_len = strlen(down_path);
	int filename_len = 0;
	int num  = 0;

	if(!down_path || !upgrade )
	{
		dzlog_warn("The function parameter  is NULL");
		return ret;
	}

	dzlog_info("down version start");

    if (access(down_path,F_OK) != 0)   //判断目录是否正确的
    {  
        dzlog_error("filename_path error:%s",down_path);  
        return -1;  
    }  
	
	if( !default_filename  ||  strlen( default_filename ) <= 0  )
	{
		filename_len = strlen(upgrade->file_name);
		num = path_len + filename_len + 2;
		filename_path = (char *)malloc(num);
		if( !filename_path )
		{
			dzlog_error(" out of memory for filename_path. ");
			return -1;
		}
		memset(filename_path,0,num);

		sprintf(filename_path,"%s/%s",down_path,upgrade->file_name);
	}else
	{
		num = path_len + strlen( default_filename ) + 2 ; 
		filename_path = (char *)malloc(num);
		if( !filename_path )
		{
			dzlog_error(" out of memory for filename_path. ");
			return -1;
		}
		memset(filename_path, 0, num);
		sprintf(filename_path,"%s/%s",down_path, default_filename );
	}
	
#if OTA_DEBUG_LOG
	dzlog_info("****** filename_path = %s******",filename_path);
#endif

#if OTA_LIBCURL_OR_WGET_DOWNLOAD_METHOD    

	ota_buf = (char *)malloc(strlen(filename_path)+strlen(upgrade->down_url) + 15);

	if( !ota_buf )
	{
		if(filename_path)
		{
			free(filename_path);
			filename_path = NULL;
		}
		dzlog_error(" out of memory for ota_buf. ");
		return -1;
	}
	memset(ota_buf, 0, strlen(filename_path)+strlen(upgrade->down_url) + 15);
	
	sprintf(ota_buf,"wget -O '%s' '%s'",filename_path,upgrade->down_url); 
	
	//zx_do_system(ota_buf,NULL);
	
	ret = zx_ota_system(ota_buf);

	if(ret < 0)
	{
	    dzlog_error("cmd: %s\t error: %s", ota_buf, strerror(errno)); 

	#if OTA_DEBUG_LOG
	
		if(WIFEXITED(ret))
		{
		    dzlog_info("normal termination, exit status = %d\n", WEXITSTATUS(ret)); 
		}
		else if(WIFSIGNALED(ret))
		{
		    dzlog_info("abnormal termination,signal number =%d\n", WTERMSIG(ret)); 
		}
	#endif	
	    goto END;
	}

#else 
	ret = zx_dnload_data(upgrade->down_url,filename_path);	//下载文件
	
	if(ret == -1)
	{	
		dzlog_error("****** dnload data is error******");
		goto END;
	}
#endif

	sync();  //缓冲区同步，系统缓冲区数据立即写入到文件中

	dev_filesize = get_file_size(filename_path);

	dzlog_info("The size of the file after download = %lu",dev_filesize);

	while(dev_filesize != upgrade->file_size)
	{
		
#if OTA_LIBCURL_OR_WGET_DOWNLOAD_METHOD
	
		ret = zx_ota_system(ota_buf);

		if(ret < 0)
		{
		    dzlog_error("cmd: %s\t error: %s", ota_buf, strerror(ret)); 

		#if OTA_DEBUG_LOG
		
			if(WIFEXITED(ret))
			{
			    dzlog_info("normal termination, exit status = %d\n", WEXITSTATUS(ret)); 
			}
			else if(WIFSIGNALED(ret))
			{
			    dzlog_info("abnormal termination,signal number =%d\n", WTERMSIG(ret)); 
			}
		#endif	
		    goto END;
		}
	
#else
		ret = zx_dnload_data(upgrade->down_url,filename_path);	//下载文件
		
		if(ret == -1)
		{
			dzlog_error("****** dnload data is error******");
			goto END;
		}
#endif	
		dzlog_warn("down count number******: %d",++i);

		sync();  //缓冲区同步，系统缓冲区数据写入到文件中
		
		dev_filesize = get_file_size(filename_path);
		
		if(i >= 3)
		{
			ret = -1;
			dzlog_error("****** file_size discrepancy  ******");
			goto END;
		}
		
		dzlog_info("The size of the file after download = %lu",dev_filesize);
	} 

	dzlog_info("******Compare file size is ok");

	ret = Compare_file_md5(filename_path,md5_str); //下载文件生成md5
	
	if(ret == -1)
	{
		dzlog_error("****** generate file md5 is error******");
		goto END;	
	}

#if OTA_DEBUG_LOG
	dzlog_info("******The md5 of the file after download = %s",md5_str);
#endif
	
	if(strcmp(upgrade->file_md5,md5_str) != 0)  //判断md5
	{
		dzlog_error("****** md5 values are different file_md5=%s and md5_str =%s******",upgrade->file_md5,md5_str);
		ret = -1;
		goto END;
	}
	dzlog_info("******Compare file md5 is ok");
	
	ret = 0;
END:

	if(filename_path)
	{
		free(filename_path);
		filename_path = NULL;
	}
	
	if(ota_buf) 
	{
		free(ota_buf); 
		ota_buf = NULL;
	}
	dzlog_info("down version end");
	
	return ret;
}


void upgrade_free(VERSION_UPGRADE *upgrade)
{
	if(!upgrade)
	{
		dzlog_warn("The function parameter is NULL");
		return;
	}
		
	if(upgrade->device_type)
	{
		free(upgrade->device_type);
		upgrade->device_type = NULL;
	}
	if(upgrade->introduction)
	{
		free(upgrade->introduction);
		upgrade->introduction = NULL;
	}
	if(upgrade->down_url)
	{
		free(upgrade->down_url);
		upgrade->down_url = NULL;
	}

	if(upgrade->rom_version_name)
	{
		free(upgrade->rom_version_name);
		upgrade->rom_version_name = NULL;
	}

	if(upgrade->file_name)
	{
		free(upgrade->file_name);
		upgrade->file_name = NULL;
	}

	if(upgrade->file_md5)
	{
		free(upgrade->file_md5);
		upgrade->file_md5 = NULL;
	}

	return;
}


int zx_upgrade_free(VERSION_UPGRADE *head)
{
	if ( !head )
	{
		dzlog_warn("The function parameter is NULL");
		return -1;
	}

	VERSION_UPGRADE * tmp = NULL;

	while (head != NULL)
	{
		tmp = head->next;  //保存head的下一个结点
		upgrade_free(head);
		free(head);
		head = NULL;
		
		head = tmp;  
	}

	return 0;
}










