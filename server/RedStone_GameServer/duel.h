#pragma once

#include "SFC.h"
#include "rsComm.h"

enum
{
	eDUEL_1VS1	=	0,
	eDUEL_2VS2,
	eDUEL_3VS3,
	eDUEL_4VS4,
	eDUEL_5VS5,
	eDUEL_TRIAL_GAME,
	eDUEL_MATCH_TYPE_COUNT,
};

enum
{
	eDUEL_MATCH_STEP_IDLE,
	eDUEL_MATCH_STEP_ENTRY,
	eDUEL_MATCH_STEP_READY,
	eDUEL_MATCH_STEP_BATTLE,
	eDUEL_MATCH_STEP_FINISH,
	eDUEL_MATCH_STEP_CLOSING,
};

enum
{
	eDUEL_BUFF_NONE,
	eDUEL_BUFF_FIRE,
	eDUEL_BUFF_WATER,
	eDUEL_BUFF_WIND,
	eDUEL_BUFF_EARTH,
	eDUEL_BUFF_STATUS_COUNT,
};

const	int	c_aiDuelBuffCorrectDamageValue[eDUEL_BUFF_STATUS_COUNT][eDUEL_BUFF_STATUS_COUNT]	=	
{
	//무	불		물		바람	대지
	{0	,	0,		0,		0,		0},	//	무
	{50	,	0,		-25,	25,		0},	//	불
	{50	,	25,		0,		0,		-25},	//	물
	{50	,	-25,	0,		0,		25},	//	바람
	{50	,	0,		25,		-25,	0},		//	대지
};

extern	int	GetDuelBattleTime(int _iType);	//	결투 시간 얻기
extern	int	GetKillCountForDuel(int _iType);	//	결투 시간 얻기
extern	int	GetWinPointForDuel(int _iType,BOOL _bIsLoser=FALSE);	//	결투 승점 얻기
extern	int	GetDuelPointForDuel(int _iType,BOOL _bIsLoser=FALSE);	//	결투 포인트 얻기

const	int	c_iDuelBuffCount				=	4;
const	int	c_iInvincibleTimeForDuelRebirth	=	16+16/2;
const	int	c_iBuffForDuelItemCount			=	3;
const	int	c_iDuelBuffCoolTime				=	10;
const	int	c_iWaitForEnterDuelFieldTime	=	30;
const	int	c_iWaitTimeToBeginDuel			=	10;
const	int	c_iWaitTimeToBanishFromDuelField=	30;
const	int	c_iDuelServerDamageFactor		=	2;

class	cACTOR;

class	cDuelBattler
{
public:
	char	m_strName[dNAME_LENGTH];
	char	m_strId[dID_LENGTH];
	WORD	m_wDuelTeamIndex;
	WORD	m_wTeam;
	WORD	m_wKillCount,m_wDeathCount;

	void	reset()
	{
		m_strName[0]	=	NULL;
		m_wTeam			=	0xffff;
		m_wDuelTeamIndex=	0xffff;
	}
};

class	cDuelApplicantInfo
{
public:
	WORD	m_wSerial;
	UINT	m_uiUniqueSerial;
	WORD	m_wType;
	WORD	m_wPartySerial,m_wMemberCount;
	int		m_iTryTime;
	UINT	m_auiMemberHashCode[5];
	int		m_iWinPoint;
	WORD	m_wTryField;
	cACTOR	*m_lpActor;

	char	m_strTeamName[dNAME_LENGTH];
	char	m_strLeaderName[dNAME_LENGTH];

			cDuelApplicantInfo()
			{
				m_wSerial	=	0xffff;
			}

			~cDuelApplicantInfo()
			{
			}

	bool	copy(cDuelApplicantInfo *_lpData)
	{
		memcpy(this,_lpData,sizeof(cDuelApplicantInfo));

		return	true;
	}

	void	reset()
	{
		m_wSerial	=	0xffff;
	}

	bool	isCorrect();
};

class	cDuelManager
{
public:
	cDen<cDuelApplicantInfo>	m_aDen[eDUEL_MATCH_TYPE_COUNT];
	UINT		m_uiCurrentUniqueSerial;
	UINT		m_uiBeginUniqueSerial;
	BOOL		m_bIsChangeApplicantInfo;

			cDuelManager();
			~cDuelManager();

	void	update();

	void	cancel(int _iType,int _iSerial);
	void	cancel(cACTOR *_lpActor);
	bool	tryDuelMatch(cACTOR *_lpActor,int _iDuelMatchType);
	cDuelApplicantInfo*	findMatchPartner(int _iMatchType,int _iApplicant);
	bool	beginMatch(cDuelApplicantInfo* _lpInfo,cDuelApplicantInfo* _lpPartner);

	void	sendCurrentApplicantInfo();
	void	getApplicantCount(int _iType,int *_lpiCount);
};


class	cDuelRanker
{
public:
	UINT	m_uiNameHashCode;
	char	m_strName[dNAME_LENGTH];
	int		m_iPoint;
};

class	cDuelRankerManager
{
public:
	WORD				m_awRankerCount[3];
	cDuelRankingInfo	m_aRankInfo[3][100];
	UINT				m_uiIDHashCode[3][100];

	cDuelRankerManager()
	{
		memset(m_awRankerCount,0,sizeof(m_awRankerCount));
	}

	int				getRanking(int _iWhat,char *_lpstrID);

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

		m_uiIDHashCode[eWeeklyRanking][_lpInfo->m_wRank]	=	GetHashCode(_lpInfo->m_strID);
	}
	void			updateDailyRanker(cDuelRankingInfo *_lpInfo)
	{
		memcpy(&m_aRankInfo[eDailyRanking][_lpInfo->m_wRank],_lpInfo,sizeof(cDuelRankingInfo));
		m_uiIDHashCode[eDailyRanking][_lpInfo->m_wRank]	=	GetHashCode(_lpInfo->m_strID);
	}
	void			updateTrialGameRanker(cDuelRankingInfo *_lpInfo)
	{
		memcpy(&m_aRankInfo[eTrialGameRanking][_lpInfo->m_wRank],_lpInfo,sizeof(cDuelRankingInfo));
		m_uiIDHashCode[eTrialGameRanking][_lpInfo->m_wRank]	=	GetHashCode(_lpInfo->m_strID);
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
	int				getRankerCount(int _iType)
	{
		return	m_awRankerCount[_iType];
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
	cDuelRankingInfo*	getRankInfo(int _iType,int _iRank)
	{
		return	&m_aRankInfo[_iType][_iRank];
	}
};

class	cAdvDuelTeamInfo	:	public cDuelTeamInfo
{
public:
	UINT	m_uiMemberHashCode[5];

	BOOL	isMember(char *_lpstrName);
};

class	cTrialGameManager
{
public:
	WORD				m_wTotalTeamCount;
	WORD				m_wTeamCount;
	WORD				m_wIsSendTrialGameResultMail;
	cAdvDuelTeamInfo	*m_pTeams;
	CTimeInfo			m_timeTrialGameBegin,m_timeTrialGameEnd;
	char				m_strTrialGameName[32];
	CTimeInfo			m_timeOfflineTournamentBegin,m_timeOfflineTournamentEnd;
	char				m_strOfflineTournamentComment[256];

	cTrialGameManager();
	~cTrialGameManager();

	void				init(int _iSize);
	bool				addTeam(cDuelTeamInfo *_lpTeamInfo);
	cAdvDuelTeamInfo	*getTeam(int _iSlot);
	cAdvDuelTeamInfo	*getTeamByMember(char *_lpstrName);

	BOOL				isTrialGameSeason();
	BOOL				isOfflineTournamentSeason();
	BOOL				checkEntry(cACTOR *_lpActor);

	void	setTrialGameSchedule(char *_lpstrName,
								int _iTrialGameBeginYear,int _iTrialGameBeginMonth,int _iTrialGameBeginDay,int _iTrialGameBeginHour,int _iTrialGameBeginMinute,
								int _iTrialGameEndYear,int _iTrialGameEndMonth,int _iTrialGameEndDay,int _iTrialGameEndHour,int _iTrialGameEndMinute);
	void	setOfflineTournamentSchedule(char *_lpstrComment,int _iBeginYear,int _iBeginMonth,int _iBeginDay,int _iBeginHour,int _iBeginMinute,
															 int _iEndYear,int _iEndMonth,int _iEndDay,int _iEndHour,int _iEndMinute);

};

extern	cDuelManager		*g_pDuelManager;
extern	cDuelRankerManager	g_duelRankerManager;
extern	cTrialGameManager	g_duelTrialGameManager;

extern	int			g_aiDuelFieldCount[6];
extern	int			g_iTrialGameTeamSize;
extern	int			g_iCorrectHealEfficientUnderDuelBattle;
const	int			c_iDuelBattleGateField[6]	=	{901,902,903,904,905,906};

extern	int			g_iDuelFieldSerial;
