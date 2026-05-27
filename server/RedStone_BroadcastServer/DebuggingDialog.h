#if !defined(AFX_DEBUGGINGDIALOG_H__490C5B60_77A9_4160_B53F_B2B8735A592A__INCLUDED_)
#define AFX_DEBUGGINGDIALOG_H__490C5B60_77A9_4160_B53F_B2B8735A592A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DebuggingDialog.h : header file
//

#include "DlgFindUserInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CDebuggingDialog dialog

class CDebuggingDialog : public CDialog
{
public:
	CPropertySheet		m_Sheet;

	CDlgFindUserInfo	m_pageUserInfo;

// Construction
public:
	CDebuggingDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDebuggingDialog)
	enum { IDD = IDD_DEBUGGING_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDebuggingDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDebuggingDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEBUGGINGDIALOG_H__490C5B60_77A9_4160_B53F_B2B8735A592A__INCLUDED_)
