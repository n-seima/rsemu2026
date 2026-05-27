#include	 "duel.h"
#include	 "cMESSAGE.h"
#include	 "CGamePlay.h"

cDuelManager	g_duelManager;
cDuelRanking	g_duelRanking;

void
cDuelManager::beginBattle(int _iBattleTime)
{
	m_uiBeginBattleTime	=	timeGetTime();
	m_uiBattleTime		=	_iBattleTime;
	m_uiRemainBattleTime=	_iBattleTime;
}

void
cDuelManager::updateCount(int _iType,int _iCount)
{
	m_iMatchApplicantCount	=	_iCount;
	m_iBattleType			=	_iType;

	if	(m_uiTryTime	==	0)
		m_uiTryTime		=	timeGetTime();
}

void
cDuelManager::update()
{
	if	(m_uiBeginBattleTime)
	{
		DWORD	dwCurrentTime	=	timeGetTime();
		DWORD	dwPastSecond;

		if	(dwCurrentTime	>=	m_uiBeginBattleTime)
			dwPastSecond	=	(dwCurrentTime-m_uiBeginBattleTime)/1000;
		else
			dwPastSecond	=	((0xffffffff-m_uiBeginBattleTime)+dwCurrentTime)/1000;

		if	(dwPastSecond	>=	m_uiBattleTime)
		{
			m_uiBeginBattleTime	=	0;
			return;
		}
			
		m_uiRemainBattleTime=	m_uiBattleTime-dwPastSecond;
	}

	if	(m_uiTryTime)
	{
		DWORD	dwCurrentTime	=	timeGetTime();
		DWORD	dwPastSecond;

		if	(dwCurrentTime	>=	m_uiTryTime)
			dwPastSecond	=	(dwCurrentTime-m_uiTryTime)/1000;
		else
			dwPastSecond	=	((0xffffffff-m_uiTryTime)+dwCurrentTime)/1000;

		char	strText[512];

		if	(m_iBattleType	==	eDUEL_1VS1)
		{
			sprintf(strText,dMSG_1VS1_DUEL_APPLICANT_FORM,
					m_iMatchApplicantCount,dwPastSecond/60,dwPastSecond%60);
		}
		else
		{
			char	strType[64];

			if	(m_iBattleType	==	eDUEL_TRIAL_GAME)
				strcpy(strType,m_strTrialGameName);
			else
				sprintf(strType,"%d vs %d",m_iBattleType+1,m_iBattleType+1);

			sprintf(strText,dMSG_DUEL_APPLICANT_FORM,
					strType,m_iMatchApplicantCount,dwPastSecond/60,dwPastSecond%60);
		}

		if	(m_bIsPoped	==	FALSE)
		{
			m_bIsPoped	=	TRUE;

			g_msgBox.cPopup(dMSG_WAIT_FOR_DUEL_APPLICANT_COUNT,strText,dMSG_CANCEL);
			g_msgBox.setButtonId(eMBI_CANCEL_TRY_DUEL);
			g_msgBox.alignTextByCenter();
			g_msgBox.setIcon(eMBIS_NONE);
			g_msgBox.setSize(0,140);
		}
		else
		{
			g_msgBox.setText(strText);
		}
	}
}

void
cDuelRanking::updateRank(int _iType,int _iCount,cDuelRankingInfoForClient *_lpRank)
{
	m_aiRankCount[_iType]	=	_iCount;
	memcpy(&m_aList[_iType],_lpRank,sizeof(cDuelRankingInfoForClient )*_iCount);
	memcpy(&m_aReceiveTime[_iType],&CGamePlay::s_serverTime,sizeof(CTimeInfo));

	cFILE	file;

	if	(!file.Open("data/duelRanking.dat","wb"))
		return;

	file.Write(this,sizeof(cDuelRanking));
	file.Close();
}