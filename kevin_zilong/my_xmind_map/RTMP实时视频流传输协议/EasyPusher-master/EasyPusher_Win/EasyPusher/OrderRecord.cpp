/*
	Copyright (c) 2013-2016 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.EasyDarwin.org
*/
#include "stdafx.h"
#include "OrderRecord.h"
#include "EasyPusher.h"
#include "IniFileLoad.h"

COrderRecordInfo::COrderRecordInfo()
{

}
COrderRecordInfo::~COrderRecordInfo()
{

}
COrderRecord::COrderRecord(void)
{
	m_bStopManual=FALSE;
	m_nCurRecordIndex=-1;
}

COrderRecord::~COrderRecord(void)
{
}

BOOL COrderRecord::LoadOrderRecordData(CString strOrderRcPath)
{
	CString strFilePath("");
	strFilePath = GET_MODULE_FILE_INFO.strPath + _T("ini\\config.ini");
	CIniFileLoad IniFileLoad;
	IniFileLoad.SetFileName(strFilePath);
	int nValue = 0;
	//�Ƿ�ʹ�ö�ʱ¼��
	IniFileLoad.LoadDataInt(_T("OrderRecord"), _T("bUserOrderRecord"),nValue);
	if(nValue<=0)
	{
		m_bUserOrderRecord = FALSE;
		if(nValue<0)
		{
			nValue=0;
			IniFileLoad.SetDataInt(_T("OrderRecord"), _T("bUserOrderRecord"),nValue);
		}
		return FALSE;

	}
	else
	{
		m_bUserOrderRecord = TRUE;
	}
	//ʹ�ö�ʱ���͵�����  0=��������ͷ������ 1=����RTSP�� 2=��Ļ���� 3=�ļ�����
	IniFileLoad.LoadDataInt(_T("OrderRecord"), _T("nOrderType"),m_nOrderRecordType);
	if(m_nOrderRecordType<0)
	{
		m_nOrderRecordType = 0;
		IniFileLoad.SetDataInt(_T("OrderRecord"), _T("nOrderType"),m_nOrderRecordType);
	}

	//�ֶ�¼���ʱ��,����Ϊ��λ,���Ϊ-1�������ֶ�¼��
	IniFileLoad.LoadDataInt(_T("OrderRecord"), _T("nManualTime"),m_nManualTime);

	//ʱ��ƫ��
	IniFileLoad.LoadDataInt(_T("OrderRecord"), _T("nSubTime"),m_nSubTime);
	if(m_nSubTime<0)
	{
		m_nSubTime = 0;
		IniFileLoad.SetDataInt(_T("OrderRecord"), _T("nSubTime"),m_nSubTime);
	}

	//�Ƿ�����ʱ��������
	IniFileLoad.LoadDataInt(_T("OrderRecord"), _T("bStartLoadList"),nValue);
	if(nValue<=0)
	{
		m_bStartLoadList = FALSE;
		if(nValue<0)
		{
			IniFileLoad.SetDataInt(_T("OrderRecord"), _T("bStartLoadList"),0);
		}
	}
	else
	{
		m_bStartLoadList = TRUE;
	}

	m_strOrderInfoPath = GET_MODULE_FILE_INFO.strPath + _T("ini\\")+strOrderRcPath;
	//IniFileLoad.LoadString(_T("OrderRecord"), _T("strOrderInfoPath"),m_strOrderInfoPath);

	IniFileLoad.LoadDataInt(_T("OrderRecord"),_T("nCheckTimer"),m_nCheckTimer);
	if(m_nCheckTimer<=0)
	{
		m_nCheckTimer=1000;
		IniFileLoad.SetDataInt(_T("OrderRecord"), _T("nCheckTimer"), m_nCheckTimer);
	}


	IniFileLoad.LoadDataInt(_T("OrderRecord"), _T("bStopManual"), nValue);
	if(nValue<=0)
	{
		m_bStopManual=FALSE;
		if(nValue<0)
		{
			IniFileLoad.SetDataInt(_T("OrderRecord"), _T("bStopManual"),0);
		}
	}
	else
	{
		m_bStopManual=TRUE;
	}
	return 1;
}

int COrderRecord::LoadOrderRecordList(CString strOrderInfoPath)
{
	
	m_listOrderRecord.RemoveAll();
	if(strOrderInfoPath.IsEmpty())
		return 0;
	return LoadOrderRecordList(strOrderInfoPath,m_listOrderRecord);
/*	CIniFileLoad IniFileLoad;

	IniFileLoad.SetFileName(strOrderInfoPath);
	
	CStringArray arrKey,arrValue;
	CString strSection = "OrderRecordList";

	int nArrayKeyCount = IniFileLoad.GetKeyValues(arrKey,arrValue,strSection);
	if(nArrayKeyCount<=0)
	{
		strSection=_T("");
		nArrayKeyCount = IniFileLoad.GetKeyValues(arrKey,arrValue,strSection);
	}
	CString strKey="";
	CString strValue="";
	for(int k=0;k<nArrayKeyCount;k++)
	{
		strKey = arrKey.GetAt(k);
		strValue = arrValue.GetAt(k);
		if(strValue.IsEmpty())
		{
			continue;
		}
		COrderRecordInfo tmpOrderRecordInfo;
		CString2OrderRecordInfo(strValue,tmpOrderRecordInfo);
		tmpOrderRecordInfo.name=strKey;
		m_listOrderRecord.AddTail(tmpOrderRecordInfo);
	}
	return nArrayKeyCount;*/
}

CString COrderRecord::ReadKeyValueFromS(CString strKey,CString strValue)
{
	CString strRet(_T(""));
	if(strValue.IsEmpty()||strKey.IsEmpty())
	{
		return strRet;
	}
	//�õ�nameֵ
	int nKey=strKey.GetLength();
	if(nKey<=0)
		return strRet;
	//���ҹؼ�ֵ
	int nFind = strValue.Find(strKey);
	if(nFind<0) return strRet;

	//�ҵ��ؼ�ֵ���ҹؼ�ֵ�������
	CString strTmp0=strValue.Mid(nFind+nKey);

	int nFind1 = strTmp0.Find(_T("\""));
	if(nFind1<0)
	{
		return strRet;
	}
	CString strTmp1=strTmp0.Mid(nFind1+1);
	int nFind2 = strTmp1.Find(_T("\""));
	if(nFind2<0)
	{
		strRet=strTmp1.Left(nFind1);
	}
	else
		strRet=strTmp1.Left(nFind2);
	return strRet;	
}

CString COrderRecord::GetArrayData(CStringArray &array,int nIndex)
{
	int nCount = array.GetSize();
	if(nIndex<0||nIndex>=nCount)
		return _T("");
	CString strValue = array.GetAt(nIndex);
	return strValue;
}

int COrderRecord::GetParamData(CString strValue,CStringArray &arrayValue,CString strFlag)
{
	int nPos=0;
	while(nPos>=0)
	{
		nPos = strValue.Find(strFlag);
		if(nPos>=0)
		{
			CString strTmp;
			strTmp = strValue.Left(nPos);
			strTmp.TrimLeft();
			strTmp.TrimRight();
			arrayValue.Add(strTmp);
			strValue.Delete(0, nPos+1);
			strValue.TrimLeft();
			strValue.TrimRight();
		}
		else
		{
			strValue.TrimLeft();
			strValue.TrimRight();
			CString strTmp;
			strTmp = strValue;
			arrayValue.Add(strTmp);
		}
	}
	return arrayValue.GetSize();
}

/*
��ʼʱ��+����ʱ��+��־( 0--�ƻ�����,1--ÿ��)+�Ƿ����(isenable)+����(name)+	����(intro)+��ʦ(teacherid)		//
CString strValue�ĸ�ʽΪ:2010-04-12 13:45:12,2010-03-17 14:45:12,1
*/
void COrderRecord::CString2OrderRecordInfo(CString strValue,COrderRecordInfo &tmpOrderRecordInfo)
{
	if(strValue.IsEmpty())
		return ;
	CStringArray arrayValue;

	int nCount = GetParamData(strValue,arrayValue,_T(","));
	CString strName=_T("");
	CString strStartTime = GetArrayData(arrayValue,0);
	CString strEndTime = GetArrayData(arrayValue,1);
	CString strProperty1 =  GetArrayData(arrayValue,2);//Eg: �γ�����,
	CString strProperty2 = GetArrayData(arrayValue,3); //Eg: ��ʦ����,
	CString strProperty3 =  GetArrayData(arrayValue,4);//Eg: �ڿεص�
	
	tmpOrderRecordInfo.nState=0;
	if(!strStartTime.IsEmpty())
	{
		tmpOrderRecordInfo.strStartTimeString = strStartTime;
		tmpOrderRecordInfo.starttime = CString2CTimer(strStartTime);
		if(tmpOrderRecordInfo.starttime.GetYear()<=1900)
		{
			tmpOrderRecordInfo.nState=1;
		}	
	}
	if(!strEndTime.IsEmpty())
	{
		tmpOrderRecordInfo.strEndTimeString = strEndTime;
		tmpOrderRecordInfo.endtime = CString2CTimer(strEndTime);
	}
	tmpOrderRecordInfo.name = strName;
	tmpOrderRecordInfo.property1 = strProperty1;
	tmpOrderRecordInfo.property2 = strProperty2;
	tmpOrderRecordInfo.property3 = strProperty3;

}

/*
������CString2CTimer
���ܣ���CString ��ʱ������ת��ΪCTimer��ʱ������
������strTimer����ʽΪ��_20090921102106�������ǣ�20090921102106
*/
COleDateTime COrderRecord::CString2CTimer(CString strTimer)
{
	COleVariant   VariantTime;   
	VariantTime   =   strTimer;   
	try 
	{
		VariantTime.ChangeType(VT_DATE);   
	}
	catch(COleException* e)
	{
		e->ReportError();

	}
	COleDateTime   DataTime   =   VariantTime;

	return DataTime;
}



void COrderRecord::SetOrderRecordFlag(int nRecordFlag)
{
	if(nRecordFlag==0)
	{
		m_nCurRecordIndex=-1;
	}
	else if(nRecordFlag==1||nRecordFlag==2)
	{
		
	}

}
/*
����:CheckIsRecordFlag
����:����Ƿ���¼�ƻ���ֹͣ¼��״̬����
��ע:����:0--û���κβ���,1---��ʼ,2---ֹͣ
	nCurrentRecordTime:��ǰ¼��ʱ��,curTimer:��ǰʱ��
	0--�ֶ�ֹͣ nRecordType = 1 �ֶ������ť��ʼ(��ʱ),2--ʱ���⿪ʼ(�б�)

*/
int COrderRecord::CheckIsListFlag(int nCurRecFlag, int& nActiveOrderId)
{
	COleDateTime curTime=curTime.GetCurrentTime();
	BOOL bStopManual=m_bStopManual;//�Ƿ�ֹͣ�ֶ�¼��
	if(nCurRecFlag==1)//������
	{
		//TRACE("¼����.���..%d\r\n",m_nCurRecordIndex);
		if(m_nCurRecordIndex<0&&bStopManual)
		{		
			int nCheckType=1;//0--��ʼʱ����У�1--����ʱ���
			CUIntArray  arrayTimeIn;
			arrayTimeIn.RemoveAll();
			int nInCount = CheckIsRecordFlag_List(curTime,nCheckType,arrayTimeIn);
			if(nInCount>0)
			{
				nActiveOrderId = arrayTimeIn.GetAt(0);
				if (nActiveOrderId>-1)
				{	
					//��Ϊ��ֹͣ״̬
					SetOrderRecListRunState(nActiveOrderId, 2);
					
				}
				return 2;
			}
		}
		else if(m_nCurRecordIndex>=0)
		{

			int nCheckType=1;
			int nRet = CheckIsRecordFlag_Index(curTime,nCheckType,m_nCurRecordIndex);
			if(nRet>=1)
			{
				nActiveOrderId = m_nCurRecordIndex;
				m_nCurRecordIndex=-1;
				//��Ϊ��ֹͣ״̬
				SetOrderRecListRunState(nActiveOrderId, 2);
				return 2;
			}
		}
		return 0;
	}
	else if(nCurRecFlag==0)//��ǰֹͣ����������
	{
		if(m_nCurRecordIndex<0)//��ǰ����¼�Ƽƻ���
		{
			//��⿪ʼ¼�Ƶ�ʱ��
			int nCheckType=0;//0--��ʼʱ����У�1--����ʱ���
			CUIntArray  arrayTimeIn;
			arrayTimeIn.RemoveAll();
			int nInCount = CheckIsRecordFlag_List(curTime,nCheckType,arrayTimeIn);
			//TRACE("ֹͣ��.���..%d\r\n",nInCount);	
			if(nInCount>0)
			{
				m_nCurRecordIndex=arrayTimeIn.GetAt(0);	
				nActiveOrderId = m_nCurRecordIndex;
				//��Ϊ������״̬
				SetOrderRecListRunState(nActiveOrderId, 1);
				return 1;
			}
			return 0;
		}
	}
	return 0;
}

/*
-1-----�����б���
����0--����ָ��ʱ�䷶Χ��
����1--��ָ����ʱ�䷶Χ��
*/
int COrderRecord::CheckIsRecordFlag_Index(COleDateTime  curTime,int nCheckType,int nCurIndex)
{
	if(nCurIndex<0)
		return -1;
	POSITION pos = m_listOrderRecord.FindIndex(nCurIndex);
	if(pos==NULL||pos<0)
	{
		return -1;
	}
	COrderRecordInfo tOrderRecordInfo;
	tOrderRecordInfo = m_listOrderRecord.GetAt(pos);
	COleDateTime useDateTime=tOrderRecordInfo.starttime;

	if(nCheckType==0)//��ʼʱ��
	{
		useDateTime=tOrderRecordInfo.starttime;
	}
	else if(nCheckType==1)//����ʱ��
	{
		useDateTime=tOrderRecordInfo.endtime;
	}

	BOOL bInTime=FALSE;
	if(tOrderRecordInfo.nState==0)//����
	{
		COleDateTimeSpan  startSpan;
		startSpan = curTime-useDateTime;

		double nSeconds = startSpan.GetTotalSeconds();
		int dSub = abs(m_nSubTime-abs((int)nSeconds));
		//int dSub = (abs)((int)nSeconds);
	
		if(dSub<= m_nSubTime)
		{
			bInTime=TRUE;
		}
	}
	else if(tOrderRecordInfo.nState==1)//ÿ��
	{

		int nHours1 = useDateTime.GetHour();
		int nMunites1 = useDateTime.GetMinute();
		int nSeconds1 = useDateTime.GetSecond();

		int nHours0 = curTime.GetHour();
		int nMunites0 = curTime.GetMinute();
		int nSeconds0 = curTime.GetSecond();

		int dSub = abs(nSeconds1-nSeconds0);
		if(nHours0==nHours1&&nMunites1==nMunites0&&(dSub<=m_nSubTime))
		{
			bInTime=TRUE;
		}
	}
	return bInTime;
}
/*
����:CheckIsRecordFlag_List(COleDateTime   curTime)
����:����б����Ƿ���¼�ƿ�ʼ����ͽ�������
	0--û��,1--��ʼ¼��,2--����¼��
*/
int COrderRecord::CheckIsRecordFlag_List(COleDateTime  curTime,int nCheckType,CUIntArray  &arrayTimeIn)
{
	int nCount = m_listOrderRecord.GetCount();
	if(nCount<=0)
	{
		return -1;
	}
	arrayTimeIn.RemoveAll();
	for(int i=0;i<nCount;i++)
	{
		int nRet = CheckIsRecordFlag_Index(curTime,nCheckType,i);
		if(nRet>=1)
		{
			arrayTimeIn.Add(i);
		}
	}
	return arrayTimeIn.GetSize();
}

int COrderRecord::GetCurOrderPlanInfo(CString& sName, CString& sStartTime, CString& sEndTime, CString &strFilePath1,CString &strFilePath2,CString &strFilePath3)
{
	if(m_nCurRecordIndex<=-1)
	{
		return -1;
	}
	POSITION pos = m_listOrderRecord.FindIndex(m_nCurRecordIndex);
	if(pos==NULL||pos<0)
	{
		return -1;
	}
	COrderRecordInfo tOrderRecordInfo;
	tOrderRecordInfo = m_listOrderRecord.GetAt(pos);

	sName =tOrderRecordInfo.name;
	sStartTime = tOrderRecordInfo.strStartTimeString;
	sEndTime = tOrderRecordInfo.strEndTimeString;
	strFilePath1=tOrderRecordInfo.property1;
	strFilePath2=tOrderRecordInfo.property2;
	strFilePath3=tOrderRecordInfo.property3;
	return 1;	
}

BOOL COrderRecord::CheckOrderIni(CString strPath)
{

	if(strPath.IsEmpty())
		return FALSE;
	CIniFileLoad IniFileLoad;

	IniFileLoad.SetFileName(strPath);
	
	CStringArray arrKey,arrValue;
	CString strSection = _T("OrderRecordList");

	int nArrayKeyCount = IniFileLoad.GetKeyValues(arrKey,arrValue,strSection);
	if(nArrayKeyCount<=0)
	{
		strSection=_T("");
		nArrayKeyCount = IniFileLoad.GetKeyValues(arrKey,arrValue,strSection);
	}
	if(nArrayKeyCount<=0)
	{
		return FALSE;
	}
	return TRUE;
}

int COrderRecord::LoadOrderRecordList(CString strOrderInfoPath,CList<COrderRecordInfo,COrderRecordInfo&> &listOrderRecord)
{
	listOrderRecord.RemoveAll();
	if(strOrderInfoPath.IsEmpty())
		return 0;
	CIniFileLoad IniFileLoad;

	IniFileLoad.SetFileName(strOrderInfoPath);
	
	CStringArray arrKey,arrValue;
	CString strSection = _T("OrderRecordList");

	int nArrayKeyCount = IniFileLoad.GetKeyValues(arrKey,arrValue,strSection);//�ҳ��Ⱥ����ߵ�ֵ
	if(nArrayKeyCount<=0)
	{
		strSection=_T("");
		nArrayKeyCount = IniFileLoad.GetKeyValues(arrKey,arrValue,strSection);
	}
	CString strKey=_T("");
	CString strValue=_T("");
	int nItemId = 0;
	for(int k=0;k<nArrayKeyCount;k++)
	{
		strKey = arrKey.GetAt(k);
		strValue = arrValue.GetAt(k);
		if(strValue.IsEmpty())
		{
			continue;
		}
		COrderRecordInfo tmpOrderRecordInfo;
		CString2OrderRecordInfo(strValue,tmpOrderRecordInfo);
		tmpOrderRecordInfo.id = nItemId;
		nItemId++;
		tmpOrderRecordInfo.name=strKey;
		if (tmpOrderRecordInfo.nState==0)//����
		{	
			COleDateTime  curTime = curTime.GetCurrentTime();
			COleDateTimeSpan  startSpan;
			startSpan = curTime - tmpOrderRecordInfo.starttime;
			
			double nSeconds = startSpan.GetTotalSeconds();

			if (nSeconds>5)//��ȥ��ʱ��
			{
				tmpOrderRecordInfo.nRunState = -1;//��ʼ��ʱ��Ĭ������״̬Ϊ ��ȥʽ������
			} 
			else
			{		
				tmpOrderRecordInfo.nRunState = 0;//��ʼ��ʱ��Ĭ������״̬Ϊ �ȴ�������
			}
		}
		else if (tmpOrderRecordInfo.nState==1)//ÿ��
		{
			tmpOrderRecordInfo.nRunState = 0;//��ʼ��ʱ��Ĭ������״̬Ϊ �ȴ�������
		}
		listOrderRecord.AddTail(tmpOrderRecordInfo);//�����б�
	}
	return nArrayKeyCount;
}

int COrderRecord::GetOrderRecInfoById(int nIndex, COrderRecordInfo& tmpOrderRecordInfo)
{
	if(nIndex<0)
		return -1;
	POSITION pos = m_listOrderRecord.FindIndex(nIndex);
	if(pos==NULL||pos<0)
	{
		return -1;
	}

	tmpOrderRecordInfo = m_listOrderRecord.GetAt(pos);

	return 1;
}

int COrderRecord::SetOrderRecListRunState(int nIndex, int nRunState)
{
	if(nIndex<0)
		return -1;
	POSITION pos = m_listOrderRecord.FindIndex(nIndex);
	if(pos==NULL||pos<0)
	{
		return -1;
	}
	COrderRecordInfo tmpOrderRecordInfo;
	tmpOrderRecordInfo = m_listOrderRecord.GetAt(pos);
	tmpOrderRecordInfo.nRunState = nRunState;
	m_listOrderRecord.SetAt(pos, tmpOrderRecordInfo);
	return 1;
}

int COrderRecord::UpdateListInfoId()
{
	int nCount = m_listOrderRecord.GetCount();
	if(nCount<=0)
	{
		return -1;
	}

	for(int i=0;i<nCount;i++)
	{
		POSITION pos = m_listOrderRecord.FindIndex(i);
		if(pos==NULL||pos<0)
		{
			return -1;
		}
		COrderRecordInfo tmpOrderRecordInfo;
		tmpOrderRecordInfo = m_listOrderRecord.GetAt(pos);

		tmpOrderRecordInfo.id = i;

		m_listOrderRecord.SetAt(pos, tmpOrderRecordInfo);
	}
	return 1;
}