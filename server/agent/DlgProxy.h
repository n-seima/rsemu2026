// DlgProxy.h : header file
//

#if !defined(AFX_DLGPROXY_H__45C56BA0_CD01_4EE5_8386_B57F81DFDA2F__INCLUDED_)
#define AFX_DLGPROXY_H__45C56BA0_CD01_4EE5_8386_B57F81DFDA2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAgentDlg;

/////////////////////////////////////////////////////////////////////////////
// CAgentDlgAutoProxy command target

class CAgentDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CAgentDlgAutoProxy)

	CAgentDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CAgentDlg* m_pDialog;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAgentDlgAutoProxy)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAgentDlgAutoProxy();

	// Generated message map functions
	//{{AFX_MSG(CAgentDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CAgentDlgAutoProxy)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CAgentDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROXY_H__45C56BA0_CD01_4EE5_8386_B57F81DFDA2F__INCLUDED_)
