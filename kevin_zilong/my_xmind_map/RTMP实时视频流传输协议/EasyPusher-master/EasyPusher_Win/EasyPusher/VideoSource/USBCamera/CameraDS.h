//////////////////////////////////////////////////////////////////////
// Video Capture using DirectShow
// Author: Shiqi Yu (shiqi.yu@gmail.com)
// Thanks to:
//        HardyAI@OpenCV China
//        flymanbox@OpenCV China (for his contribution to function CameraName, and frame width/height setting)
// Last modification: April 9, 2009
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ʹ��˵����
//   1. ��CameraDS.h CameraDS.cpp�Լ�Ŀ¼DirectShow���Ƶ������Ŀ��
//   2. �˵� Project->Settings->Settings for:(All configurations)->C/C++->Category(Preprocessor)->Additional include directories
//      ����Ϊ DirectShow/Include
//   3. �˵� Project->Settings->Settings for:(All configurations)->Link->Category(Input)->Additional library directories
//      ����Ϊ DirectShow/Lib
//////////////////////////////////////////////////////////////////////
#pragma once

#ifndef CCAMERA_H
#define CCAMERA_H

#define WIN32_LEAN_AND_MEAN

#include "DllManager.h"
#include "ICameraCaptuer.h"

#include <atlbase.h>
#include "DirectShow/Include/qedit.h"
#include "DirectShow/Include/dshow.h"
#include <windows.h>
//#include <cxcore.h>

#include "../../../libEasyPlayer/libEasyPlayerAPI.h"
#include "../../FFEncoder/FFEncoderAPI.h"
#pragma comment(lib, "FFEncoder/FFEncoder.lib")

#define MYFREEMEDIATYPE(mt)    {if ((mt).cbFormat != 0)        \
                    {CoTaskMemFree((PVOID)(mt).pbFormat);    \
                    (mt).cbFormat = 0;                        \
                    (mt).pbFormat = NULL;                    \
                }                                            \
                if ((mt).pUnk != NULL)                        \
                {                                            \
                    (mt).pUnk->Release();                    \
                    (mt).pUnk = NULL;                        \
                }}                                    


//template class DLL_EXPORT CComPtr<IGraphBuilder>; 
//template class DLL_EXPORT CComPtr<IBaseFilter>;
//template class DLL_EXPORT CComPtr<IMediaControl>;
//template class DLL_EXPORT CComPtr<IBaseFilter>;
//template class DLL_EXPORT CComPtr<ISampleGrabber>;
//template class DLL_EXPORT CComPtr<IPin>;
//template class DLL_EXPORT CComPtr<IPin>;
//template class DLL_EXPORT CComPtr<IPin>;
//template class DLL_EXPORT CComPtr<IMediaEvent>;
//template class DLL_EXPORT CComPtr<IBaseFilter>;
//template class DLL_EXPORT CComPtr<IPin>;

typedef struct __CAMERA_INFO_T
{
	char	friendlyName[64];
	int		width;
	int		height;

	__CAMERA_INFO_T	*pNext;
}CAMERA_INFO_T;
typedef struct __CAMERA_LIST_T
{
	int			count;
	CAMERA_INFO_T		*pCamera;
}CAMERA_LIST_T;

typedef struct __CAMERA_SOURCE_THREAD_T
{
	int			flag;
	HANDLE		hThread;

	int			camNo;
	int			width;
	int			height;

    CComPtr<IGraphBuilder> m_pGraph;
    CComPtr<IBaseFilter> m_pDeviceFilter;
    CComPtr<IMediaControl> m_pMediaControl;
    CComPtr<IBaseFilter> m_pSampleGrabberFilter;
    CComPtr<ISampleGrabber> m_pSampleGrabber;
    CComPtr<IPin> m_pGrabberInput;
    CComPtr<IPin> m_pGrabberOutput;
    CComPtr<IPin> m_pCameraOutput;
    CComPtr<IMediaEvent> m_pMediaEvent;
    CComPtr<IBaseFilter> m_pNullFilter;
    CComPtr<IPin> m_pNullInputPin;

	FFE_HANDLE	ffeVideoHandle;

	MediaSourceCallBack	pCallback;
	void		*userPtr;
	void		*pEx;
}CAMERA_SOURCE_THREAD_T;

class CCameraDS//: public ICameraCaptuer  
{
private:
	CAMERA_LIST_T		cameraList;
	CAMERA_SOURCE_THREAD_T	cameraSourceThread;
private:
    bool BindFilter(int nCamIDX, IBaseFilter **pFilter);
    void SetCrossBar();

public:
    // :  Ҫ��nCamID���캯���Ĳ�����ʹ��һ��CCameraDS����ֻ����һ������ͷ
    CCameraDS();
    virtual ~CCameraDS();

	int		InRunning();

	CAMERA_LIST_T *GetCameraList();

    //������ͷ��nCamIDָ�����ĸ�����ͷ��ȡֵ����Ϊ0,1,2,...
    //bDisplayPropertiesָʾ�Ƿ��Զ���������ͷ����ҳ
    //nWidth��nHeight���õ�����ͷ�Ŀ�͸ߣ��������ͷ��֧�����趨�Ŀ�Ⱥ͸߶ȣ��򷵻�false
    bool OpenCamera(int nCamID, int nWidth=352, int nHeight=288, MediaSourceCallBack _callback=NULL, void *userPtr=NULL);

    //�ر�����ͷ�������������Զ������������
    void CloseCamera();
};

#endif 
