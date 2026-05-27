#include "CAgent.H"
#include "CActor.H"
#include "CGamePlay.H"
#include "CWindowInterface.H"


BOOL
CAgent::sendAskOpenThePitchmanShop()	//	노점상 열거라고 요청
{
	CG_ASK_OPEN_THE_PITCHMAN_SHOP		packet;

	packet.base.set(sizeof(packet),dCG_ASK_OPEN_THE_PITCHMAN_SHOP);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//	노점상 아이템 추가
BOOL
CAgent::sendAddPitchmanShopItem(int _iSerial,int _iInventorySlot,int _iShopSlot,int _iPrice, BYTE _bDenomination)		//09.08.21
{
	CG_ADD_PITCHMAN_SHOP_ITEM			packet;

	packet.base.set(sizeof(packet),dCG_ADD_PITCHMAN_SHOP_ITEM);
	packet.wSerial			=	_iSerial;
	packet.wInventorySlot	=	_iInventorySlot;
	packet.wShopSlot		=	_iShopSlot;
	packet.dwPrice			=	_iPrice;
	packet.bDenomination	=	_bDenomination;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//	노점상에서 아이템 제거
BOOL
CAgent::sendRemovePitchmanShopItem(int _iSerial,int _iSlot)
{
	CG_REMOVE_PITCHMAN_SHOP_ITEM		packet;

	packet.base.set(sizeof(packet),dCG_REMOVE_PITCHMAN_SHOP_ITEM);
	packet.wSlot	=	_iSlot;
	packet.wSerial	=	_iSerial;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//	노점상 상태 변경 (오픈,클로즈,종료)
BOOL
CAgent::sendChangePitchmanShopInfo(int _iSerial,int _iStatus,char *_lpstrName,int _iSignboardShape,WORD _wTextColor,BOOL _bIsBold,BOOL _bIsShadowText)
{
	CG_CHANGE_PITCHMAN_SHOP_INFO		packet;

	packet.base.set(sizeof(packet),dCG_CHANGE_PITCHMAN_SHOP_INFO);

	packet.wSerial			=	_iSerial;
	packet.bf2Status		=	_iStatus;
	packet.bf10SignboardShape=	_iSignboardShape;
	packet.wTextColor		=	_wTextColor;
	packet.bf1IsBold		=	_bIsBold;
	packet.bf1IsShadowText	=	_bIsShadowText;

	ustrncpy(packet.strName,_lpstrName,sizeof(packet.strName)-2);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//	노점상 아이템 위치 변경
BOOL
CAgent::sendChangePitchmanShopItemPlace(int _iSerial,int _iSlot1,int _iSlot2)
{
	CG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE	packet;

	packet.base.set(sizeof(packet),dCG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE);

	packet.wSerial			=	_iSerial;
	packet.wSlot1			=	_iSlot1;
	packet.wSlot2			=	_iSlot2;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//	노점상에서 아이템 구입
BOOL
CAgent::sendBuyPitchmanShopItem(int _iSerial,int _iSlot,cItem *_lpItem,DWORD _dwPrice, BYTE _bDenomination)		// 09.08.21
{
	CG_BUY_PITCHMAN_SHOP_ITEM			packet;

	packet.base.set(sizeof(packet),dCG_BUY_PITCHMAN_SHOP_ITEM);
	packet.wPitchmanSerial	=	_iSerial;
	packet.wSlot			=	_iSlot;
	packet.dwPrice			=	_dwPrice;
	packet.bDenomination	=	_bDenomination;
	memcpy(&packet.item,_lpItem,sizeof(cItem));

	return	sendPacket((char *)&packet,packet.base.wSize);
}
//	노점상 정보 요청
BOOL
CAgent::sendAskPitchmanShopInfo(int _iSerial)
{
	CG_ASK_PITCHMAN_SHOP_INFO			packet;

	packet.base.set(sizeof(packet),dCG_ASK_PITCHMAN_SHOP_INFO);
	packet.wPitchmanSerial	=	_iSerial;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendAskSimplePitchmanShopInfoList(WORD *_lpList,int _iCount)
{
	CG_ASK_SIMPLE_PITCHMAN_SHOP_INFO_LIST	packet;

	packet.base.set(sizeof(packet),dCG_ASK_SIMPLE_PITCHMAN_SHOP_INFO_LIST);
	packet.wCount	=	_iCount;
	memcpy(packet.awList,_lpList,sizeof(WORD)*_iCount);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//	노점상 닫기
BOOL
CAgent::sendPitchmanShopClose(int _iSerial)
{
	CG_CLOSE_PITCHMAN_SHOP	packet;

	packet.base.set(sizeof(packet),dCG_CLOSE_PITCHMAN_SHOP);
	packet.wSerial		=	_iSerial;

	return	sendPacket((char *)&packet,packet.base.wSize);
}