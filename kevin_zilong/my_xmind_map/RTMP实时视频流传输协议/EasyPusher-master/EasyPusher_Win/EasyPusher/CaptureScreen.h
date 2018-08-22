/*
	Copyright (c) 2013-2015 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.EasyDarwin.org
*/
#pragma once

#include "FlashingWnd.h"
#include <d3d9.h>

//֪ͨ�����濪ʼ������Ϣ
#define WM_USER_RECORDSTART WM_USER+0x101
#define DINV    3

#ifndef  MAX_VIDEO_WINDOW_NUM
#define  MAX_VIDEO_WINDOW_NUM  16
#endif //MAX_VIDEO_WINDOW_NUM

#define SWAP(x,y)   ((x)^=(y)^=(x)^=(y))

typedef struct tagScreenCapDataInfo
{
	int nWidth;
	int nHeight;
	int nDataType;
}ScreenCapDataInfo;

typedef int (WINAPI *CaptureScreenCallback)(int nDevId, unsigned char *pBuffer, int nBufSize, int nRealDatatYPE, void* realDataInfo, void* pMaster);

class CEasyPusherDlg ;

class CCaptureScreen
{
public:
	CCaptureScreen(void);
	~CCaptureScreen(void);

public:
	//Interface Function
	int StartScreenCapture(HWND hShowWnd, int nCapMode);
	void StopScreenCapture();
	void GetCaptureScreenSize(int& nWidth, int& nHeight );
	//���ò������ݻص�����
	void SetCaptureScreenCallback(CaptureScreenCallback callBack, void * pMaster);

	BOOL IsInCapturing()
	{
		return m_bCapturing;
	}

	int Init();
	void UnInit();

public:
		//���ྲ̬ʵ����ָ��(������Ҫ���ڶര��ʱʹ�ã��Ҳ�֪�������Ƿ������ʱҲû�и��õİ취����)
		static CCaptureScreen* s_pCaptureScreen[MAX_VIDEO_WINDOW_NUM]; 
		static CCaptureScreen* Instance(int& nIndex);
		static void UnInstance(int nI);
//////////////////////////////////////////////////////////////////////////
	//��Ļ����֡������
	//Use these 2 functions to create frames and free frames
	void* CaptureScreenFrame(int left,int top,int width, int height,int tempDisableRect);
	void FreeFrame(void*) ;
	void InsertHighLight(HDC hdc,int xoffset, int yoffset);

	//���ѡ������ Region Select  Functions
	static LRESULT WINAPI MouseCaptureWndProc(HWND hWnd, UINT wMessage, WPARAM wParam, LPARAM lParam);
	int InitDrawShiftWindow();
	int InitSelectRegionWindow();
	int CreateShiftWindow();
	int DestroyShiftWindow();
	LRESULT ProcessMouseCapMsg(HWND hWnd, UINT wMessage, WPARAM wParam, LPARAM lParam);

	//inner call function
	BOOL isRectEqual(RECT a, RECT b);

	void SaveBitmapCopy(HDC hdc,HDC hdcbits, int x, int y, int sx, int sy);
	void RestoreBitmapCopy(HDC hdc,HDC hdcbits, int x, int y, int sx, int sy) ;
	void DeleteBitmapCopy();
	void DrawSelect(HDC hdc, BOOL fDraw, LPRECT lprClip);
	void NormalizeRect(LPRECT prc);
	void FixRectSizePos(LPRECT prc,int maxxScreen, int maxyScreen);
	void DrawFlashingRect(BOOL bDraw , int mode);
	//Mouse Capture functions 
	HCURSOR FetchCursorHandle();
	HANDLE Bitmap2Dib(HBITMAP, UINT);

	//�����߳̽�����Ļ����
	int CreateCaptureScreenThread();
	static UINT WINAPI CaptureScreenThread(LPVOID pParam);
	void CaptureVideoProcess();


public:
		HWND m_hMainWnd;
		//���ѡ�����ھ��
		HWND m_hMouseCaptureWnd;
		//���ѡ��ģʽ : 0==�̶���Сѡ�� 1=�Զ���ѡ�����Լ����� 2=ȫ��
		int m_nMouseCaptureMode;
		HBITMAP m_hSavedBitmap;
		CFlashingWnd* m_pFrame ;
		//��Ļ�豸�����ɫλ�� 24/32
		int m_nColorBits;

		//�Ƿ����ڲ���
		BOOL m_bCapturing;
		// ����Ƿ�������ק
		BOOL m_bMouseDraging;
		//�Ƿ������˸�ľ��ο�(Flashing Region)
		BOOL m_bFlashingRect;
		//�Ƿ�ѡ����������ƶ�
		BOOL  m_bAutopan;
		int m_nAutopanSpeed;
		//����������(��һ��ȦȦ��ɫ����ʾ�����λ��)
		BOOL m_bHighlightCursor;
		int m_highlightsize;
		int m_highlightshape;
		COLORREF m_highlightcolor ;

		//�Ƿ�¼�����
		BOOL m_bRecordCursor;

		//about Region
		RECT   m_rcOffset;
		RECT   m_rcClip; 
		RECT   m_rcUse; 
		RECT   m_old_rcClip;
		POINT  m_ptOrigin;
		RECT m_rcFixed;//�����úõ�Region 
		int m_nCaptureWidth;
		int m_nCaptureHeight;
		int m_nMaxxScreen;
		int m_nMaxyScreen;

		int m_nCursorType; 
		HCURSOR m_hSavedCursor;
		HANDLE m_hScreenCaptureThread;

		CaptureScreenCallback m_pCallback;
		void* m_pMaster;
		int m_nId;

		//ͼ��RGB�ڴ滺��
		PRGBTRIPLE m_hdib;
};

