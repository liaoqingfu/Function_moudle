/*
	Copyright (c) 2013-2015 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.easydarwin.org
	Author: Gavin@easydarwin.org
*/
#pragma once

#include "EasySkinUI.h"

class  CEasySkinListBox : public CListBox,public IEasySkinControl
{
	DECLARE_DYNAMIC(CEasySkinListBox)

	//��������
protected:
	int								m_nHovenItem;
	vector<tagItemStruct>			m_ItemStruct;
	int								nItemHeight;

	//��Դ����
protected:
	CImageEx 						* m_pBackImgN;			//������Դ
	CImageEx 						* m_pBackImgH;			//������Դ
	CImageEx 						* m_pSelectImg;			//ѡ����Դ

	//��������
public:
	//���캯��
	CEasySkinListBox(void);
	//��������
	~CEasySkinListBox(void);

	//���ܺ���
public:
	//�Ƿ�ѡ
	bool IsSingleSel();
	//�����ı�
	void AddString(LPCTSTR lpszItem);

	void InsertItem();

	//���ø߶�
	int SetItemHeight( int nIndex, UINT cyItemHeight );
	//ɾ���ı�
	int DeleteString( UINT nIndex );

	//��Դ����
public:
	//��������
	BOOL SetBackNormalImage(LPCTSTR lpszFileName, CONST LPRECT lpNinePart = NULL);
	//ѡ����Դ
	BOOL SetSelectImage(LPCTSTR lpszFileName, CONST LPRECT lpNinePart = NULL);
	//�ڵ����
	BOOL SetHovenImage(LPCTSTR lpszFileName, CONST LPRECT lpNinePart = NULL);

	//��Ϣ����
private:
	//��������
	afx_msg void OnDestroy();
	//����Ҽ�
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//����ƶ�
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//�ǿͻ���
	afx_msg void OnNcPaint();

	//���غ���
protected:
	//���ƺ���
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);

protected:
	DECLARE_MESSAGE_MAP()
};


