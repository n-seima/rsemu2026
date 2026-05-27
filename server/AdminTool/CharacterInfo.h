#if !defined(AFX_CHARACTERINFO_H__71F3AC0F_CC38_47D3_A2C6_D87B5BA3A267__INCLUDED_)
#define AFX_CHARACTERINFO_H__71F3AC0F_CC38_47D3_A2C6_D87B5BA3A267__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CharacterInfo.h : header file
//
#include "resource.h"
#include "cPLAYER_DATA.h"
#include "cITEM.h"

class CINDEX_TABLE{

public:
	DWORD	dwIndex;
	char	strName[dNAME_LENGTH];
	char	strId[dID_LENGTH];
	CINDEX_TABLE()
	{	
		dwIndex = 0xffffffff;
		memset(strName,0,dNAME_LENGTH);
		memset(strId,0,dID_LENGTH);
	}
	void	reset()
	{
		dwIndex = 0xffffffff;
		memset(strName,0,dNAME_LENGTH);
		memset(strId,0,dID_LENGTH);
	}
};


/////////////////////////////////////////////////////////////////////////////
// CCharacterInfo dialog

#define	dMAX_AVATAR_DATA_COUNT	20

class	CAvatarData : public cPLAYER_DATA
{
public:
	CItemDefine*		getValidItem()
	{
		int	i;

		int	iInventorySize	=	getInventorySize();

		for (i=0;i<iInventorySize;i++)
			if (m_aItems[i].m_wBaseItem == 0xffff)
				return	(CItemDefine*)&m_aItems[i];
			
		for (i=0;i<m_bf6ExtraInventorySize;i++)
			if (m_aExtraInventory[i].m_wBaseItem == 0xffff)
				return	(CItemDefine*)&m_aExtraInventory[i];


		return	NULL;
	}
	
	CItemDefine*		getExistItem(CItemDefine* _lpItem);
	CItemDefine*		getExistItem(int _iBaseItem,int _iIndex,BOOL _bEquip = TRUE);
	int					getItemCount(int	_iBaseItem, BOOL _bEquip = TRUE);
	int					getItemSlotCount(int	_iBaseItem, BOOL _bEquip = TRUE);
};

class	CAvatarManager
{
public:
	cDuelRecord		m_duelRecord;
	int				m_iLoadDataCount;
	CAvatarData		m_aData[dMAX_AVATAR_DATA_COUNT];
	BOOL			m_abIsLoadData[dMAX_AVATAR_DATA_COUNT];
	CBankInfo		m_bank;
	BOOL			m_bIsChangeBankData;
	BOOL			m_bIsInvalidBankData;
	char			m_strId[64];
	char			m_strName[32];

	cITEM*			getFreeSlotInBank();
	cITEM*			getExistItemInBank(CItemDefine* _lpItem);
	cITEM*			getItem(WORD _wBaseItem,DWORD _dwSerial,char *_lpstrOwner=NULL);
	cITEM*			getItemInBank(WORD _wBaseItem,DWORD _dwSerial);
	int				getItemInBankCount(int _iBaseItem);
	void			checkIncorrectItemData();
	CAvatarData*	getByName(char *_lpstrName)
	{
		for (int i=0;i<m_iLoadDataCount;i++)
		{
			if	(STRICMP(m_aData[i].m_strName,_lpstrName)	==	0)
				return	&m_aData[i];
		}

		return	NULL;

	}
};

class CCharacterInfo : public CPropertyPage
{
// Construction
public:
	CCharacterInfo(CWnd* pParent = NULL);   // standard constructor

	int				m_iSelectWorld;
	int				m_iSkillCount;
	char			m_strDataFolder[512];
	BOOL			m_bIsNormalDB;

	CAvatarManager	m_am;
	int				m_iWorldCount;
	int				m_iSelectAvatar;
	BOOL			m_bIsVirgin;
	BOOL			m_bIsFindPlayer;
	int				m_iFindJob,m_iFindLevel,m_iFindPlayerCount;

	DWORD			m_dwLogExp,m_dwLogLevel,m_dwLastVillage,m_dwLastField;

	inline	void	setVirgin()	{m_bIsVirgin	=	TRUE;}
	inline	void	lostVirgin(){m_bIsVirgin	=	FALSE;}
	inline	BOOL	isVirgin()	{return m_bIsVirgin;}

	void			resetCharacterInfo();
	void			checkData();

	void			fillData();
	BOOL			reviseIncorrectPrefixItem(int _iServer,char *_lpstrName,cITEM *_lpItem,int _iLine);
	int				removeGold(DWORD _dwSerial);

// Dialog Data
	//{{AFX_DATA(CCharacterInfo)
	enum { IDD = IDD_CHARACTER_DATA };
	CEdit	m_ebInclination;
	CComboBox	m_cbOperatorShape;
	CEdit	m_editRedStonePoint;
	CEdit	m_ebExplorationCount;
	CEdit	m_ebGuildDungeonPlayTime;
	CEdit	m_ebExpandInventorySize;
	CEdit	m_editWapointYPos;
	CEdit	m_editWapointXPos;
	CEdit	m_editWaypoint;
	CComboBox	m_cbWaypoint;
	CEdit	m_editDeathPeneltyTime;
	CEdit	m_editScript;
	CEdit	m_editPeneltyTime;
	CEdit	m_editPosTy;
	CEdit	m_editPosTx;
	CEdit	m_editLevel;
	CEdit	m_editGold;
	CEdit	m_editYPos;
	CEdit	m_editXPos;
	CEdit	m_editLastField;
	CComboBox	m_cbJobList;
	CEdit	m_editSelectedName;
	CEdit	m_editSkillBonus;
	CEdit	m_editAvatarIndex;
	CEdit	m_editSelectedId;
	CComboBox	m_cbCharacterList;
	CComboBox	m_cbWorldList;
	CEdit	m_editName;
	CEdit	m_editId;
	CEdit	m_editLoadDbName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCharacterInfo)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void	OnRecoverLauncher(FILE * fp, char * _strName,char	* strScript);
	// Generated message map functions
	//{{AFX_MSG(CCharacterInfo)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnClickSkillList();
	afx_msg void OnConnectAndLoad();
	afx_msg void OnSelchangeCharacterList();
	afx_msg void OnSaveFile();
	afx_msg void OnLoadFile();
	afx_msg void OnSaveToDb();
	afx_msg void OnMoveToGate();
	afx_msg void OnMoveToJail();
	afx_msg void OnMoveToVillage();
	afx_msg void OnResetSkillPoint();
	afx_msg void OnResetStatePoint();
	afx_msg void OnCheckRich();
	afx_msg void OnAddItem();
	afx_msg void OnCheckSkillPoint();
	afx_msg void OnRemoveItem();
	afx_msg void OnSaveAllToDb();
	afx_msg void OnItemList();
	afx_msg void OnResetLevel();
	afx_msg void OnSendToVillage();
	afx_msg void OnSendToJail();
	afx_msg void OnSendToGate();
	afx_msg void OnScripterLauncher();
	afx_msg void OnDataConvert();
	afx_msg void OnBankDataConvert();
	afx_msg void OnCheckLoginDB();
	afx_msg void OnRecoverCheckId();
	afx_msg void OnCalculateExp();
	afx_msg void OnGivePrize();
	afx_msg void OnGiveBoc();
	afx_msg void OnGiveGuildTicket();
	afx_msg void OnRestoreStrangeFault();
	afx_msg void OnCloseupCharacterList();
	afx_msg void OnSelchangeWorldList2();
	afx_msg void OnCheckStatePoint();
	afx_msg void OnOwnTitle();
	afx_msg void OnReadyToCreateGuild();
	afx_msg void OnSelchangeWayPointIndex();
	afx_msg void OnRequitalPeriodItem();
	afx_msg void OnQuestList();
	afx_msg void OnGetGuildName();
	afx_msg void OnResetJoinGuildDate();
	afx_msg void OnItemInfo();
	afx_msg void OnMinipetList();
	afx_msg void OnPetList();
	afx_msg void OnApplyDuelRecord();
	afx_msg void OnGetDuelRecord();
	afx_msg void OnChangeSealOfDawnCount();
	afx_msg void OnChangeRebirthCount();
	afx_msg void OnResetTransGvgDate();
	afx_msg void OnReviseSp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

extern	CCharacterInfo			*g_lpCharacterInfo;

#endif // !defined(AFX_CHARACTERINFO_H__71F3AC0F_CC38_47D3_A2C6_D87B5BA3A267__INCLUDED_)
