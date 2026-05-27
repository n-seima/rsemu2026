// StressTestClient.h : main header file for the STRESSTESTCLIENT application
//

#if !defined(AFX_STRESSTESTCLIENT_H__00463E07_16FD_4802_95C6_CE71F77BC398__INCLUDED_)
#define AFX_STRESSTESTCLIENT_H__00463E07_16FD_4802_95C6_CE71F77BC398__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CStressTestClientApp:
// See StressTestClient.cpp for the implementation of this class
//

class CStressTestClientApp : public CWinApp
{
public:
	CStressTestClientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStressTestClientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CStressTestClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRESSTESTCLIENT_H__00463E07_16FD_4802_95C6_CE71F77BC398__INCLUDED_)
