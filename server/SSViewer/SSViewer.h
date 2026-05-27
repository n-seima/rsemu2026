// SSViewer.h : main header file for the SSVIEWER application
//

#if !defined(AFX_SSVIEWER_H__DD7670A9_0A92_49E0_A638_1F2E73E2E471__INCLUDED_)
#define AFX_SSVIEWER_H__DD7670A9_0A92_49E0_A638_1F2E73E2E471__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSSViewerApp:
// See SSViewer.cpp for the implementation of this class
//

class CSSViewerApp : public CWinApp
{
public:
	CSSViewerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSSViewerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSSViewerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSVIEWER_H__DD7670A9_0A92_49E0_A638_1F2E73E2E471__INCLUDED_)
