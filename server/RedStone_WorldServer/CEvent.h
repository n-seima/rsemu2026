#ifndef _ClassEVENT_H
#define _ClassEVENT_H

#include "SFC.H"
#include "cPACKET_WORLDSERVER.H"
#include "eventDefine.h"

#define	dMAX_REQUITAL_TIME_COUNT	1024	

class	cEventWord	:	public cEventWordDefine
{
public:
	void			reset();
	void			save();
	void			load();
};
		

class	cOXQuiz		:	public cOXQuizDefine
{
public:
	WORD			m_wGateVillage;

	void			reset();
	void			save();
	void			load();
};

class	cWordQuiz	:	public cWordQuizDefine
{
public:
	WORD			m_wGateVillage;

	void			reset();
	void			save();
	void			load();
};

class	cDuelTeamManager
{
public:
	WORD			m_wTotalTeamCount;
	WORD			m_wTeamCount;
	cDuelTeamInfo	*m_pTeams;

	cDuelTeamManager();
	~cDuelTeamManager();

	void			init(int _iSize);
	bool			addTeam(cDuelTeamInfo *_lpTeamInfo);
	cDuelTeamInfo	*getTeam(int _iSlot);
	cDuelTeamInfo	*getTeam(char *_lpstrName);
};

class	cDuelRankerManager
{
public:
	WORD				m_awRankerCount[3];
	cDuelRankingInfo	m_aRankInfo[3][100];

	cDuelRankerManager()
	{
		memset(m_awRankerCount,0,sizeof(m_awRankerCount));
	}

	void			setWeeklyRankerCount(int _iCount)
	{
		m_awRankerCount[eWeeklyRanking]	=	_iCount;
	}
	void			setDailyRankerCount(int _iCount)
	{
		m_awRankerCount[eDailyRanking]	=	_iCount;
	}
	void			setTrialGameRankerCount(int _iCount)
	{
		m_awRankerCount[eTrialGameRanking]	=	_iCount;
	}

	void			updateWeeklyRanker(cDuelRankingInfo *_lpInfo)
	{
		memcpy(&m_aRankInfo[eWeeklyRanking][_lpInfo->m_wRank],_lpInfo,sizeof(cDuelRankingInfo));
	}
	void			updateDailyRanker(cDuelRankingInfo *_lpInfo)
	{
		memcpy(&m_aRankInfo[eDailyRanking][_lpInfo->m_wRank],_lpInfo,sizeof(cDuelRankingInfo));
	}
	void			updateTrialGameRanker(cDuelRankingInfo *_lpInfo)
	{
		memcpy(&m_aRankInfo[eTrialGameRanking][_lpInfo->m_wRank],_lpInfo,sizeof(cDuelRankingInfo));
	}

	int				getWeeklyRankerCount()
	{
		return	m_awRankerCount[eWeeklyRanking];
	}
	int				getDailyRankerCount()
	{
		return	m_awRankerCount[eDailyRanking];
	}
	int				getTrialGameRankerCount()
	{
		return	m_awRankerCount[eTrialGameRanking];
	}
	cDuelRankingInfo*	getWeeklyRanker(int _iRank)
	{
		return	&m_aRankInfo[eWeeklyRanking][_iRank];
	}
	cDuelRankingInfo*	getDailyRanker(int _iRank)
	{
		return	&m_aRankInfo[eDailyRanking][_iRank];
	}
	cDuelRankingInfo*	getTrialGameRanker(int _iRank)
	{
		return	&m_aRankInfo[eTrialGameRanking][_iRank];
	}
};

class	CClient;

class	CEventTimeManager
{
public:
	int					m_iCount;
	cEventTimeDefine	m_aEventTimeList[dMAX_EVENT_TIME_COUNT];

	CEventTimeManager();

	void	updateData(int _iCount,cEventTimeDefine *_lpData);
	void	sendEventData(CClient *_lpTargetServer);
};


class	CLottoEventManager
{
public:
	int					m_iCount;
	cLottoItemDropEvent	m_aEventList[dLOTTO_EVENT_COUNT];

	void	load();
	void	save();
	void	updateData(int _iCount,cLottoItemDropEvent *_lpData);
	void	sendEventData(CClient *_lpTargetServer);
};

class	CEventManager
{
public:
	cOXQuiz			m_oxQuiz;
	cWordQuiz		m_wordQuiz;
	CTimeInfo		m_timeLastUpdate;
	CTimeInfo		m_timeTrialGameBegin,m_timeTrialGameEnd;
	CTimeInfo		m_timeOfflineTournamentBegin,m_timeOfflineTournamentEnd;
	CTimeInfo		m_timeLastSoundOfLeadersBell;

	int				m_iBoostExpByLeadersSoundofBell,m_iBoostItemDropChanceByLeadersSoundofBell;
	int				m_iRemainSoundofBellTime;

	char			m_strSoundOfLeadersBellOwner[dNAME_LENGTH];
	char			m_strSoundOfLeadersBellMessage[128];
	BOOL			m_bIsDisplayNameWithSoundofLeadersBell;
	BOOL			m_bIsCancelSoundofLeadersBellEvent;

	int				m_iPastSecondCounter,m_iOldSecondCounter,m_iCurrentSecondCount;

	BOOL			m_bIsChangeSecondTime;
	int				m_iIsSendTrialGameResultMail,m_iTrialGameTeamSize;
	int				m_iUpdateWeeklyRankingDay;
	char			m_strTrialGameName[64];
	char			m_strOfflineTournamentComment[256];
	cEventWord		m_wordCompareEvent;

	int				m_aiDBControlledEventValue[c_iDBControlledEvnetCount];

	CEventManager();

	void			useSoundOfLeadersBell(CClient *_lpServer,SERVERStoWORLD_SOUND_OF_LEADERS_BELL *_lpPacket);
	void			resetSoundOfLedersBellTime(BOOL _bIsCancelEvent);
	void			sendSoundOfLeadersBellEventInfo(CClient *_lpServer);
	void			changeMsgSoundOfLeadersBell(char *_lpstrMsg);

	void			init();
	void			setQuizEventBeginTime(int _iType,int _iBeginYear,int _iBeginMonth,int _iBeginDay,int _iBeginHour,int _iBeginMinute,int _iLimitPlayerCount,BOOL _bIsTest);
	void			setOXQuizBeginTime(int _iBeginYear,int _iBeginMonth,int _iBeginDay,int _iBeginHour,int _iBeginMinute,int _iLimitPlayerCount,BOOL _bIsTest);
	void			setWordQuizBeginTime(int _iBeginYear,int _iBeginMonth,int _iBeginDay,int _iBeginHour,int _iBeginMinute,int _iLimitPlayerCount,BOOL _bIsTest);

	void			setOXQuizVillage(int _iVillage);
	void			setWordQuizVillage(int _iVillage);

	void			increaseRedStoneCount(int _iType);

	void			update();	//	업데이트 
	void			updateSoundOfLeadersBell();
	
	void			updateDBControlledEventValue(int _iEvent,int _iValue);
	void			updateDBControlledEventValue(int *_lpiValues);
	void			resetDBControlledEventValue(int _iEvent)
	{
		m_aiDBControlledEventValue[_iEvent]	=	0;
	}
	int*			getDBControlledEventValue()
	{
		return	m_aiDBControlledEventValue;
	}

	int					m_iChangeSystemTimeMinute;

	void				rollbackChangeTimeInfo();
	void				changeSystemTime(int _iYear,int _iMonth,int _iDay,int _iHour,int _iMinute);	//	시스템 시간 변경.. -_-?;;
};

class	cRenewPremiumItemExpireTime
{
public:
	int							m_iCount,m_iLastRenew;
	cRenewItemExpireTimeInfo	m_aList[dMAX_REQUITAL_TIME_COUNT];

	void						init();
	void						addRenewItemExpireTime(UTime *_lpTime,DWORD _dwRenewMinute,BOOL _bIsForYahoo,BOOL _bIsForRedGem);
	void						save();
};

class	cItemPackManager
{
public:
	cItemPackInfo	m_aPacks[256];

	void			init();
	void			save();
	void			reset();
	DWORD			getCheckSum();
	void			modifyData(int _iSerial,cItemPackInfo *_lpData);
	int				increaseItemCounter(int _iPackSerial,int _iIndex);
	void			resetCounter(int _iPackSerial);
};

class	cGoldRush
{
public:
	UTime			m_begin,m_end;
	WORD			m_wBoostExperience,m_wCorrectItemDropChance;
	char			m_strBeginComment[256],m_strEndComment[256];

	void			init();
	void			reset();
	void			save();
	void			updateInfo(UTime *_lpBegin,UTime *_lpEnd,int _iBoostExperience,int _iCorrectItemDropChance,char *_lpstrBeginComment,char *_lpstrEndComment);
};

class	cCarrotShop	:	public cCarrotShopDefine
{
public:
	void			init();
//	void			save();
	void			reset();
	void			updateInfo(cCarrotShopDefine *_lpData);
//	void			updateInfo(int _iCategory,cCarrotShopCategoryInfoDefine *_lpData);
};

extern	CEventTimeManager			g_eventTimeManager;
extern	CLottoEventManager			g_lottoEventManager;
extern	cDuelRankerManager			g_duelRankerManager;
extern	cDuelTeamManager			g_duelTeamManager;
extern	CEventManager				g_eventManager;
extern	cRenewPremiumItemExpireTime	g_renewPremiumItemExpireTime;
extern	cItemPackManager			g_itemPackManager;
extern	cGoldRush					g_goldRush;
extern	cCarrotShop					g_carrotShopInfo;
extern	int							g_iBoostGoldByTreasureMapEventCount;
extern	int							g_iBoostExpByTreasureMapEventCount;

extern	int							g_iHeavenRedStoneCount;
extern	int							g_iHellRedStoneCount;
extern	int							g_iRedDevilRedStoneCount;

extern	int							g_aiWorldValue[dMAX_WORLD_VALUE_MAX];

#endif