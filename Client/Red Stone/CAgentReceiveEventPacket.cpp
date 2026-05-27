#include "CAgent.H"
#include "CActor.H"
#include "CGamePlay.H"
#include "CMessage.H"
#include "CWindowInterface.H"
#include "cSound.H"
#include "dialog_box.H"

//
//대화
void
cPACKET_HANDLER::ReceiveComplexSpeech(SERVER_PACKETS *_lpPacket)
{
	SG_COMPLEX_SPEECH	*lpPacket	=	&_lpPacket->mSG_COMPLEX_SPEECH;
	int					iStringPoint=	0;

	g_dialogBox.m_speech.init(lpPacket->wOwner,lpPacket->wSpeechIndex,lpPacket->strText,lpPacket->wDialogBoxSize);

	iStringPoint	+=	strlen(lpPacket->strText)+1;

	for (int i=0;i<lpPacket->wContentsCount;i++)
	{
		g_dialogBox.m_speech.addContent(lpPacket->strText+iStringPoint);
		iStringPoint	+=	strlen(lpPacket->strText+iStringPoint)+1;
	}

	g_dialogBox.beginComplexDialog(lpPacket->wOwner,lpPacket->wEventArea);	//	대화 시작
}

//
//이벤트 실패
void
cPACKET_HANDLER::ReceiveEventFailed(SERVER_PACKETS *_lpPacket)
{
	SG_EVENT_FAILED		*lpPacket	=	&_lpPacket->mSG_EVENT_FAILED;

	switch(lpPacket->wReason)
	{
		case	eREF_NOT_OWNED_EVENT	:	//	가지고 있는 이벤트가 없다.
			break;

		case	eREF_TOO_FAR			:	//	너무 멀다.
			g_lpHero->warning(dMSG_TOO_FAR_DISTANCE_TARGET);
			break;
	}
}	//	cPACKET_HANDLER::ReceiveEventFailed(SERVER_PACKETS *_lpPacket)

//
//대화 종료
void
cPACKET_HANDLER::ReceiveEndDialog(SERVER_PACKETS *_lpPacket)
{
	SG_END_DIAOLG		*lpPacket	=	&_lpPacket->mSG_END_DIAOLG;

	g_dialogBox.closeComplexDialog();
}	//	cPACKET_HANDLER::ReceiveEndDialog(SERVER_PACKETS *_lpPacket)

//대화 상점 열기 
void
cPACKET_HANDLER::ReceiveOpenShop(SERVER_PACKETS *_lpPacket)
{
	SG_OPEN_SHOP		*lpPacket	=	&_lpPacket->mSG_OPEN_SHOP;

	g_dialogBox.closeComplexDialog();
	g_gwShop.open(lpPacket);
}

//길드 상점 열기 
void
cPACKET_HANDLER::ReceiveOpenGuildShop(SERVER_PACKETS *_lpPacket)
{
	g_gwShop.openForGuildShop((SG_OPEN_GUILD_SHOP *)_lpPacket);
}

//아이템 구입
void
cPACKET_HANDLER::ReceiveBuyItem(SERVER_PACKETS *_lpPacket)
{
	SG_BUY_ITEM	*lpPacket		=	&_lpPacket->mSG_BUY_ITEM;
	cItem		itemAdd;

	itemAdd.copy(&lpPacket->aBuyItem[0]);

	s_agent.m_bIsWaitTradeItemResult	=	FALSE;

	for	(int i=0;i<lpPacket->wCount;i++)
	{
		if	(!g_hero.addItemToInventory((cItem *)&lpPacket->aBuyItem[i]))
		{
			s_agent.sendAskItemData();

			CGamePlay::AddSystemMessage(WHITE,dMSG_BUY_ITEM);

			return;
		}
	}

	cBasicItem	*lpBasicItem	=	itemAdd.getBasicItem();

	int	iBuyCount				=	itemAdd.m_bCount*lpPacket->wCount;

	if	(lpPacket->bf1IsDuelPointItem)
	{
		g_hero.m_duelRecord.m_iDuelPoint=	lpPacket->iRemainGold;

		CGamePlay::AddSystemMessage(WHITE,dMSG_BUY_DUEL_POINT_ITEM_FORM,lpBasicItem->m_strName,iBuyCount,CGamePlay::GetMoneyString((DWORD)lpPacket->dwPrice*lpPacket->wCount));
	}
	else if (lpPacket->wTokenShop != 0xffff)
	{
		//CGamePlay::AddSystemMessage(WHITE,"ggg");
		//CGamePlay::AddSystemMessage(WHITE,dMSG_BUY_DUEL_POINT_ITEM_FORM,lpBasicItem->m_strName,iBuyCount,CGamePlay::GetMoneyString((DWORD)lpPacket->dwPrice*lpPacket->wCount));
	}
	else
	{
		g_hero.m_iGold			=	lpPacket->iRemainGold;

		CGamePlay::AddSystemMessage(WHITE,dMSG_BUY_ITEM_FORM,lpBasicItem->m_strName,iBuyCount,CGamePlay::GetMoneyString((DWORD)lpPacket->dwPrice*lpPacket->wCount));
	}

	if	(lpPacket->bf1IsPigeonPostItem)
	{
		g_lpHero->addEffect(eHEOP_FOOT,g_im.m_wPigeonPost);
		g_esm.play("pigeon.wav");
	}
}

//
//아이템 구입 실패	
void
cPACKET_HANDLER::ReceiveFailedBuyItem(SERVER_PACKETS *_lpPacket)
{
	SG_FAILED_BUY_ITEM	*lpPacket	=	&_lpPacket->mSG_FAILED_BUY_ITEM;

	s_agent.m_bIsWaitTradeItemResult		=	FALSE;

	switch(lpPacket->wReason)
	{
		case	eFRBI_TOO_FAR_DISTANCE		:	//	너무 멀다.
		case	eFRBI_NPC_NOT_OWN_THAT_SHOP	:	//	그 NPC는 그런 상점을 운영-_-하고 있지 않다.
		case	eFRBI_NOT_EXIST_SHOP		:	//	존재하지 않는 상점이다.
		case	eFRBI_NOT_EXIST_ITEM		:	//	그 상점에는 그런 아이템이 없는디유.. -o-
		case	eFRBI_UNKONWN				:	//	걍 실패
			CGamePlay::AddSystemMessage(WHITE,dMSG_BUY_ITEM_FAILED);
			break;

		case	eFRBI_CAN_NOT_OWN_SAME_ITEM	:
			g_msgBox.cPopup("",dMSG_CAN_NOT_OWN_SAME_ITEM,dMSG_OK);
			break;

		case	eFRBI_TOO_MANY_BADGE_ITEM	:
			g_msgBox.cPopup("",_ms(dMSG_TOO_MANY_BADGE_ITEM_FORM,dLIMIT_BADGE_ITEM_COUNT_IN_INVENTORY),dMSG_OK);
			break;

		case	eFRBI_LACK_OF_DUEL_POINT	:
			CGamePlay::AddSystemMessage(WHITE,dMSG_LACK_OF_DUEL_POINT);
			break;

		case	eFRBI_LACK_OF_GOLD			:	//	골드가 부족하다.
			g_hero.m_iGold	=	lpPacket->iGold;
			CGamePlay::AddSystemMessage(WHITE,dMSG_LACK_OF_GOLD);
			break;

		case	eFRBI_INVENTORY_FULL		:	//	인벤토리가 꽉 찼다.
			s_agent.sendAskItemData();
			CGamePlay::AddSystemMessage(WHITE,dMSG_INVENTORY_FULL);
			break;
	}
}	//	cPACKET_HANDLER::ReceiveFailedBuyItem(SERVER_PACKETS *_lpPacket)

//
//아이템 판매
void
cPACKET_HANDLER::ReceiveSellItem(SERVER_PACKETS *_lpPacket)
{
	SG_SELL_ITEM	*lpPacket	=	&_lpPacket->mSG_SELL_ITEM;
	cItem			*lpItem		=	&g_hero.m_aItems[lpPacket->wSellItemSlot];

	s_agent.m_bIsWaitTradeItemResult	=	FALSE;

	if (lpItem->m_wBaseItem	==	0xffff)
	{
		s_agent.sendAskItemData();

		CGamePlay::AddSystemMessage(WHITE,dMSG_SELL_ITEM);

		return;
	}

	cBasicItem	*lpBasicItem=	lpItem->getBasicItem();

	g_hero.m_iGold			=	lpPacket->iRemainGold;

	CGamePlay::AddSystemMessage(WHITE,dMSG_SELL_ITEM_FORM,lpBasicItem->m_strName,lpPacket->wCount,lpPacket->iPrice);

	if (!lpItem->decreaseCount(lpPacket->wCount))
	{
		s_agent.sendAskItemData();

		return;
	}

	g_hero.arrangeInventory();
}	//	cPACKET_HANDLER::ReceiveSellItem(SERVER_PACKETS *_lpPacket)

//
//아이템 판매 실패
void
cPACKET_HANDLER::ReceiveFailedSellItem(SERVER_PACKETS *_lpPacket)
{
	SG_FAILED_SELL_ITEM	*lpPacket	=	&_lpPacket->mSG_FAILED_SELL_ITEM;

	s_agent.m_bIsWaitTradeItemResult	=	FALSE;

	switch(lpPacket->wReason)
	{
		case	eFRSI_CHANGE_ITEM_PRICE		:
			if (g_gwShop.isOpened())
			{
				CGamePlay::AddSystemMessage(WHITE,dMSG_CLOSE_SHOP_BY_ITEM_PRICE);
				g_gwShop.close();
			}
			break;
		case	eFRSI_TOO_FAR_DISTANCE		:	//	너무 멀다.
		case	eFRSI_NPC_NOT_OWN_THAT_SHOP	:	//	그 NPC는 그런 상점을 운영-_-하고 있지 않다.
		case	eFRSI_NOT_EXIST_SHOP		:	//	존재하지 않는 상점이다.
		case	eFRSI_UNKONWN				:	//	걍 실패
			CGamePlay::AddSystemMessage(WHITE,dMSG_SELL_ITEM_FAILED);
			break;

		case	eFRSI_CAN_NOT_SELL_ITEM		:
			CGamePlay::AddSystemMessage(WHITE,dMSG_CAN_NOT_SELL_ITEM);
			break;

		case	eFRSI_OWN_TOO_MANY_GOLD		:
			CGamePlay::AddSystemMessage(WHITE,dMSG_FRSI_OWN_TOO_MANY_GOLD);
			break;

		case	eFRSI_NOT_EXIST_ITEM		:	//	그런 아이템이 없는디유.. -o-
		case	eFRSI_NOT_MATCHED_ITEM_COUNT:	//	아이템 갯수가 맞지 않아요.
			s_agent.sendAskItemData();
			CGamePlay::AddSystemMessage(WHITE,dMSG_SELL_ITEM_FAILED);
			break;
	}
}

//
//	퀘스트 정보 받기
void
cPACKET_HANDLER::ReceiveQuestInfo(SERVER_PACKETS *_lpPacket)
{
	SG_QUEST_INFO	*lpPacket	=	&_lpPacket->mSG_QUEST_INFO;

	memcpy(g_hero.m_aProcessQuest,lpPacket->m_aProcessQuest,sizeof(g_hero.m_aProcessQuest));
}

//
//	변경된 퀘스트 정보 받기
void
cPACKET_HANDLER::ReceiveChangeQuestInfo(SERVER_PACKETS *_lpPacket)
{
	SG_CHANGE_QUEST_INFO	*lpPacket	=	&_lpPacket->mSG_CHANGE_QUEST_INFO;
	tsProcessQuestField		*lpQuestInfo=	&g_hero.m_aProcessQuest[lpPacket->bf4ProcessSlot];

	if	(lpQuestInfo->isActive && lpPacket->processQuest.isActive == FALSE)
	{
		CQuestItem	*lpQuest	=	g_hero.getQuest(lpPacket->bf4ProcessSlot);

		if	(lpQuest->m_isRemoveQuestItemByCancelQuest)
			g_hero.removeQuestItem(lpQuestInfo->questKind,lpQuestInfo->questIndex);
	}

	BOOL	bIsNewQuest	=	FALSE,bIsChangeProcessLevel	=	FALSE;

	if (lpQuestInfo->isActive == FALSE && lpPacket->processQuest.isActive)
		bIsNewQuest	=	TRUE;
	else
	if (lpQuestInfo->questProcessLevel	!=	lpPacket->processQuest.questProcessLevel)
		bIsChangeProcessLevel	=	TRUE;

	memcpy(lpQuestInfo,&lpPacket->processQuest,sizeof(tsProcessQuestField));

	g_hero.m_awQuestCountDownTimer[lpPacket->bf4ProcessSlot]	=	lpPacket->wCountDownTime;

	if (g_gwQuest.isOpened())
	{
		g_gwQuest.open();	//	닫고/열고
		g_gwQuest.open();

		g_gwQuest.selectQuest(lpPacket->bf4ProcessSlot);
	}

	if (bIsNewQuest)
	{
		CQuestItem	*lpQuest	=	g_hero.getQuest(lpPacket->bf4ProcessSlot);

		if (lpQuest)
			CGamePlay::AddSystemMessage(WHITE,dMSG_ADD_QUEST_FORM,lpQuest->m_strName);
	}

	if (bIsChangeProcessLevel)
	{
		CQuestItem	*lpQuest	=	g_hero.getQuest(lpPacket->bf4ProcessSlot);

		if (lpQuest)
			CGamePlay::AddSystemMessage(WHITE,dMSG_CHANGE_QUEST_INFO_NOTICE_FORM,lpQuest->m_strName);
	}

	int iQuestIndex	= g_hero.m_aProcessQuest[g_map.m_iQuestIndex[g_map.m_iSelectQuest]].questIndex;
	CGamePlay::UpdateMinimapQuestList();

	g_map.FindQuestLinkField();
}

//
//	때려!!
void
cPACKET_HANDLER::ReceiveDamageToActorByKarma(SERVER_PACKETS *_lpPacket)
{
	SG_DAMAGE_TO_ACTOR_BY_KARMA	*lpPacket	=	&_lpPacket->mSG_DAMAGE_TO_ACTOR_BY_KARMA;

	for (int i=0;i<lpPacket->wCount;i++)
	{
		CDamageToActorByKarmaE	*lpData	=	&lpPacket->aData[i];

		CActor	*lpActor	=	g_am.getTestedActor(lpData->wActor,FALSE);

		if (!lpActor)	continue;

		lpActor->addHitEffectByEvent(lpPacket->wDamageType,lpData->wDamage);//	타격효과 추가
	}
}	//	cPACKET_HANDLER::ReceiveDamageToActorByKarma(SERVER_PACKETS *_lpPacket)
