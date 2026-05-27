#include "CAgent.H"
#include "CGamePlay.H"
#include "CHero.H"
#include "CMessage.H"

//
//이벤트 한다.
BOOL
CAgent::sendEventToTarget(int _iTarget)
{
	CG_SELECT_NPC	packet;

	packet.base.set(sizeof(CG_SELECT_NPC),dCG_SELECT_NPC);
	packet.wNpcSerial	=	_iTarget;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	특정 대사를 선택했다.
BOOL
CAgent::sendSelectSpeech(int _iSpeaker,int _iSpeechIndex,int _iSelectSpeech,int _iEventArea)
{
	CG_SELECT_SPEECH	packet;

	packet.base.set(sizeof(CG_SELECT_SPEECH),dCG_SELECT_SPEECH);
	packet.wSpeaker		=	_iSpeaker;
	packet.wSpeechIndex	=	_iSpeechIndex;
	packet.wSelectSpeech=	_iSelectSpeech;
	packet.wEventArea	=	_iEventArea;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	아이템 구입
BOOL
CAgent::sendBuyItem(int _iShopeSerial,int _iOwner,int _iSelectItem,BOOL _bIsGuildShop,int _iCount , BOOL _bIsTokenShop, BOOL _bIsBadgeOfTraderShop)
{
	if (m_bIsWaitTradeItemResult)
	{
		CGamePlay::AddSystemMessage(WHITE,dMSG_CAN_NOT_BUY_ITEM_THIS_TIME);
		return	FALSE;
	}

	m_bIsWaitTradeItemResult	=	TRUE;

	CG_BUY_ITEM		packet;

	packet.base.set(sizeof(CG_BUY_ITEM),dCG_BUY_ITEM);

	packet.wShopSerial				=	_iShopeSerial;
	packet.wSelectItem				=	_iSelectItem;
	packet.wOwner					=	_iOwner;
	packet.wIsGuildShop				=	_bIsGuildShop;
	packet.wBuyCount				=	_iCount;
	packet.wIsTokenShop			=	_bIsTokenShop;
	packet.bf1IsBadgeOfTraderShop	=	_bIsBadgeOfTraderShop; // 거래상인의 배지 상점.

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	아이템 판매
BOOL
CAgent::sendSellItem(int _iShopSerial,int _iShopKeeper,int _iSellItemSlot,int _iSellItemCount, BOOL _bIsBadgeOfTraderShop)
{
	if (m_bIsWaitTradeItemResult)
	{
		CGamePlay::AddSystemMessage(WHITE,dMSG_CAN_NOT_SELL_ITEM_THIS_TIME);
		return	FALSE;
	}

	m_bIsWaitTradeItemResult=	s_iFrameCounter;

	CG_SELL_ITEM	packet;

	packet.base.set(sizeof(CG_SELL_ITEM),dCG_SELL_ITEM);

	packet.wShopKeeper		=	_iShopKeeper;
	packet.wShopSerial		=	_iShopSerial;
	packet.wSellItemSlot	=	_iSellItemSlot;
	packet.wCount			=	_iSellItemCount;
	packet.wCheckSum		=	g_hero.getItemCheckSum();
	packet.wBaseItem		=	g_hero.m_aItems[_iSellItemSlot].m_wBaseItem;//	베이스 아이템
	packet.bf1IsBadgeOfTraderShop	=	_bIsBadgeOfTraderShop;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	퀘스트 취소
BOOL
CAgent::sendCancelQuest(int _iSelectQuest)
{
	CG_CANCEL_QUEST	packet;

	packet.base.set(sizeof(packet),dCG_CANCEL_QUEST);

	tsProcessQuestField	*lpQuestInfo	=	g_hero.getQuestInfo(_iSelectQuest);

	if (!lpQuestInfo)	return	FALSE;

	packet.wQuestIndex	=	lpQuestInfo->questIndex;
	packet.wQuestSlot	=	_iSelectQuest;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendBuyCarrotShopItem(int _iCategory,int _iItemIndex)
{
	CG_BUY_CARROT_SHOP_ITEM	packet;

	packet.base.set(sizeof(packet),dCG_BUY_CARROT_SHOP_ITEM);

	packet.wCategory	=	_iCategory;
	packet.wItemIndex	=	_iItemIndex;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendChangeDoorStatus(int _iArea,int _iMethod)
{
	CG_CHANGE_DOOR_STATUS	packet;

	packet.base.set(sizeof(packet),dCG_CHANGE_DOOR_STATUS);

	packet.wArea		=	_iArea;
	packet.wMethod		=	_iMethod;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendAskDoorInfo(int _iArea)
{
	CG_ASK_DOOR_INFO	packet;

	packet.base.set(sizeof(packet),dCG_ASK_DOOR_INFO);

	packet.wDoorArea		=	_iArea;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendTouchEventArea(int _iArea)
{
	CG_TOUCH_EVENT_AREA	packet;
	
	packet.base.set(sizeof(packet),dCG_TOUCH_EVENT_AREA);
	packet.wArea	=	_iArea;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendArcaWork(int _iArca,int _iWork)
{
	CG_ARCA_WORK	packet;

	packet.base.set(sizeof(packet),dCG_ARCA_WORK);
	packet.wArca	=	_iArca;
	packet.wWork	=	_iWork;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendDisarmFieldTrap(int _iTrap)
{
	CG_DISARM_FIELD_TRAP	packet;

	packet.base.set(sizeof(packet),dCG_DISARM_FIELD_TRAP);
	packet.wFieldTrap	=	_iTrap;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendAskArcaInfo(int _iCount,WORD *_lpArcaIndex)
{
	if (_iCount == 0)
		return	FALSE;

	CG_ASK_ARCA_INFO	packet;
	
	packet.base.set(sizeof(packet),dCG_ASK_ARCA_INFO);

	memcpy(packet.awArcaIndex,_lpArcaIndex,sizeof(WORD)*_iCount);
	packet.wCount			=	_iCount;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendPutBanner(int _iX,int _iY,int _iItemSlot,char *_lpstrTitle,char *_lpstrText)
{
	CG_PUT_BANNER		packet;

	packet.base.set(sizeof(packet),dCG_PUT_BANNER);

	packet.wX				=	_iX;
	packet.wY				=	_iY;
	packet.wBannerItemSlot	=	_iItemSlot;

	strcpy(packet.strTitle,_lpstrTitle);
	strcpy(packet.strComment,_lpstrText);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendAskBannerText(int _iSerial)
{
	CG_ASK_BANNER_TEXT	packet;

	packet.base.set(sizeof(packet),dCG_ASK_BANNER_TEXT);

	packet.wBannerSerial	=	_iSerial;

	return	sendPacket((char *)&packet,packet.base.wSize);
}