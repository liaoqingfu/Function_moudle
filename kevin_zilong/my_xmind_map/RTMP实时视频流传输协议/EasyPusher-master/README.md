# EasyPusher #

EasyPusher是EasyDarwin开源流媒体团队开发的一个推送流媒体音/视频流给开源流媒体服务器EasyDarwin的标准RTSP/RTP协议推送库，全平台支持(包括Windows/Linux(32 & 64)，ARM各平台，Android、IOS)，通过EasyPusher我们就可以避免接触到稍显复杂的RTSP/RTP/RTCP推送流程，只需要调用EasyPusher的几个API接口，就能轻松、稳定地把流媒体音视频数据推送给EasyDarwin服务器进行转发和分发，EasyPusher经过长时间的企业用户检验，稳定性非常高;

## Android ##

EasyPusher_Android迁移到了：https://github.com/EasyDarwin/EasyPusher_Android

EasyPusher_iOS迁移到了：https://github.com/EasyDarwin/EasyPusher_iOS

## 调用示例 ##

- **EasyPusher_Android**：实时采集安卓摄像头音视频（Android 5.0+支持采集手机桌面屏幕进行直播），进行H264/AAC编码后，调用libEasyPusher进行直播推送；

- **EasyPusher_File**：推送本地文件到EasyDarwin流媒体服务器；

- **EasyPusher_RTSP**：通过EasyRTSPClient库，将RTSP/RTP数据获取到本地，再通过EasyPusher推送到EasyDarwin进行RTSP、HLS直播；

- **EasyPusher_Win**：支持本地摄像头和声卡、RTSP流、屏幕捕获、MP4文件通过EasyPusher推送到EasyDarwin进行RTSP、HLS直播；

- **EasyPusher_SDK**：通过调用摄像机厂家的Camera SDK回调的音视频数据，进行RTSP/RTP直播推送，示例中的SDK是我们EasyDarwin开源摄像机的配套库，EasyCamera SDK及配套源码可在 [http://www.easydarwin.org](http://www.easydarwin.org "EasyDarwin") 或者 [https://github.com/EasyDarwin/EasyCamera](https://github.com/EasyDarwin/EasyCamera "EasyCamera") 获取到，您也可以用自己项目中用到的SDK获取音视频数据进行推送。EasyPusher_SDK可以接入所有的IP Camera，其他IP Camera只需要使用其对于SDK进行调用即可。

	Windows编译方法，

    	Visual Studio 2010 编译：./EasyPusher-master/win/EasyPusher.sln

	Linux编译方法，
		
		chmod +x ./Buildit
		./Buildit

	> 调用提示：目前的调用示例程序，可以接收参数，具体参数的使用，请在调用时增加**-h**命令查阅，EasyPusher_File示例需要将本地文件copy到可执行文件同目录！


- **我们同时提供Windows、Linux、ARM、Android、IOS版本的EasyPusher库**：EasyPusher SDK商业使用需要经过授权才能永久使用，商业授权方案可以邮件发送至[support@easydarwin.org](mailto:support@easydarwin.org "EasyDarwin support mail")或者通过电话Tel：**13718530929** 进行更深入的技术与合作咨询；

	<table>
	<tr><td><b>支持平台</b></td><td><b>芯片</b></td><td><b>位置名称</b></td></tr>
	<tr><td>Windows</td><td>x86</td><td>./Lib/</td></tr>
	<tr><td>Windows</td><td>x64</td><td>./Lib/x64/</td></tr>
	<tr><td>Linux</td><td>x86</td><td>./Lib/</td></tr>
	<tr><td>Linux</td><td>x64</td><td>./Lib/x64/</td></tr>
	<tr><td>海思</td><td>arm-hisiv100-linux</td><td>./Lib/hisiv100/</td></tr>
	<tr><td>海思</td><td>arm-hisiv200-linux</td><td>./Lib/hisiv200/</td></tr>
	<tr><td>Android</td><td>armeabi</td><td>armeabi libeasypusher.so</td></tr>
	<tr><td>Android</td><td>armeabi-v7a</td><td>libeasypusher.so</td></tr>
	<tr><td>Android</td><td>arm64-v8a</td><td>libeasypusher.so</td></tr>
	<tr><td colspan="3"><center>邮件获取更多平台版本</center></td></tr>
	</table>


## Demo下载 ##


- Android [https://fir.im/EasyPusher ](https://fir.im/EasyPusher "EasyPusher_Android")

![EasyPusher_Android](http://www.easydarwin.org/skin/bs/images/app/EasyPusher_AN.png)

- iOS [https://itunes.apple.com/us/app/easypusher/id1211967057](https://itunes.apple.com/us/app/easypusher/id1211967057 "EasyPusher_iOS")

![EasyPusher_iOS](http://www.easydarwin.org/skin/bs/images/app/EasyPusher_iOS.png)


## 调用过程 ##
![](http://www.easydarwin.org/skin/easydarwin/images/easypusher20160902.gif)


## 特殊说明 ##
EasyPusher目前支持的音视频格式：

	/* 视频编码 */
	#define EASY_SDK_VIDEO_CODEC_H264	0x01000001		/* H264  */
	#define	EASY_SDK_VIDEO_CODEC_MJPEG	0x01000002		/* MJPEG */
	#define	EASY_SDK_VIDEO_CODEC_MPEG4	0x01000004		/* MPEG4 */
	
	/* 音频编码 */
	#define EASY_SDK_AUDIO_CODEC_AAC	0x01000011		/* AAC */
	#define EASY_SDK_AUDIO_CODEC_G711A	0x01000012		/* G711 alaw*/
	#define EASY_SDK_AUDIO_CODEC_G711U	0x01000014		/* G711 ulaw*/

EasyPusher回调事件定义：

	typedef enum __EASY_PUSH_STATE_T
	{
	    EASY_PUSH_STATE_CONNECTING   =   1,     /* 连接中 */
	    EASY_PUSH_STATE_CONNECTED,              /* 连接成功 */
	    EASY_PUSH_STATE_CONNECT_FAILED,         /* 连接失败 */
	    EASY_PUSH_STATE_CONNECT_ABORT,          /* 连接异常中断 */
	    EASY_PUSH_STATE_PUSHING,                /* 推流中 */
	    EASY_PUSH_STATE_DISCONNECTED,           /* 断开连接 */
	    EASY_PUSH_STATE_ERROR
	}EASY_PUSH_STATE_T;


## 获取更多信息 ##

邮件：[support@easydarwin.org](mailto:support@easydarwin.org) 

WEB：[www.EasyDarwin.org](http://www.easydarwin.org)

QQ交流群：[465901074](http://jq.qq.com/?_wv=1027&k=2G045mo "EasyPusher & EasyRTSPClient")

Copyright &copy; EasyDarwin.org 2012-2017

![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
