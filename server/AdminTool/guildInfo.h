#if !defined(AFX_GUILDINFO_H__825645B9_D795_4737_AF05_134FB8477978__INCLUDED_)
#define AFX_GUILDINFO_H__825645B9_D795_4737_AF05_134FB8477978__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// guildInfo.h : header file
//
#include	"CGuildDefine.h"
#include	"guild_skill_define.h"
#include	"windowExtendAPI.h"

/////////////////////////////////////////////////////////////////////////////
// cGuildInfo dialog

class	CGuildSkillManager
{
public:
	int					m_iSkillCount;
	CGuildSkillDefine	m_aSkills[dMAX_GUILD_SKILL_COUNT];
	
	BOOL				init();
	void				reset();
	BOOL				loadData();
	CGuildSkillDefine*	get(int _iSkill);
};

class cGuildInfo : public CPropertyPage
{
// Construction
public:
	cGuildInfo();   // standard constructor

	cListView			m_listMemberList,m_listSkillList,m_listGuildList;
	CGuildAdvanceInfo	m_guildInfo;
	int					m_iFirstGuild;
	CGuildMan			m_aMember[dGUILD_MAX_PLAYER];
	int					m_iGuildCount;
	int					m_iSelectedServer;
	char				m_strGuildList[dGUILD_MAX_COUNT][dGUILD_NAME_LENGTH];
	BOOL				m_bIsWantAllGuildInfo;

	void	receiveGuildInfo(int _iSerial,char *_lpstrName,CGuildAdvanceInfo *_lpInfo);
	void	receiveGuildMemberInfo(int _iSerial,int _iAllCount,int _iListCount,CGuildMan *_lpMember);
	void	receiveGuildNameList(BOOL _bIsBegin,BOOL _bIsLast,int _iCount,char _lpstrNameList[100][dGUILD_NAME_LENGTH],int _iServerIndex,int _iLastGuild);

	void	fillData();
// Dialog Data
	//{{AFX_DATA(cGuildInfo)
	enum { IDD = IDD_GUILD };
	CEdit	m_ebWinCount;
	CEdit	m_ebVictoryPoint;
	CEdit	m_ebThisWeekBattleCount;
	CEdit	m_ebOrganizeDateYear;
	CEdit	m_ebOrganizeDateMonth;
	CEdit	m_ebOrganizeDateDay;
	CEdit	m_ebNotice;
	CEdit	m_ebMasterName;
	CEdit	m_ebLastBattleGuild;
	CEdit	m_ebGuildSkillPoint;
	CEdit	m_ebExpTax;
	CEdit	m_ebDefeatCount;
	CEdit	m_ebDrawCount;
	CEdit	m_ebCVCount;
	CEdit	m_ebChangeMasterDateYear;
	CEdit	m_ebChangeMasterDateMonth;
	CEdit	m_ebChangeMasterDateDay;
	CEdit	m_ebSerial;
	CEdit	m_ebExperience;
	CEdit	m_ebLevel;
	CEdit	m_ebGuildName;
	CEdit	m_ebGuildVillage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cGuildInfo)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cGuildInfo)
	afx_msg void OnGetGuildInfo();
	virtual BOOL OnInitDialog();
	afx_msg void OnAskGuildList();
	afx_msg void OnSave();
	afx_msg void OnChangeGuildmaster();
	afx_msg void OnGuildInventory();
	afx_msg void OnPetAndGuardian();
	afx_msg void OnTwinkleMark();
	afx_msg void OnResetAllGuildTwinkleStatus();
	afx_msg void OnChangeHallLevel();
	afx_msg void OnGetAll();
	afx_msg void OnRewindList();
	afx_msg void OnChangePlaceReated();
	afx_msg void OnButtonSaveGuildEtc();
	afx_msg void OnSaveCountOfGpb();
	afx_msg void OnSyncLoadedPlayer();
	afx_msg void OnRclickMemberList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSaveTodayGuildBattleCount();
	afx_msg void OnSaveThisWeekGuildBattleCount();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern	CGuildSkillManager	g_gsManager;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUILDINFO_H__825645B9_D795_4737_AF05_134FB8477978__INCLUDED_)
