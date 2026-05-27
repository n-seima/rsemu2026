// AnalyzeLog.h : main header file for the ANALYZELOG application
//

#if !defined(AFX_ANALYZELOG_H__19821EE1_2F28_43BE_8C31_3A44651959B3__INCLUDED_)
#define AFX_ANALYZELOG_H__19821EE1_2F28_43BE_8C31_3A44651959B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAnalyzeLogApp:
// See AnalyzeLog.cpp for the implementation of this class
//

class CAnalyzeLogApp : public CWinApp
{
public:
	CAnalyzeLogApp();
	~CAnalyzeLogApp();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnalyzeLogApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAnalyzeLogApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANALYZELOG_H__19821EE1_2F28_43BE_8C31_3A44651959B3__INCLUDED_)
