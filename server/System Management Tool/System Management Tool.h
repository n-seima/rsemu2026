// System Management Tool.h : main header file for the SYSTEM MANAGEMENT TOOL application
//

#if !defined(AFX_SYSTEMMANAGEMENTTOOL_H__EB2AC86B_50F0_4ED8_8834_81BF61197750__INCLUDED_)
#define AFX_SYSTEMMANAGEMENTTOOL_H__EB2AC86B_50F0_4ED8_8834_81BF61197750__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSMTApp:
// See System Management Tool.cpp for the implementation of this class
//

class CSMTApp : public CWinApp
{
public:
	CSMTApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMTApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSMTApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMMANAGEMENTTOOL_H__EB2AC86B_50F0_4ED8_8834_81BF61197750__INCLUDED_)
