#include "CEvent.h"
#include "CGame.h"
#include "CDecoder.h"

void
cWordQuiz::reset()
{
	m_dwFinishEventTime		=	0;
	m_wIsWaitFinishEvent	=	FALSE;
	m_wStatus				=	eWQPS_SLEEP;

	m_dwLastAnnounceTime	=	0;
	m_wQuestionCounter		=	0;

	memset(&m_tiBeginTime,0,sizeof(m_tiBeginTime));
}

void
cWordQuiz::setData(int _iYear,int _iMonth,int _iDay,int _iHour,int _iMinute,int _iGateVillage,int _iLimitPlayerCount,BOOL _bIsTest , char* _strEventName)
{
	cFIELD	*lpField	=	g_game.getWordQuizField();

	if	(lpField)
	{
		if	(m_wIsWaitFinishEvent)
		{
			lpField->endWordQuiz();

			m_wStatus	=	eWQPS_SLEEP;

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
	//if(_strEventName)
	//	strcpy(m_strWordEventName ,_strEventName);


	DWORD	dwCurrentTimeValue	=	(g_currentTime.m_wYear-2000)*60*32*12*60+ g_currentTime.m_wMonth*60*32*60+ g_currentTime.m_wDay*60*60+g_currentTime.m_wHour*60+g_currentTime.m_wMinute;
	DWORD	dwBeginTimeValue	=	(m_tiBeginTime.m_wYear-2000)*60*32*12*60+ m_tiBeginTime.m_wMonth*60*32*60+ m_tiBeginTime.m_wDay*60*60+m_tiBeginTime.m_wHour*60+m_tiBeginTime.m_wMinute;

	if	(dwCurrentTimeValue		>=	dwBeginTimeValue)	//	이미 시작했다.
		reset();
}

void
cWordQuiz::update()
{
	if	(m_wIsWaitFinishEvent)
	{
		if	(m_dwFinishEventTime+10*60*1000 <	g_dwCurrentTime)
		{
			cFIELD	*lpField	=	g_game.getWordQuizField();

			if	(lpField)
				lpField->endWordQuiz();

			m_wStatus	=	eWQPS_SLEEP;
	
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
			if	(m_dwLastDecreaseCountTime > dwCurrentTime)
				m_dwLastDecreaseCountTime	=	dwCurrentTime;
			else
				m_dwLastDecreaseCountTime	+=	1000;

			m_wCountDown--;
		}

		if	(m_wCountDown	==	0)
		{
			cFIELD	*lpField	=	g_game.getWordQuizField();

			if	(lpField)
				lpField->operateWordQuizResult();

			m_wWinnerCount		=	0;
		}
	}

//	낱말 퀴즈 시작 시간 체크
	if	(m_tiBeginTime.m_wYear	==	0)
		return;

	DWORD	dwCurrentTimeValue	=	(g_currentTime.m_wYear-2000)*60*32*12*60+ g_currentTime.m_wMonth*60*32*60+ g_currentTime.m_wDay*60*60+g_currentTime.m_wHour*60+g_currentTime.m_wMinute;
	DWORD	dwBeginTimeValue	=	(m_tiBeginTime.m_wYear-2000)*60*32*12*60+ m_tiBeginTime.m_wMonth*60*32*60+ m_tiBeginTime.m_wDay*60*60+m_tiBeginTime.m_wHour*60+m_tiBeginTime.m_wMinute;

	cFIELD	*lpField	=	g_game.getWordQuizField();

	if	(dwCurrentTimeValue		>=	dwBeginTimeValue)	//	시작했다.
	{
		if	(lpField)
			lpField->beginWordQuiz();

		m_wStatus	=	eWQPS_WAIT_QUESTION;

		reset();

		return;
	}

	if	(lpField)
		dwCurrentTimeValue	+=	2;	//	낱말 퀴즈 이벤트 맵이 있는 서버는 실제보다 2분전에 준비 시작

	if	(dwCurrentTimeValue+30	<	dwBeginTimeValue)	//	아직 시작 하기 30분전이 아니다.
		return;

//
//	낱말 퀴즈 시작 시간 30분 전부터 5분에 한번씩 전 서버에 공지한다.
	{
		DWORD	dwTime	=	g_dwCurrentTime;

		if	(lpField)
			lpField->readyWordQuiz();

		m_wStatus	=	eWQPS_READY;

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
			packet.wType			=	eET_WORD_QUIZ;
			strcpy(packet.strQuizEventName,m_strWordEventName);

			g_game.sendPacketToAllPlayer((char *)&packet);
		}
	}
}

//
//	해답 설정
void
cWordQuiz::setSolution(char *_lpstrSolution)
{
	strcpy(m_strSolution,_lpstrSolution);
}

//
//	낱말 퀴즈 답 설정
void
cWordQuiz::setQuestion(char *_lpstrQuestion,char *_lpstrSolution,int _iCount)
{
	m_dwLastDecreaseCountTime	=	g_dwCurrentTime;
	
	m_wCountDown	=	_iCount;
	m_wWinnerCount	=	0;

	m_wQuestionCounter++;

	strcpy(m_strQuestion,_lpstrQuestion);
	strcpy(m_strSolution,_lpstrSolution);
}

void
cWordQuiz::increaseCorrectAnswerCount()
{
	m_wWinnerCount++;
}

int
cWordQuiz::getCurrentScore()
{
	int	iRankerCount	=	5;
	int	aiScore[]		=	{7,5,5,3,3,3,-1};

	if	(m_wWinnerCount	>	iRankerCount)
		return	1;

	return	aiScore[m_wWinnerCount];
}


void
cWordQuiz::cancelQuiz()
{
	if	(m_tiBeginTime.m_wYear	==	0)
		return;

	DWORD	dwCurrentTimeValue	=	(g_currentTime.m_wYear-2000)*60*32*12*60+ g_currentTime.m_wMonth*60*32*60+ g_currentTime.m_wDay*60*60+g_currentTime.m_wHour*60+g_currentTime.m_wMinute;
	DWORD	dwBeginTimeValue	=	(m_tiBeginTime.m_wYear-2000)*60*32*12*60+ m_tiBeginTime.m_wMonth*60*32*60+ m_tiBeginTime.m_wDay*60*60+m_tiBeginTime.m_wHour*60+m_tiBeginTime.m_wMinute;

	if	(dwCurrentTimeValue+35	>=	dwBeginTimeValue)	//	아직 시작 하기 30분전이 아니다.
	{
		SG_CANCEL_QUIZ_EVENT	packet;

		packet.base.set(sizeof(packet),dSG_CANCEL_QUIZ_EVENT);

		packet.wYear	=	m_tiBeginTime.m_wYear;
		packet.wMonth	=	m_tiBeginTime.m_wMonth;
		packet.wDay		=	m_tiBeginTime.m_wDay;
		packet.wHour	=	m_tiBeginTime.m_wHour;
		packet.wMinute	=	m_tiBeginTime.m_wMinute;
		packet.wQuizType=	eET_WORD_QUIZ;

		g_game.sendPacketToAllPlayer((char *)&packet);
	}

	reset();

	cFIELD	*lpField	=	g_game.getWordQuizField();

	if	(lpField)
		lpField->endWordQuiz();

	m_wStatus	=	eWQPS_FINISH;
}

//
//	낱말 퀴즈 종료
void
cWordQuiz::endEvent()
{
	cFIELD	*lpField	=	g_game.getWordQuizField();

	if	(lpField)
		lpField->finishWordQuiz();

	m_wStatus			=	eWQPS_FINISH;

	m_dwFinishEventTime	=	g_dwCurrentTime;
	m_wIsWaitFinishEvent=	TRUE;
}