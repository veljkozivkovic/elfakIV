
// Kolok2023.h : main header file for the Kolok2023 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKolok2023App:
// See Kolok2023.cpp for the implementation of this class
//

class CKolok2023App : public CWinApp
{
public:
	CKolok2023App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKolok2023App theApp;
