/*
	Copyright (c) 2013-2014 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.EasyDarwin.org
*/
// CaptureAudio.cpp: implementation of the CCaptureAudio class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CaptureAudio.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// void CCaptureAudio::ERR_DEBUG(CString strError)
// {
// 	AfxMessageBox(strError);
// }
CCaptureAudio::CCaptureAudio()
{

//	if(FAILED(CoInitialize(NULL))) 
//	{
//		AfxMessageBox("CoInitialize Failed!\r\n"); 
//		return ;
//	}
	m_pGraphBuilder = NULL;
	
	m_pCaptureGraphBulid = NULL;
	m_pBaseFilter = NULL;
	m_pVideoWin = NULL; 
	m_pMediaCon = NULL;
	m_pSampleGrabberFilter = NULL;
	m_pSampleGrabber =NULL;

	m_nChannels=2;
	m_nBytesPerSample=16;//λ��16λ
	m_nSampleRate=44100;//44100;,������
	m_nAudioBufferType=0;
	m_nPinType=1;
	m_pDataBuffer=NULL;
	m_nBufferSize=2;

	m_hThreadEvent=NULL;
	m_bThread=FALSE;
	m_bThreadExit = FALSE;
}

CCaptureAudio::~CCaptureAudio()
{
	if(m_bThread==TRUE)
	{	
		m_bThreadExit = TRUE;
		if(m_hThreadEvent)
		{	
			Sleep(100);
			CloseHandle(m_hThreadEvent);
		}
	}
	if(m_pMediaCon)
	{
		m_pMediaCon->Stop();
		m_pMediaCon->Release();
		m_pMediaCon = NULL;
	}
	if(m_pVideoWin)
	{
		m_pVideoWin->put_Visible(OAFALSE);
		m_pVideoWin->put_Owner(NULL);
		m_pVideoWin->Release();
		m_pVideoWin = NULL;
	}
	TearDownGraph();
	SAFE_RELEASE(m_pGraphBuilder);
	SAFE_RELEASE(m_pCaptureGraphBulid);
	SAFE_RELEASE(m_pSampleGrabberFilter);
	SAFE_RELEASE(m_pSampleGrabber);
	m_nBufferSize=0;
	if(m_pDataBuffer)
	{
		delete []m_pDataBuffer;
		m_pDataBuffer=NULL;
	}


//	CoUninitialize();
}
//�ϵ�Graph
void CCaptureAudio::TearDownGraph()
{
	//���ͷŵ�û��ʹ�õ�
	if(m_pVideoWin)
	{
		m_pVideoWin->put_Owner(NULL);
		m_pVideoWin->put_Visible(OAFALSE);
		m_pVideoWin->Release();
		m_pVideoWin = NULL;
	}
	if(m_pBaseFilter)
        NukeDownstream(m_pGraphBuilder,m_pBaseFilter);
   	SAFE_RELEASE(m_pGraphBuilder);
	SAFE_RELEASE(m_pCaptureGraphBulid);
	SAFE_RELEASE(m_pSampleGrabberFilter);
	SAFE_RELEASE(m_pSampleGrabber);
}
// Tear down everything downstream of a given filter
//�ر��������е�����
void CCaptureAudio::NukeDownstream(IGraphBuilder * inGraph, IBaseFilter * inFilter) 
{
	if (inGraph && inFilter)
	{
		IEnumPins * pinEnum = 0;
		if (SUCCEEDED(inFilter->EnumPins(&pinEnum)))
		{
			pinEnum->Reset();
			IPin * pin = 0;
			ULONG cFetched = 0;
			bool pass = true;
			while (pass && SUCCEEDED(pinEnum->Next(1, &pin, &cFetched)))
			{
				if (pin && cFetched)
				{
					IPin * connectedPin = 0;
					pin->ConnectedTo(&connectedPin);
					if(connectedPin) 
					{
						PIN_INFO pininfo;
						if (SUCCEEDED(connectedPin->QueryPinInfo(&pininfo)))
						{
							if(pininfo.dir == PINDIR_INPUT) 
							{
								NukeDownstream(inGraph, pininfo.pFilter);
								inGraph->Disconnect(connectedPin);
								inGraph->Disconnect(pin);
								inGraph->RemoveFilter(pininfo.pFilter);
							}
							pininfo.pFilter->Release();
						}
						connectedPin->Release();
					}
					pin->Release();
				}
				else
				{
					pass = false;
				}
			}
			pinEnum->Release();
		}
	}
}


//������Ƶ����Graph
HRESULT CCaptureAudio::CreateCaptureGraphBuilder()
{
	HRESULT hr=NOERROR;
	
	if(m_pGraphBuilder==NULL)
	{
		hr=CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_pGraphBuilder);
		if(FAILED(hr))
		{
			//ERR_DEBUG("CreateCaptureGraphBuilder Create m_pGraphBuilder Failed");
			return hr;
		}	
	}
	if(m_pCaptureGraphBulid==NULL)
	{
		hr = CoCreateInstance (CLSID_CaptureGraphBuilder2 , NULL, CLSCTX_INPROC,
			IID_ICaptureGraphBuilder2, (void **) &m_pCaptureGraphBulid);
		if (FAILED(hr))		
		{
			//ERR_DEBUG("CreateCaptureGraphBuilder CaptureGraphBuilder2 Failed");
			return hr;
		}	
		hr = m_pCaptureGraphBulid->SetFiltergraph(m_pGraphBuilder);
	}
	if(m_pMediaCon==NULL)
	{
		hr = m_pGraphBuilder->QueryInterface(IID_IMediaControl, (void **)&m_pMediaCon);
		if (FAILED(hr))
		{
			//ERR_DEBUG("CreateCaptureGraphBuilder  QueryInterface m_pMediaCon Failed");
			return hr;
		}
	}
	return hr;
}

//������Ƶ�豸��������Ƶ�豸���뵽
BOOL CCaptureAudio::BindToAudioDev(int deviceId, IBaseFilter **pFilter)
{
	if (deviceId < 0)
	{
		return FALSE;
	}
	CComPtr<ICreateDevEnum> pCreateDevEnum;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
		IID_ICreateDevEnum, (void**)&pCreateDevEnum);
	if (hr != NOERROR)
	{
		return FALSE;
	}
	CComPtr<IEnumMoniker> pEm;
	//�˴�����Ƶ��ͬ�ĵط�
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_AudioInputDeviceCategory,&pEm, 0);
	if (hr != NOERROR)
	{
		return FALSE;
	}
	pEm->Reset();
	ULONG cFetched;
	IMoniker *pM;
	
	int index = 0;
	while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK, index <= deviceId)
	{
		IPropertyBag *pBag;
		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
		if(SUCCEEDED(hr)) 
		{
			VARIANT var;
			var.vt = VT_BSTR;
			hr = pBag->Read(L"FriendlyName", &var, NULL);
			if (hr == NOERROR) 
			{
				if (index == deviceId)
				{
					//����Ƶ�豸�󶨵�������������
					pM->BindToObject(0, 0, IID_IBaseFilter, (void**)pFilter);
					//m_iDeviceId = deviceId;
				}
				SysFreeString(var.bstrVal);
			}
			pBag->Release();
		}
		pM->Release();
		index++;
	}
	return TRUE;
}


void CCaptureAudio::SetAudioCaptureData(int iDeviceId,int nChannels,int nBytesPerSample,int nSampleRate,int nAudioBufferType,int nPinType, int nDataType, BOOL bIsThread)
{
	//nDataType:��������1--��Ƶ��2--��Ƶ����
	//nIndex���豸��ţ���Ƶ-1����Ƶ0---N
	m_iDeviceId		  = iDeviceId;
	m_nChannels		  = nChannels;
	m_nBytesPerSample = nBytesPerSample;
	m_nSampleRate     = nSampleRate;
	m_nAudioBufferType= nAudioBufferType;
	m_nPinType		  = nPinType;
	m_nDataType		  = nDataType;
	m_bThread		  = bIsThread;
}

//��ʼ�������������豸,�������һ��pGraph����
int CCaptureAudio::CreateCaptureGraph()
{

	//����Capture
	int bFlag = 1;
	if(FAILED(CreateCaptureGraphBuilder()))
	{
		bFlag=-1;
		return bFlag;
	}
	
	HRESULT hr=NOERROR;
	hr = BindToAudioDev(m_iDeviceId,&m_pBaseFilter);
	if(FAILED(hr))
	{
		m_iDeviceId=-1;
		//ERR_DEBUG("BindVideoFilter Failed");
		return -1;
	}
	hr = m_pGraphBuilder->AddFilter(m_pBaseFilter, L"Capture");
	return bFlag;

}


HRESULT CCaptureAudio::CreateCaptureSampleGrabber()
{
	HRESULT hr=NOERROR;
	if(m_pSampleGrabberFilter==NULL)
	{
		hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, 
			(LPVOID *)&m_pSampleGrabberFilter);
		if(FAILED(hr))
		{
			SAFE_RELEASE(m_pSampleGrabberFilter);
			//ERR_DEBUG("CreateCaptureGraphBuilder  QueryInterface m_pSampleGrabberFilter Failed");
			return hr;
		}
	}
	if(m_pSampleGrabber==NULL)
	{
		hr = m_pSampleGrabberFilter->QueryInterface(IID_ISampleGrabber, (void**)&m_pSampleGrabber);
		if(FAILED(hr))
		{
			SAFE_RELEASE(m_pSampleGrabberFilter);
			SAFE_RELEASE(m_pSampleGrabber);
			//ERR_DEBUG("CreateCaptureGraphBuilder  QueryInterface m_pSampleGrabber Failed");
			return hr;
		}

		CMediaType audioType;
		audioType.SetType(&MEDIATYPE_Audio);
		hr = m_pSampleGrabber->SetMediaType( &audioType );
		hr = m_pGraphBuilder->AddFilter(m_pSampleGrabberFilter, L"Grabber");
	}
	return hr;
}


/*
����: BulidPrivewGraph()
����: ��������
*/
int CCaptureAudio::BulidCaptureGraph()
{
	HRESULT hr=NOERROR;
	if(m_iDeviceId <0)
	{
		return -1;
	}
	// �������������ʱ���ֵ��ó����쳣������ [10/23/2014-16:28:00 Dingshuai]
	if(m_pBaseFilter==NULL)
	{
		return -1;
	}

	GUID pCategorySuc = PIN_CATEGORY_PREVIEW;
	GUID pCategoryFail = PIN_CATEGORY_CAPTURE;
	if(m_nPinType==0)
	{
		pCategorySuc = PIN_CATEGORY_PREVIEW;
		pCategoryFail = PIN_CATEGORY_CAPTURE;

	}
	else
	{
		pCategorySuc = PIN_CATEGORY_CAPTURE;
		pCategoryFail = PIN_CATEGORY_PREVIEW;	
	}
	hr = SetAudioFormat(m_nChannels,m_nBytesPerSample,m_nSampleRate,m_nAudioBufferType,m_nPinType);
	
	if(FAILED(hr))
	{
		//ERR_DEBUG("SetAudioFormat Failed");
		return -1;
		
	}
	hr = CreateCaptureSampleGrabber();
	if(FAILED(hr))
	{
		SAFE_RELEASE(m_pSampleGrabberFilter);
		SAFE_RELEASE(m_pSampleGrabber);
		//ERR_DEBUG("CreateCaptureSampleGrabber Failed");
		return -1;
	}

	hr = m_pCaptureGraphBulid->RenderStream(&pCategorySuc,&MEDIATYPE_Audio,m_pBaseFilter,NULL,m_pSampleGrabberFilter);
	if(FAILED(hr))
	{	
			//ERR_DEBUG("PrivewVideoDev RenderStream Failed ");
			return -1;
	}
	if(m_bThread)//��ʶ�Ƿ񴴽��߳�
	{
		m_pSampleGrabber->SetBufferSamples(TRUE);
		m_pSampleGrabber->SetOneShot(FALSE);
	}
	else
	{
		m_pSampleGrabber->SetBufferSamples(TRUE);
		m_pSampleGrabber->SetOneShot(FALSE);
		m_cSampleGrabberCB.SetDataInfo(-1, m_nDataType);
		int nMode=1;//0--SampleCB,1--BufferCB
		m_pSampleGrabber->SetCallback(&m_cSampleGrabberCB, nMode);
	}

	hr = StartPreview();
	if(FAILED(hr))
	{
		return -1;
	}
	return 1;
}

HRESULT CCaptureAudio::StartPreview()
{
	HRESULT hr=NOERROR;
	if(m_pMediaCon==NULL)
	{
		hr = m_pGraphBuilder->QueryInterface(IID_IMediaControl, (void **)&m_pMediaCon);
		if(SUCCEEDED(hr))
		{
			hr = m_pMediaCon->Run();
			if(FAILED(hr))
			{
				m_pMediaCon->Stop();
			}
		}
	}
	else
	{
		hr = m_pMediaCon->Run();
		if(FAILED(hr))
		{
			m_pMediaCon->Stop();
		}
	}
	return hr;
}

HRESULT CCaptureAudio::StopPreview()
{
	HRESULT hr=NOERROR;
	if(m_pMediaCon==NULL)
	{
		hr = m_pGraphBuilder->QueryInterface(IID_IMediaControl, (void **)&m_pMediaCon);
		if(SUCCEEDED(hr))
		{
			hr = m_pMediaCon->Stop();
		}
		if(FAILED(hr))
		{
			return hr;
		}
	}
	else
	{
		hr = m_pMediaCon->Stop();
	}
	return hr;
}

HRESULT CCaptureAudio::SetAudioFormat(int nChannels,int nBytesPerSample,long nFrequency,int nAudioBufferType,int nPinType)
{
	HRESULT   hr=0;   
    IAMBufferNegotiation   *pAMbufNeg=0;   
    IAMStreamConfig   *pAMStreamConfig=0; 
	

	if(m_pBaseFilter==NULL)
		return -1;
//	int   nChannels=2;   
//	int   nBytesPerSample=(int)2; //16λ
//	long   nFrequency=22050;//44100;    

	nBytesPerSample=nBytesPerSample/8;
    long   lBytesPerSecond  = (long)   (nBytesPerSample*nFrequency*nChannels);          
    long   lBufferSize=1024*128;//(long)((float)lBytesPerSecond*0.05);//50ms 
	BOOL bFlag=TRUE;
	if(nAudioBufferType==0)
	{
		lBufferSize=1024*128;
		bFlag=TRUE;
	}
	else if(nAudioBufferType==1)
	{
		lBufferSize=(long)((float)lBytesPerSecond*0.05);
		bFlag=TRUE;
	}
	else if(nAudioBufferType==2)
	{
		bFlag=FALSE;
	}
	else if(nAudioBufferType>=512)
	{
		lBufferSize=nAudioBufferType;
		bFlag=TRUE;
	}
	if(bFlag==TRUE)
	{
		//�޸Ĳ��񻺳����Ĵ�С
		IPin   *pPin=0; 
		hr = GetPin( m_pBaseFilter, PINDIR_OUTPUT, 0, &pPin);  
		if(SUCCEEDED(hr)) 
		{
			hr=pPin->QueryInterface(IID_IAMBufferNegotiation,(void**)&pAMbufNeg);
			if(FAILED(hr))   
			{   
				return hr; 
				
			}
			ALLOCATOR_PROPERTIES   prop={0};
			//Alignment of the buffer; buffer start will be aligned on a multiple of this value
			prop.cbAlign   =   nBytesPerSample * nChannels;//   -1   means   no   preference.   
			prop.cbBuffer   =   lBufferSize;//ÿһ���������ĳ���
			prop.cbPrefix   =   -1;   
			prop.cBuffers   =   8; //�������Ŀ���
			hr   =   pAMbufNeg->SuggestAllocatorProperties(&prop);   
			pAMbufNeg->Release();
			
		}
	}
	//������Ƶ�����ȵȲ���
	hr = m_pCaptureGraphBulid->FindInterface(&PIN_CATEGORY_CAPTURE,&MEDIATYPE_Audio,
		m_pBaseFilter,IID_IAMStreamConfig,(void **)&pAMStreamConfig);
	AM_MEDIA_TYPE   *pmt={0};   
	hr   =   pAMStreamConfig->GetFormat(&pmt);   
	if   (SUCCEEDED(hr))   
	{   
		WAVEFORMATEX   *pWF   =   (WAVEFORMATEX   *)   pmt->pbFormat;   
		pWF->nChannels   = (WORD)   nChannels;   
		pWF->nSamplesPerSec   =nFrequency;   
		pWF->nAvgBytesPerSec =  lBytesPerSecond;   
		pWF->wBitsPerSample  =  (nBytesPerSample   *   8);   
		pWF->nBlockAlign   = (WORD)   (nBytesPerSample*2   );   
		hr = pAMStreamConfig->SetFormat(pmt);   
		FreeMediaType(*pmt);   
	}  
	pAMStreamConfig->Release();
	return   hr;   
}

void CCaptureAudio::FreeMediaType(AM_MEDIA_TYPE& mt)
{
	if (mt.cbFormat != 0) 
	{
		CoTaskMemFree((PVOID)mt.pbFormat);
		mt.cbFormat = 0;
		mt.pbFormat = NULL;
	}
	if (mt.pUnk != NULL) 
	{
		mt.pUnk->Release();
		mt.pUnk = NULL;
	}
}

HRESULT CCaptureAudio::GetPin( IBaseFilter * pFilter, PIN_DIRECTION dirrequired, int iNum, IPin **ppPin)
{
    CComPtr< IEnumPins > pEnum;
    *ppPin = NULL;
	
    if (!pFilter)
		return E_POINTER;
	
    HRESULT hr = pFilter->EnumPins(&pEnum);
    if(FAILED(hr)) 
        return hr;
	
    ULONG ulFound;
    IPin *pPin;
    hr = E_FAIL;
	
    while(S_OK == pEnum->Next(1, &pPin, &ulFound))
    {
        PIN_DIRECTION pindir = (PIN_DIRECTION)3;
		
        pPin->QueryDirection(&pindir);
        if(pindir == dirrequired)
        {
            if(iNum == 0)
            {
                *ppPin = pPin;  // Return the pin's interface
                hr = S_OK;      // Found requested pin, so clear error
                break;
            }
            iNum--;
        } 
		
        pPin->Release();
    } 
	
    return hr;
}

//��ʼ
void CCaptureAudio::BegineCaptureThread()
{
	if(m_bThread==FALSE) 
		return;

	BOOL bInnerThread = FALSE;
	if (!bInnerThread)
	{
		return;
	}
	m_bThreadExit = FALSE;

	//�򿪽����̣߳�����ʧ��ʱҲ�������˳�
	m_hThreadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if(m_hThreadEvent == INVALID_HANDLE_VALUE)
	{
		return;
	}
	AfxBeginThread(OnDataThread,this, THREAD_PRIORITY_NORMAL);//HIGHEST);//NORMAL);//
}

UINT CCaptureAudio::OnDataThread(LPVOID lParam)//LPVOID* lParam)
{
#define FLAG _T("{5DDA0840-6AE0-4c1b-9488-35F95396E4A8}")
	HANDLE handle=::CreateMutex(NULL,FALSE,FLAG);
	if(GetLastError()==ERROR_ALREADY_EXISTS)
	{	
		return 0;
	}
	
	CCaptureAudio* pControler=(CCaptureAudio *)lParam;
	if(!pControler)
	{
		return 0;
	}
	
	int nState=0;
	
	while(!pControler->m_bThreadExit)
	{
		// ����Ƿ��йرձ��̵߳��ź�
		DWORD dwEvent = WaitForSingleObject(pControler->m_hThreadEvent, 10);
		if (dwEvent == WAIT_OBJECT_0)
		{
			AfxEndThread(0,TRUE);
			return 0;
		}
		pControler->OnThreadDeal();	
	}
	return 0;
}

BYTE* CCaptureAudio::GetCurrentBuffer(long &lDatasize)
{
	if(m_pSampleGrabber==NULL)
	{
		lDatasize=0;
		return NULL;
	}
	long  lSize = 0;
	HRESULT hr=NOERROR;
	hr = m_pSampleGrabber->GetCurrentBuffer(&lSize, NULL);
	if (FAILED(hr))
	{
		lDatasize=0;
		return NULL;
	}
	if(lSize<=0)
	{
		lDatasize=0;
		return NULL;
	}
	lDatasize=lSize;
	if(lSize!=m_nBufferSize)
	{
		if (m_pDataBuffer)
		{
			delete[] m_pDataBuffer;
			m_pDataBuffer=NULL;
		}
		m_nBufferSize=lSize;
		m_pDataBuffer = new BYTE[m_nBufferSize];
	}
	hr = m_pSampleGrabber->GetCurrentBuffer(&lDatasize,(long*)m_pDataBuffer);
	if (FAILED(hr))
	{
		lDatasize=0;
		return NULL;
	}
	return (BYTE*)m_pDataBuffer;
}


void CCaptureAudio::OnThreadDeal()
{
	BYTE *pData=NULL;
	long lDatasize=0;
	if(m_pSampleGrabber==NULL)
	{
		lDatasize=0;
		return ;
	}
	long  lSize = 0;
	HRESULT hr=NOERROR;
	m_pSampleGrabber->GetCurrentBuffer(&lSize, NULL);
	if (FAILED(hr))
	{
		lDatasize=0;
		return;
	}
	if(lSize<=0)
	{
		lDatasize=0;
		return ;	
	}
	lDatasize=lSize;
	if(lSize!=m_nBufferSize)
	{
		if (m_pDataBuffer)
		{
			delete[] m_pDataBuffer;
			m_pDataBuffer=NULL;
		}
		m_nBufferSize=lSize;
		m_pDataBuffer = new BYTE[m_nBufferSize];
	}
	hr = m_pSampleGrabber->GetCurrentBuffer(&lDatasize,(long*)m_pDataBuffer);
	if (FAILED(hr))
	{
		lDatasize=0;
		return;
	}
	pData = m_pDataBuffer;
	if (m_sThreadCalbackInfo.realDataCalFunc&&m_sThreadCalbackInfo.pMaster&&pData&&lDatasize>0)
	{
		//ִ�����ݻص�
		m_sThreadCalbackInfo.realDataCalFunc(-1, pData, lDatasize, 
			(RealDataStreamType)m_nDataType,NULL, m_sThreadCalbackInfo.pMaster);
	}

}

void CCaptureAudio::SetDShowCaptureCallback(RealDataCallback realDataCalBack, void* pMaster)
{
	if(!m_bThread)//�ص�ģʽ
	{
		m_cSampleGrabberCB.m_realDataCallback = realDataCalBack;
		m_cSampleGrabberCB.m_pMaster = pMaster;
	}
	else//�̻߳���ģʽ
	{
		m_sThreadCalbackInfo.realDataCalFunc = realDataCalBack;
		m_sThreadCalbackInfo.pMaster = pMaster;
	}
}