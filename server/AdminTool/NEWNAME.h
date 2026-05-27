#if !defined(AFX_NEWNAME_H__443AE3D3_AF3E_48AA_BD69_0FC7963FF02A__INCLUDED_)
#define AFX_NEWNAME_H__443AE3D3_AF3E_48AA_BD69_0FC7963FF02A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NEWNAME.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNEWNAME dialog

class CNEWNAME : public CDialog
{
// Construction
public:
	CNEWNAME(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNEWNAME)
	enum { IDD = IDD_NEW_NAME };
	CEdit	m_ctlNewName;
	CString	m_strNewName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNEWNAME)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNEWNAME)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWNAME_H__443AE3D3_AF3E_48AA_BD69_0FC7963FF02A__INCLUDED_)
