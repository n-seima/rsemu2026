// SMManager.h : main header file for the SMMANAGER application
//

#if !defined(AFX_SMMANAGER_H__EED314BB_C64B_42F0_9444_31A6E2E9394A__INCLUDED_)
#define AFX_SMMANAGER_H__EED314BB_C64B_42F0_9444_31A6E2E9394A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSMManagerApp:
// See SMManager.cpp for the implementation of this class
//

class CSMManagerApp : public CWinApp
{
public:
	CSMManagerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMManagerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSMManagerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMMANAGER_H__EED314BB_C64B_42F0_9444_31A6E2E9394A__INCLUDED_)
