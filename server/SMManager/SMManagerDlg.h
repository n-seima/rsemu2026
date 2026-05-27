// SMManagerDlg.h : header file
//

#if !defined(AFX_SMMANAGERDLG_H__E74D11BB_EE4A_40D9_A8BD_F942776F37B7__INCLUDED_)
#define AFX_SMMANAGERDLG_H__E74D11BB_EE4A_40D9_A8BD_F942776F37B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSMManagerDlg dialog

class CSMManagerDlg : public CDialog
{
// Construction
public:
	CSMManagerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSMManagerDlg)
	enum { IDD = IDD_SMMANAGER_DIALOG };
	CListBox	m_lbOutput;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMManagerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSMManagerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnOK();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMMANAGERDLG_H__E74D11BB_EE4A_40D9_A8BD_F942776F37B7__INCLUDED_)
