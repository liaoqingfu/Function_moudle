/*
	Copyright (c) 2013-2015 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.easydarwin.org
	Author: Gavin@easydarwin.org
*/
#pragma once


// CEasySkinProgressCtrl
#include "EasySkinUI.h"

class  CEasySkinProgressCtrl : public CProgressCtrl,public IEasySkinControl
{
	DECLARE_DYNAMIC(CEasySkinProgressCtrl)

	//��Դ����
public:
	CImageEx * m_pBackImg, * m_pStepImg, * m_pFloatImg;

	//��������
protected:
	int						m_nFloatPos;			//������Դ����
	CSize					m_szFloatsize;			//������Դ��С

	//��������
public:
	//���캯��
	CEasySkinProgressCtrl();
	//��������
	virtual ~CEasySkinProgressCtrl();

	//��Դ����
public:
	//������Դ
	BOOL SetBackImage(LPCTSTR lpszFileName,bool bFixed = true);
	//������Դ
	BOOL SetStepImage(LPCTSTR lpszFileName);
	//������Դ
	BOOL SetFloatImage(LPCTSTR lpszFileName,int nTime=100);

	//��Ϣӳ��
public:
	//������Ϣ
	afx_msg void OnPaint();
	//���Ʊ���
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//������Ϣ
	afx_msg void OnDestroy();
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//�ǿͻ���
	afx_msg void OnNcPaint();

protected:
	DECLARE_MESSAGE_MAP()
};


