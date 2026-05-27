#include "CActor.h"
#include "CPitchmanShop.h"
#include "CField.h"
#include "debugCode.h"
#include "packetManager.h"

int
cACTOR::openPitchmanShop()
{
	CPlayerTitleInfo	*lpTitle	=	getTitle(eTN_DEALER);

	if	(g_bIsTestServer	==	FALSE)
		if	(!lpTitle)
			return	sendPitchmanShopMessage(ePSM_SETUP_FAILED_BY_REQUIRE_DEALER_TITLE);

	if	(m_wRestraintTimeByBattle)
		return	sendPitchmanShopMessage(ePSM_SETUP_FAILED);

	if	(m_wTransToWeaponTime)
		return	ePSM_FAILED;

	if	(m_lpField->m_bIsNotOpenPitchManShop)
	{
		return	sendPitchmanShopMessage(ePSM_CAN_NOT_OPEN_THIS_FIELD);
	}

	CPitchmanShop		*lpShop		=	getPitchmanShop();

	if	(lpShop)
		closePitchmanShop();

	if	(m_lpField->isExistTrafficObjectToOpenPitchmanShop(m_pos.x,m_pos.y))
		return	sendPitchmanShopMessage(ePSM_SETUP_FAILED_BY_EXIT_ANOTHER_PITCHMAN_SHOP);

	WORD	wShopSerial	=	g_pitchmanShopManager.addShop(this);

	if	(wShopSerial	==	0xffff)
		return	sendPitchmanShopMessage(ePSM_SETUP_FAILED_BY_TOO_MANY_PITCHMAN_SHOP);

	m_wPitchmanShopSerial	=	wShopSerial;
	m_iAnm					=	dACT_SITDOWN;

	{
		SG_ADD_PITCHMAN_SHOP	packet;

		packet.base.set(sizeof(packet),dSG_ADD_PITCHMAN_SHOP);
		g_pitchmanShopManager.getSimpleShopInfo(&packet.info,wShopSerial);

		m_lpField->addSendPacket((ALL_MSG *)&packet,m_wSerialInField);
	}

	{
		SG_OPEN_PITCHMAN_SHOP	packet;

		packet.base.set(sizeof(packet),dSG_OPEN_PITCHMAN_SHOP);

		packet.wSerial	=	wShopSerial;
		packet.wX		=	m_pos.x;
		packet.wY		=	m_pos.y-10;

		g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
	}

	return	ePSM_OK;
}

int
cACTOR::addPitchmanShopItem(int _iShopSerial,int _iInventorySlot,int _iShopSlot,DWORD _dwPrice, BYTE _bDenomination)		// 09.08.21
{
	CPitchmanShop*	lpShop	=	getPitchmanShop();

	if (!lpShop)
		return	sendPitchmanShopMessage(ePSM_FAILED_SHOP_NOT_OPENED);
		
	if (_dwPrice	==	0)
		return	sendPitchmanShopMessage(ePSM_FAILED);

	if (_iShopSlot	>=	m_wPitchmanShopSize)
		return	sendPitchmanShopMessage(ePSM_FAILED);

	int iResult	=	lpShop->addItem(_iInventorySlot,_iShopSlot,_dwPrice, _bDenomination);

	if (iResult	!=	ePSM_OK)
		return	sendPitchmanShopMessage(iResult);

	{
		SG_ADD_PITCHMAN_SHOP_ITEM	packet;

		packet.base.set(sizeof(packet),dSG_ADD_PITCHMAN_SHOP_ITEM);

		packet.wShopSlot		=	_iShopSlot;
		packet.wInventorySlot	=	_iInventorySlot;
		packet.dwPrice			=	_dwPrice;
		packet.bDenomination	=	_bDenomination;
		g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
	}

	return	ePSM_OK;
}

int
cACTOR::removePitchmanShopItem(int _iShopSerial,int _iShopSlot)
{
	CPitchmanShop*	lpShop	=	getPitchmanShop();

	if (!lpShop)
		return	sendPitchmanShopMessage(ePSM_FAILED_SHOP_NOT_OPENED);
		
	if (m_wPitchmanShopSerial	!=	_iShopSerial)
	{
		if (lpShop->m_iPitchmanSerial==	m_iZoneSerial)
			closePitchmanShop();

		return	sendPitchmanShopMessage(ePSM_FAILED_SHOP_NOT_OPENED);
	}

	int iResult	=	lpShop->removeItem(_iShopSlot);

	if (iResult	!=	ePSM_OK)
		return	sendPitchmanShopMessage(iResult);

	{
		SG_REMOVE_PITCHMAN_SHOP_ITEM	packet;

		packet.base.set(sizeof(packet),dSG_REMOVE_PITCHMAN_SHOP_ITEM);

		packet.wSlot	=	_iShopSlot;

		g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
	}

	return	ePSM_OK;
}

int
cACTOR::changePitchmanShopInfo(int _iShopSerial,int _iStatus,char *_lpstrName,int _iSignboardShape,WORD _wTextColor,BOOL _bIsBold,BOOL _bIsShadowText)
{
	char	strText[dPITCHMAN_SHOP_NAME_LENGTH];

	memcpy(strText,_lpstrName,dPITCHMAN_SHOP_NAME_LENGTH);

	strText[dPITCHMAN_SHOP_NAME_LENGTH-1]	=	NULL;

	CPitchmanShop*	lpShop	=	getPitchmanShop();

	if	(!lpShop)
		return	sendPitchmanShopMessage(ePSM_FAILED_SHOP_NOT_OPENED);
		
	if	(m_wPitchmanShopSerial	!=	_iShopSerial)
	{
		if	(lpShop->m_iPitchmanSerial==	m_iZoneSerial)
			closePitchmanShop();

		return	sendPitchmanShopMessage(ePSM_FAILED_SHOP_NOT_OPENED);
	}

	if	(m_bf1IsOwnedPitchmanShopSignBoard1	==	FALSE	&&	_iSignboardShape	==	1)
		_iSignboardShape	=	0;
	if	(m_bf1IsOwnedPitchmanShopSignBoard2	==	FALSE	&&	_iSignboardShape	==	2)
		_iSignboardShape	=	0;
	if	(m_bf1IsOwnedPitchmanShopSignBoard3	==	FALSE	&&	_iSignboardShape	==	3)
		_iSignboardShape	=	0;
	if	(m_bf1IsOwnedPitchmanShopSignBoard4	==	FALSE	&&	_iSignboardShape	==	4)
		_iSignboardShape	=	0;
	if	(m_bf1IsOwnedPitchmanShopSignBoard5	==	FALSE	&&	_iSignboardShape	==	5)
		_iSignboardShape	=	0;
//	if (m_bf1IsOwnedPitchmanShopLoudSpeaker	==	FALSE	&&	_iSignboardShape	==	1)
//		_iSignboardShape	=	0;

	if	(m_bf1IsOwnedPitchmanColorPen		==	FALSE)
		_wTextColor	=	0;

	if	(m_bf1IsOwnedPitchmanShopBoldPen	==	FALSE)
		_bIsBold	=	FALSE;

	if	(m_bf1IsOwnedPitchmanShopLongPen	==	FALSE)
	{
		strncpy(strText,_lpstrName,(dPITCHMAN_SHOP_NAME_LENGTH-2)/2);
		strText[(dPITCHMAN_SHOP_NAME_LENGTH-2)/2]	=	NULL;
	}
	else
	{
		strncpy(strText,_lpstrName,dPITCHMAN_SHOP_NAME_LENGTH-2);
		strText[dPITCHMAN_SHOP_NAME_LENGTH-2]	=	NULL;
	}

//	m_bf1IsOwnedPitchmanShopTwinkle		=	FALSE;


	int iResult	=	lpShop->changeShopInfo(_iStatus,strText,_iSignboardShape,_wTextColor,_bIsBold,_bIsShadowText);

	if (iResult	!=	ePSM_OK)
		return	sendPitchmanShopMessage(iResult);

	{
		SG_CHANGE_PITCHMAN_SHOP_INFO	packet;

		packet.base.set(sizeof(packet),dSG_CHANGE_PITCHMAN_SHOP_INFO);

		lpShop->getSimpleShopInfo(&packet.info);

		strcpy(packet.strName,strText);

		if (lpShop->m_wStatus	==	ePSS_READY)
			packet.base.wSize	=	sizeof(packet)-sizeof(packet.strName);
		else
			packet.base.wSize	=	sizeof(packet)-sizeof(packet.strName)+strlen(strText)+1;

		m_lpField->addSendPacket((ALL_MSG *)&packet,m_wSerialInField);
	}

	return	ePSM_OK;
}

int
cACTOR::changePitchmanShopPlace(int _iShopSerial,int _iShopSlot1,int _iShopSlot2)
{
	CPitchmanShop*	lpShop	=	getPitchmanShop();

	if (!lpShop)
		return	sendPitchmanShopMessage(ePSM_FAILED_SHOP_NOT_OPENED);
		
	if (m_wPitchmanShopSerial	!=	_iShopSerial)
	{
		if (lpShop->m_iPitchmanSerial==	m_iZoneSerial)
			closePitchmanShop();

		return	sendPitchmanShopMessage(ePSM_FAILED_SHOP_NOT_OPENED);
	}

	int iResult	=	lpShop->changePitchmanShopPlace(_iShopSlot1,_iShopSlot2);

	if (iResult	!=	ePSM_OK)
		return	sendPitchmanShopMessage(iResult);

	{
		SG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE	packet;

		packet.base.set(sizeof(packet),dSG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE);
		packet.wSlot1	=	_iShopSlot1;
		packet.wSlot2	=	_iShopSlot2;

		g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
	}

	return	ePSM_OK;
}

int
cACTOR::sendSoldPitchmanShopItem(char *_lpstrName,int _iShopSlot,DWORD _dwPrice, BYTE _bDenomination)	// 09.08.21
{
	SG_SOLD_PITCHMAN_SHOP_ITEM	packet;

	packet.base.set(sizeof(packet),dSG_SOLD_PITCHMAN_SHOP_ITEM);
	packet.dwPrice	=	_dwPrice;
	packet.wSlot	=	_iShopSlot;
	packet.bDenomination	=	_bDenomination;
	strcpy(packet.strName,_lpstrName);

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);

	return	ePSM_OK;
}

int
cACTOR::buyPitchmanShopItem(int _iShopSerial,int _iShopSlot,cItem *_lpItem,DWORD _dwPrice, BYTE _bDenomination)
{	
	CPitchmanShop*	lpShop	=	g_pitchmanShopManager.get(_iShopSerial);

	if (!lpShop)
		return	sendPitchmanShopMessage(ePSM_FAILED_BY_VALID_SHOP_SERIAL);

	cACTOR	*lpOwner	=	lpShop->getOwner();

	if	(!lpOwner)
	{
		g_pitchmanShopManager.closeShop(_iShopSerial);

		return	sendPitchmanShopMessage(ePSM_FAILED_OPEN_SHOP_BY_NOT_OPENED);
	}

	if	(_iShopSlot	>=	dPITCHMAN_SHOP_INVENTORY_SIZE)
		return	sendPitchmanShopMessage(ePSM_FAILED_BUY_ITEM_BY_CHANGE_SHOP_INFO);

	cItem	*lpItem		=	lpOwner->getInventoryItem(lpShop->m_aItem[_iShopSlot].m_wSlotIndex);

	if	(!lpItem	||	lpOwner->m_wPitchmanShopSerial != _iShopSerial)
		return	sendPitchmanShopMessage(ePSM_FAILED_BUY_ITEM_BY_CHANGE_SHOP_INFO);

	if	(lpShop->m_aItem[_iShopSlot].m_dwPrice	!=	_dwPrice)
		return	sendPitchmanShopMessage(ePSM_FAILED_BUY_ITEM_BY_CHANGE_SHOP_INFO);

	if	(lpShop->m_aItem[_iShopSlot].m_bDenomination !=	_bDenomination)
		return	sendPitchmanShopMessage(ePSM_FAILED_BUY_ITEM_BY_CHANGE_SHOP_INFO);

	if	(lpItem->isCanNotTradeItem())
	{
		lpOwner->m_wPitchmanShopSerial = _iShopSerial;
		lpOwner->closePitchmanShop();

		return	sendPitchmanShopMessage(ePSM_FAILED_BUY_ITEM_BY_CHANGE_SHOP_INFO);
	}

	_lpItem->m_dwSerial	=	lpItem->m_dwSerial;

	if	(lpItem->checkFirstEffect(eIE_MINIPET_POUCH))
	{
		if	(!lpItem->isExactlySameMiniPetPouch(_lpItem))
			return	sendPitchmanShopMessage(ePSM_FAILED_BUY_ITEM_BY_CHANGE_SHOP_INFO);
	}
	else
	{
		if	(!lpItem->isExactlySameItem(_lpItem))
			return	sendPitchmanShopMessage(ePSM_FAILED_BUY_ITEM_BY_CHANGE_SHOP_INFO);
	}

	cItem	*lpValidSlot=	getValidInventorySlot();

	if	(!lpValidSlot)
		return	sendPitchmanShopMessage(ePSM_FAILED_BUY_ITEM_BY_FULL_INVENTORY);

	LONGLONG	llMoney	=0;

	if	(_bDenomination)
	{
		int iGoldBarCount = getStackItemCount(dITEM_INDEX_GOLD_BAR, FALSE);

		int iStackLimit = g_aBasicItem[dITEM_INDEX_GOLD_BAR].m_wStackLimit ;
		if	(_dwPrice>iGoldBarCount)
			return	sendPitchmanShopMessage(ePSM_FAILED_BUY_ITEM_BY_INSUFFICIENT_GOLD_BAR);

		int iSlot = (_dwPrice/iStackLimit);
		int iOwnerSlot = lpOwner->getRemainInventorySlotCount();

		if	(iSlot>iOwnerSlot)
			return sendPitchmanShopMessage(ePSM_FAILED_BUY_ITEM_BY_OWNER_NOT_SLOT);

	}
	else
	{
		if	(m_iGold	<	(int)_dwPrice)
			return	sendPitchmanShopMessage(ePSM_FAILED_BUY_ITEM_BY_INSUFFICIENT_GOLD);
		
		llMoney	=	lpOwner->m_iGold;
		llMoney	+=	_dwPrice;
		
		if	(llMoney >	dMAX_OWN_GOLD)
			return	sendPitchmanShopMessage(ePSM_FAILED_BUY_ITEM_BY_FULL_GOLD);
	}
	
	BOOL	bIsOperatorTrade	=	FALSE;

	if	(lpOwner->isTester()+isTester())
		bIsOperatorTrade			=	TRUE;

	sendPitchmanShopTradeLog(lpItem,lpOwner,_dwPrice,bIsOperatorTrade,_bDenomination);

	{
		SG_BUY_PITCHMAN_SHOP_ITEM	packet;

		packet.base.set(sizeof(packet),dSG_BUY_PITCHMAN_SHOP_ITEM);
		packet.wPitchmanSerial	=	_iShopSerial;	//	노점상 시리얼
		packet.wSlot			=	_iShopSlot;		//	노점상에 아이템 위치
		memcpy(&packet.item,lpItem,sizeof(cItem));		//	구입한 아이템
		packet.dwPrice			=	_dwPrice;	//	가격
		packet.bDenomination	=	_bDenomination;// 통화 단위  09.08.21
		strcpy(packet.strOwnerName,lpOwner->m_strName);

		g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
	}

	lpValidSlot->copy(lpItem);
	lpItem->reset();

	if	(_bDenomination)			// 09.10.08
	{
		removeItem(dITEM_INDEX_GOLD_BAR,_dwPrice, FALSE,eRI_BY_PITCHMANSHOP,eSPENT_GOLD_IGNORE);

		LONGLONG	llTradeGold	=	_dwPrice;

		llTradeGold				*=	100000000;

		lpOwner->m_llTradeGettingGold	+=	llTradeGold;
		m_llTradeSpentGold				-=	llTradeGold;

		int iStackLimit = g_aBasicItem[dITEM_INDEX_GOLD_BAR].m_wStackLimit ;
		int iAddItemSlot = _dwPrice/iStackLimit;
		int iRemainder = 0;

		if	(_dwPrice%iStackLimit)
			iRemainder = 1;

		cItem*	addItem = new cItem[iAddItemSlot + iRemainder];

		for(int i=0;i<iAddItemSlot; ++i)
		{
			g_im.generateItemByBaseItem(&addItem[i],dITEM_INDEX_GOLD_BAR,iStackLimit);

			cItem	*lpEmptySlot	=	lpOwner->getEmptyItemSlot();

			if	(!lpEmptySlot)
				return sendPitchmanShopMessage(ePSM_FAILED_BUY_ITEM_BY_OWNER_NOT_SLOT);

			lpEmptySlot->copy(&addItem[i]);
		}

		if	(iRemainder)
		{
			g_im.generateItemByBaseItem(&addItem[iAddItemSlot],dITEM_INDEX_GOLD_BAR,_dwPrice%iStackLimit);

			cItem	*lpEmptySlot	=	lpOwner->getEmptyItemSlot();

			if	(!lpEmptySlot)	
				return sendPitchmanShopMessage(ePSM_FAILED_BUY_ITEM_BY_OWNER_NOT_SLOT);

			lpEmptySlot->copy(&addItem[iAddItemSlot]);
		}

		pKILL(addItem);
		
	}
	else
	{
		lpOwner->m_iGold			=	(int)llMoney;
		m_iGold						-=	_dwPrice;

		lpOwner->m_llTradeGettingGold	+=	_dwPrice;
		m_llTradeSpentGold			-=	_dwPrice;
	}

	lpOwner->sendSoldPitchmanShopItem(m_strName,_iShopSlot,_dwPrice, _bDenomination);
	lpShop->removeItem(_iShopSlot);

	lpShop->m_wActorItemChecksum=	lpOwner->getItemCheckSum();

	lpOwner->immediatelySendSaveDataToDBMemory(TRUE);
	immediatelySendSaveDataToDBMemory(TRUE);
	
	return	ePSM_OK;
}

CPitchmanShop*
cACTOR::getPitchmanShop()
{
	if (m_wPitchmanShopSerial	==	0xffff)
		return	NULL;

	return	g_pitchmanShopManager.get(m_wPitchmanShopSerial);
}

int
cACTOR::askPitchmanShopInfo(int _iShopSerial)
{
	CPitchmanShop	*lpShop	=	getPitchmanShop();
	cACTOR			*lpOwner;
	SG_PITCHMAN_SHOP_INFO	packet;

	if (lpShop)
		return	sendPitchmanShopMessage(ePSM_FAILED);
	
	lpShop	=	g_pitchmanShopManager.get(_iShopSerial);

	if (!lpShop)
		goto	label_valid_shop;

	if	(lpShop->m_wStatus	==	ePSS_READY)
		return	sendPitchmanShopMessage(ePSM_FAILED_OPEN_SHOP_BY_NOT_OPENED);

	lpOwner	=	lpShop->getOwner();

	if (lpOwner	==	NULL)
		goto	label_valid_shop;

	if (lpOwner->m_wPitchmanShopSerial	!=	_iShopSerial)
	{
		lpOwner->m_wPitchmanShopSerial	=	_iShopSerial;

		lpOwner->closePitchmanShop();

		return	sendPitchmanShopMessage(ePSM_FAILED);
	}

	packet.base.set(sizeof(packet),dSG_PITCHMAN_SHOP_INFO);

	if (g_pitchmanShopManager.getShopInfo(&packet.shopInfo,_iShopSerial) == FALSE)
		goto	label_valid_shop;

	packet.base.wSize	=	sizeof(packet) - sizeof(packet.shopInfo.m_aItem) + sizeof(CPitchmanShopItemInfoForClient)*packet.shopInfo.m_bf4ItemCount;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);

	return	ePSM_OK;

label_valid_shop:

	g_pitchmanShopManager.closeShop(_iShopSerial);

	SG_REMOVE_PITCHMAN_SHOP	packetError;

	packetError.base.set(sizeof(packetError),dSG_REMOVE_PITCHMAN_SHOP);
	packetError.wSerial	=	_iShopSerial;

	m_lpField->addSendPacket((ALL_MSG *)&packetError,m_wSerialInField);

	return	sendPitchmanShopMessage(ePSM_FAILED);

}

//	닫기
int
cACTOR::closePitchmanShop()
{
	if	(m_wPitchmanShopSerial	==	0xffff)
		return	ePSM_OK;

	g_pitchmanShopManager.closeShop(m_wPitchmanShopSerial);

	SG_REMOVE_PITCHMAN_SHOP	packet;

	packet.base.set(sizeof(packet),dSG_REMOVE_PITCHMAN_SHOP);
	packet.wSerial	=	m_wPitchmanShopSerial;

	m_lpField->addSendPacket((ALL_MSG *)&packet,m_wSerialInField);
	
	m_wPitchmanShopSerial	=	0xffff;

	return	ePSM_OK;
}

int
cACTOR::closePitchmanShop(int _iShopSerial)
{
	CPitchmanShop*	lpShop	=	getPitchmanShop();

	if (!lpShop)
		return	sendPitchmanShopMessage(ePSM_FAILED_SHOP_NOT_OPENED);
		
	if (m_wPitchmanShopSerial	!=	_iShopSerial)
	{
		if (lpShop->m_iPitchmanSerial	==	m_iZoneSerial)
			closePitchmanShop();

		return	sendPitchmanShopMessage(ePSM_FAILED_SHOP_NOT_OPENED);
	}

	closePitchmanShop();

	return	ePSM_OK;
}