#include "cACTOR.H"
#include "cFIELD.H"
#include "debugCode.H"
#include "CGame.H"
#include "itemSet.H"
#include "pieceItem.H"
#include "cPACKET_LOGSERVER.H"
#include "packetManager.H"
#include "booking_work.H"
#include "fireWork.H"
#include "itemPackList.H"

//아이템 부수기 관련 보상 테이블 [유니크 레벨][랜덤 보상 토큰]
int l_DisjointingItemTable[10][2]	=	{
	{20 , 0},
	{40 , 1},
	{60 , 2},
	{80 , 3},
	{100 , 4},
	{120 , 5},
	{140 , 6},
	{160 , 7},
	{180 , 8},
};
int l_DisjointingItemTableForJapan[10][2]	=		// 일본분해보상
{
	{10 , 0},
	{20 , 0},
	{30 , 1},
	{40 , 1},
	{50 , 2},
	{60 , 2},
	{70 , 3},
	{80 , 3},
	{90 , 4},
};
int l_UpgradeChanceForJapan[dETERNAL_ITEM_MAX_LEVEL] =	// 일본 강화 확률
{	
	0,
	50000,	//1->2		50%
	40000,	//2->3		40%
	20000,	//3->4		20%
	20000,	//4->5		20%
	20000,	//5->6		20%
	10000,	//6->7		10%
	10000,	//7->8		10%
	10000,	//8->9		10%
	5000,	//9->10		5%

};
int l_UpgradeMaterialsForJapan[dETERNAL_ITEM_MAX_LEVEL][4] =	// 일본 강화 아이템 재료
{	// 돈 , 신비석, 탄틸리스유물, 결정석//
	{0, 0, 0, 0},

	{10000000, 1, 1,0},		// 1 -> 2
	{20000000, 1, 1,0},		// 2 -> 3
	{30000000, 1, 1,0},		// 3 -> 4
	{40000000, 2, 2,1},		// 4 -> 5
	
	{40000000, 2, 2,1},		// 5 -> 6
	{50000000, 2, 2,2},		// 6 -> 7
	{50000000, 2, 2,2},		// 7 -> 8
	{60000000, 2, 2,3},		// 8 -> 9
	{60000000, 2, 2,3},		// 9 -> 10

};

#define		dRAND_VALUE_FOR_DISJOINTING_ITEM_FOR_RANDOM_TOKKEN	20
enum		// 재료 enum 09.08.26
{
	eMATERIALS_GOLD,
	eMATERIALS_STONEOFMYSTERY,
	eMATERIALS_TANTILLESS,
	eMATERIALS_CRYSTAL,

};

int l_UpgradeMaterials[dETERNAL_ITEM_MAX_LEVEL][4] =	// 강화 아이템 재료 09.08.26
{	// 돈 , 신비석, 탄틸리스유물, 결정석// - 마지막 수정일 : 09.09.23
	{0, 0, 0, 0},

	{1000000, 1, 1,0},		// 1 -> 2
	{2000000, 1, 1,0},		// 2 -> 3
	{3000000, 1, 1,0},		// 3 -> 4
	{4000000, 2, 2,1},		// 4 -> 5
	
	{4000000, 2, 2,1},		// 5 -> 6
	{5000000, 2, 2,2},		// 6 -> 7
	{5000000, 2, 2,2},		// 7 -> 8
	{6000000, 2, 2,3},		// 8 -> 9
	{6000000, 2, 2,3},		// 9 -> 10

};
//
//	사용 가능한 아이템이냐?
BOOL
cACTOR::isUseAbleItem(cItem *_lpItem,BOOL _bIsCheckSecondJob)
{
	cBasicItem	*lpBasicItem		=	_lpItem->getBasicItem(TRUE);

	if	(!lpBasicItem										)
		return	FALSE;
	if	(_lpItem->isExceptionItem()	==	FALSE	&&	_lpItem->m_isBroken	)
		return	FALSE;
	if	(_lpItem->isBreakdownAbleItem() && _lpItem->m_bDurability == 0	)
		return	FALSE;
	if	(isInterruptingArmor(_lpItem))
		return	FALSE;
	if(isInterruptingWeapon(_lpItem))	// 무기해제.
		return	FALSE;
	cSetItem* pSetItem = _lpItem->getSetItem();

	if	(pSetItem)
	{	// 세트아이템 체크....
		BOOL bSet = pSetItem->isEnableJob(m_wJob);

		if	(_bIsCheckSecondJob)		// 보조직업 검사 추가..
			bSet += getAnotherJob();

		if	(bSet == FALSE)
			return FALSE;
	}

	if	(lpBasicItem->m_questIndex	!=	dNOT_QUEST_ITEM)
		if	(lpBasicItem->m_questIndex<	dNOT_QUEST_ITEM)	
			return	FALSE;

	{
		if	(_lpItem->isNotDefineCoupleCoupleRing() || _lpItem->isPresentCoupleRing())
			return	FALSE;
	}

	BOOL	bIsUseAbleJob	=	_lpItem->isEnableJob(m_wJob);

	if	(!lpBasicItem->m_bf1IsMaleJobEquipAble)	
	{
		if	(c_aIsMale[m_wJob])
			return	FALSE;		//	남자는 착용 불가

		bIsUseAbleJob	=	TRUE;
	}
	if	(!lpBasicItem->m_bf1IsFemaleJobEquipAble)	
	{
		if	(!c_aIsMale[m_wJob])	
			return	FALSE;		//	여자는 착용 불가

		bIsUseAbleJob	=	TRUE;
	}
	
	if(m_wTransformationTime)		// 변신 중이면 ...
	{
		if(!lpBasicItem->m_bf1IsUseItemAfterTransformation)
			return FALSE;
	}
//	직업 체크
	if	(bIsUseAbleJob	==	FALSE)
	{
		if	(lpBasicItem->m_wKind	==	eIK_BULLET)
			if	(m_bf1IsUseAbleBulletSubstituteArrow)
				bIsUseAbleJob	=	TRUE;

		if	(bIsUseAbleJob	==	FALSE)
		{
			if	(!_bIsCheckSecondJob)
				return	FALSE;

			int	iSecondJob	=	getAnotherJob();
			
			if	(!lpBasicItem->isEquipAbleJob(iSecondJob))	
				return	FALSE;
		}
	}

//	성향 체크
	switch(lpBasicItem->m_wRequireAllignment)
	{
		case	dALLIGNMENT_ABSOLUTE_EVIL	:	//	절대 악
			if (GetAllignment(m_sAllignment)	!=	dALLIGNMENT_ABSOLUTE_EVIL		)
				return	FALSE;
			break;

		case	dALLIGNMENT_EVIL			:	//	악
			if (GetAllignment(m_sAllignment)	!=	dALLIGNMENT_ABSOLUTE_EVIL	&&
				GetAllignment(m_sAllignment)	!=	dALLIGNMENT_EVIL				)
				return	FALSE;
			break;

		case	dALLIGNMENT_NEUTRAL			:	//	중립
			if (GetAllignment(m_sAllignment)	!=	dALLIGNMENT_NEUTRAL				)
				return	FALSE;
			break;

		case	dALLIGNMENT_GOOD			:	//	선
			if (GetAllignment(m_sAllignment)	!=	dALLIGNMENT_ABSOLUTE_GOOD	&&
				GetAllignment(m_sAllignment)	!=	dALLIGNMENT_GOOD				)
				return	FALSE;
			break;

		case	dALLIGNMENT_ABSOLUTE_GOOD	:	//	절대 선
			if (GetAllignment(m_sAllignment)	!=	dALLIGNMENT_ABSOLUTE_GOOD		)
				return	FALSE;
			break;
	}
	
	if	(_lpItem->isSpecialItem())
	{
		if	(_lpItem->getSpecialItem()->m_bf1IsFreeUseItem)	//	능력치에 상관 없이 사용 가능한 아이템
			return	TRUE;
	}

	int	iRequireLevel		=	_lpItem->getRequireLevel();

	if (!_lpItem->isCanUseLevel(getLevel(),m_wLimitEquipmentLevelOverPoint))
		return FALSE;
	if (getStrength()		<	lpBasicItem->m_wRequireStrength		)
		return	FALSE;	//	사용/장비를 위한 최소 힘
	if (getAgility()		<	lpBasicItem->m_wRequireDexterity	)
		return	FALSE;	//	사용/장비를 위한 최소 민첩성
	if (getConstitution()	<	lpBasicItem->m_wRequireConstitution	)
		return	FALSE;	//	사용/장비를 위한 최소 체질
	if (getWisdom()			<	lpBasicItem->m_wRequireWisdom		)
		return	FALSE;	//	사용/장비를 위한 최소 지혜
	if (getIntelligence()	<	lpBasicItem->m_wRequireIntelligence	)
		return	FALSE;	//	사용/장비를 위한 최소 지식
	if (getCharisma()		<	lpBasicItem->m_wRequireCharisma		)
		return	FALSE;	//	사용/장비를 위한 최소 카리스마
	if (getLuck()			<	lpBasicItem->m_wRequireLuck			)
		return	FALSE;	//	사용/장비를 위한 최소 운

//	기타 체크
	int		iValue			=	lpBasicItem->m_wEquipUseLimitValue;
	int		iContents		=	lpBasicItem->m_wEquipUseLimitContents;
	int		iMethod			=	lpBasicItem->m_wEquipUseLimitMethod;
	int		iContentsValue	=	0;

	if	(iContents	==	0xffff)
		return	TRUE;

	switch(iContents)
	{
		case	0	:	
			iContentsValue	=	getLevel();
			break;//"레벨",
		case	1	:	
			iContentsValue	=	getStrength();
			break;//"힘",
		case	2	:	
			iContentsValue	=	getAgility();
			break;//"민첩성",
		case	3	:	
			iContentsValue	=	getConstitution();
			break;//"건강",
		case	4	:	
			iContentsValue	=	getCharisma();
			break;//"카리스마",
		case	5	:	
			iContentsValue	=	getIntelligence();
			break;//"지식",
		case	6	:	
			iContentsValue	=	getWisdom();
			break;//"지혜",
		case	7	:	
			iContentsValue	=	getLuck();
			break;//"행운",
	}

	switch(iMethod)
	{
		case	0xffff	:
			if	(iContentsValue	<	iValue)	
				return	FALSE;
			break;

		case	0		:
			iValue	*=	(int )_lpItem->m_aOption[0];
			if	(iContentsValue	<	iValue)	
				return	FALSE;
			break;

		case	1		:
			iValue	*=	(int )_lpItem->m_aOption[1];
			if	(iContentsValue	<	iValue)	
				return	FALSE;
			break;

		case	2		:
			iValue	=	(int )_lpItem->m_aOption[0]/iValue;
			if	(iContentsValue	<	iValue)	
				return	FALSE;
			break;

		case	3		:
			iValue	=	(int )_lpItem->m_aOption[1]/iValue;
			if	(iContentsValue	<	iValue)	
				return	FALSE;
			break;
	}

	return	TRUE;
}
//
//	이미 존재하는 아이템
BOOL
cACTOR::isExistItem(cItem *_lpItem,BOOL _bIsStripItem)
{
	int		i,iCount=0;
	
	if(_lpItem->m_wBaseItem == 4006)
		iCount += 0;

	for (i=0;i<dOWN_ITEM_COUNT;i++)
	{
		if (m_aItems[i].m_wBaseItem		==	0xffff				)	continue;
		if (m_aItems[i].m_dwSerial		<=	1					)	continue;
		if (m_aItems[i].getStackLimit()	>	1					)	continue;
		if (m_aItems[i].m_dwSerial		!=	_lpItem->m_dwSerial	)	continue;

		if (m_aItems[i].isSameItem(_lpItem)						)	iCount++;
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		if (m_aExtraInventory[i].m_wBaseItem		==	0xffff			)	continue;
		if (m_aExtraInventory[i].m_dwSerial		<=	1					)	continue;
		if (m_aExtraInventory[i].getStackLimit()	>	1				)	continue;
		if (m_aExtraInventory[i].m_dwSerial		!=	_lpItem->m_dwSerial	)	continue;

		if (m_aExtraInventory[i].isSameItem(_lpItem)					)	iCount++;
	}

	for (i=0;i<dEQUIPMENT_PART_COUNT;i++)
	{
		if (m_aEquip[i].m_wBaseItem		==	0xffff				)	continue;
		if (m_aEquip[i].m_dwSerial		<=	1					)	continue;
		if (m_aEquip[i].getStackLimit()	>	1					)	continue;
		if (m_aEquip[i].m_dwSerial		!=	_lpItem->m_dwSerial	)	continue;

		if (m_aEquip[i].isSameItem(_lpItem)						)	iCount++;
	}

	if (_bIsStripItem)	
	{
		if (iCount	>=	2)	return	TRUE;

		return	FALSE;
	}

	if (iCount)	return	TRUE;

	return	FALSE;
}

//
//	인벤토리에 아이템을 추가한다
int
cACTOR::addGoldItemToInventory(cItem *_lpItem,int _iMethod)
{
	return	addItemToInventory(_lpItem,FALSE,_iMethod);
}
//
//	인벤토리에 아이템을 추가한다
int
cACTOR::addItemToInventory(cItem *_lpItem,BOOL _bIsStripItem,int _iMethod)
{
	if	(m_wIsWaitCartMessage	&&	getRemainItemSlotCount() == 1)
		return	eUPR_INVENTORY_FULL;	//	크하하하~~

	if	(_lpItem->m_wBaseItem	==	dITEM_INDEX_GOLD_BAR)
	{
		LONGLONG	llGold	=	_lpItem->m_bCount;

		llGold	*=	100000000;

		updateGoldLog(llGold,_iMethod);
	}

	if	(_lpItem->m_wBaseItem	==	dITEM_MONEY)
	{
		LONGLONG	llGold	=	m_iGold+_lpItem->m_dwSerial;

		if	(llGold	>	dMAX_OWN_GOLD)
		{
			if (m_iGold	>=	dMAX_OWN_GOLD)
				return	eUPR_CAN_NOT_OWN_MORE_GOLD;

			int	iAddGold		=	(dMAX_OWN_GOLD-m_iGold);

			_lpItem->m_dwSerial	-=	iAddGold;
			
			m_iGold	=	dMAX_OWN_GOLD;

			if (_lpItem->m_dwSerial	==	0)
				_lpItem->reset();

			updateGoldLog(iAddGold,_iMethod);

			return	eUIR_OK;
		}

		m_iGold	+=	_lpItem->m_dwSerial;

		updateGoldLog(_lpItem->m_dwSerial,_iMethod);

		_lpItem->reset();

		return	eUIR_OK;
	}

	if	(isExistItem(_lpItem,_bIsStripItem))
	{
		sendDoubleItemLog(_lpItem,eDIL_PICK);
		_lpItem->reset();

		return	eUPR_PICK_ITEM_IS_COPY_ITEM;
	}

	cBasicItem	*lpBasicItem	=	_lpItem->getBasicItem();

	if	(lpBasicItem)
	{
		if (lpBasicItem->m_attr.isCanNotOwnSameItem)
			if (getItemByBasicItem(_lpItem->m_wBaseItem))
				return	eUAIR_CAN_NOT_OWN_SAME_ITEM;

		if (lpBasicItem->m_attr.isBadge)
			if (getBadgeCount()	>=	dLIMIT_BADGE_ITEM_COUNT_IN_INVENTORY)
				return	eUAIR_TOO_MANY_BADGE_ITEM;
	}

	int	iCount	=	_lpItem->m_bCount;

	if	(_lpItem->isStackAbleItem())	//	겹칠 수 있는 아이템이다.
	{
		cItem	*lpSameItem	=	getStackAbleItem(_lpItem);

		while(lpSameItem)	//	같은 아이템이 있다면 겹치자.
		{
			lpSameItem->addItem(_lpItem,lpSameItem->getStackLimit());

			if (_lpItem->m_wBaseItem	==	0xffff)	break;

			lpSameItem	=	getStackAbleItem(_lpItem);
		}

		if (_lpItem->m_wBaseItem	==	0xffff)
		{
			arrangeInventory();

			return	eUIR_OK;	//	위에서 겹쳐-_-졌나 보다.
		}
	}

	cItem	*lpEmptySlot	=	getEmptyItemSlot();

	if	(!lpEmptySlot)
	{
		if	(iCount	!=	_lpItem->m_bCount)
		{
			arrangeInventory();

			return	eUIR_OK;
		}

		return	eUPR_INVENTORY_FULL;	//	크하하하~~
	}

	lpEmptySlot->copy(_lpItem);
	_lpItem->reset();

	if	(lpEmptySlot->getBasicItem()	&&	lpEmptySlot->m_bDurability	==	0	&&	lpEmptySlot->getBasicItem()->m_attr.isDurabilityToUseCount)
		lpEmptySlot->reset();

	if	(lpBasicItem && lpBasicItem->m_attr.isEffectInInventoryItem)
		buildPower();

	arrangeInventory();

	return	eUIR_OK;
}

int
cACTOR::getEquipItemRebirthCount(int _iRebirthCount, BOOL _isIgnoreRingOfInfinity)		
{	// 인벤토리에 
	int		iCount = 0;
	
	for (int i=0;i<dEQUIPMENT_PART_COUNT;++i)
	{
		cItem	*lpItem	=	&m_aEquip[i];
		if(!lpItem)
			continue;
		
		if (lpItem->m_wBaseItem	==	0xffff)
			continue;
		cBasicItem* lpBasic = lpItem->getBasicItem();
		if(!lpBasic)
			continue;

		if(!_isIgnoreRingOfInfinity && !lpBasic->m_bf1IsRingOfInfinity)
			continue;

		if(lpBasic->m_bf4ItemRebirthCount == _iRebirthCount)
			++iCount;
	
	}
	return iCount;
}

BOOL
cACTOR::removeItem(int _iItem,int _iCount,int _iIncludeEquipment,int _iReason,int _iRemoveGBReason)
{
	if	(!isPlayer())
		return	FALSE;

	int	iRemovedCount	=	_removeItem(_iItem,_iCount,_iIncludeEquipment,_iReason);

	iRemovedCount	=	max(iRemovedCount,0);

	if	(_iItem	==	dITEM_INDEX_GOLD_BAR && _iReason != eRI_BY_PITCHMANSHOP)
		//	노점상 거래시 골드량 변화는 유저끼리의 것이므로 변화량 체크할 필요 없음
	{
		LONGLONG	llGold	=	iRemovedCount;

		llGold	*=	100000000;

		updateGoldLog(-llGold,_iRemoveGBReason);
	}

	immediatelySendSaveDataToDBMemory();		// 데이터베이스 저장..+_+;;

	buildPower();

	
	if	(_iReason == eRI_QUEST)		
		_iReason	=	eRIR_NORMAL;

	sendRemoveItem(_iItem,iRemovedCount,FALSE,_iReason,_iIncludeEquipment);

	return	TRUE;
}


int
cACTOR::_removeItem(int _iItem,int _iCount,int _iIncludeEquipment,int _iReason)		// 09.08.21
{
	int	iRemoveItemCount	=	_iCount,i=0;
	int	iRemovedItemCount	=	0;

	if	(_iCount	==	0)
		iRemoveItemCount	=	0xffff;

	if	(_iIncludeEquipment	==	0)
		i	=	dEQUIP_BELT_SLOT1;

	for (;i<dEQUIPMENT_PART_COUNT;i++)
	{
		cItem	*lpItem	=	&m_aEquip[i];

		if	(lpItem->m_wBaseItem	==	0xffff)
			continue;
		if	(lpItem->m_wBaseItem	!=	_iItem)
			continue;

		cItem	itemRemove;

		itemRemove.copy(lpItem);

		int	iItemCount		=	lpItem->m_bCount;

		lpItem->decreaseCount(iRemoveItemCount);

		iRemovedItemCount	+=	min(iItemCount,iRemoveItemCount);
		iRemoveItemCount	-=	iItemCount;

		itemRemove.m_bCount	=	iItemCount-itemRemove.m_bCount;

		if	(_iReason != eRI_BY_PITCHMANSHOP)
			sendRemoveItemLog(&itemRemove,_iReason);

		if	(iRemoveItemCount	<=	0)
			return	iRemovedItemCount;
	}

	for (i=0;i<dOWN_ITEM_COUNT;i++)
	{
		cItem	*lpItem	=	&m_aItems[i];

		if	(lpItem->m_wBaseItem	==	0xffff)
			continue;
		if	(lpItem->m_wBaseItem	!=	_iItem)
			continue;

		cItem	itemRemove;

		itemRemove.copy(lpItem);

		int	iItemCount		=	lpItem->m_bCount;

		lpItem->decreaseCount(iRemoveItemCount);

		iRemovedItemCount	+=	min(iItemCount,iRemoveItemCount);
		iRemoveItemCount	-=	iItemCount;

		itemRemove.m_bCount	=	iItemCount-itemRemove.m_bCount;

		if	(_iReason != eRI_BY_PITCHMANSHOP)
			sendRemoveItemLog(&itemRemove,_iReason);

		if	(iRemoveItemCount	<=	0)
			return	iRemovedItemCount;
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		cItem	*lpItem	=	&m_aExtraInventory[i];

		if	(lpItem->m_wBaseItem	==	0xffff)
			continue;
		if	(lpItem->m_wBaseItem	!=	_iItem)
			continue;

		cItem	itemRemove;

		itemRemove.copy(lpItem);

		int	iItemCount	=	lpItem->m_bCount;

		lpItem->decreaseCount(iRemoveItemCount);

		iRemovedItemCount	+=	min(iItemCount,iRemoveItemCount);
		iRemoveItemCount	-=	iItemCount;

		itemRemove.m_bCount	=	iItemCount-itemRemove.m_bCount;

		if	(_iReason != eRI_BY_PITCHMANSHOP)
			sendRemoveItemLog(&itemRemove,_iReason);

		if	(iRemoveItemCount	<=	0)
			return	iRemovedItemCount;
	}

	return	iRemovedItemCount;
}
//
//	장비를 착용한다.
int
cACTOR::equipItem(cItem *_lpItem,int _iPart,int *_lpiResultPart)
{
	if	(_iPart	!=	0xffff)
		if	(_iPart	<	0	||	_iPart	>=	dEQUIPMENT_PART_COUNT)
			return	eUER_WEAR_LIMIT;
	
	cItem	tempItem;
	int		iKind				=	_lpItem->getBasicItem(TRUE)->m_wKind;
	int		iPart				=	_iPart;
	int		iLimitCount			=	0xffff;

	if	(*_lpiResultPart)
		*_lpiResultPart			=	_iPart;

	if(_lpItem->getBasicItem(TRUE)->m_bf3WearLimit)
	{

		int iCount = getSameEquipItemCount(_lpItem);
		if(iCount >= _lpItem->getBasicItem(TRUE)->m_bf3WearLimit)
			return eUER_WEAR_LIMIT;
	}

	cBasicItem* lpBasic = _lpItem->getBasicItem();

	if(lpBasic)
	{
		if(lpBasic->m_bf1IsRingOfInfinity)
		{
			if(lpBasic->m_bf4ItemRebirthCount > m_bf4RebirthCount)
				return	eUER_LACK_ABILITY;	//	착용하기에 능력치가 모자른다.
			int iCount = getEquipItemRebirthCount(lpBasic->m_bf4ItemRebirthCount, FALSE);
			if(iCount >= dLIMIT_EQUIP_RING_OF_INFINITY_COUNT)
				return eUER_WEAR_LIMIT;
		}
	}

	if	(!isUseAbleItem(_lpItem,TRUE))
		return	eUER_LACK_ABILITY;	//	착용하기에 능력치가 모자른다.

	if	(_iPart	<	dEQUIP_BELT_SLOT1 || iPart	==	0xffff)
	{
		if	(c_aItemEquipPlace[iKind]	==	dITEM_KIND_WASTING)
			return	eUER_NOT_EQUIPMENT_ITEM;//	소비형 아이템이다.

		if	(iKind	==	eIK_RING)
		{
			if (iPart<dEQUIP_RING1	||	iPart>dEQUIP_RING8)
				return	eUER_NOT_RING_PLACE;	//	반지의 경우는 제자리에 끼워야 한다.

			if (_lpItem->is4FingerRing())
			{
				if (_iPart != dEQUIP_RING1 && _iPart != dEQUIP_RING5)
					return	eUER_NOT_RING_PLACE;	//	반지의 경우는 제자리에 끼워야 한다.

				if (_iPart == dEQUIP_RING1)
				{
					if (m_aEquip[dEQUIP_RING2].m_wBaseItem != 0xffff)	return	eUER_NOT_FIST_RING_PLACE;	//	반지의 경우는 제자리에 끼워야 한다.
					if (m_aEquip[dEQUIP_RING3].m_wBaseItem != 0xffff)	return	eUER_NOT_FIST_RING_PLACE;	//	반지의 경우는 제자리에 끼워야 한다.
					if (m_aEquip[dEQUIP_RING4].m_wBaseItem != 0xffff)	return	eUER_NOT_FIST_RING_PLACE;	//	반지의 경우는 제자리에 끼워야 한다.
				}

				if (_iPart == dEQUIP_RING5)
				{
					if (m_aEquip[dEQUIP_RING6].m_wBaseItem != 0xffff)	return	eUER_NOT_FIST_RING_PLACE;	//	반지의 경우는 제자리에 끼워야 한다.
					if (m_aEquip[dEQUIP_RING7].m_wBaseItem != 0xffff)	return	eUER_NOT_FIST_RING_PLACE;	//	반지의 경우는 제자리에 끼워야 한다.
					if (m_aEquip[dEQUIP_RING8].m_wBaseItem != 0xffff)	return	eUER_NOT_FIST_RING_PLACE;	//	반지의 경우는 제자리에 끼워야 한다.
				}
			}
		}
		else
		{
			if	(iPart	==	0xffff	)
			{
				iPart	=	c_aItemEquipPlace[iKind];	//	장비 슬롯이 아닌 곳에 올렸다. 적당히 빈자리 찾아서 넣는다.

				if	(iPart	==	dEQUIP_WEAPON)
				{
					if	(m_wJob	%2)
					{
						if	(_lpItem->isEnableJob(m_wJob))
							iPart	=	dEQUIP_WEAPON_2;
					}
					else
					{
						if	(!_lpItem->isEnableJob(m_wJob)	&&	_lpItem->isEnableJob(getAnotherJob()))
							iPart	=	dEQUIP_WEAPON_2;
					}
				}
			}
			else
			if	(c_aItemEquipPlace[iKind]!=	iPart	)
			{
				if	(c_aItemEquipPlace[iKind]	!=	dEQUIP_WEAPON	||	iPart	!=	dEQUIP_WEAPON_2)
					iPart	=	c_aItemEquipPlace[iKind];//	엉뚱한 위치에 있다.
			}

			if	(iPart	==	dEQUIP_WEAPON_2)
				if	(m_wJob/2	==	dJOB_BEAST_TAMER/2)
					iPart	=	dEQUIP_WEAPON;
		}
	}
	else
		iLimitCount	=	min(m_wBeltStackCount,_lpItem->getStackLimit());

	if	(iLimitCount==	0)
		return	eUER_BELT_NOT_FOUND;

	if	(iPart	==	dEQUIP_SHIELD_BULLET	&&	m_wCastBlocker			!=	0xffff )
		return	eUSR_CAN_NOT_CHANGE_SHIELD_STATUS;

	if	(iPart	==	dEQUIP_WEAPON			&&	m_bf10CastWeaponBlocker	!=	dVALID_BLOCKER_CODE)
		return	eUSR_CAN_NOT_CHANGE_WEAPON_STATUS;

	if	(iPart == dEQUIP_WEAPON		&& m_wHideWeapon1Time)
		return	eUER_PICK_WEAPON;
	if	(iPart == dEQUIP_WEAPON_2	&& m_wHideWeapon2Time)
		return	eUER_PICK_WEAPON;

	if	(*_lpiResultPart)
		*_lpiResultPart	=	iPart;

//	직업 체크
	{
		BOOL	bIsExceptCondition	=	FALSE;

		if	(iKind	==	eIK_BULLET	&&	iPart	==	dEQUIP_SHIELD_BULLET)
			if	(m_bf1IsUseAbleBulletSubstituteArrow)
				bIsExceptCondition	=	TRUE;

		if	(bIsExceptCondition	==	FALSE)
		{
			int	iCheckJob	=	m_wJob;

			if	((m_wJob%2)	&&	iPart	==	dEQUIP_WEAPON)
				iCheckJob	=	getAnotherJob();
			else
			if	((m_wJob%2)==0	&&	iPart	==	dEQUIP_WEAPON_2)
				iCheckJob	=	getAnotherJob();

			if	(!_lpItem->isEnableJob(iCheckJob))
				return	eUER_WRONG_PLACE;
		}
	}

	if	(iLimitCount	==	0xffff)	//	일반 아이템
	{
		if	(m_aEquip[iPart].m_wBaseItem	!=	0xffff)		//	이미 뭔가를 장비하고 있다.
		{
			if	(!m_aEquip[iPart].isFull())//겹칠 수 있는거고...
			{
				if	(m_aEquip[iPart].isSameItem(_lpItem))//같은거.. -o-
				{
					m_aEquip[iPart].addItem(_lpItem);

					arrangeInventory();
					buildPower();

					return	eUIR_OK;
				}
			}

			m_aEquip[iPart].swapItem(_lpItem);
		}
		else
			m_aEquip[iPart].swapItem(_lpItem);

		m_aEquip[iPart].operateReversionItemWhenEquip();

		arrangeInventory();
		buildPower();

		return	eUIR_OK;
	}

//	벨트에 아이템을 장착하는 거다.
	if (m_aEquip[iPart].m_wBaseItem	!=	0xffff && m_aEquip[iPart].m_bCount > 0)		//	이미 뭔가를 장비하고 있다.
	{
		if (!m_aEquip[iPart].isFull(iLimitCount))//겹칠 수 있는거고...
		{
			if (m_aEquip[iPart].isSameItem(_lpItem))//같은거.. -o-
			{
				m_aEquip[iPart].addItem(_lpItem,iLimitCount);
				m_aEquip[iPart].operateReversionItemWhenEquip();

				arrangeInventory();
				buildPower();

				return	eUIR_OK;
			}
		}

		if (_lpItem->m_bCount	>	iLimitCount)	//	아이템 위치를 바꾸려고 하는데, 아이템 갯수가 벨트 스택 아이템 갯수 보다 많다.
		{
			cItem	*lpValidInventorySlot	=	getValidInventorySlot();

			if (lpValidInventorySlot)
			{
				m_aEquip[iPart].swapItem(_lpItem);

				lpValidInventorySlot->copy(&m_aEquip[iPart]);

				int	iSpareItemCount				=	m_aEquip[iPart].m_bCount-iLimitCount;

				m_aEquip[iPart].m_bCount		=	iLimitCount;
				lpValidInventorySlot->m_bCount	=	iSpareItemCount;
			}
			else
				return	eUER_INVENTORY_FULL;	//	인벤토리가 꽉차있다.
		}
		else
			m_aEquip[iPart].swapItem(_lpItem);

		m_aEquip[iPart].operateReversionItemWhenEquip();
	}
	else
	{
		if (_lpItem->m_bCount	<=	iLimitCount)
		{
			m_aEquip[iPart].swapItem(_lpItem);

			_lpItem->checkItemCount();
		}
		else	//	아이템 위치를 바꾸려고 하는데, 아이템 갯수가 벨트 스택 아이템 갯수 보다 많다.
		{
			m_aEquip[iPart].copy(_lpItem);
			m_aEquip[iPart].m_bCount	=	iLimitCount;
			_lpItem->m_bCount			=	_lpItem->m_bCount-iLimitCount;
		}

		m_aEquip[iPart].operateReversionItemWhenEquip();
	}

	arrangeInventory();
	buildPower();

	return eUIR_OK;
}

//
//	아이템 카운트 줄이기
int
cACTOR::decreaseItemCount(cItem *_lpItem,BOOL _isBeltSlotItem)
{
	cBasicItem	*lpBasicItem	=	_lpItem->getBasicItem();

	if	(!lpBasicItem)
		return	eUIR_FAILED;

	if	(lpBasicItem->m_attr.isDurabilityToUseCount)
	{
		if	(_lpItem->m_bDurability	==	0)
			if	(!_lpItem->decreaseCount(1,_isBeltSlotItem)		)
				return	eUIR_FAILED;

		_lpItem->m_bDurability--;

		if	(_lpItem->m_bDurability	==	0)
			if (!_lpItem->decreaseCount(1,_isBeltSlotItem)		)
				return	eUIR_FAILED;
	}
	else
		if	(!_lpItem->decreaseCount(1,_isBeltSlotItem)		)
			return	eUIR_FAILED;

	return	eUIR_OK;
}

//
//	같은 아이템을 찾아서 리턴한다.
cItem*
cACTOR::getSameItem(cItem *_lpItem)
{
	int		i;

	for (i=0;i<dOWN_ITEM_COUNT;i++)
	{
		cItem	*lpItem	=	&m_aItems[i];

		if (lpItem->m_wBaseItem	==	0xffff)	
			continue;

		if (lpItem->isSameItem(_lpItem))
			return	lpItem;
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		cItem	*lpItem	=	&m_aExtraInventory[i];

		if (lpItem->m_wBaseItem	==	0xffff)	
			continue;

		if (lpItem->isSameItem(_lpItem))
			return	lpItem;
	}

	return	NULL;
}
int
cACTOR::getSameInventoryItemCount(cItem* _lpItem)		//09.08.11
{
	int		iCount = 0;
	
	for (int i=0;i<m_wItemSlotCount;++i)
	{
		cItem	*lpItem	=	&m_aItems[i];
		
		if (lpItem->m_wBaseItem	==	0xffff)
			continue;
		
		if (lpItem->isSameItem(_lpItem)){
			++iCount;
		}
			
		cBasicItem* lpBasicItem = lpItem->getBasicItem(TRUE);
	}
	return iCount;

	
}
int
cACTOR::getSameEquipItemCount(cItem* _lpItem)		
{	// 인벤토리에 착용중인 아이템중에 찾을 아이템 시리얼이 같은 아이템 수를 리턴한다.
	int		iCount = 0;
	
	if(!_lpItem || _lpItem->m_wBaseItem == 0xffff)
		return iCount;

	for (int i=0;i<dEQUIPMENT_PART_COUNT;++i)
	{
		cItem	*lpItem	=	&m_aEquip[i];
		if(!lpItem)
			continue;
		
		if (lpItem->m_wBaseItem	==	0xffff)
			continue;
		
		if(_lpItem->m_wBaseItem == lpItem->m_wBaseItem)
			++iCount;
		
	}
	return iCount;
}
//
//	같은 아이템을 찾아서 리턴한다.
cItem*
cACTOR::getStackAbleItem(cItem *_lpItem)
{
	int		i;

	for (i=0;i<dOWN_ITEM_COUNT;i++)
	{
		cItem	*lpItem	=	&m_aItems[i];

		if (lpItem->m_wBaseItem	==	0xffff)
			continue;

		if (lpItem->isSameItem(_lpItem))
			if (!lpItem->isFull())
				return	lpItem;
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		cItem	*lpItem	=	&m_aExtraInventory[i];

		if (lpItem->m_wBaseItem	==	0xffff)
			continue;

		if (lpItem->isSameItem(_lpItem))
			if (!lpItem->isFull())
				return	lpItem;
	}

	return	NULL;
}

//
//	같은 아이템을 찾아서 리턴한다.
cItem*
cACTOR::getSameKindItem(cItem *_lpItem)
{
	int	i,iKind	=	_lpItem->getBasicItem(TRUE)->m_wKind;

	for (i=0;i<dOWN_ITEM_COUNT;i++)
	{
		cItem	*lpItem	=	&m_aItems[i];

		if	(lpItem->m_wBaseItem	==	0xffff)
			continue;

		if	(lpItem->getBasicItem(TRUE)->m_wKind	==	iKind)
			return	lpItem;
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		cItem	*lpItem	=	&m_aExtraInventory[i];

		if	(lpItem->m_wBaseItem	==	0xffff)
			continue;

		if	(lpItem->getBasicItem(TRUE)->m_wKind	==	iKind)
			return	lpItem;
	}

	return	NULL;
}

//
//	벨트 슬롯에 특정 아이템 채우기
BOOL
cACTOR::fillBeltSlot(cItem *_lpItem,int _iSlot)
{
	cItem		*lpSlot;
	cBasicItem	*lpBasicItem	=	_lpItem->getBasicItem();

	if	(!_lpItem->isBeltStackAbleItem())
		return	FALSE;

	lpSlot	=	&m_aEquip[dEQUIP_BELT_SLOT1+_iSlot];

	if (lpSlot->m_wBaseItem	!=	_lpItem->m_wBaseItem	&&	lpSlot->m_bCount	>	0)	//	다른 종류
	{
		lpSlot->swapItem(_lpItem);
		arrangeInventory();

		return	TRUE;
	}
	int		iLimitCount		=	min(_lpItem->getStackLimit(),m_wBeltStackCount);
	
//	int		iLimitCount		=	m_wBeltStackCount;

	if (lpSlot->m_bCount	>=	iLimitCount)	return	FALSE;

	int		iRemainCount	=	iLimitCount-lpSlot->m_bCount;

	if (_lpItem->m_bCount	<	iRemainCount)	iRemainCount	=	_lpItem->m_bCount;

	lpSlot->m_bCount		+=	iRemainCount;
	_lpItem->m_bCount		-=	iRemainCount;

	_lpItem->checkItemCount();

	arrangeInventory();

	return	TRUE;
}

//
//	리로드
BOOL
cACTOR::reloadBeltSlot()
{
	if	(!isAvailReloadBeltSlot())
		return	FALSE;

	if	(m_isAutoReload	||	m_sIsReloadingBeltItem)
		return	TRUE;

	setBeltItemReloadTime();

	m_sIsReloadingBeltItem	=	TRUE;

	return	TRUE;
}

//
//	벨트 아이템 리로드 타임 설정
void
cACTOR::setBeltItemReloadTime()
{
	int	iReloadTime		=	3200/(100+getAgility()*2)*(100+m_bCorrectReloadSpeed)/100;
	if (m_isAutoReload)	iReloadTime	/=	2;
	m_sReloadTime		=	iReloadTime;
}

//
//	벨트에 아이템을 리로드 한다.
BOOL
cACTOR::operateReloadBeltSlot()
{
	if	(m_sIsReloadingBeltItem	== FALSE && !m_isAutoReload)
		return FALSE;

	if	(m_bIsMoving)
	{
		m_sIsReloadingBeltItem	=	FALSE;
		setBeltItemReloadTime();

		return	FALSE;
	}

	m_sReloadTime--;

	if	(m_sReloadTime	>	0)	
		return	FALSE;

	setBeltItemReloadTime();

	int		i;
	cItem	*lpSlot;
	BOOL	isReload	=	FALSE;

	for (i=dEQUIP_BELT_SLOT1;i<=dEQUIP_BELT_SLOT5;)
	{
		lpSlot	=	&m_aEquip[i];

		i++;

		if	(lpSlot->m_wBaseItem	==	0xffff				)
			continue;

		int	iLimitCount			=	min(m_wBeltStackCount,lpSlot->getStackLimit());
		//int	iLimitCount			=	m_wBeltStackCount;

		if	(lpSlot->m_bCount	>=	iLimitCount	)
			continue;

		cItem	*lpItem		=	getSameItem(lpSlot);

		if	(!lpItem)
			continue;

		if	(lpItem->m_bCount	==	0)
		{
			lpItem->reset();
			continue;
		}

		if	(!lpSlot->addItem(lpItem,iLimitCount))
			continue;

		if	(lpSlot->m_bCount && lpSlot->m_bCount < iLimitCount) 
			i--;

		isReload	=	TRUE;
	}

	m_sIsReloadingBeltItem	= FALSE;

	if	(isReload)
	{
		arrangeInventory();
		
		sendReloadBeltItem();
	}

	return	isReload;
}

//
//	벨트에 아이템을 리로드 한다.
BOOL
cACTOR::isAvailReloadBeltSlot()
{
	int		i;
	cItem	*lpSlot;
	BOOL	isReload	=	FALSE;

	for (i=dEQUIP_BELT_SLOT1;i<=dEQUIP_BELT_SLOT5;i++)
	{
		lpSlot	=	&m_aEquip[i];

		if (lpSlot->m_wBaseItem	==	0xffff					)	continue;
		int		iLimitCount		=	min(m_wBeltStackCount,lpSlot->getStackLimit());
		if (lpSlot->m_bCount	>=	iLimitCount				)	continue;

		cItem	*lpItem		=	getSameItem(lpSlot);

		if (!lpItem)	continue;

		isReload	=	TRUE;
	}

	return	isReload;
}

//
//	장비 하나의 능력치를 적용시킨다.
BOOL
cACTOR::applyEquipmentAbility(int _iPart,cItem *_lpItem)
{
	cBasicItem	*lpBasicItem	=	_lpItem->getBasicItem(TRUE);
	cItem		item;
	int			iEfficiency		=	100;

	item.copy(_lpItem);

	if	(_lpItem->isExtraItem())
	{
		cExtraItemDefine	*lpExItem			=	(cExtraItemDefine *)_lpItem;

		lpExItem->updateCurrentEfficency();
		iEfficiency	=	lpExItem->m_sCurrentEfficiency;

		if (lpExItem->m_sCurrentEfficiency	<	0)
		{
			g_im.generateItemByBaseItem(&item,lpBasicItem->m_wBaseItem,1,100);
			lpBasicItem	=	item.getBasicItem(TRUE);
		}
	}

	if	(!isUseAbleItem(&item))
		return	FALSE;
//	if (lpBasicItem->m_wKind	==	eIK_ARROW	|| lpBasicItem->m_wKind==	eIK_BULLET)	return	FALSE;

	if	(_iPart	==	dEQUIP_WEAPON || lpBasicItem->m_wKind == eIK_SHIELD)
	{
		m_iWeaponMinDamage		+=	lpBasicItem->m_wMinDamage*100;
		m_iWeaponMaxDamage		+=	lpBasicItem->m_wMaxDamage*100;

		if (lpBasicItem->m_attr.isUseAbleBulletSubstituteArrow)
			m_bf1IsUseAbleBulletSubstituteArrow	=	TRUE;
	}

	if	(_iPart == dEQUIP_GLOVE && (m_wJob == dJOB_FIGHTER || m_wJob == dJOB_WEREWOLF))	//	장갑-o-의 데미지는 격투가와 늑대인간 에게만 적용된다.
	{
		m_iWeaponMinDamage	+=	lpBasicItem->m_wMinDamage*100;
		m_iWeaponMaxDamage	+=	lpBasicItem->m_wMaxDamage*100;
	}

	if	(_iPart == dEQUIP_SHOES && m_wJob == dJOB_FIGHTER)	//	신발의 데미지는 격투가에게만 적용된다.
	{
		m_iWeaponMinDamage	+=	lpBasicItem->m_wMinDamage*100;
		m_iWeaponMaxDamage	+=	lpBasicItem->m_wMaxDamage*100;
	}

	lpBasicItem	=	_lpItem->getBasicItem(TRUE);

	if	(lpBasicItem->m_bf1AddEntryGuildDungeon)
		m_bf1AddEntryGuildDungeon	=	TRUE;	//	

	if	(lpBasicItem->m_bf1IsAddGetMysticStone)
		m_bf1IsAddGetMysticStone	=	TRUE;	//	
	if	(lpBasicItem->m_bf1IsAddGetTantalissRelic)
		m_bf1IsAddGetTantalissRelic	=	TRUE;	//	
	if	(lpBasicItem->m_bf1IsIgnoreGuildHallLevelForEntryTantalisExile)
		m_bf1IsIgnoreGuildHallLevelForEntryTantalisExile	=	TRUE;	//	
	if	(lpBasicItem->m_bf1IsInfinitySeriousUpgradeChanceUp)
		m_bf1IsInfinitySeriousUpgradeChanceUp	=	TRUE;	//	

//	기본 데이터
	for (int iGenData=0;iGenData<dGENERATE_ITEM_DATA_COUNT;iGenData++)
	{
		if (lpBasicItem->m_aGenerateData[iGenData].m_wEffect	==	0xffff)
			break;

		int	iReferenceIndex1	=	lpBasicItem->m_aGenerateData[iGenData].m_aValue[0];
		int	iReferenceIndex2	=	lpBasicItem->m_aGenerateData[iGenData].m_aValue[1];

		operateEquipmentBasicData(lpBasicItem->m_aGenerateData[iGenData].m_wEffect,item.m_aOption[iReferenceIndex1],item.m_aOption[iReferenceIndex2]);
	}

//	유니크
	if (iEfficiency	>	0)
	{
		for (int iUnique=0;iUnique<dITEM_UNIQUE_DATA_COUNT;iUnique++)
		{
			cUniqueData	*lpInfo	=	&lpBasicItem->m_aUniqueData[iUnique];
			cUniqueData	uniqueData;

			if (lpInfo->m_wEffect	==	0xffff)
				break;

			if (iEfficiency	!=	100)
			{
				memcpy(&uniqueData,lpInfo,sizeof(cUniqueData));

				for (int i=0;i<dITEM_UNIQUE_DATA_VALUE_COUNT;i++)
					uniqueData.m_aValue[i]	=	uniqueData.m_aValue[i]*iEfficiency/100;

				lpInfo	=	&uniqueData;
			}

			operateEquipmentUniqueData(lpInfo);
		}
	}

//	접두사
	if	(item.isExceptionItem() == FALSE)
		for (int iPrefix=0;iPrefix<dITEM_PREFIX_COUNT;iPrefix++)
		{
			if (item.m_aPrefix[iPrefix].m_wPrefix	==	0xffff)
				break;

			operateEquipmentPrefix(&_lpItem->m_aPrefix[iPrefix]);
		}

	if	(item.isPrincessWeapon())
	{
		cPrincessWeaponDefine	*lpWeapon	=	item.getPrincessWeapon();

		m_attackPower.m_sCorrectPercentageValueByPrefix	+=	lpWeapon->m_wAddDamagePercentage;

		addAddFireDamage(lpWeapon->m_bFireDamage);
		addAddWaterDamage(lpWeapon->m_bWaterDamage);
		addAddWindDamage(lpWeapon->m_bWindDamage);
		addAddLightDamage(lpWeapon->m_bLightDamage);
		addAddDarkDamage(lpWeapon->m_bDarkDamage);
		addAddEarthDamage(lpWeapon->m_bEarthDamage);

		m_attackPower.m_sCorrectPercentageValueByPrefix	+=	lpWeapon->m_wAddDamagePercentage;
	}

	return	TRUE;
}

//
//	장비 하나의 능력치를 적용시킨다.
BOOL
cACTOR::getDamageByBullet(cACTOR *_lpCaster,int *_lpiFireDamage,int *_lpiWaterDamage,int *_lpiWindDamage,int *_lpiEarthDamage,int *_lpiLightDamage,int *_lpiDarkDamage)
{
	cItem		*lpBullet		=	_lpCaster->getBullet();

	if	(!lpBullet)
		return	FALSE;

	cBasicItem	*lpBasicItem	=	lpBullet->getBasicItem(TRUE);
	cItem		item;
	int			iEfficiency		=	100;

	item.copy(lpBullet);

	if	(!_lpCaster->isUseAbleItem(&item))
		return	FALSE;

	lpBasicItem		=	lpBullet->getBasicItem(TRUE);

	*_lpiFireDamage	=	0;
	*_lpiWaterDamage=	0;
	*_lpiWindDamage	=	0;
	*_lpiEarthDamage=	0;
	*_lpiLightDamage=	0;
	*_lpiDarkDamage	=	0;

//	기본 데이터
	for (int iGenData=0;iGenData<dGENERATE_ITEM_DATA_COUNT;iGenData++)
	{
		if	(lpBasicItem->m_aGenerateData[iGenData].m_wEffect	==	0xffff)
			break;

		int	iReferenceIndex1	=	lpBasicItem->m_aGenerateData[iGenData].m_aValue[0];
		int	iReferenceIndex2	=	lpBasicItem->m_aGenerateData[iGenData].m_aValue[1];
		int	iValue1				=	item.m_aOption[iReferenceIndex1];
		int	iValue2				=	item.m_aOption[iReferenceIndex2];
		int	iEffect				=	lpBasicItem->m_aGenerateData[iGenData].m_wEffect;

		getItemMagicDamage(iEffect,iValue1,iValue2,_lpiFireDamage,_lpiWaterDamage,_lpiWindDamage,_lpiEarthDamage,_lpiLightDamage,_lpiDarkDamage);
	}

//	유니크
	for (int iUnique=0;iUnique<dITEM_UNIQUE_DATA_COUNT;iUnique++)
	{
		cUniqueData	*lpInfo	=	&lpBasicItem->m_aUniqueData[iUnique];
		cUniqueData	uniqueData;

		if (lpInfo->m_wEffect	==	0xffff)
			break;

		if (iEfficiency	!=	100)
		{
			memcpy(&uniqueData,lpInfo,sizeof(cUniqueData));

			for (int i=0;i<dITEM_UNIQUE_DATA_VALUE_COUNT;i++)
				uniqueData.m_aValue[i]	=	uniqueData.m_aValue[i]*iEfficiency/100;

			lpInfo	=	&uniqueData;
		}

		getItemPrefixMagicDamage(lpInfo->m_wEffect,lpInfo->m_aValue[0],lpInfo->m_aValue[1],_lpiFireDamage,_lpiWaterDamage,_lpiWindDamage,_lpiEarthDamage,_lpiLightDamage,_lpiDarkDamage);
	}

//	접두사
	for (int iPrefix=0;iPrefix<dITEM_PREFIX_COUNT;iPrefix++)
	{
		if (item.m_aPrefix[iPrefix].m_wPrefix	==	0xffff)
			break;

		int	iEffect	=	g_aItemPrefix[lpBullet->m_aPrefix[iPrefix].m_wPrefix].m_wType;
		int	iValue1	=	lpBullet->m_aPrefix[iPrefix].m_aValue[0];
		int	iValue2	=	lpBullet->m_aPrefix[iPrefix].m_aValue[1];

		getItemPrefixMagicDamage(iEffect,iValue1,iValue2,_lpiFireDamage,_lpiWaterDamage,_lpiWindDamage,_lpiEarthDamage,_lpiLightDamage,_lpiDarkDamage);
	}

	return	TRUE;
}


BOOL	
cACTOR::isCanNotUseItemToGVG(int _iItem)
{
	if	(g_iWorldServerType	!=	eSERVER_TYPE_GVG)
		return FALSE;

	cItem	*lpItem	=	getInventoryItem(_iItem);

	if	(!lpItem)
		return	TRUE;

	cBasicItem*	lpBasicItem	=	lpItem->getBasicItem();

	if	(!lpBasicItem)
		return	TRUE;

	if	(lpBasicItem->m_bf1IsNotUseToGVG)
		return	TRUE;

	return FALSE;
}


void
cACTOR::operateBodyEffectItem()
{

	WORD	wBodyEffect	=	0xffff;

	for (int i=dEQUIP_WEAPON;i<=dEQUIP_RING8;i++)
	{
		cItem		*lpItem		=	getEquipment(i);

		if	(!lpItem)
			continue;

		if	(!isUseAbleItem(lpItem))
			continue;
		
		cBasicItem*	lpBasicItem	=	lpItem->getBasicItem(TRUE);

		if(!lpBasicItem)
			continue;

		if(lpBasicItem->m_wItemEffect != 0)	//	0 이 디폴트이다! 
			wBodyEffect = lpBasicItem->m_wItemEffect-1;			
	}

	if(m_wItemEffect != wBodyEffect)
	{
		m_wItemEffect	=	wBodyEffect;
		addSendActorMagicEffectStatus();
	}
}



//
//	능력치 데이터를 계산한다.

void
cACTOR::operateEquipmentData()
{
	int		i;
	BOOL	aIsCanNotSupportByLowAbility[dEQUIP_WEAPON_2];

	memset(aIsCanNotSupportByLowAbility,0,sizeof(aIsCanNotSupportByLowAbility));

	//	몬스터는 rebuildBasicData면 다 된다.. --;;
	if	(!isPlayer())
		return;

	//	보석 데이터 처리
	if (m_wJewelTime)
		for (int iPrefix=0;iPrefix<dITEM_PREFIX_COUNT;iPrefix++)
		{
			if (m_jewel.m_aPrefix[iPrefix].m_wPrefix	==	0xffff)
				break;

			operateEquipmentPrefix((CItemPrefixInfo*)&m_jewel.m_aPrefix[iPrefix]);
		}

//	기본 공/방력
	{
		m_defensePower.m_iValue	=	m_sDefensivePower*100;	//	최소 방어력
		m_iWeaponMinDamage		=	m_sMinDamage*100;		//	최소 데미지
		m_iWeaponMaxDamage		=	m_sMaxDamage*100;		//	최대 데미지
		
		if	(m_wTransformationTime || m_wJob	==	dJOB_FIGHTER)
		{
			int iLevel = m_iLevel;
			if(m_wTransformationTime)
				iLevel = getLevel();
			m_iWeaponMinDamage		=	m_sMinDamage*100+g_fighterAttackInfo.getMinAttackPower(iLevel);		//	최소 데미지
			m_iWeaponMaxDamage		=	m_sMaxDamage*100+g_fighterAttackInfo.getMaxAttackPower(iLevel);		//	최대 데미지
		}
	}	//	기본 공/방력

//	장비에 데이터 처리
	for (i=dEQUIP_WEAPON;i<=dEQUIP_RING8;i++)
	{
		cItem		*lpItem		=	getEquipment(i);

		if	(!lpItem)
			continue;

		if	(!applyEquipmentAbility(i,lpItem))
			aIsCanNotSupportByLowAbility[i]	=	TRUE;
	}

	operateItemSetEffect();

	
	int aiItemSetList[dEQUIPMENT_PART_COUNT] = {0,};
	int aiItemSetCount[dEQUIPMENT_PART_COUNT] = {0,};

	int iItemSetListCount = getOperateItemSetList(aiItemSetList);

	for(i=0;i<iItemSetListCount;++i)
		aiItemSetCount[i] = getSetItemCount(aiItemSetList[i]);

	//	능력치가 딸려서 착용 못했던 아이템들 처리
	while(1)
	{
		
		
		BOOL	bIsChange	=	FALSE;
		//	장비에 데이터 처리
		for (int i=dEQUIP_WEAPON;i<=dEQUIP_RING8;i++)
		{
			if	(!aIsCanNotSupportByLowAbility[i])
				continue;
			
			cItem		*lpItem		=	getEquipment(i);
			
			if	(!lpItem)
				continue;
			
			if	(applyEquipmentAbility(i,lpItem))
			{
				aIsCanNotSupportByLowAbility[i]	=	FALSE;
				bIsChange						=	TRUE;
			}
		}
		
		if	(bIsChange	==	FALSE)
			break;
	}

	removeItemSetEffect(aiItemSetList,aiItemSetCount, iItemSetListCount);
	operateBodyEffectItem();	//	 아이템 효과 처리(몸에 붙는 이팩트)~!
	operateItemSetEffect();

	if (m_iWeaponMinDamage>	m_iWeaponMaxDamage)
	{
		int	iDamage			=	m_iWeaponMinDamage;

		m_iWeaponMinDamage	=	m_iWeaponMaxDamage;		//	최소 데미지
		m_iWeaponMaxDamage	=	iDamage;
	}

	for (i=dEQUIP_BELT_SLOT1;i<=dEQUIP_BELT_SLOT5;i++)
	{
		if (m_aEquip[i].m_wBaseItem	==	0xffff)
			continue;

		if (m_aEquip[i].m_bCount	>	m_wBeltStackCount)
		{
			int	iRemoveItemCount	=	m_aEquip[i].m_bCount-m_wBeltStackCount;
			m_aEquip[i].m_bCount	=	(BYTE)m_wBeltStackCount;

			cItem	item,itemOld;

			item.copy(&m_aEquip[i]);
			itemOld.copy(&m_aEquip[i]);
			m_aEquip[i].reset();

			item.m_bCount	=	iRemoveItemCount;

			if	(item.isMoneyItem())
				CLOG("goldIOError","operateEquipmentData");

			addItemToInventory(&item);

			m_aEquip[i].copy(&itemOld);

			if	(item.m_bCount)
			{
				if	(m_aEquip[i].isCanNotDropItem())
					m_aEquip[i].m_bCount+=	item.m_bCount;
				else
				{
					CDroppedItem	dropItem;

					dropItem.m_item.copy(&item);
					dropItem.m_pos.x			=	m_pos.x+random(40)-20;
					dropItem.m_pos.y			=	m_pos.y+random(40)-20;
					dropItem.m_wDropMethod		=	eDIM_PLAYER_DROP;
					dropItem.m_wOwner			=	m_wSerialInField;
					dropItem.m_wSealedTime		=	6000;

					m_lpField->dropItem(&dropItem);
				}
			}

			m_aEquip[i].checkItemCount();
		}
	}

	calcAddPoisonDamageOption();
//	타이틀 처리
//	operateEquipmentPrefix
}

//
//	어떤 부위의 것을 벗냐?
BOOL
cACTOR::stripEquipment(int _iPart)
{
	if (_iPart	>=	dEQUIPMENT_PART_COUNT)
		return	TRUE;

	if (_iPart == dEQUIP_WEAPON		&& m_wHideWeapon1Time)
		return	eUSR_PLEASE_PICK_WEAPON;
	if (_iPart == dEQUIP_WEAPON_2	&& m_wHideWeapon2Time)
		return	eUSR_PLEASE_PICK_WEAPON;

	if	(_iPart	==	dEQUIP_SHIELD_BULLET	&&	(m_wCastBlocker			!=	0xffff  || isActiveCompleteProtection() ))		//   컴플리트 프로텍션 사용중 방패 못벗게 추가. 09.09.10 
		return	eUSR_CAN_NOT_STRIP_SHIELD_STATUS;

	if	(_iPart	==	dEQUIP_WEAPON			&&	m_bf10CastWeaponBlocker	!=	dVALID_BLOCKER_CODE )
		return	eUSR_CAN_NOT_STRIP_WEAPON_STATUS;

	cBasicItem	*lpBasicItem	=	m_aEquip[_iPart].getBasicItem(TRUE);

	if	(!lpBasicItem)
		return	eUIR_WRONG_ITEM_DATA;

	if	(m_aEquip[_iPart].checkItemCount())
		return	eUIR_OK;

	if	(m_aEquip[_iPart].isMoneyItem())
		CLOG("goldIOError","stripEquipment");

	int	iResult	=	addItemToInventory(&m_aEquip[_iPart],TRUE);

	if	(iResult	!=	eUIR_OK)
		return	iResult;

	buildPower();

	if (lpBasicItem->isAppearanceChangeItem())
		sendAppearanceInfo();

	arrangeInventory();

	return eUIR_OK;
}

//
//	두 아이템의 위치를 바꾼다.
BOOL
cACTOR::changeItemPlace(int _iSrcItem,int _iDestItem)
{
	if	(_iSrcItem	<		0)
		return	FALSE;
	if	(_iSrcItem	>=	m_wItemSlotCount &&	_iSrcItem < dOWN_ITEM_COUNT)
		return	FALSE;
	if	(_iSrcItem	>=	dOWN_ITEM_COUNT	+	m_bf6ExtraInventorySize)
		return	FALSE;
	if	(_iDestItem	<		0)
		return	FALSE;
	if	(_iDestItem	>=	m_wItemSlotCount &&	_iDestItem < dOWN_ITEM_COUNT)
		return	FALSE;
	if	(_iDestItem	>=	dOWN_ITEM_COUNT	+	m_bf6ExtraInventorySize)
		return	FALSE;

	if	(_iSrcItem	==	_iDestItem)
		return	TRUE;

	cItem	*lpSrcItem	=	getInventoryItem(_iSrcItem);
	cItem	*lpDestItem	=	getInventoryItem(_iDestItem,TRUE);

	if	(!lpSrcItem	||	lpSrcItem->m_wBaseItem	==	0xffff)
		return	FALSE;

	if	(lpSrcItem->isSameItem(lpDestItem) && !lpDestItem->isFull())	//	만약에 같은 아이템이면 겹친다.
	{
		lpDestItem->addItem(lpSrcItem);

		arrangeInventory();

		return	TRUE;	//	겹치고 났더니 남은게 없으면 리턴
	}

	lpSrcItem->swapItem(lpDestItem);

	arrangeInventory();

	return	TRUE;
}

int
cACTOR::destroyItem(int _iItemSlot)
{
	if	((_iItemSlot	<	dOWN_ITEM_COUNT	&&	_iItemSlot	>=	m_wItemSlotCount)	||	_iItemSlot	<	0	||
		_iItemSlot	>=	dOWN_ITEM_COUNT	+	m_bf6ExtraInventorySize)
		return	eUDIR_VAILD_ITEM;

	cItem	*lpItem	=	getInventoryItem(_iItemSlot);

	if	(!lpItem || lpItem->m_wBaseItem	==	0xffff)
		return	eUDIR_VAILD_ITEM;

	if	(lpItem->isCanNotDestroyItem())
		return	eUDIR_CAN_NOT_DESTROY_ITEM;

	if	(lpItem->isPrincessWeapon())
	{
		cPrincessWeaponDefine	*lpWeapon	=	lpItem->getPrincessWeapon();
		cACTOR					*lpPrincess	=	m_lpField->getActor(lpWeapon->m_wPrincessSerial);

		if	(lpPrincess)
			lpPrincess->releasePrincessWeaponStatus(TRUE);
	}
	else
		sendRemoveItemLog(lpItem,eRI_DESTROY);

	lpItem->reset();

	buildPower();

	return	eUIR_OK;
}

//
//	체크섬을 구한다.
WORD
cACTOR::getItemCheckSum()
{
	int		i;

	arrangeInventory();

	int	iCheckSum	=	m_wItemCount+m_iGold+m_bf6ExtraInventorySize;

	for (i=0;i<dOWN_ITEM_COUNT;i++)
	{
		iCheckSum	+=	m_aItems[i].getCheckSum()%(100+i);
	}

	for (i=0;i<dEQUIPMENT_PART_COUNT;i++)
	{
		iCheckSum	+=	m_aEquip[i].getCheckSum()%(100+i);
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		iCheckSum	+=	m_aExtraInventory[i].getCheckSum()%(100+i);
	}

	return	(WORD)iCheckSum;
}


//
//	아이템 떨구기.. 어디다가?
void
cACTOR::throwWeapon(int _iX,int _iY,cAbility *_lpAbility,BOOL _bIsThrowOtherJobWeapon)
{
	cItem	*lpWeapon;
	
	if	(_bIsThrowOtherJobWeapon)
		lpWeapon	=	getOtherJobWeapon();
	else
		lpWeapon	=	getWeapon();

	CDroppedItem	droppedItem;

	if	(!lpWeapon)
		return;

	droppedItem.reset();
	droppedItem.m_item.copy(lpWeapon);
	droppedItem.m_wSealedTime	=	dDROP_ITEM_SEAL_TIME;	//	5분
	droppedItem.m_wOwner		=	m_wSerialInField;
	droppedItem.m_pos.x			=	_iX;
	droppedItem.m_pos.y			=	_iY;
	droppedItem.m_wDropMethod	=	eDIM_DUMMY;

	m_lpField->sendHideWeapon(m_wSerialInField,_lpAbility,_bIsThrowOtherJobWeapon);
	m_lpField->dropItem(&droppedItem);

	arrangeInventory();
}


//
//	소비성 아이템이냐?
BOOL
cACTOR::isExhaustItem(int _iItemSlot)
{
	cItem	*lpItem				=	&m_aItems[_iItemSlot];

	if	(!lpItem		)
		return	FALSE;

	cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

	if	(!lpBasicItem)
		return	FALSE;

	return 1-lpBasicItem->m_attr.isCanNotUseableItem;
}

//
//	빈슬롯 찾아서 리턴~
cItem*
cACTOR::getValidInventorySlot(BOOL _bIsFullSlot)
{
	int	iCount	=	getItemSlotCount(),i;

	if (_bIsFullSlot)
		iCount	=	dOWN_ITEM_COUNT;

	for (i=0;i<iCount;i++)
		if (m_aItems[i].m_wBaseItem	==	0xffff)
			return	&m_aItems[i];

	for (i=0;i<m_bf6ExtraInventorySize;i++)
		if (m_aExtraInventory[i].m_wBaseItem	==	0xffff)
			return	&m_aExtraInventory[i];

	return	NULL;
}

//
//	빈슬롯 찾아서 리턴~
int
cACTOR::getValidInventorySlotIndex(BOOL _bIsFullSlot)
{
	int	iCount	=	getItemSlotCount(),i;

	if (_bIsFullSlot)
		iCount	=	dOWN_ITEM_COUNT;

	for (i=0;i<iCount;i++)
		if (m_aItems[i].m_wBaseItem	==	0xffff)
			return	i;

	for (i=0;i<m_bf6ExtraInventorySize;i++)
		if (m_aExtraInventory[i].m_wBaseItem	==	0xffff)
			return	i+dOWN_ITEM_COUNT;

	return	0xffff;
}

//
//	아이템 정렬
void
cACTOR::arrangeInventory()
{
	int	i;

	for (i=0;i<getItemSlotCount();i++)
	{
		cItem	*lpItem	=	&m_aItems[i];

		if	(lpItem->m_wBaseItem	==	0xffff				)	
			continue;
		if	(lpItem->m_wBaseItem	>=	dBASIC_ITEM_COUNT	)
		{
			lpItem->reset();
			continue;
		}

		if	(lpItem->isExceptionItem())
			continue;
		
		if	(lpItem->isInfinityBullet())
			lpItem->m_bCount	=	1;

		if	(!lpItem->isExtraItem() && !lpItem->isMagicCarpetEmblem())
		{
			lpItem->checkItemCount();

			if (lpItem && lpItem->m_wBaseItem	!=	0xffff)
				lpItem->m_bCount	=	min(lpItem->m_bCount,lpItem->getStackLimit());
		}
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		cItem	*lpItem	=	&m_aExtraInventory[i];

		if	(lpItem->m_wBaseItem	==	0xffff				)	
			continue;
		if	(lpItem->m_wBaseItem	>=	dBASIC_ITEM_COUNT	)
		{
			lpItem->reset();
			continue;
		}
		if	(lpItem->isExceptionItem())
			continue;

		if	(lpItem->isInfinityBullet())
			lpItem->m_bCount	=	1;

		if (!lpItem->isExtraItem() && !lpItem->isMagicCarpetEmblem())
		{
			lpItem->checkItemCount();

			if (lpItem && lpItem->m_wBaseItem	!=	0xffff)
				lpItem->m_bCount	=	min(lpItem->m_bCount,lpItem->getStackLimit());
		}
	}

	for (i=0;i<dEQUIPMENT_PART_COUNT;i++)
	{
		cItem	*lpItem	=	&m_aEquip[i];

		if	(lpItem->m_wBaseItem	==	0xffff				)	
			continue;
		if	(lpItem->m_wBaseItem	>=	dBASIC_ITEM_COUNT	)	
		{
			lpItem->reset();
			continue;
		}

		if	(lpItem->isExceptionItem())
			continue;

		cSpecialItem	*lpSpItem	=	lpItem->getSpecialItem();

		if	(lpSpItem && lpSpItem->m_bf1IsReversionItemWhenEquip)
			lpSpItem->m_bf1IsReversionItem			=	TRUE;

		if	(lpItem->isInfinityBullet())
			lpItem->m_bCount	=	1;

		if (!lpItem->isExtraItem() && !lpItem->isMagicCarpetEmblem())
			lpItem->m_bCount	=	min(lpItem->m_bCount,lpItem->getStackLimit());
	}

	computeItemCount();
}

//
//	똑같은 시리얼을 가진 아이템이 있나 검사
int
cACTOR::findSameSerialItem(DWORD _dwSerial,int _iSlot)
{
	int		i;
	WORD	wBaseItem;
	cItem	*lpItem;

	if (_iSlot > 100)
	{
		wBaseItem		=	m_aEquip[_iSlot-100].m_wBaseItem;
		lpItem			=	&m_aEquip[_iSlot-100];
	}
	else
	if (_iSlot >= dOWN_ITEM_COUNT)
	{
		wBaseItem		=	m_aExtraInventory[_iSlot-dOWN_ITEM_COUNT].m_wBaseItem;
		lpItem			=	&m_aExtraInventory[_iSlot-dOWN_ITEM_COUNT];
	}
	else
	{
		wBaseItem		=	m_aItems[_iSlot].m_wBaseItem;
		lpItem			=	&m_aItems[_iSlot];
	}

	if (wBaseItem	==	0xffff	)
		return	0xffff;

	if	(lpItem->isExceptionItem())
		return	0xffff;

	for (i=0;i<dEQUIPMENT_PART_COUNT;i++)
	{
		if (m_aEquip[i].m_wBaseItem		!=	wBaseItem)	continue;
		if (i+100						== _iSlot	)	continue;
		if (m_aEquip[i].getStackLimit() >	1		)	continue;
		if (m_aEquip[i].m_dwSerial		<=	1		)	continue;

		if (m_aEquip[i].m_dwSerial		==	_dwSerial)	return	i+100;
	}

	for (i=0;i<dOWN_ITEM_COUNT;i++)
	{
		if (m_aItems[i].m_wBaseItem		!=	wBaseItem)	continue;
		if (i							== _iSlot	)	continue;
		if (m_aItems[i].getStackLimit() >	1		)	continue;
		if (m_aItems[i].m_dwSerial		<=	1		)	continue;

		if (m_aItems[i].m_dwSerial		==	_dwSerial)	return	i;
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		cItem	*lpItem	=	&m_aExtraInventory[i];

		if (lpItem->m_wBaseItem		!=	wBaseItem)	continue;
		if (i+dOWN_ITEM_COUNT		== _iSlot	)	continue;
		if (lpItem->getStackLimit() >	1		)	continue;
		if (lpItem->m_dwSerial		<=	1		)	continue;

		if (lpItem->m_dwSerial		==	_dwSerial)	return	i;
	}

	return	0xffff;
}

//
//	아이템 무결성 검사
void
cACTOR::checkIncorrectItem(int _iWhere)
{
	int		i,iSameItemCount=0,iReviseIncorrectPrefixCount=0;
	int		iDestroyByMoveFieldItemCount	=	0;
	int		iExpireTreasureMapCount			=	0;
	int		iPrincessWeapon					=	0;
	SG_EXPIRED_ITEM_LIST	packet;

	packet.base.set(sizeof(SG_EXPIRED_ITEM_LIST),dSG_EXPIRED_ITEM_LIST);
	packet.wItemCount	=	0;

	if	(m_lpField->m_wIsOXQuizField		==	FALSE)
		removeOXQuizTicket();

	if	(m_lpField->m_bIsGuildBattleField==	FALSE)
		removeGuildBattleItem();

	for (i=dEQUIPMENT_PART_COUNT+dBORDER_OF_ITEM_AND_EQUIPMENT;i;i)
	{
		--i;

		cItem	*lpItem				=	getInventoryItem(i);

		if	(!lpItem)
			continue;

		if	(lpItem->getBasicItem()	==	NULL)
			continue;

		if	(_iWhere	==	eDIL_LOGIN)
		{
			if	(lpItem->getBasicItem()->m_bf1IsDestroyWhenMoveField)
			{
				if	(lpItem->isTreasureMap())
					iExpireTreasureMapCount++;
				else	
					iDestroyByMoveFieldItemCount++;
			
				if	(lpItem->isPrincessWeapon()	==	FALSE)
					sendRemoveItemLog(lpItem,eRI_BY_MOVE_FIELD);
				else
					++iPrincessWeapon;
				lpItem->reset();

				continue;
			}
		}
		
		int	iResult		=	lpItem->checkIncorrectItem(this,i,&iSameItemCount,&iReviseIncorrectPrefixCount);

		switch(iResult)
		{
			case	cItem::eII_CORRECT:
				if	(i	<	dOWN_ITEM_COUNT	&&	i	>=	getItemSlotCount())
				{
					cItem	*lpValidSlot	=	getValidInventorySlot();

					if	(lpValidSlot)
					{
						lpValidSlot->copy(lpItem);
						lpItem->reset();
					}
				}
				break;

			case	cItem::eII_EXPIRED:
			{
				sendExpiredItemLog(lpItem);
				packet.awItemList[packet.wItemCount][0]	=	i;
				packet.awItemList[packet.wItemCount][1]	=	lpItem->m_wBaseItem;
				packet.wItemCount++;

				lpItem->reset();
				break;
			}
			case	cItem::eII_IS_MONEY:
				lpItem->reset();
				break;
			case	cItem::eII_INVALID_QUEST:
				lpItem->reset();
				break;
			case	cItem::eII_DOUBLE_ITEM:
				sendDoubleItemLog(lpItem,_iWhere);
				lpItem->reset();
				break;
		}
	}

	if	(packet.wItemCount)
	{
		packet.base.wSize	=	sizeof(packet)-sizeof(packet.awItemList)+sizeof(WORD)*2*packet.wItemCount;

		g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
	}

	if	(iSameItemCount)
		sendRegistServerMessage(eRSM_FIND_COPIED_ITEM);

	if	(iReviseIncorrectPrefixCount)
		sendRegistServerMessage(eRSM_REVISE_INCORRECT_PREFIX);

	iDestroyByMoveFieldItemCount	-=	iPrincessWeapon;

	if	(iDestroyByMoveFieldItemCount)
		sendEtcWork(eEW_DESTROY_ITEM_BY_MOVE_FIELD);

	if	(iExpireTreasureMapCount)
		sendEtcWork(eEW_DESTROY_TREASURE_MAP_BY_MOVE_FIELD);

	buildPower();
}

//
//	비어 있는 인벤토리 슬롯의 수
int
cACTOR::getRemainInventorySlotCount()
{
	int	iCount	=	0,i;

	for (i=0;i<getItemSlotCount();i++)
		if (m_aItems[i].m_wBaseItem			==	0xffff	)
			iCount++;

	for (i=0;i<m_bf6ExtraInventorySize;i++)
		if (m_aExtraInventory[i].m_wBaseItem==	0xffff	)
			iCount++;

	return	iCount;
}

int
cACTOR::getItemSlotIndex(cItem *_lpItem)
{
	if	(_lpItem	==	NULL)
		return	0xffff;

	int	iSlot	=	_lpItem	-	m_aItems;

	if	(iSlot	<	dOWN_ITEM_COUNT	&&	iSlot	>=	0)
		return	iSlot;

	iSlot	=	_lpItem	-	m_aExtraInventory;

	if	(iSlot	<	m_bf6ExtraInventorySize	&&	iSlot	>=	0)
		return	iSlot+dOWN_ITEM_COUNT;

	iSlot	=	_lpItem	-	m_aEquip;

	if	(iSlot	>=	dEQUIP_BELT_SLOT1	&&	iSlot	<=	dEQUIP_BELT_SLOT4)
		return	iSlot+dBORDER_OF_ITEM_AND_EQUIPMENT;

	return	0xffff;
}

int
cACTOR::getBlockLightPocketItemIndex()
{
	int	i;

	for (i=0;i<getItemSlotCount();i++)
		if	(m_aItems[i].isBlockLightPocket())
			return	i;

	for (i=0;i<m_bf6ExtraInventorySize;i++)
		if (m_aExtraInventory[i].isBlockLightPocket())
			return	i+dOWN_ITEM_COUNT;

	return	0xffff;
}

//
//	인벤토리에 아이템 얻어오기
cItem*
cACTOR::getInventoryItem(int _iSlot,BOOL _bIsWantSlot)
{
	if	(_iSlot	<	0)
		return	NULL;

	if	(_iSlot	<	dBORDER_OF_ITEM_AND_EQUIPMENT)
	{
		if	(_iSlot	<	dOWN_ITEM_COUNT	&&	_iSlot	>=	getItemSlotCount())
			return	NULL;
		if	(_iSlot	>=	dOWN_ITEM_COUNT+m_bf6ExtraInventorySize)
			return	NULL;
	}

	if	(_iSlot	>=	dBORDER_OF_ITEM_AND_EQUIPMENT+dEQUIPMENT_PART_COUNT)
		return	NULL;	

	cItem	*lpItem;

	if	(_iSlot	>=	dBORDER_OF_ITEM_AND_EQUIPMENT)
		lpItem	=	&m_aEquip[_iSlot-dBORDER_OF_ITEM_AND_EQUIPMENT];
	else
	if	(_iSlot	>=	dOWN_ITEM_COUNT)
		lpItem	=	&m_aExtraInventory[_iSlot-dOWN_ITEM_COUNT];
	else
		lpItem	=	&m_aItems[_iSlot];

	if	(_bIsWantSlot	==	FALSE)
	{
		if	(lpItem->m_wBaseItem	==	0xffff	)
			return	NULL;

		if	(!lpItem->isExceptionItem() && !lpItem->isExtraItem() )
			if	(lpItem->m_bCount	<=	0		)
				return	NULL;
	}

	return	lpItem;
}

cItem*
cACTOR::getInventoryItemInData(int _iSlot)
{
	if	(_iSlot	<	0)
		return	NULL;

	if	(_iSlot	<	dBORDER_OF_ITEM_AND_EQUIPMENT)
		if	(_iSlot	>=	dOWN_ITEM_COUNT+m_bf6ExtraInventorySize)
			return	NULL;

	if	(_iSlot	>=	dBORDER_OF_ITEM_AND_EQUIPMENT+dEQUIPMENT_PART_COUNT)
		return	NULL;	

	cItem	*lpItem;

	if	(_iSlot	>=	dBORDER_OF_ITEM_AND_EQUIPMENT)
		lpItem	=	&m_aEquip[_iSlot-dBORDER_OF_ITEM_AND_EQUIPMENT];
	else
	if	(_iSlot	>=	dOWN_ITEM_COUNT)
		lpItem	=	&m_aExtraInventory[_iSlot-dOWN_ITEM_COUNT];
	else
		lpItem	=	&m_aItems[_iSlot];

	if	(lpItem->m_wBaseItem	==	0xffff	)
		return	NULL;

	if	(!lpItem->isExceptionItem() && !lpItem->isExtraItem() )
		if	(lpItem->m_bCount	<=	0		)
			return	NULL;

	return	lpItem;
}

//
//	비어 있는 아이템 시리얼 체크
void
cACTOR::checkValidSerialItem()
{
	int	i;

	for (i=0;i<dEQUIPMENT_PART_COUNT;i++)
	{
		cItem	*lpItem	=	&m_aEquip[i];

		if (lpItem->m_wBaseItem	==	0xffff)
			continue;
		if (lpItem->isExceptionItem())
			continue;
		if (lpItem->m_dwSerial	<=	1	)
			g_game.sendWPAskItemUniqueSerial(this);
	}

	for (i=0;i<dOWN_ITEM_COUNT;i++)
	{
		cItem	*lpItem	=	&m_aItems[i];

		if (lpItem->m_wBaseItem	==	0xffff)
			continue;
		if (lpItem->isExceptionItem())
			continue;
		if (lpItem->m_dwSerial	<=	1	)
			g_game.sendWPAskItemUniqueSerial(this);
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		cItem	*lpItem	=	&m_aExtraInventory[i];

		if	(lpItem->m_wBaseItem	==	0xffff)
			continue;
		if	(lpItem->isExceptionItem())
			continue;
		if	(lpItem->m_dwSerial	<=	1	)
			g_game.sendWPAskItemUniqueSerial(this);
	}
}

//
//	아이템 유니크 시리얼 설정
void
cACTOR::setItemUniqueSerial(DWORD _dwSerial)
{
	int	i;

	for (i=0;i<dEQUIPMENT_PART_COUNT;i++)
	{
		cItem	*lpItem	=	&m_aEquip[i];

		if	(lpItem->m_wBaseItem	==	0xffff)
			continue;
		if	(lpItem->isExceptionItem())
			continue;
		if	(lpItem->m_dwSerial	>	1	)
			continue;

		lpItem->m_dwSerial		=	_dwSerial;

		sendItemUniqueSerial(i+dBORDER_OF_ITEM_AND_EQUIPMENT,_dwSerial);	//	쿨라이언트에 전송

		return;
	}

	for (i=0;i<dOWN_ITEM_COUNT;i++)
	{
		cItem	*lpItem	=	&m_aItems[i];

		if	(lpItem->m_wBaseItem	==	0xffff)
			continue;
		if	(lpItem->isExceptionItem())
			continue;
		if	(lpItem->m_dwSerial	>	1	)
			continue;

		lpItem->m_dwSerial		=	_dwSerial;

		sendItemUniqueSerial(i,_dwSerial);	//	쿨라이언트에 전송

		return;
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		cItem	*lpItem	=	&m_aExtraInventory[i];

		if	(lpItem->m_wBaseItem	==	0xffff)
			continue;
		if	(lpItem->isExceptionItem())
			continue;
		if	(lpItem->m_dwSerial	>	1	)
			continue;

		lpItem->m_dwSerial		=	_dwSerial;

		sendItemUniqueSerial(i+dOWN_ITEM_COUNT,_dwSerial);	//	쿨라이언트에 전송

		return;
	}
}

//
//	_iItem의 보유 수
int
cACTOR::getItemCount(int _iItem,BOOL _bIsIncludeEquippedItem)
{
	int	i,iItemCount=0;

	if (_iItem	==	0xffff)
	{
		for (i=0;i<dEQUIPMENT_PART_COUNT;i++)
			if	(m_aEquip[i].m_wBaseItem	!=	0xffff)	
				iItemCount	+=	m_aEquip[i].m_bCount;

		for (i=0;i<getItemSlotCount();i++)
			if	(m_aItems[i].m_wBaseItem	!=	0xffff)	
				iItemCount	+=	m_aItems[i].m_bCount;

		for (i=0;i<m_bf6ExtraInventorySize;i++)
			if	(m_aExtraInventory[i].m_wBaseItem	!=	0xffff)	
				iItemCount	+=	m_aExtraInventory[i].m_bCount;

		return	iItemCount;
	}

	if	(_bIsIncludeEquippedItem)
	{
		for (i=0;i<dEQUIPMENT_PART_COUNT;i++)
			if	(m_aEquip[i].m_wBaseItem	==	_iItem)	
				iItemCount	+=	m_aEquip[i].m_bCount;
	}
	else
	{
		for (i=dEQUIP_BELT_SLOT1;i<=dEQUIP_BELT_SLOT4;i++)
			if	(m_aEquip[i].m_wBaseItem	==	_iItem)	
				iItemCount	+=	m_aEquip[i].m_bCount;
	}

	for (i=0;i<dOWN_ITEM_COUNT;i++)
		if	(m_aItems[i].m_wBaseItem	==	_iItem)	
			iItemCount	+=	m_aItems[i].m_bCount;

	for (i=0;i<m_bf6ExtraInventorySize;i++)
		if	(m_aExtraInventory[i].m_wBaseItem	==	_iItem)	
			iItemCount	+=	m_aExtraInventory[i].m_bCount;

	return	iItemCount;
}

BOOL
cACTOR::removePrincessWeapon(int _iSerial,DWORD _dwNameHashCode)
{
	if	(_iSerial	==	0xffff)
	{
		cACTOR	*lpPrincess	=	m_lpField->getActor(m_wPrincessWeaponPrincess);

		if	(lpPrincess	&&	lpPrincess->m_dwNameHashCode	==	m_dwPrincessWeaponPrincessNameHashCode)
		{
			lpPrincess->releasePrincessWeaponStatus();

			return	TRUE;
		}
	}

	for (int	i=dEQUIPMENT_PART_COUNT+dBORDER_OF_ITEM_AND_EQUIPMENT;i;)
	{
		--i;

		cItem	*lpItem				=	getInventoryItem(i);

		if	(!lpItem	||	!lpItem->isPrincessWeapon())
			continue;
		printf("[%s]\n",lpItem->getName());
		cPrincessWeaponDefine	*lpWeapon	=	(cPrincessWeaponDefine	*)lpItem;

		if	(_iSerial	!=	0xffff)
			if	(lpWeapon->m_wPrincessSerial	!=	_iSerial	||	lpWeapon->m_dwPrincessNameHashCode	!=	_dwNameHashCode)
				continue;

		
		if	(i	>=	dBORDER_OF_ITEM_AND_EQUIPMENT)
		{	// 만약 무기슬롯에 착용중이고.. 무기가 hide 상태라면... 드랍아이템을 없애준다..+_+; hide시간도없애고....
			int iWeaponSlot = i - dBORDER_OF_ITEM_AND_EQUIPMENT;
			if(iWeaponSlot == dEQUIP_WEAPON && m_wHideWeapon1Time)
			{
				cItem	*lpWeapon	=	&m_aEquip[dEQUIP_WEAPON];
				if(lpWeapon)
				{
					m_wHideWeapon1Time = 0;
					m_lpField->m_droppedItem.removeHideWeapon(this,m_lpField,lpWeapon->m_wBaseItem,lpWeapon->m_dwSerial,dEQUIP_WEAPON);
				}
			}
			else if(iWeaponSlot == dEQUIP_WEAPON_2 && m_wHideWeapon2Time)
			{
				
				cItem	*lpWeapon	=	&m_aEquip[dEQUIP_WEAPON_2];
				if(lpWeapon)
				{
					m_wHideWeapon2Time = 0;
					m_lpField->m_droppedItem.removeHideWeapon(this,m_lpField,lpWeapon->m_wBaseItem,lpWeapon->m_dwSerial,dEQUIP_WEAPON_2);
				}
			}
		}
		m_wPrincessWeaponPrincess	=	0xffff;

		lpItem->reset();

		sendUpdateItemData(i);

		buildPower();
		
		sendAppearanceInfo();

		return	TRUE;
	}

	return	FALSE;
}


//
//
int
cACTOR::removeQuestItem(int _iQuestKind,int _iQuestIndex)
{
	int		i;
	BOOL	bIsRemovedEquipment	=	FALSE;
	int		iRemoveItemCount	=	0;

	for (i=0;i<dEQUIPMENT_PART_COUNT;i++)
	{
		cItem		*lpItem			=	&m_aEquip[i];
		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(!lpBasicItem								)
			continue;
		if	(lpBasicItem->m_questKind == _iQuestKind && lpBasicItem->m_questIndex == _iQuestIndex)
		{
			lpItem->reset();
			bIsRemovedEquipment	=	TRUE;
			iRemoveItemCount++;
		}
	}

	for (i=0;i<dOWN_ITEM_COUNT;i++)
	{
		cItem		*lpItem			=	&m_aItems[i];
		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(!lpBasicItem								)
			continue;
		if	(lpBasicItem->m_questKind == _iQuestKind && lpBasicItem->m_questIndex == _iQuestIndex)
		{
			lpItem->reset();
			iRemoveItemCount++;
		}
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		cItem		*lpItem			=	&m_aExtraInventory[i];
		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(!lpBasicItem	)
			continue;

		if	(lpBasicItem->m_questKind == _iQuestKind && lpBasicItem->m_questIndex == _iQuestIndex)
		{
			lpItem->reset();
			iRemoveItemCount++;
		}
	}

	if	(bIsRemovedEquipment)
		buildPower();

	return	iRemoveItemCount;
}

void
cACTOR::operateSpecialItemEffectInInventory(int _iIndex,cItem *_lpItem,int *_lpiBadgeCount,BOOL *_lpbHaveAddExpPortalItem,BOOL *_lpbHaveExceptExpPortal,BOOL *_lpaIsCanNotSupportByLowAbility)
{
	cBasicItem	*lpBasicItem	=	_lpItem->getBasicItem();

	if	(!lpBasicItem	)	
		return;

	if	(lpBasicItem->m_attr.isEffectInInventoryItem	==	FALSE)	
		return;

	int	iFindGuildStatueKind,iFindGuildStatueLevel;

	if	(_lpItem->getGuildStatueExporterInfo(&iFindGuildStatueKind,&iFindGuildStatueLevel))
	{
		m_wFindGuildStatueKind	=	iFindGuildStatueKind;
		m_wFindGuildStatueLevel	=	iFindGuildStatueLevel;

		return;
	}

	if	(lpBasicItem->m_attr.isBadge)
	{
		if	(*_lpiBadgeCount	>=	dLIMIT_BADGE_ITEM_COUNT_IN_INVENTORY)
			return;

		*_lpiBadgeCount++;
	}

	if	(lpBasicItem->m_wPremiumLevel	==	ePSL_EXCEPT_EXP_PORTAL)	//	얘는 예외다. 여기다 넣으면 안되는데 여기다 넣었으니 그냥 써야지. 아놔. 내잘못 아님. 코드 잘못 해석한 병찬씨 잘못이여!!
		*_lpbHaveExceptExpPortal	=	TRUE;
	else
	if	(lpBasicItem->m_wPremiumLevel	==	ePSL_ADD_PORTAL)	//	얘는 예외다. 여기다 넣으면 안되는데 여기다 넣었으니 그냥 써야지. 아놔. 내잘못 아님. 코드 잘못 해석한 병찬씨 잘못이여!!
		*_lpbHaveAddExpPortalItem	=	TRUE;
	else
	if	(lpBasicItem->m_wPremiumItemLevelForWithdraw	&&	m_wPremiumServiceGrade	<=	lpBasicItem->m_wPremiumItemLevelForWithdraw)
	{
 		m_wPremiumServiceLevel	=	(WORD)lpBasicItem->m_wPremiumLevel;
		m_wPremiumServiceGrade	=	(WORD)lpBasicItem->m_wPremiumItemLevelForWithdraw;
	}

	if	(lpBasicItem->m_bf1IsCanSummonMagicCarpet)
	{
		m_bf1IsHaveSummonMagicCarpetItem = TRUE;
	}

	if	(_lpItem->isMagicCarpetEmblem())
	{
		m_wMagicCarpetShape	=	_lpItem->m_aOption[0];

		cMagicCarpetEmblemDefine	*lpEmblem	=	(cMagicCarpetEmblemDefine	*)_lpItem;

		if	(lpEmblem->m_bf1IsSquirrelCarpet)
			m_bf1IsExistSquirrelCarpet	=	TRUE;
		if	(lpEmblem->m_bf1IsTurtleCarpet)
			m_bf1IsExistTurttleCarpet	=	TRUE;
		if	(lpEmblem->m_bf1IsDrakeCarpet)
			m_bf1IsExistDrakeCarpet		=	TRUE;
		if	(lpEmblem->m_bf1IsRedDrakeCarpet)
			m_bf1IsExistDrakeCarpet		=	TRUE;

		return;
	}

	if	(!applyEquipmentAbility(0xffff,_lpItem))
		_lpaIsCanNotSupportByLowAbility[_iIndex]	=	TRUE;
}

void
cACTOR::operateSpecialItemEffectInInventory()
{
	BOOL	aIsCanNotSupportByLowAbility[dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE];
	BOOL	bHaveExceptExpPortal	=	FALSE;
	BOOL	bHaveAddExpPortalItem	=	FALSE;

	memset(aIsCanNotSupportByLowAbility,0,sizeof(aIsCanNotSupportByLowAbility));

	m_wLimitEquipmentLevelOverPoint		=	0;
	m_wLimitEquipmentLevelOverPointByCH5=	0;

	m_wPremiumServiceLevel				=	0;
	m_wPremiumServiceGrade				=	0;
	m_wMagicCarpetShape					=	0;

	m_bf1IsOwnedPitchmanShopSignBoard1	=	FALSE;
	m_bf1IsOwnedPitchmanShopSignBoard2	=	FALSE;
	m_bf1IsOwnedPitchmanShopSignBoard3	=	FALSE;
	m_bf1IsOwnedPitchmanShopSignBoard4	=	FALSE;
	m_bf1IsOwnedPitchmanShopSignBoard5	=	FALSE;
	m_bf1IsOwnedPitchmanShopLoudSpeaker	=	FALSE;
	m_bf1IsOwnedPitchmanColorPen		=	FALSE;
	m_bf1IsOwnedPitchmanShopBoldPen		=	FALSE;
	m_bf1IsOwnedPitchmanShopLongPen		=	FALSE;
	m_bf1IsOwnedPitchmanShopTwinkle		=	FALSE;

	m_bf1IsExistSquirrelCarpet			=	FALSE;
	m_bf1IsExistTurttleCarpet			=	FALSE;
	m_bf1IsExistDrakeCarpet				=	FALSE;
	m_bf1IsHaveSummonMagicCarpetItem	=	FALSE;

	int		iBadgeCount					=	0,i;

	for (i=0;i<getItemSlotCount();i++)
		operateSpecialItemEffectInInventory(i,&m_aItems[i],&iBadgeCount,&bHaveAddExpPortalItem,&bHaveExceptExpPortal,aIsCanNotSupportByLowAbility);

	for (i=0;i<m_bf6ExtraInventorySize;i++)
		operateSpecialItemEffectInInventory(i+dOWN_ITEM_COUNT,&m_aExtraInventory[i],&iBadgeCount,&bHaveAddExpPortalItem,&bHaveExceptExpPortal,aIsCanNotSupportByLowAbility);

	while(1)
	{
		BOOL	bIsChange	=	FALSE;

		iBadgeCount	=	0;

		for (i=0;i<getItemSlotCount();i++)
		{
			cItem	*lpItem	=	&m_aItems[i];

			if	(lpItem->m_wBaseItem	==	0xffff)
				continue;

			cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

			if	(!lpBasicItem	)	
				continue;
			if	(lpBasicItem->m_attr.isEffectInInventoryItem	==	FALSE)	
				continue;

			if	(lpBasicItem->m_attr.isBadge)
			{
				if (iBadgeCount	>=	dLIMIT_BADGE_ITEM_COUNT_IN_INVENTORY)
					continue;

				iBadgeCount++;
			}

			if (aIsCanNotSupportByLowAbility[i])
				if	(applyEquipmentAbility(0xffff,lpItem))
				{
					aIsCanNotSupportByLowAbility[i]	=	FALSE;
					bIsChange	=	TRUE;
				}
		}

		for (i=0;i<m_bf6ExtraInventorySize;i++)
		{
			cItem	*lpItem	=	&m_aExtraInventory[i];

			if	(lpItem->m_wBaseItem	==	0xffff)
				continue;

			cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

			if	(!lpBasicItem	)	
				continue;
			if	(lpBasicItem->m_attr.isEffectInInventoryItem	==	FALSE)	
				continue;

			if (lpBasicItem->m_attr.isBadge)
			{
				if (iBadgeCount	>=	dLIMIT_BADGE_ITEM_COUNT_IN_INVENTORY)
					continue;

				iBadgeCount++;
			}

			if	(aIsCanNotSupportByLowAbility[i+dOWN_ITEM_COUNT])
				if	(applyEquipmentAbility(0xffff,lpItem))
				{
					aIsCanNotSupportByLowAbility[i+dOWN_ITEM_COUNT]	=	FALSE;
					bIsChange	=	TRUE;
				}
		}

		if (bIsChange	==	FALSE)
			break;
	}

	CClient	*lpClient	=	getClient();
	
	if	(lpClient)
		if	(lpClient->GetIsNetCafeUser())				//JBC	넷카페 유저 특전 40랩 배지.		08-08-12
			m_wLimitEquipmentLevelOverPoint	+=	40;

	m_wCorrectExp					=	100;
	m_wCorrectItemDropRate			=	100;
	m_wLimitEquipmentLevelOverPoint	+=	m_wLimitEquipmentLevelOverPointByCH5;
	m_bf1IsExistExceptExpPortal		=	bHaveExceptExpPortal;

	if	(m_bf5MagicCarpetShape)
	{
		callMagicCarpet(m_bf5MagicCarpetShape);
	}

	if	(m_wPremiumServiceLevel == ePSL_LITE)
	{
		m_wCorrectExp			=	120;
		m_wCorrectItemDropRate	=	115;
	}
	if (m_wPremiumServiceLevel == ePSL_PREMIUM)
	{
		m_wCorrectExp			=	150;
		m_wCorrectItemDropRate	=	140;
	}
	if	(m_wPremiumServiceLevel == ePSL_PLATINUM)
	{
		m_wCorrectExp			=	200;
		m_wCorrectItemDropRate	=	180;
	}
	if	(m_wPremiumServiceLevel == ePSL_LITE_LITE)
	{
		m_wCorrectExp			=	120;
	}
	if	(m_wPremiumServiceLevel == ePSL_LITE_PREMIUM)
	{
		m_wCorrectExp			=	150;
		m_wCorrectItemDropRate	=	130;
	}
	if	(m_wPremiumServiceLevel == ePSL_LITE_PLATINUM)
	{
		m_wCorrectExp			=	200;
		m_wCorrectItemDropRate	=	165;
	}

	if	(m_bf1IsExistExceptExpPortal)	//	EXP 보너스를 제외한 모든 기능이 되는 포탈 스피어
		m_wCorrectItemDropRate	=	max(180,m_wCorrectItemDropRate);

	if	(bHaveAddExpPortalItem)
	{
		int	iAddExp				=	m_wCorrectExp-100;
		m_wCorrectExp			=	100+iAddExp*2;
	}
}
BOOL
cACTOR::copyPrefix(int _iUseItem,int _iDestItem,int _iPrefixIndex)
{
	cItem	*lpUseItem	=	getInventoryItem(_iUseItem);
	cItem	*lpDestItem	=	getInventoryItem(_iDestItem);

	if	(!lpUseItem	||	!lpDestItem	||	lpDestItem->getPrefixCount() == dITEM_PREFIX_COUNT)
		return	FALSE;

	cBasicItem*	lpBasicUseItem = lpUseItem->getBasicItem();

	if(!lpBasicUseItem)
		return FALSE;

	BOOL	bIsCanNotTrade = FALSE;

	switch(lpBasicUseItem->m_aGenerateData[0].m_wEffect)
	{	
	case eIE_COPY_PREFIX:
		bIsCanNotTrade = FALSE;
		break;
		
	case eIE_COPY_PREFIX_CAN_NOT_TRADE:
		bIsCanNotTrade = TRUE;
		break;
		
	default:
		return FALSE;	
	}

	if	(lpDestItem->isExceptionItem())
		return	FALSE;

	if(lpDestItem->m_aPrefix[_iPrefixIndex].m_wPrefix == 0xffff)
		return FALSE;

	cBasicItem* lpUseBasic = lpUseItem->getBasicItem();
	
	if(!lpUseBasic)
		return FALSE;

	int	iValue1				=	lpUseItem->m_aOption[0];
	int iResult				=	ePCOPR_STRANGE_ITEM;

	if(random(100) < iValue1)
	{	// 성공..!
		if(bIsCanNotTrade)
		{
			cSpecialItem	*lpSpItem		=	(cSpecialItem	*)lpDestItem;
			
			if	(lpDestItem->isSpecialItem()==	FALSE)
				lpSpItem->reset();
			
			lpSpItem->m_bf1IsReversionItem	=	TRUE;
		}
		for(int i=0;i<dITEM_PREFIX_COUNT;++i)
		{
			if(lpDestItem->m_aPrefix[i].m_wPrefix== 0xffff)
			{
				memcpy(&lpDestItem->m_aPrefix[i], &lpDestItem->m_aPrefix[_iPrefixIndex], sizeof(CItemPrefixInfo));
				iResult = ePCOPR_OK;
				arrangeInventory();
				sendUpdateItemData(_iDestItem,TRUE);			
				sendEnchantItemLog(lpDestItem,eEIW_SUCCESS_COPY_PREFIX);
				break;
			}
		}
	}
	else
	{
		sendEnchantItemLog(lpDestItem,eEIW_FAILED_COPY_PREFIX);
		lpDestItem->decreaseCount(1);
		iResult = ePCOPR_FAILED;
	}

	lpUseItem->decreaseCount(1);
	sendUseItemLog(lpUseItem);


	SG_USE_ITEM_TO_ITEM_RESULT	sgPacket;
	
	sgPacket.base.set(sizeof(sgPacket),dSG_USE_ITEM_TO_ITEM_RESULT);
	sgPacket.bItemSlot		=	_iUseItem;
	sgPacket.bTargetItemSlot=	_iDestItem;
	sgPacket.wResult		=	iResult;
	
	g_userPM.add(m_iClientSerial,&sgPacket,sgPacket.base.wSize);


	return	TRUE;
}







BOOL
cACTOR::cutPrefix(int _iUseItem,int _iDestItem,int _iPrefixIndex)
{
	cItem	*lpUseItem	=	getInventoryItem(_iUseItem);
	cItem	*lpDestItem	=	getInventoryItem(_iDestItem);

	if	(!lpUseItem	||	!lpDestItem	||	lpDestItem->getPrefixCount() < _iPrefixIndex)
		return	FALSE;

	if	(lpDestItem->isExceptionItem())
		return	FALSE;

	if(lpDestItem->m_aPrefix[_iPrefixIndex].m_wPrefix == 0xffff)
		return FALSE;

	cBasicItem*	lpBasicUseItem = lpUseItem->getBasicItem();

	if(!lpBasicUseItem)
		return FALSE;
	
	cItem	item;

	switch(lpBasicUseItem->m_aGenerateData[0].m_wEffect)
	{
		
	case eIE_CUT_PREFIX:
		g_im.generateItemByBaseItem(&item, dITEM_INDEX_ATTACH_PREFIX,1);
		break;
		
	case eIE_CUT_PREFIX_CAN_NOT_TRADE:
		g_im.generateItemByBaseItem(&item, dITEM_INDEX_ATTACH_PREFIX_CAN_NOT_TRADE,1);
		break;
		
	default:
		return FALSE;
		
	}

	CItemPrefixInfo	prefixInfo;


	cBasicItem* lpBasic = item.getBasicItem();
	
	if(!lpBasic)
		return FALSE;

	item.m_bDurability = lpBasic->getDurability()*100/100;
	item.m_aOption[0] = lpUseItem->m_aOption[0];
	memcpy(&prefixInfo,&lpDestItem->m_aPrefix[_iPrefixIndex],sizeof(CItemPrefixInfo));
	item.m_dwSerial	=	largeRandom(0x7ffffff0)+10;
	int iResult = g_im.attachPrefix(&item,prefixInfo.m_wPrefix, prefixInfo.m_aValue[0],prefixInfo.m_aValue[1]);

	cItem	*lpCopyItem	=	getEmptyItemSlot();

	if	(!lpCopyItem)
	{
		iResult = ePCR_INVENTORY_FULL;
	}

	if(iResult == eAPR_SUCCESS)
	{
		for	(int i=_iPrefixIndex;i<dITEM_PREFIX_COUNT-1;i++)
			memcpy(&lpDestItem->m_aPrefix[i],&lpDestItem->m_aPrefix[i+1],sizeof(CItemPrefixInfo));
		
		memset(&lpDestItem->m_aPrefix[dITEM_PREFIX_COUNT-1],0xff,sizeof(CItemPrefixInfo));

		iResult = ePCR_OK;
		sendUseItemLog(lpUseItem);
		lpUseItem->decreaseCount(1);
		lpCopyItem->copy(&item);	
		sendAddItem(lpCopyItem);
		sendCreateItemLog(lpCopyItem,eCIM_CUT_PREFIX);		///접두사 잘라내서 아이템이 생성됐다고 로그서버에 보낸당.
	}
	
	SG_USE_ITEM_TO_ITEM_RESULT	sgPacket;
	
	sgPacket.base.set(sizeof(sgPacket),dSG_USE_ITEM_TO_ITEM_RESULT);
	sgPacket.bItemSlot		=	_iUseItem;
	sgPacket.bTargetItemSlot=	_iDestItem;
	sgPacket.wResult		=	iResult;
	
	g_userPM.add(m_iClientSerial,&sgPacket,sgPacket.base.wSize);
	return	TRUE;
}

//
//	프리미엄 아이템의 유효기간 체크
void
cACTOR::checkItemValidateTerm()
{
	BOOL	bIsExistEndItem	=	FALSE;
	int		i;

	for	(i=dEQUIPMENT_PART_COUNT+dBORDER_OF_ITEM_AND_EQUIPMENT;i;)	//	
	{
		--i;

		cItem		*lpItem		=	getInventoryItem(i);

		if	(!lpItem)
			continue;

		if	(lpItem->isPrincessWeapon())
		{
			cPrincessWeaponDefine	*lpWeapon	=	(cPrincessWeaponDefine	*)lpItem;
			cACTOR					*lpPrincess	=	m_lpField->getActor(lpWeapon->m_wPrincessSerial);

			if	(!lpPrincess	||	lpPrincess->m_dwNameHashCode	!=	lpWeapon->m_dwPrincessNameHashCode)
			{
				lpItem->reset();
				sendUpdateItemData(i);
				bIsExistEndItem	=	TRUE;
			}

			continue;
		}

		if	(lpItem->m_wBaseItem==	0xffff)
			continue;

		if	(lpItem->isExpiredCoupleRing())
		{
			sendExpiredItemLog(lpItem);

			bIsExistEndItem	=	TRUE;
			sendExpiredItem(i,lpItem->m_wBaseItem);
			lpItem->reset();
			continue;
		}

		if	(lpItem->isExtraItem() || lpItem->isExceptionItem() || lpItem->isSpecialItem())
			continue;

		if	(lpItem->isExpiredItem())
		{
			sendExpiredItemLog(lpItem);

			bIsExistEndItem	=	TRUE;
			sendExpiredItem(i,lpItem->m_wBaseItem);
			lpItem->reset();
		}

	}

	if	(bIsExistEndItem)
		buildPower();
}

//	배지의 수를 리퉈어어언~~
int
cACTOR::getBadgeCount()
{
	int		i,iBadgeCount=0;

	for (i=0;i<dOWN_ITEM_COUNT;i++)
	{
		cItem		*lpItem			=	&m_aItems[i];
		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(lpBasicItem	==	NULL)
			continue;

		if	(lpBasicItem->m_attr.isBadge)
			iBadgeCount++;
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		cItem		*lpItem			=	&m_aExtraInventory[i];
		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(lpBasicItem	==	NULL)
			continue;

		if	(lpBasicItem->m_attr.isBadge)
			iBadgeCount++;
	}

	return	iBadgeCount;
}

//	탄환이 필요한 스킬이었다면 탄환을 줄인다.
void
cACTOR::decreaseBullet(cAbility *_lpAbility)
{
	if	(_lpAbility->isDecreaseWeaponCountSkill())
	{
		cItem	*lpWeapon	=	getWeapon();

		if	(lpWeapon	&&	!lpWeapon->isInfinityBullet()	&&	!lpWeapon->isPrincessWeapon())
			lpWeapon->decreaseCount(1);
	}

	if	(_lpAbility->isRequireBullet())
	{
		int	iCount	=	_lpAbility->getContinuousShotCount();

		if	(iCount	<=	0)	
			iCount	=	1;

		cItem	*lpBullet	=	getBullet();
		
		if(!lpBullet && m_isMagicBullet)
			return;
		
		if	(lpBullet	&&	!lpBullet->isInfinityBullet())	
			lpBullet->decreaseCount(iCount,FALSE);
	}
}
//	무기의 컬러 이펙트
int
cACTOR::getWeaponColorizeEffect()
{
	int		aEffect[dITEM_PREFIX_COUNT],i,iEffectCount=0,iEffect=0xffff;
	cItem	*lpWeapon	=	getWeapon();

	if	(!lpWeapon)
		return	0xffff;

	cBasicItem *lpWeponBasicItem = lpWeapon->getBasicItem(TRUE);
	
	if(!lpWeponBasicItem)
		return	0xffff;

	if	(lpWeapon->isPrincessWeapon())
		return	ePOE_PRINCESS_WEAPON;

	if	(lpWeponBasicItem->m_bCompulsionColorizeEffect)
 		aEffect[iEffectCount++]	=	lpWeponBasicItem->m_bCompulsionColorizeEffect;

	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (lpWeapon->m_aPrefix[i].m_wPrefix	==	0xffff)
			break;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[lpWeapon->m_aPrefix[i].m_wPrefix];

		if (lpPrefix->m_bWeaponEffect	!=	0xff)
			aEffect[iEffectCount++]	=	lpPrefix->m_bWeaponEffect;
	}

	if (iEffectCount	==	0)
		return	0xffff;

	if (iEffectCount	==	1)
		return	aEffect[0];

	for (i=0;i<iEffectCount;i++)
	{
		if (iEffect	==	0xffff)
			iEffect	=	aEffect[i];
		else
		{
			if (aEffect[i]%3 > iEffect%3)
				iEffect	=	aEffect[i];
		}
	}

	return	iEffect;
}

//	방패의 컬러 이펙트
int
cACTOR::getShieldColorizeEffect()
{
	return	0xffff;
}

//	방어구의 컬러 이펙트
int
cACTOR::getArmorColorizeEffect()
{
	return	0xffff;
}

//
//	OX퀴즈 티켓을 가지고 있다.
BOOL
cACTOR::isOwnedOXQuizTicket()
{
	int i;

	for (i=0;i<getItemSlotCount();i++)
		if (m_aItems[i].isOXQuizTicket())
			return	TRUE;

	for (i=0;i<m_bf6ExtraInventorySize;i++)
		if (m_aExtraInventory[i].isOXQuizTicket())
			return	TRUE;

	return	FALSE;
}

BOOL
cACTOR::isOwnGuildHallPortal()
{
	int i;

	for (i=0;i<getItemSlotCount();i++)
		if	(m_aItems[i].isGuildHallPortal())
			return	TRUE;

	for (i=0;i<m_bf6ExtraInventorySize;i++)
		if	(m_aExtraInventory[i].isGuildHallPortal())
			return	TRUE;

	return	FALSE;
}

BOOL
cACTOR::openRepairItemWindow(cACTOR *_lpOwner)
{
	sendOpenRepairWindow(_lpOwner->m_wSerialInField);

	return	TRUE;
}

BOOL
cACTOR::openDisjointingItemWindow(cACTOR *_lpOwner)
{
	sendOpenDisjointingWindow(_lpOwner->m_wSerialInField);
	
	return	TRUE;
}

//
//	OX 퀴즈 티켓 제거
void
cACTOR::removeOXQuizTicket()
{
	int	i;
	
	for (i=0;i<getItemSlotCount();i++)
		if (m_aItems[i].isOXQuizTicket())
		{
			m_aItems[i].reset();
			sendUpdateItemData(i);
		}
	for (i=0;i<m_bf6ExtraInventorySize;i++)
		if (m_aExtraInventory[i].isOXQuizTicket())
		{
			m_aExtraInventory[i].reset();
			sendUpdateItemData(i+dOWN_ITEM_COUNT);
		}
}

void
cACTOR::removeGuildBattleItem()
{
	int i,iRemoveItemCount=0;

	for (i=0;i<getItemSlotCount();i++)
		if (m_aItems[i].isGuildBattleItem())
		{
			sendRemoveItemLog(&m_aItems[i],eRI_GUILD_BATTLE_ITEM_BY_NOT_GUILD_BATTLE_FIELD);
			m_aItems[i].reset();
			sendUpdateItemData(i);
			iRemoveItemCount++;
		}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
		if (m_aExtraInventory[i].isGuildBattleItem())
		{
			sendRemoveItemLog(&m_aExtraInventory[i],eRI_GUILD_BATTLE_ITEM_BY_NOT_GUILD_BATTLE_FIELD);
			m_aExtraInventory[i].reset();
			sendUpdateItemData(i+dOWN_ITEM_COUNT);
			iRemoveItemCount++;
		}
}

void
cACTOR::breakdownArmorDurability()
{
#define	dDURABILITY_BREAKDOWN_VALUE	100
	if	(random(dDURABILITY_BREAKDOWN_VALUE)	!=	0)
		return;

	cBasicItem	*lpBasicItem;
	int			iDurability;

//	방패
	{
		cItem	*lpShield	=	&m_aEquip[dEQUIP_SHIELD_BULLET];

		lpBasicItem			=	lpShield->getBasicItem();
		
		if	(lpShield->isBreakdownAbleItem() && lpShield->m_bDurability && lpBasicItem->m_wKind != eIK_ARROW && lpBasicItem->m_wKind != eIK_BULLET)
		{
			if	(lpBasicItem->isEnableJob(m_wJob) && isUseAbleItem(lpShield))
			{
				if	(lpShield->isDXItem())
					iDurability	=	c_aDX_ItemDurability[lpBasicItem->m_wKind]*lpBasicItem->m_wBoostDurability/100;
				else
					iDurability	=	c_aUltimateItemDurability[lpBasicItem->m_wKind]*lpBasicItem->m_wBoostDurability/100;

				if	(random(iDurability/dDURABILITY_BREAKDOWN_VALUE) == 0)
				{
					lpShield->m_bDurability--;

					sendUpdateItemDurability(dEQUIP_SHIELD_BULLET);
				}
			}
		}
	}

	for (int iPart=dEQUIP_ARMOR;iPart<dEQUIP_WEAPON_2;iPart++)
	{
		cItem	*lpArmor	=	&m_aEquip[iPart];
		lpBasicItem	=	lpArmor->getBasicItem(TRUE);

		if	(lpArmor->m_bDurability	==	0)
			continue;
		if	(lpArmor->isBreakdownAbleItem() == FALSE)
			continue;
		if	(!lpBasicItem->isEnableJob(m_wJob))
			continue;
		if	(!isUseAbleItem(lpArmor))
			continue;

		if	(lpArmor->isDXItem())
			iDurability		=	c_aDX_ItemDurability[lpBasicItem->m_wKind]*lpBasicItem->m_wBoostDurability/100;
		else
			iDurability		=	c_aUltimateItemDurability[lpBasicItem->m_wKind]*lpBasicItem->m_wBoostDurability/100;

		if	(random(iDurability/dDURABILITY_BREAKDOWN_VALUE) == 0)
		{
			lpArmor->m_bDurability--;

			sendUpdateItemDurability(iPart);
		}
	}
}

void
cACTOR::breakdownWeaponDurability()
{
#define	dDURABILITY_BREAKDOWN_VALUE	100

	if	(random(dDURABILITY_BREAKDOWN_VALUE)	!=	0)
		return;

	cBasicItem	*lpBasicItem;
	int			iDurability;
	cItem		*lpWeapon;

//	무한 탄환
	{
		lpWeapon	=	&m_aEquip[dEQUIP_SHIELD_BULLET];
		lpBasicItem	=	lpWeapon->getBasicItem(TRUE);

		if	(lpWeapon->isBreakdownAbleItem() && lpWeapon->m_bDurability && lpBasicItem->m_wKind	!=	eIK_SHIELD && lpWeapon->isInfinityBullet())	//	무한 화살
		{
			if	(lpBasicItem->isEnableJob(m_wJob) && isUseAbleItem(lpWeapon))
			{
				if	(lpWeapon->isDXItem())
					iDurability		=	c_aDX_ItemDurability[lpBasicItem->m_wKind]*lpBasicItem->m_wBoostDurability/100;
				else
					iDurability		=	c_aUltimateItemDurability[lpBasicItem->m_wKind]*lpBasicItem->m_wBoostDurability/100;

				if	(random(iDurability/dDURABILITY_BREAKDOWN_VALUE) == 0)
				{
					lpWeapon->m_bDurability--;

					sendUpdateItemDurability(dEQUIP_SHIELD_BULLET);
				}
			}
		}
	}

//	무기
	while(1)
	{
		lpWeapon	=	getWeapon();

		if	(lpWeapon	==	NULL)
			break;

		lpBasicItem	=	lpWeapon->getBasicItem(TRUE);

		if	(lpWeapon->isBreakdownAbleItem() && lpWeapon->m_bDurability && isUseAbleItem(lpWeapon))
		{
			if	(lpWeapon->isDXItem())
				iDurability	=	c_aDX_ItemDurability[lpBasicItem->m_wKind]*lpBasicItem->m_wBoostDurability/100;
			else
				iDurability	=	c_aUltimateItemDurability[lpBasicItem->m_wKind]*lpBasicItem->m_wBoostDurability/100;

			if	(random(iDurability/dDURABILITY_BREAKDOWN_VALUE) == 0)
			{
				int	iPart	=	lpWeapon-m_aEquip;

				lpWeapon->m_bDurability--;

				sendUpdateItemDurability(iPart);
			}
		}

		break;
	}
}

int
cACTOR::getBookCompletePercentage(int _iBook)
{
	int	i,iPercentage	=	-1;

	for	(i=0;i<dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE;i++)	//	동일한 아이템 조각 검색
	{
		cItem		*lpItem		=	getInventoryItem(i);

		if	(!lpItem)
			continue;

		cBookItem	*lpBookItem	=	lpItem->getBook();

		if	(!lpBookItem	||	lpBookItem->m_wBookIndex	!=	_iBook)
			continue;

		iPercentage	=	max(lpBookItem->getCompletePercentage(),iPercentage);
	}

	return	iPercentage;
}

BOOL
cACTOR::isAvailBookPage(int _iBook,int _iCategory,int _iPage)
{
	int	i;

	for	(i=0;i<dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE;i++)	//	동일한 아이템 조각 검색
	{
		cItem		*lpItem		=	getInventoryItem(i);

		if	(!lpItem)
			continue;

		cBookItem	*lpBookItem	=	lpItem->getBook();

		if	(!lpBookItem	||	lpBookItem->m_wBookIndex	!=	_iBook)
			continue;

		if	(lpBookItem->isWrittenPage(_iCategory,_iPage))
			return	TRUE;
	}

	return	FALSE;
}

cBookPageItemDefine*
cACTOR::getAddBookPageItem(int _iBook,int _iCategory,int _iPage)
{
	int	i,iPercentage	=	-1;

	for	(i=0;i<dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE;i++)	//	동일한 아이템 조각 검색
	{
		cItem		*lpItem		=	getInventoryItem(i);

		if	(!lpItem)
			continue;

		cBookPageItemDefine	*lpBookPage	=	lpItem->getAddPageToBookItem();

		if	(!lpBookPage	||	lpBookPage->m_wBookIndex	!=	_iBook	||	lpBookPage->m_wCategory	!=	_iCategory)
			continue;

		if	(_iPage	<	lpBookPage->m_wBeginPage	||	_iPage	>	lpBookPage->m_wEndPage)
			continue;

		return	lpBookPage;
	}

	return	NULL;
}


void
cACTOR::writeBookPage(int _iBook,int _iCategory,int _iPage)
{
	int	i,iPercentage	=	-1;

	for	(i=0;i<dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE;i++)	//	동일한 아이템 조각 검색
	{
		cItem		*lpItem		=	getInventoryItem(i);

		if	(!lpItem)
			continue;

		cBookItem	*lpBookItem	=	lpItem->getBook();

		if	(!lpBookItem	||	lpBookItem->m_wBookIndex	!=	_iBook)
			continue;

		lpBookItem->writePage(_iCategory,_iPage);

		sendUpdateItemData(i,TRUE);
	}
}

void
cACTOR::eraseBookPage(int _iBook,int _iCategory,int _iPage)
{
	int	i,iPercentage	=	-1;

	for	(i=0;i<dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE;i++)	//	동일한 아이템 조각 검색
	{
		cItem		*lpItem		=	getInventoryItem(i);

		if	(!lpItem)
			continue;

		cBookItem	*lpBookItem	=	lpItem->getBook();

		if	(!lpBookItem	||	lpBookItem->m_wBookIndex	!=	_iBook)
			continue;

		lpBookItem->erasePage(_iCategory,_iPage);

		sendUpdateItemData(i,TRUE);
	}
}

BOOL
cACTOR::removeBook(int _iBook)
{
	int	i;

	for	(i=0;i<dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE;i++)	//	동일한 아이템 조각 검색
	{
		cItem		*lpItem		=	getInventoryItem(i);

		if	(!lpItem)
			continue;

		cBookItemDefine	*lpBookItem	=	lpItem->getBook();

		if	(!lpBookItem	||	lpBookItem->m_wBookIndex	!=	_iBook)
			continue;

		sendRemoveItemLog(lpItem,eRI_BY_KARMA);
		lpItem->reset();

		sendRemoveItem(i,1,TRUE);
	}

	return	TRUE;
}

BOOL
cACTOR::removeAddPageItem(int _iBook,int _iCategory,int _iBeginPage,int _iEndPage)
{
	int	i;

	for	(i=0;i<dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE;i++)	//	동일한 아이템 조각 검색
	{
		cItem		*lpItem		=	getInventoryItem(i);

		if	(!lpItem)
			continue;

		cBookPageItemDefine	*lpBookPage	=	lpItem->getAddPageToBookItem();

		if	(!lpBookPage	||	lpBookPage->m_wBookIndex	!=	_iBook	||	lpBookPage->m_wCategory	!=	_iCategory)
			continue;

		if	(lpBookPage->m_wBeginPage	!=	_iBeginPage	||	lpBookPage->m_wEndPage	!=	_iEndPage	)
			continue;

		sendRemoveItemLog(lpItem,eRI_BY_KARMA);
		lpItem->reset();

		sendRemoveItem(i,1,TRUE);
	}

	return	TRUE;
}

BOOL
cACTOR::removePrefx(int _iUseItem,int _iDestItem,int _iPrefixIndex)
{
	cItem	*lpUseItem	=	getInventoryItem(_iUseItem);
	cItem	*lpDestItem	=	getInventoryItem(_iDestItem);

	if	(!lpUseItem	||	!lpDestItem	||	lpDestItem->getPrefixCount() <= _iPrefixIndex)
		return	FALSE;

	if	(lpDestItem->isExceptionItem())
		return	FALSE;

	if	(!lpUseItem->checkFirstEffect(eIE_REMOVE_PREFIX) && !lpUseItem->checkFirstEffect(eIE_CH5_REMOVE_PREFIX))
		return	FALSE;

	for	(int i=_iPrefixIndex;i<dITEM_PREFIX_COUNT-1;i++)
		memcpy(&lpDestItem->m_aPrefix[i],&lpDestItem->m_aPrefix[i+1],sizeof(CItemPrefixInfo));

	memset(&lpDestItem->m_aPrefix[dITEM_PREFIX_COUNT-1],0xff,sizeof(CItemPrefixInfo));

	sendUseItemLog(lpUseItem);
	sendEnchantItemLog(lpDestItem,eEIW_REMOVE_PREFIX);

	lpUseItem->decreaseCount(1);

	SG_REMOVE_PREFIX	packet;

	packet.base.set(sizeof(packet),dSG_REMOVE_PREFIX);

	packet.wUseItem		=	_iUseItem;
	packet.wDestItem	=	_iDestItem;
	packet.wPrefixIndex	=	_iPrefixIndex;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);

	return	TRUE;
}

BOOL
cACTOR::judgeItem(int _iItemSlot)
{
	cItem	*lpItem	=	getInventoryItem(_iItemSlot);

	if	(!lpItem	||	lpItem->isUnknownItem()	==	FALSE)
		return	FALSE;

	int	iJudgePrice	=	lpItem->getJudgePrice();

	if	(m_iGold	<	iJudgePrice)
	{
		sendRegistServerMessage(eRSM_LACK_MONEY_FOR_JUDGE_ITEM,eSM_MSG_BOX);
			
		return	FALSE;
	}

	m_iGold			-=	iJudgePrice;

	updateGoldLog(-iJudgePrice,eSPENT_GOLD_BY_JUDGE_ITEM);

	cSpecialItem	*lpSpecialItem	=	lpItem->getSpecialItem();

	lpSpecialItem->m_bf1IsUnknown	=	FALSE;

	sendEtcWork(eEW_JUDGE_ITEM,_iItemSlot,iJudgePrice/0xffff,iJudgePrice%0xffff);
	sendEnchantItemLog(lpItem,eEIW_JUDGE_ITEM,iJudgePrice);

	return	TRUE;
}

BOOL
cACTOR::mergeItem(int _iItemSlot)
{
	cItem	*lpFirstItem	=	getInventoryItem(_iItemSlot);

	if	(!lpFirstItem	||	lpFirstItem->isUnknownItem())
		return	FALSE;

	cPieceItem	*lpFirstPiece=	lpFirstItem->getPieceItem();		//	임의의 조각을 선택한후에

	if	(!lpFirstPiece)
		return	FALSE;

	int		iMergePrice			=	lpFirstPiece->getMergePrice();

	if	(m_iGold	<	iMergePrice)
	{
		sendRegistServerMessage(eRSM_LACK_MONEY_FOR_MERGE_ITEM,eSM_MSG_BOX);
			
		return	FALSE;
	}

	int		i;
	int		iPieceCount			=	0;
	WORD	awPieceList[dMAX_PIECE_ITEM_PIECE_COUNT];

	awPieceList[iPieceCount++]	=	_iItemSlot;

	BYTE	abExistPiece[dMAX_PIECE_ITEM_PIECE_COUNT];

	memset(abExistPiece,0,sizeof(abExistPiece));

	abExistPiece[lpFirstItem->m_aOption[0]]	=	1;

	for	(i=0;i<dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE;i++)	//	동일한 아이템 조각 검색
	{
		if	(i	==	_iItemSlot)
			continue;

		cItem		*lpItem		=	getInventoryItem(i);

		if	(!lpItem	||	lpItem->isUnknownItem())
			continue;

		cPieceItem	*lpPieceItem=	lpItem->getPieceItem();

		if	(!lpPieceItem)
			continue;

		if	(lpPieceItem->m_wSerial	!=	lpFirstPiece->m_wSerial)
			continue;

		if	(abExistPiece[lpItem->m_aOption[0]]	)
			continue;

		abExistPiece[lpItem->m_aOption[0]]	=	1;
		awPieceList[iPieceCount++]			=	i;

		if	(iPieceCount	>=	lpFirstPiece->m_wPieceCount)
			break;
	}

	if	(lpFirstPiece->m_wPieceCount	>	iPieceCount)
		return	FALSE;

	for	(i=1;i<iPieceCount;i++)
	{
		cItem		*lpItem		=	getInventoryItem(awPieceList[i]);

		lpItem->reset();
	}

	lpFirstPiece->generateItem(lpFirstItem);
	m_iGold			-=	iMergePrice;

	updateGoldLog(-iMergePrice,eSPENT_GOLD_BY_MERGE_ITEM);

	BOOL	bIsCanNotTradeItem	=	FALSE;

	for	(i=0;i<iPieceCount;i++)
	{
		if	(lpFirstPiece->m_aPiece[i].m_bf1IsReversion)
			bIsCanNotTradeItem	=	TRUE;
	}
	lpFirstItem->getSpecialItem()->m_bf1IsReversionItem	=	bIsCanNotTradeItem;

	sendEtcWork(eEW_MERGE_ITEM,_iItemSlot,iMergePrice/0xffff,iMergePrice%0xffff);
	sendEnchantItemLog(lpFirstItem,eEIW_MERGE_ITEM,iMergePrice);

	return	TRUE;
}

cItem*
cACTOR::getNonameCoupleRing()
{
	int	i;

	for (i=0;i<getItemSlotCount();i++)
	{
		cCoupleRingDefine*	lpCoupleRing	=	m_aItems[i].getNotDefineCoupleCoupleRing();

		if	(!lpCoupleRing)
			continue;

		return	&m_aItems[i];
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		cCoupleRingDefine*	lpCoupleRing	=	m_aExtraInventory[i].getNotDefineCoupleCoupleRing();

		if	(!lpCoupleRing)
			continue;

		return	&m_aExtraInventory[i];
	}

	return	NULL;
}

int
cACTOR::getCoupleRingIndex(char *_lpstrName)
{
	int	i;

	for (i=0;i<getItemSlotCount();i++)
	{
		cCoupleRingDefine*	lpCoupleRing	=	m_aItems[i].getCoupleRing();

		if	(!lpCoupleRing)
			continue;

		if	(STRICMP(lpCoupleRing->getCoupleName(),_lpstrName)==0)
			return	i;
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		cCoupleRingDefine*	lpCoupleRing	=	m_aExtraInventory[i].getCoupleRing();

		if	(!lpCoupleRing)
			continue;

		if	(STRICMP(lpCoupleRing->getCoupleName(),_lpstrName)==0)
			return	i+dOWN_ITEM_COUNT;
	}

	for (i=dEQUIP_RING1;i<=dEQUIP_RING8;i++)
	{
		cCoupleRingDefine*	lpCoupleRing	=	m_aEquip[i].getCoupleRing();

		if	(!lpCoupleRing)
			continue;

		if	(STRICMP(lpCoupleRing->getCoupleName(),_lpstrName)==0)
			return	i+dBORDER_OF_ITEM_AND_EQUIPMENT;
	}

	return	0xffff;
}

cItem*
cACTOR::getFirstPotion(cAbility *_lpAbility)
{
	int	i;

	for (i=0;i<dEQUIPMENT_PART_COUNT+dBORDER_OF_ITEM_AND_EQUIPMENT;i++)
	{
		cItem	*lpItem				=	getInventoryItem(i);

		if	(!lpItem)
			continue;

		if	(!_lpAbility->isThrowAblePotion(lpItem))
			continue;

		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(!lpBasicItem	||	lpBasicItem->m_wKind	!=	eIK_HEAL_POTION)
			continue;

		return	lpItem;
	}

	return	NULL;
}

cItem*
cACTOR::getCureAllWrongStatusItem()
{
	int		i;
	int		iMinDropLevel=10000,iMinDropLevelItem=0xffff;

	for (i=0;i<dEQUIPMENT_PART_COUNT+dBORDER_OF_ITEM_AND_EQUIPMENT;i++)
	{
		cItem	*lpItem				=	getInventoryItem(i);

		if	(!lpItem	||	!lpItem->isCureAllWrongStatusItem())
			continue;

		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(!lpBasicItem	||	lpBasicItem->m_wKind	!=	eIK_DRUG)
			continue;

		if	(lpBasicItem->m_wDropLevel	<	iMinDropLevel	)
		{
			iMinDropLevel		=	lpBasicItem->m_wDropLevel;
			iMinDropLevelItem	=	i;
		}
	}

	if	(iMinDropLevelItem==0xffff)
		return	NULL;

	return	getInventoryItem(iMinDropLevelItem);
}

cItem*
cACTOR::getCurePoisonStatusItem()
{
	int		i;
	int		iMinDropLevel=10000,iMinDropLevelItem=0xffff;

	for (i=0;i<dEQUIPMENT_PART_COUNT+dBORDER_OF_ITEM_AND_EQUIPMENT;i++)
	{
		cItem	*lpItem				=	getInventoryItem(i);

		if	(!lpItem	||	!lpItem->isCurePoisonStatusItem())
			continue;

		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(!lpBasicItem	||	lpBasicItem->m_wKind	!=	eIK_DRUG)
			continue;

		if	(lpBasicItem->m_wDropLevel	<	iMinDropLevel	)
		{
			iMinDropLevel		=	lpBasicItem->m_wDropLevel;
			iMinDropLevelItem	=	i;
		}
	}

	if	(iMinDropLevelItem==0xffff)
		return	NULL;

	return	getInventoryItem(iMinDropLevelItem);
}

cItem*
cACTOR::getCureStrangeStatusItem()
{
	int		i;
	int		iMinDropLevel=10000,iMinDropLevelItem=0xffff;

	for (i=0;i<dEQUIPMENT_PART_COUNT+dBORDER_OF_ITEM_AND_EQUIPMENT;i++)
	{
		cItem	*lpItem				=	getInventoryItem(i);

		if	(!lpItem	||	!lpItem->isCureStrangeStatusItem())
			continue;

		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(!lpBasicItem	||	lpBasicItem->m_wKind	!=	eIK_DRUG)
			continue;

		if	(lpBasicItem->m_wDropLevel	<	iMinDropLevel	)
		{
			iMinDropLevel		=	lpBasicItem->m_wDropLevel;
			iMinDropLevelItem	=	i;
		}
	}

	if	(iMinDropLevelItem==0xffff)
		return	NULL;

	return	getInventoryItem(iMinDropLevelItem);
}

cItem*
cACTOR::getCureDeclinePowerStatusItem()
{
	int		i;
	int		iMinDropLevel=10000,iMinDropLevelItem=0xffff;

	for (i=0;i<dEQUIPMENT_PART_COUNT+dBORDER_OF_ITEM_AND_EQUIPMENT;i++)
	{
		cItem	*lpItem				=	getInventoryItem(i);

		if	(!lpItem	||	!lpItem->isCureDeclinePowerStatusItem())
			continue;

		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(!lpBasicItem	||	lpBasicItem->m_wKind	!=	eIK_DRUG)
			continue;

		if	(lpBasicItem->m_wDropLevel	<	iMinDropLevel	)
		{
			iMinDropLevel		=	lpBasicItem->m_wDropLevel;
			iMinDropLevelItem	=	i;
		}
	}

	if	(iMinDropLevelItem==0xffff)
		return	NULL;

	return	getInventoryItem(iMinDropLevelItem);
}

cItem*
cACTOR::getCureCurseStatusItem()
{
	int		i;
	int		iMinDropLevel=10000,iMinDropLevelItem=0xffff;

	for (i=0;i<dEQUIPMENT_PART_COUNT+dBORDER_OF_ITEM_AND_EQUIPMENT;i++)
	{
		cItem	*lpItem				=	getInventoryItem(i);

		if	(!lpItem	||	!lpItem->isCureCurseStatusItem())
			continue;

		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(!lpBasicItem	||	lpBasicItem->m_wKind	!=	eIK_DRUG)
			continue;

		if	(lpBasicItem->m_wDropLevel	<	iMinDropLevel	)
		{
			iMinDropLevel		=	lpBasicItem->m_wDropLevel;
			iMinDropLevelItem	=	i;
		}
	}

	if	(iMinDropLevelItem==0xffff)
		return	NULL;

	return	getInventoryItem(iMinDropLevelItem);
}

cItem*
cACTOR::getFirstDrug(cACTOR *_lpTarget)
{
	int		i;
	int		iMinDropLevel=10000,iMinDropLevelItem=0xffff;

	if	(_lpTarget	&&	_lpTarget->isWrongStatus())
	{
		BOOL	bIsStrangeStatus		=	_lpTarget->isStrangeStatus();
		BOOL	bIsDeclinePowerStatus	=	_lpTarget->isDeclinePowerStatus();
		BOOL	bIsCurseStatus			=	_lpTarget->isCurseStatus();
		BOOL	bIsPoisoned				=	FALSE;
		cItem	*lpDrug;

		if	(_lpTarget->m_wPoisonTime)
			bIsPoisoned				=	TRUE;

		if	(bIsStrangeStatus+bIsDeclinePowerStatus+bIsCurseStatus+bIsPoisoned	>=	2)
		{
			lpDrug	=	getCureAllWrongStatusItem();

			if	(lpDrug)
				return	lpDrug;
		}
		if	(bIsPoisoned)
		{
			lpDrug	=	getCurePoisonStatusItem();

			if	(lpDrug)
				return	lpDrug;
		}
		if	(bIsStrangeStatus)
		{
			lpDrug	=	getCureStrangeStatusItem();

			if	(lpDrug)
				return	lpDrug;
		}
		if	(bIsDeclinePowerStatus)
		{
			lpDrug	=	getCureDeclinePowerStatusItem();

			if	(lpDrug)
				return	lpDrug;
		}
		if	(bIsCurseStatus)
		{
			lpDrug	=	getCureCurseStatusItem();

			if	(lpDrug)
				return	lpDrug;
		}
	}

	for (i=0;i<dEQUIPMENT_PART_COUNT+dBORDER_OF_ITEM_AND_EQUIPMENT;i++)
	{
		cItem	*lpItem				=	getInventoryItem(i);

		if	(!lpItem)
			continue;

		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(!lpBasicItem	||	lpBasicItem->m_wKind	!=	eIK_DRUG)
			continue;

		if	(lpBasicItem->m_wDropLevel	<	iMinDropLevel	)
		{
			iMinDropLevel		=	lpBasicItem->m_wDropLevel;
			iMinDropLevelItem	=	i;
		}
	}

	if	(iMinDropLevelItem==0xffff)
		return	NULL;

	return	getInventoryItem(iMinDropLevelItem);
}

cItem*
cACTOR::getFirstFlower()
{
	int	i;

	for (i=0;i<dEQUIPMENT_PART_COUNT+dBORDER_OF_ITEM_AND_EQUIPMENT;i++)
	{
		cItem	*lpItem				=	getInventoryItem(i);

		if	(!lpItem)
			continue;

		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(!lpBasicItem	||	lpBasicItem->m_bf1IsUseAbleByThrowFlowerSkill	==	FALSE)
			continue;

		return	lpItem;
	}

	return	NULL;
}

cItem*
cACTOR::getFirstCandy()
{
	int	i;

	for (i=0;i<dEQUIPMENT_PART_COUNT+dBORDER_OF_ITEM_AND_EQUIPMENT;i++)
	{
		cItem	*lpItem				=	getInventoryItem(i);

		if	(!lpItem)
			continue;

		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(!lpBasicItem	||	lpBasicItem->m_bf1IsUseAbleByThrowCandySkill	==	FALSE)
			continue;

		return	lpItem;
	}

	return	NULL;
}

cNameStone*
cACTOR::getNameStone()
{
	int	i;

	for (i=0;i<getItemSlotCount();i++)
	{
		cItem	*lpItem	=	&m_aItems[i];

		if	(lpItem->m_wBaseItem	==	0xffff	||	lpItem->isOwnSpecificEffectItem(eIE_NAME_STONE) == FALSE)
			continue;

		return	(cNameStone*)lpItem;
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		cItem	*lpItem	=	&m_aExtraInventory[i];

		if	(lpItem->m_wBaseItem	==	0xffff	||	lpItem->isOwnSpecificEffectItem(eIE_NAME_STONE) == FALSE)
			continue;

		return	(cNameStone*)lpItem;
	}

	return	NULL;
}

int
cACTOR::getOperatorShapeByBook()
{
	int	i;

	for (i=0;i<getItemSlotCount();i++)
	{
		cItem	*lpItem	=	&m_aItems[i];

		if	(lpItem->m_wBaseItem	==	0xffff	||	lpItem->isTransToOperatorBook() == FALSE)
			continue;

		return	lpItem->m_aOption[0];
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		cItem	*lpItem	=	&m_aExtraInventory[i];

		if	(lpItem->m_wBaseItem	==	0xffff	||	lpItem->isTransToOperatorBook() == FALSE)
			continue;

		return	lpItem->m_aOption[0];
	}

	return	-1;
}


cItem*
cACTOR::getSecretDungeonKey(int _iDungeonSerial)
{
	int	i;

	for (i=0;i<getItemSlotCount();i++)
	{
		cItem	*lpItem	=	&m_aItems[i];

		if (lpItem->m_wBaseItem	==	0xffff	||	lpItem->isSecretDungeonKey() == FALSE)
			continue;

		if (lpItem->m_aPrefix[0].m_wPrefix	!=	_iDungeonSerial)
			continue;

		return	lpItem;
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		cItem	*lpItem	=	&m_aExtraInventory[i];

		if (lpItem->m_wBaseItem	==	0xffff	||	lpItem->isSecretDungeonKey() == FALSE)
			continue;

		if (lpItem->m_aPrefix[0].m_wPrefix	!=	_iDungeonSerial)
			continue;

		return	lpItem;
	}

	return	NULL;
}

cItem*
cACTOR::getMagicCarpetEmblem()
{
	int	i;

	for (i=0;i<getItemSlotCount();i++)
	{
		if (m_aItems[i].m_wBaseItem	==	0xffff)
			continue;

		if (m_aItems[i].getBasicItem()->m_attr.isMagicCarpetEmblem)
			return	&m_aItems[i];
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		if (m_aExtraInventory[i].m_wBaseItem	==	0xffff)
			continue;

		if (m_aExtraInventory[i].getBasicItem()->m_attr.isMagicCarpetEmblem)
			return	&m_aExtraInventory[i];
	}

	return	NULL;
}

int
cACTOR::getMagicCarpetEmblemIndex()
{
	int	i;

	for (i=0;i<getItemSlotCount();i++)
	{
		if (m_aItems[i].m_wBaseItem	==	0xffff)
			continue;

		if (m_aItems[i].getBasicItem()->m_attr.isMagicCarpetEmblem)
			return	i;
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		if (m_aExtraInventory[i].m_wBaseItem	==	0xffff)
			continue;

		if (m_aExtraInventory[i].getBasicItem()->m_attr.isMagicCarpetEmblem)
			return	i+dOWN_ITEM_COUNT;
	}

	return	-1;
}


int
cACTOR::getSetItemCount(int _iItemSet)
{
	int			i,iItemSetCount=0,iPart;

	cItemSet	*lpItemSet	=	g_itemSetManager.get(_iItemSet);

	if (!lpItemSet)
		return	0;

	for (i=0;i<lpItemSet->getItemCount();i++)
	{
		cSetItem	*lpSetItem	=	lpItemSet->getItem(i);

		if (!lpItemSet)
			continue;

		for (iPart=dEQUIP_WEAPON;iPart<=dEQUIP_RING8;iPart++)
		{
			cItem			*lpItem			=	getEquipment(iPart);

			if	(!lpItem)
				continue;

			cSpecialItem	*lpSpecialItem	=	lpItem->getSpecialItem();

			if	(!lpSpecialItem || lpSpecialItem->m_bf1IsSetItem == FALSE	||	lpSpecialItem->m_bf11SValue != (DWORD)_iItemSet)
				continue;

			if	(lpItem->m_wBaseItem	!=	lpSetItem->m_wBaseItem)
				continue;

			if	(!isUseAbleItem(lpItem,FALSE))
				continue;

			iItemSetCount++;

			break;
		}
	}

	return	iItemSetCount;
}

void
cACTOR::operateItemSetEffect(int _iItemSet)
{
	cItemSet	*lpItemSet	=	g_itemSetManager.get(_iItemSet);

	if (!lpItemSet)
		return;

	int	iSetItemCount	=	getSetItemCount(_iItemSet);

	for (int i=0;i<lpItemSet->getSetEffectCount(iSetItemCount);i++)
	{
		cUniqueData	*lpEffect	=	lpItemSet->getSetEffect(iSetItemCount,i);

		operateEquipmentUniqueData(lpEffect);
	}
}

void
cACTOR::operateItemSetEffect()
{
	int	aiItemSetList[dEQUIPMENT_PART_COUNT];
	
	
	memset(aiItemSetList,0,sizeof(aiItemSetList));
	
	int	iItemSetCount	=	getOperateItemSetList(aiItemSetList);
	
	for (int i=0;i<iItemSetCount;i++)
		operateItemSetEffect(aiItemSetList[i]);
}
int
cACTOR::getOperateItemSetList(int* _lpItemSetList)
{
// output	: 세트아이템의 수
	// input	: 저장할 세트아이템리스트 
	int iItemSetCount = 0;
	for (int i=dEQUIP_WEAPON;i<=dEQUIP_RING8;i++)
	{
		cItem			*lpItem			=	getEquipment(i);
		
		if	(!lpItem)
			continue;

		cSpecialItem	*lpSpecialItem	=	lpItem->getSpecialItem();
		
		if	(!lpSpecialItem || lpSpecialItem->m_bf1IsSetItem == FALSE)
			continue;

		for (int j=0;j<iItemSetCount;j++)
		{
			if	(_lpItemSetList[j]	==	(int)lpSpecialItem->m_bf11SValue)
			{
				j	=	-1;
				break;
			}
		}

		if	(j	==	-1)
			continue;

		_lpItemSetList[iItemSetCount++]	=	lpSpecialItem->m_bf11SValue;
	}

	return iItemSetCount;

}

void
cACTOR::removeItemSetEffect(int* _lpItemSetList,int* _lpItemSetCount, int _iItemSetListCount)
{

	for (int i=0;i<_iItemSetListCount;++i)
	{
		cItemSet	*lpItemSet	=	g_itemSetManager.get(_lpItemSetList[i]);
		
		if (!lpItemSet)
			return;
		
		int	iSetItemCount	=	_lpItemSetCount[i];
		
		for (int j=0;j<lpItemSet->getSetEffectCount(iSetItemCount);++j)
		{
			cUniqueData	*lpEffect	=	lpItemSet->getSetEffect(iSetItemCount,j);
			cUniqueData uniqueData;
			
			if(!lpEffect)
				continue;
			memcpy(&uniqueData, lpEffect, sizeof(cUniqueData));
			for(int k=0;k<dITEM_UNIQUE_DATA_VALUE_COUNT;++k)
			{
				uniqueData.m_aValue[k] = uniqueData.m_aValue[k]*-1;
			}

			operateEquipmentUniqueData(&uniqueData);
		}
	}
}



void
cACTOR::buyPigeonPostItem(int _iItemIndex)
{
	if	(m_bf1IsReadyToBuyPigeonPostItem	==	FALSE	||	_iItemIndex	>	(int)m_bf6EnablePigeonPostItemCount)
		return;

	m_bf1IsReadyToBuyPigeonPostItem	=	FALSE;

	cItem	item;
	char	cDeliverPrice			=	(char)m_bf8DeliverPrice;

	if	(!g_pigeonPostShop.getItemForShop(&item,_iItemIndex,cDeliverPrice))
		return;

	int		iPrice					=	item.m_dwSerial;

	if	(m_iGold<	iPrice)
		return;

	item.m_dwSerial	=	largeRandom(300000000);

	SG_BUY_ITEM		packet;

	packet.base.set(sizeof(SG_BUY_ITEM),dSG_BUY_ITEM);

	memcpy(&packet.aBuyItem[0],&item,sizeof(cItem));	//	아이템
	packet.dwPrice				=	iPrice;		//	가격
	packet.bf1IsPigeonPostItem	=	TRUE;
	packet.bf1IsDuelPointItem	=	FALSE;
	packet.wCount				=	1;

	cItem	itemAdd;

	itemAdd.copy((cItem *)&item);

	if	(itemAdd.isMoneyItem())
		CLOG("goldIOError","buyPigeonPostItem");

	int	iAddResult	=	addItemToInventory(&itemAdd);

	if	(iAddResult == eUIR_OK)
	{
		m_iGold				-=	packet.dwPrice;
		packet.iRemainGold	=	m_iGold;//	남은 돈

		LONGLONG	llSpentGold	=	packet.dwPrice;

		updateGoldLog(-llSpentGold,eSPENT_GOLD_BY_BUY_PIGEON_POST_SHOP_ITEM);

		g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);

		if	(packet.dwPrice	>=	1000)
		{
			packet.aBuyItem[0].setBitFieldValue(60);
			sendBuyItemLog((cItem *)&packet.aBuyItem[0],packet.dwPrice,1,eBUY_ITEM_IN_PIGEON_SHOP);	//	로그 전송
		}
	}
	else
	{
		switch(iAddResult)
		{
			case	eUPR_INVENTORY_FULL				:
				iAddResult	=	eFRBI_INVENTORY_FULL;
				break;

			case	eUAIR_CAN_NOT_OWN_SAME_ITEM		:
				iAddResult	=	eFRBI_CAN_NOT_OWN_SAME_ITEM;
				break;

			case	eUAIR_TOO_MANY_BADGE_ITEM		:
				iAddResult	=	eFRBI_TOO_MANY_BADGE_ITEM;
				break;

			default									:
				iAddResult	=	eFRBI_UNKONWN;
				break;
		}

		sendFailedBuyItem(iAddResult);
	}
}

void
cACTOR::warpFieldByGateGlove(int _iTargetField , BOOL _isNetCafeUser,int _iItemSlot)
{
	if	(_iItemSlot	!=	-1)
	{
		cItem	*lpItem	=	getInventoryItem(_iItemSlot);

		if	(!lpItem)
			return;

		if	(!lpItem->isOwnSpecificEffectItem(eIE_USE_WORLD_MAP_TELEPORT))
			return;
	}
	else
	if	(!_isNetCafeUser)
	{
		if	(!isOwnGateGlove())
		{
			sendHackingLog(ePPPTR_PACKET_HACKING);

			return;
		}

		if	(m_bRemainSecondForUseGateGlove)
		{
			sendEtcWork(eEW_USE_GATE_GLOVE_REMAIN_SECOND,m_bRemainSecondForUseGateGlove);

			return;
		}
	}
	else
	{
		CClient	*lpClient	=	getClient();

 		if	(!lpClient || !lpClient->GetIsNetCafeUser())
 			return;

		if	(m_bRemainSecondForUseGateGlove)
		{
			sendEtcWork(eEW_USE_GATE_GLOVE_REMAIN_SECOND,m_bRemainSecondForUseGateGlove);
			
			return;
		}
	}

	cFIELD_INFO	*lpInfo	=	g_game.getFieldInfo(_iTargetField);

	if	(!lpInfo	||	lpInfo->isSurfaceField()	==	FALSE)
	{
		sendHackingLog(ePPPTR_PACKET_HACKING);

		return;
	}

	if	(_iItemSlot	==	-1)	//	
		m_bRemainSecondForUseGateGlove	=	60*3;

	g_pMoveFieldUserManager->booking(this,NULL,_iTargetField,0x7fff);

	if	(_iItemSlot	!=	-1)	//	아이템을 이용해 이동한거다.
	{
		cItem	*lpItem	=	getInventoryItem(_iItemSlot);

		if	(!lpItem)
			return;

		lpItem->decreaseCount(1);
		sendUpdateItemData(_iItemSlot,TRUE);
	}
}

BOOL
cACTOR::checkFreePastPortalOption()
{
	int	i;

	for (i=0;i<getItemSlotCount();i++)
	{
		if	(m_aItems[i].m_wBaseItem	==	0xffff)
			continue;

		cBasicItem* lpBasicItem	= m_aItems[i].getBasicItem();
		
		if	(lpBasicItem->m_bf1IsFreePastPortal)
			return	TRUE;
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		if	(m_aExtraInventory[i].m_wBaseItem	==	0xffff)
			continue;

		cBasicItem* lpBasicItem	= m_aExtraInventory[i].getBasicItem();
		
		if	(lpBasicItem->m_bf1IsFreePastPortal)
			return	TRUE;
	}
	
	return	FALSE;
}

BOOL
cACTOR::checkHalfPastPortalOption()
{
	int	i;

	for (i=0;i<getItemSlotCount();i++)
	{
		if	(m_aItems[i].m_wBaseItem	==	0xffff)
			continue;

		cBasicItem* lpBasicItem	= m_aItems[i].getBasicItem();
		
		if	(lpBasicItem->m_bf1IsHalfPastPortal)
			return	TRUE;
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		if	(m_aExtraInventory[i].m_wBaseItem	==	0xffff)
			continue;

		cBasicItem* lpBasicItem	= m_aExtraInventory[i].getBasicItem();
		
		if	(lpBasicItem->m_bf1IsHalfPastPortal)
			return	TRUE;
	}
	
	return	FALSE;
}

BOOL
cACTOR::checkFreeTeleportOption()
{
	int	i;

	for (i=0;i<getItemSlotCount();i++)
	{
		if	(m_aItems[i].m_wBaseItem	==	0xffff)
			continue;

		cBasicItem* lpBasicItem	= m_aItems[i].getBasicItem();
		
		if	(lpBasicItem->m_bf1IsFreeTeleport)
			return	TRUE;
	}
	
	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		if	(m_aExtraInventory[i].m_wBaseItem	==	0xffff)
			continue;

		cBasicItem* lpBasicItem	= m_aExtraInventory[i].getBasicItem();
		
		if	(lpBasicItem->m_bf1IsFreeTeleport)
			return	TRUE;
	}
	
	return	FALSE;
}

BOOL
cACTOR::checkRequestSummonCarpetOption()
{
	int	i;

	for (i=0;i<getItemSlotCount();i++)
	{
		if	(m_aItems[i].m_wBaseItem	==	0xffff)
			continue;

		cBasicItem* lpBasicItem	= m_aItems[i].getBasicItem();
		
		if	(lpBasicItem->m_bf1IsRequestSummonCarpet)
			return	TRUE;
	}
	
	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		if	(m_aExtraInventory[i].m_wBaseItem	==	0xffff)
			continue;

		cBasicItem* lpBasicItem	= m_aExtraInventory[i].getBasicItem();
		
		if	(lpBasicItem->m_bf1IsRequestSummonCarpet)
			return	TRUE;
	}
	
	return	FALSE;
}

BOOL
cACTOR::isOwnGateGlove()
{
	int	i;

	for (i=0;i<getItemSlotCount();i++)
	{
		if	(m_aItems[i].m_wBaseItem	==	0xffff)
			continue;

		if	(m_aItems[i].isGateGlove())
			return	TRUE;
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		if	(m_aExtraInventory[i].m_wBaseItem	==	0xffff)
			continue;

		if	(m_aExtraInventory[i].isGateGlove())
			return	TRUE;
	}

	return	FALSE;
}

void
cACTOR::useCoupleRing(int _iItemIndex)
{
	if	(getRestraintTimeByBattle())
	{
		sendEtcWork(eEW_CAN_NOT_TELEPORT_DURING_BATTLE,m_wRestraintTimeByBattle);

		return;
	}

	cItem	*lpItem	=	getInventoryItem(_iItemIndex);

	if	(!lpItem)
		return;

	cCoupleRingDefine	*lpCoupleRing	=	lpItem->getCoupleRing();

	if	(!lpCoupleRing)
		return;

	if	(m_lpField->isBlockedToTeleportField()	||	m_lpField->m_wIsBlockToWarpField)
	{
		sendRegistServerMessage(eRSM_CAN_NOT_USE_THAT_ITEM_IN_THIS_FIELD);
		return;
	}

	cACTOR	*lpTarget	=	g_game.getPlayer(lpCoupleRing->getCoupleName());

	if	(!lpTarget)
	{
		g_game.sendWPFindUser(m_strName,lpCoupleRing->getCoupleName(),m_wCurrentField,eFUW_USE_COUPLE_RING);

		return;
	}

	if	(lpTarget->getCoupleRingIndex(m_strName)	==	0xffff)
	{
		sendFindUserResult(lpTarget->m_strName,eFUR_TARGET_USER_NOT_OWNED_COUPLE_RING,0xffff,0xffff);
		return;
	}

	if	(lpTarget->m_lpField->isBlockedToTeleportField() || lpTarget->m_lpField->m_wIsBlockToWarpField)
	{
		sendRegistServerMessage(eRSM_TARGET_IS_AT_BLOCKED_FIELD_TO_USE_THAT_ITEM);
		return;
	}

	if	(lpTarget->m_lpField->m_wIsPremiumZone	&&	m_wPremiumServiceLevel	==	ePSL_NORMAL	&&	m_bf1IsExistExceptExpPortal	==	FALSE)
	{
		sendEtcWork(eEW_TARGET_IS_IN_PREMIUM_ZONE);
		return;
	}

	if	(lpTarget->m_wCurrentField	!= m_wCurrentField)
	{
		m_bf1IsMoveFieldByTeleport	=	TRUE;
		m_bf1IsTeleportToCouple		=	TRUE;
		m_bf7UseCoupleRingSlot		=	_iItemIndex;

		g_pMoveFieldUserManager->booking(this,NULL,lpTarget->m_wCurrentField,0x7fff,lpTarget->m_pos.x,lpTarget->m_pos.y);

		return;
	}

	stop(lpTarget->m_pos.x,lpTarget->m_pos.y,FALSE);
	sendCurrentPosition();
	addPacketStop();
}

BOOL
cACTOR::useFireWork(int _iKind,int _iTime)
{
	int	iSelectFireWork		=	_iKind;
	int	iSelectFireTime		=	_iTime;

	if	(_iKind	==	0)
	{
		int	iValue						=	random(100)+1;
		int	aiSelectFireWorkValue[10]	=	{18,18,14,14,10,10,5,5,3,3};
		int	aiSelectFireWorkTime[10]	=	{15,15,16,16,17,17,18,18,19,20};
		int	iSelectFireWorkValueSum		=	0;

		int	i=0;

		for(i=0;i<10;i++)
		{
			iSelectFireWorkValueSum		+=	aiSelectFireWorkValue[i];

			if	(iValue	<	iSelectFireWorkValueSum)
			{
				iSelectFireWork			=	i+1;
				iSelectFireTime			=	aiSelectFireWorkTime[i];
				break;
			}
		}
	}

	if	(g_fireWorkManager.getByActor(m_iZoneSerial))
	{
		sendEtcWork(eEW_CAN_NOT_FIRE_CRACKER_BY_ALREADY_USE_FIRE_CRACKER);
		return	FALSE;
	}
	
	if	(g_fireWorkManager.add(this,iSelectFireWork,iSelectFireTime)	==	0xffff)
		return	FALSE;

	return	TRUE;
}

void
cACTOR::changeISPISetting(int _iItemIndex,int _iPowerType)
{
	cItem	*lpItem	=	getInventoryItem(_iItemIndex);

	if	(!lpItem	||	!lpItem->isIncreaseSelectPowerItem())
		return;

	cBasicItem*	lpBasic	=	lpItem->getBasicItem();

	if(!lpBasic)
		return;

	if	(_iPowerType<0	||	_iPowerType>=7)
		return;
	for	(int i=0;i<dGENERATE_ITEM_DATA_COUNT;++i)
	{
		if(	lpBasic->m_aGenerateData[i].m_wEffect == eIE_INCREASE_FIX_SELECT_POWER)
		{
				if(lpItem->m_aOption[1] != 0)
					return;
				lpItem->m_aOption[1]	=	_iPowerType + 1;
		}
		else if(lpBasic->m_aGenerateData[i].m_wEffect == eIE_INCREASE_SELECT_POWER)
		{
			lpItem->m_aOption[1]	=	_iPowerType;
		}
	}
	
	sendEtcWork(eEW_CHANGE_ISPI_SETTING,_iItemIndex,_iPowerType);
}


void
cACTOR::selectItemInPack(int _iItemIndex,int _iItemSlot,int _iPrefix)
{
	cItem		*lpItem		=	getInventoryItem(_iItemIndex);

	if	(!lpItem)
		return;

	cBasicItem	*lpBasicItem=	lpItem->getBasicItem();

	if	(!lpBasicItem	||	lpBasicItem->m_aGenerateData[0].m_wEffect	!=	eIE_SELECT_ITEM_IN_ITEM_PACK)
		return;

	int	iPackIndex	=	lpItem->m_aOption[0];

	cItemPackInfo	*lpPackInfo	=	&g_itemPackList.m_aPacks[iPackIndex];

	if	(lpPackInfo->m_wSerial	==	0xffff	||	lpPackInfo->m_wCount	==	0)
		return;

	if	(_iItemSlot	<	0	||	_iItemSlot	>=	lpPackInfo->m_wCount)
		return;

	cItem	itemResult;

	if	(!g_itemPackList.getItem(&itemResult,lpItem->m_aOption[0],_iItemSlot,_iPrefix))
		return;

	lpItem->copy(&itemResult);
	sendCreateItemLog(&itemResult,eCIM_SELECT_ITEM_IN_PACK);

	SG_LOTTERY_TICKET_PRIZE_WINNING		packetPrize;

	packetPrize.base.set(sizeof(packetPrize),dSG_LOTTERY_TICKET_PRIZE_WINNING);
	packetPrize.wItemSlot			=	_iItemIndex;
	packetPrize.bf1IsBySelect		=	TRUE;

	memcpy(&packetPrize.itemPrize,&itemResult,sizeof(cItem));

	g_userPM.add(m_iClientSerial,&packetPrize,packetPrize.base.wSize);

}

void
cACTOR::askItemListInItemPackForSelect(int _iItemIndex)
{
	cItem		*lpItem		=	getInventoryItem(_iItemIndex);

	if	(!lpItem)
		return;

	cBasicItem	*lpBasicItem=	lpItem->getBasicItem();

	if	(!lpBasicItem	||	lpBasicItem->m_aGenerateData[0].m_wEffect	!=	eIE_SELECT_ITEM_IN_ITEM_PACK)
		return;

	int	iPackIndex	=	lpItem->m_aOption[0];

	if	(iPackIndex	>=	dMAX_ITEM_PACK_COUNT)
		return;

	cItemPackInfo	*lpPackInfo	=	&g_itemPackList.m_aPacks[iPackIndex];

	if	(!lpPackInfo || lpPackInfo->m_wSerial	==	0xffff	||	lpPackInfo->m_wCount	==	0)
		return;

	SG_ITEM_PACK_DATA	packet;

	packet.base.set(sizeof(packet),dSG_ITEM_PACK_DATA);
	packet.wItem	=	_iItemIndex;

	memcpy(&packet.packInfo,lpPackInfo,sizeof(cItemPackInfo));

	packet.base.wSize	=	sizeof(packet)-sizeof(packet.packInfo.m_aItems)+sizeof(cItemInPack)*lpPackInfo->m_wCount;

	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

BOOL
cACTOR::addItemWithOption(WORD _wItemIndex, int _iCount, int* _lpOption,int _iMethod)
{
	if	(!getValidInventorySlot())
		return FALSE;
	
	cItem	addItem;
	cItem	orgItem;

	if	(!g_im.generateItemByBaseItem(&addItem,_wItemIndex,_iCount,-1,_lpOption))
		return FALSE;

	orgItem.copy(&addItem);

	if	(addItem.isMoneyItem())
		addGoldItemToInventory(&addItem,_iMethod);
	else
		addItemToInventory(&addItem,FALSE);

	sendAddItem(&orgItem);
	buildPower();

	return TRUE;
}

BOOL
cACTOR::isExistItemEnteringGuildDungeon()			//  길드던전에 입장할 수 있는 아이템 존재 여부
{ 

	for(int i=0;i<dEQUIPMENT_PART_COUNT;++i)
	{
		cItem	*lpItem	=	&m_aEquip[i];
		
		cBasicItem*	lpBasic = lpItem->getBasicItem(TRUE);
		if(lpItem->m_wBaseItem == 0xffff)
			continue;
		
		if(lpBasic->m_bf1IsIgnoreGuildHallLevelForEntryTantalisExile)
			return	TRUE;
		
	}
	
	for (int j=0;j<dOWN_ITEM_COUNT;++j)
	{
		cItem	*lpItem	=	&m_aItems[j];
		cBasicItem*	lpBasic = lpItem->getBasicItem(TRUE);
		if(lpItem->m_wBaseItem == 0xffff)
			continue;

		if(lpBasic->m_bf1IsIgnoreGuildHallLevelForEntryTantalisExile)
			return	TRUE;	
		
	}
	
	for(int k=0;k<m_bf6ExtraInventorySize;++k)
	{
		cItem	*lpItem	=	&m_aExtraInventory[k];
		cBasicItem*	lpBasic = lpItem->getBasicItem(TRUE);
		if(lpItem->m_wBaseItem == 0xffff)
			continue;

		if(lpBasic->m_bf1IsIgnoreGuildHallLevelForEntryTantalisExile)
			return	TRUE;	
	}

	return FALSE;
}

void
cACTOR::useBottomlessBox(int _iBottomlessBox,int _iItem1,int _iItem2)
{
	cItem	*lpItem1		=	getInventoryItem(_iItem1);
	cItem	*lpItem2		=	getInventoryItem(_iItem2);
	cItem	*lpBottomlessBox=	getInventoryItem(_iBottomlessBox);
	cItem	*lpMixItem		=	getValidInventorySlot();
	int		iIndex			=	getValidInventorySlotIndex();

	BOOL	bIsRed			=	FALSE;

	if	(g_iWorldServerType	==	eSERVER_TYPE_GVG	||	!lpItem1	||	!lpItem2	||	!lpBottomlessBox || !lpMixItem || iIndex == 0xffff)
	{
		sendEtcWork(eEW_USE_BOTTOMLESS_BOX_FAILED);
		return;
	}

	cBasicItem	*lpBasicItem1	=	lpItem1->getBasicItem();
	cBasicItem	*lpBasicItem2	=	lpItem2->getBasicItem();

	if	(!lpBasicItem1	||	!lpBasicItem2	||	lpBasicItem1->isEternalItem() || lpBasicItem2->isEternalItem())
	{
		sendEtcWork(eEW_USE_BOTTOMLESS_BOX_FAILED);
		return;
	}

	if(lpItem1->getCarvingLevel() ||lpItem2->getCarvingLevel() || lpBasicItem1->m_bf1IsRingOfInfinity || lpBasicItem2->m_bf1IsRingOfInfinity )
	{
		sendEtcWork(eEW_USE_BOTTOMLESS_BOX_FAILED);
		return;
	}

	if	(lpItem1->isExceptionItem() || lpItem2->isExceptionItem()  )
	{
		sendEtcWork(eEW_USE_BOTTOMLESS_BOX_FAILED);
		return;
	}

	if	(_iItem1	==	_iItem2)
	{
		sendEtcWork(eEW_USE_BOTTOMLESS_BOX_FAILED);
		return;
	}

	if	(lpBottomlessBox->isBottomlessBox()	==	FALSE	&&	lpBottomlessBox->checkFirstEffect(eIE_CH5_MIX_ITEM_BOX)	==	FALSE)
	{
		sendEtcWork(eEW_USE_BOTTOMLESS_BOX_FAILED);
		return;
	}

	bIsRed		=	lpBottomlessBox->checkFirstEffect(eIE_CH5_MIX_ITEM_BOX);
	
	if	(lpItem1->getItemType()	!=	lpItem2->getItemType())
	{
		sendEtcWork(eEW_USE_BOTTOMLESS_BOX_FAILED);
		return;
	}

	if(c_aItemEquipPlace[lpItem1->getItemType()] == dITEM_KIND_WASTING || c_aItemEquipPlace[lpItem1->getItemType()] == 0xffff)		//  변경..
	{
		sendEtcWork(eEW_USE_BOTTOMLESS_BOX_FAILED);
		return;
	}

	if	(lpItem1->getPrefixCount() + lpItem2->getPrefixCount() == 0)
	{
		sendEtcWork(eEW_USE_BOTTOMLESS_BOX_FAILED);
		return;
	}

	if	(lpItem1->isCanNotTradeItem() || lpItem2->isCanNotTradeItem())
	{
		sendEtcWork(eEW_USE_BOTTOMLESS_BOX_FAILED);
		return;
	}

	if	(lpItem1->getSetItem() || lpItem2->getSetItem())
	{
		sendEtcWork(eEW_USE_BOTTOMLESS_BOX_FAILED);
		return;
	}

	if	(lpItem1->isExtraItem() || lpItem2->isExtraItem())
	{
		sendEtcWork(eEW_USE_BOTTOMLESS_BOX_FAILED);
		return;
	}
	
	cItem	item1,item2;
	int		aiPrefix[3],i;
	cItem	itemMix;

//	같은 종류의 접두사가 중복되지 않게 정렬
	{
		if	(lpItem1->getPrefixCount() < lpItem2->getPrefixCount())	//	접두사 적은 아이템을 기준으로 한다.
		{
			item1.copy(lpItem1);
			item2.copy(lpItem2);
		}
		else
		{
			item1.copy(lpItem2);
			item2.copy(lpItem1);
		}

		for	(i=0;i<dITEM_PREFIX_COUNT;i++)
		{
			int	iPrefix1	=	item1.m_aPrefix[i].m_wPrefix;

			if	(iPrefix1	==	0xffff)
				break;

			int	iPrefixType1	=	g_aItemPrefix[iPrefix1].m_wType;

			for	(int j=0;j<dITEM_PREFIX_COUNT;j++)
			{
				int	iPrefix2	=	item2.m_aPrefix[j].m_wPrefix;

				if	(iPrefix2	==	0xffff)
					continue;

				int	iPrefixType2	=	g_aItemPrefix[iPrefix2].m_wType;

				if	(iPrefixType1	==	iPrefixType2 && i!=j)	//	접두사 타입이 같고 위치가 다르다.
				{
					CItemPrefixInfo	info;

					memcpy(&info				,&item2.m_aPrefix[i],sizeof(CItemPrefixInfo));
					memcpy(&item2.m_aPrefix[i]	,&item2.m_aPrefix[j],sizeof(CItemPrefixInfo));	//	위치를 같게 만들어 준다.
					memcpy(&item2.m_aPrefix[j]	,&info				,sizeof(CItemPrefixInfo));
				}
			}
		}
	}

	if	(random(2)	==	0)
		itemMix.copy(&item1);
	else
		itemMix.copy(&item2);

//	접두사의 개수는 (총 접두사의 개수/2)로 줄어들며 선택 접두사는 랜덤 선택. 단, 금칙 조항 검사는 해야함. 아이템의 종류도 랜덤 선택
	if	(bIsRed)
	{
		int	iDropLevel1	=	item1.getBasicItem()->m_wDropLevel;
		int	iDropLevel2	=	item2.getBasicItem()->m_wDropLevel;

		if	(random(100)	<	75)
		{
			if	(iDropLevel1	<	iDropLevel2)
				itemMix.copy(&item1);
			else
				itemMix.copy(&item2);
		}
		else
		{
			if	(iDropLevel1	>	iDropLevel2)
				itemMix.copy(&item1);
			else
				itemMix.copy(&item2);
		}
	}

	memset(aiPrefix,0xff,sizeof(aiPrefix));

	for	(i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		itemMix.m_aPrefix[i].m_wPrefix	=	0xffff;

		if	(item1.m_aPrefix[i].m_wPrefix	==	0xffff	&&	item2.m_aPrefix[i].m_wPrefix	==	0xffff	)
			continue;

		aiPrefix[i]	=	random(2);

		if	(bIsRed)
		{
			if	(item1.m_aPrefix[i].m_wPrefix	==	0xffff)
			{
				if	(random(100)	<	75)
					aiPrefix[i]	=	0;
				else
					aiPrefix[i]	=	1;
			}
			if	(item2.m_aPrefix[i].m_wPrefix	==	0xffff)
			{
				if	(random(100)	<	75)
					aiPrefix[i]	=	1;
				else
					aiPrefix[i]	=	0;
			}
		}
	}

	int	iPrefixCount	=	0;

	for	(i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if	(aiPrefix[i]	==	0xffffffff)
			continue;

		CItemPrefixInfo	*lpPrefixInfo	=	NULL;

		if	(aiPrefix[i]	==	0)
			lpPrefixInfo	=	&item1.m_aPrefix[i];
		else
			lpPrefixInfo	=	&item2.m_aPrefix[i];

		if	(lpPrefixInfo	&&	lpPrefixInfo->m_wPrefix!=	0xffff)
		{
			memcpy(&itemMix.m_aPrefix[iPrefixCount],lpPrefixInfo,sizeof(CItemPrefixInfo));

			iPrefixCount++;
		}
	}
	
	if	(itemMix.isInfinityBullet())
		itemMix.m_bCount	=	1;

	sendRemoveItemLog(lpItem1,eRI_BY_USE_BOTTOMLESS_BOX);
	sendRemoveItemLog(lpItem2,eRI_BY_USE_BOTTOMLESS_BOX);
	sendCreateItemLog(&itemMix,eCIM_USE_BOTTOMLESS_BOX);

	lpItem1->reset();
	lpItem2->reset();
	lpBottomlessBox->decreaseCount(1);

	lpMixItem->copy(&itemMix);

	sendEtcWork(eEW_USE_BOTTOMLESS_BOX_SUCCESS,_iBottomlessBox,_iItem1,_iItem2);
	sendUpdateItemData(iIndex,TRUE);
}

BOOL
cACTOR::isInterruptingArmor(cItem *_lpItem)
{
	const	int	c_iInterruptingPartLevelCount	=	8;

	int		iLevel	=	m_wInterruptingArmorLevel-1;

	if	(m_wInterruptingArmorLevel	==	0	||	iLevel	>=	c_iInterruptingPartLevelCount)
		return	FALSE;

	int		iItemKind=	_lpItem->getItemType();
	int		iPart	=	c_aItemEquipPlace[iItemKind];

	if	(iPart	>=	dEQUIPMENT_PART_COUNT	||	iItemKind	==	eIK_BULLET)
		return	FALSE;

	int	aiInterruptingPart[c_iInterruptingPartLevelCount][20]	=	
	{
		{dEQUIP_EARING,-1},
		{dEQUIP_EARING,dEQUIP_NECKLACE,-1},
		{dEQUIP_EARING,dEQUIP_NECKLACE,dEQUIP_SHOES,-1},
		{dEQUIP_EARING,dEQUIP_NECKLACE,dEQUIP_SHOES,dEQUIP_HELM,-1},
		{dEQUIP_EARING,dEQUIP_NECKLACE,dEQUIP_SHOES,dEQUIP_HELM,dEQUIP_GLOVE,-1},
		{dEQUIP_EARING,dEQUIP_NECKLACE,dEQUIP_SHOES,dEQUIP_HELM,dEQUIP_GLOVE,dEQUIP_ARMOR,-1},
		{dEQUIP_EARING,dEQUIP_NECKLACE,dEQUIP_SHOES,dEQUIP_HELM,dEQUIP_GLOVE,dEQUIP_ARMOR,dEQUIP_RING,-1},
		{dEQUIP_EARING,dEQUIP_NECKLACE,dEQUIP_SHOES,dEQUIP_HELM,dEQUIP_GLOVE,dEQUIP_ARMOR,dEQUIP_RING,dEQUIP_SHIELD_BULLET	,-1},
	};

	int	iIndex	=	0;

	while(aiInterruptingPart[iLevel][iIndex]	!=	-1)
	{
		if	(aiInterruptingPart[iLevel][iIndex]	==	iPart)
			return	TRUE;

		if	(aiInterruptingPart[iLevel][iIndex]	==	dEQUIP_RING)
		{
			if	(iPart	>=	dEQUIP_RING1	&&	iPart	<=	dEQUIP_RING8)
				return	TRUE;
		}

		iIndex++;
	}

	return	FALSE;
}
BOOL
cACTOR::isInterruptingWeapon(cItem *_lpItem)
{

	if	(!m_wInterruptingWeaponTime)
		return	FALSE;

	int		iItemKind=	_lpItem->getItemType();
	int		iPart	=	c_aItemEquipPlace[iItemKind];

	if	(c_aItemEquipPlace[iItemKind] == dEQUIP_WEAPON)
		return	TRUE;

	return	FALSE;
}
//	기본 아이템이 _iBasicItem인 아이템을 찾아서 리턴한다.
cItem*
cACTOR::getItemByBasicItem(int _iBasicItem,int _iOrder)
{
	int	i,iCount=0;

	for (i=0;i<dOWN_ITEM_COUNT;i++)
		if	(m_aItems[i].m_wBaseItem	==	_iBasicItem)	
		{
			iCount++;

			if	(iCount	==	_iOrder)
				return	&m_aItems[i];
		}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
		if (m_aExtraInventory[i].m_wBaseItem	==	_iBasicItem)	
		{
			iCount++;

			if	(iCount	==	_iOrder)
				return	&m_aExtraInventory[i];
		}

	return	NULL;
}

//	기본 아이템이 _iBasicItem인 아이템을 찾아서 리턴한다.
int
cACTOR::getItemSlotByBasicItem(int _iBasicItem)
{
	int	i;

	for (i=0;i<dOWN_ITEM_COUNT;i++)
		if (m_aItems[i].m_wBaseItem	==	_iBasicItem)	
			return	i;

	for (i=0;i<m_bf6ExtraInventorySize;i++)
		if (m_aExtraInventory[i].m_wBaseItem	==	_iBasicItem)	
			return	i+dOWN_ITEM_COUNT;

	return	0xffff;
}

cItem*
cACTOR::getEmptyItemSlot(int* _iSlot)	//	비어 있는 아이템 슬롯을 찾아서 리턴한다.
{
	int	i;
	
	for (i=0;i<getItemSlotCount();i++)
		if (m_aItems[i].m_wBaseItem	==	0xffff)	
		{
			if(_iSlot)
				*_iSlot = i;

			return	&m_aItems[i];
		}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
		if	(m_aExtraInventory[i].m_wBaseItem	==	0xffff)	
		{
			if	(_iSlot)
				*_iSlot = i+dOWN_ITEM_COUNT;

			return	&m_aExtraInventory[i];
		}

	return	NULL;
}

int
cACTOR::getRemainItemSlotCount()
{
	int	iCount	=	0;

	for (int i=0;i<getItemSlotCount();i++)
		if	(m_aItems[i].m_wBaseItem	==	0xffff)	
			iCount++;

	for (i=0;i<m_bf6ExtraInventorySize;i++)
		if	(m_aExtraInventory[i].m_wBaseItem	==	0xffff)	
			iCount++;

	return	iCount;
}

void
cACTOR::computeItemCount()//아이템의 수 계산.
{
	int	i;

	m_wItemCount	=	0;

	for (i=0;i<m_wItemSlotCount;i++)
		if (m_aItems[i].m_wBaseItem	!=	0xffff)	
			m_wItemCount++;

	for (i=0;i<m_bf6ExtraInventorySize;i++)
		if (m_aExtraInventory[i].m_wBaseItem	!=	0xffff)	
			m_wItemCount++;
}

void
cACTOR::resetAllItem()
{
	int	i;

	for (i=0;i<dOWN_ITEM_COUNT;i++)
		m_aItems[i].reset();

	for (i=0;i<m_bf6ExtraInventorySize;i++)
		m_aExtraInventory[i].reset();
	
	for (i=0;dEQUIPMENT_PART_COUNT;i++)
		m_aEquip[i].reset();

}

void
cACTOR::releaseItemReversion(int _iUseItem,int _iTargetItem)
{
	cItem	*lpUseItem		=	getInventoryItem(_iUseItem);
	cItem	*lpTargetItem	=	getInventoryItem(_iTargetItem);

	if	(!lpUseItem	||	!lpTargetItem	|| lpTargetItem->getSetItem())
		return;

	if	(lpUseItem->checkFirstEffect(eIE_CH5_RELASE_REVERSION)	==	FALSE)
		return;

	if	(lpTargetItem->getSpecialItem()	==	NULL)
		return;

	if	(lpTargetItem->getSpecialItem()->m_bf1IsReversionItem	==	FALSE)
		return;
	if	(lpTargetItem->getSpecialItem()->m_bf1IsFreeUseItem	)	//	새벽의 봉인구 사용 아이템
		return;

	cBasicItem*	lpBasicItem	=	lpTargetItem->getBasicItem();

	if(lpBasicItem && lpBasicItem->m_bf1IsNotApplyReversionEffect)
		return;

	lpTargetItem->getSpecialItem()->m_bf1IsReversionItem	=	FALSE;

	lpUseItem->reset();

	sendEnchantItemLog(lpTargetItem,eEIW_RELEASE_REVERSION);

	sendEtcWork(eEW_CH5_RELEASE_ITEM_REVERSION,_iUseItem,_iTargetItem);
}

void
cACTOR::levelUpPrefix(int _iUseItem,int _iTargetItem,int _iSlot,BOOL _bIsUnique)
{
	cItem	*lpUseItem		=	getInventoryItem(_iUseItem);
	cItem	*lpTargetItem	=	getInventoryItem(_iTargetItem);

	if	(!lpUseItem	||	!lpTargetItem	||	_iSlot	<	0	||	_iSlot	>=	dITEM_PREFIX_COUNT || lpTargetItem->getSetItem())
		return;

	BOOL	bIsDX	=	lpTargetItem->isDXItem();

	if	(_bIsUnique)
	{
		if	(lpTargetItem->isUniqueItem()	==	FALSE)
			return;
	}
	else
	{
		if	(lpTargetItem->isUniqueItem())
			return;
	}

	if	(lpTargetItem->m_aPrefix[_iSlot].m_wPrefix	==	0xffff)
		return;

	int	iCheckEffect;

	if	(_bIsUnique	==	FALSE)
		iCheckEffect	=	eIE_CH5_UPGRADE_NORMAL_ITEM_PREFIX_POWER;
	else
		iCheckEffect	=	eIE_CH5_UPGRADE_UNIQUE_ITEM_PREFIX_POWER;
	
	if	(lpUseItem->checkFirstEffect(iCheckEffect)	==	FALSE)
		return;

	int	iOldPrefx	=	lpTargetItem->m_aPrefix[_iSlot].m_wPrefix;

	if	(!g_itemPrefix.levelUpPrefix(&lpTargetItem->m_aPrefix[_iSlot],lpTargetItem->getItemType()))
		return;

	int	iNewPrefx	=	lpTargetItem->m_aPrefix[_iSlot].m_wPrefix;

	if	(bIsDX	==	FALSE	&&	lpTargetItem->isDXItem())
		lpTargetItem->m_bDurability	=	0;

	sendEtcWork(eEW_CH5_ITEM_PREFIX_LEVEL_UP,_iUseItem,_iTargetItem,iOldPrefx,iNewPrefx);

	sendUpdateItemData(_iTargetItem,TRUE);

	lpUseItem->decreaseCount(1);
}



//장소 기억이 가능한지를 체크 한다.
//인벤토리 아이템중에 기억이 가능한 아이템이 있는 지를 걍 검색.
BOOL
cACTOR::isAbleRemeberPlace(int _iSlot)
{
	if(!isPlayer())
		return FALSE;
	
	for (int i=0;i<dOWN_ITEM_COUNT;i++)
	{
		cItem	*lpItem						=	getInventoryItem(i);
		
		if(lpItem == NULL)
			continue;
		
		cBasicItem	*lpBasicItem			=	lpItem->getBasicItem();
		
		if(lpBasicItem == NULL)
			continue;
		
		if(lpBasicItem->m_bf1RemeberPlace0 && _iSlot == 0)
			return TRUE;

		if(lpBasicItem->m_bf1RemeberPlace1 && _iSlot == 1)
			return TRUE;

		if(lpBasicItem->m_bf1RemeberPlace2 && _iSlot == 8)
			return TRUE;
	}
	
	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		cItem	*lpItem	=	&m_aExtraInventory[i];
		
		if(lpItem == NULL)
			continue;
		
		cBasicItem	*lpBasicItem			=	lpItem->getBasicItem();
		
		if(lpBasicItem == NULL)
			continue;
		
		if(lpBasicItem->m_bf1RemeberPlace0 && _iSlot == 0)
			return TRUE;
		
		if(lpBasicItem->m_bf1RemeberPlace1 && _iSlot == 1)
			return TRUE;
		
		if(lpBasicItem->m_bf1RemeberPlace2 && _iSlot == 8)
			return TRUE;
	}
	
	return FALSE;
}
int
cACTOR::getStackItemCount(int _iSerial, BOOL _bEQuip)	// 스택아이템 갯수(슬롯갯수가 아니다)를 리턴한다.		//09.08.21
{


	int iCount = 0;

	if(_bEQuip == TRUE)
	{
		for(int i=0;i<dEQUIPMENT_PART_COUNT;++i)
		{
			cItem	*lpItem	=	&m_aEquip[i];
			
			
			if(lpItem->m_wBaseItem == 0xffff)
				continue;
			if(_iSerial != lpItem->m_wBaseItem)
				continue;
			
			iCount += lpItem->m_bCount;
			
		}
	}
	for (int j=0;j<dOWN_ITEM_COUNT;++j)
	{
		cItem	*lpItem	=	&m_aItems[j];
		if(lpItem->m_wBaseItem == 0xffff)
			continue;
		if(_iSerial != lpItem->m_wBaseItem)
			continue;
		
		iCount	+=	lpItem->m_bCount;
	}
	
	for(int k=0;k<m_bf6ExtraInventorySize;++k)
	{
		cItem	*lpItem	=	&m_aExtraInventory[k];
		if(lpItem->m_wBaseItem == 0xffff)
			continue;
		if(_iSerial != lpItem->m_wBaseItem)
			continue;
		
		iCount	+=	lpItem->m_bCount;
		
		
	}
	
	return iCount;

}

//포탈 스피어관련 정보만 조사하여 최고 서비스 레벨을 얻는다. 즉 게이트 글로브는 포함안된다.
//게이트 글로브로 인해 포탈스피어 능력이 발동 안할때만 사용하자.
int
cACTOR::getMaxPremiumItemLevel()
{
	if(!isPlayer())
		return 0;
	
	int iMaxPrimiumItemLevel = 0;
	
	for (int i=0;i<dOWN_ITEM_COUNT;i++)
	{
		cItem	*lpItem						=	getInventoryItem(i);
		
		if(lpItem == NULL)
			continue;
		
		cBasicItem	*lpBasicItem			=	lpItem->getBasicItem();
		
		if(lpBasicItem == NULL)
			continue;
		
		if(iMaxPrimiumItemLevel < lpBasicItem->m_wPremiumLevel)
			iMaxPrimiumItemLevel = lpBasicItem->m_wPremiumLevel;
	}
	
	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		cItem	*lpItem	=	&m_aExtraInventory[i];
		
		if(lpItem == NULL)
			continue;
		
		cBasicItem	*lpBasicItem			=	lpItem->getBasicItem();
		
		if(lpBasicItem == NULL)
			continue;
		
		if(iMaxPrimiumItemLevel < lpBasicItem->m_wPremiumLevel)
			iMaxPrimiumItemLevel = lpBasicItem->m_wPremiumLevel;
	}
	
	return iMaxPrimiumItemLevel;
}

//
//아이템을 분해

void
cACTOR::disJointingItem(int _iTargetItem)
{ 
	cItem	*lpTargetItem	=	getInventoryItem(_iTargetItem);
	
	if	(!lpTargetItem)
	{
		sendEtcWork(eEW_DISJOINTING_ITEM_RESULT_FLASE);
		return;
	}
	

	cBasicItem* lpBasic = lpTargetItem->getBasicItem();
	
	if(!lpBasic)
		return;
	cItem	logItem;
	logItem.copy(lpTargetItem);

	if	(lpTargetItem->isUniqueItem()	==	FALSE)
	{
		sendEtcWork(eEW_DISJOINTING_ITEM_RESULT_FLASE);
		return;
	}

	if	(lpTargetItem->isExpirationData())
	{
		sendEtcWork(eEW_DISJOINTING_ITEM_RESULT_FLASE);
		return;
	}
	if(lpBasic->m_wExpireYear != 0)
	{
		sendEtcWork(eEW_DISJOINTING_ITEM_RESULT_FLASE);
		return;
	}
		

	int iItemLevel = lpTargetItem->getRequireLevel() - 1;

	lpTargetItem->decreaseCount(1);

	int iMesteryPiece		= 0;
	int iCrystal			= 0;
	int iTantillessPiece	= 0;
	iTantillessPiece	=	l_DisjointingItemTable[iItemLevel/100][0];
#ifdef	_FOR_JAPAN	
	iTantillessPiece	=	l_DisjointingItemTableForJapan[iItemLevel/100][0];
#endif

	if	(iTantillessPiece)
	{
		cItem addItem;
		g_im.generateItemByBaseItem(&addItem,dITEM_INDEX_TANTILLESS_PIECE ,iTantillessPiece);
		addItem.m_dwSerial		=	largeRandom(0x7ffffff0)+10;
		sendCreateItemLog(&addItem,eCIM_ITEM_PACK);

		if	(addItem.isMoneyItem())
			CLOG("goldIOError","disJointingItem");

		addItemToInventory(&addItem);	//	탄틸이니 골드 아니다
	}

	int iRand = rand() % 100;
	if( iRand < dRAND_VALUE_FOR_DISJOINTING_ITEM_FOR_RANDOM_TOKKEN)
	{
		
		iMesteryPiece = l_DisjointingItemTable[iItemLevel/100][0];
#ifdef	_FOR_JAPAN	
		iMesteryPiece	=	l_DisjointingItemTableForJapan[iItemLevel/100][0];
#endif
		if(iMesteryPiece)
		{
			cItem addItem;
			g_im.generateItemByBaseItem(&addItem,dITEM_INDEX_STONEOFMYSTERY_PIECE ,iMesteryPiece);	
			addItem.m_dwSerial		=	largeRandom(0x7ffffff0)+10;			
			sendCreateItemLog(&addItem,eCIM_ITEM_PACK);

			if	(addItem.isMoneyItem())
				CLOG("goldIOError","disJointingItem");

			addItemToInventory(&addItem);	//	골드 아님
		}
	}
	iRand = rand() % 100 ;
	if(iRand < dRAND_VALUE_FOR_DISJOINTING_ITEM_FOR_RANDOM_TOKKEN)
	{	
		
		iCrystal = l_DisjointingItemTable[iItemLevel/100][1];
#ifdef	_FOR_JAPAN	
		iCrystal	=	l_DisjointingItemTableForJapan[iItemLevel/100][1];
#endif
		if(iCrystal)
		{
			cItem addItem;
			g_im.generateItemByBaseItem(&addItem,dITEM_INDEX_CRYSTAL ,iCrystal);	
			addItem.m_dwSerial		=	largeRandom(0x7ffffff0)+10;				
			sendCreateItemLog(&addItem,eCIM_ITEM_PACK);

			if	(addItem.isMoneyItem())
				CLOG("goldIOError","disJointingItem");

			addItemToInventory(&addItem);		
		}
	}
	
	//분해 로그
	sendRemoveItemLog(&logItem,eRI_BY_DISJOINTING_ITEM,m_wCurrentField);
	
	sendEtcWork(eEW_DISJOINTING_RESULT, _iTargetItem, iTantillessPiece, iMesteryPiece, iCrystal);			// 분해 결과 보내기 !! 09.09.07

}
void
cACTOR::upgradeItem(int _iUseItem,int _iTargetItem,int _iSlot,BOOL _bIsUnique)
{
	cItem	*lpUseItem		=	getInventoryItem(_iUseItem);
	cItem	*lpTargetItem	=	getInventoryItem(_iTargetItem);

	if	(!lpUseItem	||	!lpTargetItem || lpTargetItem->getSetItem())
		return;

	if	(_bIsUnique)
	{
		if	(lpTargetItem->isUniqueItem()	==	FALSE)
			return;
	}
	else
	{
		if	(lpTargetItem->isUniqueItem())
			return;
	}

	int	iCheckEffect;

	if	(_bIsUnique	==	FALSE)
		iCheckEffect	=	eIE_CH5_IMPROVE_NORMAL_ITEM_POWER;
	else
		iCheckEffect	=	eIE_CH5_IMPROVE_UNIQUE_ITEM_POWER;
	
	if	(lpUseItem->checkFirstEffect(iCheckEffect)	==	FALSE)
		return;

	if	(!lpTargetItem->upgrade(_iSlot))
		return;

	sendEtcWork(eEW_CH5_UPGRADE_ITEM,_iUseItem,_iTargetItem,_iSlot,lpTargetItem->m_aPrefix[_iSlot].m_aValue[0],lpTargetItem->m_aPrefix[_iSlot].m_aValue[1]);

	lpUseItem->decreaseCount(1);
}

BOOL
cACTOR::checkItemCoolTime(cBasicItem *_lpBasicItem)
{
	if	(_lpBasicItem->m_wCoolTimeSecond	==	0)
		return	TRUE;

	if	(m_awItemCoolTime[_lpBasicItem->m_bCoolTimeIndex])
		return	FALSE;

	return	TRUE;
}
int
cACTOR::getUpgradeMaterialsGold(int _iCurrentItemLevel)							//  아이템 업그래이드 재료 (골드)  09.08.27
{
	if(0 < _iCurrentItemLevel && dETERNAL_ITEM_MAX_LEVEL >_iCurrentItemLevel)
	{
		return l_UpgradeMaterials[_iCurrentItemLevel][eMATERIALS_GOLD];
	}

	return 0;
}

int
cACTOR::getUpgradeMaterialsStoneOfMystery(int _iCurrentItemLevel)							//  아이템 업그래이드 재료 (신비석)  09.08.27
{
	if(0 < _iCurrentItemLevel && dETERNAL_ITEM_MAX_LEVEL >_iCurrentItemLevel)
	{
		return l_UpgradeMaterials[_iCurrentItemLevel][eMATERIALS_STONEOFMYSTERY];
	}

	return 0;
}

int
cACTOR::getUpgradeMaterialsTantilless(int _iCurrentItemLevel)							//  아이템 업그래이드 재료 (탄틸리스유물)  09.08.27
{
	if(0 < _iCurrentItemLevel && dETERNAL_ITEM_MAX_LEVEL >_iCurrentItemLevel)
	{
		return l_UpgradeMaterials[_iCurrentItemLevel][eMATERIALS_TANTILLESS];
	}

	return 0;
}
int
cACTOR::getUpgradeMaterialsCrystal(int _iCurrentItemLevel)							//  아이템 업그래이드 재료 (결정석)  09.08.27
{
	if(0 < _iCurrentItemLevel && dETERNAL_ITEM_MAX_LEVEL >_iCurrentItemLevel)
	{
		return l_UpgradeMaterials[_iCurrentItemLevel][eMATERIALS_CRYSTAL];
	}

	return 0;
}
int
cACTOR::getUpgradeMaterialsGoldForJapan(int _iCurrentItemLevel)							//  아이템 업그래이드 재료 (골드)  일본용
{
	if(0 < _iCurrentItemLevel && dETERNAL_ITEM_MAX_LEVEL >_iCurrentItemLevel)
	{
		return l_UpgradeMaterialsForJapan[_iCurrentItemLevel][eMATERIALS_GOLD];
	}

	return 0;
}

int
cACTOR::getUpgradeMaterialsStoneOfMysteryForJapan(int _iCurrentItemLevel)							//  아이템 업그래이드 재료 (신비석)  일본용
{
	if(0 < _iCurrentItemLevel && dETERNAL_ITEM_MAX_LEVEL >_iCurrentItemLevel)
	{
		return l_UpgradeMaterialsForJapan[_iCurrentItemLevel][eMATERIALS_STONEOFMYSTERY];
	}

	return 0;
}

int
cACTOR::getUpgradeMaterialsTantillessForJapan(int _iCurrentItemLevel)							//  아이템 업그래이드 재료 (탄틸리스유물)  일본용
{
	if(0 < _iCurrentItemLevel && dETERNAL_ITEM_MAX_LEVEL >_iCurrentItemLevel)
	{
		return l_UpgradeMaterialsForJapan[_iCurrentItemLevel][eMATERIALS_TANTILLESS];
	}

	return 0;
}
int
cACTOR::getUpgradeMaterialsCrystalForJapan(int _iCurrentItemLevel)							//  아이템 업그래이드 재료 (결정석) 일본용
{
	if(0 < _iCurrentItemLevel && dETERNAL_ITEM_MAX_LEVEL >_iCurrentItemLevel)
	{
		return l_UpgradeMaterialsForJapan[_iCurrentItemLevel][eMATERIALS_CRYSTAL];
	}

	return 0;
}
int
cACTOR::getInventoryCosmicPowerUpgradeChanceMax()	// 인벤토리에 코스믹파워아이템이 존재하는지 확인하여 확률을 리턴한다.		//09.08.26
{
	int iChance = 0;
	for(int i=0;i<dEQUIPMENT_PART_COUNT;++i)
	{
		cItem	*lpItem	=	&m_aEquip[i];
		
		cBasicItem*	lpBasic = lpItem->getBasicItem(TRUE);
		if(lpItem->m_wBaseItem == 0xffff)
			continue;
		
		if(lpBasic->isCosmicPower())
			iChance = max(lpBasic->getUpgradeCosmicPowerSuccessChance(), iChance);
		
	}
	
	for (int j=0;j<dOWN_ITEM_COUNT;++j)
	{
		cItem	*lpItem	=	&m_aItems[j];
		cBasicItem*	lpBasic = lpItem->getBasicItem(TRUE);
		if(lpItem->m_wBaseItem == 0xffff)
			continue;

		if(lpBasic->isCosmicPower())
			iChance = max(lpBasic->getUpgradeCosmicPowerSuccessChance(), iChance);
		
		
	}
	
	for(int k=0;k<m_bf6ExtraInventorySize;++k)
	{
		cItem	*lpItem	=	&m_aExtraInventory[k];
		cBasicItem*	lpBasic = lpItem->getBasicItem(TRUE);
		if(lpItem->m_wBaseItem == 0xffff)
			continue;

		if(lpBasic->isCosmicPower())
			iChance = max(lpBasic->getUpgradeCosmicPowerSuccessChance(), iChance);	
		
	}
	
	return iChance;
	
}

int
cACTOR::getUpgradeSuccessChance(int _iWeaponSlot, int _iUpgradeItemSlot, int _iUpgradeNormalItemSlot)			//  이터널 아이템 업그래이드 확률~
{ 

	cItem* lpWeaponItem = getInventoryItem(_iWeaponSlot);
	
	if(!lpWeaponItem)
		return FALSE;

	cBasicItem*	lpBasic	=	lpWeaponItem->getBasicItem(TRUE);
	
	if(!lpBasic)
		return FALSE;

	if(lpWeaponItem->m_wBaseItem == 0xffff)
		return FALSE;
	
	if(!lpBasic->isEternalItem())
		return FALSE;

	int iCurrentItemLevel	= lpBasic->getCurrentEternalItemLevel();
	
	if(iCurrentItemLevel == dETERNAL_ITEM_MAX_LEVEL || iCurrentItemLevel < 1)
		return FALSE;
	
	int	iNextItemSerial		= lpBasic->getNextEternalItemSerial();
	
	int iUpgradeChance					= min(100, lpBasic->getUpgradeBaseSuccessChance()) * 1000;
#ifdef _FOR_JAPAN
	iUpgradeChance						= l_UpgradeChanceForJapan[iCurrentItemLevel];
#endif
	int iUpgradeCosmicChance			= (iUpgradeChance * (getInventoryCosmicPowerUpgradeChanceMax()  / 100.0));
	int iUpgradeMaterialsChance			= 0;
	int iUpgradeNormalMeterialsChance	= 0;
	
	
	if(_iUpgradeItemSlot !=0xffff)
	{
		
		cItem* lpMaterials = getInventoryItem(_iUpgradeItemSlot);
		if(!lpMaterials)
			return FALSE;
		
		cBasicItem*	lpBasic	=	lpMaterials->getBasicItem(TRUE);
		if(lpBasic && lpBasic->isUpgradeMaterialsItem() && lpBasic->m_wKind == eIK_PREMIUM_ITEM)
			iUpgradeMaterialsChance	+=	(iUpgradeChance * (lpBasic->getUpgradeMaterialsItemSuccessChance()/100.0));
		
	}
	if(_iUpgradeNormalItemSlot !=0xffff)
	{
		
		cItem* lpMaterials = getInventoryItem(_iUpgradeNormalItemSlot);
		if(!lpMaterials)
			return FALSE;
		
		cBasicItem*	lpBasic	=	lpMaterials->getBasicItem(TRUE);
		if(lpBasic && lpBasic->isUpgradeMaterialsItem() && lpBasic->m_wKind != eIK_PREMIUM_ITEM)
			iUpgradeNormalMeterialsChance	+=	(iUpgradeChance * (lpBasic->getUpgradeMaterialsItemSuccessChance()/100.0));
		
	}

	iUpgradeChance = iUpgradeChance + iUpgradeCosmicChance + iUpgradeMaterialsChance + iUpgradeNormalMeterialsChance;

	return min(100000,	iUpgradeChance);
}

BOOL
cACTOR::upgradeEternalItem(int _iWeaponSlot, int _iUpgradeItemSlot, int _iUpgradeNormalItemSlot, BOOL _bUpgradePerfect)			//  이터널 아이템 업그레이드 
{

	if	(isOperator()	==	FALSE)
		_bUpgradePerfect	=	FALSE;

	int iUpgradeChance				= getUpgradeSuccessChance(_iWeaponSlot,	_iUpgradeItemSlot,_iUpgradeNormalItemSlot);
	
	cItem* lpWeaponItem				= getInventoryItem(_iWeaponSlot);
	
	if(!lpWeaponItem)
		return FALSE;

	cBasicItem*	lpBasic				= lpWeaponItem->getBasicItem(TRUE);
	
	if(!lpBasic)
		return FALSE;

	if(_bUpgradePerfect)
	{
		if	(m_wOperatorLevel <= eAL_NORMAL || !g_bIsTestServer)
		{
			_bUpgradePerfect = FALSE;
		}
	}


	if(lpWeaponItem->isExpirationData())
	{		// 기간제한아이템은 강화시키지않는다.
		return FALSE;
	}

	int iCurrentLevel				= lpBasic->getCurrentEternalItemLevel();
	int iNextItemSerial				= lpBasic->getNextEternalItemSerial();
	int iUpgradeItemSerial			= 0xffff;
	int iUpgradeNormalItemSerial		= 0xffff;
	int iUpgradePenaltyItemSerial	= lpBasic->getUpgradePenaltyItemSerial();
	int iRemoveWeaponSlot			=	_iWeaponSlot;
	int iUpgradeMaterialsGold				=	getUpgradeMaterialsGold(iCurrentLevel);		
	int iUpgradeMaterialsStoneOfMystery		=	getUpgradeMaterialsStoneOfMystery(iCurrentLevel);
	int iUpgradeMaterialsTantilless			=	getUpgradeMaterialsTantilless(iCurrentLevel);	
	int iUpgradeMaterialsCrystal			=	getUpgradeMaterialsCrystal(iCurrentLevel);		

#ifdef	_FOR_JAPAN
	iUpgradeMaterialsGold				=	getUpgradeMaterialsGoldForJapan(iCurrentLevel);		
	iUpgradeMaterialsStoneOfMystery		=	getUpgradeMaterialsStoneOfMysteryForJapan(iCurrentLevel);
	iUpgradeMaterialsTantilless			=	getUpgradeMaterialsTantillessForJapan(iCurrentLevel);	
	iUpgradeMaterialsCrystal			=	getUpgradeMaterialsCrystalForJapan(iCurrentLevel);	
#endif
	BOOL	bIsNotPenalty				= FALSE;


	CItemPrefixInfo		m_aWeaponPrefix[dITEM_PREFIX_COUNT];	
	memcpy(m_aWeaponPrefix, lpWeaponItem->m_aPrefix,sizeof(CItemPrefixInfo) * dITEM_PREFIX_COUNT);

	cItem	cLogTargetItem;
	g_im.generateItemByBaseItem(&cLogTargetItem, lpWeaponItem->m_wBaseItem ,1);


	if(iCurrentLevel<1 || iCurrentLevel>=dETERNAL_ITEM_MAX_LEVEL || !lpBasic->isEternalItem())
		return FALSE;

	if(	m_iGold												< iUpgradeMaterialsGold				|| 
		getStackItemCount(dITEM_INDEX_STONEOFMYSTERY,FALSE) < iUpgradeMaterialsStoneOfMystery	|| 
		getStackItemCount(dITEM_INDEX_TANTILLESS,FALSE)		<iUpgradeMaterialsTantilless		|| 
		getStackItemCount(dITEM_INDEX_CRYSTAL,FALSE)		< iUpgradeMaterialsCrystal		)
	{

		sendEtcWork(eEW_UPGRADE_MATERIALS_ERR);		// 재료가 부족하다는 메세지를 보낸다. 
		return FALSE;
	}

	if(_iUpgradeItemSlot != 0xffff)
	{
		cItem*	lpMaterials = getInventoryItem(_iUpgradeItemSlot);
		if(!lpMaterials)
			return FALSE;
		
			cBasicItem*	lpBasic	=	lpMaterials->getBasicItem(TRUE);
			iUpgradeItemSerial = lpMaterials->m_wBaseItem;
			if(lpBasic && lpBasic->isUpgradeEternalItemNotPenalty() && lpBasic->m_wKind == eIK_PREMIUM_ITEM)
				bIsNotPenalty = TRUE;
			
			lpMaterials->decreaseCount(1);
			
		
	}
	
	if(_iUpgradeNormalItemSlot != 0xffff)
	{
		cItem*	lpMaterials = getInventoryItem(_iUpgradeNormalItemSlot);
		if(!lpMaterials)
			return FALSE;
		
			cBasicItem*	lpBasic	=	lpMaterials->getBasicItem(TRUE);
			iUpgradeNormalItemSerial = lpMaterials->m_wBaseItem;
			if(lpBasic && lpBasic->isUpgradeEternalItemNotPenalty()&& lpBasic->m_wKind != eIK_PREMIUM_ITEM)
				bIsNotPenalty = TRUE;
			
			lpMaterials->decreaseCount(1);
			
		
	}

	m_iGold -= iUpgradeMaterialsGold;

	updateGoldLog(-iUpgradeMaterialsGold,eSPENT_GOLD_BY_UPGRADE_ETERNAL_ITEM);
	
	sendAddGold(-iUpgradeMaterialsGold);

	if(iUpgradeMaterialsStoneOfMystery)
		removeItem(dITEM_INDEX_STONEOFMYSTERY,iUpgradeMaterialsStoneOfMystery, FALSE,eRI_BY_ETERNAL,eSPENT_GOLD_IGNORE);
	if(iUpgradeMaterialsTantilless)
		removeItem(dITEM_INDEX_TANTILLESS,iUpgradeMaterialsTantilless, FALSE,eRI_BY_ETERNAL,eSPENT_GOLD_IGNORE);
	if(iUpgradeMaterialsCrystal)
		removeItem(dITEM_INDEX_CRYSTAL,iUpgradeMaterialsCrystal, FALSE,eRI_BY_ETERNAL,eSPENT_GOLD_IGNORE);

	int iRand = largeRandom(100000);

	if	(iRand < iUpgradeChance || _bUpgradePerfect )			//100000 == 100%다! (100(%)*1000)		 운영자 명령어 입력시 성공률100%
	{
		lpWeaponItem->decreaseCount(1);
			
		cItem	addItem;
		g_im.generateItemByBaseItem(&addItem,iNextItemSerial ,1);
		addItem.m_dwSerial	=	largeRandom(0x7ffffff0)+10;		
		memcpy(addItem.m_aPrefix, m_aWeaponPrefix,sizeof(CItemPrefixInfo) * dITEM_PREFIX_COUNT);

		cBasicItem* lpBasic = addItem.getBasicItem(TRUE);

		if	(!lpBasic)
			return FALSE;

		addItem.m_bDurability = lpBasic->getDurability();

		if	(lpBasic->getCurrentEternalItemLevel() == dETERNAL_ITEM_MAX_LEVEL)
		{
			WORLDtoSERVERS_EVENT_MESSAGE packet;		
			strcpy(packet.strMessage,m_strName);		
			packet.wType = eCT_ETERNAL_UPGRADE_MESSAGE;			
			g_game.operateEventMessage(&packet);

		}
	
		if	(addItem.isMoneyItem())
			CLOG("goldIOError","upgradeEternalItem");
		
		addItemToInventory(&addItem);	
		
		sendEtcWork(eEW_UPGRADE_SUCCESS,_iWeaponSlot, iNextItemSerial, _iUpgradeItemSlot, _iUpgradeNormalItemSlot);		// 강화가 성공했다는 메세지를 보낸다~아아~
	}
	else
	{
		int iSerial = iUpgradePenaltyItemSerial;

		if(bIsNotPenalty)
		{
			if(iCurrentLevel<7)
			{
				iSerial	=0xffff;
				iNextItemSerial = cLogTargetItem.m_wBaseItem;
			}
			else
			{
				iNextItemSerial = iUpgradePenaltyItemSerial;
				for(int i=0;i<iCurrentLevel-2;++i)
				{
					cItem	addItem;
					g_im.generateItemByBaseItem(&addItem,iNextItemSerial ,1);
					iNextItemSerial = addItem.getBasicItem(TRUE)->getNextEternalItemSerial();

				}
				lpWeaponItem->decreaseCount(1);
				cItem	addItem;
				g_im.generateItemByBaseItem(&addItem,iNextItemSerial ,1);
				addItem.m_dwSerial	=	largeRandom(0x7ffffff0)+10;	
				memcpy(addItem.m_aPrefix, m_aWeaponPrefix,sizeof(CItemPrefixInfo) * dITEM_PREFIX_COUNT);

				if	(addItem.isMoneyItem())
					CLOG("goldIOError","upgradeEternalItem2");

				addItemToInventory(&addItem);
				iSerial = iNextItemSerial;
				
			}
		}
		else
		{
			if(iCurrentLevel>1)
			{
				lpWeaponItem->decreaseCount(1);				
				cItem	addItem;
				g_im.generateItemByBaseItem(&addItem,iUpgradePenaltyItemSerial ,1);
				addItem.m_dwSerial	=	largeRandom(0x7ffffff0)+10;	
				memcpy(addItem.m_aPrefix, m_aWeaponPrefix,sizeof(CItemPrefixInfo) * dITEM_PREFIX_COUNT);
				cBasicItem* lpBasic = addItem.getBasicItem(TRUE);
				if(!lpBasic)
					return FALSE;
				
				addItem.m_bDurability = lpBasic->getDurability();

				if	(addItem.isMoneyItem())
					CLOG("goldIOError","upgradeEternalItem3");

				addItemToInventory(&addItem);

				iNextItemSerial = iUpgradePenaltyItemSerial;
			}else
			{
				iSerial	=0xffff;
				iNextItemSerial = cLogTargetItem.m_wBaseItem;
			}

		}

		sendEtcWork(eEW_UPGRADE_FAILED,iRemoveWeaponSlot,iSerial, _iUpgradeItemSlot, _iUpgradeNormalItemSlot);	// 강화가 실패했다는 메세지를 보낸다~아아~

		
	}

	sendUpgradeItemLog(eEIW_UPGRADE_ETERNAL_ITEM, &cLogTargetItem,iNextItemSerial, iUpgradeMaterialsGold, iUpgradeMaterialsStoneOfMystery,
					iUpgradeMaterialsTantilless, iUpgradeMaterialsCrystal,iUpgradeNormalItemSerial,iUpgradeItemSerial);


	return TRUE;

}