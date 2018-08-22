#include "StdAfx.h"
#include "CHI_AO_MM.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHI_AO_MM::CHI_AO_MM()
{
    m_bDevOpen	= HI_FALSE;
    m_bBuffer	= HI_FALSE;
    m_bPlaying	= HI_FALSE;
    m_hEvent	= NULL;
    m_pWaveHead = NULL;

    m_u32ReadLen = 0;
    m_u64TimeStamp = 0;
	m_u64CurTimeStamp = 0;
	m_s32PackageNum = 0;
    m_s32LeftNum = 0;
    m_s32NeedPkNum = PLAY_AUDIO_PACKAGE_NUM_NORMAL;
    m_s32PlayStamp = 0;
    m_s32Number = 0;
    m_s32PlayStamp = 0;
    m_uTimerID = 0;
    m_hWaveOut = 0;
    m_mmr = 0;
    m_bMuteState = HI_FALSE;
    m_s32RightNum = 150;//PLAY_AUDIO_PACKAGE_NUM;
    m_s32SavePackage = 0;
    /*��Ƶ���ݴ洢������*/
    m_pAudioData = NULL;
    m_pAudioData = new HI_U8[PLAY_AUDIO_FRAME_SIZE + 1];
    if (NULL != m_pAudioData)
    {
        memset(m_pAudioData, 0, PLAY_AUDIO_FRAME_SIZE);
    }
    
    memset(&m_struWaveCaps, 0, sizeof(m_struWaveCaps));
    memset((HI_VOID*)m_u32TSArray, 0, PLAY_AUDIO_BUF_NUM * sizeof(HI_U32));
    
    /*��ʼ����ƵRb*/
    m_COMMONRingBuffer.RB_Init(PLAY_AUDIO_MAX_RB_SIZE);

    /*��ȡ��ǰϵͳ����*/
    waveOutGetVolume(0, &m_dwVolume);

    //Ĭ�Ϸ���ͣ
    m_bPause = HI_FALSE;

    m_pWaveFormat.cbSize = 0; //sizeof(WAVEFORMATEX);
    m_pWaveFormat.nChannels = 1;
    m_pWaveFormat.nSamplesPerSec = 8000;
    m_pWaveFormat.wBitsPerSample = 16;
    m_pWaveFormat.wFormatTag = WAVE_FORMAT_PCM;
    m_pWaveFormat.nBlockAlign = 2;
    m_pWaveFormat.nAvgBytesPerSec = 16000;

    TIMECAPS tcaps;
    if (timeGetDevCaps(&tcaps, sizeof(TIMECAPS)) == TIMERR_NOERROR)
    {
        m_uAccuracy = min(max(tcaps.wPeriodMin, TIMER_ACCURACY), tcaps.wPeriodMax);
        timeBeginPeriod(m_uAccuracy);
    }

	m_pAudioDec = new CHI_ADEC_DEC();
    
    m_bFistLostPack = HI_FALSE;
    InitializeCriticalSection(&m_critSection);
}

CHI_AO_MM::~CHI_AO_MM()
{
    /*�˳�ǰִ�йرա���ղ���*/
    AudioStop();

    SAFE_DELETEA(m_pAudioData);
    m_COMMONRingBuffer.RB_Destroy();
    
    if (NULL != m_hEvent)
    {
        CloseHandle(m_hEvent);
        m_hEvent = NULL;
    }

	if(NULL != m_pAudioDec)
	{
		delete m_pAudioDec;
		m_pAudioDec = NULL;
	}

    timeEndPeriod(m_uAccuracy);
    DeleteCriticalSection(&m_critSection);
}

//////////////////////////////////////////////////////////////////////////
/************************************************
* ������	     : CHI_AO_MM::HI_AO_Play
* ��  ��  	     : ��Ƶ����
* ��������       : HI_S32 
* ����           : UINT uFrequency
* �����         : lizhaozao 
* ����޶�ʱ��   : 2008-1-18 17:12:47
************************************************/

HRESULT CHI_AO_MM::HI_AO_Play(HI_U32 u32Frequency)
{
    if (HI_TRUE == m_bPause)
    {
        waveOutRestart(m_hWaveOut);
        m_bPause = HI_FALSE;
        return HI_SUCCESS;
    }

    if (HI_TRUE == m_bPlaying)
    {
        /*�Ѿ����ڲ���״̬*/
        return HI_SUCCESS;
    }

    AudioStart();
    m_bFistLostPack = HI_TRUE;
    return HI_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/************************************************
* ������	     : CHI_AO_MM::HI_AO_Stop
* ��  ��  	     : ֹͣ��Ƶ����
* ��������       : HI_S32 
* ����           : HI_VOID
* �����         : lizhaozao 
* ����޶�ʱ��   : 2008-1-18 17:13:42
************************************************/

HRESULT CHI_AO_MM::HI_AO_Stop(HI_VOID)
{
    if (HI_FALSE == m_bPlaying)
    {
        /*û�п������ţ�����ִ��stop����*/
        return HI_SUCCESS;
    }

    m_bPause = HI_FALSE;
    HI_S32 s32Rel = AudioStop();
    m_bFistLostPack = HI_FALSE;
    return s32Rel;
}



//////////////////////////////////////////////////////////////////////////
/************************************************
* ������	     : CHI_AO_MM::HI_AO_SetMute
* ��  ��  	     : ���þ���״̬
* ��������       : HI_S32 
* ����           : HI_S32 s32Mute
* �����         : lizhaozao 
* ����޶�ʱ��   : 2008-1-18 17:12:57
************************************************/
HRESULT CHI_AO_MM::HI_AO_SetMute(HI_BOOL bMute)
{
    if ((HI_FALSE != bMute) && (HI_TRUE != bMute))
    {
        /*�Ƿ��������*/
        return HI_ERR_INVALID_ARGUMENTS;
    }

    if (bMute == m_bMuteState)
    {
        /*��ǰ״̬�����û������״̬*/
        return HI_SUCCESS;
    }

    m_bMuteState = bMute;

    if (HI_FALSE == m_bMuteState)
    {
        /*����ǴӾ���ת�����Ǿ���״̬�����������ò�������*/
        if (!m_hWaveOut)
        {
            return HI_SUCCESS;
        }

        m_mmr = waveOutSetVolume(m_hWaveOut, m_dwVolume);
        if (MMSYSERR_NOERROR != m_mmr)
        {
            return HI_ERR_ABORTED_OPERATION;
        }
    }
    return HI_SUCCESS;
}

HRESULT CHI_AO_MM::HI_AO_SetOutFormat(PLAYER_ATTR_AUDIO_S* struAStreamParam)
{
    m_pWaveFormat.cbSize = 0; //sizeof(WAVEFORMATEX);
    m_pWaveFormat.nChannels = (WORD)struAStreamParam->lChannels;
    m_pWaveFormat.nSamplesPerSec = (DWORD)struAStreamParam->lSamplesPerSec;
    m_pWaveFormat.wBitsPerSample = 16;
    m_pWaveFormat.wFormatTag = WAVE_FORMAT_PCM;
    m_pWaveFormat.nBlockAlign = m_pWaveFormat.nChannels * 2;
    m_pWaveFormat.nAvgBytesPerSec =
    m_pWaveFormat.nSamplesPerSec * m_pWaveFormat.nBlockAlign;

	m_pAudioDec->HI_ADEC_Init(struAStreamParam, struAStreamParam);
    return HI_SUCCESS;
}

HRESULT CHI_AO_MM::HI_AO_GetOutFormat(PLAYER_ATTR_AUDIO_S *pAStreamParam)
{
    if (NULL == pAStreamParam)
    {
        return HI_ERR_NULL_POINTER;
    }

    pAStreamParam->lChannels = m_pWaveFormat.nChannels;
    pAStreamParam->lSamplesPerSec = m_pWaveFormat.nSamplesPerSec;
    pAStreamParam->lBitsPerSample = m_pWaveFormat.wBitsPerSample;
    pAStreamParam->lBlockAlign = m_pWaveFormat.nBlockAlign;

    return HI_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/************************************************
* ������	     : CHI_AO_MM::HI_AO_ClearRb
* ��  ��  	     : �����Ƶrb
* ��������       : HI_S32 
* ����           : HI_VOID
* �����         : lizhaozao 
* ����޶�ʱ��   : 2007-1-16 21:15:03
************************************************/
HI_S32 CHI_AO_MM::HI_AO_ClearRb(HI_VOID)
{
    if (HI_TRUE == m_bPlaying)
    {
        return HI_ERR_INVALID_OPERATION;
    }
    /*���Rb,���rb������*/
    m_COMMONRingBuffer.RB_Clear();
    return HI_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/************************************************
* ������	     : CHI_AO_MM::HI_AO_GetMuteState
* ��  ��  	     : ��ȡ����״̬
* ��������       : HI_S32 
* ����           : LONG *pMuteState
* �����         : lizhaozao 
* ����޶�ʱ��   : 2008-1-18 17:11:55
************************************************/
HRESULT CHI_AO_MM::HI_AO_GetMuteState(HI_BOOL *pMuteState)
{
    if (NULL == pMuteState)
    {
        return HI_ERR_NULL_POINTER;
    }
    *pMuteState = m_bMuteState;

    return HI_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////

WAVEOUTCAPS* CHI_AO_MM::GetDeviceCap(HI_VOID)
{
    m_mmr = waveOutGetDevCaps(WAVE_MAPPER, &m_struWaveCaps, sizeof(m_struWaveCaps));
    if ( MMSYSERR_NOERROR != m_mmr )
    {
        return &m_struWaveCaps;
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////////

HI_S32 CHI_AO_MM::AudioStart(HI_VOID)
{
    HI_S32 s32Rel = 0;
    /*��ǰ�Ƿ��Ѿ����ڲ���״̬*/
    if (HI_FALSE == m_bPlaying)
    {
        /*�򿪲����豸*/
        s32Rel = OpenDevice();
        if (HI_SUCCESS != s32Rel)
        {
            return s32Rel;
        }
        /*Ϊ����׼�����ݿ飬��Ϊ���õ���wave���ţ�����ǰ��׼�����ݿ�*/
        s32Rel = PrepareHeader();
        if (HI_SUCCESS != s32Rel)
        {
            return s32Rel;
        }
        
        EnterCriticalSection(&m_critSection);
        m_bPlaying = HI_TRUE;
        LeaveCriticalSection(&m_critSection);
        if (NULL == m_hEvent)
        {
            m_hEvent = CreateEvent(
                NULL,               // no security attributes
                TRUE,               // manual-reset event
                FALSE,              // initial state is signaled
                NULL          // object name
                );
        }
    }
    else
    {   /*��ǰ�Ѿ����ڲ���״̬*/
        return HI_SUCCESS;
    }

    return HI_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////

HI_S32 CHI_AO_MM::AudioStop(HI_VOID)
{
    HI_S32 s32Rel = HI_SUCCESS;
    /*��ǰ�Ƿ��ڲ���״̬*/
    if (HI_TRUE == m_bPlaying )
    {
        /*�Ƚ����ű�־Ϊ��Ϊfalse*/
        EnterCriticalSection(&m_critSection);
        m_bPlaying = HI_FALSE;
        LeaveCriticalSection(&m_critSection);

        WaitForSingleObject(m_hEvent, INFINITE);
        ResetEvent(m_hEvent);

        Sleep(50);
        /*�ر�����*/
        s32Rel = CloseData();
        Sleep(30);
        /*�������ݿ�*/
        s32Rel = UnperpareBuffer();
        Sleep(10);
        /*�رմ򿪵Ĳ����豸*/
        s32Rel = CloseDevice();
        if (HI_SUCCESS != s32Rel)
        {
            return s32Rel;
        }
        Sleep(10);
        /*��ʱ�������Ϊ0*/
        memset((HI_VOID*)m_u32TSArray, 0, sizeof(HI_U32) * PLAY_AUDIO_BUF_NUM);
        m_u64TimeStamp = 0;
		m_u64CurTimeStamp = 0;
    }
    else
    {
        m_bPlaying = HI_FALSE;
        return HI_ERR_ABORTED_OPERATION;
    }

    return s32Rel;
}

//////////////////////////////////////////////////////////////////////////

HI_S32 CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, \
                            DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
    if (WOM_DONE == uMsg)
    {
        WAVEHDR *pWaveHdr = (WAVEHDR *)dwParam1;
        CHI_AO_MM *pAudioOut = (CHI_AO_MM *)(pWaveHdr->dwUser);
        HI_S32 s32Rel = 0;
        if (NULL == pAudioOut)
        {
            return HI_FAILURE;
        }

        /*ÿ������һ�����ݿ飬�ڸú������յ�һ��wave_done��Ϣ�������������*/
        /*���µ�ǰ��ʱ���*/
        pAudioOut->SetAudioPTSVal();
		//if(pAudioOut->m_u64TimeStamp > 0)

        EnterCriticalSection(&pAudioOut->m_critSection);
        if (HI_TRUE == pAudioOut->m_bPlaying)
        {
            LeaveCriticalSection(&pAudioOut->m_critSection);
            if (NULL != pWaveHdr && NULL != hwo)
            {
				//if(!pAudioOut->m_COMMONRingBuffer.RB_GetPlayState())
				//	return HI_ERR_INVALID_ARGUMENTS;
                //���
//				//if(pAudioOut->m_COMMONRingBuffer.GetRBuffer()){
                s32Rel = pAudioOut->AudioOut(hwo, pAudioOut, pWaveHdr);
                if (HI_SUCCESS != s32Rel)
                {
                    return s32Rel;
                }
				//}
            } //end-if3
            else
            {
                return HI_ERR_INVALID_ARGUMENTS;
            }
        } //end-if4
        else
        {
            LeaveCriticalSection(&pAudioOut->m_critSection);
            SetEvent(pAudioOut->m_hEvent);
            return HI_ERR_INVALID_ARGUMENTS;
        }
    } //end if5
    else
    {
        return HI_ERR_INVALID_ARGUMENTS;
    }
    return HI_SUCCESS;
}

HI_S32 CHI_AO_MM::AudioOut(HWAVEOUT hwo, CHI_AO_MM *pAudioOut, WAVEHDR *pWaveHdr)
{
    HI_S32 s32Rel;
    if (NULL == pAudioOut->m_pAudioData)
    {
        return HI_ERR_INVALID_ARGUMENTS;
    }
	
    pAudioOut->m_mmr = waveOutUnprepareHeader(hwo, pWaveHdr, sizeof(WAVEHDR));
    if (MMSYSERR_NOERROR != pAudioOut->m_mmr)
    {
        return HI_ERR_INVALID_ARGUMENTS;
    }
	
    if (pWaveHdr->dwBufferLength > PLAY_AUDIO_FRAME_SIZE)
    {
        /*��ֹ����Խ�����*/
        pWaveHdr->dwBufferLength = PLAY_AUDIO_FRAME_SIZE;
    }
    /*�������ݿռ�*/
    HI_AI_Buffer buff(pWaveHdr->lpData, pWaveHdr->dwBufferLength);
    /*��ȡ��ǰ��rb�е���Ƶ������*/
    pAudioOut->m_s32PackageNum = (HI_S32)pAudioOut->m_COMMONRingBuffer.RB_GetNodeNums();
	
    /*�жϵ�ǰ��Ƶ�����������ķ�Χ��С�����޲���������*/
    if (pAudioOut->m_s32PackageNum <= pAudioOut->m_s32LeftNum)
    {
        buff.m_u32ByteLen = pWaveHdr->dwBufferLength;
        if (buff.m_u32ByteLen > PLAY_AUDIO_FRAME_SIZE)
        {
            /*��ֹԽ��*/
            buff.m_u32ByteLen = PLAY_AUDIO_FRAME_SIZE;
        }
        memset(buff.ptr.u8, 0, buff.m_u32ByteLen);
        /*��ʱ������鶼��0*/
        memset((HI_VOID*)(pAudioOut->m_u32TSArray), 0, PLAY_AUDIO_BUF_NUM * sizeof(HI_U32));
        pAudioOut->m_u64TimeStamp = 0;
        pAudioOut->AddNewHeader(hwo, &buff, pWaveHdr);
		//#endif
    }
    else
    {   /*�������ޣ��ȶ���һ����Ƶ��*/
        if (pAudioOut->m_s32PackageNum >= pAudioOut->m_s32RightNum)
        {
            TRACE("......Lost a Audio\r\n");
            HI_S32 s32Lost = pAudioOut->m_s32PackageNum - 4;//mod ny glping
            for (HI_S32 i = 0; i <s32Lost; i++)
            {
                pAudioOut->m_COMMONRingBuffer.SkipOneNodeReadPos();
            }
        }
		
        /*����Ƶ������������ݿ�*/
        s32Rel = pAudioOut->m_COMMONRingBuffer.RB_Read_X(pAudioOut->m_pAudioData, \
			&(pAudioOut->m_u32ReadLen));
        if (HI_SUCCESS == s32Rel)
        {
            //����ʱ���RingBuf�а��϶ֱ࣬���Ե�ֻʣ3����
            if (HI_TRUE == m_bFistLostPack && pAudioOut->m_s32PackageNum > 5)
            {
                m_bFistLostPack = HI_FALSE;
                TRACE("......Lost a lot of Audio\r\n");
                HI_S32 s32Lost = pAudioOut->m_s32PackageNum - PLAY_AUDIO_PACKAGE_NUM_NORMAL;
                for (HI_S32 i = 0; i < s32Lost; i++)
                {
                    pAudioOut->m_COMMONRingBuffer.SkipOneNodeReadPos();
                }
            }
            buff.m_u32ByteLen = pAudioOut->m_u32ReadLen - sizeof(HI_AUDIOSample);
			if (HI_FALSE == m_bMuteState)
			{
				memcpy(buff.ptr.u8, pAudioOut->m_pAudioData + sizeof(HI_AUDIOSample), \
					buff.m_u32ByteLen);
			}
			else
			{
				memset(buff.ptr.u8, 0, buff.m_u32ByteLen);
			}
            /*����ʱ���*/
            pAudioOut->SetAudioPTS(&pAudioOut->m_pAudioData);
            /*Ϊ�����豸������ݿ�*/
            pAudioOut->AddNewHeader(hwo, &buff, pWaveHdr);
        } //end-if1
        else
        {
            return HI_ERR_INVALID_ARGUMENTS;
        }
    } //end-if2
	
    return HI_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////

HI_S32 CHI_AO_MM::SetAudioPTS(HI_U8 **pData)
{
    if (NULL == *pData)
    {   /*�������*/
        return HI_ERR_NULL_POINTER;
    }

    HI_U32 timeStamp;
    /*���ݿ��ǰ���˳�򲥷ŵģ����ѭ������ʱ���*/
    memcpy((VOID *)(&timeStamp), (VOID *)(*pData), sizeof(HI_U32));

    m_s32Number = m_s32Number % PLAY_AUDIO_BUF_NUM;
    m_u32TSArray[m_s32Number] = timeStamp;
    m_s32Number = m_s32Number + 1;
	m_u64CurTimeStamp = timeStamp;

    return HI_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////

HI_S32 CHI_AO_MM::SetAudioPTSVal(HI_VOID)
{
    m_s32PlayStamp = m_s32PlayStamp % PLAY_AUDIO_BUF_NUM;
    m_u64TimeStamp = m_u32TSArray[m_s32PlayStamp];
    m_s32PlayStamp = m_s32PlayStamp + 1;

    return HI_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////

HI_S32 CHI_AO_MM::CloseData(HI_VOID)
{
    if ( !m_hWaveOut )
    {
        return HI_ERR_ABORTED_OPERATION;
    }
    /*�豸����*/
    m_mmr = waveOutReset(m_hWaveOut);
    if ( MMSYSERR_NOERROR != m_mmr )
    {
        return HI_ERR_ABORTED_OPERATION;
    }
    return HI_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////

HI_S32 CHI_AO_MM::CloseDevice(HI_VOID)
{
    if (HI_FALSE == m_bDevOpen)
    {
        return HI_ERR_INVALID_OPERATION;
    }

    if (!m_hWaveOut)
    {
        return HI_ERR_ABORTED_OPERATION;
    }
    /*�ر��豸*/
    m_mmr = waveOutClose(m_hWaveOut);
    if ( MMSYSERR_NOERROR != m_mmr )
    {
        return HI_ERR_ABORTED_OPERATION;
    }
    /*���ñ�־λ��ֵ*/
    m_hWaveOut = 0;
    m_bDevOpen = HI_FALSE;

    return HI_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////

HI_S32 CHI_AO_MM::UnperpareBuffer(HI_VOID)
{
    if (HI_FALSE == m_bBuffer )
    {
        return HI_ERR_INVALID_ARGUMENTS;
    }

    for (HI_S32 i = 0; i < PLAY_AUDIO_BUF_NUM; i++)
    {
        /*ѭ���������ݿ�*/
        m_mmr = waveOutUnprepareHeader(m_hWaveOut, &m_pWaveHead[i], sizeof(WAVEHDR));
        if ( MMSYSERR_NOERROR != m_mmr )
        {
            continue;
        }
        SAFE_DELETE(m_pWaveHead[i].lpData);
    }
    SAFE_DELETE(m_pWaveHead);

    m_bBuffer = HI_FALSE;
    return HI_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////

HI_S32 CHI_AO_MM::AddNewHeader(HWAVEOUT hwo, HI_AI_Buffer *buffer, HI_S32 s32Index)
{
    /*�������ݿ������*/
    m_pWaveHead[s32Index].lpData = buffer->ptr.c;
    m_pWaveHead[s32Index].dwBufferLength = buffer->m_u32ByteLen;
    m_pWaveHead[s32Index].dwFlags = 0;
    m_pWaveHead[s32Index].dwLoops = 1;
    m_pWaveHead[s32Index].dwUser = (DWORD)(HI_VOID*)this;
    /*׼�����ݿ�*/
    m_mmr = waveOutPrepareHeader(hwo, &m_pWaveHead[s32Index], sizeof(WAVEHDR));
    if ( MMSYSERR_NOERROR != m_mmr )
    {
        return HI_ERR_ABORTED_OPERATION;
    }

    m_mmr = waveOutWrite(hwo, &m_pWaveHead[s32Index], sizeof(WAVEHDR));
    if ( MMSYSERR_NOERROR != m_mmr )
    {
        waveOutUnprepareHeader(hwo, &m_pWaveHead[s32Index], sizeof(WAVEHDR));
        m_pWaveHead[s32Index].lpData = NULL;
        m_pWaveHead[s32Index].dwBufferLength = 0;
        m_pWaveHead[s32Index].dwFlags = 0;
        m_pWaveHead[s32Index].dwUser = NULL;
        return HI_ERR_ABORTED_OPERATION;
    }

    return HI_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////

HI_S32 CHI_AO_MM::AddNewHeader(HWAVEOUT hwo, HI_AI_Buffer *buffer, WAVEHDR *pWaveHead)
{
    pWaveHead->lpData = buffer->ptr.c;
    pWaveHead->dwBufferLength = buffer->m_u32ByteLen;
    pWaveHead->dwFlags = 0;
    pWaveHead->dwLoops = 1;
    pWaveHead->dwUser = (DWORD)(HI_VOID*)this;
    /*׼�����ݿ�*/
    m_mmr = waveOutPrepareHeader(hwo, pWaveHead, sizeof(WAVEHDR));
    if ( MMSYSERR_NOERROR != m_mmr )
    {
        return HI_ERR_ABORTED_OPERATION;
    }
    /*���ݿ���ӵ������豸*/
    m_mmr = waveOutWrite(hwo, pWaveHead, sizeof(WAVEHDR));
    if ( MMSYSERR_NOERROR != m_mmr )
    {
        waveOutUnprepareHeader(hwo, pWaveHead, sizeof(WAVEHDR));
        pWaveHead->lpData = NULL;
        pWaveHead->dwBufferLength = 0;
        pWaveHead->dwFlags = 0;
        pWaveHead->dwUser = NULL;
        return HI_ERR_ABORTED_OPERATION;
    }

    return HI_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////

HI_S32 CHI_AO_MM::OpenDevice(HI_VOID)
{
    if (HI_TRUE == m_bDevOpen)
    {
        return HI_ERR_INVALID_OPERATION;
    }
    /*����Ƶ�����豸����ָ���ص�����*/
    m_mmr = waveOutOpen(0, WAVE_MAPPER, &m_pWaveFormat, 0, 0, WAVE_FORMAT_QUERY);
    if ( MMSYSERR_NOERROR != m_mmr )
    {
        return HI_ERR_ABORTED_OPERATION;
    }

    m_mmr = waveOutOpen(&m_hWaveOut, WAVE_MAPPER, &m_pWaveFormat, (DWORD)waveOutProc, \
                        (DWORD)this, CALLBACK_FUNCTION);
    if ( MMSYSERR_NOERROR != m_mmr )
    {
        return HI_ERR_ABORTED_OPERATION;
    }

    if (HI_TRUE == m_bMuteState)
    {
        /*�������ʱΪ�����򽫲���״̬���ó�Ϊ����״̬*/
        HI_AO_SetMute(HI_TRUE);
    }
    else
    {
        WORD wLeftVolume = (WORD)(m_dwVolume & 0x0000FFFF);
        WORD wRightVolume = (WORD)((m_dwVolume & 0xFFFF0000) >> 16);
    }
    m_bDevOpen = HI_TRUE;

    return HI_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////

HI_S32 CHI_AO_MM::PrepareHeader(HI_VOID)
{
    if (HI_TRUE == m_bBuffer)
    {
        return HI_ERR_INVALID_ARGUMENTS;
    }
    HI_S32 s32Rel = HI_SUCCESS;
    /*���ò����豸*/
    m_mmr = waveOutReset(m_hWaveOut);
    if ( MMSYSERR_NOERROR != m_mmr )
    {
        return HI_ERR_ABORTED_OPERATION;
    }

    if (NULL == m_pWaveHead)
    {
        /*���������žݿ�*/
        m_pWaveHead = new WAVEHDR[PLAY_AUDIO_BUF_NUM];
        if (NULL == m_pWaveHead)
        {
            return HI_FAILURE;
        }

        for (HI_S32 i = 0; i < PLAY_AUDIO_BUF_NUM; i++)
        {
            HI_AI_Buffer buff(PLAY_AUDIO_FRAME_SIZE, HI_FALSE);
            /*����������ݿ�*/
            buff.m_u32ByteLen = 0;
            memset(buff.ptr.u8, 0, buff.m_u32ByteLen);
            s32Rel = HI_SUCCESS;

            /*�������ʱ���*/
            SetAudioPTS(&m_pAudioData);
            /*������ݿ�*/
            s32Rel = AddNewHeader(m_hWaveOut, &buff, i);
            if ( HI_SUCCESS != s32Rel )
            {
                return s32Rel;
            }
        } //end-for
        /*���õ�һ��ʱ���*/
        SetAudioPTSVal();
    } //end-if
    else
    {
        return HI_ERR_NULL_POINTER;
    }
    m_bBuffer = HI_TRUE;

    return s32Rel;
}

HRESULT CHI_AO_MM::HI_ADEC_InPutAudioData(HI_U8 *pBuffer, HI_S32 s32DataLen, HI_U64 u64TimeStamp)
{
    if (NULL == pBuffer //|| NULL == m_COMMONRingBuffer
        || NULL == m_pAudioDec)
    {
        return HI_ERR_NULL_POINTER;
    }

	HI_U8  u8Buffer[1024] = {0};
	HI_U32 u32WriteSize;
	
    if (s32DataLen <= 0 || s32DataLen > PLAY_AUDIO_FRAME_SIZE)
    {
        return HI_ERR_INVALID_ARGUMENTS;
    }
	
    //if (HI_TRUE == m_bMute)
    //{
	//����ʱ����������
    //    return HI_ERR_ABORTED_OPERATION;
    //}
	
    /*����*/
    HI_S32 s32OutDecLen = 0;
	HI_U32 u32MaxWriteLength = 0;
	u32MaxWriteLength = m_COMMONRingBuffer.RB_MaxWriteSize();
	//if(u32MaxWriteLength < (HI_U32)(m_strOutPutAttr.lBitRate/(8*50)+8))
	//{
	//	return HI_ERR_OVERFLOW_MEMORY;
	//}
	
    HI_S32 s32Rec = m_pAudioDec->HI_ADEC_Decode(pBuffer, s32DataLen, u8Buffer, &s32OutDecLen);
    if (HI_SUCCESS == s32Rec && s32OutDecLen < PLAY_AUDIO_FRAME_SIZE)
    {
        /*����������Ƶ����д��Rb*/
        HI_S32 s32Rel = m_COMMONRingBuffer.RB_Write_X(u8Buffer,
			s32OutDecLen,
			(HI_U32)u64TimeStamp,
			0,
			&u32WriteSize);
        return s32Rel;
    }
    else
    {
        return s32Rec;
    }
}


//////////////////////////////////////////////////////////////////////////
