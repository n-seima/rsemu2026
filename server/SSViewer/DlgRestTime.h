#if !defined(AFX_DLGRESTTIME_H__909C2F4A_8F21_4EDD_B18F_4606E5A354F6__INCLUDED_)
#define AFX_DLGRESTTIME_H__909C2F4A_8F21_4EDD_B18F_4606E5A354F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRestTime.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRestTime dialog

class CDlgRestTime : public CDialog
{
// Construction
public:
	CDlgRestTime(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRestTime)
	enum { IDD = IDD_DLG_TIMEINPUT };
	CEdit	m_ctlRestTime;
	int		m_iRestTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRestTime)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRestTime)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRESTTIME_H__909C2F4A_8F21_4EDD_B18F_4606E5A354F6__INCLUDED_)
