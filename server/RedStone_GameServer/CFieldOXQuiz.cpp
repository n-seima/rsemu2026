#include "cFIELD.H"
#include "cGAME.H"
#include "debugCode.H"
#include "CGuild.H"
#include "CEvent.H"
#include "booking_work.H"

//
//	OX 퀴즈 준비
void
cFIELD::readyOXQuiz()
{
	m_oxQuizStatus.m_wStatus				=	eOIPS_READY;
	m_oxQuizStatus.m_wReviveLoserBattleCount=	0;
	m_oxQuizStatus.m_wWinnerCount			=	0;
	m_oxQuizStatus.m_wComerCount			=	0;
	m_oxQuizStatus.m_wIsSaveResult			=	FALSE;
}

//
//	OX 퀴즈 시작
void
cFIELD::beginOXQuiz()
{
	if (m_oxQuizStatus.m_wStatus!=	eOIPS_READY)
		return;

	m_oxQuizStatus.m_wStatus				=	eOIPS_WAIT_QUESTION;
	m_oxQuizStatus.m_wReviveLoserBattleCount=	0;
	m_oxQuizStatus.m_wWinnerCount			=	0;
	m_oxQuizStatus.m_wComerCount			=	0;
	m_oxQuizStatus.m_wIsSaveResult			=	FALSE;

	SG_BEGIN_QUIZ_EVENT	packet;

	packet.base.set(sizeof(packet),dSG_BEGIN_QUIZ_EVENT);
	packet.wType	=	eET_OX_QUIZ;

	addSendPacket((ALL_MSG *) &packet,0xffff);
	sendPacketToAllOperator((ALL_MSG *) &packet);

	char	astrList[100][dNAME_LENGTH];
	int		iCount	=	0;

	CLOG("OXQuiz","-------- 참가자 리스트 ------------------------------------------------");
	CLOG("OXQuiz","▶▶ %4d년 %2d월 %2d일 %2d시 %2d분 %2d초 ◀◀",g_currentTime.m_wYear,g_currentTime.m_wMonth,g_currentTime.m_wDay,g_currentTime.m_wHour,g_currentTime.m_wMinute,g_currentTime.m_wSecond);

	for (int iPlayer=m_aiActorCount[0];iPlayer;iPlayer--)
	{
		cACTOR	*lpPlayer	=	m_alpActor[m_awActorList[0][iPlayer-1]];

		if(lpPlayer->isOperator())
			continue;

		strcpy(astrList[iCount++],lpPlayer->m_strName);
		CLOG("OXQuiz","    %s/%s",lpPlayer->m_strId,lpPlayer->m_strName);

		m_oxQuizStatus.m_wComerCount++;

		if	(iCount >= 100)
		{
			g_game.sendQuizComerListLog((char *)astrList,iCount,eEW_OXQUIZ_COMER_LIST);

			iCount	=	0;
		}
	}

	CLOG("OXQuiz","총 %d명의 참가자.",m_oxQuizStatus.m_wComerCount);
	CLOG("OXQuiz","-----------------------------------------------------------------------");
	CLOG("OXQuiz"," ");

	if	(iCount)
		g_game.sendQuizComerListLog((char *)astrList,iCount,eEW_OXQUIZ_COMER_LIST);

	{
		SG_OX_QUIZ_SURVIVOR_COUNT	packetInfo;

		packetInfo.base.set(sizeof(SG_OX_QUIZ_SURVIVOR_COUNT),dSG_OX_QUIZ_SURVIVOR_COUNT);

		packetInfo.wLoserCount		=	0;
		packetInfo.wSurvivorCount	=	m_oxQuizStatus.m_wComerCount;

		sendPacketToAllOperator((ALL_MSG *)&packetInfo);	//	운영자에게 우선적으로 전송한다.
	}
}

//
//	OX 퀴즈 문제 제공 받았다.
void
cFIELD::producedOxQuizQuestion()
{
	if	(m_oxQuizStatus.m_wStatus	!=	eOIPS_WAIT_QUESTION)
		return;

	g_game.sendOXQuizQuestionLog(g_eventManager.getOXQuizQuestion(),g_eventManager.getOXQuizSolution(),g_eventManager.getOXQuizQuestionCounter());

	char	*astrOXQuizResult[]	=	{"O","X"};

	CLOG("OXQuiz","-------- 문제 %.2d-------------------------------------------------------",g_eventManager.getOXQuizQuestionCounter());
	CLOG("OXQuiz","    질문 : %s",g_eventManager.getOXQuizQuestion());
	CLOG("OXQuiz","    답   : %s",astrOXQuizResult[g_eventManager.getOXQuizSolution()]);
	CLOG("OXQuiz","-----------------------------------------------------------------------");
	CLOG("OXQuiz"," ");

	BOOL	bIsForcedSend	=	FALSE;

	SG_QUIZ_EVENT_QUESTION		packet;

	packet.base.set(sizeof(packet),dSG_QUIZ_EVENT_QUESTION);

	strcpy(packet.strQuestion,g_eventManager.getOXQuizQuestion());

	packet.wWaitCount		=	g_eventManager.getOXQuizCount()-dOX_QUIZ_SPARE_COUNT;
	packet.bf10QuestionCounter=	g_eventManager.getOXQuizQuestionCounter();
	packet.base.wSize		=	sizeof(packet)-sizeof(packet.strQuestion) + strlen(packet.strQuestion)+1;
	m_oxQuizStatus.m_wStatus=	eOIPS_COUNTDOWN;
	packet.bf4QuizType		=	eET_OX_QUIZ;

	if	(bIsForcedSend	==	FALSE)
		addSendPacket((ALL_MSG *) &packet,0xffff);
	else
	{
		for (int i=m_aiActorCount[0];i;)
		{
			i--;

			cACTOR *lpActor		=	getActor(m_awActorList[0][i]);

			if	(!lpActor	||	!lpActor->isPlayer())
				continue;

			CM.SEND_PACKET(lpActor->m_iClientSerial,&packet,packet.base.wSize);
		}
	}
}

//
//	OX 퀴즈 결과 처리
void
cFIELD::operateOXQuizResult()
{
//	탈락자 리스트 처리
	int	iCurrentSurvivorCount	=	0;	//	이번에 살아남은 애들 수
	int	iCurrentLoserCount		=	0;	//	이번에 탈락한 애들 수
	int	iTotalLoserCount		=	0;	//	총 탈락자 수

	{
		cAreaInfo	*lpOArea		=	m_pArea->getOXQuizOArea();
		cAreaInfo	*lpXArea		=	m_pArea->getOXQuizXArea();
		cAreaInfo	*lpSolutionArea;

		int	iSolution	=	g_eventManager.getOXQuizSolution();

		if (iSolution	==	0)
			lpSolutionArea	=	lpOArea;
		else
			lpSolutionArea	=	lpXArea;

		SG_QUIZ_EVENT_LOSER_LIST	packetLoser;

		packetLoser.base.set(sizeof(packetLoser),dSG_QUIZ_EVENT_LOSER_LIST);
		packetLoser.bf8LoserCount	=	0;
		packetLoser.bf4QuizType		=	eET_OX_QUIZ;

		char	astrLoserList[100][dNAME_LENGTH];
		int		iLoserCountForLog	=	0;

		CLOG("OXQuiz","-------- 탈락자 리스트 -----------------------------------------------");


		for (int i=m_aiActorCount[0];i;i--)
		{
			cACTOR	*lpPlayer	=	m_alpActor[m_awActorList[0][i-1]];

			if (lpPlayer->isOperator())	
				continue;

			if (lpPlayer->m_bf1IsOXQuizWinner)
				continue;

			if (lpPlayer->m_bf1IsOXQuizLoser)
			{
				if (lpPlayer->m_bf1IsOXQuizWinner == FALSE)
					iTotalLoserCount++;

				continue;
			}

			if (lpSolutionArea->isIn(lpPlayer->m_pos.x,lpPlayer->m_pos.y))	//	답 맞췄다~
			{
				iCurrentSurvivorCount	++;

				continue;
			}

			packetLoser.awLoserList[packetLoser.bf8LoserCount++]	=	m_awActorList[0][i-1];

			lpPlayer->m_bf1IsOXQuizLoser	=	TRUE;

			CLOG("OXQuiz","    %s/%s",lpPlayer->m_strId,lpPlayer->m_strName);
			iCurrentLoserCount++;
			iTotalLoserCount++;

			if (packetLoser.bf8LoserCount	>=	dQUIZ_EVENT_LOSER_SLOT_SIZE)
			{
				sendPacketToAllOperator((ALL_MSG *)&packetLoser,1);	//	운영자에게 우선적으로 전송한다.
				addSendPacket((ALL_MSG *)&packetLoser,0xffff);

				packetLoser.bf8LoserCount	=	0;
			}

			strcpy(astrLoserList[iLoserCountForLog++],lpPlayer->m_strName);

			if (iLoserCountForLog >= 100)
			{
				g_game.sendQuizLoserListLog((char *)astrLoserList,iLoserCountForLog,g_eventManager.getOXQuizQuestionCounter(),eEW_OXQUIZ_LOSER_LIST);

				iLoserCountForLog	=	0;
			}
		}

		if (iCurrentLoserCount)
			CLOG("OXQuiz","총 %d명의 탈락자가 있다.",iCurrentLoserCount);
		else
			CLOG("OXQuiz","탈락자가 없다.");

		CLOG("OXQuiz","--------------------------------------------------------------------");
		CLOG("OXQuiz"," ");

		if (iLoserCountForLog)
			g_game.sendQuizLoserListLog((char *)astrLoserList,iLoserCountForLog,g_eventManager.getOXQuizQuestionCounter(),eEW_OXQUIZ_LOSER_LIST);

		if (packetLoser.bf8LoserCount)
		{
			packetLoser.base.wSize	=	sizeof(packetLoser)-sizeof(packetLoser.awLoserList)+packetLoser.bf8LoserCount*sizeof(WORD);

			sendPacketToAllOperator((ALL_MSG *)&packetLoser);	//	운영자에게 우선적으로 전송한다.
			addSendPacket((ALL_MSG *)&packetLoser,0xffff);
		}

		{
			SG_OX_QUIZ_SURVIVOR_COUNT	packetInfo;

			packetInfo.base.set(sizeof(SG_OX_QUIZ_SURVIVOR_COUNT),dSG_OX_QUIZ_SURVIVOR_COUNT);

			packetInfo.wLoserCount		=	iTotalLoserCount;
			packetInfo.wSurvivorCount	=	iCurrentSurvivorCount;

			sendPacketToAllOperator((ALL_MSG *)&packetInfo);	//	운영자에게 우선적으로 전송한다.
		}
	}

	m_oxQuizStatus.m_wStatus	=	eOIPS_WAIT_QUESTION;

//	결과 보내기
	{
		SG_OX_QUIZ_RESULT	packet;

		packet.base.set(sizeof(packet),dSG_OX_QUIZ_RESULT);

		packet.wSolution		=	g_eventManager.getOXQuizSolution();
		packet.wLoserCount		=	iCurrentLoserCount;
		packet.wSurvivorCount	=	iCurrentSurvivorCount;

		addSendPacket((ALL_MSG *) &packet,0xffff);
		sendPacketToAllOperator((ALL_MSG *)&packet);	//	운영자에게 우선적으로 전송한다.
	}
}

//
//	OX 승자 리스트 뽑기
void
cFIELD::checkOXQuizWinner()
{
	char	strWinners[100*dNAME_LENGTH];
	int		iWinnerCount = 0,iTotalWinnerCount = 0;

	if (m_oxQuizStatus.m_wReviveLoserBattleCount)
		CLOG("OXQuiz","-------- 패자 부활전 승자 리스트 -------------------------------------");
	else
		CLOG("OXQuiz","-------- 승자 리스트 -------------------------------------------------");

	CLOG("OXQuiz","▶▶ %4d년 %2d월 %2d일 %2d시 %2d분 %2d초 ◀◀",g_currentTime.m_wYear,g_currentTime.m_wMonth,g_currentTime.m_wDay,g_currentTime.m_wHour,g_currentTime.m_wMinute,g_currentTime.m_wSecond);

	for (int i=m_aiActorCount[0];i;i--)
	{
		cACTOR	*lpPlayer	=	m_alpActor[m_awActorList[0][i-1]];

		if (lpPlayer->isOperator())	
			continue;

		if (lpPlayer->m_bf1IsOXQuizLoser)
			continue;

		CLOG("OXQuiz","    %s/%s",lpPlayer->m_strId,lpPlayer->m_strName);

		strcpy(strWinners+iWinnerCount*dNAME_LENGTH,lpPlayer->m_strName);
		iWinnerCount++;
		iTotalWinnerCount++;

		if	(iWinnerCount >= 100)
		{
			g_game.sendQuizEventWinnerLog(eEW_OXQUIZ_WINNER,strWinners,iWinnerCount,m_oxQuizStatus.m_wReviveLoserBattleCount);

			iWinnerCount	=	0;
		}
	}

	m_oxQuizStatus.m_wWinnerCount	+=	iTotalWinnerCount;

	if	(iTotalWinnerCount)
		CLOG("OXQuiz","총 %d명의 OX 퀴즈 승자 있음.",iTotalWinnerCount);
	else
		CLOG("OXQuiz","승자 없음(승자는 커녕 말자도 없음!!)");

	CLOG("OXQuiz","----------------------------------------------------------------------");
	CLOG("OXQuiz"," ");

	if	(iTotalWinnerCount==0 || iWinnerCount)
		g_game.sendQuizEventWinnerLog(eEW_OXQUIZ_WINNER,strWinners,iWinnerCount,m_oxQuizStatus.m_wReviveLoserBattleCount);

	m_oxQuizStatus.m_wIsSaveResult	=	TRUE;
}


//
//	패자 부활전
void
cFIELD::launchOXQuizLoserReviveBattle()
{
	checkOXQuizWinner();

	m_oxQuizStatus.m_wIsSaveResult	=	FALSE;

	m_oxQuizStatus.m_wReviveLoserBattleCount++;

	int	iRevivorCount	=	0;
	

	for (int iPlayer=m_aiActorCount[0];iPlayer;iPlayer--)
	{
		cACTOR	*lpPlayer	=	m_alpActor[m_awActorList[0][iPlayer-1]];

		if (lpPlayer->isOperator())
			continue;

		if (lpPlayer->m_bf1IsOXQuizLoser == FALSE)	//	승자
		{
			lpPlayer->m_bf1IsOXQuizWinner=	TRUE;
			lpPlayer->m_bf1IsOXQuizLoser =	TRUE;

			continue;
		}

		lpPlayer->m_bf1IsOXQuizLoser =	FALSE;	//	부활!!
		iRevivorCount++;
	}

	{
		SG_OX_QUIZ_LOSER_REVIVE_BATTLE	packet;

		packet.base.set(sizeof(packet),dSG_OX_QUIZ_LOSER_REVIVE_BATTLE);
		packet.wBattleCount	=	m_oxQuizStatus.m_wReviveLoserBattleCount;
		packet.wWinnerCount	=	m_oxQuizStatus.m_wWinnerCount;
		packet.wRevivorCount=	iRevivorCount;

		sendPacketToAllOperator((ALL_MSG *)&packet);	//	영자 한테 우선적으로 전달.
		addSendPacket((ALL_MSG *) &packet,0xffff);
	}

//	OX 퀴즈 승자 리스트 전송
	{
		SG_OX_QUIZ_WINNER_LIST			packet;

		packet.base.set(sizeof(packet),dSG_OX_QUIZ_WINNER_LIST);
		packet.wWinnerCount	=	0;

		for (int iPlayer=m_aiActorCount[0];iPlayer;iPlayer--)
		{
			cACTOR	*lpPlayer	=	m_alpActor[m_awActorList[0][iPlayer-1]];

			if (lpPlayer->isOperator())
				continue;

			if (lpPlayer->m_bf1IsOXQuizWinner	== FALSE)
				continue;

			packet.awWinnerList[packet.wWinnerCount++]	=	m_awActorList[0][iPlayer-1];

			if (packet.wWinnerCount >= 100)
			{
				addSendPacket((ALL_MSG *) &packet,0xffff);
				sendPacketToAllOperator((ALL_MSG *)&packet);	//	영자 한테 우선적으로 전달.

				packet.wWinnerCount	=	0;
			}
		}

		{
			packet.base.wSize	=	sizeof(packet)-sizeof(packet.awWinnerList)+packet.wWinnerCount*sizeof(WORD);

			sendPacketToAllOperator((ALL_MSG *)&packet);	//	영자 한테 우선적으로 전달.
			addSendPacket((ALL_MSG *) &packet,0xffff);
		}
	}
	
}

//
//	OX 퀴즈 종료
void
cFIELD::finishOxQuiz()
{
	if (m_oxQuizStatus.m_wStatus == eOIPS_WAIT_QUESTION)
	{
		checkOXQuizWinner();

		for (int i=m_aiActorCount[0];i;i--)
		{
			cACTOR	*lpPlayer	=	m_alpActor[m_awActorList[0][i-1]];

			lpPlayer->removeOXQuizTicket();
		}
	}

	m_oxQuizStatus.m_wStatus	=	eOIPS_FINISH;

	SG_END_QUIZ_EVENT	packet;

	packet.base.set(sizeof(packet),dSG_END_QUIZ_EVENT);

	packet.wComerCount	=	m_oxQuizStatus.m_wComerCount;
	packet.wWinnerCount	=	m_oxQuizStatus.m_wWinnerCount;
	packet.wType		=	eET_OX_QUIZ;

	addSendPacket((ALL_MSG *) &packet,0xffff);
}

//
//	OX 퀴즈 종료
void
cFIELD::endOXQuiz()
{
	m_oxQuizStatus.m_wStatus	=	eOIPS_SLEEP;

	for (int iPlayer=m_aiActorCount[0];iPlayer;iPlayer--)
	{
		cACTOR	*lpPlayer	=	m_alpActor[m_awActorList[0][iPlayer-1]];

		lpPlayer->m_bf1IsMoveByEndOfOXQuiz	=	TRUE;

		g_pMoveFieldUserManager->booking(lpPlayer,NULL,lpPlayer->m_wLastVillage,0xffff);
	}
}