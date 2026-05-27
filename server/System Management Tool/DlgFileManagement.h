#if !defined(AFX_DLGFILEMANAGEMENT_H__ED389E70_96A5_4A6A_BC74_24749264E169__INCLUDED_)
#define AFX_DLGFILEMANAGEMENT_H__ED389E70_96A5_4A6A_BC74_24749264E169__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFileManagement.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgFileManagement dialog

class DlgFileManagement : public CPropertyPage
{
	DECLARE_DYNCREATE(DlgFileManagement)

// Construction
public:
	DlgFileManagement();
	~DlgFileManagement();

// Dialog Data
	//{{AFX_DATA(DlgFileManagement)
	enum { IDD = IDD_DLG_FILE_MNG };
	BOOL	m_isDeleteSource;
	BOOL	m_isUsingScriptFile;
	BOOL	m_isControlFolder;
	CString	m_strScriptFile;
	CString	m_strSourcePath;
	CString	m_strTargetPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(DlgFileManagement)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(DlgFileManagement)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFILEMANAGEMENT_H__ED389E70_96A5_4A6A_BC74_24749264E169__INCLUDED_)
