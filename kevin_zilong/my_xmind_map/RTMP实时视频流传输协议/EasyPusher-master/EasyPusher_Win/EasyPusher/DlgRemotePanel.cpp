/*
	Copyright (c) 2013-2014 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.EasyDarwin.org
*/
// DlgRemotePanel.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EasyPusher.h"
#include "DlgRemotePanel.h"
#include "afxdialogex.h"


// CDlgRemotePanel �Ի���

IMPLEMENT_DYNAMIC(CDlgRemotePanel, CDialogEx)

CDlgRemotePanel::CDlgRemotePanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgRemotePanel::IDD, pParent)
{
	pDlgVideo		=	NULL;
}

CDlgRemotePanel::~CDlgRemotePanel()
{
}

void CDlgRemotePanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgRemotePanel, CDialogEx)
END_MESSAGE_MAP()


// CDlgRemotePanel ��Ϣ�������


BOOL CDlgRemotePanel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	pDlgVideo = new CDlgVideo();
	pDlgVideo->Create(IDD_DIALOG_VIDEO, this);
	pDlgVideo->ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


LRESULT CDlgRemotePanel::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (WM_PAINT == message || WM_SIZE == message || WM_MOVE == message)
	{
		UpdateComponents();
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}


BOOL CDlgRemotePanel::DestroyWindow()
{
	__DESTROY_WINDOW(pDlgVideo);

	return CDialogEx::DestroyWindow();
}
void CDlgRemotePanel::UpdateComponents()
{
	CRect	rcClient;
	GetClientRect(&rcClient);
	if (rcClient.IsRectEmpty())		return;

	CRect	rcVideo;
	rcVideo.SetRect(rcClient.left+VIDEO_WINDOW_BORDER_WIDTH, 0, rcClient.right-VIDEO_WINDOW_BORDER_WIDTH, rcClient.bottom-VIDEO_WINDOW_BORDER_WIDTH);
	__MOVE_WINDOW(pDlgVideo, rcVideo);
	if (pDlgVideo)
	{
		pDlgVideo->Invalidate(FALSE);
	}
}



CWnd* CDlgRemotePanel::GetDlgVideo()
{
	if (!pDlgVideo)
	{
		return NULL;
	}
	return pDlgVideo;
}
