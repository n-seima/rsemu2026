#include "CBank.h"
#include "CHero.h"
#include "CGamePlay.h"
#include "CMessage.h"


CBank	g_bank;

void
CBank::reset()
{
	m_wBankSerial	=	0xffff;
	m_wGoldBarCount	= 0;		// 09.08.20
	m_wGoldBarTax	= 0;		// 09.08.20
}

//
//	은행 열어
void
CBank::open(CBankInfo *_lpBankData,int _iBankSerial,int _iBankSize,int _iBankCharge)
{
	m_wBankSerial	=	_iBankSerial;
	m_wBankSize		=	_iBankSize;
	m_wBankCharge	=	_iBankCharge;

	memcpy(&m_firstBankInfo,_lpBankData,sizeof(CBankInfo));
	memcpy(&m_bankInfo,_lpBankData,sizeof(CBankInfo));

	m_iOwnGold		=	g_hero.m_iGold;
	m_wInventorySize=	g_hero.m_wItemSlotCount;

	memcpy(m_aEquipment,g_hero.m_aEquip,sizeof(g_hero.m_aEquip));
	memcpy(m_aInventoryItem,g_hero.m_aItems,sizeof(g_hero.m_aItems));
}

//
//	은행에 아이템 저장하기
BOOL
CBank::storeItemToTheBank(int _iInventorySlot,int _iBankSlot,DWORD _dwCheckSum,BOOL _bIsCheck)
{
	if (_iBankSlot	>=	m_wBankSize)	return	FALSE;

	cItem	*lpBankSlot,*lpItemSlot;

	lpBankSlot	=	(cItem*)&m_bankInfo.m_aItems[_iBankSlot];

	if (_iInventorySlot		>=	dBORDER_OF_ITEM_AND_EQUIPMENT)
	{
		_iInventorySlot		-=	dBORDER_OF_ITEM_AND_EQUIPMENT;

		if (_iInventorySlot	>=	dEQUIPMENT_PART_COUNT)	return	FALSE;

		lpItemSlot			=	&m_aEquipment[_iInventorySlot];
	}
	else
	{
		if (_iInventorySlot	>=	m_wInventorySize)	return	FALSE;

		lpItemSlot			=	&m_aInventoryItem[_iInventorySlot];
	}

	if (lpItemSlot->m_wBaseItem		== 0xffff)	return	FALSE;
	if (lpBankSlot->m_wBaseItem		!= 0xffff)
	{
		if (_bIsCheck)
			CGamePlay::AddSystemMessage(WHITE,dMSG_CAN_NOT_STORE_ITEM_TO_USED_SLOT);

		return	FALSE;
	}

	if (lpItemSlot->m_wBaseItem	==	0xffff)	return	FALSE;
	if (lpBankSlot->m_wBaseItem	!=	0xffff)	return	FALSE;

	if (_bIsCheck)	return	TRUE;
	else
	{
		if (_dwCheckSum	!=	getCheckSum())	
			return	FALSE;
	}

	lpBankSlot->copy(lpItemSlot);
	lpItemSlot->reset();

	return	TRUE;
}

//
//	배지의 수 계산
int
CBank::getBadgeCountInInventory()
{
	int		i,iBadgeCount=0;

	for (i=0;i<dOWN_ITEM_COUNT;i++)
	{
		cItem		*lpItem			=	&m_aInventoryItem[i];
		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(lpBasicItem	==	NULL)
			continue;

		if	(lpBasicItem->m_attr.isBadge)
			iBadgeCount++;
	}

	for (i=0;i<g_hero.m_bf6ExtraInventorySize;i++)
	{
		cItem		*lpItem			=	&g_hero.m_aExtraInventory[i];
		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(lpBasicItem	==	NULL)
			continue;

		if	(lpBasicItem->m_attr.isBadge)
			iBadgeCount++;
	}

	return	iBadgeCount;
}

cItem*
CBank::getItemInInventory(int _iBaseItem)
{
	int		i;

	for (i=0;i<dOWN_ITEM_COUNT;i++)
	{
		cItem		*lpItem			=	&m_aInventoryItem[i];

		if	(lpItem->m_wBaseItem	==	_iBaseItem)
			return	lpItem;
	}

	for (i=0;i<g_hero.m_bf6ExtraInventorySize;i++)
	{
		cItem		*lpItem		=	&g_hero.m_aExtraInventory[i];

		if	(lpItem->m_wBaseItem==	_iBaseItem)
			return	lpItem;
	}

	return	NULL;
}

//
//	은행에서 아이템 꺼내오기
BOOL
CBank::withdrawItemFromTheBank(int _iInventorySlot,int _iBankSlot,DWORD _dwCheckSum,BOOL _bIsCheck)
{
	if (_iBankSlot	>=	m_wBankSize)	return	FALSE;

	cItem	*lpBankSlot,*lpItemSlot;

	lpBankSlot	=	(cItem*)&m_bankInfo.m_aItems[_iBankSlot];

	if (_iInventorySlot		>=	dBORDER_OF_ITEM_AND_EQUIPMENT)
	{
		_iInventorySlot		-=	dBORDER_OF_ITEM_AND_EQUIPMENT;

		if (_iInventorySlot	>=	dEQUIPMENT_PART_COUNT)	return	FALSE;

		lpItemSlot			=	&m_aEquipment[_iInventorySlot];
	}
	else
	{
		if (_iInventorySlot	>=	m_wInventorySize)	return	FALSE;

		lpItemSlot			=	&m_aInventoryItem[_iInventorySlot];
	}

	if (lpItemSlot->m_wBaseItem	!=	0xffff)
	{
		if (_bIsCheck)
			CGamePlay::AddSystemMessage(WHITE,dMSG_CAN_NOT_STORE_ITEM_TO_USED_SLOT);

		return	FALSE;
	}

	if (lpBankSlot->m_wBaseItem	==	0xffff)	return	FALSE;

	cBasicItem	*lpBasicItem	=	lpBankSlot->getBasicItem();

	if (lpBasicItem )
	{
		if (lpBasicItem->m_attr.isCanNotOwnSameItem)
			if (getItemInInventory(lpBankSlot->m_wBaseItem))
			{
				g_msgBox.cPopup("",dMSG_CAN_NOT_OWN_SAME_ITEM,dMSG_OK);
				return	FALSE;
			}

		if (lpBasicItem->m_attr.isBadge)
			if (getBadgeCountInInventory()	>=	dLIMIT_BADGE_ITEM_COUNT_IN_INVENTORY)
			{
				g_msgBox.cPopup("",_ms(dMSG_TOO_MANY_BADGE_ITEM_FORM,dLIMIT_BADGE_ITEM_COUNT_IN_INVENTORY),dMSG_OK);
				return	FALSE;
			}
	}


	if (_bIsCheck)	return	TRUE;
	else
	{
		if (_dwCheckSum	!=	getCheckSum())	
			return	FALSE;
	}

	lpItemSlot->copy(lpBankSlot);
	lpBankSlot->reset();
	
	return	TRUE;
}

//
//	돈 저장
BOOL
CBank::storeGoldToTheBank(int _iGold,DWORD _dwCheckSum,BOOL _bIsCheck)
{
	if (m_iOwnGold < _iGold)	return	FALSE;

	LONGLONG	llGold	=	_iGold+m_bankInfo.m_dwGold;
	
	if (llGold > 0xffffffff)
	{
		if (_bIsCheck)
			CGamePlay::AddSystemMessage(WHITE,dMSG_NO_MORE_STORE_GOLD);

		return	FALSE;
	}

	if (_bIsCheck)	return	TRUE;
	else
	{
		if (_dwCheckSum	!=	getCheckSum())	
			return	FALSE;
	}

	m_bankInfo.m_dwGold	+=	_iGold;
	m_iOwnGold			-=	_iGold;

	return	TRUE;
}

//
//	돈 꺼내기
BOOL
CBank::withdrawGoldFromTheBank(int _iGold,DWORD _dwCheckSum,BOOL _bIsCheck)
{
	if	(m_bankInfo.m_dwGold	< (DWORD)_iGold)
		return	FALSE;

	if	(m_iOwnGold	>=	2000000000)
	{
		if	(_bIsCheck)
			CGamePlay::AddSystemMessage(WHITE,dMSG_CAN_NOT_POSSESS_NO_MORE_THE_GOLD);
		return	FALSE;
	}

	if	(_bIsCheck)
		return	TRUE;
	else
	{
		if	(_dwCheckSum	!=	getCheckSum())	
			return	FALSE;
	}

	LONGLONG	llGold	=	m_iOwnGold	+ _iGold;

	if (llGold > 2000000000)
	{
		if	(m_iOwnGold	>=	2000000000)
			return	TRUE;

		m_bankInfo.m_dwGold	-=	(2000000000-m_iOwnGold);
		m_iOwnGold			=	2000000000;

		return	TRUE;
	}

	m_iOwnGold			+=	_iGold;
	m_bankInfo.m_dwGold	-=	_iGold;

	return	TRUE;
}

BOOL
CBank::moveItemInTheBank(int _iSlot1,int _iSlot2,DWORD _dwCheckSum,BOOL _bIsCheck)
{
	if (_iSlot1	>=	m_wBankSize || _iSlot2 >= m_wBankSize)	return	FALSE;
	if (_iSlot1	==	_iSlot2 )	return	FALSE;

	cItem	*lpSlot1,*lpSlot2;

	lpSlot1	=	(cItem*)&m_bankInfo.m_aItems[_iSlot1];
	lpSlot2	=	(cItem*)&m_bankInfo.m_aItems[_iSlot2];

	if (lpSlot1->m_wBaseItem == 0xffff)	return	FALSE;

	if (_bIsCheck)	return	TRUE;
	else
	{
		if (_dwCheckSum	!=	getCheckSum())	
			return	FALSE;
	}

	cItem	tempItem;

	tempItem.copy(lpSlot1);
	lpSlot1->copy(lpSlot2);
	lpSlot2->copy(&tempItem);

	return	TRUE;
}

BOOL
CBank::moveItemInInventory(int _iSlot1,int _iSlot2,DWORD _dwCheckSum,BOOL _bIsCheck)
{
	if (_iSlot1	>=	m_wInventorySize || _iSlot2 >= m_wInventorySize)
		return	FALSE;
	if (_iSlot1	==	_iSlot2 )
		return	FALSE;

	cItem	*lpSlot1,*lpSlot2;

	lpSlot1	=	&m_aInventoryItem[_iSlot1];
	lpSlot2	=	&m_aInventoryItem[_iSlot2];

	if (lpSlot1->m_wBaseItem == 0xffff)
		return	FALSE;

	if (_bIsCheck)
		return	TRUE;
	else
	{
		if (_dwCheckSum	!=	getCheckSum())	
			return	FALSE;
	}

	cItem	tempItem;

	tempItem.copy(lpSlot1);
	lpSlot1->copy(lpSlot2);
	lpSlot2->copy(&tempItem);

	return	TRUE;
}

cItem*
CBank::getSameItemInBank(cItem *_lpItem,CBankInfo *_lpBankInfo)
{
	for (int i=0;i<m_wBankSize;i++)
	{
		cItem *lpItem	= (cItem *)&_lpBankInfo->m_aItems[i];

		if (lpItem->m_wBaseItem == 0xffff)
			continue;

		if (lpItem->isExactlySameItem(_lpItem))
			return	lpItem;
	}

	return	NULL;
}

//
//	은행거래 수수료
DWORD
CBank::getBankTransactionCharge()
{
	cItem	*lpItem;

	DWORD	dwCharge	=	0;

	for (int i=0;i<m_wBankSize;i++)
	{
		lpItem	= (cItem *)&m_firstBankInfo.m_aItems[i];
		cBasicItem* lpBasicItem = lpItem->getBasicItem();

		if(lpItem->m_wBaseItem == 0xffff || (lpBasicItem && lpBasicItem->isGoldBar()) )		// 09.08.28
			continue;

			if (getSameItemInBank(lpItem,&m_bankInfo) == NULL)	//	찾아간 아이템
			{
				DWORD	dwPrice	=	lpItem->getPrice(lpItem->m_bCount);

				if	(lpItem->isExtraItem())
					dwPrice		=	0;

				LONGLONG	llMoney	=	dwPrice;
				llMoney				*=	m_wBankCharge;	
				llMoney				/=	10000;

				dwCharge		=	(DWORD)(dwCharge+llMoney);
			}

	}

	return	dwCharge;
}


//
//	체크 섬
DWORD
CBank::getCheckSum()
{
	int		i,iCheckSum;
	BYTE	*lpBankBuffer	=	(BYTE *)this;
	DWORD	dwCheckSum	=	0;

	for (i=0;i<strlen(m_bankInfo.m_strId);i++)
		dwCheckSum	+=	m_bankInfo.m_strId[i];

	dwCheckSum	+=	m_bankInfo.m_dwGold;
	dwCheckSum	+=	m_bankInfo.m_dwSaveCounter;
	dwCheckSum	+=	m_bankInfo.m_wRequitalCounter;

	for (i=0;i<dMAX_ITEM_IN_BANK;i++)
	{
		iCheckSum		=	((cItem *)&m_bankInfo.m_aItems[i])->getCheckSum();

		if (iCheckSum)
			dwCheckSum	+=	iCheckSum+(i+1)*3;
	}

	dwCheckSum	+=	m_iOwnGold;
	dwCheckSum	+=	m_wInventorySize;
	dwCheckSum	+=	m_wBankSize;
	dwCheckSum	+=	m_wBankCharge;

	for (i=0;i<dEQUIPMENT_PART_COUNT;i++)
	{
		iCheckSum		=	m_aEquipment[i].getCheckSum();

		if (iCheckSum)
			dwCheckSum	+=	iCheckSum+(i+1)*3;
	}

	for (i=0;i<dOWN_ITEM_COUNT;i++)
	{
		iCheckSum		=	m_aInventoryItem[i].getCheckSum();

		if (iCheckSum)
			dwCheckSum	+=	iCheckSum+(i+1)*3;
	}
	
	return	dwCheckSum;
}

//
//	거래 취소
void
CBank::cancelTransaction()
{
	m_wBankSerial	=	0xffff;
}

//
//	거래 완료
void
CBank::completeTransaction()
{
	m_wBankSerial	=	0xffff;

	DWORD	dwCharge=	getBankTransactionCharge();

	m_iOwnGold		-=	dwCharge;
	g_hero.m_iGold	=	m_iOwnGold;

	memcpy(g_hero.m_aEquip,m_aEquipment,sizeof(g_hero.m_aEquip));
	memcpy(g_hero.m_aItems,m_aInventoryItem,sizeof(g_hero.m_aItems));
}