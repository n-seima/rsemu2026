#include "CAgent.H"
#include "CActor.H"
#include "CGamePlay.H"
#include "CSkill.H"
#include "cMESSAGE.H"
//#include "defineWarningMessage.H"

void
cPACKET_HANDLER::ReceiveRequestTrade(SERVER_PACKETS *_lpPacket)
{
	CGamePlay::RequestTrade(&_lpPacket->mSG_REQUEST_TRADE);
}	//	cPACKET_HANDLER::ReceiveRequestTrade(SERVER_PACKETS *_lpPacket)

void
cPACKET_HANDLER::ReceiveRequestTradeResult(SERVER_PACKETS *_lpPacket)
{
	SG_REQUEST_TRADE_RESULT	*lpPacket	=	&_lpPacket->mSG_REQUEST_TRADE_RESULT;

	switch (lpPacket->wReply)
	{
		case	eRRT_CAN_NOT_TRADE_STATE	:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_RRT_CAN_NOT_TRADE_STATE);
			break;
		case	eRRT_DENY				:	//	거절했다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_TM_DENY_TRADE);
			break;
		case	eRRT_TOO_FAR			:	//	너무 멀다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_TM_TOO_FAR_TARGET);
			break;
		case	eRRT_NOT_EXIST_TARGET	:	//	그런넘 없다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_TM_NOT_EXIST);
			break;
		case	eRRT_LOW_LEVEL			:	//	거래 최소 레벨보다 낮다.
			g_msgBox.cPopup("",dMSG_TM_MINUMUM_LEVEL,dMSG_OK);
			break;
		case	eRRT_ALREADY_TRADE		:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_TM_ALREADY_TRADE);
			break;
		case	eRRT_DEATH_CHARACTER	:	//	딴넘하고 트레이드 중이다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_TM_DEATH_CHARACTER);
			break;
		case	eRRT_TARGET_DENY_COMMUNITY_STATUS	:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_TARGET_DENY_COMMUNITY_STATUS);
			break;
		case	eRRT_TOO_MANY_TRADE_BOX				:	//	너무 많은 거래가 이루어 지고 있어요오.. -o-
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_TM_TOO_MANY_TRADE);
			break;
		case	eRRT_CAN_NOT_ASK_TRADE_TO_OPERATOR	:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_TM_CAN_NOT_ASK_TRADE_TO_OPERATOR);
			break;
	}
}

//
//	트레이드 시작 메시지 받음
void
cPACKET_HANDLER::ReceiveBeginTrade(SERVER_PACKETS *_lpPacket)
{
	SG_BEGIN_TRADE	*lpPacket	=	&_lpPacket->mSG_BEGIN_TRADE;

	CActor	*lpActor	=	g_am.getTestedActor(lpPacket->wTarget);

	if (!lpActor)
	{
		s_agent.sendCancelTrade();
		return;
	}
	if (STRICMP(lpActor->m_strName,lpPacket->strTarget)	!=	0)
	{
		s_agent.sendCancelTrade();
		return;
	}

	CGamePlay::OpenTradeBox(lpActor,lpPacket->wTradeBoxSerial,lpPacket->wTargetLevel);
}

void
cPACKET_HANDLER::ReceiveAddTradeItemByMe(SERVER_PACKETS *_lpPacket)
{
	SG_ADD_TRADE_ITEM_BY_ME	*lpPacket	=	&_lpPacket->mSG_ADD_TRADE_ITEM_BY_ME;

	CGamePlay::AddTradeItemByMe(lpPacket->wSlot,lpPacket->wCount);
}
	
void
cPACKET_HANDLER::ReceiveAddTradeItemByTradeMate(SERVER_PACKETS *_lpPacket)
{
	SG_ADD_TRADE_ITEM_BY_TRADE_MATE	*lpPacket	=	&_lpPacket->mSG_ADD_TRADE_ITEM_BY_TRADE_MATE;

	CGamePlay::AddTradeItemByTradeMate((cItem *)&lpPacket->item);
}

//
//	트레이드 아이템 변경
void
cPACKET_HANDLER::ReceiveChangeTradeItemDataByMe(SERVER_PACKETS *_lpPacket)
{
	SG_CHANGE_TRADE_ITEM_DATA_BY_ME	*lpPacket	=	&_lpPacket->mSG_CHANGE_TRADE_ITEM_DATA_BY_ME;

	CGamePlay::ChangeTradeItemDataByMe(lpPacket->wTradeSlot,lpPacket->wSlot,lpPacket->wCount);
}

//
//	트레이드 아이템 변경
void
cPACKET_HANDLER::ReceiveChangeTradeItemDataByTradeMate(SERVER_PACKETS *_lpPacket)
{
	SG_CHANGE_TRADE_ITEM_DATA_BY_TRADE_MATE	*lpPacket	=	&_lpPacket->mSG_CHANGE_TRADE_ITEM_DATA_BY_TRADE_MATE;

	CGamePlay::ChangeTradeItemDataByTradeMate(lpPacket->wTradeSlot,(cItem *)&lpPacket->item);
}
	
void
cPACKET_HANDLER::ReceiveSetTradeGold(SERVER_PACKETS *_lpPacket)
{
	SG_SET_TRADE_GOLD	*lpPacket	=	&_lpPacket->mSG_SET_TRADE_GOLD;

	CGamePlay::SetTradeGold(lpPacket->wOwner,lpPacket->iGold);
}
	
void
cPACKET_HANDLER::ReceiveRemoveTradeItem(SERVER_PACKETS *_lpPacket)
{
	SG_REMOVE_TRADE_ITEM	*lpPacket	=	&_lpPacket->mSG_REMOVE_TRADE_ITEM;

	CGamePlay::RemoveTradeItem(lpPacket->wOwner,lpPacket->wSlotIndex);
}
	
void
cPACKET_HANDLER::ReceiveReadyTrade(SERVER_PACKETS *_lpPacket)
{
	SG_READY_TRADE	*lpPacket	=	&_lpPacket->mSG_READY_TRADE;

	CGamePlay::ReadyTrade(lpPacket->wTrader);
}
	
void
cPACKET_HANDLER::ReceivePermitTrade(SERVER_PACKETS *_lpPacket)
{
	SG_PERMIT_TRADE	*lpPacket	=	&_lpPacket->mSG_PERMIT_TRADE;
	
	CGamePlay::PermitTrade(lpPacket->wTrader);
}
	
void
cPACKET_HANDLER::ReceiveTrade(SERVER_PACKETS *_lpPacket)
{
	SG_TRADE	*lpPacket	=	&_lpPacket->mSG_TRADE;

	CGamePlay::Trade(lpPacket);
}

void
cPACKET_HANDLER::ReceiveTradeMessage(SERVER_PACKETS *_lpPacket)
{
	SG_TRADE_MESSAGE	*lpPacket	=	&_lpPacket->mSG_TRADE_MESSAGE;

	switch(lpPacket->wMessage)
	{
		case	eTM_TRADE_CANCEL_BY_TRADE_MATE				:	//	트레이드 상대가 캔슬 했다.
		case	eTM_TRADE_CANCEL_BY_ME						:	//	내가 캔슬했다.
			CGamePlay::CloseTradeBox();
			g_lpHero->warning(dMSG_TM_CANCEL);
			break;

		case	eTM_TRADE_CANCEL_BY_INVENTORY_DATA_CHANGE	:	//	인벤토리에 변화가 있어서 캔슬되었다.
			CGamePlay::CloseTradeBox();
			g_lpHero->warning(dMSG_TM_CANCEL_BY_INVENTORY_DATA_CHANGE);
			break;

		case	eTM_TRADE_CANCEL_BY_INCORRECT_STATUS		:	//	뭔가 이상해서 캔슬 되었다.
			CGamePlay::CloseTradeBox();
			g_lpHero->warning(dMSG_TM_CANCEL_BY_INCORRECT_STATUS);
			break;

		case	eTM_TARGET_GOLD_FULL						:
			g_lpHero->warning(dMSG_TM_TARGET_GOLD_FULL);
			break;

		case	eTM_CAN_NOT_OWN_SAME_ITEM					:
			g_msgBox.cPopup("",dMSG_CAN_NOT_OWN_SAME_ITEM,dMSG_OK);
			break;

		case	eTM_TOO_MANY_BADGE_ITEM				:
			g_msgBox.cPopup("",_ms(dMSG_TOO_MANY_BADGE_ITEM_FORM,dLIMIT_BADGE_ITEM_COUNT_IN_INVENTORY),dMSG_OK);
			break;

		case	eTM_TARGET_INVENTORY_FULL					:	//	막상 교환 하려니까 상대방 인벤토리에 빈자리가 없다.
			break;
		case	eTM_OWNER_INVENTORY_FULL					:	//	막상 교환 하려니까 내 인벤토리에 빈자리가 없다.
			break;
		case	eTM_VALID_INVENTORY_ITEM					:	//	빈 인벤토리 아이템이다.
			break;
		case	eTM_ALREADY_ADDED_INVENTORY_ITEM			:	//	이미 출가된 인벤토 아이템이다.
			break;
		case	eTM_NOT_TRADE_STATUS						:	//	트레이드 중이 아닌디..
			break;
		case	eTM_INCORRECT_TRADE_SLOT					:	//	잘못된 트레이드 슬롯 이다.
			break;
		case	eTM_TRADE_BOX_FULL							:	//	트레이드 박스가 꽉 찼다.
			break;
		case	eTM_INCORRECT_TRADE_ITEM_COUNT				:	//	트레이드할 아이템의 수가 이상하다.
			break;
		case	eTM_INCORRECT_TRADE_ITEM_SLOT				:	//	잘못된 트레이드 아이템 슬롯
			break;
		case	eTM_LOW_GOLD								:	//	돈이 모자라는디요?
			break;

		case	eTM_LOW_EMPTY_INVENTORY_SLOT_COUNT			:
			{
				g_lpHero->warning(dMSG_TM_LOW_EMPTY_INVENTORY_SLOT_COUNT);
				break;
			}
			
		case	eTM_TARGET_GOLD_BAR_FULL:
			{
				g_lpHero->warning(dMSG_TARGET_GOLD_BAR_FULL);
				break;				
			}

	}
}