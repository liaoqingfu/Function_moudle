#ifndef _CHI_AO_MM_H_
#define _CHI_AO_MM_H_

#include "CHI_COMMON_RingBuffer.h"
#include "HI_AI_Buffer.h"
#include "CHI_ADEC_DEC.h"

#define PLAY_AUDIO_BUF_NUM 5

class CHI_AO_MM
{
public:
    CHI_AO_MM();
    virtual ~CHI_AO_MM();
    //the A/V module call the function to get the time-stamp
    HRESULT HI_AO_SetOutFormat(PLAYER_ATTR_AUDIO_S* pstruAStreamParam);
    HRESULT HI_AO_GetOutFormat(PLAYER_ATTR_AUDIO_S *pAStreamParam);
    HRESULT HI_AO_Play(HI_U32 u32Frequency);
    HRESULT HI_AO_Stop(HI_VOID);
    HRESULT HI_AO_SetMute(HI_BOOL bMute);
    HRESULT HI_AO_GetMuteState(HI_BOOL *pMuteState);
    HI_S32 HI_AO_ClearRb(HI_VOID);
	HRESULT HI_ADEC_InPutAudioData(HI_U8 *pBuffer, HI_S32 s32DataLen, HI_U64 u64TimeStamp);

public:
    //output audio
    HI_S32 AudioStart(HI_VOID);
    HI_S32 AudioOut(HWAVEOUT hwo, CHI_AO_MM *pAudioOut, WAVEHDR *pWaveHdr);
    HI_S32 AudioStop(HI_VOID);
    //we should update the time-stamp value when audio out
    HI_S32 SetAudioPTS(HI_U8 **pU8Data);
    HI_S32 SetAudioPTSVal(HI_VOID);
    HI_S32 AddNewHeader(HWAVEOUT hwo, HI_AI_Buffer *buffer, HI_S32 s32Index);
    HI_S32 AddNewHeader(HWAVEOUT hwo, HI_AI_Buffer *buffer, WAVEHDR *pWaveHead);
    //HI_S32      HI_AoOutput_GetCtrl(CHsPlayerCtrl *pCtrl);

    HI_U32 m_u32TSArray[PLAY_AUDIO_BUF_NUM];   /*ʱ�������*/
    HI_U32	m_u32ReadLen;       /*ÿ�δ�Rb�ж�ȡ����Ƶ���ݳ���*/
    HI_S32 m_s32RightNum;       /*ͬ����������*/
    HI_S32 m_s32LeftNum;        /*ͬ����������*/
    HI_U64 m_u64TimeStamp;      /*��ǰ���ŵ���Ƶ���ݿ��ʱ���*/
	HI_U64 m_u64CurTimeStamp;
	HI_S32 m_s32NeedPkNum;		/*ͨ����ƵRB������Ƶ��RB����*/
    HI_S32 m_s32SavePackage;    /*��ƵRbԤ�洢����Ƶ������*/
    HI_S32 m_s32PackageNum;     /*ÿ�δ�Rb�з��ص���Ƶ������*/
    HI_S32 m_s32Number;         /*ʱ�������������ǰʱ�����ʱ���������������*/
    HI_BOOL m_bMuteState;       /*����״̬*/
    HI_U8 *m_pAudioData;        /*ÿ�δ�Rb�ж�ȡ����Ƶʱ���*/
    UINT m_uTimerID;            /*��Ƶ����������߳�ID*/
    MMRESULT	m_mmr;
    volatile HI_BOOL m_bPlaying;  /*��ǰ�Ƿ�Ϊ����״̬*/
    HANDLE m_hEvent;              //�¼��ź�
    CRITICAL_SECTION m_critSection;  //�ٽ�
    WAVEHDR	*m_pWaveHead;       /*��Ƶ����ͷ������*/
    CHI_COMMON_RingBuffer m_COMMONRingBuffer; /*��ƵRb����*/
	CHI_ADEC_DEC *m_pAudioDec;
//    CHI_AO_Initial m_AOInitial;        /*��ʼ�������*/
    //	FILE*	m_pDumpFIle;
private:
    HI_S32	CloseData(HI_VOID);
    HI_S32	CloseDevice(HI_VOID);
    HI_S32	UnperpareBuffer(HI_VOID);
    HI_S32	OpenDevice(HI_VOID);
    HI_S32	PrepareHeader(HI_VOID);
    WAVEOUTCAPS* GetDeviceCap(HI_VOID);

    DWORD m_dwVolume;         /*��ǰ��Ƶ��������*/
    HI_BOOL	m_bBuffer;
    HI_BOOL	m_bDevOpen;         /*�����豸�Ƿ��*/

    WAVEFORMATEX m_pWaveFormat;     /*��Ƶ�����ʽ*/
    WAVEOUTCAPS	m_struWaveCaps;
    HWAVEOUT m_hWaveOut;         /*����豸���*/

    HI_S32 m_s32PlayStamp;     /*���ŵ�ʱ������*/
    HI_BOOL m_bFistLostPack;
    UINT m_uAccuracy;
    //HI_S32			    m_s32IndexWaveHdr;  /*���ݿ�������*/

    //��ͣ��ָ����ű�ʶ
    HI_BOOL m_bPause;
	HI_BOOL m_bStream;

    /**
     * ���½� CHI_AO_MM ��Ĭ�Ͽ�������� "=" ����������,
     * ��������Ȩ������Ϊprivate����,��ֹ�û�ʹ��Ĭ�Ͽ�������� "=" ������.
     * @see PROTECTMIC
     */
    PROTECTMIC(CHI_AO_MM);
};


#endif //_CHI_AO_MM_H_
