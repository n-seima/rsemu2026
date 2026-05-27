#if !defined(AFX_POLLDLG_H__BDF8A204_3E65_489B_881A_B906D727A92A__INCLUDED_)
#define AFX_POLLDLG_H__BDF8A204_3E65_489B_881A_B906D727A92A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PollDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cPollDlg dialog

class cPollDlg : public CPropertyPage
{
// Construction
public:
	cPollDlg();   // standard constructor

// Dialog Data
	//{{AFX_DATA(cPollDlg)
	enum { IDD = IDD_POLL };
	CMonthCalCtrl	m_beginDay;
	CMonthCalCtrl	m_endDay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cPollDlg)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cPollDlg)
	afx_msg void OnAddPoll();
	afx_msg void OnChangeSelectPollInfo();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(cPollDlg)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POLLDLG_H__BDF8A204_3E65_489B_881A_B906D727A92A__INCLUDED_)
