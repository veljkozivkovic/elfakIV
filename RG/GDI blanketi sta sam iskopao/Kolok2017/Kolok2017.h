
// Kolok2017.h : main header file for the Kolok2017 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKolok2017App:
// See Kolok2017.cpp for the implementation of this class
//

class CKolok2017App : public CWinApp
{
public:
	CKolok2017App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKolok2017App theApp;
