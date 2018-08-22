/*
	Copyright (c) 2013-2014 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.EasyDarwin.org
*/
// DlgVideo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EasyPusher.h"
#include "DlgVideo.h"
#include "afxdialogex.h"
#include "DlgPanel.h"

// CDlgVideo �Ի���

IMPLEMENT_DYNAMIC(CDlgVideo, CDialogEx)

CDlgVideo::CDlgVideo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgVideo::IDD, pParent)
{
	m_pEasyLogo = NULL;
	m_pMainDlg = NULL;
}

CDlgVideo::~CDlgVideo()
{
	UIRenderEngine->RemoveImage(m_pEasyLogo);

}

void CDlgVideo::SetMainDlg(CDlgPanel* pMainDlg)
{
	m_pMainDlg = pMainDlg;
}

void CDlgVideo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgVideo, CDialogEx)
	ON_WM_PAINT()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// CDlgVideo ��Ϣ�������
BOOL CDlgVideo::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_pEasyLogo = UIRenderEngine->GetImage(TEXT("SkinUI\\ͼ��\\Easylogo.png"));

	return TRUE;
}

void CDlgVideo::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	CBrush brushBkgnd; 
	CRect rcClient;
	brushBkgnd.CreateSolidBrush(RGB(0, 0, 0));
	GetClientRect(&rcClient);
	dc.FillRect(&rcClient, &brushBkgnd);
	brushBkgnd.DeleteObject(); //�ͷŻ�ˢ 

	if ( m_pEasyLogo != NULL && !m_pEasyLogo->IsNull() )
	{
		int nStartX = (rcClient.Width()-184)/2;
		int nStartY =  (rcClient.Height()-184)/2;
		m_pEasyLogo->DrawImage(CDC::FromHandle(dc.m_hDC),nStartX,nStartY);
	}
}


void CDlgVideo::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_pMainDlg&&m_pMainDlg->m_pManager)
	{
		//����˵�
		CUIMenu MainMenu;
		MainMenu.CreatePopupMenu();

		//�����Ӳ˵�
		CUIMenu SubMenu;
		SubMenu.CreateMenu();
		SubMenu.AppendMenu(3000,TEXT("MP4BOX"),NULL,0,MF_POPUP);
		SubMenu.AppendMenu(3001,TEXT("MP4Creator"),NULL,0,MF_POPUP);
		SubMenu.SetMenuBarImage(TEXT("SkinUI\\Menu\\menu_left_bg.png"),&CRect(2,2,2,2));
		SubMenu.SetMenuBackImage(TEXT("SkinUI\\Menu\\menu_right_bg.png"),&CRect(2,2,2,2));
		SubMenu.SetMenuHovenImage(TEXT("SkinUI\\Menu\\menu_selected.png"),&CRect(2,2,2,2));
		SubMenu.SetSeparatorImage(TEXT("SkinUI\\Menu\\menu_separtor.png"),&CRect(2,2,2,2));
		SubMenu.SetCheckImage(TEXT("SkinUI\\Menu\\menu_check.png"));
		if (m_pMainDlg->m_pManager->IsUseGpac())
		{
			SubMenu.CheckMenuItem(3000,MF_POPUP|MF_CHECKED);
		} 
		else
		{
			SubMenu.CheckMenuItem(3001,MF_POPUP|MF_CHECKED);
		}

		CUIMenu SubMenuEnc;
		SubMenuEnc.CreateMenu();
		SubMenuEnc.AppendMenu(3002,TEXT("x264+AAC"),NULL,0,MF_POPUP);
		SubMenuEnc.AppendMenu(3003,TEXT("FFEncoder"),NULL,0,MF_POPUP);
		SubMenuEnc.SetMenuBarImage(TEXT("SkinUI\\Menu\\menu_left_bg.png"),&CRect(2,2,2,2));
		SubMenuEnc.SetMenuBackImage(TEXT("SkinUI\\Menu\\menu_right_bg.png"),&CRect(2,2,2,2));
		SubMenuEnc.SetMenuHovenImage(TEXT("SkinUI\\Menu\\menu_selected.png"),&CRect(2,2,2,2));
		SubMenuEnc.SetSeparatorImage(TEXT("SkinUI\\Menu\\menu_separtor.png"),&CRect(2,2,2,2));
		SubMenuEnc.SetCheckImage(TEXT("SkinUI\\Menu\\menu_check.png"));
		if (m_pMainDlg->m_pManager->IsUseFFEncoder())
		{
			SubMenuEnc.CheckMenuItem(3003,MF_POPUP|MF_CHECKED);
		} 
		else
		{
			SubMenuEnc.CheckMenuItem(3002,MF_POPUP|MF_CHECKED);
		}

		//�����˵�
		MainMenu.AppendMenu(4000,TEXT("��������"), NULL,NULL, MF_POPUP);
// 		//�����ַ�
// 		MainMenu.AppendMenu(4000,TEXT("��������1+1"));

		MainMenu.AppendSeparator();
		MainMenu.AppendMenu(4001,TEXT("¼��MP4"), NULL,NULL, MF_POPUP);
		if(m_pMainDlg->m_pManager->IsWriteMP4())
		{
			MainMenu.CheckMenuItem(4001,MF_POPUP|MF_CHECKED);
		}
		MainMenu.InsertMenu((UINT)SubMenu.m_hMenu,TEXT("MP4��װ��ʽѡ��"),4002,0,0,MF_POPUP|MF_STRING|MF_BYPOSITION);
		MainMenu.AppendSeparator();
		MainMenu.AppendMenu(4003,TEXT("����RTMP"), NULL,NULL, MF_POPUP);
		if(m_pMainDlg->m_pManager->IsPushRtmp())
		{
			MainMenu.CheckMenuItem(4003,MF_POPUP|MF_CHECKED);
		}
		MainMenu.AppendSeparator();
		MainMenu.InsertMenu((UINT)SubMenuEnc.m_hMenu,TEXT("������ѡ��"),4004,NULL,NULL,MF_POPUP|MF_STRING|MF_BYPOSITION);

		MainMenu.SetMenuBarImage(TEXT("SkinUI\\Menu\\menu_left_bg.png"),&CRect(2,2,2,2));
		MainMenu.SetMenuBackImage(TEXT("SkinUI\\Menu\\menu_right_bg.png"),&CRect(2,2,2,2));
		MainMenu.SetMenuHovenImage(TEXT("SkinUI\\Menu\\menu_selected.png"),&CRect(2,2,2,2));
		MainMenu.SetSeparatorImage(TEXT("SkinUI\\Menu\\menu_separtor.png"),&CRect(2,2,2,2));
		MainMenu.SetCheckImage(TEXT("SkinUI\\Menu\\menu_check.png"));
		MainMenu.SetArrowImage(TEXT("SkinUI\\Menu\\menu_arrow.png"));

		MainMenu.TrackPopupMenu(this);
	}

	CDialogEx::OnRButtonUp(nFlags, point);
}

BOOL CDlgVideo::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case 4000:
		//AfxMessageBox(TEXT("�������"));
		break;
	}
	if (m_pMainDlg)
	{
		m_pMainDlg->ProcessVideoWndMenuMsg(LOWORD(wParam));
	}
	
	return CDialogEx::OnCommand(wParam, lParam);
}
