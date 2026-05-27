#include "CEvent.h"
#include "CGame.h"
#include "CDecoder.h"

void
COXQuiz::reset()
{
	m_dwFinishEventTime		=	0;
	m_wIsWaitFinishEvent	=	FALSE;
	m_wStatus				=	eOIPS_SLEEP;

	m_dwLastAnnounceTime	=	0;
	m_wQuestionCounter		=	0;

	memset(&m_tiBeginTime,0,sizeof(m_tiBeginTime));
}

void
COXQuiz::setData(int _iYear,int _iMonth,int _iDay,int _iHour,int _iMinute,int _iGateVillage,int _iLimitPlayerCount,BOOL _bIsTest)
{
	cFIELD	*lpField	=	g_game.getOXQuizField();

	if	(lpField)
	{
		if	(m_wIsWaitFinishEvent)
		{
			lpField->endOXQuiz();

			m_wStatus	=	eOIPS_SLEEP;

			reset();
		}

		lpField->m_iLimitPlayerCount	=	_iLimitPlayerCount;
	}
	
	m_tiBeginTime.m_wYear	=	_iYear;
	m_tiBeginTime.m_wMonth	=	_iMonth;
	m_tiBeginTime.m_wDay	=	_iDay;
	m_tiBeginTime.m_wHour	=	_iHour;
	m_tiBeginTime.m_wMinute	=	_iMinute;
	m_tiBeginTime.m_wSecond	=	0;
	m_dwLastAnnounceTime	=	0;
	m_wCountDown			=	0;
	m_dwLastDecreaseCountTime=	0;
	m_wQuestionCounter		=	0;
	m_wLimitPlayerCount		=	_iLimitPlayerCount;
	m_wIsTest				=	_bIsTest;
	m_wQuizGateField		=	_iGateVillage;

	DWORD	dwCurrentTimeValue	=	(g_currentTime.m_wYear-2000)*60*32*12*60+ g_currentTime.m_wMonth*60*32*60+ g_currentTime.m_wDay*60*60+g_currentTime.m_wHour*60+g_currentTime.m_wMinute;
	DWORD	dwBeginTimeValue	=	(m_tiBeginTime.m_wYear-2000)*60*32*12*60+ m_tiBeginTime.m_wMonth*60*32*60+ m_tiBeginTime.m_wDay*60*60+m_tiBeginTime.m_wHour*60+m_tiBeginTime.m_wMinute;

	if (dwCurrentTimeValue		>=	dwBeginTimeValue)	//	이미 시작했다.
		reset();
}

void
COXQuiz::update()
{
	if (m_wIsWaitFinishEvent)
	{
		if (m_dwFinishEventTime+10*60*1000 <	g_dwCurrentTime)
		{
			cFIELD	*lpField	=	g_game.getOXQuizField();

			if (lpField)
				lpField->endOXQuiz();
			
			m_wStatus	=	eOIPS_SLEEP;
	
			reset();
		}

		return;
	}

//	문제를 받은 상태에서 카운트 다운 시작
	if	(m_wCountDown)
	{
		DWORD	dwCurrentTime	=	g_dwCurrentTime;

		if	(m_dwLastDecreaseCountTime > dwCurrentTime || m_dwLastDecreaseCountTime+1000 < dwCurrentTime)
		{
			if (m_dwLastDecreaseCountTime > dwCurrentTime)
				m_dwLastDecreaseCountTime	=	dwCurrentTime;
			else
				m_dwLastDecreaseCountTime	+=	1000;

			m_wCountDown--;
		}

		if	(m_wCountDown	==	0)
		{
			cFIELD	*lpField	=	g_game.getOXQuizField();

			if(lpField)
				lpField->operateOXQuizResult();
		}
	}

//	OX퀴즈 시작 시간 체크
	if	(m_tiBeginTime.m_wYear	==	0)
		return;

	DWORD	dwCurrentTimeValue	=	(g_currentTime.m_wYear-2000)*60*32*12*60+ g_currentTime.m_wMonth*60*32*60+ g_currentTime.m_wDay*60*60+g_currentTime.m_wHour*60+g_currentTime.m_wMinute;
	DWORD	dwBeginTimeValue	=	(m_tiBeginTime.m_wYear-2000)*60*32*12*60+ m_tiBeginTime.m_wMonth*60*32*60+ m_tiBeginTime.m_wDay*60*60+m_tiBeginTime.m_wHour*60+m_tiBeginTime.m_wMinute;

	cFIELD	*lpField			=	g_game.getOXQuizField();

	if	(dwCurrentTimeValue		>=	dwBeginTimeValue)	//	시작했다.
	{
		if	(lpField)
			lpField->beginOXQuiz();

		m_wStatus	=	eOIPS_WAIT_QUESTION;

		reset();

		return;
	}

	if	(lpField)
		dwCurrentTimeValue	+=	2;	//	낱말 퀴즈 이벤트 맵이 있는 서버는 실제보다 2분전에 준비 시작

	if	(dwCurrentTimeValue+30	<	dwBeginTimeValue)	//	아직 시작 하기 30분전이 아니다.
		return;
//
//	OX 퀴즈 시작 시간 30분 전부터 5분에 한번씩 전 서버에 공지한다.
	{
		DWORD	dwTime	=	g_dwCurrentTime;

		if	(lpField)
			lpField->readyOXQuiz();

		m_wStatus	=	eOIPS_READY;

		if	(m_dwLastAnnounceTime + 60*1000 <= dwTime || m_dwLastAnnounceTime > dwTime)	//	5분에 한번씩 공지
		{
			m_dwLastAnnounceTime	=	dwTime;

			SG_ANNOUNCE_QUIZ_EVENT	packet;

			packet.base.set(sizeof(packet),dSG_ANNOUNCE_QUIZ_EVENT);

			packet.wYear			=	m_tiBeginTime.m_wYear;
			packet.wMonth			=	m_tiBeginTime.m_wMonth;
			packet.wDay				=	m_tiBeginTime.m_wDay;
			packet.wHour			=	m_tiBeginTime.m_wHour;
			packet.wMinute			=	m_tiBeginTime.m_wMinute;
			packet.wIsTest			=	m_wIsTest;
			packet.wQuizGateField	=	m_wQuizGateField;
			packet.wType			=	eET_OX_QUIZ;

			g_game.sendPacketToAllPlayer((char *)&packet);
		}
	}
}

//
//	해답 설정
void
COXQuiz::setSolution(int _iSolution)
{
	m_wSolution		=	_iSolution;
}

//
//	ox퀴즈 답 설정
void
COXQuiz::setQuestion(char *_lpstrQuestion,int _iSolution,int _iCount)
{
	m_dwLastDecreaseCountTime	=	g_dwCurrentTime;
	
	m_wCountDown	=	_iCount;
	m_wSolution		=	_iSolution;
	m_wQuestionCounter++;
	strcpy(m_strQuestion,_lpstrQuestion);
}

void
COXQuiz::cancelOXQuiz()
{
	if (m_tiBeginTime.m_wYear	==	0)
		return;

	DWORD	dwCurrentTimeValue	=	(g_currentTime.m_wYear-2000)*60*32*12*60+ g_currentTime.m_wMonth*60*32*60+ g_currentTime.m_wDay*60*60+g_currentTime.m_wHour*60+g_currentTime.m_wMinute;
	DWORD	dwBeginTimeValue	=	(m_tiBeginTime.m_wYear-2000)*60*32*12*60+ m_tiBeginTime.m_wMonth*60*32*60+ m_tiBeginTime.m_wDay*60*60+m_tiBeginTime.m_wHour*60+m_tiBeginTime.m_wMinute;

	if (dwCurrentTimeValue+35	>=	dwBeginTimeValue)	//	아직 시작 하기 30분전이 아니다.
	{
		SG_CANCEL_QUIZ_EVENT	packet;

		packet.base.set(sizeof(packet),dSG_CANCEL_QUIZ_EVENT);

		packet.wYear	=	m_tiBeginTime.m_wYear;
		packet.wMonth	=	m_tiBeginTime.m_wMonth;
		packet.wDay		=	m_tiBeginTime.m_wDay;
		packet.wHour	=	m_tiBeginTime.m_wHour;
		packet.wMinute	=	m_tiBeginTime.m_wMinute;
		packet.wQuizType=	eET_OX_QUIZ;

		g_game.sendPacketToAllPlayer((char *)&packet);
	}

	reset();

	cFIELD	*lpField	=	g_game.getOXQuizField();

	if (lpField)
		lpField->endOXQuiz();

	m_wStatus	=	eOIPS_FINISH;
}

//
//	OX 퀴즈 종료
void
COXQuiz::endEvent()
{
	cFIELD	*lpField	=	g_game.getOXQuizField();

	if (lpField)
		lpField->finishOxQuiz();

	m_wStatus			=	eOIPS_FINISH;

	m_dwFinishEventTime	=	g_dwCurrentTime;
	m_wIsWaitFinishEvent=	TRUE;
}

