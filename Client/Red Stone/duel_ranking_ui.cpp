#include	"commonUI.h"
#include	"cIMAGE.h"
#include	"cTEXT.h"
#include	"CGamePlay.h"
#include	"duel.h"
#include	"CImageData.h"

void
cCommonUI_duel_ranking::init()
{
	cFILE	file;

	m_iRankingType	=	0;

	g_commonUI.setBoxSize(260,10+g_iCorrectBI_Y/2,520,454+g_iCorrectBI_Y/2);

	CGamePlay::s_rectInterfaceSize.x1 = g_iScreenWidth-m_rectBox.x1;
	CGamePlay::s_rectInterfaceSize.x2 = g_iScreenWidth;

	if	(!file.Open("data/duelRanking.dat","rb"))
	{
		CGamePlay::s_agent.sendEtcWork(eCEW_ASK_DUEL_RANKING,m_iRankingType);
		return;
	}

	file.Read(&g_duelRanking,sizeof(g_duelRanking));
	file.Close();

	CTimeInfo	*lpSaveTime		=	&g_duelRanking.m_aReceiveTime[m_iRankingType];
	CTimeInfo	*lpServerTime	=	&CGamePlay::s_currentTime;

	int			iSaveDayValue	=	lpSaveTime->m_wYear*365+lpSaveTime->m_wMonth*31+lpSaveTime->m_wDay;
	int			iCurDayValue	=	lpServerTime->m_wYear*365+lpServerTime->m_wMonth*31+lpServerTime->m_wDay;

	g_duelRanking.resetRanking(m_iRankingType);

	CGamePlay::s_agent.sendEtcWork(eCEW_ASK_DUEL_RANKING,m_iRankingType);

	CGamePlay::s_sbPopupWindow.set(m_rectBox.x2-10,m_rectBox.y1+34,m_rectBox.getHeight()-66);
	CGamePlay::s_sbPopupWindow.setPos(0);

	{
		int	iX	=	m_rectBox.x1+4;
		int	iY	=	46+g_iCorrectBI_Y/2;

		CGamePlay::s_bmRadioBox.reset();
		CGamePlay::s_bmRadioBox.addImageBar(dMSG_WEEKLY_RANKING,eWeeklyRanking	,iX	,iY,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX_CHECKED);
		iX		+=	CGamePlay::s_text.getPixelSize(dMSG_WEEKLY_RANKING)+24;
		CGamePlay::s_bmRadioBox.addImageBar(dMSG_DAILY_RANKING,eDailyRanking	,iX	,iY,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX_CHECKED);
		iX		+=	CGamePlay::s_text.getPixelSize(dMSG_DAILY_RANKING)+24;
		char	strDuelTrialGameName[128];
		sprintf(strDuelTrialGameName,dMSG_WCG_RANKING,g_duelManager.m_strTrialGameName);
		CGamePlay::s_bmRadioBox.addImageBar(strDuelTrialGameName,eTrialGameRanking	,iX	,iY,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX_CHECKED);
		CGamePlay::s_bmRadioBox.check(m_iRankingType);
	}
}

void
cCommonUI_duel_ranking::draw()
{
	cDuelRankingInfoForClient*	lpList	=	g_duelRanking.getList(m_iRankingType);

	int		iCount	=	g_duelRanking.getCount(m_iRankingType);

	CPos	posAxis;
	int		iX	=	m_rectBox.x1+20;
	int		iY	=	m_rectBox.y1+54;

	posAxis.x	=	iX;
	posAxis.y	=	iY;

	CTextDC	*lpTextDC	=	&CGamePlay::s_text;
	CTextDC	*lpTitleDC	=	&CGamePlay::s_textBold13;

	lpTitleDC->cPut(iX,iY,WHITE,dMSG_RANKING);
	iX	+=	100;

	if	(m_iRankingType	==	eTrialGameRanking)
		lpTitleDC->cPut(iX,iY,WHITE,dMSG_TEAM_NAME);
	else
		lpTitleDC->cPut(iX,iY,WHITE,dMSG_NAME);

	iX	+=	120;

	if	(m_iRankingType	!=	eTrialGameRanking)
	{
		lpTitleDC->cPut(iX,iY,WHITE,dMSG_JOB);
		iX	+=	80;
	}

	lpTitleDC->cPut(iX,iY,WHITE,dMSG_WINNING_POINT);
	iX	+=	110;
	lpTitleDC->cPut(iX,iY,WHITE,dMSG_MILITARY_RECORD);

	iY	+=	18;

	int		iPageSize	=	25;
	int		iLineCount	=	0;

	for (int i=CGamePlay::s_sbPopupWindow.m_iCurrentPos;i<iCount;i++)
	{
		cDuelRankingInfoForClient*	lpInfo	=	&lpList[i];

		UINT	uiColor		=	WHITE;

		if	((i/5)%2	==	1)
			uiColor	=	GRAY;

		iX	=	posAxis.x;
		lpTextDC->cPut(iX,iY,uiColor,"%d",i+1);
		iX	+=	100;
		lpTextDC->cPut(iX,iY,uiColor,lpInfo->m_strName);
		iX	+=	120;

		if	(m_iRankingType	!=	eTrialGameRanking)
		{
			lpTextDC->cPut(iX,iY,uiColor,g_pstrHeroJobName[lpInfo->m_wJob]);
			iX	+=	80;
		}

		lpTextDC->cPut(iX,iY,uiColor,"%d",lpInfo->m_iWinPoint);
		iX	+=	110;
		lpTextDC->cPut(iX,iY,uiColor,dMSG_DUEL_RECORD_FORM,lpInfo->m_wBattleCount,lpInfo->m_wWinCount,lpInfo->m_wDefeatCount,lpInfo->m_wDrawCount);

		iY	+=	14;

		iLineCount++;

		if	(iLineCount	>=	iPageSize)
			break;
	}

	CGamePlay::s_sbPopupWindow.draw();
	CGamePlay::s_bmRadioBox.draw();

	CGamePlay::s_sbPopupWindow.setSize(iCount,iPageSize);

}

void
cCommonUI_duel_ranking::update()
{
	{
		char	strDuelTrialGameName[128];
		sprintf(strDuelTrialGameName,dMSG_WCG_RANKING,g_duelManager.m_strTrialGameName);
		CGamePlay::s_bmRadioBox.setText(eTrialGameRanking,strDuelTrialGameName);
	}

	CGamePlay::s_sbPopupWindow.update(CGamePlay::s_posMouse.x,CGamePlay::s_posMouse.y,CGamePlay::s_aKey,CGamePlay::s_isTouchLeftButton,CGamePlay::s_isLeftButtonPressed);

	int		iMenu		=	CGamePlay::s_bmRadioBox.update(CGamePlay::s_posMouse.x,CGamePlay::s_posMouse.y,CGamePlay::s_aKey,CGamePlay::s_isTouchLeftButton,CGamePlay::s_isLeftButtonPressed);
	BOOL	bIsChangeType=	FALSE;

	switch(iMenu)
	{
		case	eWeeklyRanking	:
		case	eDailyRanking	:
		case	eTrialGameRanking		:
			if	(iMenu	==	m_iRankingType)
				break;

			m_iRankingType	=	iMenu;
			bIsChangeType	=	TRUE;
			break;
	}

	if	(bIsChangeType)
	{
		switch(iMenu)
		{
			case	eWeeklyRanking	:
				strcpy(m_strTitleText,dMSG_WEEKLY_DUEL_RANK);
				break;
			case	eDailyRanking	:
				strcpy(m_strTitleText,dMSG_DAILY_DUEL_RANK);
				break;
			case	eTrialGameRanking		:
			{
				sprintf(m_strTitleText,dMSG_WCG_RANKING,g_duelManager.m_strTrialGameName);
				break;
			}
		}

		g_duelRanking.resetRanking(m_iRankingType);

		CGamePlay::s_agent.sendEtcWork(eCEW_ASK_DUEL_RANKING,m_iRankingType);
	}
}