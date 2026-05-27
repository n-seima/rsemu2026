#if !defined(AFX_PAGE_NOTICE_H__F4EB280C_C719_494F_B2D8_CC2796243C8E__INCLUDED_)
#define AFX_PAGE_NOTICE_H__F4EB280C_C719_494F_B2D8_CC2796243C8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PAGE_NOTICE.h : header file
//
#include "CFTPclient.h"
#include "cADO_ADMIN.h"

/////////////////////////////////////////////////////////////////////////////
// CPAGE_NOTICE dialog

#define dMAXADMINCOUNT		40

typedef struct 
{
	char	strName[20];
	int		iLev;
	int		iLastConnectDate;
}ADMINLIST;

class CPAGE_NOTICE : public CPropertyPage
{
	DECLARE_DYNCREATE(CPAGE_NOTICE)

// Construction
public:
	CPAGE_NOTICE();
	~CPAGE_NOTICE();
	nsFTP::CFTPClient		ftp;
	ADMINLIST		m_ctAdminList[dMAXADMINCOUNT];

	void			SetList();
// Dialog Data
	//{{AFX_DATA(CPAGE_NOTICE)
	enum { IDD = IDD_NOTICE_PAGE };
	CButton	m_ctlEraseMt;
	CButton	m_ctlISSendDirectly;
	CEdit	m_ctlWho;
	CEdit	m_ctlMsg;
	CButton	m_ctlIsSendtoAllServer;
	CDateTimeCtrl	m_ctlFinishTime;
	CDateTimeCtrl	m_ctlFinishDate;
	CComboBox	m_ctlParam1;
	CDateTimeCtrl	m_ctlReleaseTime;
	CDateTimeCtrl	m_ctlReleaseDate;
	CComboBox	m_ctlReserveCommand;
	CComboBox	m_ctlCommandList;
	CButton	m_ctlIsUseEndTime;
	CComboBox	m_ctlDbInfoType;
	CButton	m_ctlMakeDBCFile;
	CEdit	m_ctlDBPassInput;
	CStatic	m_ctlDBPassText;
	CEdit	m_ctlDBNameInput;
	CStatic	m_ctlDBNameText;
	CEdit	m_ctlDBAccountInput;
	CStatic	m_ctlDBAccountText;
	CEdit	m_ctlDBIp;
	CEdit	m_ctlFixcNotice;
	CComboBox	m_ctlFixc_Min;
	CComboBox	m_ctlFixc_Hour;
	CComboBox	m_ctlFixc_WeekDay;
	CEdit	m_ctlNewPass;
	CEdit	m_ctlOldPass;
	CComboBox	m_ctlLEV_LIST;
	CComboBox	m_ctlID_LIST;
	CEdit	m_ctlNotice;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPAGE_NOTICE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPAGE_NOTICE)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdminLoad();
	afx_msg void OnAdminPasschange();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnAdminChangelevel();
	afx_msg void OnChangeFixcheck();
	afx_msg void OnCheckFixchecktime();
	afx_msg void OnKillusers();
	afx_msg void OnChangeDelete();
	afx_msg void OnCreateaccount();
	afx_msg void OnMakedbc();
	afx_msg void OnIsuseEndtime();
	afx_msg void OnGetcommandlistBtn();
	afx_msg void OnSelchangeWorkForreserve();
	afx_msg void OnReserveBtn();
	afx_msg void OnSelchangeRegcommandList();
	afx_msg void OnModifyBtn();
	afx_msg void OnCancelBtn();
	afx_msg void OnSenddirect();
	afx_msg void OnErasemt();
	afx_msg void OnAdminDelete();
	afx_msg void OnCheckSmsAlarm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE_NOTICE_H__F4EB280C_C719_494F_B2D8_CC2796243C8E__INCLUDED_)
