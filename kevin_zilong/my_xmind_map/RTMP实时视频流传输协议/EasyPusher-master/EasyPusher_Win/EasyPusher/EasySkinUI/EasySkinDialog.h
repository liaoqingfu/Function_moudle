/*
	Copyright (c) 2013-2015 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.easydarwin.org
	Author: Gavin@easydarwin.org
*/
#pragma once


#include "EasySkinUI.h"

class  CEasySkinDialog : public CDialog,public IEasySkinControl
{
	DECLARE_DYNAMIC(CEasySkinDialog)

protected:
	IEasySkinControl				m_EasySkinControl;
	HDC							m_hMemDC;
	HBITMAP						m_hMemBmp, m_hOldBmp;
	bool						m_bExtrude;				//�Ƿ��������
	bool						m_bClip;

public:
	CEasySkinDialog(UINT nIDTemplate,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEasySkinDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	//�ӿں���
protected:
	//�Ƴ��߿�
	void RemoveBorder();
	//��ʼ��
	virtual BOOL OnInitDialog();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//���ڻ���
	virtual void DrawClientArea(CDC*pDC,int nWidth,int nHeight){}
	//
	void SetExtrude(bool bExtrude);

	//���ܺ���
public:
	//��ȡ����
	HDC GetBackDC(){ return m_hMemDC; }
	//�����Ӵ���
	void SetClipChild(bool bClip);

	//��Ϣ����
public:
	//�滭��Ϣ
	afx_msg void OnPaint();
	//�����Ϣ
	afx_msg LRESULT OnNcHitTest(CPoint point);
	//��갴��
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	//���ڳߴ�
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//��̬����
protected:
	//ö�ٺ���
	static BOOL CALLBACK EnumChildProc(HWND hWndChild, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};
