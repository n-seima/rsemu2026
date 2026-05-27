// Agent.h : main header file for the AGENT application
//

#if !defined(AFX_AGENT_H__0E14628F_4C44_4D3E_AAC7_1F1F9A690135__INCLUDED_)
#define AFX_AGENT_H__0E14628F_4C44_4D3E_AAC7_1F1F9A690135__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAgentApp:
// See Agent.cpp for the implementation of this class
//

class CAgentApp : public CWinApp
{
public:
	CAgentApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAgentApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAgentApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AGENT_H__0E14628F_4C44_4D3E_AAC7_1F1F9A690135__INCLUDED_)
