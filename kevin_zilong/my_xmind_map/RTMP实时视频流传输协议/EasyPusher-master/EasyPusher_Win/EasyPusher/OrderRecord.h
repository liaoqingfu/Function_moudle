/*
	Copyright (c) 2013-2016 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.EasyDarwin.org
*/
//  [6/14/2016 SwordTwelve] 
#pragma once
class COrderRecordInfo  
{
public:
	COrderRecordInfo();
	virtual ~COrderRecordInfo();
public:
	int id;			//id ����
	CString name;	//����
	COleDateTime starttime; //��ʼʱ��
	COleDateTime endtime;	//����ʱ��
	CString strStartTimeString;//��ʼʱ�䣨�ִ���
	CString strEndTimeString; //����ʱ�䣨�ִ���
	int	isenable;			//�Ƿ����
	CString intro;			//����

	int nState;				//��־ 0--�ƻ�����,1--ÿ�� 
	int nRunState;			//���б�־״̬��-1=���ڣ�0=�ȴ���1=�����У�2=����ɣ�
	CString property1;
	CString property2;
	CString property3;
};

class COrderRecord
{
public:
	COrderRecord(void);
	~COrderRecord(void);

public:
	CList<COrderRecordInfo,COrderRecordInfo&> m_listOrderRecord;
	BOOL m_bUserOrderRecord;
	BOOL m_nOrderRecordType;//0---�����ϵļ򵥶�ʱ¼��,1---access���ݿ�Ķ�ʱ¼��,2---sql���ݿ�Ķ�ʱ¼������
	int  m_nSubTime;
	BOOL m_bStartLoadList;
	int m_nManualTime;
	CString m_strOrderInfoPath;
	int m_nCheckTimer;
	int m_nCurManualTime;
	int m_nCurRecordIndex;
	int m_nCurRecordType;
	int m_nCurRecordTime;
	BOOL m_bStopManual;//�Ƿ񰴼ƻ�ֹͣ�ֶ�¼�Ʋ���
private:
	CString ReadKeyValueFromS(CString strKey,CString strValue);
	void CString2OrderRecordInfo(CString strValue,COrderRecordInfo &tmpOrderRecordInfo);
	int GetParamData(CString strValue,CStringArray &arrayValue,CString strFlag);
	CString GetArrayData(CStringArray &array,int nIndex);
	
public:
	COleDateTime CString2CTimer(CString strTimer);
	BOOL LoadOrderRecordData(CString strOrderRcPath);
	int LoadOrderRecordList(CString strOrderInfoPath=_T(""));
	int CheckIsRecordFlag(int nCurrentRecordLen,int nRecordType);
	int GetCurOrderPlanInfo(CString& sName, CString& sStartTime, CString& sEndTime, CString &strFilePath1,CString &strFilePath2,CString &strFilePath3);

	int CheckIsRecordFlag_List(COleDateTime  curTime,int nCheckType,CUIntArray  &arrayTimeIn);

	int CheckIsRecordFlag_Index(COleDateTime  curTime,int nCheckType,int nCurIndex);
	void SetOrderRecordFlag(int nRecFlag);
	int CheckIsListFlag(int nCurRecFlag, int& nActiveOrderId);
	BOOL CheckOrderIni(CString strPath);
	int LoadOrderRecordList(CString strOrderInfoPath,CList<COrderRecordInfo,COrderRecordInfo&> &listOrderRecord);
	int GetOrderRecInfoById(int nIndex, COrderRecordInfo& tmpOrderRecordInfo);
	int SetOrderRecListRunState(int nIndex, int nRunState);
	int UpdateListInfoId();
};
