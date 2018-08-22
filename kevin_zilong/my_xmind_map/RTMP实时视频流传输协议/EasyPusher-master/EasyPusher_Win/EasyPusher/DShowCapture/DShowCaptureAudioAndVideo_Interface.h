/*
	Copyright (c) 2013-2014 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.EasyDarwin.org
*/
// Add by SwordTwelve

//////////////////////////////////////////////////////////////////////////
//
// DShow��ȡ�������Ƶ��������Ƶԭʼ����Dll�����ඨ��
// DShowCaptureAudioAndVideo_Interface.h
// 
//////////////////////////////////////////////////////////////////////////

#ifndef DSHOWCAPTUREAUDIOANDVIDEO_H
#define DSHOWCAPTUREAUDIOANDVIDEO_H

#ifdef __cplusplus

//Ϊ��ʵ�ֶ����Ĵ��麯��
//��ΪC����������Ļ�����ҪԴ���룬Ȼ��������ȥ���Ļ�����ֻҪ�ļ�ͷ���ô����ˡ�

//ʵʱ������������
typedef enum _REALDATA_TYPE
{
	REALDATA_RAW = 0,				//������ֱ���յ�������,������Ƶ����Ƶ�ȵ�
	REALDATA_VIDEO,					//ԭʼ��Ƶ����,������YUY2 UYVY�ȵ�
	REALDATA_AUDIO,					//ԭʼ��Ƶ����,������PCM�ȵ�
	REALDATA_H264,					//��Ƶ������H264��Ƶ����
	REALDATA_AAC,					//��Ƶ������AAC��Ƶ����
}RealDataStreamType;

// �豸���������Ϣ
typedef struct tagDEVICE_CONFIG_INFO
{
	int nVideoId;//��Ƶ	ID -1==���ã�0-n == id
	int nAudioId;//��Ƶid -1==���ã�0-n == id
	int  nDeviceId;//�豸��� 1,2,3,4,5....
	struct tagVideoInfo
	{
		int nWidth;
		int nHeight;
		int nVideoWndId;
		int nFrameRate;//��Ƶ֡��
		char strDataType[64];//��ʶYUV�������ͣ���"YUY2" "YUYV" "UYVY��"
		int nRenderType;//���ַ�ʽ
		int nPinType;//�����
	}VideoInfo;

	struct tagAudioInfo
	{
		int nChannaels;//ͨ����
		int nBytesPerSample;//����λ��
		int nSampleRate;//������
		DWORD nAudioBufferType;//��Ƶ�������Ĵ�С
		int nPinType;//�����
	}AudioInfo;

}DEVICE_CONFIG_INFO;

//ʵʱ����������Ϣ �� REALDATA_TYPE ���ʹ��
typedef struct _realdata_callback_param
{	
	int		size;
	struct _video{
		int iEnable;	//�ýṹ���ڵ���Ϣ�Ƿ���Ч //REALDATA_VIDEO or REALDATA_YUV ��Ч
		unsigned int timestampe;	//ʱ�����Ϣ����λ����(ms)
		int width;		//��Ƶ���	//REALDATA_YUV ��Ч
		int height;		//��Ƶ����  //REALDATA_YUV ��Ч
		int encodeType;	//��Ƶ�������� 0:δ֪ H264 = 1 or mpeg4 = 2 //��ǰĬ��ΪH264 //REALDATA_VIDEO ��Ч
		int frameType;	//֡���� 0:δ֪ IDR֡=1 I֡=2 P֡=3 B֡=4 //REALDATA_VIDEO ��Ч
	}video;
	struct _audio{
		int iEnable;	//�ýṹ���ڵ���Ϣ�Ƿ���Ч //REALDATA_AUDIO or REALDATA_PCM ��Ч
		unsigned int timestampe;	//ʱ�����Ϣ����λ����(ms)
		int	bits;		//λ�� 16bits or 8bits	//REALDATA_PCM ��Ч
		int samplerate;	//������ 8000Hz 16000Hz 44100Hz�ȵ�	//REALDATA_PCM ��Ч
		int channels;	//������ ������=1 or ˫����=2	//REALDATA_PCM ��Ч
		int encodeType;	//��Ƶ�������� 0:δ֪ AAC=1 MP3=2 G729=10	//REALDATA_AUDIO ��Ч
	}audio;
}RealDataStreamInfo;

//����������Ϣ
//m_nResizeVideoWidth,m_nResizeVideoHeight,25,2000,3,m_nScrVideoWidth,m_nScrVideoHeight);
typedef struct tagEncoder_Config_Info
{
	int nScrVideoWidth;
	int nScrVideoHeight;
	int nResizeVideoWidth;
	int nResizeVideoHeight;
	int nFps;//֡��
	int nMainBitRate;//����Ƶ����
	int nSingleBitRate;//����Ƶ����
	int nMainEncLevel;//�����뼶��
	int nSingleEncLevel;//�ӱ��뼶��
	int nMainEncQuality;//��������Ƶ����
	int nSingleEncQuality;//�ӱ�����Ƶ����


	int nMainKeyFrame;//���ؼ�֡
	int nSingleKeyFrame;//�ӹؼ�֡

	int nMainUseQuality;
	int nSingleUseQuality;//�Ƿ�������������1--����,0--����
	int nRtmpUseQuality;//�Ƿ�������������1--����,0--����
	int nRtmpVideoWidth;//rtmp
	int nRtmpVideoHeight;
	int nRtmpEncLevel;
	int nRtmpEncQuality;
	int nRtmpBitRate;
	int nRtmpKeyFrame;
	char* sRtmpURL;

	int nMaxVideoFrames;//��Ӱģʽ�����Ƶ֡��
	int nSingleVideoFrames;//��Դģʽ�����Ƶ֡��

	DWORD nMaxPerFileSize;//���ÿ���ļ���С
	int nResizeType;
}Encoder_Config_Info;

//�ص�ȡ��ʵʱ���ݽ��д���
// int RealDataManager(int nDevId, unsigned char *pBuffer, int nBufSize, 
// 					RealDataStreamType realDataType, void* realDataInfo);

typedef int (WINAPI *RealDataCallback)(int nDevId, unsigned char *pBuffer, int nBufSize, 
								RealDataStreamType realDataType, void* realDataInfo, void* pMaster);


typedef struct tagThreadCalInfo
{
	RealDataCallback realDataCalFunc;
	void* pMaster;
}ThreadCalInfo;

//��ȡ��Ƶ���ݵ�����ӿ�
class DShowCaptureVideo_Interface
{
public:
	//nIndex���豸��ţ���Ƶ-1����Ƶ0---N
	virtual void WINAPI SetVideoCaptureData(int nIndex,int iDeviceId,HWND hWnd,int nFrameRate,
		int iWidth,int iHeight,char* strRGBByte,int nRenderType,int nPinType, 
		int nDataType, BOOL bIsThread) = 0;
	virtual int WINAPI CreateCaptureGraph() = 0;
	virtual int WINAPI BulidPrivewGraph() = 0;
	virtual void WINAPI SetCaptureVideoErr(int nError) = 0;
	virtual void WINAPI BegineCaptureThread() = 0;
	virtual HRESULT WINAPI SetupVideoWindow(HWND hWnd=0) = 0;
	virtual void WINAPI ResizeVideoWindow() = 0;
	virtual void WINAPI SetDShowCaptureCallback(RealDataCallback realDataCalBack, void* pMaster) = 0;

};

//��ȡ��Ƶ���ݵ�����ӿ�
class DShowCaptureAudio_Interface
{
public:
	virtual void WINAPI SetAudioCaptureData(int iDeviceId,int nChannels,
		int nBytesPerSample,int nSampleRate,int nAudioBufferType,int nPinType, 
		int nDataType, BOOL bIsThread) = 0;
	virtual int  WINAPI CreateCaptureGraph()  = 0;
	virtual int  WINAPI BulidCaptureGraph()   = 0;
	virtual void WINAPI BegineCaptureThread() = 0;
	virtual void WINAPI SetDShowCaptureCallback(RealDataCallback realDataCalBack, void* pMaster) = 0;
};

//��Ƶ��ȡ����ӿ�ָ������
typedef	DShowCaptureVideo_Interface*	LPVideoCapturer;	

LPVideoCapturer	APIENTRY Create_VideoCapturer();//�������ƽӿ�ָ��
void APIENTRY Release_VideoCapturer(LPVideoCapturer lpVideoCapturer);//���ٿ��ƽӿ�ָ��


//��Ƶ��ȡ����ӿ�ָ������
typedef	DShowCaptureAudio_Interface*	LPAudioCapturer;	

LPAudioCapturer	APIENTRY Create_AudioCapturer();//�������ƽӿ�ָ��
void APIENTRY Release_AudioCapturer(LPAudioCapturer lpAudioCapturer);//���ٿ��ƽӿ�ָ��

//void APIENTRY SetDShowCaptureCallback(RealDataCallback realDataCalBack, void* pMaster);

#endif//__cplusplus

#endif//DSHOWCAPTUREAUDIOANDVIDEO_H