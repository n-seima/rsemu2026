#include "CAgent.H"
#include "CActor.H"
#include "CHero.H"

//
//	아이템 데이터를 요구
BOOL
CAgent::sendAskItemData()
{
	if (isOperateItemData())
		return	FALSE;

	CG_ASK_ITEM_DATA	packet;

	packet.base.set(sizeof(CG_ASK_ITEM_DATA),dCG_ASK_ITEM_DATA);

	setOperateItemData(TRUE);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	인벤토리 아이템 데이터를 요구
BOOL
CAgent::sendAskInventoryData()
{
	if (isOperateItemData())
		return	FALSE;

	CG_ASK_INVENTORY_DATA	packet;

	packet.base.set(sizeof(CG_ASK_INVENTORY_DATA),dCG_ASK_INVENTORY_DATA);
	setOperateItemData(TRUE);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	인벤토리 아이템 데이터를 요구
BOOL
CAgent::sendAskEquipmentData()
{
	if (isOperateItemData())
		return	FALSE;

	CG_ASK_EQUIPMENT_DATA	packet;

	packet.base.set(sizeof(CG_ASK_EQUIPMENT_DATA),dCG_ASK_EQUIPMENT_DATA);
	setOperateItemData(TRUE);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendRepairItem(int _iItemCheckSum,int _iSlot,int _iNpcSerial)
{
	if (isOperateItemData())
		return	FALSE;

	CG_REPAIR_ITEM	packet;

	packet.base.set(sizeof(CG_REPAIR_ITEM),dCG_REPAIR_ITEM);

	setOperateItemData(TRUE);

	packet.wCheckSum	=	_iItemCheckSum;
	packet.wSlot		=	_iSlot;
	packet.wNpcSerial	=	_iNpcSerial;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	필드에 아이템을 줍자.
BOOL
CAgent::sendPickFieldItem()
{
	if (g_fieldItem.m_iPickItem	==	0xffff		)	return	FALSE;
	if (isOperateItemData())
		return	FALSE;

	setOperateItemData(TRUE);

	CG_PICK_ITEM	packet;

	packet.base.set(sizeof(CG_PICK_ITEM),dCG_PICK_ITEM);

	packet.wCheckSum	=	g_hero.getItemCheckSum();
	packet.wItemSerial	=	g_fieldItem.m_iPickItem;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	필드에 아이템을 줍자.
BOOL
CAgent::sendEquipItem(int _iItemSlot,int _iPart,BOOL _bIsBeltItem)
{
	if (isOperateItemData())
		return	FALSE;

	setOperateItemData(TRUE);

	CG_EQUIP_ITEM	packet;

	packet.base.set(sizeof(CG_EQUIP_ITEM),dCG_EQUIP_ITEM);

	packet.wCheckSum	=	g_hero.getItemCheckSum();
	packet.wItemSlot	=	_iItemSlot;
	packet.wPart		=	_iPart;
	packet.wBaseItem	=	g_hero.getInventoryItem(_iItemSlot)->m_wBaseItem;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	아이템의 위치를 바꾼다.
BOOL
CAgent::sendChangeItemPlace(int _iSrc,int _iDest)
{
	if (isOperateItemData())
		return	FALSE;

	setOperateItemData(TRUE);

	CG_CHANGE_ITEM_PLACE	packet;

	packet.base.set(sizeof(CG_CHANGE_ITEM_PLACE),dCG_CHANGE_ITEM_PLACE);

	packet.wCheckSum	=	g_hero.getItemCheckSum();
	packet.wSrcItemSlot	=	_iSrc;
	packet.wDestItemSlot=	_iDest;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	장비 벗기 메시지 전송
BOOL
CAgent::sendStripEquipmentMessage(int _iPart)
{
	if (isOperateItemData())
		return	FALSE;

	setOperateItemData(TRUE);

	CG_STRIP_EQUIPMENT	packet;

	packet.base.set(sizeof(CG_STRIP_EQUIPMENT),dCG_STRIP_EQUIPMENT);

	packet.wCheckSum	=	g_hero.getItemCheckSum();
	packet.wPart		=	_iPart;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	아이템 떨구기
BOOL
CAgent::sendDroppingItem(int _iPosX,int _iPosY,int _iItemSlot,int _iItemCount)
{
	if (g_hero.m_iBookedDroppingItemSlot==	0xffff	)
		return	FALSE;
	if (isOperateItemData())
		return	FALSE;

	setOperateItemData(TRUE);

	CG_DROP_ITEM	packet;

	packet.base.set(sizeof(CG_DROP_ITEM),dCG_DROP_ITEM);

	packet.wCheckSum			=	g_hero.getItemCheckSum();
	packet.wPosX				=	_iPosX;
	packet.wPosY				=	_iPosY;
	packet.dwCount				=	_iItemCount;
	packet.wItemSlot			=	_iItemSlot;

	if (g_hero.m_iBookedDroppingItemSlot	==	dITEM_DRAG_GOLD)
			packet.wBaseItem	=	dITEM_MONEY;
	else
	{
		if (g_hero.m_iBookedDroppingItemSlot	>=	dBORDER_OF_ITEM_AND_EQUIPMENT)
				packet.wBaseItem	=	g_hero.m_aEquip[g_hero.m_iBookedDroppingItemSlot-dBORDER_OF_ITEM_AND_EQUIPMENT].m_wBaseItem;
		else	packet.wBaseItem	=	g_hero.m_aItems[g_hero.m_iBookedDroppingItemSlot].m_wBaseItem;
	}

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	아이템 사용
BOOL
CAgent::sendUseItemToCharacter(int _iItemSlot,int _iDest)
{
	cItem	*lpItem	=	g_hero.getInventoryItem(_iItemSlot);

	if (!lpItem	||	isOperateItemData())
		return	FALSE;

	setOperateItemData(TRUE);

	if (_iDest	==	0xffff)
		_iDest	=	g_lpHero->m_wSerial;

	CG_USE_ITEM	packet;

	packet.base.set(sizeof(CG_USE_ITEM),dCG_USE_ITEM);

	packet.wCheckSum			=	g_hero.getItemCheckSum();
	packet.wItemSlot			=	_iItemSlot;	//	아이템 슬롯
	packet.wBaseItem			=	lpItem->m_wBaseItem;	//	어떤 아이템인겨?(최소한의 정보로 확인한다.)
	packet.wWho					=	_iDest;		//	언넘한테?

	return	sendPacket((char *)&packet,packet.base.wSize);
}


//
//	아이템 사용
BOOL
CAgent::sendUseItemToItem(int _iItemSlot,int _iDest)
{
	cItem	*lpItem	=	g_hero.getInventoryItem(_iItemSlot);

	if (!lpItem	||	isOperateItemData())
		return	FALSE;

	setOperateItemData(TRUE);

	if (_iDest	==	0xffff)
		_iDest	=	g_lpHero->m_wSerial;

	CG_USE_ITEM_TO_ITEM	packet;

	packet.base.set(sizeof(CG_USE_ITEM_TO_ITEM),dCG_USE_ITEM_TO_ITEM);

	packet.wCheckSum	=	g_hero.getItemCheckSum();
	packet.bItemSlot	=	_iItemSlot;	//	아이템 슬롯
	packet.bTargetItem	=	_iDest;		//	언넘한테?
	packet.wBaseItem	=	lpItem->m_wBaseItem;	//	어떤 아이템인겨?(최소한의 정보로 확인한다.)

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	벨트 아이템 사용
BOOL
CAgent::sendUseBeltItem(int _iBeltSlot,int _iTarget)
{
	if (isOperateItemData())
		return	FALSE;

	setOperateItemData(TRUE);

	CG_USE_BELT_ITEM	packet;

	packet.base.set(sizeof(CG_USE_BELT_ITEM),dCG_USE_BELT_ITEM);

	packet.wCheckSum			=	g_hero.getItemCheckSum();
	packet.wTarget				=	_iTarget;	//	아이템 슬롯
	packet.wBeltSlot			=	_iBeltSlot;	//	아이템 슬롯
	packet.wBaseItem			=	g_hero.getBeltItem(_iBeltSlot)->m_wBaseItem;	//	어떤 아이템인겨?(최소한의 정보로 확인한다.)

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	벨트 아이템 리로드
BOOL
CAgent::sendReloadBeltItem()
{
	if (isOperateItemData())
		return	FALSE;

	setOperateItemData(TRUE);

	CG_RELOAD_BELT_ITEM		packet;

	packet.base.set(sizeof(CG_RELOAD_BELT_ITEM	),dCG_RELOAD_BELT_ITEM);
	packet.wCheckSum			=	g_hero.getItemCheckSum();

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	아이템 파괴 요청
BOOL
CAgent::sendAskDestroyItem(int _iFocusItem)
{
	CG_DESTROY_ITEM		packet;
	
	packet.base.set(sizeof(CG_DESTROY_ITEM),dCG_DESTROY_ITEM);

	packet.wSlot		=	_iFocusItem;
	packet.wCheckSum	=	g_hero.getItemCheckSum();

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//	아이템 팩 이름 요청
BOOL
CAgent::sendAskItemPackName(int _iCount,WORD *_lpPackIndex)
{
	if (_iCount == 0)
		return	FALSE;

	CG_ASK_ITEM_PACK_NAME	packet;
	
	packet.base.set(sizeof(packet),dCG_ASK_ITEM_PACK_NAME);

	memcpy(packet.awPackIndex,_lpPackIndex,sizeof(WORD)*_iCount);
	packet.wCount			=	_iCount;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendRemovePrefix(int _iUseItem,int _iDestItem,int _iPrefixIndex)
{
	CG_REMOVE_PREFIX	packet;

	packet.base.set(sizeof(packet),dCG_REMOVE_PREFIX);

	packet.wPrefixIndex	=	_iPrefixIndex;
	packet.wUseItem		=	_iUseItem;
	packet.wDestItem	=	_iDestItem;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendMergeItem(int _iItem,int _iClient)
{
	CG_MERGE_ITEM		packet;

	packet.base.set(sizeof(packet),dCG_MERGE_ITEM);

	packet.wItemSlot	=	_iItem;
	packet.wNpc			=	_iClient;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendJudgeItem(int _iItem,int _iClient)
{
	CG_JUDGE_TO_ITEM	packet;

	packet.base.set(sizeof(packet),dCG_JUDGE_TO_ITEM);

	packet.wItemSlot	=	_iItem;
	packet.wNpc			=	_iClient;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendUseGateGlove(int _iTargetField, BOOL _bIsNetCafeUser,int _iItemSlot)
{
	CG_WARP_FIELD_BY_GATE_GLOVE	packet;

	if (_bIsNetCafeUser)
		packet.base.set(sizeof(packet),dCG_WARP_FIELD_BY_NET_CAFE);
	else
		packet.base.set(sizeof(packet),dCG_WARP_FIELD_BY_GATE_GLOVE);
	packet.wDestField	=	_iTargetField;
	packet.iItemSlot	=	_iItemSlot;

	return	sendPacket((char *)&packet,packet.base.wSize);
}
