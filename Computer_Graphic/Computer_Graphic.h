
// Computer_Graphic.h : Computer_Graphic Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CComputer_GraphicApp:
// �йش����ʵ�֣������ Computer_Graphic.cpp
//

class CComputer_GraphicApp : public CWinApp
{
public:
	CComputer_GraphicApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CComputer_GraphicApp theApp;
