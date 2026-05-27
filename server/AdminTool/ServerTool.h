#if !defined(AFX_SERVERTOOL_H__9AE5D878_23D6_40C4_AA4A_6700A8C4E2F6__INCLUDED_)
#define AFX_SERVERTOOL_H__9AE5D878_23D6_40C4_AA4A_6700A8C4E2F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerTool.h : header file
//
#include	"cADO_ADMIN.h"
#include	"windowExtendAPI.h"
#include	"RenewItemExpireDate.h"
#include	"cPACKET_WORLDSERVER.H"
#include	"CharacterInfo.H"
#include	"cItem.H"

/////////////////////////////////////////////////////////////////////////////
// CServerTool dialog

class CServerTool : public CPropertyPage
{
// Construction
public:
	cADO_ADMIN		m_db;
	cADO_ADMIN		m_aDb[20];
	BOOL			m_abDBConnetionInfo[20];
	cListView		m_listExtendItemDeathTime;

	int				m_iRenewItemExpireTimeInfoCount;
	cRenewItemExpireTimeInfo	m_aRenewItemExpireTimeInfoList[1024];
	CAvatarManager	m_commonAm;

	CServerTool(CWnd* pParent = NULL);   // standard constructor
	void			receiveList(cRenewItemExpireTimeInfo *_lpList,int _iCount,int _iFullCount,BOOL _bIsLast);
	BOOL			ConnectAllServerDB();

// Dialog Data
	//{{AFX_DATA(CServerTool)
	enum { IDD = IDD_SERVER_TOOL };
	CEdit	m_editLoadDbName;
	CEdit	m_editGoldLimit;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerTool)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
//	rookieEvent090721.cpp
	class	c090720RookieEventInfo
	{
	public:
		char	m_strRookieID[dID_LENGTH];
		char	m_strCallerID[dID_LENGTH];

		char	m_strReceiveRookieName[dNAME_LENGTH];
		char	m_strReceiveCallerName[dNAME_LENGTH];

		int		m_iReceiveRookieServer;
		int		m_iReceiveCallerServer;

		int		m_iReceivedTimeRookieLevelForRookie;
		int		m_iReceivedTimeRookieLevelForCaller;

		c090720RookieEventInfo()
		{
			memset(this,0,sizeof(c090720RookieEventInfo));

			m_iReceiveRookieServer	=	-1;
			m_iReceiveCallerServer	=	-1;
		}
	};

	class	c090720RookieEventProcessInfo
	{
	public:
		int		m_iTargetCount;
		int		m_iInvalidCharacterCount;
		int		m_aiCheckLevel[10];
		int		m_aiCheckLevelASC[10];

		cITEM	m_aRookieItem[5],m_aCallerItem[5];
	};

	c090720RookieEventProcessInfo	m_rookieEventProcessInfo;

	int			GetCurrentLevelForEvent090720(int _iLevel);
	char*		GetMaxLevelActor(int *_lpiServerIndex,int *_lpiMaxLevel,char *_lpstrId,int *_lpiResult);
	BOOL		read090720RookieEventExistText(char *_lpstrFileName,c090720RookieEventInfo *_lpList,int _iMaxCount,cFILE *_lpLogFile);
	BOOL		build090720RookieEvent(char *_lpstrFileName,c090720RookieEventInfo *_lpList,cFILE *_lpLogFile);
	BOOL		run090720RookieEvent(char *_lpstrFileName,c090720RookieEventInfo *_lpList,cFILE *_lpLogFile);
	BOOL		give090720RookieEventItem(cADO_ADMIN *_lpDB,c090720RookieEventInfo *_lpInfo,int _iRookieLevel,CAvatarData *_lpData,BOOL _bIsRookie,cFILE *_lpLogFile);
	BOOL		give090720RookieEventItemRollBack(cADO_ADMIN *_lpDB,c090720RookieEventInfo *_lpInfo,int _iRookieLevel,CAvatarData *_lpData,BOOL _bIsRookie,cFILE *_lpLogFile);
	void		build090720RookieEventRewardItem();
	void		write090720Info(c090720RookieEventInfo *_lpInfo,cFILE *_lpLogFile);


protected:
	class	cQuestInfo050708
	{
		public:
			char	m_strName[100];
			int		m_iExp;
			int		m_iBoostExp;
	};

	class	cQuestOperatorInfo050708ForQuest
	{
	public:
		int					m_iQuestCount;
		int					m_iTotalExp;
		cQuestInfo050708	m_aQuestList[100];
		int					m_iLevel;
		char				m_strId[dID_LENGTH];		
		char				m_strName[dNAME_LENGTH];	

		void				reset()
		{
			m_iQuestCount	=	0;
			m_iTotalExp		=	0;
		}

		BOOL				addQuest(cQuestInfo050708 *_lpQuest)
		{
			for	(int	i=0;i<m_iQuestCount;i++)
			{
				if	(stricmp(m_aQuestList[i].m_strName,_lpQuest->m_strName)==0)
					return	FALSE;
			}

			m_iTotalExp	+=	_lpQuest->m_iExp;

			memcpy(&m_aQuestList[m_iQuestCount],_lpQuest,sizeof(cQuestInfo050708));
			m_iQuestCount++;
			return	TRUE;
		}
	};

	class	cQuestOperatorInfo050708
	{
	public:
		char				m_strId[dID_LENGTH];		
		char				m_strName[dNAME_LENGTH];	
		DWORD				m_dwDecreaseExp;			//	깍아야 할 경험치
		DWORD				m_dw1StExp;					//	1차로 깎은 경험치
		DWORD				m_dw2StExp;					//	2차로 깍을 경험치

	};

	cQuestOperatorInfo050708*			getQuestOperator(char *_lpstrName)
	{
		for	(int i=0;i<m_iQuestOperatorCount;i++)
		{
			if	(stricmp(m_aList[i].m_strName,_lpstrName)	==	0)
				return	&m_aList[i];
		}

		return	NULL;
	}

	cQuestOperatorInfo050708ForQuest*	getQuestInfoByUser(char *_lpstrName)
	{
		for(int	i=0;i<m_iQuestCount;i++)
		{
			if	(stricmp(m_aQuestList[i].m_strName,_lpstrName)	==	0)
				return	&m_aQuestList[i];
		}

		cQuestOperatorInfo050708ForQuest*	lpReturn	=	&m_aQuestList[m_iQuestCount];

		m_iQuestCount++;

		return	lpReturn;
	}
	BOOL		operateCorrectExpWork(int _iServer,char *_lpstrName,int _iYesterDayLevel,int _iCorrectExp,cFILE *_lpFile,char *_lpstrWord=NULL,BOOL _bIsWantSave=TRUE);
	BOOL		uploadCompleteExpWorkActorList(int _iServer,int _iDay=7);

	int									m_iQuestOperatorCount;
	int									m_iQuestReversionItemCount;
	int									m_iQuestLevelDownPetCount;
	int									m_iQuestLevelDownCount;
	int									m_iQuestAddDownCount;

	int									m_iQuestCount;

	cQuestOperatorInfo050708			m_aList[1024];
	cQuestOperatorInfo050708ForQuest	m_aQuestList[1024];
	// Generated message map functions
	//{{AFX_MSG(CServerTool)
	afx_msg void OnCheckRich();
	virtual BOOL OnInitDialog();
	afx_msg void OnSerchItem();
	afx_msg void OnExportUniqueItem();
	afx_msg void OnUpdateList();
	afx_msg void OnAddRequital();
	afx_msg void OnButton3();
	afx_msg void OnCheckQuest050707();
	afx_msg void OnCheckQuest50708();
	afx_msg void OnCheckQuest50709();
	afx_msg void OnCheckQuest50710();
	afx_msg void OnCheckQuest50711();
	afx_msg void OnCheckQuest50712();
	afx_msg void On050720();
	afx_msg void On50721();
	afx_msg void On50722();
	afx_msg void On50723();
	afx_msg void On50724();
	afx_msg void On050818();
	afx_msg void On50819();
	afx_msg void OnBuildGuildBattlePrologResult();
	afx_msg void OnRookieEvent090720();
	afx_msg void OnRookieEventRun090720();
	afx_msg void On100721();
	afx_msg void OnRewardOldRebirther();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

extern	int		GetServerIndexForEvent(char *_lpstrServerName);
extern	char*	GetServerNameForEvent(int _iIndex);

enum
{
	eResult_OK,
	eCanNotConnectServer,
	eCanNotFindID,
	eCanNotFindCharacter,
};

extern	HANDLE	g_hConsole;
extern	void	Printf(char *str,...);

#endif // !defined(AFX_SERVERTOOL_H__9AE5D878_23D6_40C4_AA4A_6700A8C4E2F6__INCLUDED_)
