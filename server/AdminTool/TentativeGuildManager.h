#if !defined(AFX_TENTATIVEGUILDMANAGER_H__EE52FD03_592B_4B02_B2E1_36582EBD2B55__INCLUDED_)
#define AFX_TENTATIVEGUILDMANAGER_H__EE52FD03_592B_4B02_B2E1_36582EBD2B55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TentativeGuildManager.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTentativeGuildManager dialog
#include "cPACKET_WORLDSERVER.h"
#include "cADO_ADMIN.h"

class CTentativeGuildManager : public CPropertyPage
{
	DECLARE_DYNCREATE(CTentativeGuildManager)
// Construction
public:
	CTentativeGuildManager();
	~CTentativeGuildManager();

	CGuildBattleSchedule		todaySchedule,tomorrowSchedule;
	GUILDINFO_FORADMIN			guildInfo[dGUILD_MAX_COUNT];
	cADO_ADMIN					DB;

	void	recvGuildData(char	* data); 
	void	SetGuildDataSetting(int _iCount);
	void	changeGuildBattle(char* _strGuild1, char* _strGuild2, WORD	_wField,int _iField);
// Dialog Data
	//{{AFX_DATA(CTentativeGuildManager)
	enum { IDD = IDD_TENTATIVE_GUILD };
	CEdit	m_ebGuild2Name;
	CEdit	m_ebGuild1Name;
	CEdit	m_ctlField9;
	CEdit	m_ctlField8;
	CEdit	m_ctlField7;
	CEdit	m_ctlField6;
	CEdit	m_ctlField5;
	CEdit	m_ctlField4;
	CEdit	m_ctlField3;
	CEdit	m_ctlField2;
	CEdit	m_ctlField10;
	CEdit	m_ctlField1;
	CComboBox	m_ctlSGuild9;
	CComboBox	m_ctlSGuild8;
	CComboBox	m_ctlSGuild7;
	CComboBox	m_ctlSGuild6;
	CComboBox	m_ctlSGuild5;
	CComboBox	m_ctlSGuild4;
	CComboBox	m_ctlSGuild3;
	CComboBox	m_ctlSGuild2;
	CComboBox	m_ctlSGuild10;
	CComboBox	m_ctlSGuild1;
	CComboBox	m_ctlFGuild9;
	CComboBox	m_ctlFGuild8;
	CComboBox	m_ctlFGuild7;
	CComboBox	m_ctlFGuild6;
	CComboBox	m_ctlFGuild5;
	CComboBox	m_ctlFGuild4;
	CComboBox	m_ctlFGuild3;
	CComboBox	m_ctlFGuild2;
	CComboBox	m_ctlFGuild10;
	CComboBox	m_ctlFGuild1;
	CComboBox	m_ctlDateList;
	CComboBox	m_ctlTimeList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTentativeGuildManager)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTentativeGuildManager)
	virtual BOOL OnInitDialog();
	afx_msg void OnGetGuildlist();
	afx_msg void OnSelchangeDateList();
	afx_msg void OnSelchangeTimeList();
	afx_msg void OnModify1();
	afx_msg void OnModify2();
	afx_msg void OnModify3();
	afx_msg void OnModify4();
	afx_msg void OnModify5();
	afx_msg void OnModify6();
	afx_msg void OnModify7();
	afx_msg void OnModify8();
	afx_msg void OnModify9();
	afx_msg void OnModify10();
//	afx_msg void OnGetguildinfo();
	afx_msg void OnInitguild1();
	afx_msg void OnBeginTestBattle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TENTATIVEGUILDMANAGER_H__EE52FD03_592B_4B02_B2E1_36582EBD2B55__INCLUDED_)
