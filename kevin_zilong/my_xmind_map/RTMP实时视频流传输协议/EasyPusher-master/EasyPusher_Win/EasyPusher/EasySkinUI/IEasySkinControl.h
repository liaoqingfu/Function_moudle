/*
	Copyright (c) 2013-2015 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.easydarwin.org
	Author: Gavin@easydarwin.org
*/
#ifndef CONTROL_HEAD_FILE
#define CONTROL_HEAD_FILE

#pragma once

#include "EasySkinUI.h"

class  IEasySkinControl
{
protected:
	HDC							m_hParentDC;
	BOOL						m_bTransparent;
	CImageEx					*m_pImageScroll;						//������Դ
	DWORD						m_dwTextAlign;
	HFONT						m_Font;

	//������ɫ
public:
	COLORREF					m_colDefText;						//Ĭ������
	COLORREF					m_colNormalText;					//��������
	COLORREF					m_colSelectText;					//��������
	COLORREF					m_colDisableText;					//ʧЧ����
	COLORREF					m_colReadOnlyText;					//ֻ������

	COLORREF					m_colBack;							//���ڱ���
	COLORREF					m_colFrameNormal;					//�߿���ɫ

public:
	IEasySkinControl(void);
	virtual ~IEasySkinControl(void);

public:
	//���ξ���
	void CalcCenterRect(RECT& rcDest, int cx, int cy, RECT& rcCenter);
	//���Ʊ���
	void DrawParentWndBg(HWND hWnd,HDC hDC);
	//���ñ���
	void SetParentBack(HDC hDC){ m_bTransparent = true; m_hParentDC = hDC;}
	
	BOOL TrackMouseLeave(HWND hWnd);

	//��������
public:
	//��������
	void SetCtrlFont(HFONT hFont);
	//��ȡ����
	HFONT GetCtrlFont();
	//����λ��
	void SetTextAlign(WORD wTextAlign);

	//������ɫ
public:
	//Ĭ���ı�
	void SetDefText(COLORREF colText);
	//�����ı�
	void SetNormalText(COLORREF colText);
	//�����ı�
	void SetSelectText(COLORREF colText);
	//ʧЧ�ı�
	void SetDisableText(COLORREF colText);

	//�߿���ɫ
public:
	//����״̬
	void SetFrameColor(COLORREF colFrame);
	//����״̬
	void SetBackColor(COLORREF colBack);

	//���ú���
public:
	//
	void SetUnTransparent();

	//������
public:
	//������Դ
	BOOL SetScrollImage(CWnd * pOwn,LPCTSTR pszFileName);
	//
	void RemoveScorll();
};

#endif