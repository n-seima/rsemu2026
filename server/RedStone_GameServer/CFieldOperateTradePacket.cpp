#include "cFIELD.H"
#include "cGAME.H"
#include "cPACKET_WORLDSERVER.H"
#include "cJOB.H"
#include "packetManager.H"

//
//	언넘에게 트레이드 요청을 한다.
void
cFIELD::operateRequestTradePacket(CG_REQUEST_TRADE *_lpPacket,int _iSerial)
{
	if	(!checkActor(_iSerial))
		return;

	if	(g_iWorldServerType	==	eSERVER_TYPE_GVG)
		return;

	cACTOR	*lpAsker	=	getTestedActor(_iSerial);
	cACTOR	*lpTarget	=	getTestedActor(_lpPacket->wTradeTarget);

	if	(!lpAsker)
		return;
	
	SG_REQUEST_TRADE_RESULT	packet;

	packet.base.set(sizeof(packet),dSG_REQUEST_TRADE_RESULT);

	if	(g_bIsDuelServer)
	{
		packet.wReply	=	eRRT_CAN_NOT_TRADE_STATE;
		g_userPM.add(lpAsker->m_iClientSerial,&packet,packet.base.wSize);

		return;
	}

	if	(lpAsker->m_wTradeBoxSerial	!=	0xffff)
	{
		packet.wReply	=	eRRT_ALREADY_TRADE;
		g_userPM.add(lpAsker->m_iClientSerial,&packet,packet.base.wSize);

		return;
	}

	if	(!lpTarget	||	lpTarget->m_bf1IsHide	||	lpAsker->m_bf1IsHide)	//	그런넘 없어!!
	{
		packet.wReply	=	eRRT_NOT_EXIST_TARGET;
		g_userPM.add(lpAsker->m_iClientSerial,&packet,packet.base.wSize);

		return;
	}

	if	(lpAsker->m_wTeam	!=	lpTarget->m_wTeam)
	{
		packet.wReply	=	eRRT_CAN_NOT_TRADE_STATE;
		g_userPM.add(lpAsker->m_iClientSerial,&packet,packet.base.wSize);
	
		return;
	}

	if (lpTarget->m_wPitchmanShopSerial	!=	0xffff)
	{
		packet.wReply	=	eRRT_ALREADY_TRADE;
		g_userPM.add(lpAsker->m_iClientSerial,&packet,packet.base.wSize);

		return;
	}

	if (lpAsker->isIncorrectClient())	return;

	if (lpTarget->isIncorrectClient())
	{
		packet.wReply	=	eRRT_NOT_EXIST_TARGET;
		g_userPM.add(lpAsker->m_iClientSerial,&packet,packet.base.wSize);

		return;
	}

	if (lpTarget->isOperator())
	{
		packet.wReply	=	eRRT_CAN_NOT_ASK_TRADE_TO_OPERATOR;
		g_userPM.add(lpAsker->m_iClientSerial,&packet,packet.base.wSize);
		return;
	}

	if (lpTarget->m_wTradeBoxSerial	!=	0xffff)
	{
		packet.wReply	=	eRRT_ALREADY_TRADE;
		g_userPM.add(lpAsker->m_iClientSerial,&packet,packet.base.wSize);

		return;
	}
	
	if (lpAsker->m_iLevel < 4 || lpTarget->m_iLevel < 4 )
	{
		packet.wReply	=	eRRT_LOW_LEVEL;
		g_userPM.add(lpAsker->m_iClientSerial,&packet,packet.base.wSize);

		return;
	}

	int	iRange	=	GetOvalRange(lpAsker->m_pos.x,lpAsker->m_pos.y,lpTarget->m_pos.x,lpTarget->m_pos.y);

	if (iRange	>	dRANGE_OF_ENABLE_TRADE)
	{
		packet.wReply	=	eRRT_TOO_FAR;
		g_userPM.add(lpAsker->m_iClientSerial,&packet,packet.base.wSize);

		return;
	}

	if (lpTarget->isDeath()	||	lpAsker->isDeath())
	{
		packet.wReply	=	eRRT_DEATH_CHARACTER; 
		g_userPM.add(lpAsker->m_iClientSerial,&packet,packet.base.wSize);

		return;
	}

	if (lpTarget->isDenyCommunity()	==	TRUE)
	{
		packet.wReply	=	eRRT_TARGET_DENY_COMMUNITY_STATUS; 
		g_userPM.add(lpAsker->m_iClientSerial,&packet,packet.base.wSize);
		return;
	}

	lpTarget->m_wRequestTradePlayer	=	lpAsker->m_wSerialInField;

//	트레이드 요청 메시지 전송
	{
		SG_REQUEST_TRADE	requestPacket;

		requestPacket.base.set(sizeof(requestPacket),dSG_REQUEST_TRADE);

		strcpy(requestPacket.strAsker,lpAsker->m_strName);
		g_userPM.add(lpTarget->m_iClientSerial,&requestPacket,requestPacket.base.wSize);
	}
}

//
//	트레이드 요청을 받아 들인다.
void
cFIELD::operateAcceptTradePacket(CG_TRADE_REPLY *_lpPacket,int _iSerial)
{
	if (!checkActor(_iSerial))
		return;

	cACTOR	*lpReplyer	=	getTestedActor(_iSerial);

	if (!lpReplyer	||	lpReplyer->isIncorrectClient())
		return;

	cACTOR	*lpAsker	=	getPlayerByName(_lpPacket->strAsker);

	SG_REQUEST_TRADE_RESULT	resultPacket;

	resultPacket.base.set(sizeof(SG_REQUEST_TRADE_RESULT),dSG_REQUEST_TRADE_RESULT);

	if (!lpAsker || lpAsker->isIncorrectClient() || lpReplyer->m_wRequestTradePlayer !=	lpAsker->m_wSerialInField)
	{
		lpReplyer->m_wRequestTradePlayer	=	0xffff;
		resultPacket.wReply					=	eRRT_NOT_EXIST_TARGET;
		g_userPM.add(lpReplyer->m_iClientSerial,&resultPacket,resultPacket.base.wSize);

		return;
	}

	lpReplyer->m_wRequestTradePlayer	=	0xffff;

	if (lpReplyer->m_wTradeBoxSerial	!=	0xffff)
	{
		resultPacket.wReply	=	eRRT_ALREADY_TRADE;
		g_userPM.add(lpReplyer->m_iClientSerial,&resultPacket,resultPacket.base.wSize);

		return;
	}

	int	iRange	=	GetOvalRange(lpAsker->m_pos.x,lpAsker->m_pos.y,lpReplyer->m_pos.x,lpReplyer->m_pos.y);

	if (iRange	>	dRANGE_OF_ENABLE_TRADE)
	{
		resultPacket.wReply	=	eRRT_TOO_FAR;
		g_userPM.add(lpReplyer->m_iClientSerial,&resultPacket,resultPacket.base.wSize);

		return;
	}

	if (lpReplyer->isDeath()	||	lpAsker->isDeath())
	{
		resultPacket.wReply	=	eRRT_DEATH_CHARACTER;
		g_userPM.add(lpReplyer->m_iClientSerial,&resultPacket,resultPacket.base.wSize);

		return;
	}
	
	if (_lpPacket->wReply	==	eRTR_DENY)
	{
		resultPacket.wReply	=	eRRT_DENY;
		g_userPM.add(lpAsker->m_iClientSerial,&resultPacket,resultPacket.base.wSize);

		return;
	}

	int	iTradeBoxSerial	=	m_tradeManager.addTradeBox(lpAsker,lpReplyer);

	if (iTradeBoxSerial	==	0xffff)
	{
		resultPacket.wReply	=	eRRT_TOO_MANY_TRADE_BOX;

		g_userPM.add(lpReplyer->m_iClientSerial,&resultPacket,resultPacket.base.wSize);
		g_userPM.add(lpAsker->m_iClientSerial,&resultPacket,resultPacket.base.wSize);
		return;
	}

	SG_BEGIN_TRADE	beginPacket;

	beginPacket.base.set(sizeof(beginPacket),dSG_BEGIN_TRADE);
	beginPacket.wTradeBoxSerial	=	iTradeBoxSerial;
	beginPacket.wTargetLevel	=	lpAsker->m_iLevel;
	beginPacket.wTarget			=	lpAsker->m_wSerialInField;	
	strcpy(beginPacket.strTarget,lpAsker->m_strName);
	g_userPM.add(lpReplyer->m_iClientSerial,&beginPacket,beginPacket.base.wSize);

	beginPacket.wTargetLevel	=	lpReplyer->m_iLevel;
	beginPacket.wTarget			=	lpReplyer->m_wSerialInField;	
	strcpy(beginPacket.strTarget,lpReplyer->m_strName);
	g_userPM.add(lpAsker->m_iClientSerial,&beginPacket,beginPacket.base.wSize);
}	//	cFIELD::operateAcceptTradePacket(CG_TRADE_REPLY *_lpPacket,int _iSerial)


//
//	트레이드 박스 체크
BOOL
cFIELD::checkTradeBox(int _iSerial)
{
	if (!checkActor(_iSerial))	return	FALSE;

	cACTOR	*lpTrader	=	getTestedActor(_iSerial);

	if (!lpTrader	||	lpTrader->isIncorrectClient())	return	FALSE;

	CTradeBox			*lpTradeBox	=	m_tradeManager.getTradeBox(lpTrader);
	SG_TRADE_MESSAGE	resultPacket;

	resultPacket.base.set(sizeof(resultPacket),dSG_TRADE_MESSAGE);

	if (!lpTradeBox)
	{
		lpTrader->m_wTradeBoxSerial	=	0xffff;
		resultPacket.wMessage		=	eTM_TRADE_CANCEL_BY_INCORRECT_STATUS;
		g_userPM.add(lpTrader->m_iClientSerial,&resultPacket,resultPacket.base.wSize);	//	트레이드 중이 아니여..
		return	FALSE;
	}

	cACTOR	*lpMate		=	lpTradeBox->getTradeMate(lpTrader);

	if (!lpMate		||	lpMate->isIncorrectClient())
	{
		resultPacket.wMessage	=	eTM_TRADE_CANCEL_BY_INCORRECT_STATUS;
		g_userPM.add(lpTrader->m_iClientSerial,&resultPacket,resultPacket.base.wSize);	//	트레이드 중이 아니여..
		return	FALSE;
	}

	return	TRUE;
}	//	cFIELD::checkTradeBox(int _iSerial)

//
//	트레이드 취소
void
cFIELD::operateCancelTradePacket(CG_CANCEL_TRADE *_lpPacket,int _iSerial)
{
	if	(!checkActor(_iSerial))
		return;

	cACTOR		*lpTrader	=	getTestedActor(_iSerial);

	if	(!lpTrader	||	lpTrader->isIncorrectClient())
		return;

	CTradeBox	*lpTradeBox	=	m_tradeManager.getTradeBox(lpTrader);

	SG_TRADE_MESSAGE	packet;

	packet.base.set(sizeof(packet),dSG_TRADE_MESSAGE);

	packet.wMessage	=	eTM_TRADE_CANCEL_BY_ME;
	g_userPM.add(lpTrader->m_iClientSerial,&packet,packet.base.wSize);	//	트레이드 중이 아니여..

	lpTrader->m_wTradeBoxSerial	=	0xffff;

	if	(!lpTradeBox)
		return;

	cACTOR	*lpMate	=	lpTradeBox->getTradeMate(lpTrader);

	if	(lpMate	&&	lpMate->isCorrectClient())
	{
		packet.wMessage	=	eTM_TRADE_CANCEL_BY_TRADE_MATE;
		g_userPM.add(lpMate->m_iClientSerial,&packet,packet.base.wSize);	//	트레이드 중이 아니여..

		lpMate->m_wTradeBoxSerial	=	0xffff;
	}

	m_tradeManager.removeTradeBox(lpTradeBox->m_wSerial,lpTrader->m_strName);
}	//	cFIELD::operateCancelTradePacket(CG_CANCEL_TRADE *_lpPacket,int _iSerial)

//
//	트레이드 아이템 추가 패킷 처리
void
cFIELD::operateAddTradeItemPacket(CG_ADD_TRADE_ITEM *_lpPacket,int _iSerial)
{
	if (!checkTradeBox(_iSerial))	return;

	cACTOR		*lpTrader	=	getTestedActor(_iSerial);
	CTradeBox	*lpTradeBox	=	m_tradeManager.getTradeBox(lpTrader);
	cACTOR		*lpMate		=	lpTradeBox->getTradeMate(lpTrader);

	SG_TRADE_MESSAGE	resultPacket;

	resultPacket.base.set(sizeof(resultPacket),dSG_TRADE_MESSAGE);

	int	iResult	=	lpTradeBox->addTradeItem(lpTrader,_lpPacket->wItemSlot,_lpPacket->wCount);

	switch(iResult)
	{
		case	eRATI_ALREADY_ADDED_ITEM			:
		case	eRATI_VALID_ITEM					:
		case	eRATI_INCORRECT_TRADER				:
		case	eRATI_INCORRECT_TRADER_ITEM_COUNT	:
		case	eRATI_CAN_NOT_TRADE_ITEM			:
			resultPacket.wMessage		=	eTM_TRADE_CANCEL_BY_INCORRECT_STATUS;
			m_tradeManager.removeTradeBox(lpTrader->m_wTradeBoxSerial,lpTrader->m_strName);

			g_userPM.add(lpTrader->m_iClientSerial,&resultPacket,resultPacket.base.wSize);	//	먼가 잘못됐어!!
			g_userPM.add(lpMate->m_iClientSerial,&resultPacket,resultPacket.base.wSize);	//	먼가 잘못됐어!!
			return;

		case	eRATI_TRADE_BOX_FULL				:
			resultPacket.wMessage	=	eTM_TRADE_BOX_FULL;
			g_userPM.add(lpTrader->m_iClientSerial,&resultPacket,resultPacket.base.wSize);	//	트레이드 박스가 꽉 찼다.
			return;
			
		case	eRATI_CAN_NOT_OWN_SAME_ITEM			:
			resultPacket.wMessage	=	eTM_CAN_NOT_OWN_SAME_ITEM;
			g_userPM.add(lpTrader->m_iClientSerial,&resultPacket,resultPacket.base.wSize);	//	트레이드 박스가 꽉 찼다.
			return;

		case	eRATI_TOO_MANY_BADGE_ITEM			:
			resultPacket.wMessage	=	eTM_TOO_MANY_BADGE_ITEM;
			g_userPM.add(lpTrader->m_iClientSerial,&resultPacket,resultPacket.base.wSize);	//	트레이드 박스가 꽉 찼다.
			return;
		case	eRATI_TOO_MANY_GOLD_BAR			:
			resultPacket.wMessage	=	eTM_TARGET_GOLD_BAR_FULL;
			g_userPM.add(lpTrader->m_iClientSerial,&resultPacket,resultPacket.base.wSize);
			return;
	}

//	상대에게 정보 전송
	{
		SG_ADD_TRADE_ITEM_BY_TRADE_MATE	packet;

		packet.base.set(sizeof(SG_ADD_TRADE_ITEM_BY_TRADE_MATE),dSG_ADD_TRADE_ITEM_BY_TRADE_MATE);

		cItem	*lpItem	=	lpTrader->getInventoryItem(_lpPacket->wItemSlot);

		if (!lpItem)
		{
			resultPacket.wMessage		=	eTM_TRADE_CANCEL_BY_INCORRECT_STATUS;
			m_tradeManager.removeTradeBox(lpTrader->m_wTradeBoxSerial,lpTrader->m_strName);

			g_userPM.add(lpTrader->m_iClientSerial,&resultPacket,resultPacket.base.wSize);	//	먼가 잘못됐어!!
			g_userPM.add(lpMate->m_iClientSerial,&resultPacket,resultPacket.base.wSize);	//	먼가 잘못됐어!!
			return;
		}

		memcpy(&packet.item,lpItem,sizeof(cItem));
		
		if	(lpItem->isExceptionItem()	==	FALSE)
			packet.item.m_bCount	=	(BYTE)_lpPacket->wCount;

		g_userPM.add(lpMate->m_iClientSerial,&packet,packet.base.wSize);
	}

//	신청한넘에게 정보 전송
	{
		SG_ADD_TRADE_ITEM_BY_ME	packet;

		packet.base.set(sizeof(SG_ADD_TRADE_ITEM_BY_ME),dSG_ADD_TRADE_ITEM_BY_ME);

		packet.wSlot	=	_lpPacket->wItemSlot;
		packet.wCount	=	_lpPacket->wCount;
		g_userPM.add(lpTrader->m_iClientSerial,&packet,packet.base.wSize);
	}
}

//
//	트레이드 아이템 데이터 변경 처리
void
cFIELD::operateSetTradeItemPacket(CG_CHANGE_TRADE_ITEM_DATA *_lpPacket,int _iSerial)
{
	if (!checkTradeBox(_iSerial))	return;

	cACTOR		*lpTrader	=	getTestedActor(_iSerial);
	CTradeBox	*lpTradeBox	=	m_tradeManager.getTradeBox(lpTrader);
	cACTOR		*lpMate		=	lpTradeBox->getTradeMate(lpTrader);

	SG_TRADE_MESSAGE	resultPacket;

	resultPacket.base.set(sizeof(resultPacket),dSG_TRADE_MESSAGE);

	int	iResult	=	lpTradeBox->changeTradeItemData(lpTrader,_lpPacket->wTradeSlot,_lpPacket->wItemSlot,_lpPacket->wCount);

	switch(iResult)
	{
		case	eRATI_VALID_ITEM					:
		case	eRATI_INCORRECT_TRADER				:
		case	eRATI_INCORRECT_TRADE_SLOT			:
		case	eRATI_INCORRECT_TRADER_ITEM_COUNT	:
			resultPacket.wMessage	=	eTM_TRADE_CANCEL_BY_INCORRECT_STATUS;
			m_tradeManager.removeTradeBox(lpTrader->m_wTradeBoxSerial,lpTrader->m_strName);

			g_userPM.add(lpMate->m_iClientSerial,&resultPacket,resultPacket.base.wSize);	//	먼가 잘못됐어!!
			g_userPM.add(lpTrader->m_iClientSerial,&resultPacket,resultPacket.base.wSize);	//	먼가 잘못됐어!!
			return;

		case	eRATI_TRADE_BOX_FULL	:
			resultPacket.wMessage	=	eTM_TRADE_BOX_FULL;
			g_userPM.add(lpTrader->m_iClientSerial,&resultPacket,resultPacket.base.wSize);	//	트레이드 박스가 꽉 찼다.
			return;
	}

//	상대에게 정보 전송
	{
		SG_CHANGE_TRADE_ITEM_DATA_BY_TRADE_MATE	packet;

		packet.base.set(sizeof(SG_CHANGE_TRADE_ITEM_DATA_BY_TRADE_MATE),dSG_CHANGE_TRADE_ITEM_DATA_BY_TRADE_MATE);

		cItem	*lpItem	=	lpTrader->getInventoryItem(_lpPacket->wItemSlot);

		if (!lpItem)
		{
			resultPacket.wMessage	=	eTM_TRADE_CANCEL_BY_INCORRECT_STATUS;
			m_tradeManager.removeTradeBox(lpTrader->m_wTradeBoxSerial,lpTrader->m_strName);

			g_userPM.add(lpMate->m_iClientSerial,&resultPacket,resultPacket.base.wSize);	//	먼가 잘못됐어!!
			g_userPM.add(lpTrader->m_iClientSerial,&resultPacket,resultPacket.base.wSize);	//	먼가 잘못됐어!!
			return;
		}

		memcpy(&packet.item,lpItem,sizeof(cItem));

		if	(lpItem->isExceptionItem()	==	FALSE)
			packet.item.m_bCount	=	(BYTE)_lpPacket->wCount;

		packet.wTradeSlot		=	_lpPacket->wTradeSlot;

		g_userPM.add(lpMate->m_iClientSerial,&packet,packet.base.wSize);
	}

//	신청한넘에게 정보 전송
	{
		SG_CHANGE_TRADE_ITEM_DATA_BY_ME	packet;

		packet.base.set(sizeof(SG_CHANGE_TRADE_ITEM_DATA_BY_ME),dSG_CHANGE_TRADE_ITEM_DATA_BY_ME);

		packet.wSlot			=	_lpPacket->wItemSlot;
		packet.wCount			=	_lpPacket->wCount;
		packet.wTradeSlot		=	_lpPacket->wTradeSlot;
		g_userPM.add(lpTrader->m_iClientSerial,&packet,packet.base.wSize);
	}
}

//
//
void
cFIELD::operateRemoveTradeItemPacket(CG_REMOVE_TRADE_ITEM *_lpPacket,int _iSerial)
{
	if (!checkTradeBox(_iSerial))	return;

	cACTOR		*lpTrader	=	getTestedActor(_iSerial);
	CTradeBox	*lpTradeBox	=	m_tradeManager.getTradeBox(lpTrader);
	cACTOR		*lpMate		=	lpTradeBox->getTradeMate(lpTrader);

	SG_TRADE_MESSAGE	resultPacket;

	resultPacket.base.set(sizeof(resultPacket),dSG_TRADE_MESSAGE);

	int	iResult	=	lpTradeBox->removeTradeItem(lpTrader,_lpPacket->wTradeSlot);

	switch(iResult)
	{
		case	eRRTI_INCORRECT_TRADE_BOX_SLOT	:
		case	eRRTI_VALID_TRADE_BOX_SLOT		:
			resultPacket.wMessage	=	eTM_INCORRECT_TRADE_ITEM_COUNT;
			g_userPM.add(lpTrader->m_iClientSerial,&resultPacket,resultPacket.base.wSize);	//	아이템 카운트가 이상하다.
			break;

		case	eRRTI_INCORRECT_TRADER			:
			resultPacket.wMessage	=	eTM_TRADE_CANCEL_BY_INCORRECT_STATUS;
			m_tradeManager.removeTradeBox(lpTrader->m_wTradeBoxSerial,lpTrader->m_strName);

			g_userPM.add(lpTrader->m_iClientSerial,&resultPacket,resultPacket.base.wSize);	//	먼가 잘못됐어!!
			g_userPM.add(lpMate->m_iClientSerial,&resultPacket,resultPacket.base.wSize);	//	먼가 잘못됐어!!
			return;
	}


	SG_REMOVE_TRADE_ITEM	packet;

	packet.base.set(sizeof(SG_REMOVE_TRADE_ITEM),dSG_REMOVE_TRADE_ITEM);

	packet.wOwner		=	lpTrader->m_wSerialInField;
	packet.wSlotIndex	=	_lpPacket->wTradeSlot;

	g_userPM.add(lpTrader->m_iClientSerial,&packet,packet.base.wSize);	//	
	g_userPM.add(lpMate->m_iClientSerial,&packet,packet.base.wSize);		//	
}

//
//
void
cFIELD::operateSetTradeGoldPacket(CG_SET_TRADE_GOLD *_lpPacket,int _iSerial)
{
	if	(!checkTradeBox(_iSerial))
		return;
	
	if	(_lpPacket->iGold	<	0)
		return;

	cACTOR		*lpTrader	=	getTestedActor(_iSerial);
	CTradeBox	*lpTradeBox	=	m_tradeManager.getTradeBox(lpTrader);
	cACTOR		*lpMate		=	lpTradeBox->getTradeMate(lpTrader);

	SG_TRADE_MESSAGE	resultPacket;
	resultPacket.base.set(sizeof(resultPacket),dSG_TRADE_MESSAGE);

	int	iResult	=	lpTradeBox->setTradeGold(lpTrader,_lpPacket->iGold);

	switch(iResult)
	{
		case	eRSTG_INCORRECT_TRADER	:
			resultPacket.wMessage	=	eTM_INCORRECT_TRADE_ITEM_COUNT;
			g_userPM.add(lpTrader->m_iClientSerial,&resultPacket,resultPacket.base.wSize);	//	아이템 카운트가 이상하다.
			return;

		case	eRSTG_LOW_GOLD			:
			resultPacket.wMessage	=	eTM_LOW_GOLD;
			g_userPM.add(lpTrader->m_iClientSerial,&resultPacket,resultPacket.base.wSize);	//	아이템 카운트가 이상하다.
			return;
			
		case	eRSTG_TARGET_GOLD_FULL	:
			resultPacket.wMessage	=	eTM_TARGET_GOLD_FULL;
			g_userPM.add(lpTrader->m_iClientSerial,&resultPacket,resultPacket.base.wSize);	//	아이템 카운트가 이상하다.
			return;
	}

	SG_SET_TRADE_GOLD	packet;

	packet.base.set(sizeof(SG_SET_TRADE_GOLD),dSG_SET_TRADE_GOLD);
	packet.wOwner	=	lpTrader->m_wSerialInField;
	packet.iGold	=	_lpPacket->iGold;

	g_userPM.add(lpTrader->m_iClientSerial,&packet,packet.base.wSize);	//	
	g_userPM.add(lpMate->m_iClientSerial,&packet,packet.base.wSize);		//	
}

//
//	트레이드 준비!! 패킷 처리
void
cFIELD::operateReadyTradePacket(CG_READY_TRADE *_lpPacket,int _iSerial)
{
	if (!checkTradeBox(_iSerial))	return;

	cACTOR		*lpTrader	=	getTestedActor(_iSerial);
	CTradeBox	*lpTradeBox	=	m_tradeManager.getTradeBox(lpTrader);
	cACTOR		*lpMate		=	lpTradeBox->getTradeMate(lpTrader);

	if (lpTradeBox->readyTrade(lpTrader))
	{
		SG_READY_TRADE	packet;
		packet.base.set(sizeof(SG_READY_TRADE),dSG_READY_TRADE);
		packet.wTrader	=	lpTrader->m_wSerialInField;

		g_userPM.add(lpTrader->m_iClientSerial,&packet,packet.base.wSize);	//
		g_userPM.add(lpMate->m_iClientSerial,&packet,packet.base.wSize);		//
	}
}

//
//	트레이드 허락 패킷 처리
void
cFIELD::operatePermitTradePacket(CG_PERMIT_TRADE *_lpPacket,int _iSerial)
{
	if (!checkTradeBox(_iSerial))	return;

	cACTOR		*lpTrader	=	getTestedActor(_iSerial);
	CTradeBox	*lpTradeBox	=	m_tradeManager.getTradeBox(lpTrader);
	cACTOR		*lpMate		=	lpTradeBox->getTradeMate(lpTrader);

	int			iCheckSum	=	lpTradeBox->getCheckSum(lpTrader);

	SG_TRADE_MESSAGE	resultPacket;
	resultPacket.base.set(sizeof(resultPacket),dSG_TRADE_MESSAGE);
	
	if (iCheckSum	!=	_lpPacket->iCheckSum)
	{
		resultPacket.wMessage	=	eTM_TRADE_CANCEL_BY_INCORRECT_STATUS;
		m_tradeManager.removeTradeBox(lpTrader->m_wTradeBoxSerial,lpTrader->m_strName);

		g_userPM.add(lpTrader->m_iClientSerial,&resultPacket,resultPacket.base.wSize);	//	
		g_userPM.add(lpMate->m_iClientSerial,&resultPacket,resultPacket.base.wSize);		//	
		return;
	}

	if	(lpTradeBox->permitTrade(lpTrader))
	{
		if (lpTradeBox->isReadyToTrade())
		{
			int	iResult	=	lpTradeBox->isTradeAble(lpTrader);

			if (iResult ==	eRT_OK)
			{
				if (lpTradeBox->check())
				{
					SG_TRADE	packet;

					packet.wFirstWorkTrader	=	lpTrader->m_wSerialInField;

					lpTradeBox->getTradePacketInfo(lpTrader,&packet);
					g_userPM.add(lpTrader->m_iClientSerial,&packet,packet.base.wSize);	//

					lpTradeBox->getTradePacketInfo(lpMate,&packet);
					g_userPM.add(lpMate->m_iClientSerial,&packet,packet.base.wSize);		//

					lpTradeBox->trade(lpTrader);
				}
				else
					lpTradeBox->sendTradeCancelMessage();

				m_tradeManager.removeTradeBox(lpTradeBox->m_wSerial,lpTrader->m_strName);
			}
			else
			{
				switch(iResult)
				{
					case	eRT_LOW_SLOT		:
						resultPacket.wMessage	=	eTM_LOW_EMPTY_INVENTORY_SLOT_COUNT;
						break;

					case	eRT_INCORRECT_TRADER	:
						m_tradeManager.removeTradeBox(lpTrader->m_wTradeBoxSerial,lpTrader->m_strName);
						resultPacket.wMessage	=	eTM_TRADE_CANCEL_BY_INCORRECT_STATUS;
						break;
				}

				g_userPM.add(lpTrader->m_iClientSerial,&resultPacket,resultPacket.base.wSize);	//	
				g_userPM.add(lpMate->m_iClientSerial,&resultPacket,resultPacket.base.wSize);		//	
			}
		}
		else
		{
			SG_PERMIT_TRADE	packet;
			packet.base.set(sizeof(SG_PERMIT_TRADE),dSG_PERMIT_TRADE);
			packet.wTrader	=	lpTrader->m_wSerialInField;

			g_userPM.add(lpTrader->m_iClientSerial,&packet,packet.base.wSize);	//
			g_userPM.add(lpMate->m_iClientSerial,&packet,packet.base.wSize);		//
		}
	}
}

