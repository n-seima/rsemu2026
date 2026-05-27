#include "CAgent.H"
#include "CActor.H"
#include "CGamePlay.H"
#include "CSkill.H"
#include "cMESSAGE.H"
#include "CTitle.H"
#include "CWindowInterface.H"
#include "CGuild.H"
#include "CEvent.H"
#include "CSound.H"
#include "banner.H"

//
//	채팅
void
cPACKET_HANDLER::Chat(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::Chat(&_lpPacket->mSG_CHAT);
}

void
cPACKET_HANDLER::ReceiveEventNotice(SERVER_PACKETS *_lpPacket)
{
	SG_EVENT_NOTICE	*lpPacket	=	&_lpPacket->mSG_EVENT_NOTICE;

	CGamePlay::EventNotice(lpPacket->strMessage,lpPacket->wNoticeType,FALSE,0xffffffff,lpPacket->wTime , lpPacket->wEffectflickeringly);	
}

void
cPACKET_HANDLER::ReceiveNotice(SERVER_PACKETS *_lpPacket)
{
	SG_NOTICE	*lpPacket	=	(SG_NOTICE	*)_lpPacket;

	CGamePlay::Notice(lpPacket->strMessage);	
}

void
cPACKET_HANDLER::ReceiveGetTitle(SERVER_PACKETS *_lpPacket)
{
	SG_GET_TITLE		*lpPacket	=	&_lpPacket->mSG_GET_TITLE;
	CTitle				*lpTitle	=	g_titleManager.getTitle(lpPacket->bTitle);

	if (!lpTitle)	return;

	g_hero.m_aTitle[lpPacket->wSlot].m_bTitle	=	lpPacket->bTitle;
	g_hero.m_aTitle[lpPacket->wSlot].m_bLevel	=	lpPacket->bLevel;
	g_hero.buildPower();

	CGamePlay::AddSystemMessage(WHITE,dMSG_GET_TITLE_FORM,lpPacket->bLevel,lpTitle->m_strName);
}

void
cPACKET_HANDLER::ReceiveRemoveTitle(SERVER_PACKETS *_lpPacket)
{
	SG_REMOVE_TITLE		*lpPacket	=	&_lpPacket->mSG_REMOVE_TITLE;
	CTitle				*lpTitle	=	g_titleManager.getTitle(lpPacket->bTitle);

	if	(!lpTitle)
		return;

	CGamePlay::AddSystemMessage(WHITE,dMSG_REMOVED_TITLE_FORM,lpPacket->bLevel,lpTitle->m_strName);

	g_hero.m_aTitle[lpPacket->wSlot].m_bTitle	=	0xff;
	g_hero.m_aTitle[lpPacket->wSlot].m_bLevel	=	0;
}

//
//퀘스트 취소
void
cPACKET_HANDLER::ReceiveCancelQuest(SERVER_PACKETS *_lpPacket)
{
	SG_CANCEL_QUEST		*lpPacket	=	&_lpPacket->mSG_CANCEL_QUEST;

	switch(lpPacket->wResult)
	{
		case	eCQR_SUCCESS	:	//	퀘스트 취소 성공
		{
			CQuestItem	*lpQuest	=	g_hero.getQuest(lpPacket->wQuestSlot);

			if (!lpQuest)	return;

			CGamePlay::AddSystemMessage(WHITE,dMSG_CANCEL_QUEST_FORM,lpQuest->m_strName);
			g_hero.cancelQuest(lpPacket->wQuestSlot);
			CGamePlay::UpdateMinimapQuestList();
			g_map.FindQuestLinkField();
			break;
		}

		case	eCQR_FAILED		:	//	퀘스트 취소 실패
			CGamePlay::AddSystemMessage(WHITE,dMSG_CANCEL_QUEST_FAILED);
			break;
	}
}	//	cPACKET_HANDLER::ReceiveCancelQuest(SERVER_PACKETS *_lpPacket)

void
cPACKET_HANDLER::ReceiveAnnounceQuizEvent(SERVER_PACKETS *_lpPacket)
{
	SG_ANNOUNCE_QUIZ_EVENT		*lpPacket	=	&_lpPacket->mSG_ANNOUNCE_QUIZ_EVENT;

	char	*lpstrFieldName	=	cMAP::GetFieldName(lpPacket->wQuizGateField);

	if	(!lpstrFieldName)
		return;

	char	*lpstrTimeStr	=	cGAME_UTIL::GetTimeStringHM(lpPacket->wHour,lpPacket->wMinute);

	if	(lpPacket->wType	==	eET_OX_QUIZ)
	{
		g_eventOxQuiz.reset();

		if	(lpPacket->wIsTest == FALSE)
			sprintf(g_eventOxQuiz.m_strBeginTimeAnnounce,dMSG_ANNOUNCE_QUIZ_EVENT_FORM,lpstrTimeStr,lpstrFieldName,dMSG_OX_QUIZ,"");
		else
			sprintf(g_eventOxQuiz.m_strBeginTimeAnnounce,dMSG_ANNOUNCE_QUIZ_EVENT_FORM,lpstrTimeStr,lpstrFieldName,dMSG_OX_QUIZ,dMSG_IS_TEST_EVENT);

		CGamePlay::Notice(g_eventOxQuiz.m_strBeginTimeAnnounce);

		if	(s_bIsOXQuizField)
			g_am.resetQuizEventStatus();
	}

	if	(lpPacket->wType	==	eET_WORD_QUIZ)
	{
		g_eventWordQuiz.reset();

		if	(lpPacket->wIsTest == FALSE)
			sprintf(g_eventWordQuiz.m_strBeginTimeAnnounce,dMSG_ANNOUNCE_QUIZ_EVENT_FORM,lpstrTimeStr,lpstrFieldName,lpPacket->strQuizEventName,"");
		else
			sprintf(g_eventWordQuiz.m_strBeginTimeAnnounce,dMSG_ANNOUNCE_QUIZ_EVENT_FORM,lpstrTimeStr,lpstrFieldName,lpPacket->strQuizEventName,dMSG_IS_TEST_EVENT);

		CGamePlay::Notice(g_eventWordQuiz.m_strBeginTimeAnnounce);

		g_eventWordQuiz.m_bIsWaitBegin	=	TRUE;

		if	(s_bIsWordQuizField)
			g_am.resetQuizEventStatus();
	}
}

//
//
void
cPACKET_HANDLER::ReceiveBeginQuizEvent(SERVER_PACKETS *_lpPacket)
{
	SG_BEGIN_QUIZ_EVENT	*lpPacket	=	(SG_BEGIN_QUIZ_EVENT	*)_lpPacket;

	if	(lpPacket->wType	==	eET_OX_QUIZ)
	{
		g_msgBox.cPopup("",dMSG_BEGIN_OX_QUIZ,dMSG_OK);

		g_eventOxQuiz.m_bIsWaitBegin	=	FALSE;
	}

	if	(lpPacket->wType	==	eET_WORD_QUIZ)
	{
		g_msgBox.cPopup("",dMSG_BEGIN_WORD_QUIZ,dMSG_OK);

		g_eventWordQuiz.begin();
	}

	CGamePlay::InitNoticeBox();
}

void
cPACKET_HANDLER::ReceiveOXQuizResult(SERVER_PACKETS *_lpPacket)
{
	SG_OX_QUIZ_RESULT		*lpPacket	=	&_lpPacket->mSG_OX_QUIZ_RESULT;

	char	strText[512];
	char	*strOX[]	=	{"O","X"};

	sprintf(strText,dMSG_OX_QUIZ_RESULT_FORM,strOX[lpPacket->wSolution],g_eventOxQuiz.m_strQuestion,lpPacket->wSurvivorCount,lpPacket->wLoserCount);

	g_msgBox.cPopup("",strText,dMSG_OK);

	g_eventOxQuiz.m_wIsCountDown	=	FALSE;
	g_eventOxQuiz.m_bIsWaitBegin	=	FALSE;

	CGamePlay::InitNoticeBox();
}

//낱말 퀴즈 결과
void
cPACKET_HANDLER::ReceiveWordQuizResult(SERVER_PACKETS *_lpPacket)
{
	SG_WORD_QUIZ_RESULT	*lpPacket	=	(SG_WORD_QUIZ_RESULT	*)_lpPacket;

	g_eventWordQuiz.m_wIsCountDown	=	FALSE;
	g_eventWordQuiz.m_bIsWaitBegin	=	FALSE;
	g_eventWordQuiz.m_wScorerCount	=	lpPacket->wSurvivorCount;

	CGamePlay::InitNoticeBox();
	CGamePlay::WarningMessage(_ms(dMSG_CORRECT_SOLUTION_FORM,lpPacket->strSolution));


	{
		int	iBP	=	0;
		g_eventWordQuiz.m_wTop10Count	=	lpPacket->wTop10Count;

		for	(int i=0;i<lpPacket->wTop10Count;i++)
		{
			int	iLength	=	lpPacket->strTop10List[iBP];

			memcpy(g_eventWordQuiz.m_strTop10List[i],lpPacket->strTop10List+iBP+1,iLength);

			iBP			+=	(iLength+1);

			g_eventWordQuiz.m_awTop10Score[i]	=	lpPacket->awTop10Score[i];
		}
	}
}

void
cPACKET_HANDLER::ReceiveCountDownStart(SERVER_PACKETS *_lpPacket)
{
	SG_START_COUNTDOWN		*lpPacket	=	&_lpPacket->mSG_START_COUNTDOWN;

	g_countDown.begin(lpPacket->wTime,(void *)&s_textBold24M,cDRAW::HEIGHT/12);
}

//OX 퀴즈 질문 받음
void
cPACKET_HANDLER::ReceiveQuizEventQuestion(SERVER_PACKETS *_lpPacket)
{
	SG_QUIZ_EVENT_QUESTION		*lpPacket	=	&_lpPacket->mSG_QUIZ_EVENT_QUESTION;

	if	(lpPacket->bf4QuizType	==	eET_OX_QUIZ)
	{
		sprintf(g_eventOxQuiz.m_strQuestion,dMSG_QUIZ_EVENT_QUESTION_FORM,lpPacket->bf10QuestionCounter,lpPacket->strQuestion);

		g_eventOxQuiz.m_wIsCountDown	=	TRUE;
		g_eventOxQuiz.m_bIsWaitBegin	=	FALSE;

		g_eventCountDown.begin(lpPacket->wWaitCount,(void *)&s_textLarge32,cDRAW::HEIGHT/12);
	}

	if	(lpPacket->bf4QuizType	==	eET_WORD_QUIZ)
	{
		sprintf(g_eventWordQuiz.m_strQuestion,dMSG_QUIZ_EVENT_QUESTION_FORM,lpPacket->bf10QuestionCounter,lpPacket->strQuestion);
		strcat(g_eventWordQuiz.m_strQuestion,dMSG_WORD_QUIZ_SOLUTION_LENGTH);

		g_eventWordQuiz.m_wIsCountDown	=	TRUE;
		g_eventWordQuiz.m_bIsWaitBegin	=	FALSE;

		g_eventCountDown.begin(lpPacket->wWaitCount,(void *)&s_textLarge32,cDRAW::HEIGHT/12);
	}
}

//OX 퀴즈 종료
void
cPACKET_HANDLER::ReceiveEndQuizEvent(SERVER_PACKETS *_lpPacket)
{
	SG_END_QUIZ_EVENT			*lpPacket	=	&_lpPacket->mSG_END_QUIZ_EVENT;

	char	strText[512];

	if	(lpPacket->wType	==	eET_OX_QUIZ)
	{
		sprintf(strText,dMSG_END_OX_QUIZ_FORM,lpPacket->wComerCount,lpPacket->wWinnerCount);

		g_eventOxQuiz.m_bIsEndEvent		=	TRUE;

		g_msgBox.cPopup("",strText,dMSG_OK);
	}

	if	(lpPacket->wType	==	eET_WORD_QUIZ)
	{
		sprintf(strText,dMSG_END_WORD_QUIZ_FORM,lpPacket->wComerCount,lpPacket->wWinnerCount);

		g_eventWordQuiz.m_bIsEndEvent	=	TRUE;

		g_msgBox.cPopup("",strText,dMSG_OK);
	}
}

void
cPACKET_HANDLER::ReceiveEventTimeUpdate(SERVER_PACKETS *_lpPacket)
{
	SG_EVENT_TIME_UPDATE	*lpPacket	=	(SG_EVENT_TIME_UPDATE *)_lpPacket;

	int		iCount	=	max(CGamePlay::s_eventActiveManager.m_iCount,lpPacket->iCount);		
	BOOL	bIsChangeEventObject = FALSE;
	
	BOOL	bIsChange				= FALSE;
	BOOL	bBeforeChristmasEvent	= CGamePlay::s_eventActiveManager.m_adwEventActiveBuffer[eEVENT_TIME_CHRISTMAS/dEVENT_TIME_BUFFER_BIT] & 1<< eEVENT_TIME_CHRISTMAS;
	BOOL	bNewChristmasEvent		= lpPacket->adwEventTimeActiveBuffer[eEVENT_TIME_CHRISTMAS/dEVENT_TIME_BUFFER_BIT] & 1<< eEVENT_TIME_CHRISTMAS;

	if(bBeforeChristmasEvent != bNewChristmasEvent)
	{	
		CGamePlay::changeEventObject(bNewChristmasEvent);	
	}
	memcpy(CGamePlay::s_eventActiveManager.m_adwEventActiveBuffer ,lpPacket->adwEventTimeActiveBuffer ,sizeof(lpPacket->adwEventTimeActiveBuffer));


}

//OX 퀴즈 패배자 리스트
void
cPACKET_HANDLER::ReceiveQuizEventLoserList(SERVER_PACKETS *_lpPacket)
{
	SG_QUIZ_EVENT_LOSER_LIST	*lpPacket	=	&_lpPacket->mSG_QUIZ_EVENT_LOSER_LIST;

	if	(lpPacket->bf4QuizType	==	eET_OX_QUIZ)
	{
		BOOL	bIsHeroLoseQuiz	=	FALSE;

		for (int i=0;i<lpPacket->bf8LoserCount;i++)
		{
			CActor	*lpActor	=	g_am.getTestedActor(lpPacket->awLoserList[i]);

			if	(lpActor)
			{
				lpActor->m_bf1IsQuizEventLoser	=	TRUE;

				if	(lpActor->isHero())
					bIsHeroLoseQuiz	=	TRUE;
			}
		}

		if	(bIsHeroLoseQuiz)
		{
			g_msgBox.cPopup("",dMSG_YOU_ARE_LOSE_TO_QUIZ_EVENT,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_RETURN_TO_THE_VILLAGE_BY_END_OF_OX_QUIZ_EVENT);
		}
	}

	if	(lpPacket->bf4QuizType	==	eET_WORD_QUIZ)
	{
		BOOL	bIsHeroLoseQuiz	=	FALSE;

		for (int i=0;i<lpPacket->bf8LoserCount;i++)
		{
			CActor	*lpActor	=	g_am.getTestedActor(lpPacket->awLoserList[i]);

			if	(lpActor)
			{
				lpActor->m_bf1IsQuizEventLoser	=	TRUE;

				if	(lpActor->isHero())
					bIsHeroLoseQuiz	=	TRUE;
			}
		}

		if	(lpPacket->bf1IsFinishEvent	==	FALSE)
		{
			g_eventWordQuiz.m_wRound		=	lpPacket->wRound;
			g_eventWordQuiz.m_wTop10Count	=	0;
			g_eventWordQuiz.m_wScore		=	0;
			g_eventWordQuiz.m_wScorerCount	=	0;

			if	(bIsHeroLoseQuiz)
			{
				g_msgBox.cPopup("",dMSG_YOU_ARE_LOSE_TO_QUIZ_EVENT,dMSG_YES,dMSG_NO);
				g_msgBox.setButtonId(eMBI_RETURN_TO_THE_VILLAGE_BY_END_OF_OX_QUIZ_EVENT);
			}
			else
				CGamePlay::WarningMessage(dMSG_WQ_CHANGE_ROUND_FORM);
		}
	}

}

//	OX퀴즈 취소
void
cPACKET_HANDLER::ReceiveCancelQuizEvent(SERVER_PACKETS *_lpPacket)
{
	SG_CANCEL_QUIZ_EVENT	*lpPacket	=	&_lpPacket->mSG_CANCEL_QUIZ_EVENT;

	char	strText[256];

	char	*lpstrTimeStr	=	cGAME_UTIL::GetTimeStringHM(lpPacket->wHour,lpPacket->wMinute);

	if	(lpPacket->wQuizType==	eET_OX_QUIZ)
	{
		if	(s_bIsOXQuizField == FALSE)
		{
			sprintf(strText,dMSG_CANCEL_OX_QUIZ_FORM,lpstrTimeStr);

			CGamePlay::Notice(strText);
		}
		else
		{
			sprintf(strText,dMSG_CANCEL_OX_QUIZ_IN_OX_QUIZ_FIELD_FORM,lpstrTimeStr);
			g_msgBox.cPopup("",strText,dMSG_OK);
			g_msgBox.setButtonId(eMBI_RETURN_TO_THE_VILLAGE_BY_END_OF_OX_QUIZ_EVENT);
		}
	}

}

//	OX퀴즈 생존자 수
void
cPACKET_HANDLER::ReceiveOXQuizSurvivorCount(SERVER_PACKETS *_lpPacket)
{
	SG_OX_QUIZ_SURVIVOR_COUNT	*lpPacket	=	&_lpPacket->mSG_OX_QUIZ_SURVIVOR_COUNT;

	g_am.m_wOXQuizLoserCountInServer	=	lpPacket->wLoserCount;
	g_am.m_wOXQuizSurvivorCountInServer	=	lpPacket->wSurvivorCount;
}

//	OX퀴즈 패자 부활전 시작
void
cPACKET_HANDLER::ReceiveReviveOXQuizLoserBattle(SERVER_PACKETS *_lpPacket)
{
	SG_OX_QUIZ_LOSER_REVIVE_BATTLE	*lpPacket	=	&_lpPacket->mSG_OX_QUIZ_LOSER_REVIVE_BATTLE;

	g_am.reviveOXQuizLoser();

	g_am.m_wOXQuizWinnerCount			=	lpPacket->wWinnerCount;
	g_am.m_wOXQuizBattleCount			=	lpPacket->wBattleCount;
	g_am.m_wOXQuizSurvivorCountInServer	=	lpPacket->wRevivorCount;
	g_am.m_wOXQuizLoserCountInServer	=	0;
}


void
cPACKET_HANDLER::ReceiveOXQuizWinnerList(SERVER_PACKETS *_lpPacket)
{
	SG_OX_QUIZ_WINNER_LIST			*lpPacket	=	&_lpPacket->mSG_OX_QUIZ_WINNER_LIST;

	g_am.receiveOXQuizWinnerList(lpPacket->awWinnerList,lpPacket->wWinnerCount);
}

void
cPACKET_HANDLER::ReceiveSoundOfLeadersBell(SERVER_PACKETS *_lpPacket)
{
	SG_SOUND_OF_LEADERS_BELL		*lpPacket	=	(SG_SOUND_OF_LEADERS_BELL*)_lpPacket;

	BOOL	bIsOwner	=	FALSE;

	if	(STRICMP(g_hero.m_strName,lpPacket->strCaster)	==	0)
		bIsOwner	=	TRUE;

	if	(lpPacket->wIsDisplayNameWithSoundOfLeadersBell	==	FALSE)
		lpPacket->strCaster[0]	=	NULL;

	if	(lpPacket->wWork	==	eSoundOfLeadersBellWork_end)
	{
		if	(lpPacket->strCaster[0])
			CGamePlay::Notice(_ms(dMSG_END_EFFECT_OF_SOUND_OF_LEADERS_BELL,lpPacket->strCaster),5);
		else
			CGamePlay::Notice(_ms(dMSG_END_EFFECT_OF_SOUND_OF_LEADERS_BELL,dMSG_UNKWON_SOMEBODY),5);

		if	(bIsOwner)
			g_lpHero->m_bf1IsSoundOfLedersBellsLeader	=	FALSE;

		return;
	}

	int		iUpkeepMinute	=	lpPacket->iUpkeepSecond/60;

	if	(lpPacket->wWork	==	eSoundOfLeadersBellWork_begin)
	{
		g_floatEffect.add(400,200,g_im.m_wSoundOfLeadersBellImage,0,FALSE,0,0,FALSE,0xffff,100,TRUE);
		g_esm.play("leaders_bell.ogg");

		cSTRING	string;

		if	(lpPacket->strCaster[0])
			string.Add(dMSG_USE_SOUND_OF_LEADERS_BELL_FORM,lpPacket->strCaster);
		else
			string.Add(dMSG_USE_SOUND_OF_LEADERS_BELL_FORM,dMSG_UNKWON_SOMEBODY);

		string.Add("\n<c:LTGREEN>%s<n>\n",lpPacket->strMessage);
		string.Add(dMSG_INCREASE_EXP_AND_ITEM_DROP_CHANCE_BY_SOUND_OF_LEADERS_BELL,lpPacket->iBoostExp-100);

		string.Add(" / ");

		if	(iUpkeepMinute/60)
			string.Add(dMSG_REMAIN_TIME_HOUR_MINUTE_FORM,iUpkeepMinute/60,iUpkeepMinute%60);
		else
			string.Add(dMSG_REMAIN_TIME_MINUTE_FORM,iUpkeepMinute);

		CGamePlay::Notice(string.String,15,TRUE,24,4);

		return;
	}

	if	(lpPacket->wWork	==	eSoundOfLeadersBellWork_move_field)
	{
		cSTRING	string;

		if	(lpPacket->strCaster[0])
			string.Add(dMSG_UPKEEPING_SOUND_OF_LEADERS_BELL_FORM,lpPacket->strCaster);
		else
			string.Add(dMSG_UPKEEPING_SOUND_OF_LEADERS_BELL_FORM,dMSG_UNKWON_SOMEBODY);

		string.Add("\n<c:LTGREEN>%s\n<n>",lpPacket->strMessage);
		string.Add(dMSG_INCREASE_EXP_AND_ITEM_DROP_CHANCE_BY_SOUND_OF_LEADERS_BELL,lpPacket->iBoostExp-100);

		string.Add(" / ");

		if	(iUpkeepMinute/60)
			string.Add(dMSG_REMAIN_TIME_HOUR_MINUTE_FORM,iUpkeepMinute/60,iUpkeepMinute%60);
		else
			string.Add(dMSG_REMAIN_TIME_MINUTE_FORM,iUpkeepMinute);

		CGamePlay::Notice(string.String,5,TRUE,24,4);
	}
}

void
cPACKET_HANDLER::ReceiveBuyCarrotShopItemResult(SERVER_PACKETS *_lpPacket)
{
	SG_BUY_CARROT_SHOP_ITEM_RESULT	*lpPacket	=	(SG_BUY_CARROT_SHOP_ITEM_RESULT *)_lpPacket;

	g_gwCarrotShop.m_wIsWaitResult		=	FALSE;

	switch(lpPacket->wResult)
	{
		case	eBCIR_SUCCESS			:
		{
			g_gwCarrotShop.buyItem((cItem *)&lpPacket->item,lpPacket->iRemainCarrotCount);
			break;
		}
		case	eBCIR_FAILED			:			//	걍 실패
			g_lpHero->warning(dMSG_FAILED_TO_BUY_CARROT_SHOP_ITEM);
			break;
		case	eBCIR_LOW_CARROT		:		//	캐롯이 모자라
			g_gwCarrotShop.m_iRemainCarrotCount	=	lpPacket->iRemainCarrotCount;
			g_lpHero->warning(dMSG_LOW_CARROT_TO_BUY_CARROT_SHOP_ITEM);
			break;
		case	eBCIR_FULL_INVENTORY	:	//	인벤토리가 꽉 찼다.
			g_lpHero->warning(dMSG_INVENTORY_FULL);
			break;
	}
}

//	배너 정보
void
cPACKET_HANDLER::ReceiveBannerInfo(SERVER_PACKETS *_lpPacket)
{
	SG_BANNER_INFO	*lpPacket	=	(SG_BANNER_INFO	*)_lpPacket;

	g_bannerManager.generate(lpPacket->bannerInfo.m_wBannerSerial,lpPacket->bannerInfo.m_wX,lpPacket->bannerInfo.m_wY,lpPacket->bannerInfo.m_bf15Shape,lpPacket->bannerInfo.m_bf1IsVertical,lpPacket->bannerInfo.m_strTitle);
}

//	배너 텍스트
void
cPACKET_HANDLER::ReceiveBannerText(SERVER_PACKETS *_lpPacket)
{
	SG_BANNER_TEXT	*lpPacket	=	(SG_BANNER_TEXT	*)_lpPacket;

	g_bannerManager.setText(lpPacket->wBannerSerial,lpPacket->strText);
}

//	배너 리스트
void
cPACKET_HANDLER::ReceiveBannerList(SERVER_PACKETS *_lpPacket)
{
	SG_BANNER_LIST	*lpPacket	=	(SG_BANNER_LIST	*)_lpPacket;

	int			iBannerCount	=	lpPacket->abBuffer[0];
	int			iBP				=	1;	//	Buffer Point

	for	(int	i=0;i<iBannerCount;i++)
	{
		cBannerSimpleInfo	*lpBI	=	(cBannerSimpleInfo	*)&lpPacket->abBuffer[iBP];

		g_bannerManager.generate(lpBI->m_wBannerSerial,lpBI->m_wX,lpBI->m_wY,lpBI->m_bf15Shape,lpBI->m_bf1IsVertical,lpBI->m_strTitle);

		iBP	+=	sizeof(cBannerSimpleInfo)-sizeof(lpBI->m_strTitle)+strlen(lpBI->m_strTitle)+1;
	}
}

//	이벤트

void
cPACKET_HANDLER::ReceiveScreenMove(SERVER_PACKETS *_lpPacket)
{
	SG_SCREEN_EVENT	*lpPacket	=	(SG_SCREEN_EVENT *)_lpPacket;

	g_lpHero->ClosePopupWindow();
	g_bObserverMode = TRUE;

	g_am.ChangeObserverMode(TRUE , lpPacket->waValue[0] , lpPacket->waValue[1] , lpPacket->waValue[2], lpPacket->waValue[3]);
}

//	이벤트
void
cPACKET_HANDLER::ReceiveBoostPowerByEvent(SERVER_PACKETS *_lpPacket)
{
	SG_BOOST_POWER_BY_EVENT	*lpPacket	=	(SG_BOOST_POWER_BY_EVENT *)_lpPacket;

	switch(lpPacket->wWork)
	{
		case	eBPBE_TM_GOLD	:	//	보물 지도 이벤트를 인한 골드 4배
		{
			g_iBoostGoldSecondByTreasureMapEventInServer=	lpPacket->wSecond;
			break;
		}

		case	eBPBE_TM_EXP	:	//	보물 지도 이벤트로 인한 경험치 2배
		{
			g_iBoostExpSecondByTreasureMapEventInServer	=	lpPacket->wSecond;
			break;
		}
	}

	CGamePlay::Notice(_ms(dMSG_UNSEAL_RED_STONE_AIR_BY_SPECIFIC_PLAYER_FORM,lpPacket->strName));
}

//
//	췌팅 실패
void
cPACKET_HANDLER::ReceiveChatFailed(SERVER_PACKETS *_lpPacket)
{
	SG_CHAT_FAILED	*lpPacket	=	&_lpPacket->mSG_CHAT_FAILED;

	switch(lpPacket->wReason)
	{
		case	eRCF_CAN_NOT_SHOUT_BY_TIME	:	//	너무 자주 떠든다.
			g_lpHero->warning(dMSG_CAN_NOT_SHOUT_REASON_BY_TIME);
			break;
	}
}

//
//세이 실패
void
cPACKET_HANDLER::ReceiveSayResult(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::ReceiveSayResult(&_lpPacket->mSG_SAY_RESULT);
}

//
//	파티리스트
void
cPACKET_HANDLER::ReceivePartyList(SERVER_PACKETS *_lpPacket)
{

}

//	파티상태 바뀜
void
cPACKET_HANDLER::ReceiveChangePartyStatus(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::ChangePartyStatus((SG_CHANGE_PARTY_STATUS	*)_lpPacket);
}


//
//	파티멤버에게 보내는 파티 정보(파티원 리스트나 기타등등)
void
cPACKET_HANDLER::ReceivePartyInfo(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::SetPartyInfo(&_lpPacket->mSG_PARTY_INFO);
}

//
//	파티 참가 요청
void
cPACKET_HANDLER::ReceiveAskJoinParty(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::AskJoinParty(&_lpPacket->mSG_ASK_JOIN_PARTY);
}

//
//	파티 참가 요청
void
cPACKET_HANDLER::ReceiveAskUnionJoinParty(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::AskJoinUnionParty(&_lpPacket->mSG_ASK_JOIN_UNION_PARTY);
}

void
cPACKET_HANDLER::ReceiveReplayUnionJoinParty(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::ReplayJoinUnionParty(&_lpPacket->mSG_UNION_PARTY_ETC_INFO);
}

void
cPACKET_HANDLER::ReceiveUnionPartyMemberInfo(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::OperateUnionPartyMemberInfo(&_lpPacket->mSG_UNION_PARTY_MEMBER_INFO);
}

void
cPACKET_HANDLER::ReceiveUnionPartyInfo(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::OperateUnionPartyInfo(&_lpPacket->mSG_UNION_PARTY_INFO);
}

void
cPACKET_HANDLER::ReceiveUnionPartyDissolution(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::OperateUnionPartyDissolution(&_lpPacket->mSG_UNION_PARTY_ETC_INFO);
}


//
//	파티 멤버 추가
void
cPACKET_HANDLER::ReceiveJoinPartyMember(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::JoinPartyMember(&_lpPacket->mSG_JOIN_PARTY_MEMBER);
}

//
//	누군가 파티에서 탈퇴 했다.(거나 쫏겨 났거나.. -_-a)
void
cPACKET_HANDLER::ReceivePartyWork(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::OperatePartyWork(&_lpPacket->mSG_PARTY_WORK);
}

//
//	파티 멤버 정보를 업데이트(체력정도..)
void
cPACKET_HANDLER::ReceiveUpdatePartyMemberInfo(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::UpdatePartyMemberInfo(&_lpPacket->mSG_UPDATE_PARTY_MEMBER_INFO);
}

//	파티 멤버 정보를 체력 업데이트
void
cPACKET_HANDLER::ReceiveUpdatePartyMemberHP(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::UpdatePartyMemberHP((SG_UPDATE_PARTY_MEMBER_HP *)_lpPacket);
}

//
//	파티 이름 변경
void
cPACKET_HANDLER::ReceiveChangePartyName(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::ChangePartyName(_lpPacket->mSG_CHANGE_PARTY_NAME.strPartyName);
}

//
//	파티 헤체 메시지
void
cPACKET_HANDLER::ReceiveDissolutionParty(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::DissolutionParty();
}

//
//	친구 추가 요청 메시지
void
cPACKET_HANDLER::ReceiveRequestAddFriend(SERVER_PACKETS *_lpPacket)
{
	SG_REQUEST_ADD_FRIEND	*lpPacket	=	&_lpPacket->mSG_REQUEST_ADD_FRIEND;

	g_gwFriend.askAddFriend(lpPacket->strName);
}

//	친구 목록에서 제거 요청 메시지
void
cPACKET_HANDLER::ReceiveAskRemoveFriend(SERVER_PACKETS *_lpPacket)
{
	SG_ASK_REMOVE_FRIEND	*lpPacket	=	(SG_ASK_REMOVE_FRIEND	*)_lpPacket;//	친구 목록에서 제거 요청

	g_gwFriend.askRemoveFriend(lpPacket->strName);
}

//	친구 목록에서 제거 요청의 결과
void
cPACKET_HANDLER::ReceiveAskRemoveFriendResult(SERVER_PACKETS *_lpPacket)
{
	SG_ASK_REMOVE_FRIEND_RESULT	*lpPacket	=	(SG_ASK_REMOVE_FRIEND_RESULT	*)_lpPacket;//	친구 목록에서 제거 요청의 결과

	switch(lpPacket->wResult)
	{
		case	eRFR_SUCCESS								:
			CGamePlay::AddSystemMessage(LTGREEN,dMSG_REMOVE_MY_NAME_IN_TARGET_FRIEND_LIST_SUCCESS_FORM,lpPacket->strName);
			break;
		case	eRFR_NOT_EXIST_NAME_IN_TARGET_FRIEND_LIST	:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_NOT_EXIST_MY_NAME_IN_TARGET_FRIEND_LIST_FORM,lpPacket->strName);
			break;
		case	eRFR_TARGET_NOT_CONNECTED					:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_REMOVE_MY_NAME_IN_TARGET_FRIEND_LIST_RULE_FORM,lpPacket->strName);
			break;
	}
}


//
//	친구 추가 요청 메시지에 대한 응답
void
cPACKET_HANDLER::ReceiveReplyForAddFriend(SERVER_PACKETS *_lpPacket)
{
	SG_REPLY_FOR_ADD_FRIEND	*lpPacket	=	&_lpPacket->mSG_REPLY_FOR_ADD_FRIEND;

	switch(lpPacket->wResult)
	{
		case	eRFA_AUTO_ADD_BY_ADDED		:
		case	eRFA_OK						:			//	OK
		{
			int	iAddResult	=	g_hero.addFriend(lpPacket->strName,lpPacket->bf8Job,lpPacket->bf1IsRecover);

			switch(iAddResult)
			{
				case	eAFR_FAILED_BY_TOO_MANY			:
					if	(lpPacket->wResult	!=	eRFA_AUTO_ADD_BY_ADDED	)
						g_lpHero->warning(dMSG_TOO_MANY_FRIEND);
					return;
				case	eAFR_FAILED_BY_EXIST_SAME_NAME	:
					if	(lpPacket->wResult	!=	eRFA_AUTO_ADD_BY_ADDED	)
						g_lpHero->warning(dMSG_PLAYER_ARE_ALLREADY_FRIEND);
					return;

				case	eAFR_SUCCESS					:
				{
					g_gwFriend.resetFriendList();

					if	(lpPacket->wResult	==	eRFA_OK	)
						CGamePlay::AddSystemMessage(WHITE,dMSG_ADD_FRIEND_FORM,lpPacket->strName);
					break;
				}
			}

			if	(lpPacket->wResult	==	eRFA_AUTO_ADD_BY_ADDED	)
				CGamePlay::AddSystemMessage(WHITE,dMSG_ADD_YOU_BY_FRIEND_FROM,lpPacket->strName);

			break;
		}

		case	eRFA_DENY					:			//	거절
			g_lpHero->warning(dMSG_DENY_ADD_FRIEND_MESSAGE);
			break;

		case	eRFA_CAN_NOT_FIND			:	//	못 찾겠어요. -o-
			g_lpHero->warning(dMSG_CAN_NOT_FIND_TARGET);
			break;
			
		case	eRFA_STATUS_DENY_COMMUNITY	:	//	커뮤니티 거부 상태
			g_lpHero->warning(dMSG_TARGET_DENY_COMMUNITY_STATUS);
			break;
	}
}

void
cPACKET_HANDLER::ReceivePickPartyItem(SERVER_PACKETS *_lpPacket)
{
	SG_PICK_PARTY_ITEM	*lpPacket	=	(SG_PICK_PARTY_ITEM	*)_lpPacket;

	cItem	*lpItem	=	(cItem *)&lpPacket->item;

	char	strMessage[512];
	DWORD	dwColor;

	if	(lpItem->m_wBaseItem	==	dITEM_MONEY)
	{
		dwColor	=	LTYELLOW;

		sprintf(strMessage,dMSG_GET_PARTY_GOLD_FORM,lpPacket->strPickerName,lpItem->m_dwSerial);
	}
	else
	{
		dwColor	=	lpItem->getNameColor();

		sprintf(strMessage,dMSG_PICK_PARTY_ITEM_FORM,lpPacket->strPickerName,lpItem->getName(FALSE));
	}

	CGamePlay::EventNotice(strMessage,eCT_GET_PARTY_ITEM_MESSAGE,FALSE,dwColor);
}

//
//	파티와 관련된 이런저런 메시지(넌 리더가 아냐!!,넌 파티에 속해 있지 않아!!,파티 풀~ 등등등..)
void
cPACKET_HANDLER::ReceivePartyMessage(SERVER_PACKETS *_lpPacket)
{
	switch(_lpPacket->mSG_PARTY_MESSAGE.wMessage)
	{
		case	ePM_ASK_JOIN_FAILED_BY_TARGET_LEVEL_OVER_THE_SECRET_DUNGEON_LIMIT_LEVEL	:
			g_lpHero->warning(dMSG_PM_ASK_JOIN_FAILED_BY_TARGET_LEVEL_OVER_THE_SECRET_DUNGEON_LIMIT_LEVEL);	
			break;

		case	ePM_TARGET_DEATH_OR_NOT_EXIST								:	//	대상이 죽었거나 존재하지 않는다.
			g_lpHero->warning(dMSG_PM_TARGET_DEATH_OR_NOT_EXIST);	
			break;
		case	ePM_TARGET_BELONG_TO_ALREADY_AT_DIFFERENT_PARTY				:	//	이미 다른 파티에 소속해 있다.
			g_lpHero->warning(dMSG_PM_TARGET_BELONG_TO_ALREADY_AT_DIFFERENT_PARTY);	
			break;
		case	ePM_TARGET_NOT_BELONG_TO_PARTY								:	//	그 녀석.. 우리 파티원이 아니다. -_-
			g_lpHero->warning(dMSG_PM_TARGET_NOT_BELONG_TO_PARTY);	
			break;
		case	ePM_TARGET_NOT_EXIST										:	//	그런넘 존재하지 않는다.
			g_lpHero->warning(dMSG_PM_TARGET_NOT_EXIST);
			break;

		case	ePM_CAN_NOT_BANISH_CONNECT_MEMBER_IN_SECRET_DUNGEON			:	//	타겟이 같은 팀이 아니다.
			g_lpHero->warning(dMSG_CAN_NOT_BANISH_CONNECT_MEMBER_IN_SECRET_DUNGEON);
			break;
			
		case	ePM_TARGET_NOT_OWN_FORCE									:	//	타겟이 같은 팀이 아니다.
			g_lpHero->warning(dMSG_PM_TARGET_NOT_OWN_FORCE);
			break;

		case	ePM_TARGET_DENY_COMMUNITY_STATUS							:	//	대상이 커뮤니티 거부 상태다.
			g_lpHero->warning(dMSG_TARGET_DENY_COMMUNITY_STATUS);
			break;
		case	ePM_ONLY_PARTY_LEADER_CAN_SEND_AN_INVITATION_MESSAGE		:	//	파티 리더만 초청 메시지를 보낼수 있습니다.
			g_lpHero->warning(dMSG_PM_ONLY_PARTY_LEADER_CAN_SEND_AN_INVITATION_MESSAGE);
			break;
		case	ePM_ONLY_PARTY_LEADER_CAN_SEND_AN_BANISH_MESSAGE			:	//	파티 리더만 초청 메시지를 보낼수 있습니다.
			g_lpHero->warning(dMSG_PM_ONLY_PARTY_LEADER_CAN_SEND_AN_BANISH_MESSAGE);
			break;
		case	ePM_ONLY_PARTY_LEADER_CAN_SEND_AN_CHANGE_PARTY_NAME_MESSAGE	:	//	리더만 파티명을 변경 할 수 있다.
			g_lpHero->warning(dMSG_PM_ONLY_PARTY_LEADER_CAN_SEND_AN_CHANGE_PARTY_NAME_MESSAGE);
			break;
		case	ePM_YOU_ARE_NOT_BELONG_TO_PARTY								:	//	넌 파티에 소속되어 있지 않아!!
		{
			g_lpHero->warning(dMSG_PM_YOU_ARE_NOT_BELONG_TO_PARTY);
			CGamePlay::s_partyInfo.reset();
			g_hero.setPartySerial(0xffff);
			break;
		}
		case	ePM_ALREADY_FULL_PARTY										:	//	풀 파티라서 더 이상 멤버를 받을 수 없다.
			g_lpHero->warning(dMSG_PM_ALREADY_FULL_PARTY);
			break;
		case	ePM_ASKER_NOT_EXIST											:	//	요청자가 없어졌다.
			g_lpHero->warning(dMSG_PM_ASKER_NOT_EXIST);
			break;
		case	ePM_DENY_INVITE_MESSAGE										:	//	초청을 거부했다.
			g_lpHero->warning(dMSG_PM_DENY_INVITE_MESSAGE);
			break;
		case	ePM_DEST_PARTY_ARE_FULL										:	//	풀 파티가 되어 버렸다.
			g_lpHero->warning(dMSG_PM_DEST_PARTY_ARE_FULL);
			break;
		case	ePM_DEST_ACTOR_NOT_LEADER									:	//	초청한넘... 리더 아냐-_-
			g_lpHero->warning(dMSG_PM_DEST_ACTOR_NOT_LEADER);
			break;
		case	ePM_FAILED_TO_CREATE_PARTY									:	//	파티 생성 실패
			g_lpHero->warning(dMSG_PM_FAILED_TO_CREATE_PARTY);
			break;
		case	ePM_FAILED_TO_JOIN_PARTY									:	//	파티 참가에 실패
			g_lpHero->warning(dMSG_PM_FAILED_TO_JOIN_PARTY);
			break;
		case	ePM_CAN_NOT_BANISH_MEMBER_UNDER_DROP_PARTY_ITEM_TIME		:	//	파티 참가에 실패
			g_lpHero->warning(dMSG_CAN_NOT_BANISH_MEMBER_UNDER_DROP_PARTY_ITEM_TIME);
			break;
	}
}