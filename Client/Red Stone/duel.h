#pragma once

#include "SFC.h"
#include "cPACKET_GAMESERVER.h"

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
	eDUEL_BUFF_NONE,
	eDUEL_BUFF_FIRE,
	eDUEL_BUFF_WATER,
	eDUEL_BUFF_WIND,
	eDUEL_BUFF_EARTH,
	eDUEL_BUFF_STATUS_COUNT,
};

class	cDuelManager
{
public:
	char	m_strTrialGameName[64];

	int		m_iMatchApplicantCount,m_iBattleType;
	UINT	m_uiTryTime;
	BOOL	m_bIsPoped;

	UINT	m_uiBattleTime,m_uiRemainBattleTime,m_uiBeginBattleTime;

	void	beginBattle(int _iBattleTime);
	void	updateCount(int _iType,int _iCount);
	void	update();
	void	reset()
	{
		m_uiTryTime			=	0;
		m_uiBeginBattleTime	=	0;
		m_bIsPoped			=	FALSE;
	}
};

class	cDuelRanking
{
public:
	int							m_aiRankCount[3];
	CTimeInfo					m_aReceiveTime[3];
	cDuelRankingInfoForClient	m_aList[3][100];

	cDuelRanking()
	{
		memset(m_aReceiveTime,0,sizeof(m_aReceiveTime));
		memset(m_aiRankCount,0,sizeof(m_aiRankCount));
	}

	void	resetRanking(int _iType)
	{
		m_aiRankCount[_iType]	=	0;
	}

	int		getCount(int _iType)
	{
		return	m_aiRankCount[_iType];
	}

	cDuelRankingInfoForClient*	getList(int _iType)
	{
		return	m_aList[_iType];
	}

	void	updateRank(int _iType,int _iCount,cDuelRankingInfoForClient *_lpRank);
};

extern	cDuelManager	g_duelManager;
extern	cDuelRanking	g_duelRanking;