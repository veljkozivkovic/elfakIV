
// Kolok2018.h : main header file for the Kolok2018 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKolok2018App:
// See Kolok2018.cpp for the implementation of this class
//

class CKolok2018App : public CWinApp
{
public:
	CKolok2018App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKolok2018App theApp;
