#if !defined(AFX_DLGCHECKPROCESS_H__34BA37DB_F0B3_4FCD_B1BF_B46E5688D8BC__INCLUDED_)
#define AFX_DLGCHECKPROCESS_H__34BA37DB_F0B3_4FCD_B1BF_B46E5688D8BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCheckProcess.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgCheckProcess dialog

class DlgCheckProcess : public CPropertyPage
{
	DECLARE_DYNCREATE(DlgCheckProcess)

// Construction
public:
	DlgCheckProcess();
	~DlgCheckProcess();

// Dialog Data
	//{{AFX_DATA(DlgCheckProcess)
	enum { IDD = IDD_DLG_PROCESS };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(DlgCheckProcess)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(DlgCheckProcess)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCHECKPROCESS_H__34BA37DB_F0B3_4FCD_B1BF_B46E5688D8BC__INCLUDED_)
