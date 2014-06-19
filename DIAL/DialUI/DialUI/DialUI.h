
// DialUI.h : main header file for the DialUI application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CDialUIApp:
// See DialUI.cpp for the implementation of this class
//

class CDialUIApp : public CWinApp
{
public:
	CDialUIApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDialUIApp theApp;
