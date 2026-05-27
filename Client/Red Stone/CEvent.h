#ifndef _ClassEVENT_H
#define _ClassEVENT_H

#include "SFC.H"

#define	dOX_QUIZ_SPARE_COUNT	3	//	3초정도 더 기다린다.

class	COXQuiz
{
public:
	char			m_strQuestion[512];
	WORD			m_wIsCountDown;
	CTimeInfo		m_timeBegin;
	BOOL			m_bIsWaitBegin,m_bIsEndEvent;
	char			m_strBeginTimeAnnounce[512];

	COXQuiz()
	{
		reset();
	}

	void			reset()
	{
		memset(&m_timeBegin,0,sizeof(CTimeInfo));

		m_wIsCountDown	=	FALSE;
		m_bIsWaitBegin	=	FALSE;
		m_bIsEndEvent	=	FALSE;
	}
};

class	cWordQuiz
{
public:
	char			m_strQuestion[512];
	WORD			m_wIsCountDown;
	WORD			m_wScore;
	WORD			m_wRound;
	WORD			m_wCommerCount,m_wLiveUserCount,m_wScorerCount;
	CTimeInfo		m_timeBegin;
	BOOL			m_bIsWaitBegin,m_bIsEndEvent;
	char			m_strBeginTimeAnnounce[512];

	WORD			m_wTop10Count;
	WORD			m_awTop10Score[10];
	char			m_strTop10List[10][18];

	cWordQuiz()
	{
		reset();
	}

	void			begin()
	{
		m_wIsCountDown	=	FALSE;
		m_bIsWaitBegin	=	FALSE;
		m_bIsEndEvent	=	FALSE;
		m_wScore		=	0;
		m_wTop10Count	=	0;
		m_wRound		=	0;
		m_wScorerCount	=	0;
	}
	void			reset()
	{
		memset(&m_timeBegin,0,sizeof(CTimeInfo));

		m_wIsCountDown	=	FALSE;
		m_bIsWaitBegin	=	FALSE;
		m_bIsEndEvent	=	FALSE;
		m_wScore		=	0;
		m_wTop10Count	=	0;
		m_wRound		=	0;
		m_wCommerCount	=	0;
		m_wLiveUserCount=	0;
		m_wScorerCount	=	0;
	}
};

class	CCountDown
{
public:
	WORD			m_wYP;
	WORD			m_wIsPutTime;
	DWORD			m_dwRemainCount,m_dwLastDecreaseCountTime;
	void			*m_lpTextDC;
	char			m_strTextForm[256];

	CCountDown()
	{
		reset();
		m_lpTextDC	=	NULL;
	}

	void			reset()
	{
		m_dwRemainCount				=	0;
		m_dwLastDecreaseCountTime	=	0;
	}

	void			begin(int _iCount,void *_lpTextDC,int _iYPos,BOOL _bIsPutTime=FALSE);	//	시작
	void			begin(int _iCount,void *_lpTextDC,int _iYPos,char *_lpstrText,BOOL _bIsPutTime=FALSE);	//	시작
	void			update();
	void			draw();
};

class	CEventManager
{
public:
	void			init();
};

extern	cWordQuiz		g_eventWordQuiz;
extern	COXQuiz			g_eventOxQuiz;
extern	CCountDown		g_eventCountDown;
extern	CCountDown		g_countDown;
extern	CEventManager	g_eventManager;

extern	int				g_iBoostExpSecondByTreasureMapEventInServer,g_iBoostGoldSecondByTreasureMapEventInServer;

#endif