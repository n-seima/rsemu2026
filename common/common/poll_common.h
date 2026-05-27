#pragma once

#include	"sfc.h"

#define	dPOLL_TITLE_LENGTH	128
#define	dPOLL_ANSWER_COUNT	9
#define	dPOLL_ANSWER_LENGTH	64

#define	dMAX_POLL_COUNT		256

class	cPollAnswerInfo_common
{
public:
	char	m_strPollAnswer[dPOLL_ANSWER_LENGTH];
	int		m_iPollPoint;	//	직업 32개
	int		m_aiPollPointWithJob[32];	//	직업 32개
	int		m_aiPollPointWithLevel[40];	//	50 레벨 단위
};

class	cPollInfo_common
{
public:
	WORD	m_wPollIndex;
	WORD	m_wAnswerCount;
	WORD	m_wPollMinLevel,m_wPollMaxLevel;
	UTime	m_timeBegin,m_timeEnd;

	char	m_strPollTitle[dPOLL_TITLE_LENGTH];
	char	m_strPollAnswer[dPOLL_ANSWER_COUNT][dPOLL_ANSWER_LENGTH];
	int		m_aiPollPoint[dPOLL_ANSWER_COUNT];

	cPollInfo_common()
	{
		memset(this,0,sizeof(cPollInfo_common));

		m_wPollIndex	=	0xffff;
	}
};

class	cPollManager_common
{
public:
	WORD	m_wPollCount;

	cPollInfo_common	m_aPollList[dMAX_POLL_COUNT];
	
	cPollManager_common()
	{
		memset(this,0,sizeof(cPollInfo_common));
		
		m_wPollCount	=	0;
	}

	cPollInfo_common	
};