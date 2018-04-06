
// TestBenchMfc.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTestBenchMfcApp:
// See TestBenchMfc.cpp for the implementation of this class
//

class CTestBenchMfcApp : public CWinApp
{
public:
	CTestBenchMfcApp();

// Overrides
public:
	virtual BOOL InitInstance();
    virtual INT  ExitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
    afx_msg void OnFileExit();
};

extern CTestBenchMfcApp theApp;