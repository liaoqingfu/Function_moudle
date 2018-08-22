/*
	Copyright (c) 2013-2014 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.EasyDarwin.org
*/
// DlgLocalPanel.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EasyPusher.h"
#include "DlgLocalPanel.h"
#include "afxdialogex.h"


// CDlgLocalPanel �Ի���
CDlgLocalPanel	*gDlgLocalPanel = NULL;
IMPLEMENT_DYNAMIC(CDlgLocalPanel, CDialogEx)

CDlgLocalPanel::CDlgLocalPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgLocalPanel::IDD, pParent)
{
	pDlgVideo			=	NULL;
}

CDlgLocalPanel::~CDlgLocalPanel()
{
}

void CDlgLocalPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLocalPanel, CDialogEx)

END_MESSAGE_MAP()


// CDlgLocalPanel ��Ϣ�������


BOOL CDlgLocalPanel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	pDlgVideo = new CDlgVideo();
	pDlgVideo->Create(IDD_DIALOG_VIDEO, this);
	pDlgVideo->ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


LRESULT CDlgLocalPanel::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (WM_PAINT == message || WM_SIZE == message || WM_MOVE == message)
	{
		UpdateComponents();
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}


BOOL CDlgLocalPanel::DestroyWindow()
{
	__DESTROY_WINDOW(pDlgVideo);

	return CDialogEx::DestroyWindow();
}


CWnd* CDlgLocalPanel::GetDlgVideoHwnd()
{
	if (!pDlgVideo)
	{
		return NULL;
	}
	return pDlgVideo;
}

void CDlgLocalPanel::UpdateComponents()
{
	CRect	rcClient;
	GetClientRect(&rcClient);
	if (rcClient.IsRectEmpty())		return;

	CRect	rcPushAddr, rcEdtAddr;

	CRect	rcVideo;
	rcVideo.SetRect(rcClient.left+VIDEO_WINDOW_BORDER_WIDTH, 0, 
		rcClient.right-VIDEO_WINDOW_BORDER_WIDTH, rcClient.bottom-VIDEO_WINDOW_BORDER_WIDTH);

	__MOVE_WINDOW(pDlgVideo, rcVideo);
	if (pDlgVideo)
	{
		pDlgVideo->Invalidate(FALSE);
	}
}
