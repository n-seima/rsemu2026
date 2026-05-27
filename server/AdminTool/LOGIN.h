#if !defined(AFX_LOGIN_H__F1884512_5FFB_497C_9814_A3B7D54FB3A8__INCLUDED_)
#define AFX_LOGIN_H__F1884512_5FFB_497C_9814_A3B7D54FB3A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LOGIN.h : header file
//
#include "cADO_ADMIN.h"
/////////////////////////////////////////////////////////////////////////////
// CLOGIN dialog

class CLOGIN : public CDialog
{
// Construction
public:
	CLOGIN(CWnd* pParent = NULL);   // standard constructor

	char	m_strLogId[64];

// Dialog Data
	//{{AFX_DATA(CLOGIN)
	enum { IDD = IDD_CONNECT };
	CEdit	m_ctlPw;
	CEdit	m_ctlName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLOGIN)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	cADO_ADMIN		DB;
	// Generated message map functions
	//{{AFX_MSG(CLOGIN)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGIN_H__F1884512_5FFB_497C_9814_A3B7D54FB3A8__INCLUDED_)
