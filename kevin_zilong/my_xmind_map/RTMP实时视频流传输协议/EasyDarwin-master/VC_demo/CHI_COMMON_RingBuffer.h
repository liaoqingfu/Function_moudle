
#ifndef _CHI_COMMON_RingBuffer_H_
#define _CHI_COMMON_RingBuffer_H_

#include "afxmt.h"
//#include "hi_pcdefs.h"

#define PLAY_AUDIO_MAX_RB_SIZE 1024 * 128
const TIMER_ACCURACY = 1;
#define SAFE_DELETE(p)  { if((p) != NULL) { delete (p);     (p) = NULL; } }   //Delete object by New create

typedef struct hi_AVSample_S
{
	HI_U32 u32TimeStamp;
	HI_U32 u32FrameTag;
}
HI_AUDIOSample, HI_VIDEOSample;

typedef enum RB_STAT_ENUM
{
    E_RB_TotalReadError,
    E_RB_TotalWriteError,

    E_RB_TotalWriteSize,
    E_RB_TotalReadSize,

    E_RB_TotalReadTimes,
    E_RB_TotalWriteTimes,

    E_RB_STAT_END
}RB_STAT_E;

class CHI_COMMON_RingBuffer
{
public:
    CHI_COMMON_RingBuffer();
    virtual ~CHI_COMMON_RingBuffer();

public:
    VOID RB_Init_Stat(void);
    VOID RB_Do_Stat(RB_STAT_E, HI_U64 value);
    VOID RB_Disp_Stat(void);

    HI_U32 RB_MaxReadSize(void);
    HI_U32 RB_MaxWriteSize(void);
    HI_U32 RB_MAXSize(void);

    HI_S32 RB_Init (HI_U32 ulSize);

    VOID RB_Clear(void);
    VOID RB_Destroy(void);

    HI_S32 RB_Write_X(HI_U8 *pData, HI_U32 ulDataLen,
                      HI_U32 u32TimeStamp, HI_U32 u32FrameTag, HI_U32 *pulWriteLen);
    HI_S32 RB_Read_X(HI_U8 *pData, HI_U32 *pulData);

    HI_U32 RB_GetLen_X(void);
    HI_U8 RB_GetFirstHI_U8(void);   /*��Readָ���ȡֵ*/
    HI_U16 RB_GetFirstHI_U16(void);  /*2Byte*/
    HI_U32 RB_GetFirstHI_U32(void);

    HI_U32 RB_GetMaxWritePercent(void);
    HI_U32 RB_GetMaxReadPercent(void);

    //Added for Private Data head
    HI_U32 RB_GetNodeNums(void);

    HI_U32 ReadPTS(HI_U32 ulReadPos);
    HI_S32 ReadCurPTS(HI_U32 *pulPTS);
    HI_S32 ReadNextPTS(HI_U32 *pulPTS);
    HI_U32 GetNextReadPos(void);
    HI_S32 SkipOneNodeReadPos(void);
    HI_VOID SetNodeNumVal(HI_BOOL bSign);

	HI_BOOL RB_GetPlayState();

private:
    CCriticalSection m_criticalSection;

    HI_VOID DirectCopyData(HI_U8 *pData,
                           HI_U32 ulWriteSize,
                           HI_U32 ulDataLen,
                           HI_U32 u32TimeStamp,
                           HI_U32 u32FrameTag);
    HI_VOID WrapCopyData(HI_U8 *pData,
                         HI_U32 ulWriteSize,
                         HI_U32 ulDataLen,
                         HI_U32 u32TimeStamp,
                         HI_U32 u32FrameTag);

    HI_U32 m_ulSize;			     /*Buffer ���ܳ���*/
    HI_U8 *m_pRB;				     /*Buffer�Ŀ�ʼ��ַ*/
    HI_U32 m_ulWritePos;		     /*д��λ��, ƫ��λ*/
    HI_U32 m_ulReadPos;		     /*����λ��, ƫ��λ*/
    HI_U64 m_stat[E_RB_STAT_END];
    HI_U16 m_usMediaType; 	         /*ý������*/

    HI_U32 m_ulNodeNums;			 /*�ڵ����*/

    HI_U32 m_s32Move4;
    HI_U32 m_s32Move8;

	HI_U32 m_u32RBsize;
	HI_BOOL m_bStartPlay;
	HI_U32 m_u32BufNumber;

    /**
     * ���½� CHI_COMMON_RingBuffer ��Ĭ�Ͽ�������� "=" ����������,
     * ��������Ȩ������Ϊprivate����,��ֹ�û�ʹ��Ĭ�Ͽ�������� "=" ������.
     * @see PROTECTMIC
     */
    PROTECTMIC(CHI_COMMON_RingBuffer);
};


#endif //_CHI_COMMON_RingBuffer_H_
