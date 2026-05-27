#include "CHero.H"
#include "CActor.H"
#include "CFieldItem.H"
#include "CMessage.H"
#include "CItemEffect.H"
#include "cExceptionHandler.H"
#include "pieceItem.H"
#include "cWindowInterface.H"
#include "hero_mini_pet.H"

//	체크섬을 구한다.
WORD
CHero::getItemCheckSum()
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
//	사용 가능한 아이템이냐?
BOOL
CHero::isUseAbleItem(cItem *_lpItem,BOOL _bIsCheckSecondJob,BOOL _bIsOnlyCheckJob)
{
	if	(!_lpItem)
		return	FALSE;

	if	(isInterruptingArmor(_lpItem))
		return	FALSE;

	if(isInterruptingWeapon(_lpItem))	// 무기해제
		return	FALSE;

	cBasicItem	*lpBasicItem	=	_lpItem->getBasicItem(TRUE);

	if	(!lpBasicItem)
		return	FALSE;

	BOOL		bIsUseAbleJob	=	lpBasicItem->isEquipAbleJob(m_wJob);

	///┏ SetItem 버그 수정... 09.06.29
	cSetItem* pSetItem = _lpItem->getSetItem();

	if	(pSetItem)
	{
		BOOL bSet = pSetItem->isEnableJob(m_wJob);

		if	(bSet == FALSE)
			return FALSE;
	}
	///┗━━━━━━━━━━━━━━━━━


	if	(lpBasicItem->isManExclusiveItem())
	{
		if	(c_aIsMale[m_wJob])
			bIsUseAbleJob	=	TRUE;
		else
			bIsUseAbleJob	=	FALSE;
	}

	if	(lpBasicItem->isWomanExclusiveItem())
	{
		if	(c_aIsMale[m_wJob])
			bIsUseAbleJob	=	FALSE;
		else
			bIsUseAbleJob	=	TRUE;
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
	
	if	(!lpBasicItem->m_bf1IsMaleJobEquipAble)	
		if	(c_aIsMale[m_wJob])	
			return	FALSE;		//	남자는 착용 불가
	if	(!lpBasicItem->m_bf1IsFemaleJobEquipAble)	
		if	(!c_aIsMale[m_wJob])	
			return	FALSE;		//	여자는 착용 불가

	if	(_bIsOnlyCheckJob)
		return	TRUE;

	if	(_lpItem->isBreedingRecordBook())
	{
		if	(m_wJob	!=	dJOB_BEAST_TAMER)
			return	FALSE;

		return	TRUE;
	}

	if(m_wTransformationTime)
	{// 변신 중이당..
		if(!lpBasicItem->m_bf1IsUseItemAfterTransformation)
			return FALSE;
	}

	int	iRequireLevel	=	_lpItem->getRequireLevel();

	if	(_lpItem->isExceptionItem()	==	FALSE	&&	_lpItem->m_isBroken	)
		return	FALSE;
	if	(_lpItem->isBreakdownAbleItem() && _lpItem->m_bDurability == 0	)
		return	FALSE;

//	성향 체크
	switch(lpBasicItem->m_wRequireAllignment)
	{
		case	dALLIGNMENT_ABSOLUTE_EVIL	:	//	절대 악
			if (GetAllignment(m_sAllignment)	!=	dALLIGNMENT_ABSOLUTE_EVIL		)	return	FALSE;
			break;

		case	dALLIGNMENT_EVIL			:	//	악
			if (GetAllignment(m_sAllignment)	!=	dALLIGNMENT_ABSOLUTE_EVIL	&&
				GetAllignment(m_sAllignment)	!=	dALLIGNMENT_EVIL				)	return	FALSE;
			break;

		case	dALLIGNMENT_NEUTRAL			:	//	중립
			if (GetAllignment(m_sAllignment)	!=	dALLIGNMENT_NEUTRAL				)	return	FALSE;
			break;

		case	dALLIGNMENT_GOOD			:	//	선
			if (GetAllignment(m_sAllignment)	!=	dALLIGNMENT_ABSOLUTE_GOOD	&&
				GetAllignment(m_sAllignment)	!=	dALLIGNMENT_GOOD				)	return	FALSE;
			break;

		case	dALLIGNMENT_ABSOLUTE_GOOD	:	//	절대 선
			if (GetAllignment(m_sAllignment)	!=	dALLIGNMENT_ABSOLUTE_GOOD		)	return	FALSE;
			break;
	}

	if	(_lpItem->getSpecialItem())
	{
		if	(_lpItem->getSpecialItem()->m_bf1IsFreeUseItem)
			return	TRUE;
	}

	int iLevel	=	getLevel();

	if	(!_lpItem->isCanUseLevel(iLevel,m_wLimitEquipmentLevelOverPoint))	return FALSE;
	if	(getStrength()		<	lpBasicItem->m_wRequireStrength			)	return	FALSE;	//	사용/장비를 위한 최소 힘
	if	(getAgility()		<	lpBasicItem->m_wRequireDexterity		)	return	FALSE;	//	사용/장비를 위한 최소 민첩성
	if	(getConstitution()	<	lpBasicItem->m_wRequireConstitution		)	return	FALSE;	//	사용/장비를 위한 최소 체질
	if	(getWisdom()		<	lpBasicItem->m_wRequireWisdom		)	return	FALSE;	//	사용/장비를 위한 최소 지혜
	if	(getIntelligence()	<	lpBasicItem->m_wRequireIntelligence		)	return	FALSE;	//	사용/장비를 위한 최소 지식
	if	(getCharisma()		<	lpBasicItem->m_wRequireCharisma			)	return	FALSE;	//	사용/장비를 위한 최소 카리스마
	if	(getLuck()			<	lpBasicItem->m_wRequireLuck				)	return	FALSE;	//	사용/장비를 위한 최소 운

	int		iValue			=	lpBasicItem->m_wEquipUseLimitValue;
	int		iContents		=	lpBasicItem->m_wEquipUseLimitContents;
	int		iMethod			=	lpBasicItem->m_wEquipUseLimitMethod;
	int		iContentsValue	=	0;

	if	(iContents	==	0xffff)
		return	TRUE;

	switch(iContents)
	{
		case	0	:	iContentsValue	=	getLevel();break;//"레벨",
		case	1	:	iContentsValue	=	getStrength();break;//"힘",
		case	2	:	iContentsValue	=	getAgility();break;//"민첩성",
		case	3	:	iContentsValue	=	getConstitution();break;//"건강",
		case	4	:	iContentsValue	=	getCharisma();break;//"카리스마",
		case	5	:	iContentsValue	=	getIntelligence();break;//"지식",
		case	6	:	iContentsValue	=	getWisdom();break;//"지혜",
		case	7	:	iContentsValue	=	getLuck();break;//"행운",
	}

	switch(iMethod)
	{
		case	0xffff	:
			if (iContentsValue	<	iValue)	return	FALSE;
			break;

		case	0		:
			iValue	*=	(int )_lpItem->m_aOption[0];
			if (iContentsValue	<	iValue)	return	FALSE;
			break;

		case	1		:
			iValue	*=	(int )_lpItem->m_aOption[1];
			if (iContentsValue	<	iValue)	return	FALSE;
			break;

		case	2		:
			iValue	=	(int )_lpItem->m_aOption[0]/iValue;
			if (iContentsValue	<	iValue)	return	FALSE;
			break;

		case	3		:
			iValue	=	(int )_lpItem->m_aOption[1]/iValue;
			if (iContentsValue	<	iValue)	return	FALSE;
			break;
	}


	return	TRUE;
}


int
CHero::getBadgeCountInInventory()
{
	int		i,iBadgeCount=0;

	for (i=0;i<dOWN_ITEM_COUNT;++i)
	{
		cItem		*lpItem			=	&g_hero.m_aItems[i];
		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(lpBasicItem	==	NULL)
			continue;

		if	(lpBasicItem->m_attr.isBadge)
			++iBadgeCount;
	}

	for (i=0;i<g_hero.m_bf6ExtraInventorySize;++i)
	{
		cItem		*lpItem			=	&g_hero.m_aExtraInventory[i];
		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(lpBasicItem	==	NULL)
			continue;

		if	(lpBasicItem->m_attr.isBadge)
			++iBadgeCount;
	}

	return	iBadgeCount;
}


//	인벤토리에 아이템을 추가한다
BOOL
CHero::addItemToInventory(cItem *_lpItem)
{
	if	(_lpItem->m_wBaseItem	==	dITEM_MONEY)
	{
		m_iGold	+=	_lpItem->m_dwSerial;
		_lpItem->reset();

		return	TRUE;
	}

	if	(_lpItem->isItemPack())
	{
		int	iPackIndex	=	_lpItem->m_aOption[0];

		if	(g_astrItemPackListName[iPackIndex][0]	==	NULL)
			AddAskItemPackName(iPackIndex);
	}

	int	iCount	=	_lpItem->m_bCount;

	if	(_lpItem->isStackAbleItem() && !_lpItem->isRareItem() && !_lpItem->isUniqueItem()&& !_lpItem->isLittleGoodItem())	//	겹칠 수 있는 아이템이다.
	{
		cItem	*lpSameItem	=	getStackAbleItem(_lpItem);

		while(lpSameItem)	//	같은 아이템이 있다면 겹치자.
		{
			lpSameItem->addItem(_lpItem,lpSameItem->getStackLimit());

			if (_lpItem->m_wBaseItem	==	0xffff)	break;

			lpSameItem	=	getStackAbleItem(_lpItem);
		}

		if	(_lpItem->m_wBaseItem	==	0xffff)
		{
			arrangeInventory();

			return	TRUE;	//	위에서 겹쳐-_-졌나 보다.
		}
	}

	cItem	*lpEmptySlot	=	getEmptyItemSlot();
	
	if	(!lpEmptySlot)	//	빈 공간이 없고..
	{
		if	(iCount	!=	_lpItem->m_bCount)
		{
			arrangeInventory();

			return	TRUE;	//	위에서 겹쳤다.
		}
		
		return	FALSE;	//	겹치지도 못하고.. -o-;;
	}

	lpEmptySlot->copy(_lpItem);
	_lpItem->reset();

	if	(lpEmptySlot->m_bDurability	==	0	&&	lpEmptySlot->getBasicItem()->m_attr.isDurabilityToUseCount)
		lpEmptySlot->reset();

	cBasicItem	*lpBasicItem	=	lpEmptySlot->getBasicItem();

	if	(lpBasicItem && lpBasicItem->m_attr.isEffectInInventoryItem)
		buildPower();

	arrangeInventory();

	return	TRUE;
}

//
//	인벤토리에 아이템을 추가한다
BOOL
CHero::isPickAbleItem(cItem *_lpItem)
{
	if (_lpItem->isStackAbleItem())	//	겹칠 수 있는 아이템이다.
	{
		cItem	*lpSameItem	=	getStackAbleItem(_lpItem);

		if (lpSameItem)	return	TRUE;
	}

	cItem	*lpEmptySlot	=	getEmptyItemSlot();
	
	if (!lpEmptySlot)	return	FALSE;

	return	TRUE;
}

//
//	_iPart에 아이템 착용
BOOL
CHero::isEquipAbleItem(cItem *_lpItem,int _iPart)
{
	cItem	tempItem;

	int		iKind	=	g_aBasicItem[_lpItem->m_wBaseItem].m_wKind;
	int		iPart	=	_iPart;

/*	if (_lpItem->is4FingerRing())
	{
		if (_iPart != dEQUIP_RING1 && _iPart != dEQUIP_RING5)
			return	FALSE;	//	반지의 경우는 제자리에 끼워야 한다.

		BOOL	isIncorrect	=	FALSE;
		if (_iPart == dEQUIP_RING1)
		{
			if (m_aEquip[dEQUIP_RING2].m_wBaseItem != 0xffff)	isIncorrect	=	TRUE;	//	반지의 경우는 제자리에 끼워야 한다.
			if (m_aEquip[dEQUIP_RING3].m_wBaseItem != 0xffff)	isIncorrect	=	TRUE;	//	반지의 경우는 제자리에 끼워야 한다.
			if (m_aEquip[dEQUIP_RING4].m_wBaseItem != 0xffff)	isIncorrect	=	TRUE;	//	반지의 경우는 제자리에 끼워야 한다.

			if (isIncorrect)
			{
				g_lpHero->warning(dMSG_FIST_RING_LIMIT);

				return FALSE;
			}
		}

		if (_iPart == dEQUIP_RING5)
		{
			if (m_aEquip[dEQUIP_RING6].m_wBaseItem != 0xffff)	isIncorrect	=	TRUE;	//	반지의 경우는 제자리에 끼워야 한다.
			if (m_aEquip[dEQUIP_RING7].m_wBaseItem != 0xffff)	isIncorrect	=	TRUE;	//	반지의 경우는 제자리에 끼워야 한다.
			if (m_aEquip[dEQUIP_RING8].m_wBaseItem != 0xffff)	isIncorrect	=	TRUE;	//	반지의 경우는 제자리에 끼워야 한다.

			if (isIncorrect)
			{
				g_lpHero->warning(dMSG_FIST_RING_LIMIT);

				return FALSE;
			}
		}
	}
*/
	return TRUE;
}

//
//	_iPart에 아이템 착용
BOOL
CHero::equipItem(cItem *_lpItem,int _iPart)
{
	cItem		tempItem;
	cBasicItem	*lpBasicItem	=	_lpItem->getBasicItem(TRUE);

	if	(!lpBasicItem)
		return	FALSE;

	int		iKind		=	lpBasicItem->m_wKind;
	int		iPart		=	_iPart;
	int		iLimitCount	=	0xffff;

	if	(_iPart	<	dEQUIP_BELT_SLOT1 || iPart	==	0xffff)
	{
		if	(c_aItemEquipPlace[iKind]	==	dITEM_KIND_WASTING)
			return	FALSE;//	소비형 아이템이다.

		if	(iKind	==	eIK_RING)
		{
			if	(iPart<dEQUIP_RING1	||	iPart>dEQUIP_RING8)
				return	FALSE;	//	반지의 경우는 제자리에 끼워야 한다.

/*			if (_lpItem->is4FingerRing())
			{
				if (_iPart != dEQUIP_RING1 && _iPart != dEQUIP_RING5)
					return	dITEM_EQUIP_RESULT_NOT_RING_PLACE;	//	반지의 경우는 제자리에 끼워야 한다.

				if (_iPart == dEQUIP_RING1)
				{
					if (m_aEquip[dEQUIP_RING2].m_wBaseItem != 0xffff)	return	dITEM_EQUIP_RESULT_NOT_FIST_RING_PLACE;	//	반지의 경우는 제자리에 끼워야 한다.
					if (m_aEquip[dEQUIP_RING3].m_wBaseItem != 0xffff)	return	dITEM_EQUIP_RESULT_NOT_FIST_RING_PLACE;	//	반지의 경우는 제자리에 끼워야 한다.
					if (m_aEquip[dEQUIP_RING4].m_wBaseItem != 0xffff)	return	dITEM_EQUIP_RESULT_NOT_FIST_RING_PLACE;	//	반지의 경우는 제자리에 끼워야 한다.
				}

				if (_iPart == dEQUIP_RING5)
				{
					if (m_aEquip[dEQUIP_RING6].m_wBaseItem != 0xffff)	return	dITEM_EQUIP_RESULT_NOT_FIST_RING_PLACE;	//	반지의 경우는 제자리에 끼워야 한다.
					if (m_aEquip[dEQUIP_RING7].m_wBaseItem != 0xffff)	return	dITEM_EQUIP_RESULT_NOT_FIST_RING_PLACE;	//	반지의 경우는 제자리에 끼워야 한다.
					if (m_aEquip[dEQUIP_RING8].m_wBaseItem != 0xffff)	return	dITEM_EQUIP_RESULT_NOT_FIST_RING_PLACE;	//	반지의 경우는 제자리에 끼워야 한다.
				}
			}
			*/
		}
		else
		{
			if	(iPart					==	0xffff	)
				iPart	=	c_aItemEquipPlace[iKind];	//	장비 슬롯이 아닌 곳에 올렸다. 적당히 빈자리 찾아서 넣는다.
			if	(c_aItemEquipPlace[iKind]!=	iPart	)
			{
				if	(c_aItemEquipPlace[iKind]	==	dEQUIP_WEAPON	&&	iPart	==	dEQUIP_WEAPON_2)
					iPart	=	dEQUIP_WEAPON_2;
				else
					iPart	=	c_aItemEquipPlace[iKind];//	엉뚱한 위치에 있다.
			}
		}
	}
	else
		iLimitCount	=	min(m_wBeltStackCount,_lpItem->getStackLimit());

	if	(iLimitCount	==	0)
		return	FALSE;

	if	(iPart	==	dEQUIP_WEAPON)
	{
		if	(isUseAbleItem(_lpItem) == (m_wJob%2))
		{
			if	(m_wJob	!=	dJOB_SUMMONER	&&	m_wJob	!=	dJOB_DEVIL)
				iPart	=	dEQUIP_WEAPON_2;
		}
		//	현 직업에서 사용 가능한 아이템이고 세컨직업이면 무기 2
		//	현 직업에서 사용 불가하고 아이템이고 본직업이면 무기 2
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

					return	TRUE;
				}
			}

			m_aEquip[iPart].swapItem(_lpItem);
		}
		else
			m_aEquip[iPart].swapItem(_lpItem);

		arrangeInventory();
		buildPower();
		syncHeroApprearance();

		return	TRUE;
	}

//	벨트에 아이템을 장착하는 거다.
	if (m_aEquip[iPart].m_wBaseItem	!=	0xffff && m_aEquip[iPart].m_bCount > 0)		//	이미 뭔가를 장비하고 있다.
	{
		if (!m_aEquip[iPart].isFull(iLimitCount))//겹칠 수 있는거고...
		{
			if (m_aEquip[iPart].isSameItem(_lpItem))//같은거.. -o-
			{
				m_aEquip[iPart].addItem(_lpItem,iLimitCount);

				arrangeInventory();

				return	TRUE;
			}
		}

		if	(_lpItem->m_bCount	>	m_wBeltStackCount)	//	아이템 위치를 바꾸려고 하는데, 아이템 갯수가 벨트 스택 아이템 갯수 보다 많다.
		{
			cItem	*lpValidInventorySlot	=	getValidInventorySlot();

			if (lpValidInventorySlot)
			{
				m_aEquip[iPart].swapItem(_lpItem);

				lpValidInventorySlot->copy(&m_aEquip[iPart]);

				int	iSpareItemCount				=	m_aEquip[iPart].m_bCount-m_wBeltStackCount;

				m_aEquip[iPart].m_bCount		=	(BYTE)m_wBeltStackCount;
				lpValidInventorySlot->m_bCount	=	iSpareItemCount;
			}
			else	return	FALSE;
		}
		else	m_aEquip[iPart].swapItem(_lpItem);
	}
	else
	{
		if (_lpItem->m_bCount	<=	m_wBeltStackCount)
		{
			m_aEquip[iPart].swapItem(_lpItem);
			if	(_lpItem->m_bCount	<=	0)
				_lpItem->reset();
		}
		else	//	아이템 위치를 바꾸려고 하는데, 아이템 갯수가 벨트 스택 아이템 갯수 보다 많다.
		{
			m_aEquip[iPart].copy(_lpItem);
			m_aEquip[iPart].m_bCount	=	(BYTE)m_wBeltStackCount;
			_lpItem->m_bCount			=	_lpItem->m_bCount-m_wBeltStackCount;
		}
	}

	arrangeInventory();

	return TRUE;
}

//
//	_iPart에 아이템 착용
BOOL
CHero::equipItem(int _iItemSlot,int _iPart)
{
	cItem	*lpItem	=	getInventoryItem(_iItemSlot);

	return	equipItem(lpItem,_iPart);
}

//
//	어떤 부위의 것을 벗냐?
BOOL
CHero::stripEquipment(int _iPart)
{
	cBasicItem	*lpBasicItem	=	m_aEquip[_iPart].getBasicItem();

	if	(!lpBasicItem)
		return	FALSE;

	if	(!m_aEquip[_iPart].isExtraItem()	&&	!m_aEquip[_iPart].isMagicCarpetEmblem() && !m_aEquip[_iPart].isExceptionItem())
		if (m_aEquip[_iPart].m_bCount	==	0)
		{
			m_aEquip[_iPart].reset();
			return	TRUE;
		}

	addItemToInventory(&m_aEquip[_iPart]);
	buildPower();
	syncHeroApprearance();

	return TRUE;
}

//
//	두 아이템의 위치를 바꾼다.
BOOL
CHero::changeItemPlace(int _iSrcItem,int _iDestItem)
{
	if (_iSrcItem	<		0)
		return	FALSE;
	if (_iSrcItem	>=	m_wItemSlotCount &&	_iSrcItem < dOWN_ITEM_COUNT)
		return	FALSE;
	if (_iSrcItem	>=	dOWN_ITEM_COUNT	+	m_bf6ExtraInventorySize)
		return	FALSE;
	if (_iDestItem	<		0)
		return	FALSE;
	if (_iDestItem	>=	m_wItemSlotCount &&	_iDestItem < dOWN_ITEM_COUNT)
		return	FALSE;
	if (_iDestItem	>=	dOWN_ITEM_COUNT	+	m_bf6ExtraInventorySize)
		return	FALSE;

	if (_iSrcItem	==	_iDestItem)
		return	TRUE;

	cItem	*lpSrcItem	=	getInventoryItem(_iSrcItem);
	cItem	*lpDestItem=	getInventoryItem(_iDestItem,FALSE,TRUE);

	if (!lpSrcItem	||	lpSrcItem->m_wBaseItem	==	0xffff)
		return	FALSE;

	if (lpSrcItem->isSameItem(lpDestItem) && !lpDestItem->isFull())	//	만약에 같은 아이템이면 겹친다.
	{
		lpDestItem->addItem(lpSrcItem);

		arrangeInventory();

		return	TRUE;	//	겹치고 났더니 남은게 없으면 리턴
	}

	lpSrcItem->swapItem(lpDestItem);

	arrangeInventory();

	return	TRUE;
}

//
//	같은 아이템을 찾아서 리턴한다.
cItem*
CHero::getSameItem(cItem *_lpItem)
{
	int		i;

	for (i=0;i<m_wItemSlotCount;i++)
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
CHero::getSameInventoryItemCount(cItem* _lpItem)		//09.08.11
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
CHero::getSameEquipItemCount(cItem* _lpItem)		
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
CHero::getStackAbleItem(cItem *_lpItem)
{
	int		i;

	for (i=0;i<m_wItemSlotCount;i++)
	{
		cItem	*lpItem	=	&m_aItems[i];

		if (lpItem->m_wBaseItem	==	0xffff)	continue;

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
CHero::getSameKindItem(cItem *_lpItem)
{
	int	i,iKind	=	_lpItem->getBasicItem(TRUE)->m_wKind;

	for (i=0;i<m_wItemSlotCount;i++)
	{
		cItem	*lpItem	=	&m_aItems[i];

		if (lpItem->m_wBaseItem	==	0xffff)	continue;

		if (lpItem->getBasicItem(TRUE)->m_wKind	==	iKind)	return	lpItem;
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		cItem	*lpItem	=	&m_aExtraInventory[i];

		if (lpItem->m_wBaseItem	==	0xffff)
			continue;

		if (lpItem->getBasicItem(TRUE)->m_wKind	==	iKind)
			return	lpItem;
	}

	return	NULL;
}

//
//	리로드
BOOL
CHero::reloadBeltSlot()
{
	int		i;
	cItem	*lpSlot;
	BOOL	isReload	=	FALSE;

	for (i=dEQUIP_BELT_SLOT1;i<=dEQUIP_BELT_SLOT5;)
	{
		lpSlot	=	&m_aEquip[i];

		i++;

		if (lpSlot->m_wBaseItem	==	0xffff		)
			continue;

		int	iLimitCount			=	min(m_wBeltStackCount,lpSlot->getStackLimit());

		if	(lpSlot->m_bCount	>=	iLimitCount	)
			continue;
		
		cItem	*lpItem		=	getSameItem(lpSlot);

		if	(!lpItem)
			continue;

		if	(!lpSlot->addItem(lpItem,iLimitCount))
			continue;

		if (lpSlot->m_bCount < iLimitCount)
			i--;

		isReload	=	TRUE;
	}

	if (isReload)
	{
		arrangeInventory();
		CGamePlay::AddSystemMessage(WHITE,dMSG_RELOAD_BELT_ITEM);
	}

	m_bf1IsReloadingBeltItem	=	FALSE;

	return	TRUE;
}

//
//	현재 락온된-_- 필드 아이템을 줍는다.
BOOL
CHero::pickFieldItem()
{
	if (m_iBookedPickItem			!=	0xffff	)
	{
		if	(g_fieldItem.get(m_iBookedPickItem)	==	NULL)
			m_iBookedPickItem	=	0xffff;
		else
			return	TRUE;
	}
	if (g_fieldItem.m_iFocusItem	==	0xffff	)
		return	FALSE;
	if (!s_isLeftButtonPressed					)
		return	FALSE;

	cDroppedItem	*lpDroppedItem	=	g_fieldItem.getFocusedItem();

	if (!lpDroppedItem)
	{
		g_fieldItem.releasePickItem();
		
		return	FALSE;
	}

	g_fieldItem.m_iPickItem			=	g_fieldItem.m_iFocusItem;

	int	iRange	=	GetOvalRange(lpDroppedItem->m_pos.x,lpDroppedItem->m_pos.y,g_lpHero->m_pos.x,g_lpHero->m_pos.y);

	if (iRange	<=	dPICK_ABLE_RANGE_FOR_CLIENT)	//	줍기 가능한 거리
	{
		if (m_iLastActionTime+dSEND_HERO_PICK_ITEM_PERIOD	>	s_iFrameCounter	)
			return	FALSE;

		if (m_isWalking)
			sendStop();

		if (s_agent.sendPickFieldItem())
		{
			m_iLastActionTime	=	s_iFrameCounter;
			m_iBookedPickItem	=	g_fieldItem.m_iPickItem;	//	줍기 예약
		}
	}
	else
	{
		moveTo(lpDroppedItem->m_pos.x,lpDroppedItem->m_pos.y);	//	줍기 가능한 거리로 이동
	}

	return	TRUE;
}

//
//	
void
CHero::pickNearestItem()
{
#ifndef	_FOR_JAPAN

#ifndef	_DEBUG
	return;
#endif

#endif

	cDroppedItem	*lpDroppedItem	=	g_fieldItem.pickNearestItem(g_lpHero->m_pos.x,g_lpHero->m_pos.y);

	if (!lpDroppedItem)
		return;
// 
// #ifdef	_DEBUG
// 	if	(g_bIsDevPC)
// 	{
// 		CG_PICK_ITEM	packet;
// 
// 		packet.base.set(sizeof(CG_PICK_ITEM),dCG_PICK_ITEM);
// 
// 		packet.wCheckSum	=	g_hero.getItemCheckSum();
// 		packet.wItemSerial	=	lpDroppedItem->m_wSerial;
// 
// 		s_agent.sendPacket((char *)&packet,packet.base.wSize);
// 
// 		return;
// 	}
// #endif

	int	iRange	=	GetOvalRange(lpDroppedItem->m_pos.x,lpDroppedItem->m_pos.y,g_lpHero->m_pos.x,g_lpHero->m_pos.y);

	if (iRange	<=	dPICK_ABLE_RANGE_FOR_CLIENT)
	{
		if (m_iLastActionTime+dSEND_HERO_PICK_ITEM_PERIOD	>	s_iFrameCounter	)
			return;

		g_fieldItem.m_iPickItem	=	lpDroppedItem->m_wSerial;

		if (s_agent.sendPickFieldItem())
		{
			m_iLastActionTime	=	s_iFrameCounter;
			m_iBookedPickItem	=	g_fieldItem.m_iPickItem;	//	줍기 예약
		}
	}
}

void
CHero::askDestroyItem(int _iItemIndex)
{
	if	(_iItemIndex	>=	dBORDER_OF_ITEM_AND_EQUIPMENT)
		return;

	cItem	*lpItem	=	g_hero.getInventoryItem(_iItemIndex);
	
	if	(!lpItem)
		return;

	s_iBookedWorkItem	=	_iItemIndex;

	if	(lpItem->getBasicItem()->m_attr.isCanNotDestroyItem)
	{
		g_msgBox.cPopup("",dMSG_CAN_NOT_DESTROY_ITEM,dMSG_OK);
		return;
	}
#ifdef	_FOR_KOREA
	if (lpItem->isExtraItem())
	{
		g_msgBox.cPopup("",dMSG_CAN_NOT_DESTROY_ITEM,dMSG_OK);
		return;
	}
#endif
	g_msgBox.cPopup("",_ms(dMSG_ARE_YOU_REALLY_DESTROY_ITEM_FORM,lpItem->getName()),dMSG_YES,dMSG_NO);
	g_msgBox.setButtonId(eMBI_DESTROY_ITEM);
}

//
//	_iPart에 아이템 착용 준비
BOOL
CHero::sendEquipItem(int _iItemIndex,int _iPart,BOOL _bIsBeltItem)
{
	cItem	*lpItem	=	getInventoryItem(_iItemIndex);

	if (!lpItem)
		return	FALSE;

	int	iType	=	lpItem->getItemType();

	if	(lpItem->isPrincessWeapon())
		iType	=	lpItem->getBasicItem(TRUE)->m_wKind;

	if	(!_bIsBeltItem)
			if(c_aItemEquipPlace[iType] == dITEM_KIND_WASTING || c_aItemEquipPlace[iType] == 0xffff)		//  변경..
			return	FALSE;

	if	(!isUseAbleItem(lpItem,TRUE))
	{
		g_lpHero->warning(dMSG_ITS_NOT_EQUIPABLE_ITEM);
		//	이건 착용 할 수 없는 아이템이여!!!!!1
		return	FALSE;
	}

	if	(!isEquipAbleItem(lpItem,_iPart))
		return	FALSE;

	return	s_agent.sendEquipItem(_iItemIndex,_iPart,_bIsBeltItem);
}

//
//	아이템 버리기 준비
void
CHero::prepareDroppingItem(int _iItemIndex)
{
	m_iBookedDroppingItemSlot	=	_iItemIndex;
}

//
//	아이템 버리기 메시지 전송
BOOL
CHero::sendDroppingItem(int _iItemSlot,int _iCount)
{
	return	s_agent.sendDroppingItem(g_lpHero->m_pos.x+random(40)-20,g_lpHero->m_pos.y+random(40)-20,_iItemSlot,_iCount);
}

//
//	아이템 갯수와 관련 있는 작업 처리
BOOL
CHero::operateBookedWorkWithItemCount(int _iCount)
{
	if (m_iBookedDroppingItemSlot	!=	0xffff)
	{
		if (m_iBookedDroppingItemSlot	==	dITEM_DRAG_GOLD)
		{
			if (g_hero.m_iGold	==	0)
			{
				m_iBookedDroppingItemSlot	=	0xffff;

				return	FALSE;
			}

			if ((int)g_hero.m_iGold	<	_iCount)
				_iCount	=	g_hero.m_iGold;
		}
		else
		if (m_iBookedDroppingItemSlot	<	dBORDER_OF_ITEM_AND_EQUIPMENT)
		{
			if	(m_aItems[m_iBookedDroppingItemSlot].m_bCount < _iCount)
			{
				_iCount	=	m_aItems[m_iBookedDroppingItemSlot].m_bCount;

				if	(_iCount	<=	0)
				{
					m_iBookedDroppingItemSlot	=	0xffff;

					return	FALSE;
				}
			}
		}
		else
		{
			int	iPart	=	m_iBookedDroppingItemSlot-dBORDER_OF_ITEM_AND_EQUIPMENT;

			if	(m_aEquip[iPart].m_bCount < _iCount)
				_iCount	=	m_aItems[iPart].m_bCount;

			if	(_iCount	==	0)
			{
				s_agent.sendStripEquipmentMessage(iPart);			//	장비 벗기
				return	TRUE;
			}
		}

		sendDroppingItem(m_iBookedDroppingItemSlot,_iCount);

		return	TRUE;
	}

	return	FALSE;
}

//
//	아이템 갯수와 관련 있는 작업 취소
void
CHero::cancelBookedWorkWithItemCount()
{
	m_iBookedDroppingItemSlot	=	0xffff;
	
}

void
CHero::askUseSoundOfLeadersBell()
{
	cSTRING	string;

	string.Add(dMSG_PLEASE_CHECK_DOWN_CONTENTS);
	string.Add("\n\n");
	string.Add("- %s ",dMSG_CHARACTER_NAME);

	if	(m_bf1IsDisplayNameWithSoundOfLeadersBell)
		string.Add("<c:LTGREEN>%s<n>",dMSG_DISPLAY);
	else
		string.Add("<c:LTGREEN>%s<n>",dMSG_NOT_DISPLAY);

	string.Add("\n- ");
	string.Add(dMSG_DISPLAY_MESSAGE_FORM,m_strMessageForSoundOfLeadersBell);
	string.Add("\n\n%s",dMSG_DO_YOU_WANT_USE_ITEM);

	g_msgBox.cPopup("",string.String,dMSG_YES,dMSG_NO);
	g_msgBox.setButtonId(eMBI_USE_SOUND_OF_LEADERS_BELL);
}

void
CHero::useSoundOfLeadersBell(BOOL _bIsTest)	//	인도자의 종소리 사용
{
	CG_USE_SOUND_OF_LEADERS_BELL	packet;

	packet.base.set(sizeof(packet),dCG_USE_SOUND_OF_LEADERS_BELL);

	packet.iItemSlot	=	s_iReadyToUseItem;
	packet.wIsDisplayName=	m_bf1IsDisplayNameWithSoundOfLeadersBell;
	packet.wTest		=	_bIsTest;

	strcpy(packet.strMessage,m_strMessageForSoundOfLeadersBell);

	s_agent.sendPacket((char *)&packet,packet.base.wSize);
}

void
CHero::askUseSoundOfLeadersBell(int _iItemSlot)	//	인도자의 종소리 사용
{
	if	(m_bf1IsUseAbleSoundOfLeadersBellStatus	==	FALSE)
	{
		m_strMessageForSoundOfLeadersBell[0]=	NULL;
		s_iReadyToUseItem					=	_iItemSlot;

		useSoundOfLeadersBell(TRUE);

		return;
	}

	m_bf1IsUseAbleSoundOfLeadersBellStatus	=	FALSE;

	cItem	*lpItem	=	getInventoryItem(_iItemSlot);

	if	(!lpItem)
		return;

	cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

	if	(!lpBasicItem				)
		return;

	s_iReadyToUseItem					=	_iItemSlot;
	m_bf1IsReadyToUseSoundOfLeadersBell	=	TRUE;

	EnableChattingBar(TRUE);

	s_inputChatting.setStr(0,dMSG_PLEASE_WRITE_OUPUT_MESSAGE_WITH_SOUND_OF_LEADERS_BELL);
	s_inputChatting.setSelect(0,0,-1);
}

void
CHero::useWorldMapTeleport(int _iItemSlot)
{
	cItem	*lpItem	=	getInventoryItem(_iItemSlot);

	if	(!lpItem)
		return;

	cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

	if	(!lpBasicItem				)
		return;

	if	(g_hero.m_wRestraintTimeByBattle)
	{
		char	strMessage[256];

		int		iRemainTime	=	max(1,g_hero.m_wRestraintTimeByBattle);

		sprintf(strMessage,dMSG_CAN_NOT_USE_GATE_GLOVE_DURING_BATTLE_FORM,iRemainTime);

		WarningMessage(strMessage);
		return;
	}

	if	(g_hero.m_wCurrentField	==	27)
		return;
	if	(g_map.m_bf1IsBlockToWarpField )
		return;
	g_gwWorldMap.open(TRUE,_iItemSlot);

	return;
}

void
CHero::useChangeAllignmentValueItem(int _iItemSlot)
{
	int	iTitleLevel	=	getTitleLevel(eTN_RED_STONE_HUNTER);
	int	iRange		=	15+iTitleLevel;

	g_gwCommonList.reset();

	for (int i=-iRange;i<=iRange;i++)
		g_gwCommonList.addContents(_ms("%d",i),i);

	g_gwCommonList.setOkButtonText(dMSG_SELECT);
	g_gwCommonList.setTitleText(dMSG_PLEASE_SELECT_WANT_VALUE);
	g_gwCommonList.setValue(0,_iItemSlot);

	g_gwCommonList.open(eCLW_SELECT_ALLIGNMENT,FALSE,TRUE);
}

void
CHero::useImproveItemPowerItem(int _iItemSlot)
{
	cItem	*lpItem	=	getInventoryItem(_iItemSlot);

	if	(!lpItem)
		return;

	cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

	if	(!lpBasicItem				)
		return;

	BOOL	bIsUnique	=	lpItem->isExistSpecificEffect(eIE_CH5_IMPROVE_UNIQUE_ITEM_POWER);

	g_gwCommonList.reset();

	int i;

	for (i=0;i<dOWN_ITEM_COUNT+m_bf6ExtraInventorySize;i++)
	{
		cItem	*lpItem	=	getInventoryItem(i);

		if	(!lpItem || lpItem->isExceptionItem() ||  lpItem->isSetItem())
			continue;

		if(c_aItemEquipPlace[lpItem->getItemType()] == dITEM_KIND_WASTING || c_aItemEquipPlace[lpItem->getItemType()] == 0xffff)		//  변경..
			continue;

		if	(bIsUnique)
		{
			if	(!lpItem->isUniqueItem())
				continue;
		}
		else
		{
			if	(lpItem->isUniqueItem())
				continue;
		}

		int	iCount	=	0;

		for (int j=0;j<dITEM_PREFIX_COUNT;j++)
		{
			int	iPrefix	=	lpItem->m_aPrefix[j].m_wPrefix;

			if	(iPrefix==	0xffff)
				continue;

			iCount++;
		}

		if	(iCount	==	0)
			continue;

		g_gwCommonList.addContents(_ms("<c:LTORANGE>%s<n>",lpItem->getBasicItem()->m_strName),i,-1,TRUE);

		for (j=0;j<dITEM_PREFIX_COUNT;j++)
		{
			int	iPrefix	=	lpItem->m_aPrefix[j].m_wPrefix;

			if	(iPrefix==	0xffff)
				continue;

			cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[iPrefix];

			g_gwCommonList.addContents(lpPrefix->m_str1stPrefix,i,j,FALSE);
		}
	}

	if	(g_gwCommonList.m_iCount	==	0)
	{
		WarningMessage(dMSG_THERE_ARE_NOTHING_TO_LEVELUP_ABLE_ITEM);

		return;
	}

	g_gwCommonList.setOkButtonText(dMSG_SELECT);
	g_gwCommonList.setTitleText(dMSG_UPGRADE_ITEM);
	g_gwCommonList.setValue(0,_iItemSlot);

	if	(bIsUnique)
		g_gwCommonList.open(eCLW_SELECT_CH5_UNIQUE_ITEM_PREFIX_UPGRADE,FALSE,TRUE);
	else
		g_gwCommonList.open(eCLW_SELECT_CH5_NORMAL_ITEM_PREFIX_UPGRADE,FALSE,TRUE);
}

void
CHero::useImproveItemPrefixPowerItem(int _iItemSlot)
{
	cItem	*lpItem	=	getInventoryItem(_iItemSlot);

	if	(!lpItem)
		return;

	cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

	if	(!lpBasicItem				)
		return;

	BOOL	bIsUnique	=	lpItem->isExistSpecificEffect(eIE_CH5_UPGRADE_UNIQUE_ITEM_PREFIX_POWER);

	g_gwCommonList.reset();

	int i;

	for (i=0;i<dOWN_ITEM_COUNT+m_bf6ExtraInventorySize;i++)
	{
		cItem	*lpItem	=	getInventoryItem(i);


		if	(!lpItem || lpItem->isExceptionItem() || lpItem->isSetItem())
			continue;

		if(c_aItemEquipPlace[lpItem->getItemType()] == dITEM_KIND_WASTING || c_aItemEquipPlace[lpItem->getItemType()] == 0xffff)		//  변경..
			continue;

		if	(bIsUnique)
		{
			if	(!lpItem->isUniqueItem())
				continue;
		}
		else
		{
			if	(lpItem->isUniqueItem())
				continue;
		}

		int	iCount	=	0;

		for (int j=0;j<dITEM_PREFIX_COUNT;j++)
		{
			int	iPrefix	=	lpItem->m_aPrefix[j].m_wPrefix;

			if	(iPrefix==	0xffff)
				continue;

			int	iNextPrefix;

			if	(!g_itemPrefix.isLevelUpAble(lpItem->getItemType(),iPrefix,&iNextPrefix))
				continue;

			iCount++;
		}

		if	(iCount	==	0)
			continue;

		g_gwCommonList.addContents(_ms("<c:LTORANGE>%s<n>",lpItem->getBasicItem()->m_strName),i,-1,TRUE);

		char strTemp[256];
		strcpy(strTemp , _ms("<c:LTORANGE>%s<n>",lpItem->getBasicItem()->m_strName));

		for (j=0;j<dITEM_PREFIX_COUNT;j++)
		{
			int	iPrefix	=	lpItem->m_aPrefix[j].m_wPrefix;

			if	(iPrefix==	0xffff)
				continue;

			int	iNextPrefix;

			if	(!g_itemPrefix.isLevelUpAble(lpItem->getItemType(),iPrefix,&iNextPrefix))
			{
				g_gwCommonList.addContents(_ms("<c:GRAY>[%s]<n>",g_aItemPrefix[iPrefix].m_str1stPrefix),-1,-1,FALSE);
				continue;
			}

			char	*lpstrText	=	_ms(dMSG_PREFIX_LEVEL_UP_FORM,g_aItemPrefix[iPrefix].m_str1stPrefix,g_aItemPrefix[iNextPrefix].m_str1stPrefix);

			g_gwCommonList.addContents(lpstrText,i,j,FALSE,iNextPrefix);
		}
	}

	if	(g_gwCommonList.m_iCount	==	0)
	{
		WarningMessage(dMSG_THERE_ARE_NOTHING_TO_LEVELUP_ABLE_ITEM);

		return;
	}

	g_gwCommonList.setOkButtonText(dMSG_SELECT);
	g_gwCommonList.setTitleText(dMSG_UPGRADE_ITEM);
	g_gwCommonList.setValue(0,_iItemSlot);

	if	(bIsUnique)
		g_gwCommonList.open(eCLW_SELECT_CH5_UNIQUE_ITEM_PREFIX_LEVEL_UP,FALSE,TRUE);
	else
		g_gwCommonList.open(eCLW_SELECT_CH5_NORMAL_ITEM_PREFIX_LEVEL_UP,FALSE,TRUE);
}

void
CHero::useUpgradeTitleItem(int _iItemSlot)
{
	cItem	*lpItem	=	getInventoryItem(_iItemSlot);

	if	(!lpItem)
		return;

	cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

	if	(!lpBasicItem				)
		return;

	g_gwCommonList.reset();

	int	iTitleLevel	=	getTitleLevel(eTN_RED_STONE_HUNTER);

	for (int i=0;i<dMAX_PLAYER_TITLE_COUNT;i++)
	{
		CPlayerTitleInfo*	lpTitleInfo	=	getTitleByIndex(i);

		if	(!lpTitleInfo)
			continue;

		CTitle*				lpTitle		=	g_titleManager.getTitle(lpTitleInfo->m_bTitle);

		if	(!lpTitle	||	lpTitle->m_bf1ExceptCH5Reward)
			continue;

		if	(lpTitle->m_wSerial == eTN_DEALER && lpTitleInfo->m_bLevel >= 5)
			continue;

		g_gwCommonList.addContents(_ms("%s[LV %d]",lpTitle->m_strName,lpTitleInfo->m_bLevel),lpTitleInfo->m_bTitle);
	}

	int	iCount	=	iTitleLevel/3;

	if	(iTitleLevel %3 )
		iCount++;

	g_gwCommonList.setOkButtonText(dMSG_COMPLETE);
	g_gwCommonList.setBottomText(dMSG_CH5_UPGRADE_TITLE_BOTTOM_FORM);
	g_gwCommonList.setTitleText(dMSG_INCREASE_LEVEL_TITLE);
	g_gwCommonList.setValue(0,iCount);
	g_gwCommonList.setValue(1,_iItemSlot);

	g_gwCommonList.open(eCLW_SELECT_CH5_TITLE_BONUS,TRUE,FALSE);
}

void
CHero::useReleaseReversionItem(int _iItemSlot)
{
	cItem	*lpItem	=	getInventoryItem(_iItemSlot);

	if	(!lpItem)
		return;

	cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

	if	(!lpBasicItem				)
		return;

	g_gwCommonList.reset();

	int i;

	for (i=0;i<dOWN_ITEM_COUNT+m_bf6ExtraInventorySize;i++)
	{
		cItem	*lpItem	=	getInventoryItem(i);

		if	(!lpItem	||	!lpItem->getSpecialItem())
			continue;

		cBasicItem*	lpBasicTargetItem = lpItem->getBasicItem();

		if(lpBasicTargetItem && lpBasicTargetItem->m_bf1IsNotApplyReversionEffect)
			continue;

		if	(lpItem->getSpecialItem()->m_bf1IsReversionItem	==	FALSE)
			continue;

		if	(lpItem->getSpecialItem()->m_bf1IsFreeUseItem	==	TRUE)	//	새벽의 봉인구 사용 아이템
			continue;		

		g_gwCommonList.addContents(lpItem->getName(),i);
	}

	g_gwCommonList.setOkButtonText(dMSG_LETS_TRANS);
	g_gwCommonList.setTitleText(dMSG_SELECT_UNSEAL_ITEM);
	g_gwCommonList.setValue(0,_iItemSlot);

	g_gwCommonList.open(eCLW_SELECT_CH5_RELEASE_REVERSION,FALSE,TRUE);

}


void
CHero::useCH5ReverseStatePointItem(int _iItemSlot)
{
	cItem	*lpItem	=	getInventoryItem(_iItemSlot);

	if	(!lpItem)
		return;

	cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

	if	(!lpBasicItem				)
		return;

	g_gwCommonList.reset();

	int i,iCount=0;


	for (i=0;i<7;i++)
	{
		int	iCurrentPoint	=	0;
		int	iAblePoint		=	getResetAbleStatePoint(i,&iCurrentPoint);

		if	(iAblePoint	==	0)
			continue;

		g_gwCommonList.addContents(_ms(dMSG_RESET_STATE_POINT_FORM,g_pstrEquipUseLimitContents[i+1],iCurrentPoint,iCurrentPoint-iAblePoint),
									i,iAblePoint);
		iCount++;
	}

	if	(iCount	==	0)
	{
		WarningMessage(dMSG_THERE_ARE_NOTHING_TO_RESET_ABLE_STATE);

		return;
	}

	g_gwCommonList.setOkButtonText(dMSG_OK);
	g_gwCommonList.setTitleText(dMSG_SELECT_RESET_STATE);
	g_gwCommonList.setValue(0,_iItemSlot);

	g_gwCommonList.open(eCLW_SELECT_CH5_REVERSE_STATE_POINT,FALSE,TRUE);
}

void
CHero::useCH5ReverseSkillPointItem(int _iItemSlot)
{
	cItem	*lpItem	=	getInventoryItem(_iItemSlot);

	if	(!lpItem)
		return;

	cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

	if	(!lpBasicItem				)
		return;

	g_gwCommonList.reset();

	int i,iCount=0;

	for (i=0;i<dABILITY_COUNT;i++)
	{
		if	(m_aAbility[i].m_wLevel	==	0)
			continue;

		int	iAbleLevel		=	0;
		int	iAblePoint		=	getResetAbleSkillPoint(i,&iAbleLevel);

		if	(iAblePoint	==	0)
			continue;

		g_gwCommonList.addContents(_ms(dMSG_RESET_SKILL_POINT_FORM,m_aAbility[i].getSkill()->m_strName,
								m_aAbility[i].m_wLevel,iAbleLevel),i,iAblePoint);
		iCount++;
	}

	if	(iCount	==	0)
	{
		WarningMessage(dMSG_THERE_ARE_NOTHING_TO_RESET_ABLE_SKILL);

		return;
	}

	g_gwCommonList.setOkButtonText(dMSG_OK);
	g_gwCommonList.setTitleText(dMSG_SELECT_RESET_SKILL);
	g_gwCommonList.setValue(0,_iItemSlot);

	g_gwCommonList.open(eCLW_SELECT_CH5_REVERSE_SKILL_POINT,FALSE,TRUE);
}

void
CHero::askUseChainOfRestraint(int _iItemSlot)	//	리플렉팅 믹서 사용 요청
{
	cMiniPet	*lpSelectedMiniPet	=	iMiniPet()->getMiniPet(g_gwMiniPetStatus.m_iSelectMiniPet);

	if	(!lpSelectedMiniPet || lpSelectedMiniPet->m_bf7Level == 0	||	g_config.m_bf1IsShowMiniPetWindow	==	FALSE)
	{
		g_msgBox.cPopup("",dMSG_PLEASE_SELECT_MINIPET_AND_RETRY,dMSG_OK);

		return;
	}

	if	(g_gwMiniPetStatus.m_iSelectMinipetCount	>	1)
	{
		g_msgBox.cPopup("",dMSG_PLEASE_SELECT_ONLY_ONE_MINIPET,dMSG_OK);

		return;
	}

	if	(g_gwMiniPetStatus.isIncludeActiveMiniPetInSelectedList())
	{
		g_msgBox.cPopup("",dMSG_PLEASE_DISACTIVE_SELECTED_MINIPET_FOR_RESTRAINT,dMSG_OK);

		return;
	}

	int		iEatCount;
	char	*lpstrState;

	switch(lpSelectedMiniPet->getAttr())
	{
		case	nsMiniPetType::SpritOfFire		:
			iEatCount	=	m_bf4EatFireMiniPetCount;
			lpstrState	=	g_alpstrStateName[eSTATE_STRENGTH	];
			break;
		case	nsMiniPetType::SpritOfWater		:
			iEatCount	=	m_bf4EatWaterMiniPetCount;
			lpstrState	=	g_alpstrStateName[eSTATE_WISDOM	];
			break;
		case	nsMiniPetType::SpritOfWind		:
			iEatCount	=	m_bf4EatWindMiniPetCount;
			lpstrState	=	g_alpstrStateName[eSTATE_AGILITY	];
			break;
		case	nsMiniPetType::SpritOfEarth		:
			iEatCount	=	m_bf4EatEarthMiniPetCount;
			lpstrState	=	g_alpstrStateName[eSTATE_CONSTITUTION	];
			break;
		case	nsMiniPetType::SpritOfLight		:
			iEatCount	=	m_bf4EatLightMiniPetCount;
			lpstrState	=	g_alpstrStateName[eSTATE_INTELLIGENCE	];
			break;
		case	nsMiniPetType::SpritOfDark		:
			iEatCount	=	m_bf4EatDarkMiniPetCount;
			lpstrState	=	g_alpstrStateName[eSTATE_CHARISMA	];
			break;
	}

	if	(iEatCount	>=	dLIMIT_EAT_SAME_TYPE_MINIPET_COUNT)
	{
		g_msgBox.cPopup("",_ms(dMSG_CAN_NOT_RESTRAINT_MORE_MINIPET_WITH_THAT_KIND_FORM,dLIMIT_EAT_SAME_TYPE_MINIPET_COUNT),dMSG_OK);

		return;
	}

	s_iReadyToUseItem	=	_iItemSlot;

	char	*lpstrName	=	g_aMiniPetTypeInfo[lpSelectedMiniPet->m_bf5Type].m_strName;
	int		iLevel		=	lpSelectedMiniPet->m_bf7Level;

	g_msgBox.cPopup("",_ms(dMSG_ARE_YOU_REALLY_RESTRAINT_MINIPET_FORM,iLevel,lpstrName,lpstrState,dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET),dMSG_YES,dMSG_NO);
	g_msgBox.setButtonId(eMBI_RESTRAINT_MINIPET);
}

void
CHero::askUseReflectingMixer(int _iItemSlot)	//	리플렉팅 믹서 사용 요청
{
	cItem	*lpItem	=	getInventoryItem(_iItemSlot);

	if	(!lpItem)
		return;

	int	iRequireMiniPetCount	=	lpItem->m_aOption[0];

	if	(g_gwMiniPetStatus.m_iSelectMinipetCount	!=	iRequireMiniPetCount)
	{
		g_msgBox.cPopup("",dMSG_PLEASE_SELECT_MANY_MINIPET_WITH_SHIFT,dMSG_OK);

		return;
	}

	if	(g_gwMiniPetStatus.isIncludeActiveMiniPetInSelectedList())
	{
		g_msgBox.cPopup("",dMSG_PLEASE_DISACTIVE_SELECTED_MINIPET,dMSG_OK);

		return;
	}

	cSTRING	string;

	for (int i=0;i<g_gwMiniPetStatus.m_iSelectMinipetCount;i++)
	{
		cMiniPet	*lpMiniPet	=	iMiniPet()->getMiniPet(g_gwMiniPetStatus.m_aiMultiSelect[i]);

		string.Add("[Lv %d] %s\n",lpMiniPet->m_bf7Level,g_aMiniPetTypeInfo[lpMiniPet->m_bf5Type].m_strName);
	}

	string.Add("\n %s",dMSG_CONFIRM_MIX_MINIPET);

	s_iReadyToUseItem					=	_iItemSlot;

	g_msgBox.cPopup("",string.String,dMSG_YES,dMSG_NO);
	g_msgBox.setButtonId(eMBI_MIX_MINIPET);
}

//
//	아이템 사용
BOOL
CHero::askUseItem(int _iItemSlot)
{
	cItem	*lpItem	=	getInventoryItem(_iItemSlot);

	if	(!lpItem)
		return	FALSE;

	cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

	if	(!lpBasicItem				)
		return	FALSE;
	if	(!isConsumeItem(_iItemSlot)	)
		return	FALSE;

	if(_iItemSlot	<	dOWN_ITEM_COUNT+g_hero.m_bf6ExtraInventorySize)
	{
		if(lpBasicItem->m_bf1IsUseAfterWear)
			return FALSE;
	}
	if	(lpItem->getCoolTime(lpBasicItem))
	{
		g_lpHero->warning(dMSG_WARNING_ITEM_COOLTIME);
		return	FALSE;
	}

	if	(lpItem->isExistSpecificEffect(eIE_CHAIN_OF_RESTRAINT))
	{
		askUseChainOfRestraint(_iItemSlot);

		return	FALSE;
	}
	else
	if	(lpItem->isExistSpecificEffect(eIE_REFLECTING_MIXER))
	{
		askUseReflectingMixer(_iItemSlot);

		return	FALSE;
	}
	else
	if	(lpItem->isExistSpecificEffect(eIE_SOUND_OF_LEADERS_BELL))
	{
		askUseSoundOfLeadersBell(_iItemSlot);

		return	FALSE;
	}
	else
	if	(lpItem->isExistSpecificEffect(eIE_USE_WORLD_MAP_TELEPORT))
	{
		useWorldMapTeleport(_iItemSlot);

		return	FALSE;
	}
	else
	if	(lpItem->isExistSpecificEffect(eIE_CH5_SELECT_ALLIGNMENT))
	{
		useChangeAllignmentValueItem(_iItemSlot);

		return	FALSE;
	}
	else
	if	(lpItem->isExistSpecificEffect(eIE_CH5_IMPROVE_NORMAL_ITEM_POWER)	||	
		lpItem->isExistSpecificEffect(eIE_CH5_IMPROVE_UNIQUE_ITEM_POWER)		)
	{
		useImproveItemPowerItem(_iItemSlot);

		return	FALSE;
	}
	else
	if	(lpItem->isExistSpecificEffect(eIE_CH5_UPGRADE_NORMAL_ITEM_PREFIX_POWER)	||	
		lpItem->isExistSpecificEffect(eIE_CH5_UPGRADE_UNIQUE_ITEM_PREFIX_POWER)		)
	{
		useImproveItemPrefixPowerItem(_iItemSlot);

		return	FALSE;
	}
	else
	if	(lpItem->isExistSpecificEffect(eIE_CH5_SELECT_UPGRADE_TITLE))
	{
		useUpgradeTitleItem(_iItemSlot);

		return	FALSE;
	}
	else
	if	(lpItem->isExistSpecificEffect(eIE_CH5_RELASE_REVERSION))
	{
		useReleaseReversionItem(_iItemSlot);

		return	FALSE;
	}
	else
	if	(lpItem->isExistSpecificEffect(eIE_CH5_REVERSE_STATE_POINT))
	{
		useCH5ReverseStatePointItem(_iItemSlot);

		return	FALSE;
	}
	else
	if	(lpItem->isExistSpecificEffect(eIE_CH5_REVERSE_SKILL_POINT))
	{
		useCH5ReverseSkillPointItem(_iItemSlot);

		return	FALSE;
	}


	if(setItemUseToTarget(lpItem,_iItemSlot))
		return FALSE;

	for (int iState=0;iState<dSTATE_TYPE_COUNT;iState++)
	{
		if	(lpItem->isExistSpecificEffect(eIE_RESET_STRENGTH_STATE+iState))
		{
			g_msgBox.cPopup("",_ms(dMSG_REALLY_RESET_ONE_STATE_POINT_FORM,g_alpstrStateName[iState]),dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_USE_ITEM);
			
			s_iReadyToUseItem	=	_iItemSlot;

			return	TRUE;
		}
	}

	if	(lpBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_RESET_SKILL_POINT)
	{
		g_msgBox.cPopup("",dMSG_REALLY_RESET_SKILL_POINT,dMSG_YES,dMSG_NO);
		g_msgBox.setButtonId(eMBI_USE_ITEM);

		s_iReadyToUseItem	=	_iItemSlot;

		return	TRUE;
	}

	if	(lpBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_RESET_STATE_POINT)
	{
		g_msgBox.cPopup("",dMSG_REALLY_RESET_STATE_POINT,dMSG_YES,dMSG_NO);
		g_msgBox.setButtonId(eMBI_USE_ITEM);

		s_iReadyToUseItem	=	_iItemSlot;

		return	TRUE;
	}

	if	(lpBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_SELECT_ITEM_IN_ITEM_PACK)	//	선택 가능 아이템 팩
	{
		s_agent.sendEtcWork(eCEW_ASK_ITEM_LIST_IN_ITEM_PACK,_iItemSlot);

		return	TRUE;
	}

	if	(lpItem->isPolisher())
	{
		s_iReadyToUseItem	=	_iItemSlot;
		g_msgBox.cPopup("",dMSG_REALLY_USE_POLISHER,dMSG_YES,dMSG_NO);
		g_msgBox.setButtonId(eMBI_USE_POLISHER);
		
		return	TRUE;
	}

	if	(lpItem->isMassivePolisher())
	{
		s_iReadyToUseItem	=	_iItemSlot;
		g_msgBox.cPopup("",dMSG_REALLY_USE_MASSIVE_POLISHER,dMSG_YES,dMSG_NO);
		g_msgBox.setButtonId(eMBI_USE_MASSIVE_POLISHER);
		
		return	TRUE;
	}


	//	08-12-29	미니펫 3차 각성.	JBC
	if	(lpItem->isMiniPetAwakenItem50())
	{
		cMiniPet	*lpMiniPet	=	&g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet];

		if(lpMiniPet->m_bf7Level == 0)
			return FALSE;

		char strComment[512];
		ZeroMemory(strComment , 512);
		sprintf(strComment , "%s\n%s <c:LTYELLOW>[%s]<n>" , dMSG_REALLY_USE_AWAKEN50 , dMSG_TARGET_MINI_PET  , lpMiniPet->getName());
		s_iReadyToUseItem	=	_iItemSlot;
		g_msgBox.cPopup("",strComment,dMSG_YES,dMSG_NO);
		g_msgBox.setButtonId(eMBI_USE_AWAKEN_ITME50);
		
		return	TRUE;
	}

	//	08-12-29	미니펫 3차 각성.	JBC
	if	(lpItem->isMiniPetAwakenItem100())
	{
		cMiniPet	*lpMiniPet	=	&g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet];
		
		if(lpMiniPet->m_bf7Level == 0)
			return FALSE;

		char strComment[512];
		ZeroMemory(strComment , 512);
		sprintf(strComment , "%s\n%s <c:LTYELLOW>[%s]<n>" , dMSG_REALLY_USE_AWAKEN100 , dMSG_TARGET_MINI_PET  , lpMiniPet->getName());
		s_iReadyToUseItem	=	_iItemSlot;
		g_msgBox.cPopup("",strComment,dMSG_YES,dMSG_NO);
		g_msgBox.setButtonId(eMBI_USE_AWAKEN_ITME100);
		
		return	TRUE;
	}

	if	(lpItem->checkFirstEffect(eIE_CHANGE_MINIPET_NAME))
	{
		if	(g_gwMiniPetStatus.m_iSelectMinipetCount	>	1)
		{
			g_msgBox.cPopup("",dMSG_PLEASE_SELECT_ONLY_ONE_MINIPET,dMSG_OK);
			return	TRUE;
		}

		s_iReadyToUseItem	=	_iItemSlot;
		g_msgBox.cPopup("",dMSG_DO_YOU_WANT_CHANGE_SELECT_MINI_PET_NAME,dMSG_YES,dMSG_NO);
		g_msgBox.setButtonId(eMBI_CHANGE_MINIPET_NAME);

		return	TRUE;
	}
	if( lpItem->isExistSpecificEffect(eIE_MINIPET_FEED))
	{
		if	(g_gwMiniPetStatus.m_iSelectMinipetCount	>	1)
		{
			g_msgBox.cPopup("",dMSG_PLEASE_SELECT_ONLY_ONE_MINIPET,dMSG_OK);
			return	TRUE;
		}
		if	(g_gwMiniPetStatus.m_iSelectMiniPet	>=	c_iMiniPetCount	||	g_gwMiniPetStatus.m_iSelectMiniPet	<	0	)
			return TRUE;
		
		cMiniPet	*lpMiniPet	=	&g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet];
		
		if	(lpMiniPet->m_bf7Level	==	0 || lpMiniPet->m_bf7Level>= 100)
		{
			g_msgBox.cPopup("",dMSG_CAN_NOT_USE_ITEM,dMSG_OK);
			return TRUE;
		}

		int iType	=	lpItem->getEffectMiniPetFeedType();
		int iExp	=	lpItem->getMiniPetFeedExp();

		if	(g_hero.m_iLevel < lpMiniPet->m_bf7Level )
		{
			g_msgBox.cPopup("",dMSG_CAN_NOT_USE_ITEM,dMSG_OK);
			return TRUE;
		}

		int	iTempExp	=	0;

		for(int i=lpMiniPet->m_bf7Level; i<=g_hero.m_iLevel && i < 100; ++i)
		{
			iTempExp	+=	g_aiExpTableForMiniPet[i];
		}

		iTempExp	=	(lpMiniPet->m_bf16Exp + iExp * 10000) - iTempExp;
		if(iTempExp > 0xffff)	
		{
			g_msgBox.cPopup("",dMSG_CAN_NOT_USE_ITEM,dMSG_OK);
			return TRUE;
		}

		char strComment[512] = "";


		switch(iType)
		{
			case nsMiniPetStyle::Bio	:
				sprintf(strComment , dMSG_FORM_FEED_EFFECT_ITEM_TO_MINIPET , dMSG_MINIPET_BIO , iExp , lpMiniPet->getName());
				break;
			case nsMiniPetStyle::Energy	:
				sprintf(strComment , dMSG_FORM_FEED_EFFECT_ITEM_TO_MINIPET , dMSG_MINIPET_ENERGY , iExp , lpMiniPet->getName());
				break;
			case nsMiniPetStyle::Mineral	:
				sprintf(strComment , dMSG_FORM_FEED_EFFECT_ITEM_TO_MINIPET , dMSG_MINIPET_MINERAL , iExp , lpMiniPet->getName());
				break;
			default :
				return TRUE;
		}

		
		s_iReadyToUseItem	=	_iItemSlot;
		g_msgBox.cPopup("", strComment,dMSG_YES,dMSG_NO);
		g_msgBox.setButtonId(eMBI_MINIPET_FEED);
		return TRUE;
	}


	if( lpItem->isExistSpecificEffect(eIE_RESET_MINIPET_SKILL))
	{
		if	(g_gwMiniPetStatus.m_iSelectMinipetCount	>	1)
		{
			g_msgBox.cPopup("",dMSG_PLEASE_SELECT_ONLY_ONE_MINIPET,dMSG_OK);
			return	TRUE;
		}
		if	(g_gwMiniPetStatus.m_iSelectMiniPet	>=	c_iMiniPetCount	||	g_gwMiniPetStatus.m_iSelectMiniPet	<	0	)
			return TRUE;
		
		cMiniPet	*lpMiniPet	=	&g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet];
		
		if	(lpMiniPet->m_bf7Level	==	0)
			return TRUE;

		char strComment[512] = "";
		sprintf(strComment,dMSG_FORM_RESET_MINIPET_SKILL, lpMiniPet->getName());
		s_iReadyToUseItem	=	_iItemSlot;
		
		g_msgBox.cPopup("", strComment,dMSG_YES,dMSG_NO);
		g_msgBox.setButtonId(eMBI_RESET_MINIPET_SKILL);

		return TRUE;
	}
	if( lpItem->isExistSpecificEffect(eIE_SET_MINIPET_TYPE))
	{
		if	(g_gwMiniPetStatus.m_iSelectMinipetCount	>	1)
		{
			g_msgBox.cPopup("",dMSG_PLEASE_SELECT_ONLY_ONE_MINIPET,dMSG_OK);
			return	TRUE;
		}
		if	(g_gwMiniPetStatus.m_iSelectMiniPet	>=	c_iMiniPetCount	||	g_gwMiniPetStatus.m_iSelectMiniPet	<	0	)
			return TRUE;
		
		cMiniPet	*lpMiniPet	=	&g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet];
		
		if	(lpMiniPet->m_bf7Level	==	0)
			return TRUE;
		int iType	=	lpItem->getSetMiniPetType();
		char strComment[512] = "";
		switch(iType)
		{
			case nsMiniPetStyle::Bio	:
				sprintf(strComment , dMSG_CHANGE_MINIPET_TYPE ,lpMiniPet->getName() , dMSG_MINIPET_BIO );
				break;
			case nsMiniPetStyle::Energy	:
				sprintf(strComment , dMSG_CHANGE_MINIPET_TYPE , lpMiniPet->getName(), dMSG_MINIPET_ENERGY );
				break;
			case nsMiniPetStyle::Mineral	:
				sprintf(strComment , dMSG_CHANGE_MINIPET_TYPE , lpMiniPet->getName(), dMSG_MINIPET_MINERAL );
				break;
			default :
				return TRUE;
		}

		if(lpMiniPet->m_iEnergyPoint + lpMiniPet->m_iMineralPoint + lpMiniPet->m_iVitalPoint == 0)
		{
			g_msgBox.cPopup("", dMSG_CAN_NOT_USE_TO_MINIPET_HAVE_NOT_EXP,dMSG_YES);
			return TRUE;
		}
		s_iReadyToUseItem	=	_iItemSlot;
		g_msgBox.cPopup("", strComment,dMSG_YES,dMSG_NO);
		g_msgBox.setButtonId(eMBI_SET_MINIPET_TYPE);
		return TRUE;
	}

	return	s_agent.sendUseItemToCharacter(_iItemSlot,0xffff);
}


BOOL
CHero::setItemUseToTarget(cItem* _lpItem , int _iItemSlot)
{
	cBasicItem* lpBasicItem = _lpItem->getBasicItem();
	if(!lpBasicItem)
		return FALSE;


	if	(lpBasicItem->m_attr.isUseToItem)
	{
		s_iReadyToUseItem	=	_iItemSlot;

		s_propose.m_work.isReadyUseItemToItem		=	TRUE;
		cINPDEV::SetExclusive(eMOUSE_STATE_PROPOSE_TO_WORK);

		return TRUE;
	}
	else
	if	(lpBasicItem->m_attr.isUseToCharacter)
	{
		s_iReadyToUseItem	=	_iItemSlot;

		s_propose.m_work.isReadyUseItemToCharacter	=	TRUE;
		cINPDEV::SetExclusive(eMOUSE_STATE_PROPOSE_TO_WORK);

		return TRUE;
	}
	else
	if	(lpBasicItem->m_attr.isUseToCorpse)
	{
		s_iReadyToUseItem	=	_iItemSlot;

		s_propose.m_work.isReadyUseItemToCorpse	=	TRUE;
		cINPDEV::SetExclusive(eMOUSE_STATE_PROPOSE_TO_WORK);

		return TRUE;
	}
	else if(lpBasicItem->m_bf1IsUseToEnemy)
	{
		s_iReadyToUseItem	=	_iItemSlot;

		s_propose.m_work.isReadyUseItemToEnemy	=	TRUE;
		cINPDEV::SetExclusive(eMOUSE_STATE_PROPOSE_TO_WORK);

		return TRUE;


	}

	return FALSE;

}
//
//	대미지 접두사를 바꿔주는 이벤트 접두사 Delux!!
BOOL
CHero::changeDamagePrefix20031117(cItem *_lpItem,cSTRING *_lpString)
{
	return	TRUE;
	/*
	int			i;
	cBasicItem	*lpBasicItem	=	_lpItem->getBasicItem();
	BOOL		bIsOK			=	TRUE;

	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		int	iPrefix	=	_lpItem->m_aPrefix[i].m_wPrefix;
		int	iChangePrefix;

		if(iPrefix	==	0xffff)	break;

		if (iPrefix < 202 || iPrefix > 261)
			if (iPrefix < 468 || iPrefix > 471)
				continue;

		iChangePrefix	=	91 + ((iPrefix-2)%10);

		if (iPrefix >= 468	&& iPrefix <= 471	)	iChangePrefix	=	459;

		cITEM_PREFIX	*lpChangePrefix	=	&g_aItemPrefix[iChangePrefix];
		cITEM_PREFIX	*lpPrefix		=	&g_aItemPrefix[iPrefix];

		if	(lpChangePrefix->m_aStickableItem[lpBasicItem->m_wKind]	==	FALSE)
		{
			int	iPrefixLevel			=	(iChangePrefix-91)%10;
			iChangePrefix				-=	iPrefixLevel;
			iPrefixLevel--;
			lpChangePrefix				=	NULL;
			bIsOK						=	FALSE;

			for (;iPrefixLevel >= 0;iPrefixLevel--)
			{
				cITEM_PREFIX	*lpNewPrefix=	&g_aItemPrefix[iChangePrefix+iPrefixLevel];

				if	(lpNewPrefix->m_aStickableItem[lpBasicItem->m_wKind]	==	TRUE)
				{
					iChangePrefix	=	iChangePrefix+iPrefixLevel;
					lpChangePrefix	=	lpNewPrefix;

					_lpString->Add("<c:LTYELLOW>%s<n>옵션이 <c:LTYELLOW>%s<n>옵션으로 하향 조정됩니다.\n",lpPrefix->m_str1stPrefix,lpChangePrefix->m_str1stPrefix);
					break;
				}
			}

			if (lpChangePrefix	==	NULL)
			{
				_lpString->Add("<c:LTYELLOW>%s<n>옵션은 <c:LTYELLOW>%s<n>에 붙을 수 없습니다.\n",lpPrefix->m_str1stPrefix,lpBasicItem->m_strName);

				continue;
			}
		}
	}
	return	bIsOK;
*/
}

//
//	아이템 사용
BOOL
CHero::askUseItemToItem(int _iItemSlot,int _iTargetItem)
{
	if (_iTargetItem < 0 || _iTargetItem >= getItemSlotCount())
	{
		AddSystemMessage(LTYELLOW,dMSG_ITEM_USE_TO_ITEM_COMMENT);
		return	FALSE;
	}

	cItem		*lpItem			=	getItem(_iItemSlot);
	cItem		*lpTargetItem	=	getItem(_iTargetItem);

	if	(!lpItem || !lpTargetItem)
		return	FALSE;

	m_wUseItem		=	_iItemSlot;
	m_wTargetItem	=	_iTargetItem;

	if	(lpItem->isExistSpecificEffect(eIE_SEAL_SPHERE_OF_DAWN))	//	새벽의 봉인구
	{
		if	(g_hero.m_bf4RebirthCount	==	0)	//	전생자를 위한 아이템이다.
		{
			g_msgBox.cPopup("",_ms(dMSG_EXLCUSIVE_TO_REBIRTH_CHARACTER_ITEM_FORM,lpItem->getName()),dMSG_OK);

			return	FALSE;
		}

		if	(g_hero.m_bf4UseSealSphereOfDawnCount	>=	g_hero.m_bf4RebirthCount)	//	새벽의 봉인구는 환생 횟수만큼만 사용 가능하다.
		{
			g_msgBox.cPopup("",dMSG_LIMIT_OF_SEAL_SPHERE_OF_DAWN,dMSG_OK);

			return	FALSE;
		}

		cBasicItem	*lpBasicItem	=	lpTargetItem->getBasicItem();

		if	(c_aItemEquipPlace[lpBasicItem->m_wKind] == dITEM_KIND_WASTING || c_aItemEquipPlace[lpBasicItem->m_wKind] == 0xffff	||	lpTargetItem->isExceptionItem()	||	lpTargetItem->isExtraItem()||
			lpTargetItem->m_isBroken|| lpTargetItem->isLimitTermItem())	// 변경.
		{
			g_msgBox.cPopup("",dMSG_FAILED_TO_ITEM_ITS_NOT_EQUIPMENT,dMSG_OK);

			return	FALSE;	//	장비만 봉인구를 사용 할 수 있다.
		}

		if	(lpTargetItem->isEnableJob(g_hero.m_wJob)==FALSE)
		{
			g_msgBox.cPopup("",dMSG_CAN_NOT_EQUIP_ITEM_WITH_CURRENT_JOB,dMSG_OK);

			return	FALSE;	//	장비만 봉인구를 사용 할 수 있다.
		}

		if	(lpTargetItem->getSpecialItem())
		{
			if	(lpTargetItem->getSpecialItem()->m_bf1IsFreeUseItem)
			{
				g_msgBox.cPopup("",dMSG_ALREADY_OWN_THAT_EFFECT_ITEM,dMSG_OK);
				return	FALSE;
			}
		}

		int	iAbleLevel	=	lpItem->m_aOption[0];
		iAbleLevel		*=	10;

		if	(!lpTargetItem->isCanUseLevel(iAbleLevel))
		{
			g_msgBox.cPopup("",dMSG_FAILE_BY_TARGET_ITEMS_LEVEL,dMSG_OK);

			return	FALSE;
		}

		cSTRING	strMessage;
		char	strSecondItemName[128];

		strcpy(strSecondItemName,lpTargetItem->getName());

		strMessage.Add(dMSG_USE_ITEM_TO_ITEM_FORM,lpItem->getName(),strSecondItemName);
		strMessage.Add("\n\n%s\n\n%s",dMSG_WARNING_FOR_SEAL_SHPERE_OF_DAWN,dMSG_DO_YOU_WANT_USE_THIS_ITEM);

		g_msgBox.cPopup("",strMessage.String,dMSG_YES,dMSG_NO);

		g_msgBox.setButtonId(eMBI_USE_ITEM_TO_ITEM,eMBI_CANCEL);

		return	TRUE;
	}

	if	(lpItem->isExistSpecificEffect(eIE_CH5_COPY_ITEM))
	{
		cBasicItem	*lpBasicItem	=	lpTargetItem->getBasicItem();

		if	(c_aItemEquipPlace[lpBasicItem->m_wKind] == dITEM_KIND_WASTING || c_aItemEquipPlace[lpBasicItem->m_wKind] == 0xffff	||	lpTargetItem->isExceptionItem()	||	lpTargetItem->isExtraItem()||	lpTargetItem->m_isBroken	||
			lpBasicItem->isEternalItem() || lpBasicItem->m_bf1IsRingOfInfinity || lpItem->getCarvingLevel())
		{	// 변경.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_FAILED_TO_ITEM_ITS_NOT_EQUIPMENT);

			return	FALSE;	//	장비만 복사되
		}
		if(lpBasicItem->m_bf1IsNotApplyCopyEffect)
		{	
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_USE_ITEM_ON_DEST_ITEM);
			return	FALSE;	
		}
		CPlayerTitleInfo	*lpTitleInfo	=	g_hero.getTitleByIndex(eTN_RED_STONE_HUNTER);

		if	(!lpTitleInfo)
			return	FALSE;	//	장비만 복사되
		
		float	fLevel	=	lpTitleInfo->m_bLevel;
		int		iChance	=	(int)(33.0f+fLevel*1.5f);

		char	strText[512];

		sprintf(strText,dMSG_DO_YOU_WANT_COPY_THIS_ITEM_FORM,lpTargetItem->getName(),iChance,dMSG_PERCENTAGE);

		g_msgBox.cPopup("",strText,dMSG_YES,dMSG_NO);

		g_msgBox.setButtonId(eMBI_USE_ITEM_TO_ITEM,eMBI_CANCEL);

		return	TRUE;
	}
	if	(lpItem->checkFirstEffect(eIE_REMOVE_PREFIX) || lpItem->checkFirstEffect(eIE_CH5_REMOVE_PREFIX) )
		return	s_agent.sendUseItemToItem(_iItemSlot,_iTargetItem);

	if	(lpItem->isExistSpecificEffect(eIE_ATTACH_PREFIX) ||lpItem->isExistSpecificEffect(eIE_ATTACH_PREFIX_CAN_NOT_TRADE) )
	{	// 접두사 붙이기
		cBasicItem*	lpTargetBasic = lpTargetItem->getBasicItem();

		if(!lpTargetBasic)
			return FALSE;
		if(!lpTargetBasic->isEquipmentItem())
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UEUIR_CAN_NOT_ATTACH_THAT_PREFIX_ITEM);
			return FALSE;
		}
		if(lpTargetItem->getPrefixCount() == dITEM_PREFIX_COUNT)
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UJMER_CAN_NOT_ATTACH_PREFIX_BY_PREFIX_FULL);
			return FALSE;
		}

		char	strText[512];

		cItem	item;
		
		item.copy(lpTargetItem);

		for(int i=0;i<dITEM_PREFIX_COUNT;++i)
		{
			if(item.m_aPrefix[i].m_wPrefix == 0xffff)
			{
				memcpy(&item.m_aPrefix[i],&lpItem->m_aPrefix[0],sizeof(CItemPrefixInfo));
				break;
			}
		}
		CItemPrefixInfo	*lpItemPrefixInfo	=	&lpItem->m_aPrefix[0];
		cITEM_PREFIX	*lpPrefix			=	&g_aItemPrefix[lpItemPrefixInfo->m_wPrefix];
		char			*lpstrComment		=	g_itemPrefix.getComment(lpItemPrefixInfo);

		sprintf(strText,dMSG_CONFIRM_COPY_PREFIX_FORM,lpPrefix->m_str1stPrefix,lpstrComment,item.getRequireLevel());

		g_msgBox.cPopup("",strText,dMSG_YES,dMSG_NO);

		g_msgBox.setButtonId(eMBI_USE_ITEM_TO_ITEM,eMBI_CANCEL);

		return TRUE;

	}

	int	iSuccessChance1	=	getJimMorysEnchantServiceSuccessChance(lpItem);
	int	iSuccessChance2	=	getEnchantItemSuccessChanceByUniqueInfo(lpItem);
	int	iSuccessChance	=	0xffffffff;

	if (iSuccessChance1	!=	0xffffffff)
		iSuccessChance	=	iSuccessChance1;
	if (iSuccessChance2	!=	0xffffffff)
		iSuccessChance	=	iSuccessChance2;

	if	(lpItem->getBasicItem()->m_aGenerateData[0].m_wEffect == eIE_UNSKILLED_REPAIR_BROKEN_ITEM)
	{
		if	(lpTargetItem->getPrefixCount() >= 1)
		{
			g_msgBox.cPopup(dMSG_REPAIR_ITEM,dMSG_ARE_YOU_REALLY_REPAIR_BROKEN_ITEM_BY_UNSKILLED_REPAIR_KIT,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_ACCEPT_ENCHANT_ITEM,eMBI_CANCEL);

			return	TRUE;
		}
	}

	if	(iSuccessChance	!=	0xffffffff)
	{
		if	(lpTargetItem->getBasicItem()->m_wKind	==	eIK_RING && iSuccessChance2 != 0xffffffff)
			g_msgBox.cPopup(dMSG_IMPROVE_ITEM,_ms(dMSG_ARE_YOU_ENCHANT_RING_ITEM_FORM,iSuccessChance/100,iSuccessChance%100),dMSG_YES,dMSG_NO);
		else
			g_msgBox.cPopup(dMSG_IMPROVE_ITEM,_ms(dMSG_ARE_YOU_ENCHANT_ITEM_FORM,iSuccessChance/100,iSuccessChance%100),dMSG_YES,dMSG_NO);

		g_msgBox.setButtonId(eMBI_ACCEPT_ENCHANT_ITEM,eMBI_CANCEL);

		return	TRUE;
	}
	if	(lpItem->isExistSpecificEffect(eIE_INCREASE_CARVING_LEVEL))
	{
		return 	askIncreaseCarvingLevel(lpItem,lpTargetItem);
	}
	if( lpItem->isExistSpecificEffect(eIE_SET_CARVING_LEVEL))
	{
		return askSetCarvingLevel(lpItem,lpTargetItem);
	}
	return	s_agent.sendUseItemToItem(_iItemSlot,_iTargetItem);
}

BOOL
CHero::askIncreaseCarvingLevel(cItem* _lpItem, cItem* _lpTargetItem)
{
	
	cBasicItem* lpBasicItem = _lpItem->getBasicItem();
	cBasicItem* lpTargetBasicItem = _lpTargetItem->getBasicItem();
	
	
	if(!lpBasicItem || !lpTargetBasicItem)
		return FALSE;
	
	if(!_lpTargetItem->isExistSpecificEffect(eIE_CARVING_LEVEL) ||  !lpTargetBasicItem->m_bf1IsRingOfInfinity)
	{
		g_msgBox.cPopup("",dMSG_CAN_NOT_USE_ITEM_ON_DEST_ITEM,dMSG_OK);
		return FALSE;
	}
	if(lpTargetBasicItem->m_bf5LimitUpgradeRingOfInfinity  <= _lpTargetItem->getCarvingLevel())
	{
		g_msgBox.cPopup("",dMSG_CAN_NOT_USE_ITEM_ON_DEST_ITEM,dMSG_OK);
		return FALSE;
	}
	
	char	strMessage[512] = "";
	int iCarvingLevel	=	_lpTargetItem->getCarvingLevel();
	if(!iCarvingLevel)
	{
		g_msgBox.cPopup("",dMSG_CAN_NOT_USE_ITEM_ON_DEST_ITEM,dMSG_OK);
		return FALSE;
	}
	int iCristalCount	= lpTargetBasicItem->getCarveNeedCristalCount(iCarvingLevel - 1);
	LONGLONG llMoney	= lpTargetBasicItem->getCarveNeedMoney(iCarvingLevel - 1) * 10000;
	float iLuckChance	= getLuck() *  dCARVING_LUCK_FACTOR;
	float iChance		= _lpItem->getIncreaseCarvingChance();
	if(iCristalCount)
	{
		sprintf(strMessage, "%s\n\n%s\n\n%s <c:LTYELLOW>%s<n>\n%s <c:LTYELLOW>%d%s<n>\n%s <c:LTYELLOW>%.2f%s<n>\n\n%s",
			_lpTargetItem->getName(),
			dMSG_CARVING_MATERIALS, 
			dMSG_KOR_GOLD,	GetMoneyString(llMoney), 
			dMSG_ITEM_CRYSTAL, iCristalCount ,dMSG_COUNT,
			dMSG_SUCCESS_CHANCE,iLuckChance + iChance,dMSG_PERCENT,
			dMSG_REALLY_IS_CARVE);
	}
	else
	{
		sprintf(strMessage, "%s\n\n%s\n\n%s <c:LTYELLOW>%s<n>\n%s <c:LTYELLOW>%.2f%s<n>\n\n%s",
			_lpTargetItem->getName(),
			dMSG_CARVING_MATERIALS, 
			dMSG_KOR_GOLD,	GetMoneyString(llMoney), 
			dMSG_SUCCESS_CHANCE,iLuckChance + iChance,dMSG_PERCENT,
			dMSG_REALLY_IS_CARVE);
		
	}
	
	g_msgBox.cPopup(NULL, strMessage,dMSG_YES,dMSG_NO);
	g_msgBox.setButtonId(eMBI_USE_ITEM_TO_ITEM);
	g_msgBox.setSize(320,150);
	
	return TRUE;
	

}



BOOL
CHero::askSetCarvingLevel(cItem* _lpItem, cItem* _lpTargetItem)
{
	
	cBasicItem* lpBasicItem = _lpItem->getBasicItem();
	cBasicItem* lpTargetBasicItem = _lpTargetItem->getBasicItem();
	
	
	if(!lpBasicItem || !lpTargetBasicItem)
		return FALSE;
	
	if(!_lpTargetItem->isExistSpecificEffect(eIE_CARVING_LEVEL) ||  !lpTargetBasicItem->m_bf1IsRingOfInfinity)
	{
		g_msgBox.cPopup("",dMSG_CAN_NOT_USE_ITEM_ON_DEST_ITEM,dMSG_OK);
		return FALSE;
	}
	if(lpTargetBasicItem->m_bf5LimitUpgradeRingOfInfinity  < _lpTargetItem->getCarvingLevel())
	{
		g_msgBox.cPopup("",dMSG_CAN_NOT_USE_ITEM_ON_DEST_ITEM,dMSG_OK);
		return FALSE;
	}
	
	if(_lpTargetItem->getCarvingLevel()> _lpItem->getSetCarvingLevel() )
	{
		g_msgBox.cPopup("",dMSG_CAN_NOT_USE_ITEM_ON_DEST_ITEM,dMSG_OK);
		return FALSE;
	}	

	char	strMessage[512] = "";
	float iChance		= _lpItem->getSetCarvingChance();
	
	sprintf(strMessage, "%s\n\n%s <c:LTYELLOW>%.2f%s<n>\n\n%s",
		_lpTargetItem->getName(),
		dMSG_SUCCESS_CHANCE,iChance,dMSG_PERCENT,
		dMSG_REALLY_IS_CARVE);
	
	
	g_msgBox.cPopup(NULL, strMessage,dMSG_YES,dMSG_NO);
	g_msgBox.setButtonId(eMBI_USE_ITEM_TO_ITEM);

	
	return TRUE;
}
//
//	아이템 사용
BOOL
CHero::askUseItemToCharacter(int _iItemSlot,int _iTarget)
{
	cItem		*lpItem			=	getItem(_iItemSlot);
	CActor		*lpActor		=	g_am.getTestedActor(_iTarget,FALSE);

	if (!lpItem || !lpActor)
		return	FALSE;

	return	s_agent.sendUseItemToCharacter(_iItemSlot,_iTarget);
}

//
//	소비성 아이템이냐?
BOOL
CHero::isConsumeItem(int _iItemSlot)
{
	cItem	*lpItem				=	getInventoryItem(_iItemSlot);

	if (!lpItem		)
		return	FALSE;

	if	(lpItem->getExpandInventorySize())	//	임시 코드 이거 아이템 데이터에서 수정해야 한다.
		return	TRUE;

	return	lpItem->isConsumeItem();
}

//
//	아이템 빈자리 얻기
cItem*
CHero::getValidInventorySlot()
{
	int	i;

	for (i=0;i<getItemSlotCount();i++)
		if (m_aItems[i].m_wBaseItem	==	0xffff)
			return	&m_aItems[i];

	for (i=0;i<m_bf6ExtraInventorySize;i++)
		if (m_aExtraInventory[i].m_wBaseItem	==	0xffff)
			return	&m_aExtraInventory[i];

	return	NULL;
}

//
//	아이템 정렬
void
CHero::arrangeInventory(BOOL _bIsEnterField)
{
	cItem	*lpItem;
	int		i;

	for (i=0;i<m_wItemSlotCount;i++)
	{
		lpItem	=	&m_aItems[i];

		if	(lpItem->m_wBaseItem	==	0xffff	)
			continue;

		if	(lpItem->m_wBaseItem	>=	dBASIC_ITEM_COUNT	)
		{
			lpItem->reset();
			continue;
		}

		if	(_bIsEnterField	&&	lpItem->getBasicItem()->m_bf1IsDestroyWhenMoveField)
		{
			lpItem->reset();
			continue;
		}

		if	(lpItem->isExtraItem() || lpItem->isMagicCarpetEmblem() || lpItem->isExceptionItem())
			continue;

		if	(lpItem->m_bCount	<=	0					)
		{
			lpItem->reset();
			continue;
		}
		
		if	(lpItem->isInfinityBullet()					)
			lpItem->m_bCount	=	1;

		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		for (int j=0;j<dITEM_PREFIX_COUNT;j++)
			if (lpItem->m_aPrefix[j].m_wPrefix	>=	g_itemPrefix.m_iCount)
				lpItem->m_aPrefix[j].m_wPrefix	=	0xffff;

		lpItem->m_bCount		=	min(lpItem->m_bCount,(BYTE)lpBasicItem->m_wStackLimit);
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		lpItem	=	&m_aExtraInventory[i];

		if	(lpItem->m_wBaseItem	==	0xffff	)
			continue;

		if	(lpItem->m_wBaseItem	>=	dBASIC_ITEM_COUNT	)
		{
			lpItem->reset();
			continue;
		}

		if	(_bIsEnterField	&&	lpItem->getBasicItem()->m_bf1IsDestroyWhenMoveField)
		{
			lpItem->reset();
			continue;
		}

		if	(lpItem->isExtraItem() || lpItem->isMagicCarpetEmblem() || lpItem->isExceptionItem())
			continue;

		if	(lpItem->m_bCount	<=	0					)
		{
			lpItem->reset();
			continue;
		}

		if	(lpItem->isInfinityBullet()					)
			lpItem->m_bCount	=	1;

		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		for (int j=0;j<dITEM_PREFIX_COUNT;j++)
			if (lpItem->m_aPrefix[j].m_wPrefix	>=	g_itemPrefix.m_iCount)
				lpItem->m_aPrefix[j].m_wPrefix	=	0xffff;

		lpItem->m_bCount		=	min(lpItem->m_bCount,(BYTE)lpBasicItem->m_wStackLimit);
	}

	if	(_bIsEnterField)
		for (i=0;i<dEQUIPMENT_PART_COUNT;i++)
		{
			cItem		*lpItem			=	&m_aEquip[i];
			cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

			if	(lpBasicItem	&&	lpBasicItem->m_bf1IsDestroyWhenMoveField)
				lpItem->reset();
		}


	{
		for (i=0;i<dEQUIPMENT_PART_COUNT;i++)
		{
			cItem			*lpItem		=	&m_aEquip[i];
			cSpecialItem	*lpSpItem	=	lpItem->getSpecialItem();

			if	(lpSpItem && lpSpItem->m_bf1IsReversionItemWhenEquip)
			{
				lpSpItem->m_bf1IsReversionItem			=	TRUE;
			}
		}
	}

	computeItemCount();
}

//
//	외모 싱크.. *-_-*
void
CHero::syncHeroApprearance()
{
	if	(g_lpHero	==	NULL)
		return;

	cItem	*lpArmor	=	getArmor();
	cItem	*lpWeapon	=	getWeapon();

	if	(lpWeapon	&&	lpWeapon->isPrincessWeapon())
	{
		cItem	item;

		item.m_wBaseItem=	lpWeapon->getPrincessWeapon()->m_wBasicItem;

		g_lpHero->m_itemWeapon.copy(&item);
	}
	else
		g_lpHero->m_itemWeapon.copy(lpWeapon);

	g_lpHero->m_itemShield.copy(getShield());
	g_lpHero->m_itemArmor.copy(lpArmor);
	g_lpHero->getBodyIndex();

	g_lpHero->m_itemWeapon.m_wColorizeEffect	=	getWeaponColorizeEffect();
	g_lpHero->m_wOperatorLevel					=	m_wOperatorLevel;

	g_lpHero->m_itemArmor.m_wColorizeEffect		=	0;

	if	(lpArmor)
		g_lpHero->m_itemArmor.m_wColorizeEffect	=	lpArmor->getBasicItem(TRUE)->m_wPaletteIndex;

	g_lpHero->checkEquipmentShape();

	int	iMiniPetType1=nsMiniPetType::Count,iMiniPetType2=nsMiniPetType::Count;

	if	(m_wActiveMiniPet	<	c_iMiniPetCount)
	{
		if	(m_aMiniPet[m_wActiveMiniPet].m_bf7Level)
			iMiniPetType1	=	m_aMiniPet[m_wActiveMiniPet].m_bf5Type;
	}
	if	(m_wActiveMiniPet2	<	c_iMiniPetCount)
	{
		if	(m_aMiniPet[m_wActiveMiniPet2].m_bf7Level)
			iMiniPetType2	=	m_aMiniPet[m_wActiveMiniPet2].m_bf5Type;
	}

	g_lpHero->changeMinipetType(iMiniPetType1,iMiniPetType2);
}

//	무기의 컬러 이펙트
int
CHero::getWeaponColorizeEffect()
{
	int		aEffect[dITEM_PREFIX_COUNT],i,iEffectCount=0,iEffect=0xffff;
	cItem	*lpWeapon	=	getWeapon(TRUE);

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


//
//	벨트 아이템 사용
BOOL
CHero::sendUseBeltItem(int _iSlot)
{
	cItem	*lpItem	=	getBeltItem(_iSlot);

	if	(!lpItem || lpItem->m_wBaseItem	==	0xffff	)
		return	FALSE;
	if	(lpItem->m_bCount	<=	0		)
		return	FALSE;

	int	iFocusMember	=	s_partyInfo.getFocusMember();

	cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

	if	(iFocusMember	!=	0xffff)
	{
		if	(!g_hero.isPet(iFocusMember) && !g_hero.isSummonBeastActor(iFocusMember))
			iFocusMember=	0xffff;

		if(g_hero.isPet(iFocusMember) || g_hero.isSummonBeastActor(iFocusMember))
		{
			if(lpBasicItem->m_bf1IsItemNotUsePet)
			{
				AddSystemMessage(WHITE,dMSG_ITEM_NOT_USE_PET);
				return FALSE;
			}	
		}

	}
	if	(lpItem->getCoolTime(lpBasicItem))
	{
		g_lpHero->warning(dMSG_WARNING_ITEM_COOLTIME);
		return	FALSE;
	}

	if	(s_aKey[KEY_LSHIFT])
	{
		CActor	*lpPet	=	getPetActor(0);
		if(lpBasicItem->m_bf1IsItemNotUsePet)
		{
			AddSystemMessage(WHITE,dMSG_ITEM_NOT_USE_PET);
			return FALSE;
		}
		if	(lpPet)
			iFocusMember=	lpPet->m_wSerial;
	}
	
	if	(s_aKey[KEY_LALT])
	{
		CActor	*lpPet	=	getPetActor(1);
		if(lpBasicItem->m_bf1IsItemNotUsePet)
		{
			AddSystemMessage(WHITE,dMSG_ITEM_NOT_USE_PET);
			return FALSE;
		}
		if	(lpPet)
			iFocusMember=	lpPet->m_wSerial;
	}

	if	(iFocusMember	==	0xffff)
		if	(g_hero.isPet(s_iFocusActor) || g_hero.isSummonBeastActor(s_iFocusActor))
		{
			iFocusMember=	s_iFocusActor;
			
			if(lpBasicItem->m_bf1IsItemNotUsePet)
			{
				AddSystemMessage(WHITE,dMSG_ITEM_NOT_USE_PET);
				return FALSE;
			}

			CActor	*lpFocusActor	=	g_am.getTestedActor(iFocusMember,FALSE);

			if	(!lpFocusActor	||	lpFocusActor->isDeath())
				iFocusMember	=	0xffff;
		}

	if	(iFocusMember	==	0xffff)
	{
		if	(s_iFocusActor	!=	0xffff)
		{
			if	(lpBasicItem->m_attr.isUseToCorpse)
			{
				CActor	*lpFocusActor	=	g_am.getActor(s_iFocusActor);

				if	(lpFocusActor	&&	lpFocusActor->isDeath())
					iFocusMember	=	s_iFocusActor;
			}
		}

		if	(iFocusMember	==	0xffff)
			iFocusMember	=	m_iSerial;
	}

	return	s_agent.sendUseBeltItem(_iSlot,iFocusMember);
}

//
//	벨트 아이템 리로드 메시지 전송
BOOL
CHero::sendBeltItemReload()
{
	if (m_iLastActionTime+dSEND_HERO_ACTION_PERIOD	>	s_iFrameCounter	)	return	FALSE;

	m_iLastActionTime	=	s_iFrameCounter;

	if (!isAvailReloadBeltSlot())
	{
		g_lpHero->warning(dMSG_NOT_EXIST_RELOAD_ABLE_ITEM);

		return	FALSE;
	}

	if (m_bf1IsReloadingBeltItem)
	{
		g_lpHero->warning(dMSG_ALREADY_RELOADING_BELT_SLOT);
		return	FALSE;
	}

	return	s_agent.sendReloadBeltItem();
}

//
//	벨트에 아이템을 리로드 한다.
BOOL
CHero::isAvailReloadBeltSlot()
{
	int		i;
	cItem	*lpSlot;
	BOOL	isReload	=	FALSE;

	for (i=dEQUIP_BELT_SLOT1;i<=dEQUIP_BELT_SLOT5;i++)
	{
		lpSlot	=	&m_aEquip[i];

		if (lpSlot->m_wBaseItem	==	0xffff				)
			continue;
		if (lpSlot->m_bCount	>=	m_wBeltStackCount	)
			continue;

		cItem	*lpItem		=	getSameItem(lpSlot);

		if (!lpItem)	continue;

		isReload	=	TRUE;
	}

	return	isReload;
}

cItem*
CHero::getNonameCoupleRing()
{
	int	i;

	for (i=0;i<getItemSlotCount();i++)
	{
		if	(!m_aItems[i].isNotDefineCoupleCoupleRing())
			continue;

		return	&m_aItems[i];
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		if	(!m_aExtraInventory[i].isNotDefineCoupleCoupleRing())
			continue;

		return	&m_aExtraInventory[i];
	}

	return	NULL;
}

cItem*
CHero::getSecretDungeonKey(int _iDungeonSerial)
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

//
//	인벤토리 아이템 얻어오기
cItem*
CHero::getInventoryItem(int _iSlot,BOOL _bIsIgnoreTrade,BOOL _bIsWantSlot)
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

		if	(!lpItem->isExtraItem()	&&	!lpItem->isMagicCarpetEmblem()  && !lpItem->isExceptionItem())
			if (lpItem->m_bCount	<=	0	)
				return	NULL;

		if	(_bIsIgnoreTrade	==	FALSE)	
			if	(CGamePlay::IsInvisibleItem(_iSlot))
				return	NULL;
	}

	return	lpItem;
}

//
//	아이템 얻기
cItem*
CHero::getItem(int _iSlot)
{
	if (_iSlot <  0)
		return	NULL;
	if (_iSlot	<	dBORDER_OF_ITEM_AND_EQUIPMENT)
	{
		if (_iSlot	<	dOWN_ITEM_COUNT	&&	_iSlot	>=	getItemSlotCount())
			return	NULL;
		if (_iSlot	>=	dOWN_ITEM_COUNT+m_bf6ExtraInventorySize)
			return	NULL;
	}
	if (_iSlot >= dBORDER_OF_ITEM_AND_EQUIPMENT+dEQUIPMENT_PART_COUNT			)
		return	NULL;

	cItem	*lpItem;

	if	(_iSlot	>=	dBORDER_OF_ITEM_AND_EQUIPMENT)
		lpItem	=	&m_aEquip[_iSlot-dBORDER_OF_ITEM_AND_EQUIPMENT];
	else
	if	(_iSlot	>=	dOWN_ITEM_COUNT)
		lpItem	=	&m_aExtraInventory[_iSlot-dOWN_ITEM_COUNT];
	else
		lpItem	=	&m_aItems[_iSlot];

	if	(lpItem->m_wBaseItem	==	0xffff		)
		return	NULL;

	if	(!lpItem->isExtraItem()	&&	!lpItem->isMagicCarpetEmblem()  && !lpItem->isExceptionItem())
		if (lpItem->m_bCount	<=	0		)
			return	NULL;

	if (CGamePlay::IsInvisibleItem(_iSlot))
		return	NULL;

	return	lpItem;
}

int
CHero::getMinipetFeedTypeCount(int iFeedType , BOOL _bCheckExtraBag)
{
	int	i , iItemCount = 0;
	
	for (i=0;i<m_wItemSlotCount;i++)
	{
		cItem	*lpItem	=	&m_aItems[i];
		
		if (lpItem->m_wBaseItem	==	0xffff)
			continue;
		if (lpItem->getMiniPetFeedType()	==	iFeedType)
			iItemCount++;
	}
	
	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		cItem	*lpItem	=	&m_aExtraInventory[i];
		
		if (lpItem->m_wBaseItem	==	0xffff)
			continue;
		if (lpItem->getMiniPetFeedType()	==	iFeedType)
			iItemCount++;
	}

	return iItemCount;
}

cItem*
CHero::getItemByBaseItemIndex(int _iItem,BOOL _bIsIncludeEquipment)	//	아이템 얻기
{
	int	i;

	for (i=0;i<m_wItemSlotCount;i++)
	{
		cItem	*lpItem	=	&m_aItems[i];

		if (lpItem->m_wBaseItem	==	0xffff)
			continue;
		if (lpItem->m_wBaseItem	!=	_iItem)
			continue;

		return	lpItem;
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		cItem	*lpItem	=	&m_aExtraInventory[i];

		if (lpItem->m_wBaseItem	==	0xffff)
			continue;
		if (lpItem->m_wBaseItem	!=	_iItem)
			continue;

		return	lpItem;
	}

	i	=	dEQUIP_BELT_SLOT1;

	if	(_bIsIncludeEquipment)
		i	=	0;

	for (;i<dEQUIPMENT_PART_COUNT;i++)
	{
		cItem	*lpItem	=	&m_aEquip[i];

		if	(lpItem->m_wBaseItem	==	0xffff)
			continue;
		if	(lpItem->m_wBaseItem	!=	_iItem)
			continue;

		return	lpItem;
	}

	return	NULL;
}

void
CHero::decreaseBullet(cAbility *_lpAbility)	//	탄환이 필요한 스킬이었다면 탄환을 줄인다.
{
	if (_lpAbility->isDecreaseWeaponCountSkill())
	{
		cItem	*lpWeapon	=	getWeapon();

		if (lpWeapon && !lpWeapon->isInfinityBullet() && !lpWeapon->isPrincessWeapon())
			lpWeapon->decreaseCount(1);

		if	(lpWeapon->m_bCount	==	0)
		{
#ifdef	_DEBUG
			//JBC 도적 던지기류 아이템 잔탄 없으면 경고해줌. 08-06-30
			g_lpHero->warning(dMSG_EMPTY_BULLET_FORM);
#endif
			syncHeroApprearance();
		}
	}

	if (_lpAbility->isRequireBullet())
	{
		int	iCount	=	_lpAbility->getContinuousShotCount();

		if (iCount	<=	0)
			iCount	=	1;

		cItem	*lpBullet	=	getBullet();

		if(!lpBullet && m_isMagicBullet)
			return;

		if (lpBullet && !lpBullet->isInfinityBullet())
			lpBullet->decreaseCount(iCount);

#ifdef	_DEBUG
		if	(lpBullet->m_bCount	==	0)
		{
			//JBC 탄환류 아이템 잔탄 없으면 경고해줌. 08-06-30
			g_lpHero->warning(dMSG_EMPTY_BULLET_FORM);
		}
#endif
	}
}

int
CHero::getEmptyInventorySlotCount()
{
	computeItemCount();

	return	m_wItemSlotCount+m_bf6ExtraInventorySize-m_wItemCount;
}

//
//	아이템 제거
BOOL
CHero::removeItemByBaseItemIndex(int _iItem,int _iCount,BOOL _bIsIncludeEquipment)
{

	// 10.01.07 추가
	vector<int>VectorSlot;
    	vector<int>::iterator it ;
	
	for(int i=0,Index = 0; i<10; i++)
		for(int j=0; j<10; j++)
			for(int k=0; k<2; k++, Index++)
			{
				if(m_aQuickSkill[i][j][k] >= dEQUIPMENT_SKILL_SLOT && m_aQuickSkill[i][j][k] <= dEQUIPMENT_SKILL_SLOT * 2 )
				{
					cItem	*lpItem	=	g_hero.getInventoryItem(m_aQuickSkill[i][j][k] - dEQUIPMENT_SKILL_SLOT);
					// 10.02.01 추가
					if(!lpItem)
					{
						m_aQuickSkill[i][j][k]					= 0xffff;
						sendChangeUserSkillSetting(cUserCustomDataDefine::eUSS_QUICK_SKILL,i, j, k,0xffff);
						m_sQuickSlotItem[i][j][k].IconShape		= 0xffff;
						m_sQuickSlotItem[i][j][k].NotUseItem	= FALSE;
						m_sQuickSlotItem[i][j][k].DxItem		= FALSE;
						m_sQuickSlotItem[i][j][k].UmItem		= FALSE;

						continue;
					}							
					if(lpItem->m_wBaseItem == _iItem)
					{
						VectorSlot.push_back(Index);	
					}
				}
			}

	while(1)
	{
		cItem	*lpItem		=	getItemByBaseItemIndex(_iItem,_bIsIncludeEquipment);

		if	(!lpItem)
			return	FALSE;

		int		iItemCount	=	lpItem->m_bCount;

		lpItem->decreaseCount(_iCount);

		_iCount				-=	iItemCount;

		if (_iCount	<=	0)
		{
			// 10.01.07 추가
			for(it=VectorSlot.begin(); it!=VectorSlot.end(); it++)
			{
				int EquiItemSlot1 = (*it/ 2) / 10;
				int EquiItemSlot2 = (*it / 2) % 10;
				int EquiItemSlot3 = *it % 2;
			
				cItem	*lpItem	=	g_hero.getInventoryItem(m_aQuickSkill[EquiItemSlot1][EquiItemSlot2][EquiItemSlot3] - dEQUIPMENT_SKILL_SLOT);
				
				if(!lpItem)
				{
					m_aQuickSkill[EquiItemSlot1][EquiItemSlot2][EquiItemSlot3]					= 0xffff;
					sendChangeUserSkillSetting(cUserCustomDataDefine::eUSS_QUICK_SKILL,EquiItemSlot1,EquiItemSlot2,EquiItemSlot3,0xffff);

					m_sQuickSlotItem[EquiItemSlot1][EquiItemSlot2][EquiItemSlot3].IconShape		= 0xffff;
					m_sQuickSlotItem[EquiItemSlot1][EquiItemSlot2][EquiItemSlot3].NotUseItem	= FALSE;
					m_sQuickSlotItem[EquiItemSlot1][EquiItemSlot2][EquiItemSlot3].DxItem		= FALSE;
					m_sQuickSlotItem[EquiItemSlot1][EquiItemSlot2][EquiItemSlot3].UmItem		= FALSE;
				}
			}
			return	TRUE;
		}
	}

	return	FALSE;
}
// 10.01.07 함수 추가
// 장비창에 있는 아이템이 스킬 등록되었는지 여부
int
CHero::getEquipItemBySlotIndex(int _iItemSlot)
{
	int Index = 0;
	for(int i=0; i<10; i++)
		for(int j=0; j<10; j++)
			for(int k=0; k<2; k++, Index++)
			{
				if(m_aQuickSkill[i][j][k] == _iItemSlot + dEQUIPMENT_SKILL_SLOT)
					return Index;
			}

	return 0xffff;
}
//
//
int
CHero::removeQuestItem(int _iQuestKind,int _iQuestIndex)
{
	int		i;
	BOOL	bIsRemovedEquipment	=	FALSE;
	int		iRemoveItemCount	=	0;

	for (i=0;i<dEQUIPMENT_PART_COUNT;i++)
	{
		cItem		*lpItem			=	&m_aEquip[i];
		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(!lpBasicItem								)	continue;
		if	(lpBasicItem->m_questKind == _iQuestKind && lpBasicItem->m_questIndex == _iQuestIndex)
		{
			lpItem->reset();
			bIsRemovedEquipment	=	TRUE;
			iRemoveItemCount++;
		}
	}

	for (i=0;i<m_wItemSlotCount;i++)
	{
		cItem		*lpItem			=	&m_aItems[i];
		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(!lpBasicItem								)	continue;
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

		if	(!lpBasicItem								)
			continue;
		if	(lpBasicItem->m_questKind == _iQuestKind && lpBasicItem->m_questIndex == _iQuestIndex)
		{
			lpItem->reset();
			iRemoveItemCount++;
		}
	}

	if (iRemoveItemCount)
	{
		CQuestItem	*lpQuest = NULL;

		if (_iQuestKind == eQT_NORMAL)
			lpQuest	=	g_quest.getQuest(_iQuestIndex);
		if (_iQuestKind == eQT_ARBEIT)
			lpQuest	=	g_questArbeit.getQuest(_iQuestIndex);

		CGamePlay::AddSystemMessage(WHITE,dMSG_REMOVE_QUEST_ITEM_FORM,lpQuest->m_strName);
	}

	if (bIsRemovedEquipment)	buildPower();

	return	iRemoveItemCount;
}


void
CHero::getBreakdownItemList(int *_lpiItemList,int *_lpiItemCount)
{
	int	iBreakdownItemCount	=	0;

	for (int i=0;i<getItemSlotCount();i++)
	{
		cItem	*lpItem	=	&m_aItems[i];

		if	(!lpItem->isBreakdownItem())
			continue;

		_lpiItemList[iBreakdownItemCount++]	=	i;
	}

	*_lpiItemCount	=	iBreakdownItemCount;
}

int 
CHero::findSpecificEffectItem(int _iEffect,BOOL _bIsIncludeCheckBelt)
{
	int	i;

	for (i=0;i<getItemSlotCount();i++)
	{
		cItem	*lpItem	=	&m_aItems[i];

		if	(lpItem->m_wBaseItem	==	0xffff	||	lpItem->m_bCount	==	0)
			continue;

		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem(TRUE);

		if	(!lpBasicItem	)
			continue;

		for (int iGenData=0;iGenData<dGENERATE_ITEM_DATA_COUNT;iGenData++)
			if	(lpBasicItem->m_aGenerateData[iGenData].m_wEffect	==	_iEffect)
				return	i;
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		cItem	*lpItem	=	&m_aExtraInventory[i];

		if	(lpItem->m_wBaseItem	==	0xffff	||	lpItem->m_bCount	==	0)
			continue;

		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem(TRUE);

		if	(!lpBasicItem	)
			continue;

		for (int iGenData=0;iGenData<dGENERATE_ITEM_DATA_COUNT;iGenData++)
			if	(lpBasicItem->m_aGenerateData[iGenData].m_wEffect	==	_iEffect)
				return	i+dOWN_ITEM_COUNT;
	}

	if	(_bIsIncludeCheckBelt)
		for (i=dEQUIP_BELT_SLOT1;i<=dEQUIP_BELT_SLOT5;i++)
		{
			cItem	*lpItem	=	&m_aEquip[i];

			if	(lpItem->m_wBaseItem	==	0xffff	||	lpItem->m_bCount	==	0)
				continue;

			cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

			if	(!lpBasicItem	)
				continue;

			for (int iGenData=0;iGenData<dGENERATE_ITEM_DATA_COUNT;iGenData++)
				if	(lpBasicItem->m_aGenerateData[iGenData].m_wEffect	==	_iEffect)
					return	i+dBORDER_OF_ITEM_AND_EQUIPMENT;
		}

	return	0xffff;
}

void
CHero::operateExpiredItem(int _iSlot,int _iItem)
{
	cItem	*lpItem	;

	if (_iSlot	>=	dBORDER_OF_ITEM_AND_EQUIPMENT)
		lpItem	=	&m_aEquip[_iSlot-dBORDER_OF_ITEM_AND_EQUIPMENT];
	else
	if (_iSlot	>=	dOWN_ITEM_COUNT)
		lpItem	=	&m_aExtraInventory[_iSlot-dOWN_ITEM_COUNT];
	else
		lpItem	=	&m_aItems[_iSlot];

	cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

	if	(!lpBasicItem)
		return;

	if	(lpItem->m_wBaseItem	!=	_iItem)
	{
		if	(lpBasicItem->m_bf1IsDestroyWhenMoveField)
			CGamePlay::AddSystemMessage(WHITE,dMSG_TREASURE_MAP_DESTROYED);
		else
			CGamePlay::AddSystemMessage(WHITE,dMSG_REMOVE_EXPIRED_ITEM_FORM,g_aBasicItem[_iItem].m_strName);

		return;
	}

	if	(lpBasicItem->m_bf1IsDestroyWhenMoveField)
		CGamePlay::AddSystemMessage(WHITE,dMSG_TREASURE_MAP_DESTROYED);
	else
		CGamePlay::AddSystemMessage(WHITE,dMSG_REMOVE_EXPIRED_ITEM_FORM,lpItem->getName());

	lpItem->reset();

	buildPower();
}

void
CHero::operateExpiredItemList(int _iCount,WORD *_lpItemList)
{
	BOOL	bIsExistValidItem	=	FALSE;

	for (int i=0;i<_iCount;i++)
	{
		int		iSlot		=	_lpItemList[i*2		];
		int		iBaseItem	=	_lpItemList[i*2+1	];

		cItem	*lpItem		=	getInventoryItem(iSlot);

		if	(!lpItem	||	lpItem->m_wBaseItem	!=	iBaseItem)
		{
			bIsExistValidItem	=	TRUE;
			CGamePlay::AddSystemMessage(WHITE,dMSG_REMOVE_EXPIRED_ITEM_FORM,g_aBasicItem[iBaseItem].m_strName);
			continue;
		}

		CGamePlay::AddSystemMessage(WHITE,dMSG_REMOVE_EXPIRED_ITEM_FORM,lpItem->getName());

		lpItem->reset();
	}

	if	(bIsExistValidItem)
		s_agent.sendAskItemData();
	else
		buildPower();
}

int
CHero::getUnlockArcaKeySlot(int _iLockLevel)
{
	int		i,iMinLevel	=	10000;
	int		iKey		=	0xffff;
	
	for (i=0;i<m_wItemSlotCount;i++)
	{
		int	iLevel	=	m_aItems[i].getUnlockArcaLevel();

		if (iLevel	<	_iLockLevel)
			continue;

		if (iLevel	<	iMinLevel)
		{
			iMinLevel	=	iLevel;
			iKey		=	i;
		}
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		int	iLevel	=	m_aExtraInventory[i].getUnlockArcaLevel();

		if (iLevel	<	_iLockLevel)
			continue;

		if (iLevel	<	iMinLevel)
		{
			iMinLevel	=	iLevel;
			iKey		=	i+dOWN_ITEM_COUNT;
		}
	}

	return	iKey;
}

int
CHero::getUnlockDoorKeySlot(int _iLockLevel)
{
	int		i,iMinLevel	=	10000;
	int		iKey		=	0xffff;
	
	for (i=0;i<m_wItemSlotCount;i++)
	{
		int	iLevel	=	m_aItems[i].getUnlockDoorLevel();

		if (iLevel	<	_iLockLevel)
			continue;

		if (iLevel	<	iMinLevel)
		{
			iMinLevel	=	iLevel;
			iKey		=	i;
		}
	}

	for (i=0;i<m_bf6ExtraInventorySize;i++)
	{
		int	iLevel	=	m_aExtraInventory[i].getUnlockDoorLevel();

		if (iLevel	<	_iLockLevel)
			continue;

		if (iLevel	<	iMinLevel)
		{
			iMinLevel	=	iLevel;
			iKey		=	i+dOWN_ITEM_COUNT;
		}
	}

	return	iKey;
}

cItem*
CHero::getUnlockArcaKey(int _iLockLevel)
{
	int	iSlot	=	getUnlockArcaKeySlot(_iLockLevel);

	if (iSlot	==	0xffff)
		return	NULL;

	if (iSlot	<	dOWN_ITEM_COUNT)
		return	&m_aItems[iSlot];

	return	&m_aExtraInventory[iSlot-dOWN_ITEM_COUNT];
}

cItem*
CHero::getUnlockDoorKey(int _iLockLevel)
{
	int	iSlot	=	getUnlockDoorKeySlot(_iLockLevel);

	if (iSlot	==	0xffff)
		return	NULL;

	if (iSlot	<	dOWN_ITEM_COUNT)
		return	&m_aItems[iSlot];

	return	&m_aExtraInventory[iSlot-dOWN_ITEM_COUNT];
}


BOOL
CHero::removeItemPrefix(int _iUseItem,int _iDestItem,int _iPrefixIndex)
{
	cItem	*lpUseItem	=	getInventoryItem(_iUseItem);
	cItem	*lpDestItem	=	getInventoryItem(_iDestItem);

	if	(!lpUseItem	||	!lpDestItem	||	lpDestItem->getPrefixCount() <= _iPrefixIndex)
		return	FALSE;

	if	(!lpUseItem->checkFirstEffect(eIE_REMOVE_PREFIX)	&&	!lpUseItem->checkFirstEffect(eIE_CH5_REMOVE_PREFIX))
		return	FALSE;

	char	strText[256];

	cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[lpDestItem->m_aPrefix[_iPrefixIndex].m_wPrefix];

	sprintf(strText,dMSG_REMOVE_PREFIX_RESULT_FORM,lpDestItem->getName(),lpPrefix->m_str1stPrefix);

	for	(int i=_iPrefixIndex;i<dITEM_PREFIX_COUNT-1;i++)
		memcpy(&lpDestItem->m_aPrefix[i],&lpDestItem->m_aPrefix[i+1],sizeof(CItemPrefixInfo));

	memset(&lpDestItem->m_aPrefix[dITEM_PREFIX_COUNT-1],0xff,sizeof(CItemPrefixInfo));

	lpUseItem->decreaseCount(1);

	g_msgBox.cPopup(dMSG_REMOVE_PREFIX_RESULT,strText,dMSG_OK);

	return	TRUE;
}

BOOL
CHero::judgeItem(int _iItem,int _iPrice)
{
	cItem			*lpItem		=	g_hero.getInventoryItem(_iItem);

	if	(!lpItem)
		return	FALSE;

	cItem	item;
	char	strText[256];
	int		iPieceIndex			=	lpItem->m_aOption[0]+1;

	cSpecialItem	*lpSPItem	=	lpItem->getSpecialItem();

	if	(!lpSPItem)
		return	FALSE;

	lpSPItem->m_bf1IsUnknown	=	FALSE;

	m_iGold						-=	_iPrice;

	cPieceItem	*lpPieceItem	=	lpItem->getPieceItem();

	if	(lpPieceItem)
	{
		if	(lpPieceItem->generateItem(&item))
			lpItem	=	&item;

		sprintf(strText,dMSG_RESULT_OF_JUDGE_ITEM_PIECE_FORM,lpItem->getName(),iPieceIndex,lpPieceItem->m_wPieceCount,GetMoneyString(_iPrice));
	}
	else
		sprintf(strText,dMSG_RESULT_OF_JUDGE_ITEM_FORM,lpItem->getName(),GetMoneyString(_iPrice));

	g_msgBox.cPopup(dMSG_RESULT_OF_JUDGE_ITEM,strText,dMSG_OK);

	return	TRUE;
}

BOOL
CHero::mergeItem(int _iItem,int _iPrice)
{
	cItem		*lpFirstItem		=	g_hero.getInventoryItem(_iItem);

	if	(!lpFirstItem)
		return	FALSE;

	cPieceItem	*lpFirstPiece=	lpFirstItem->getPieceItem();		//	임의의 조각을 선택한후에

	if	(!lpFirstPiece)
		return	FALSE;

	int		i;
	int		iPieceCount			=	1;

	BYTE	abExistPiece[dMAX_PIECE_ITEM_PIECE_COUNT];

	memset(abExistPiece,0,sizeof(abExistPiece));

	abExistPiece[lpFirstItem->m_aOption[0]]	=	1;

	for	(i=0;i<dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE;i++)	//	동일한 아이템 조각 검색
	{
		if	(i	==	_iItem)
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

		lpItem->reset();
		iPieceCount++;

		if	(iPieceCount	>=	lpFirstPiece->m_wPieceCount)
			break;
	}

	lpFirstPiece->generateItem(lpFirstItem);
	m_iGold			-=	_iPrice;

	BOOL	bIsCanNotTradeItem	=	FALSE;

	for	(i=0;i<iPieceCount;i++)
		if	(lpFirstPiece->m_aPiece[i].m_bf1IsReversion)
			bIsCanNotTradeItem	=	TRUE;

	lpFirstItem->getSpecialItem()->m_bf1IsReversionItem	=	bIsCanNotTradeItem;

	g_gwMergeItem.mergeItem(_iItem);

	char	strText[256];

	sprintf(strText,dMSG_RESULT_OF_MERGE_ITEM_FORM,lpFirstItem->getName(),GetMoneyString(_iPrice));

	g_msgBox.cPopup(dMSG_RESULT_OF_MERGE_ITEM,strText,dMSG_OK);

	return	TRUE;
}

void
CHero::changeISPISetting(int _iItemSlot,int _iValue)
{
	cItem	*lpItem	=	getInventoryItem(_iItemSlot);

	if	(!lpItem	||	!lpItem->isIncreaseSelectPowerItem())
		return;

	cBasicItem	*lpBasic = lpItem->getBasicItem();
	
	if(!lpBasic)
		return;
	for	(int i=0;i<dGENERATE_ITEM_DATA_COUNT;++i)
	{
		if(lpBasic->m_aGenerateData[i].m_wEffect == eIE_INCREASE_FIX_SELECT_POWER)
		{
			if(lpItem->m_aOption[1] != 0)
				return;
			lpItem->m_aOption[1]	=	_iValue + 1;
		}
		else if(lpBasic->m_aGenerateData[i].m_wEffect == eIE_INCREASE_SELECT_POWER)
		{
			lpItem->m_aOption[1]	=	_iValue;
		}
	}
	
	g_hero.addTwinkleItem(_iItemSlot);
}
void
CHero::useBottomlessBox(int _iBottomlessBox,int _iItem1,int _iItem2)
{
	cItem	*lpBottomlessBox=	getInventoryItem(_iBottomlessBox);
	cItem	*lpItem1		=	getInventoryItem(_iItem1);
	cItem	*lpItem2		=	getInventoryItem(_iItem2);

	if	(lpBottomlessBox)
		lpBottomlessBox->decreaseCount(1);

	if	(lpItem1)
		lpItem1->reset();

	if	(lpItem2)
		lpItem2->reset();

	g_gwBottomlessBoxWindow.close();
	OpenInventoryWindow();

	AddSystemMessage(WHITE,dMSG_MIX_TWO_ITEM_BY_BOTTOMLESS_BOX);
}


cItem*
CHero::getFirstPotion()
{
	int	i;

	for (i=0;i<dEQUIPMENT_PART_COUNT+dBORDER_OF_ITEM_AND_EQUIPMENT;i++)
	{
		cItem	*lpItem				=	getInventoryItem(i);

		if	(!lpItem)
			continue;

		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(!lpBasicItem	||	lpBasicItem->m_wKind	!=	eIK_HEAL_POTION)
			continue;

		return	lpItem;
	}

	return	NULL;
}

cItem*
CHero::getFirstDrug()
{
	int		i;

	for (i=0;i<dEQUIPMENT_PART_COUNT+dBORDER_OF_ITEM_AND_EQUIPMENT;i++)
	{
		cItem	*lpItem				=	getInventoryItem(i);

		if	(!lpItem)
			continue;

		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

		if	(!lpBasicItem	||	lpBasicItem->m_wKind	!=	eIK_DRUG)
			continue;

		return	lpItem;
	}

	return	NULL;
}

cItem*
CHero::getFirstFlower()
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
CHero::getFirstCandy()
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

BOOL
CHero::isInterruptingArmor(cItem *_lpItem)
{
	const	int	c_iInterruptingPartLevelCount	=	8;

	int		iLevel	=	m_wInterruptingArmorLevel-1;

	if	(m_wInterruptingArmorTime	==	0	||	iLevel	<	0	||	iLevel	>=	c_iInterruptingPartLevelCount)
		return	FALSE;

	int		iItemKind=	_lpItem->getItemType();
	int		iPart	=	c_aItemEquipPlace[iItemKind];

	if	(iPart	==	0xffff	||	iItemKind	==	eIK_BULLET)
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
CHero::isInterruptingWeapon(cItem *_lpItem)
{	// 무기해제

	if	(!m_wInterruptingWeaponTime)
		return	FALSE;

	int		iItemKind=	_lpItem->getItemType();
	int		iPart	=	c_aItemEquipPlace[iItemKind];

	if	(c_aItemEquipPlace[iItemKind]	==	dEQUIP_WEAPON)
		return	TRUE;

	return	FALSE;
}

void
CHero::releaseItemReversion(int _iUseItem,int _iTargetItem)
{
	cItem	*lpUseItem	=	getInventoryItem(_iUseItem);
	cItem	*lpTargetItem=	getInventoryItem(_iTargetItem);

	if	(lpUseItem)
		lpUseItem->reset();

	if	(lpTargetItem)
	{
		if	(!lpTargetItem->getSpecialItem())
			return;

		lpTargetItem->getSpecialItem()->m_bf1IsReversionItem	=	FALSE;

		CGamePlay::AddSystemMessage(WHITE,dMSG_RELEASE_ITEM_REVERSION_FORM,lpTargetItem->getName());
	}
}

void
CHero::addTwinkleItem(int _iSlot)
{
	int i;

	for (i=0;i<dTWINKLE_ITEM_COUNT;i++)
	{
		if	(m_aiTwinkleItem[i]	==	_iSlot)
		{
			m_aiTwinkleItemFrame[i]	=	0;

			return;
		}
	}

	for (i=0;i<dTWINKLE_ITEM_COUNT;i++)
	{
		if	(m_aiTwinkleItem[i]	==	-1)
		{
			m_aiTwinkleItem[i]		=	_iSlot;
			m_aiTwinkleItemFrame[i]	=	0;

			return;
		}
	}
}

int
CHero::getTwinkleFrame(int _iSlot)
{
	int i;

	for (i=0;i<dTWINKLE_ITEM_COUNT;i++)
	{
		if	(m_aiTwinkleItem[i]	==	_iSlot)
			return	m_aiTwinkleItemFrame[i];
	}

	return	-1;
}

void
CHero::updateTwinkleItemFrame()
{
	for (int i=0;i<dTWINKLE_ITEM_COUNT;i++)
	{
		if	(m_aiTwinkleItem[i]	==	-1)
			continue;

		m_aiTwinkleItemFrame[i]++;

		if	(m_aiTwinkleItemFrame[i]	<	dSYNC_FPS)
			continue;

		m_aiTwinkleItemFrame[i]	=	0;
		m_aiTwinkleItem[i]		=	-1;
	}
}
