#include "CEvent.h"
#include "cNemo.h"
#include "cMessage.h"

COXQuiz			g_eventOxQuiz;
CCountDown		g_eventCountDown;
CCountDown		g_countDown;
cWordQuiz		g_eventWordQuiz;

CEventManager	g_eventManager;

int				g_iBoostExpSecondByTreasureMapEventInServer	=	0;
int				g_iBoostGoldSecondByTreasureMapEventInServer=	0;
//
//	시작
void
CCountDown::begin(int _iCount,void *_lpTextDC,int _iYPos,BOOL _bIsPutTime)
{
	m_lpTextDC					=	_lpTextDC;
	m_wYP						=	_iYPos;
	m_dwLastDecreaseCountTime	=	timeGetTime();
	m_dwRemainCount				=	_iCount;
	m_wIsPutTime				=	_bIsPutTime;
	m_strTextForm[0]			=	NULL;
}

void
CCountDown::begin(int _iCount,void *_lpTextDC,int _iYPos,char *_lpstrText,BOOL _bIsPutTime)	//	시작
{
	begin(_iCount,_lpTextDC,_iYPos,_bIsPutTime);

	strcpy(m_strTextForm,_lpstrText);
}

//
//	카운트 다운 업데이트
void
CCountDown::update()
{
	if (m_dwRemainCount	==	0)
		return;

	DWORD	dwCurrentTime	=	timeGetTime();

	if (m_dwLastDecreaseCountTime > dwCurrentTime || m_dwLastDecreaseCountTime+1000 < dwCurrentTime)
	{
		if (m_dwLastDecreaseCountTime > dwCurrentTime)
			m_dwLastDecreaseCountTime	=	dwCurrentTime;
		else
			m_dwLastDecreaseCountTime	+=	1000;

		m_dwRemainCount--;
	}
}

void
CCountDown::draw()
{
	if	(m_dwRemainCount	==	0)
		return;

	CTextDC	*lpTextDC	=	(CTextDC	*)m_lpTextDC;

	char	strNumber[32],strText[512];

	if (m_wIsPutTime)
	{
		if (m_dwRemainCount	>=	60*60)
			sprintf(strNumber,dMSG_HOUR_MINUTE_SECOND_FORM,m_dwRemainCount/60/60,(m_dwRemainCount/60)%60,m_dwRemainCount%60);
		else
		if (m_dwRemainCount	>=	60)
			sprintf(strNumber,dMSG_MINUTE_SECOND_FORM,m_dwRemainCount/60,m_dwRemainCount%60);
		else
			sprintf(strNumber,dMSG_SECOND_FORM,m_dwRemainCount);

		if	(m_strTextForm[0])
			sprintf(strText,m_strTextForm,strNumber);
		else
			strcpy(strText,strNumber);
	}
	else
	{
		sprintf(strNumber,"%d",m_dwRemainCount);

		if	(m_strTextForm[0])
			sprintf(strText,m_strTextForm,m_dwRemainCount);
		else
			strcpy(strText,strNumber);
	}


	int	iWidth	=	lpTextDC->getPixelSize(strText);
	int	iHeight	=	lpTextDC->getFontHeight();

	lpTextDC->pushShadowStatus(TRUE);

	cDRAW::FillHB(0,0,m_wYP,cDRAW::WIDTH,m_wYP+iHeight);
	lpTextDC->cPut(cDRAW::WIDTH/2,m_wYP,WHITE,strText);

	lpTextDC->popShadowStatus();
}

void
CEventManager::init()
{
	g_eventWordQuiz.reset();
	g_eventOxQuiz.reset();
	g_eventCountDown.reset();
	g_countDown.reset();
}