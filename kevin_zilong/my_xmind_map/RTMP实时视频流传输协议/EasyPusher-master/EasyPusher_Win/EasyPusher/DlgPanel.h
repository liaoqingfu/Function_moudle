/*
	Copyright (c) 2013-2014 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.EasyDarwin.org
*/
#pragma once

#include "DlgVideo.h"
#include "SourceManager.h"
#include "afxwin.h"
#include "SettingDlg.h"
#include "OrderRecord.h"

#define MSG_ORDER_RUN WM_USER+1001

// CDlgPanel �Ի���
class CEasyPusherDlg;

class CDlgPanel : public CEasySkinDialog
{
	DECLARE_DYNAMIC(CDlgPanel)

public:
	CDlgPanel(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPanel();

// �Ի�������
	enum { IDD = IDD_DIALOG_PANEL };

protected:
	CDlgVideo		*pDlgVideo;
	CEdit	*m_pEdtServerIP;		
	CEdit	*m_pEdtServerPort;		
	CEdit	*m_pEdtServerStream;		
	CComboBox* m_pCmbType;//ֱ��/������ѡ
	CComboBox* m_pCmbSourceType;//Դ����ѡ��
	CComboBox* m_pCmbCamera;
	CComboBox* m_pCmbMic;
	CComboBox* m_pCmbScreenMode;
	CEdit*	m_pEdtRtspStream;		
	CEdit*	m_pEditStartTime;
	CEdit*	m_pEditEndTime;

	CEasySkinButton m_btnStart;
	CEasyPusherDlg* m_pMainDlg;
	//EASY_CHANNEL_INFO_T	easyChannelInfo;
	//��������Ƶ��������
	AVCapParamInfo m_sAVCapParamInfo;

protected:
	//UI �������
	virtual void DrawClientArea(CDC*pDC,int nWidth,int nHeight);
	void		UpdateComponents();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCbnSelchangeComboPannelType();
	afx_msg void OnCbnSelchangeComboPannelSource();
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL DestroyWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnCbnSelchangeComboCapscreenMode();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	CWnd* GetDlgVideo();
	void SetMainDlg(CEasyPusherDlg* pMainDlg, int nId);
	void UpdataResource();
	//�����Ƹ�ʽ��
	void FormatStreamName(char* sStreamName);
	void ProcessVideoWndMenuMsg(int nId);
	//��MP4�ļ�
	CString OpenMp4File();
	//���ؽ���������Ϣ
	void LoadSourceConfigInfo( SourceConfigInfo*  pSourceInfo );
	
	// �ƻ���غ��� [6/15/2016 SwordTwelve]
	// ����:LoadOrderRecordInfo
	// ����:���붨ʱ������Ϣ
	void LoadOrderRecordInfo(CString strOrderRcPath);
	//���ƻ��б�
	void CheckOrderList();

private:
	int m_nWndId;//�Լ��Ĵ���Id
	CFont	m_ftSaticDefault;
	SourceConfigInfo  m_sSourceInfo;
	COrderRecord* m_pOrderRecord;
public:
	CSourceManager* m_pManager;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnOrderRun(WPARAM wparam, LPARAM lparam) ; 

};

