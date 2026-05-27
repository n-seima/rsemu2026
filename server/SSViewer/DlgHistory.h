#if !defined(AFX_DLGHISTORY_H__E8923F02_8BE4_4523_88DF_919D9BEA113F__INCLUDED_)
#define AFX_DLGHISTORY_H__E8923F02_8BE4_4523_88DF_919D9BEA113F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgHistory.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cDlgHistory dialog

class cDlgHistory : public CDialog
{
private:
	int m_iYear;
	int m_iMonth;

private:
	void addItem(int _iIdx, char *_pszServerName, int _iErrorCode, char *_pszErrorTime);
	void detailViewNotice(int _iIdx);
	void getData();

// Construction
public:
	cDlgHistory(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(cDlgHistory)
	enum { IDD = IDD_DIALOG_HISTORY };
	CComboBox	m_comboMonth;
	CComboBox	m_comboYear;
	CListCtrl	m_listData;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cDlgHistory)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cDlgHistory)
	afx_msg void OnClose();
	afx_msg void OnButtonClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboMonth();
	afx_msg void OnSelchangeComboYear();
	afx_msg void OnDblclkListData(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHISTORY_H__E8923F02_8BE4_4523_88DF_919D9BEA113F__INCLUDED_)
