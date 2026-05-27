#if !defined(AFX_MSGDLG_H__3F8D2A36_F381_43F9_8BBC_0231C8C5B5A1__INCLUDED_)
#define AFX_MSGDLG_H__3F8D2A36_F381_43F9_8BBC_0231C8C5B5A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MSGDLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMSGDLG dialog

class CMSGDLG : public CDialog
{
// Construction
public:
	CMSGDLG(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMSGDLG)
	enum { IDD = IDD_MSG_DIALOG };
	CProgressCtrl	m_ctlProgress;
	CStatic	m_ctlMsg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMSGDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMSGDLG)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGDLG_H__3F8D2A36_F381_43F9_8BBC_0231C8C5B5A1__INCLUDED_)
