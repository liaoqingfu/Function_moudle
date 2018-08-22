/*
	Copyright (c) 2013-2014 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.EasyDarwin.org
*/
#pragma once
#include "EasySkinManager.h"


class CGlobalUnits
{
public:
	TCHAR							m_szDefaultSkin[MAX_PATH<<1];		//ͼ��·��
	bool							m_bLeftDrawExtrude;				//�������
	vector<CEasySkinManager*>			m_WindowArray;

public:
	CGlobalUnits(void);
	~CGlobalUnits(void);

public:
	static CGlobalUnits*GetInstance();
};

#define GlobalUnits		CGlobalUnits::GetInstance()