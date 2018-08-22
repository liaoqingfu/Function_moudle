/*
	Copyright (c) 2013-2015 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.easydarwin.org
	Author: Gavin@easydarwin.org
*/
#ifndef EASYSKIN_MENU_HEAD_FILE
#define EASYSKIN_MENU_HEAD_FILE

#pragma once

#include "EasySkinUI.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
enum enMenuItemType
{
	MenuItemType_String,			//�ַ�����
	MenuItemType_Separator,			//�������
};

//////////////////////////////////////////////////////////////////////////////////

//�˵�����
class CEasySkinMenuItem
{
	//��������
public:
	const enMenuItemType			m_MenuItemType;						//��������
	HMENU							m_hSubMenu;							//�Ӳ˵�

	//��������
public:
	//���캯��
	CEasySkinMenuItem(enMenuItemType MenuItemType) : m_MenuItemType(MenuItemType) {m_hSubMenu = NULL;}
	//��������
	virtual ~CEasySkinMenuItem() {}
};

//�ַ��˵�
class CEasySkinMenuString : public CEasySkinMenuItem
{
	//��������
public:
	CString							m_strString;						//�˵��ַ�
	CImageEx						*m_pImageN;							//ͼ����
	CImageEx						*m_pImageH;							//ͼ����

	//��������
public:
	//���캯��
	CEasySkinMenuString() : CEasySkinMenuItem(MenuItemType_String) {m_pImageN=m_pImageH=NULL;}
	//��������
	virtual ~CEasySkinMenuString() {}
};

//��ֲ˵�
class CEasySkinMenuSeparator : public CEasySkinMenuItem
{
	//��������
public:
	//���캯��
	CEasySkinMenuSeparator() : CEasySkinMenuItem(MenuItemType_Separator) {}
	//��������
	virtual ~CEasySkinMenuSeparator() {}
};

//////////////////////////////////////////////////////////////////////////////////

//����˵��
typedef CArray<CEasySkinMenuItem *,CEasySkinMenuItem *>			CMenuItemArray;					//��������
typedef CArray<CEasySkinMenuString *,CEasySkinMenuString *>		CMenuStringArray;				//��������
typedef CArray<CEasySkinMenuSeparator *,CEasySkinMenuSeparator *>	CMenuSeparatorArray;			//��������

//////////////////////////////////////////////////////////////////////////////////

//�˵��ں�
class  CUIMenu : public CMenu,public IEasySkinControl
{
	//ͼƬ��Դ
protected:
	CImageEx						*m_pMenuBar;
	CImageEx						*m_pMenuBack;
	CImageEx						*m_pMenuHoven;
	CImageEx						*m_pSeparator;
	CImageEx						*m_pCheck;
	CImageEx						*m_pArrow;

	//�ں˱���
protected:
	CMenuItemArray					m_MenuItemActive;					//��������
	
	//��̬����
protected:
	static CMenuStringArray			m_MenuItemString;					//�ַ�����
	static CMenuSeparatorArray		m_MenuItemSeparator;				//�������
	static HHOOK					m_hMenuHook;

protected:
	COLORREF						m_crSeparator;

	//��������
public:
	//���캯��
	CUIMenu();
	//��������
	virtual ~CUIMenu();

	//���غ���
public:
	//�滭����
	virtual VOID DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//����λ��
	virtual VOID MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	//������
public:
	//�����˵�
	bool CreatePopupMenu();
	//���ٲ˵�
	BOOL DestroyMenu();
	//�����˵�
	bool TrackPopupMenu(CWnd * pWnd);
	//�����˵�
	bool TrackPopupMenu(INT nXPos, INT nYPos, CWnd * pWnd);

	//��Դ����
public:
	//������Դ
	BOOL SetMenuBarImage(LPCTSTR lpszFileName,CONST LPRECT lprcNinePart=NULL);
	//������Դ
	BOOL SetMenuBackImage(LPCTSTR lpszFileName,CONST LPRECT lprcNinePart=NULL);
	//������Դ
	BOOL SetMenuHovenImage(LPCTSTR lpszFileName,CONST LPRECT lprcNinePart=NULL);
	//������Դ
	BOOL SetSeparatorImage(LPCTSTR lpszFileName,CONST LPRECT lprcNinePart=NULL);
	//������Դ
	BOOL SetCheckImage(LPCTSTR lpszFileName);
	//������Դ
	BOOL SetArrowImage(LPCTSTR lpszFileName);

	//���Ӻ���
public:
	//������
	bool AppendSeparator();
	//�����ַ�
	bool AppendMenu(UINT nMenuID, LPCTSTR pszString, LPCTSTR lpszIconN=NULL, LPCTSTR lpszIconH=NULL, UINT nFlags=0);

	//���뺯��
public:
	//������
	bool InsertSeparator(UINT nPosition);
	//�����ַ�
	bool InsertMenu(UINT nMenuID, LPCTSTR pszString, UINT nPosition,  LPCTSTR lpszIconN=NULL, LPCTSTR lpszIconH=NULL, UINT nFlags=0);

	//�޸ĺ���
public:
	//ɾ���˵�
	bool RemoveMenu(UINT nPosition, UINT nFlags);
	//�޸Ĳ˵�
	bool ModifyMenu(UINT nMenuID, LPCTSTR pszString, UINT nPosition,  LPCTSTR lpszIconN=NULL, LPCTSTR lpszIconH=NULL, UINT nFlags=0);

	//�ڲ�����
private:
	//�������
	int GetSeparatorCount();
	//�ͷ�����
	VOID FreeMenuItem(CEasySkinMenuItem * pEasySkinMenuItem);
	//��ȡ����
	CEasySkinMenuItem * AcitveMenuItem(enMenuItemType MenuItemType);

	static LRESULT CALLBACK WindowsHook(int code,WPARAM wParam,LPARAM lParam);

};

//////////////////////////////////////////////////////////////////////////////////

#endif