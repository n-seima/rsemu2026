#include "CAgent.H"
#include "CActor.H"
#include "CHero.H"

//
//	트레이드 요청
BOOL
CAgent::sendRequestTrade(char *_lpstrName,WORD _wTargetSerial)
{
	CG_REQUEST_TRADE	packet;

	packet.base.set(sizeof(CG_REQUEST_TRADE),dCG_REQUEST_TRADE);
	packet.wTradeTarget	=	_wTargetSerial;
	strcpy(packet.strTargetName,_lpstrName);

	return	sendPacket((char *)&packet,packet.base.wSize);
}	//	CAgent::sendRequestTrade(char *_lpstrName)

//
//	트레이드 응답
BOOL
CAgent::sendTradeReply(char *_lpstrName,WORD _wReply)
{
	CG_TRADE_REPLY			packet;

	packet.base.set(sizeof(CG_TRADE_REPLY),dCG_TRADE_REPLY);
	strcpy(packet.strAsker,_lpstrName);
	packet.wReply	=	_wReply;

	return	sendPacket((char *)&packet,packet.base.wSize);
}	//	CAgent::sendTradeReply(char *_lpstrName,WORD _wReply)

//
//	트레이드 취소
BOOL
CAgent::sendCancelTrade()	//	트레이드 취소
{
	CG_CANCEL_TRADE			packet;

	packet.base.set(sizeof(CG_CANCEL_TRADE),dCG_CANCEL_TRADE);

	return	sendPacket((char *)&packet,packet.base.wSize);
}	//	CAgent::sendCancelTrade()

//
//	트레이드 아이템 추가
BOOL
CAgent::sendAddTradeItem(int _iItemSlot,int _iItemCount)	
{
	CG_ADD_TRADE_ITEM		packet;

	packet.base.set(sizeof(CG_ADD_TRADE_ITEM),dCG_ADD_TRADE_ITEM);

	packet.wItemSlot=	_iItemSlot;
	packet.wCount	=	_iItemCount;	//	몇개?

	return	sendPacket((char *)&packet,packet.base.wSize);
}	//	CAgent::sendAddTradeItem(int _iItemSlot,int _iItemCount)	

//
//	트레이드 아이템 정보 변경
BOOL
CAgent::sendChangeTradeItemData(int _iChangeTradeSlot,int _iItemSlot,int _iItemCount)
{
	CG_CHANGE_TRADE_ITEM_DATA	packet;
	packet.base.set(sizeof(CG_CHANGE_TRADE_ITEM_DATA),dCG_CHANGE_TRADE_ITEM_DATA);

	packet.wTradeSlot	=	_iChangeTradeSlot;	//	몇개?
	packet.wItemSlot	=	_iItemSlot;
	packet.wCount		=	_iItemCount;	//	몇개?

	return	sendPacket((char *)&packet,packet.base.wSize);
}	//	CAgent::sendChangeTradeItemData(int _iChangeTradeSlot,int _iItemSlot,int _iItemCount)

//
//	트레이드 아이템 제거
BOOL
CAgent::sendRemoveTradeItem(int _iItemSlot)
{
	CG_REMOVE_TRADE_ITEM	packet;

	packet.base.set(sizeof(CG_REMOVE_TRADE_ITEM),dCG_REMOVE_TRADE_ITEM);
	packet.wTradeSlot	=	_iItemSlot;

	return	sendPacket((char *)&packet,packet.base.wSize);
}	//	CAgent::sendRemoveTradeItem(int _iItemSlot)

//
//	트레이드 할 골드 설정
BOOL
CAgent::sendSetTradeGold(int _iGold)
{
	CG_SET_TRADE_GOLD		packet;

	packet.base.set(sizeof(CG_SET_TRADE_GOLD),dCG_SET_TRADE_GOLD);
	packet.iGold	=	_iGold;

	return	sendPacket((char *)&packet,packet.base.wSize);
}	//	CAgent::sendSetTradeGold(int _iGold)

//
//	트레이드 준비
BOOL
CAgent::sendReadyTrade()
{
	CG_READY_TRADE			packet;

	packet.base.set(sizeof(CG_READY_TRADE),dCG_READY_TRADE);

	return	sendPacket((char *)&packet,packet.base.wSize);
}	//	CAgent::sendReadyTrade()

//
//	트레이드 허락
BOOL
CAgent::sendPermitTrade(int _iCheckSum)
{
	CG_PERMIT_TRADE			packet;

	packet.base.set(sizeof(CG_PERMIT_TRADE),dCG_PERMIT_TRADE);
	packet.iCheckSum	=	_iCheckSum;

	return	sendPacket((char *)&packet,packet.base.wSize);
}	//	CAgent::sendPermitTrade()