#if !defined(AFX_DLGSCHEDULESETTING_H__0FC676AA_31A3_42FF_B38E_A642A7A26809__INCLUDED_)
#define AFX_DLGSCHEDULESETTING_H__0FC676AA_31A3_42FF_B38E_A642A7A26809__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgScheduleSetting.h : header file
//

#include "DlgTimeSetting.h"
#include "DlgFileManagement.h"
#include "DlgCheckProcess.h"
#include "DlgMonitoring.h"

/////////////////////////////////////////////////////////////////////////////
// DlgScheduleSetting dialog

class DlgScheduleSetting : public CDialog
{
private:
	int					m_iDlgType;	//	eScheduleType
	CPropertySheet		m_Sheet;
	DlgTimeSetting		m_pageTimeSetting;
	DlgFileManagement	m_pageFileManagement;
	DlgCheckProcess		m_pageCheckProcess;
	DlgMonitoring		m_pageMonitoring;

// Construction
public:
	DlgScheduleSetting(CWnd* pParent = NULL);   // standard constructor

	void setDlgType(int _dlgType) { m_iDlgType = _dlgType; }
// Dialog Data
	//{{AFX_DATA(DlgScheduleSetting)
	enum { IDD = IDD_DLG_SCHEDULE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgScheduleSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgScheduleSetting)
	virtual BOOL OnInitDialog();
	afx_msg void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSCHEDULESETTING_H__0FC676AA_31A3_42FF_B38E_A642A7A26809__INCLUDED_)
