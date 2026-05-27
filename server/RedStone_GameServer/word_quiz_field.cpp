#include "cFIELD.H"
#include "cGAME.H"
#include "debugCode.H"
#include "CGuild.H"
#include "CEvent.H"
#include "booking_work.H"

//
//	낱말 퀴즈 준비
void
cFIELD::readyWordQuiz()
{
	m_wordQuizStatus.m_wStatus					=	eWQPS_READY;
	m_wordQuizStatus.m_wWinnerCount				=	0;
	m_wordQuizStatus.m_wComerCount				=	0;
	m_wordQuizStatus.m_wIsSaveResult			=	FALSE;
}

//
//	낱말 퀴즈 시작
void
cFIELD::beginWordQuiz()
{
	if	(m_wordQuizStatus.m_wStatus!=	eWQPS_READY)
		return;

	m_wordQuizStatus.m_wStatus					=	eWQPS_WAIT_QUESTION;
	m_wordQuizStatus.m_wWinnerCount				=	0;
	m_wordQuizStatus.m_wComerCount				=	0;
	m_wordQuizStatus.m_wIsSaveResult			=	FALSE;
	m_wWordQuizRoundForRemainActor				=	0;
	m_wWordQuizRound							=	0;

	SG_BEGIN_QUIZ_EVENT	packet;

	packet.base.set(sizeof(packet),dSG_BEGIN_QUIZ_EVENT);
	packet.wType	=	eET_WORD_QUIZ;

	addSendPacket((ALL_MSG *) &packet,0xffff);
	sendPacketToAllOperator((ALL_MSG *) &packet);

	char	astrList[100][dNAME_LENGTH];
	int		iCount	=	0;

	CLOG("Word Quiz","-------- Commer List ------------------------------------------------");
	CLOG("Word Quiz","▶▶ %4d/%2d/%2d %2d:%2d:%2d ◀◀",g_currentTime.m_wYear,g_currentTime.m_wMonth,g_currentTime.m_wDay,g_currentTime.m_wHour,g_currentTime.m_wMinute,g_currentTime.m_wSecond);

	for (int iPlayer=m_aiActorCount[0];iPlayer;)
	{
		--iPlayer;

		int		iActor		=	m_awActorList[0][iPlayer];
		cACTOR	*lpPlayer	=	getActor(iActor);

		if	(!lpPlayer	||	!lpPlayer->isPlayer())
			continue;

		if	(g_bIsTestServer)
		{
			if	(lpPlayer->m_wOperatorLevel	>=	3)
				continue;
		}
		else
		{
			if	(lpPlayer->m_wOperatorLevel)
				continue;
		}

		m_pWordActorInfo[iActor].beginQuiz();

		strcpy(astrList[iCount++],lpPlayer->m_strName);
		CLOG("Word Quiz","    %s/%s",lpPlayer->m_strId,lpPlayer->m_strName);

		m_wordQuizStatus.m_wComerCount++;

		if	(iCount >= 100)
		{
			g_game.sendQuizComerListLog((char *)astrList,iCount,eEW_WORDQUIZ_COMER_LIST);

			iCount	=	0;
		}
	}

	CLOG("Word Quiz","Total %d persons commer",m_wordQuizStatus.m_wComerCount);
	CLOG("Word Quiz","-----------------------------------------------------------------------");
	CLOG("Word Quiz"," ");

	if	(iCount)
		g_game.sendQuizComerListLog((char *)astrList,iCount,eEW_WORDQUIZ_COMER_LIST);

	{
		SG_ETC_WORK	packetInfo;

		packetInfo.base.set(sizeof(SG_ETC_WORK),dSG_ETC_WORK);

		packetInfo.wWork			=	eEW_WORDQUIZ_SURVIVOR_COUNT;
		packetInfo.aiValue[0]		=	m_wordQuizStatus.m_wComerCount;
		packetInfo.aiValue[1]		=	m_wordQuizStatus.m_wComerCount;
		packetInfo.aiValue[2]		=	0;

		sendPacketToAllOperator((ALL_MSG *)&packetInfo);	//	운영자에게 우선적으로 전송한다.
	}
}

//
//	낱말 퀴즈 문제 제공 받았다.
void
cFIELD::producedWordQuizQuestion()
{
	if	(m_wordQuizStatus.m_wStatus	!=	eWQPS_WAIT_QUESTION)
	{
		CLOG("Word Quiz","!!!!!!!!!!!!!! 문제 출제할 상황이 아닌데 문제를 출제함");
		if	(m_wordQuizStatus.m_wStatus	!=	eWQPS_COUNTDOWN)
			CLOG("Word Quiz","!!!!!!!!!!!!!! 문제 %.2d 출제중 다시 문제를 냄!!!!!!!!!!!!!!!!!!!!!!!!!",g_eventManager.getWordQuiz()->m_wQuestionCounter-1);
		return;
	}

	g_game.sendWordQuizQuestionLog(g_eventManager.getWordQuiz()->m_strQuestion,g_eventManager.getWordQuiz()->m_strSolution,g_eventManager.getWordQuiz()->m_wQuestionCounter);

	CLOG("Word Quiz","-------- 문제 %.2d-------------------------------------------------------",g_eventManager.getWordQuiz()->m_wQuestionCounter);
	CLOG("Word Quiz","    질문 : %s",g_eventManager.getWordQuiz()->m_strQuestion);
	CLOG("Word Quiz","    답   : %s",g_eventManager.getWordQuiz()->m_strSolution);
	CLOG("Word Quiz","-----------------------------------------------------------------------");
	CLOG("Word Quiz"," ");

	BOOL	bIsForcedSend	=	FALSE;

	SG_QUIZ_EVENT_QUESTION		packet;

	packet.base.set(sizeof(packet),dSG_QUIZ_EVENT_QUESTION);

	strcpy(packet.strQuestion,g_eventManager.getWordQuiz()->m_strQuestion);

	packet.wWaitCount			=	g_eventManager.getWordQuiz()->m_wCountDown-dOX_QUIZ_SPARE_COUNT;
	packet.bf10QuestionCounter	=	g_eventManager.getWordQuiz()->m_wQuestionCounter;
	packet.bf4QuizType			=	eET_WORD_QUIZ;
	packet.base.wSize			=	sizeof(packet)-sizeof(packet.strQuestion) + strlen(packet.strQuestion)+1;

	m_wordQuizStatus.m_wStatus	=	eWQPS_COUNTDOWN;

	for (int i=m_aiActorCount[0];i;)
	{
		--i;

		int	iActor	=	m_awActorList[0][i];

		cACTOR *lpActor		=	getActor(iActor);

		if	(!lpActor	||	!lpActor->isPlayer())
			continue;

		m_pWordActorInfo[iActor].produceQuiz();
	}

	if	(bIsForcedSend	==	FALSE)
		addSendPacket((ALL_MSG *) &packet,0xffff);
	else
	{
		for (int i=m_aiActorCount[0];i;)
		{
			--i;

			cACTOR *lpActor		=	getActor(m_awActorList[0][i]);

			if	(!lpActor	||	!lpActor->isPlayer())
				continue;

			CM.SEND_PACKET(lpActor->m_iClientSerial,&packet,packet.base.wSize);
		}
	}
}

//	낱말 퀴즈 결과 처리
void
cFIELD::operateWordQuizResult(int _iLoserCount,int _iSurvivorCount)
{
//	점수 정렬
	sortWordQuizPlayerScore();

	int	iTop10PlayerCount		=	min(10,m_wWordQuizSortActorCount);
	int	iTop10PlayerNameLength	=	0;
	int	iScorerCount			=	0;
	
	m_wordQuizStatus.m_wStatus	=	eWQPS_WAIT_QUESTION;

	SG_WORD_QUIZ_RESULT	packet;

	packet.base.set(sizeof(packet),dSG_WORD_QUIZ_RESULT);

	{
		for(int	i=0;i<iTop10PlayerCount;i++)
		{
			int		iIndex	=	m_pSortWordQuizActor[i].m_iIndex;
			cACTOR	*lpActor=	getActor(iIndex);
			int		iNameLen=	strlen(lpActor->m_strName)+1;

			packet.awTop10Score[i]	=	m_pWordActorInfo[iIndex].m_wScore;

			packet.strTop10List[iTop10PlayerNameLength]	=	iNameLen;
			strcpy(packet.strTop10List+iTop10PlayerNameLength+1,lpActor->m_strName);

			iTop10PlayerNameLength	+=	(iNameLen+1);
		}
	}

	for (int i=0;i<m_aiActorCount[0];i++)
	{
		int		iActor		=	m_awActorList[0][i];
		cACTOR	*lpActor	=	getActor(iActor);

		if	(!lpActor	||	!lpActor->isPlayer()	||	m_pWordActorInfo[iActor].m_bf1IsActive	==	FALSE	||	m_pWordActorInfo[iActor].m_bf1IsLoser)
			continue;

		if	(g_bIsTestServer)
		{
			if	(lpActor->m_wOperatorLevel	>=	3)
				continue;
		}
		else
		{
			if	(lpActor->m_wOperatorLevel)
				continue;
		}

		if	(m_pWordActorInfo[iActor].m_wScore)
			iScorerCount++;
	}

//	결과 보내기
	strcpy(packet.strSolution,g_eventManager.getWordQuiz()->m_strSolution);

	packet.wLoserCount		=	_iLoserCount;
	packet.wSurvivorCount	=	iScorerCount;
	packet.wTop10Count		=	iTop10PlayerCount;
	packet.base.wSize		=	sizeof(packet)-sizeof(packet.strTop10List)+iTop10PlayerNameLength;

	addSendPacket((ALL_MSG *) &packet,0xffff);
	sendPacketToAllOperator((ALL_MSG *)&packet);	//	운영자에게 우선적으로 전송한다.
}

void
cFIELD::sortWordQuizPlayerScore()
{
//	점수 정렬
	m_wWordQuizSortActorCount	=	0;

	for (int i=0;i<m_aiActorCount[0];i++)
	{
		int		iActor		=	m_awActorList[0][i];
		cACTOR	*lpActor	=	getActor(iActor);

		if	(!lpActor	||	!lpActor->isPlayer()	||	m_pWordActorInfo[iActor].m_bf1IsActive	==	FALSE	||	m_pWordActorInfo[iActor].m_bf1IsLoser)
			continue;

		if	(g_bIsTestServer)
		{
			if	(lpActor->m_wOperatorLevel	>=	3)
				continue;
		}
		else
		{
			if	(lpActor->m_wOperatorLevel)
				continue;
		}

		if	(m_pWordActorInfo[iActor].m_wScore	==	0)
			continue;

		m_pSortWordQuizActor[m_wWordQuizSortActorCount].m_iIndex	=	iActor;
		m_pSortWordQuizActor[m_wWordQuizSortActorCount].m_iValue	=	m_pWordActorInfo[iActor].m_wScore;

		m_wWordQuizSortActorCount++;
	}

	qsort((void *)m_pSortWordQuizActor,m_wWordQuizSortActorCount,sizeof(cIndexForQSort),(int(*)(const void *,const void *))DesCompare);
}

//
//	낱말 퀴즈 결과 처리
void
cFIELD::changeWordQuizRound(BOOL _bIsFinishEvent)
{
	int		aiRemainActorCount[]	=	{100,50,30,10};

//	탈락자 리스트 처리
	int		iCurrentSurvivorCount	=	0;	//	이번에 살아남은 애들 수
	int		iCurrentLoserCount		=	0;	//	이번에 탈락한 애들 수
	int		iTotalLoserCount		=	0;	//	총 탈락자 수
	int		iRemainActorCount;
	int		iTop10PlayerCount		=	10;
	int		iTop10PlayerNameLength	=	0;

	if	(m_wWordQuizRoundForRemainActor	>=	4)
		iRemainActorCount	=	aiRemainActorCount[3];
	else
		iRemainActorCount	=	aiRemainActorCount[m_wWordQuizRoundForRemainActor];

//	점수 정렬
	{
		sortWordQuizPlayerScore();

		int i;

		for (i=0;i<m_aiActorCount[0];i++)
		{
			int		iActor		=	m_awActorList[0][i];
			cACTOR	*lpActor	=	getActor(iActor);

			if	(!lpActor	||	!lpActor->isPlayer()	||	m_pWordActorInfo[iActor].m_bf1IsActive	==	FALSE	||	m_pWordActorInfo[iActor].m_bf1IsLoser)
				continue;

			if	(g_bIsTestServer)
			{
				if	(lpActor->m_wOperatorLevel	>=	3)
					continue;
			}
			else
			{
				if	(lpActor->m_wOperatorLevel)
					continue;
			}

			m_pWordActorInfo[iActor].m_wScore	=	0;
			m_pWordActorInfo[iActor].m_bf1IsLoser=	TRUE;
		}
		
		while(m_wWordQuizSortActorCount	<	iRemainActorCount)
		{
			++m_wWordQuizRoundForRemainActor;
			
			if	(m_wWordQuizRoundForRemainActor	>=	4)
				iRemainActorCount	=	aiRemainActorCount[3];
			else
				iRemainActorCount	=	aiRemainActorCount[m_wWordQuizRoundForRemainActor];

			if	(m_wWordQuizRoundForRemainActor	>=	3)
				break;
		}

		iRemainActorCount	=	min(m_wWordQuizSortActorCount,iRemainActorCount);

		for	(i=0;i<iRemainActorCount;i++)
		{
			int	iIndex	=	m_pSortWordQuizActor[i].m_iIndex;

			m_pWordActorInfo[iIndex].m_bf1IsLoser	=	FALSE;
		}

		for	(i=iRemainActorCount;i<m_wWordQuizSortActorCount;i++)
		{
			if	(m_pSortWordQuizActor[i].m_iValue	==	m_pSortWordQuizActor[i-1].m_iValue)
			{
				int	iIndex	=	m_pSortWordQuizActor[i].m_iIndex;

				m_pWordActorInfo[iIndex].m_bf1IsLoser	=	FALSE;
			}
		}
	}

	m_wWordQuizRoundForRemainActor++;
	m_wWordQuizRound++;

	{
		SG_QUIZ_EVENT_LOSER_LIST	packetLoser;

		packetLoser.base.set(sizeof(packetLoser),dSG_QUIZ_EVENT_LOSER_LIST);
		packetLoser.bf8LoserCount	=	0;
		packetLoser.bf4QuizType		=	eET_WORD_QUIZ;
		packetLoser.bf1IsFinishEvent=	_bIsFinishEvent;
		packetLoser.wRound			=	m_wWordQuizRound;

		char	astrLoserList[100][dNAME_LENGTH];
		int		iLoserCountForLog	=	0;
		int		iScorerCount		=	0;

		CLOG("Word Quiz","-------- Loser List -----------------------------------------------");

		for (int i=m_aiActorCount[0];i;)
		{
			--i;

			int		iActor		=	m_awActorList[0][i];
			cACTOR	*lpPlayer	=	m_alpActor[iActor];

			if	(!lpPlayer	||	!lpPlayer->isPlayer())
				continue;

			if	(g_bIsTestServer)
			{
				if	(lpPlayer->m_wOperatorLevel	>=	3)
					continue;
			}
			else
			{
				if	(lpPlayer->m_wOperatorLevel)
					continue;
			}

			if	(m_pWordActorInfo[iActor].m_bf1IsLoser	==	FALSE)
			{
				iCurrentSurvivorCount++;
				continue;
			}

			if	(m_pWordActorInfo[iActor].m_bf1IsActive	==	FALSE)
			{
				iTotalLoserCount++;
				continue;
			}

			m_pWordActorInfo[iActor].m_bf1IsActive		=	FALSE;

			packetLoser.awLoserList[packetLoser.bf8LoserCount++]	=	iActor;

			CLOG("Word Quiz","    [%s/%s]",lpPlayer->m_strId,lpPlayer->m_strName);
			iCurrentLoserCount++;
			iTotalLoserCount++;

			if	(packetLoser.bf8LoserCount	>=	dQUIZ_EVENT_LOSER_SLOT_SIZE)
			{
				sendPacketToAllOperator((ALL_MSG *)&packetLoser);	//	운영자에게 우선적으로 전송한다.
				addSendPacket((ALL_MSG *)&packetLoser,0xffff);

				packetLoser.bf8LoserCount	=	0;
			}

			strcpy(astrLoserList[iLoserCountForLog++],lpPlayer->m_strName);

			if	(iLoserCountForLog >= 100)
			{
				g_game.sendQuizLoserListLog((char *)astrLoserList,iLoserCountForLog,g_eventManager.getWordQuiz()->m_wQuestionCounter,eEW_WORDQUIZ_LOSER_LIST);

				iLoserCountForLog	=	0;
			}
		}

		if	(iCurrentLoserCount)
			CLOG("Word Quiz","총 %d명의 탈락자가 있다.",iCurrentLoserCount);
		else
			CLOG("Word Quiz","탈락자가 없다.");

		CLOG("Word Quiz","--------------------------------------------------------------------");
		CLOG("Word Quiz"," ");

		if	(iLoserCountForLog)
			g_game.sendQuizLoserListLog((char *)astrLoserList,iLoserCountForLog,g_eventManager.getWordQuiz()->m_wQuestionCounter,eEW_WORDQUIZ_LOSER_LIST);

		{
			packetLoser.base.wSize	=	sizeof(packetLoser)-sizeof(packetLoser.awLoserList)+packetLoser.bf8LoserCount*sizeof(WORD);

			sendPacketToAllOperator((ALL_MSG *)&packetLoser);	//	운영자에게 우선적으로 전송한다.
			addSendPacket((ALL_MSG *)&packetLoser,0xffff);
		}

		{
			SG_ETC_WORK	packetInfo;

			packetInfo.base.set(sizeof(SG_ETC_WORK),dSG_ETC_WORK);

			packetInfo.wWork			=	eEW_WORDQUIZ_SURVIVOR_COUNT;
			packetInfo.aiValue[0]		=	m_wordQuizStatus.m_wComerCount;
			packetInfo.aiValue[1]		=	iCurrentSurvivorCount;
			packetInfo.aiValue[2]		=	0;

			sendPacketToAllOperator((ALL_MSG *)&packetInfo);	//	운영자에게 우선적으로 전송한다.
		}
	}

	m_wordQuizStatus.m_wStatus	=	eWQPS_WAIT_QUESTION;

//	결과 보내기
}


//
//	낱말 퀴즈 승자 리스트 뽑기
void
cFIELD::checkWordQuizWinner()
{
	changeWordQuizRound(TRUE);

	char	strWinners[100*dNAME_LENGTH];
	int		iWinnerCount = 0;

	CLOG("Word Quiz","-------- 승자 리스트 -------------------------------------------------");

	CLOG("Word Quiz","▶▶ %4d년 %2d월 %2d일 %2d시 %2d분 %2d초 ◀◀",g_currentTime.m_wYear,g_currentTime.m_wMonth,g_currentTime.m_wDay,g_currentTime.m_wHour,g_currentTime.m_wMinute,g_currentTime.m_wSecond);

	for (int i=0;i<m_wWordQuizSortActorCount;i++)
	{
		int		iActor		=	m_pSortWordQuizActor[i].m_iIndex;
		cACTOR	*lpPlayer	=	m_alpActor[iActor];

		if	(!lpPlayer	||	!lpPlayer->isPlayer()	||	lpPlayer->m_wSerialInField	==	0xffff)
			continue;

		if	(g_bIsTestServer)
		{
			if	(lpPlayer->m_wOperatorLevel	>=	3)
				continue;
		}
		else
		{
			if	(lpPlayer->m_wOperatorLevel)
				continue;
		}

		if	(m_pWordActorInfo[iActor].m_bf1IsLoser)
			continue;

		CLOG("Word Quiz","    [%s/%s]",lpPlayer->m_strId,lpPlayer->m_strName);

		strcpy(strWinners+iWinnerCount*dNAME_LENGTH,lpPlayer->m_strName);
		iWinnerCount++;

		if	(iWinnerCount	>=	100)
			break;
	}

	m_wordQuizStatus.m_wWinnerCount	+=	iWinnerCount;

	if	(iWinnerCount)
		CLOG("Word Quiz","총 %d명의 낱말 퀴즈 승자 있음.",iWinnerCount);
	else
		CLOG("Word Quiz","승자 없음(승자는 커녕 말자도 없음!!)");

	CLOG("Word Quiz","----------------------------------------------------------------------");
	CLOG("Word Quiz"," ");

	g_game.sendQuizEventWinnerLog(eEW_WORDQUIZ_WINNER,strWinners,iWinnerCount,0);

	m_wordQuizStatus.m_wIsSaveResult	=	TRUE;
}


//
//	낱말 퀴즈 종료
void
cFIELD::finishWordQuiz()
{
	if	(m_wordQuizStatus.m_wStatus	==	eWQPS_FINISH	||	m_wordQuizStatus.m_wStatus	==	eWQPS_SLEEP)
		return;

	checkWordQuizWinner();

	m_wordQuizStatus.m_wStatus	=	eWQPS_FINISH;

	SG_END_QUIZ_EVENT	packet;

	packet.base.set(sizeof(packet),dSG_END_QUIZ_EVENT);

	packet.wComerCount	=	m_wordQuizStatus.m_wComerCount;
	packet.wWinnerCount	=	m_wordQuizStatus.m_wWinnerCount;
	packet.wType		=	eET_WORD_QUIZ;

	addSendPacket((ALL_MSG *) &packet,0xffff);
}

//
//	낱말 퀴즈 종료
void
cFIELD::endWordQuiz()
{
	m_wordQuizStatus.m_wStatus	=	eWQPS_SLEEP;

	for (int iPlayer=m_aiActorCount[0];iPlayer;)
	{
		--iPlayer;

		cACTOR	*lpPlayer	=	m_alpActor[m_awActorList[0][iPlayer]];

		if	(!lpPlayer	||	!lpPlayer->isPlayer())
			continue;

		g_pMoveFieldUserManager->booking(lpPlayer,NULL,lpPlayer->m_wLastVillage,0xffff);
	}
}

void
cFIELD::operateWordQuizAnswer(cACTOR *_lpActor,char *_lpstrAnswer)
{
	if	(STRICMP(g_eventManager.getWordQuiz()->m_strSolution,_lpstrAnswer)	==	0)
	{
		int	iScore	=	g_eventManager.getWordQuiz()->getCurrentScore();

		m_pWordActorInfo[_lpActor->m_wSerialInField].addScore(iScore);
		g_eventManager.getWordQuiz()->increaseCorrectAnswerCount();

		_lpActor->sendEtcWork(eEW_ANSWER_RESULT_TO_WORD_QUIZ,iScore);
	}
	else
		_lpActor->sendEtcWork(eEW_ANSWER_RESULT_TO_WORD_QUIZ,0);
}