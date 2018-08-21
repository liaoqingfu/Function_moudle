
#include "cloud_storage_interface.h"
#include "ota_interface.h"
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#include "zlog.h"
#define LOG_CONF               "/etc_ro/zx_log.conf" //日志配置文件路径

int main()
{
	
	dzlog_init(LOG_CONF,"");
	dzlog_error("main is start");
	VERSION_UPGRADE *ver = zx_get_all_upgrade_version_info("","ota_pakage",0,"T8001G0317480032");
      if(!ver)
      {
        dzlog_error("****** zx_get_upgrade_version_info is error******");
        return -1;
      }
  
      VERSION_UPGRADE *dev1 = get_device_type_info(ver,"cam_3518_sys");
      VERSION_UPGRADE *dev2 = get_device_type_info(ver,"hub_1310_sys");
      VERSION_UPGRADE *dev3 = get_device_type_info(ver,"cam_1310_sys");
      VERSION_UPGRADE *dev4 = get_device_type_info(ver,"hub_7628_sys");

      zx_dnload_upgrade_version("/mnt/share_nfs",dev1,"cam_3518.bin");
	  //zx_dnload_upgrade_version("/mnt/share_nfs",dev3,"cam_1310.bin");
	  //zx_dnload_upgrade_version("/mnt/share_nfs",dev2,"hub_1310.bin");
	  //zx_dnload_upgrade_version("/mnt/share_nfs",dev4,"hub_7628.img");

	  zx_upgrade_free(ver);

		char * key = zx_putdata_aws_s3("b98bbb2b65accde25aa57b74cfbd0dd36792744d","mnt/3.mp4","T8001G03174800F0",4);
		if(key == NULL)
		{
			if(key) {free(key); key = NULL;}
			dzlog_error("****** zx_putdata_aws_s3 is error******");
			return -1;
		}
		dzlog_error("****** zx_putdata_aws_s3 key = %s******",key);
		
		int ret = zx_getdata_aws_s3("b98bbb2b65accde25aa57b74cfbd0dd36792744d","T8001G031748000F",key,"2018.mp4");

		if(ret == -1)
		{
			if(key) free(key); key = NULL;
			dzlog_error("****** zx_getdata_aws_s3 is error******");
			//return -1;
			//pthread_exit(0);
			_exit(EXIT_SUCCESS);
		}
		
		if(key) {free(key); key = NULL;}
		dzlog_error("****** zx_putdata_aws_s3 end");
		//return 0;
		_exit(EXIT_SUCCESS);   //强制退出，无条件退出

}
