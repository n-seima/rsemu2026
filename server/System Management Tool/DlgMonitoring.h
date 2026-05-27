#if !defined(AFX_DLGMONITORING_H__FFA3FFB0_C188_4335_BEC9_4B38C98B00E4__INCLUDED_)
#define AFX_DLGMONITORING_H__FFA3FFB0_C188_4335_BEC9_4B38C98B00E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMonitoring.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgMonitoring dialog

class DlgMonitoring : public CPropertyPage
{
	DECLARE_DYNCREATE(DlgMonitoring)

// Construction
public:
	DlgMonitoring();
	~DlgMonitoring();

// Dialog Data
	//{{AFX_DATA(DlgMonitoring)
	enum { IDD = IDD_DLG_MONITORING };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(DlgMonitoring)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(DlgMonitoring)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMONITORING_H__FFA3FFB0_C188_4335_BEC9_4B38C98B00E4__INCLUDED_)
