// SMTDlg.h : header file
//

#if !defined(AFX_SMTDLG_H__0CC653AD_ED7F_421A_990D_A18A47EB805F__INCLUDED_)
#define AFX_SMTDLG_H__0CC653AD_ED7F_421A_990D_A18A47EB805F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSMTDlg dialog

class CSMTDlg : public CDialog
{
private:
	BOOL	setDlg(int _iDlgType);	//	eScheduleType

// Construction
public:
	CSMTDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSMTDlg)
	enum { IDD = IDD_SYSTEMMANAGEMENTTOOL_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMTDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSMTDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnMakeBackup();
	afx_msg void OnFileManagement();
	afx_msg void OnCheckProcess();
	afx_msg void OnCheckSystem();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMTDLG_H__0CC653AD_ED7F_421A_990D_A18A47EB805F__INCLUDED_)
