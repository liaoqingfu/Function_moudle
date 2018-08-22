/*
	Copyright (c) 2013-2014 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.EasyDarwin.org
*/
// CaptureVideo.h: interface for the CCaptureVideo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAPTUREVIDEO_H__BB987A37_E4E3_42B4_B207_AED334084FD9__INCLUDED_)
#define AFX_CAPTUREVIDEO_H__BB987A37_E4E3_42B4_B207_AED334084FD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DshowDef.h"
#include "DShowCaptureAudioAndVideo_Interface.h"



class CCaptureVideo : public DShowCaptureVideo_Interface 
{
public:
	void WINAPI SetVideoCaptureData(int nIndex,int iDeviceId,HWND hWnd,int nFrameRate,
		int iWidth,int iHeight,/*CString*/char* strRGBByte,int nRenderType,int nPinType, 
		int nDataType, BOOL bIsThread);
	int  WINAPI CreateCaptureGraph();
	int  WINAPI BulidPrivewGraph();
	void WINAPI SetCaptureVideoErr(int nError);
	HRESULT WINAPI SetupVideoWindow(HWND hWnd=0);
	void WINAPI ResizeVideoWindow();
	void WINAPI BegineCaptureThread();
	BYTE* WINAPI GetCurrentBuffer(long &lDatasize,wchar_t* strMediaType);

	int GetCaptureVideoErr();
public:
	HRESULT CreateDeinterlaceFilter();

	HRESULT CreateVideoRender(int nType);
	GUID GetMediaTypeGuid(CString strMediaType);
	HRESULT CreateCaptureSampleGrabber(CString strRGBByte);
	HRESULT StopPreview();
	HRESULT StartPreview();
	void NukeDownstream(IGraphBuilder* inGraph, IBaseFilter * inFilter) ;
	void TearDownGraph();
//	void ERR_DEBUG(CString strError);
	void FreeMediaType(AM_MEDIA_TYPE& mt);
	int  CreateVideoFilter(int iDeviceID,HWND hWnd,int nFrameRate,int iWidth,
		int iHeight,CString strRGBByte);

	HRESULT SetVideoSize(int nPreview,CString strRGBByte,int nFrameRate,int iWidth , int iHeight);
	HRESULT CreateCaptureGraphBuilder();
	BOOL BindToVideoDev(int deviceId, IBaseFilter **pFilter);
	
	CCaptureVideo();
	virtual ~CCaptureVideo();
private:
	BOOL m_bThreadExit;//�߳��˳���־

	IGraphBuilder *m_pGraphBuilder;//filter grap ���ⲿ����

	ICaptureGraphBuilder2* m_pCaptureGraphBulid;
	IBaseFilter* m_pBaseFilter;//�豸��Filter;
	IVideoWindow* m_pVideoWin; //��Ƶ��ʾ����
	IMediaControl* m_pMediaCon;//ý�����
	IBaseFilter *m_pSampleGrabberFilter;
	ISampleGrabber *m_pSampleGrabber;
	IMediaEvent *m_pMediaEvent;
	IBaseFilter *m_pWindowRender; 
	IBaseFilter *m_pVideoDeinterlaceFilter;
private:
	int m_nIndex;
	int m_nDataType;
	int m_iDeviceId;
	HWND m_hWnd;

	int m_nFrameRate;
	int m_iWidth;
	int m_iHeight;
	CString  m_strRGBByte;
	int m_nRenderType;  //0---NULL,1--Default 2-VideoRenderer,3--NullRenderer����ʾ,3/����---NULL
	int m_nPinType;		//m_nPinType 0--privew 1-Capture;
	int m_nError;	//0--û�д��󴴽��ɹ�,-1����ʧ�� -2û�д���
	int m_nDeinterlace;//0=����Ҫ����,1--��Ҫ����

	
	BYTE *m_pColorBuffer;
	BYTE *m_pDataBuffer;//ԭʼ����
	long m_nBufferSize;

private:
	BOOL m_bThread;
	HANDLE m_hThreadEvent;
public:
	static UINT OnDataThread(LPVOID lParam);
	void OnThreadDeal();
private:
	BYTE clip255(LONG v);
	void CoverYUY2TOI420(int inWidth, int inHeight, BYTE *pSrc, BYTE *pDest);

public:
	CSampleGrabberCB m_cSampleGrabberCB;
	ThreadCalInfo    m_sThreadCalbackInfo;
	void WINAPI SetDShowCaptureCallback(RealDataCallback realDataCalBack, void* pMaster);
	//void SetDShowThreadCallback(RealDataCallback realDataCalBack, void* pMaster);

};

#endif // !defined(AFX_CAPTUREVIDEO_H__BB987A37_E4E3_42B4_B207_AED334084FD9__INCLUDED_)
