#if !defined(AFX_CDLGCHANGEGMASTER_H__0139E094_4D2D_4856_861D_60A38DBC5050__INCLUDED_)
#define AFX_CDLGCHANGEGMASTER_H__0139E094_4D2D_4856_861D_60A38DBC5050__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cDlgChangeGMaster.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cDlgChangeGMaster dialog

class cDlgChangeGMaster : public CDialog
{
// Construction
public:
	cDlgChangeGMaster(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(cDlgChangeGMaster)
	enum { IDD = IDD_DLG_CHG_GILDMASTER };
	CComboBox	m_ctlServerList;
	CStatic	m_ctlExplain;
	CEdit	m_ctlNewGuildMaster;
	CEdit	m_ctlGuildName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cDlgChangeGMaster)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cDlgChangeGMaster)
	afx_msg void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnNext();
	afx_msg void OnSelchangecbServerList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	char	GuildName[32];
	char	OldGuildMaster[dNAME_LENGTH];
	int		OldGMLevel;
	char	NewGuildMaster[dNAME_LENGTH];
	int		NewGMLevel;
	int		selectedSrvIndex;

public:
	void	FailProcess();
	void	GetGuildInfo();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGCHANGEGMASTER_H__0139E094_4D2D_4856_861D_60A38DBC5050__INCLUDED_)
