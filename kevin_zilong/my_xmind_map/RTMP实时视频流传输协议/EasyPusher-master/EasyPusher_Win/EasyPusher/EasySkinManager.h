
/*
	Copyright (c) 2013-2014 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.EasyDarwin.org
*/
#pragma once

#define IDC_EDIT_PASS                   5687
#define IDC_TABCTRL                     5688
#define IDC_MAINMENU                    5689
#define IDC_WNDMIN                      5690
#define IDC_WNDMAX                      5691
#define IDC_WNDSKIN                     5692
#define IDC_BT_WRITE                    5693
#define IDC_BUDDYLIST                   5694
#define IDC_BT_SEND                     5695
#define IDC_BT_FACE                     5696
#define IDC_COMBO_FONT                  6000
#define IDC_COMBO_SIZE                  6001
#define IDC_BOLD                        6002
#define IDC_ITALIC                      6003
#define IDC_UNDERLINE                   6004
#define IDC_COLOR                       6005
#define IDC_RICH_SEND                   6006
#define IDC_BT_FONT                     7000

// CEasySkinManager �Ի���ͳһ��������
// ͳһ�̳��࣬�������

//////////////////////////////////////////////////////////////////////////
enum AFX_WND_STYLE
{
	en_Wnd_Normal=0,			//�رգ���󻯣���С��ͬʱ����
	en_Wnd_MinimizeBox,			//����󻯰�ť
	en_Wnd_CloseBox,			//ֻ�йرհ�ť
	en_Wnd_None					//�ް�ť
};

class CEasySkinManager : public CEasySkinDialog
{
	DECLARE_DYNAMIC(CEasySkinManager)
public:
	CImageEx						*m_pImageBack;
	CImageEx						* m_pImageLogo;
	CImageEx						* m_pImageTitle;

	CEasySkinButton						m_btClose;
	CEasySkinButton						m_btMin;
	CEasySkinButton						m_btMax;

	AFX_WND_STYLE					m_enWndStyle;
	bool							m_bIsZoomed;
	bool							m_bShowLogo;
	bool							m_bShowTittle;

public:
	CEasySkinManager(UINT nIDTemplate,CWnd* pParent = NULL,AFX_WND_STYLE Style = en_Wnd_Normal);   // ��׼���캯��
	virtual ~CEasySkinManager();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	//���ڻ���
	virtual void DrawClientArea(CDC*pDC,int nWidth,int nHeight);

	void UpdateSkin();
	bool IsWindowMax();
	void SetSkinWindowStytle(AFX_WND_STYLE enWndStyle);

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickWindowMin();
	afx_msg void OnBnClickWindowMax();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
};
