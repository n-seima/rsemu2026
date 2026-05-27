#include "cItem.H"
#include "cACTOR.H"
#include "CGame.H"
#include "book.H"

//
//	무기 데미지 증가
int
cACTOR::improveWeaponDamage(int _iValue,cItem *_lpDestItem)
{
	cBasicItem	*lpTargetItemBasicData	=	_lpDestItem->getBasicItem();

	if	(lpTargetItemBasicData->m_wMinDamage	==	0)
		return	eUIR_IS_NOT_WEAPON;

	if	(_lpDestItem->getSetItem())
		return	eUIR_NO_MORE_ATTACH_PREFIX;

	int	iPrefixCount	=	0;
	int	iDestPrefix		=	0xffff;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (_lpDestItem->m_aPrefix[i].m_wPrefix	==	0xffff)
			break;
		if (_lpDestItem->m_aPrefix[i].m_wPrefix	==	eIP_ADD_DAMAGE_PERCENTAGE)
		{
			iDestPrefix	=	i;
			break;
		}
		iPrefixCount++;
	}

	if (iDestPrefix	==	0xffff)
		iDestPrefix	=	iPrefixCount;

	if (iDestPrefix	>=	dITEM_PREFIX_COUNT)
		return	eUIR_NO_MORE_ATTACH_PREFIX;	//	보강할 자리가 없다.

	CItemPrefixInfo	*lpPrefix	=	&_lpDestItem->m_aPrefix[iDestPrefix];
	int				iFailChance	=	100-10000/(_iValue*100+65);

	if	(lpPrefix->m_wPrefix	==	eIP_ADD_DAMAGE_PERCENTAGE)
		if (lpPrefix->m_aValue[0]	>=	dMIXIMUM_IMPROVE_WEAPON_DAMAGE_VALUE)
			return	eUIR_NO_MORE_IMPROVE_WEAPON_DAMAGE;	//	한계다.. -_-a

	if (random(100)	<	iFailChance)
	{
//	실패.. -o-
		_lpDestItem->reset();	//	뭔가 효과를 주긴해야지..

		return	eUIR_UPGRADE_FAILED;
	}

//	성공했다!!
	if (lpPrefix->m_wPrefix	==	0xffff)
	{
		lpPrefix->m_wPrefix		=	eIP_ADD_DAMAGE_PERCENTAGE;
		lpPrefix->m_aValue[0]	=	_iValue;
	}
	else
	{
		_iValue					=	min(_iValue+lpPrefix->m_aValue[0],dMIXIMUM_IMPROVE_WEAPON_DAMAGE_VALUE);
		lpPrefix->m_aValue[0]	=	_iValue;
	}

	return	eUIR_OK;
}	//	cACTOR::improveWeaponDamage(cItem *_lpItem,cItem *_lpDestItem)

//
//	갑옷 방어력 증가
int
cACTOR::improveArmorDefense(int _iValue,cItem *_lpDestItem)
{
	cBasicItem	*lpTargetItemBasicData	=	_lpDestItem->getBasicItem();

	if	(lpTargetItemBasicData->m_wKind	!=	eIK_ARMOR	&&	lpTargetItemBasicData->m_wKind	!=	eIK_EXCLUSIVE_ARMOR)
		return	eUIR_IS_NOT_ARMOR;

	if	(_lpDestItem->getSetItem())
		return	eUIR_NO_MORE_ATTACH_PREFIX;

	int	iPrefixCount	=	0;
	int	iDestPrefix		=	0xffff;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (_lpDestItem->m_aPrefix[i].m_wPrefix	==	0xffff)	break;
		if (_lpDestItem->m_aPrefix[i].m_wPrefix	==	eIP_ADD_DEFENSE)
		{
			iDestPrefix	=	i;
			break;
		}
		iPrefixCount++;
	}

	if (iDestPrefix	==	0xffff)	iDestPrefix	=	iPrefixCount;

	if	(iDestPrefix	>=	dITEM_PREFIX_COUNT)
		return	eUIR_NO_MORE_ATTACH_PREFIX;	//	보강할 자리가 없다.

	CItemPrefixInfo	*lpPrefix	=	&_lpDestItem->m_aPrefix[iDestPrefix];
	int					iFailChance	=	100-10000/(_iValue*100+65);

	if (lpPrefix->m_wPrefix	==	eIP_ADD_DEFENSE)
		if (lpPrefix->m_aValue[0]	>=	dMIXIMUM_IMPROVE_ARMOR_DEFENSE_VALUE)	return	eUIR_NO_MORE_IMPROVE_ARMOR_DEFENSE;	//	한계다.. -_-a

	if (random(100)	<	iFailChance)
	{
//	실패.. -o-
		_lpDestItem->reset();	//	뭔가 효과를 주긴해야지..

		return	eUIR_UPGRADE_FAILED;
	}

//	성공했다!!
	if (lpPrefix->m_wPrefix	==	0xffff)
	{
		lpPrefix->m_wPrefix		=	eIP_ADD_DEFENSE;
		lpPrefix->m_aValue[0]	=	_iValue;
	}
	else
	{
		_iValue					=	min(_iValue+lpPrefix->m_aValue[0],dMIXIMUM_IMPROVE_ARMOR_DEFENSE_VALUE);
		lpPrefix->m_aValue[0]	=	_iValue;
	}

	return	eUIR_OK;
}	//	cACTOR::improveWeaponDamage(cItem *_lpItem,cItem *_lpDestItem)

//
//	갑옷 방어력 증가
int
cACTOR::attachPrefix(int _iValue1,int _iValue2,cItem *_lpDestItem)
{
	int	i;

	cBasicItem	*lpTargetItemBasicData	=	_lpDestItem->getBasicItem();

	if	(lpTargetItemBasicData->m_wKind	!=	eIK_ARMOR	&&	lpTargetItemBasicData->m_wKind	!=	eIK_EXCLUSIVE_ARMOR	&&	lpTargetItemBasicData->m_wMinDamage	==	0)
		return	eUIR_CAN_NOT_USE_ON_DEST_ITEM;

	if	(_lpDestItem->getSetItem())
		return	eUIR_NO_MORE_ATTACH_PREFIX;

	int	iPrefixCount	=	0;

	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if	(_lpDestItem->m_aPrefix[i].m_wPrefix	==	0xffff)
			break;

		iPrefixCount++;
	}

	if	(iPrefixCount>=	dITEM_PREFIX_COUNT)
		return	eUIR_NO_MORE_ATTACH_PREFIX;	//	보강할 자리가 없다.

	if	(random(100)	>	_iValue2)
	{
//	실패.. -o-
		_lpDestItem->reset();	//	뭔가 효과를 주긴해야지..

		return	eUIR_UPGRADE_FAILED;
	}

	for (i=iPrefixCount;i<iPrefixCount && i<dITEM_PREFIX_COUNT;i++)	//	접두사 붙여주기
		g_im.attachRandomPrefix(_lpDestItem,i);

	return	eUIR_OK;
}	//	cACTOR::attachPrefix(int _iValue1,int _iValue2,cItem *_lpDestItem)

//
//	대미지 접두사를 바꿔주는 이벤트 접두사
int
cACTOR::changeDamagePrefix20031101(cItem *_lpItem,int _iSrcItemIndex,int _iDestItemIndex)
{
	int	i,iSuccesCount=0;

	cBasicItem	*lpBasicItem	=	_lpItem->getBasicItem();

	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		int	iPrefix	=	_lpItem->m_aPrefix[i].m_wPrefix;
		int	iChangePrefix;

		if	(iPrefix	==	0xffff)
			break;

		if	(iPrefix < 202 || iPrefix > 261)
			if	(iPrefix < 468 || iPrefix > 471)
				continue;

		iChangePrefix	=	91 + ((iPrefix-2)%10);

		if (iPrefix >= 468	&& iPrefix <= 471	)	iChangePrefix	=	459;

		cITEM_PREFIX	*lpChangePrefix	=	&g_aItemPrefix[iChangePrefix];
		cITEM_PREFIX	*lpPrefix		=	&g_aItemPrefix[iPrefix];

		if	(lpChangePrefix->isStickAbleItem(lpBasicItem->m_wKind))	continue;

		int	iPrefix1ValueRange	=	lpPrefix->m_aValue[0][1]-lpPrefix->m_aValue[0][0]+1;
		int	iValue1				=	_lpItem->m_aPrefix[i].m_aValue[0]-lpPrefix->m_aValue[0][0]+1;
		int	iPrefix2ValueRange	=	lpPrefix->m_aValue[1][1]-lpPrefix->m_aValue[1][0]+1;
		int	iValue2				=	_lpItem->m_aPrefix[i].m_aValue[1]-lpPrefix->m_aValue[1][0]+1;
		int	iSourceQuality		=	(iValue1*100/iPrefix1ValueRange+iValue2*100/iPrefix2ValueRange)/2;

		_lpItem->m_aPrefix[i].m_wPrefix	=	iChangePrefix;

		for (int j=0;j<2;j++)
		{
			int	iMinValue,iMaxValue,iRange,iAddValue;

			iMinValue	=	lpChangePrefix->m_aValue[j][0];
			iMaxValue	=	lpChangePrefix->m_aValue[j][1];
			iMaxValue	=	max(iMaxValue,iMinValue);
			iRange		=	iMaxValue-iMinValue;
			iAddValue	=	iSourceQuality*iRange/100;

			_lpItem->m_aPrefix[i].m_aValue[j]	=	iAddValue+iMinValue;
		}

		iSuccesCount++;
	}

	if	(iSuccesCount	<=	0)
		return	eUIR_FAILED_EVENT20031101;

	sendUpdateItemData(_iDestItemIndex,TRUE);

	return	eUIR_OK;
}



int
cACTOR::copyItem(cItem *_lpItem)
{
	if(!_lpItem)
		return	eUIR_FAILED;
	
	cBasicItem	*lpBasicItem	=	_lpItem->getBasicItem();

	if(!lpBasicItem)
		return	eUIR_FAILED;

	if(c_aItemEquipPlace[lpBasicItem->m_wKind] == dITEM_KIND_WASTING || c_aItemEquipPlace[lpBasicItem->m_wKind] == 0xffff)		//  변경..
		return	eCIR_FAILED_BY_NOT_EQUIPMENT;
	if	(_lpItem->isExceptionItem()	||	_lpItem->isExtraItem() || _lpItem->m_isBroken || lpBasicItem->isEternalItem() || lpBasicItem->m_bf1IsRingOfInfinity || _lpItem->getCarvingLevel())
		return	eCIR_FAILED_BY_NOT_EQUIPMENT;	//	장비만 복사되

	if(_lpItem->isExpirationData())
		return	eUIR_FAILED;
	
	if(lpBasicItem->m_bf1IsNotApplyCopyEffect)
		return	eUIR_FAILED;

	cItem	*lpCopyItem	=	getEmptyItemSlot();

	if	(!lpCopyItem)
		return	eUIR_FAILED;

	CPlayerTitleInfo	*lpTitleInfo	=	getTitle(eTN_RED_STONE_HUNTER);

	if	(!lpTitleInfo)
		return	eUIR_FAILED;

	float	fLevel	=	lpTitleInfo->m_bLevel;
	int		iChance	=	(int)(33.0f+fLevel*1.5f);

	if	(random(100)	>=	iChance)
	{
		_lpItem->m_isBroken	=	TRUE;

		sendEnchantItemLog(_lpItem,eEIW_BROKEN_BY_CH5_REWARD);

		return	eCIR_CH5_REWARD_BROKEN;
	}

	lpCopyItem->copy(_lpItem);

	lpCopyItem->m_dwSerial	=	largeRandom(0x7fffffff)+10;

	iChance	=	(int)(fLevel*100/5);	//	거래 가능한 아이템이 나올 확률

	if	(random(10000)	>=	iChance)	//	굴림 실패
	{
		cSpecialItem	*lpSpItem		=	(cSpecialItem	*)lpCopyItem;

		if	(lpCopyItem->isSpecialItem()==	FALSE)
			lpSpItem->reset();

		lpSpItem->m_bf1IsReversionItem	=	TRUE;
	}

	sendCreateItemLog(lpCopyItem,eCIM_CH5_ITEM_COPY);

	sendAddItem(lpCopyItem,eIAR_CH5_REWARD_COPY);

	return	eCIR_CH5_REWARD_COPY_ITEM;
}

//
//	대미지 접두사를 바꿔주는 이벤트 접두사 Delux!!
int
cACTOR::changeDamagePrefix20031117(cItem *_lpItem,int _iSrcItemIndex,int _iDestItemIndex)
{
	int	i,iSuccesCount=0;

	cBasicItem	*lpBasicItem	=	_lpItem->getBasicItem();

	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		int	iPrefix	=	_lpItem->m_aPrefix[i].m_wPrefix;
		int	iChangePrefix;

		if	(iPrefix	==	0xffff)
			break;

		if	(iPrefix < 202 || iPrefix > 261)
			if	(iPrefix < 468 || iPrefix > 471)
				continue;

		iChangePrefix	=	91 + ((iPrefix-2)%10);

		if	(iPrefix >= 468	&& iPrefix <= 471	)
			iChangePrefix	=	459;

		cITEM_PREFIX	*lpChangePrefix	=	&g_aItemPrefix[iChangePrefix];
		cITEM_PREFIX	*lpPrefix		=	&g_aItemPrefix[iPrefix];

		if	(lpChangePrefix->isStickAbleItem(lpBasicItem->m_wKind)	==	FALSE)
		{
			int	iPrefixLevel	=	(iChangePrefix-91)%10;
			iChangePrefix		-=	iPrefixLevel;
			iPrefixLevel--;
			lpChangePrefix		=	NULL;

			for (;iPrefixLevel >= 0;iPrefixLevel--)
			{
				cITEM_PREFIX	*lpNewPrefix=	&g_aItemPrefix[iChangePrefix+iPrefixLevel];

				if	(lpNewPrefix->isStickAbleItem(lpBasicItem->m_wKind)	==	TRUE)
				{
					iChangePrefix	=	iChangePrefix+iPrefixLevel;
					lpChangePrefix	=	lpNewPrefix;
					break;
				}
			}

			if (lpChangePrefix	==	NULL)	continue;
		}

		int	iPrefix1ValueRange	=	lpPrefix->m_aValue[0][1]-lpPrefix->m_aValue[0][0]+1;
		int	iValue1				=	_lpItem->m_aPrefix[i].m_aValue[0]-lpPrefix->m_aValue[0][0]+1;
		int	iPrefix2ValueRange	=	lpPrefix->m_aValue[1][1]-lpPrefix->m_aValue[1][0]+1;
		int	iValue2				=	_lpItem->m_aPrefix[i].m_aValue[1]-lpPrefix->m_aValue[1][0]+1;
		int	iSourceQuality		=	(iValue1*100/iPrefix1ValueRange+iValue2*100/iPrefix2ValueRange)/2;

		_lpItem->m_aPrefix[i].m_wPrefix	=	iChangePrefix;

		for (int j=0;j<2;j++)
		{
			int	iMinValue,iMaxValue,iRange,iAddValue;

			iMinValue	=	lpChangePrefix->m_aValue[j][0];
			iMaxValue	=	lpChangePrefix->m_aValue[j][1];
			iMaxValue	=	max(iMaxValue,iMinValue);
			iRange		=	iMaxValue-iMinValue;
			iAddValue	=	iSourceQuality*iRange/100;

			_lpItem->m_aPrefix[i].m_aValue[j]	=	iAddValue+iMinValue;
		}

		iSuccesCount++;
	}

	if	(iSuccesCount	<=	0)
		return	eUIR_FAILED_EVENT20031101;

	sendUpdateItemData(_iDestItemIndex,TRUE);

	return	eUIR_OK;
}

int
cACTOR::askRemoveItemPrefix(cItem *_lpItem,cItem *_lpDestItem)
{
	if	(_lpDestItem->getPrefixCount()	==	0)
		return	eRIPR_NOT_EXIST_PREFIX_ITEM;

	if	(_lpDestItem->getSetItem())
		return	eRIPR_NOT_EXIST_PREFIX_ITEM;

	cBasicItem	*lpBasicItem	=	_lpItem->getBasicItem();

	if	(!lpBasicItem	||	_lpDestItem->isShopSpecialItem())
		return	eRDIR_FAILED;

	if (lpBasicItem->m_attr.isExclusiveToNormalItem && _lpDestItem->isUniqueItem())	
		return	eRIPR_IS_EXCLUSIVE_NORTMAL_ITEM;
	if (lpBasicItem->m_attr.isExclusiveToUniqueItem && !_lpDestItem->isUniqueItem())	
		return	eRIPR_IS_EXCLUSIVE_UNIQUE_ITEM;

	int	iSrcSlot	= _lpItem		-	m_aItems;
	int	iDestSlot	= _lpDestItem	-	m_aItems;
	
	sendEtcWork(eEW_REMOVE_ITEM_PREFIX,iSrcSlot,iDestSlot);

	return	eRIPR_OK;
}

int
cACTOR::sealSphereOfDawn(cItem *_lpDestItem,cItem *_lpItem,int _iSrcItemIndex,int _iDestItemIndex)
{		// 마지막 수정일 : 09.10.08
	cBasicItem	*lpBasicItem	=	_lpDestItem->getBasicItem();
	if(c_aItemEquipPlace[lpBasicItem->m_wKind] == dITEM_KIND_WASTING || c_aItemEquipPlace[lpBasicItem->m_wKind] == 0xffff)		//  변경..
		return	eUIR_FAILED;
	if	( _lpDestItem->isExceptionItem()	||	_lpDestItem->isExtraItem() || _lpDestItem->m_isBroken || _lpDestItem->isLimitTermItem() || lpBasicItem->isEternalItem())
		return	eUIR_FAILED;

	if	(_lpDestItem->isEnableJob(m_wJob)==FALSE)
		return	eUIR_FAILED;
	
	int	iAbleLevel	=	_lpItem->m_aOption[0];
	iAbleLevel		*=	10;

	if	(!_lpDestItem->isCanUseLevel(iAbleLevel))
		return	eUIR_FAILED;

	if	(!_lpDestItem->isSpecialItem())
		_lpDestItem->makeToSpecialItem();

	cSpecialItem	*lpNewItem	=	_lpDestItem->getSpecialItem();

	if	(lpNewItem->m_bf1IsFreeUseItem)
		return	eUIR_FAILED;

	lpNewItem->m_bf1IsFreeUseItem=	TRUE;
	lpNewItem->m_bf1IsReversionItem=TRUE;

	sendEnchantItemLog(_lpDestItem,eEIW_SEAL_SPHERE_OF_DAWN);

	m_bf4UseSealSphereOfDawnCount++;

	sendUpdateItemData(_iDestItemIndex,TRUE);

	return	eUIR_SUCCESS_TO_USE_SEAL_SPHERE_OF_DAWN;
}

int
cACTOR::addCategoryToBook(cItem *_lpDestItem,cItem *_lpItem)
{
	cBookItem	*lpBookItem	=	_lpDestItem->getBook();

	if	(!lpBookItem)
		return	eBWR_DEST_ITEM_IS_NOT_BOOK;

	cBookCategoryItemDefine	*lpCategoryItem	=	_lpItem->getAddCategoryToBookItem();

	if	(!lpCategoryItem)
		return	eBWR_SOURCE_ITEM_IS_NOT_BOOK_CATEGORY;

	if	(lpBookItem->m_wBookIndex	!=	lpCategoryItem->m_wBookIndex)
		return	eBWR_IS_NOT_SAME_BOOK;

	cBookInfoDefine	*lpBook	=	g_book.getBook(lpBookItem->m_wBookIndex);

	if	(!lpBook)
		return	eBWR_IS_NOT_DEFIEND_BOOK;

	BOOL	bIsWrittenCategory	=	TRUE;
	int		iCategory;

	for	(iCategory=lpCategoryItem->m_wBeginCategory;iCategory<=lpCategoryItem->m_wEndCategory;iCategory++)
	{
		if	(iCategory	>=	lpBook->m_wCategoryCount)
			return	eBWR_INCORRECT_BOOK_DATA;

		cBookCategoryDefine	*lpCategory	=	&lpBook->m_aCategory[iCategory];

		if	(!lpBookItem->isWrittenCategory(iCategory,lpCategory->m_iPageCount))
		{
			bIsWrittenCategory	=	FALSE;
			break;
		}
	}

	if	(bIsWrittenCategory)
		return	eBWR_SOURCE_ITEM_IS_EXIST_BOOK_CATEGORY;

	for	(iCategory=lpCategoryItem->m_wBeginCategory;iCategory<=lpCategoryItem->m_wEndCategory;iCategory++)
		lpBookItem->writeCategory(iCategory);

	return	eUIR_SUCCESS_TO_WRITE_BOOK_CATEGORY;
}

int
cACTOR::addPageToBook(cItem *_lpDestItem,cItem *_lpItem,int _iSrcItemIndex,int _iDestItemIndex)
{
	cBookItem	*lpBookItem	=	_lpDestItem->getBook();

	if	(!lpBookItem)
		return	eBWR_DEST_ITEM_IS_NOT_BOOK;

	cBookPageItemDefine	*lpPageItem	=	_lpItem->getAddPageToBookItem();

	if	(!lpPageItem)
		return	eBWR_SOURCE_ITEM_IS_NOT_BOOK_PAGE;

	if	(lpBookItem->m_wBookIndex	!=	lpPageItem->m_wBookIndex)
		return	eBWR_IS_NOT_SAME_BOOK;

	cBookInfoDefine	*lpBook	=	g_book.getBook(lpBookItem->m_wBookIndex);

	if	(!lpBook)
		return	eBWR_IS_NOT_DEFIEND_BOOK;

	BOOL	bIsWrittenPage	=	TRUE;
	int		iPage;
	int		iCategory		=	lpPageItem->m_wCategory;

	if	(iCategory	>=	lpBook->m_wCategoryCount)
		return	eBWR_INCORRECT_BOOK_DATA;

	cBookCategoryDefine	*lpCategory	=	&lpBook->m_aCategory[iCategory];

	for	(iPage=lpPageItem->m_wBeginPage;iPage<=lpPageItem->m_wEndPage;iPage++)
	{
		if	(iPage	>=	lpCategory->m_iPageCount)
			return	eBWR_INCORRECT_BOOK_DATA;

		if	(!lpBookItem->isWrittenPage(iCategory,iPage))
		{
			bIsWrittenPage	=	FALSE;
			break;
		}
	}

	if	(bIsWrittenPage)
		return	eBWR_SOURCE_ITEM_IS_EXIST_BOOK_PAGE;

	for	(iPage=lpPageItem->m_wBeginPage;iPage<=lpPageItem->m_wEndPage;iPage++)
		lpBookItem->writePage(iCategory,iPage);

	sendUpdateItemData(_iDestItemIndex,TRUE);

	return	eUIR_SUCCESS_TO_WRITE_BOOK_PAGE;
}

int
cACTOR::repairDxItem(cItem *_lpDestItem,int _iRepairFactor)
{
	if	(_lpDestItem->isDXItem()	==	FALSE)
		return	eRDIR_DEST_ITEM_NOT_DX_ITEM;
		
	int	iDurabilityPercentage	=	_lpDestItem->getCurrentDurability();

	if	(iDurabilityPercentage	>	50)
		return	eRDIR_CAN_NOT_REPAIR_DEST_ITEM_BY_HIGH_DURABILITY;

	cBasicItem	*lpBasicItem	=	_lpDestItem->getBasicItem();

	if	(!lpBasicItem)
		return	eRDIR_FAILED;

	int	iFullDurability			=	lpBasicItem->getDurability();

	iFullDurability				=	iFullDurability*_iRepairFactor/100+_lpDestItem->m_bDurability;

	_lpDestItem->m_bDurability	=	min(iFullDurability,255);

	sendEnchantItemLog(_lpDestItem,eEIW_REPAIR_DX_ITEM);

	return	eRDIR_SUCCESS;
}

int
cACTOR::chargeExtraItem(cItem *_lpChargeKit,cItem *_lpDestItem)
{
	if	(!_lpDestItem->isExtraItem() || _lpDestItem->getBasicItem()->isChargeExtraItem())
		return	eUCIR_DEST_ITEM_NOT_EXTRA_ITEM;

	cExtraItemDefine	*lpExItem	=	(cExtraItemDefine	*)_lpDestItem;
	int					iRemainMana	=	lpExItem->getRemainTimeEfficency();

	if (iRemainMana	>	400)
		return	eUCIR_DEST_ITEM_CAN_NOT_CHARGEABLE;

	if (_lpChargeKit->getBasicItem()->m_wExtraGrade	!=	_lpDestItem->getBasicItem()->m_wExtraGrade	)
		return	eUCIR_MISMATCH_EX_GRADE;

	if (iRemainMana	<	0)
	{
		_lpDestItem->m_year		=	g_currentTime.m_wYear-2000;
		_lpDestItem->m_month	=	g_currentTime.m_wMonth;
		_lpDestItem->m_day		=	g_currentTime.m_wDay;
		_lpDestItem->m_hour		=	g_currentTime.m_wHour;
		_lpDestItem->m_minute	=	g_currentTime.m_wMinute;
	}

	_lpDestItem->increaseMinute(dITEM_FULL_CHARGE_TIME);

	return	eUCIR_SUCCESS;	//	성공
}

//
//	짐 모리의 인챈트 서비스
int
cACTOR::jimMorysEnchantService(cItem *_lpEnchantItem,cItem *_lpDestItem)
{
	cBasicItem		*lpTargetItemBasicData	=	_lpDestItem->getBasicItem();
	cBasicItem		*lpEnchantItemBasicData	=	_lpEnchantItem->getBasicItem();

	if	(_lpDestItem->m_isBroken)
		return	eUJMER_CAN_NOT_ATTACH_PREFIX_AT_BROKEN_ITEM;

	if	(!_lpDestItem->isSpecialItem())
		if	(_lpDestItem->m_isCanNotAttachPrefixItem)
			return	eUJMER_CAN_NOT_ATTACH_PREFIX_ITEM;

	if	(_lpDestItem->isShopSpecialItem() || _lpDestItem->isExtraItem())
		return	eUJMER_CAN_NOT_ATTACH_PREFIX_ITEM;

	if	(_lpDestItem->getPrefixCount() >= dITEM_PREFIX_COUNT)
		return	eUJMER_CAN_NOT_ATTACH_PREFIX_BY_PREFIX_FULL;

	if	(_lpDestItem->isExceptionItem() || lpTargetItemBasicData->isEternalItem())
		return	eUJMER_CAN_NOT_ATTACH_PREFIX_ITEM;

	if	(_lpDestItem->getSetItem()	||	lpTargetItemBasicData->m_bf1IsBlockToEnchant)
		return	eUJMER_CAN_NOT_ATTACH_PREFIX_ITEM;

	cItem	item;

	item.copy(_lpDestItem);

	if	(!lpEnchantItemBasicData	||	!lpTargetItemBasicData)
		return	eUJMER_FAILED;

	if (lpEnchantItemBasicData->m_attr.isExclusiveToNormalItem && _lpDestItem->isUniqueItem())	
		return	eUJMER_IS_EXCLUSIVE_NOT_UNIQUE_ITEM;
	if (lpEnchantItemBasicData->m_attr.isExclusiveToUniqueItem && !_lpDestItem->isUniqueItem())	
		return	eUJMER_IS_EXCLUSIVE_UNIQUE_ITEM;

	if(c_aItemEquipPlace[lpTargetItemBasicData->m_wKind] == dITEM_KIND_WASTING || c_aItemEquipPlace[lpTargetItemBasicData->m_wKind] == 0xffff)		//  변경..
		return	eUJMER_CAN_NOT_ATTACH_PREFIX_ITEM;

	int	aPrefixList[]	=	
	{
		101,107,
		201,207,
		301,307,
		401,407,
		501,507,
		601,607,
		701,707,
		1001,1004,
		1101,1104,
		1201,1201,
		1301,1301,
		1601,1604,
		1701,1704,
		1801,1804,
		1901,1904,
		2001,2004,
		2101,2104,
		3101,3102,
		3201,3202,
		3301,3302,
		3401,3402,
		3501,3502,
		3601,3602,
		6102,6102,
		6120,6120,
		6121,6121,
		0xffff
	};

	int	i=0,iPrefixTypeCount=0;
	int	iLowDropLevel=0xffff,iLowDropLevelPrefix;

	for (i=0;;i+=2)
	{
		if (aPrefixList[i]	==	0xffff)	break;

		for (int iIndex=aPrefixList[i];iIndex<=aPrefixList[i+1];iIndex++)
		{
			int				iPrefix		=	g_itemPrefix.getPrefixByDiscernmentCode(iIndex);
			cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[iPrefix];

			if (lpPrefix->m_wDropLevel	<	iLowDropLevel)
			{
				iLowDropLevel		=	lpPrefix->m_wDropLevel;
				iLowDropLevelPrefix	=	iPrefix;
			}
		}

		iPrefixTypeCount++;
	}

	if (m_iLevel / 2 < iLowDropLevel)
		return	eUJMER_TOO_LOW_LEVEL;

//	성공 확률 = 16% + (자신의 운 - 100)*0.06% (최대치 : 50%)

	for (i=1024;i;i--)
	{
		int	iPrefixType		=	random(iPrefixTypeCount);
		int	iPrefixCount	=	(aPrefixList[iPrefixType*2+1] - aPrefixList[iPrefixType*2]+1);
		int	iDiscernmentCode=	aPrefixList[iPrefixType*2]+random(iPrefixCount);
		int	iPrefix			=	g_itemPrefix.getPrefixByDiscernmentCode(iDiscernmentCode);

		if	(i==1)	iPrefix	=	iLowDropLevelPrefix;
		if	(iPrefix==	0xffff)	continue;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[iPrefix];

		if (m_iLevel / 2	<	lpPrefix->m_wDropLevel)	continue;
		if (lpPrefix->isStickAbleItem(lpTargetItemBasicData->m_wKind) == FALSE)	continue;
		if (item.isExistSameTypePrefix(iPrefix))	continue;

		int	iResult	=	g_im.attachPrefix(&item,iPrefix);

		if (iResult	==	eAPR_SUCCESS)
		{
//	성공 확률 = 16% + (자신의 운 - 100)*0.06% (최대치 : 50%)
			int	iMinSuccessChance	=	lpEnchantItemBasicData->m_wEnchantMinChance*100;
			int	iMaxSuccessChance	=	lpEnchantItemBasicData->m_wEnchantMaxChance*100;
			int	iSuccessChance		=	min(iMinSuccessChance+(getLuck()-100)*6,iMaxSuccessChance);
			iSuccessChance			=	max(iSuccessChance,iMinSuccessChance);

			if (random(10000)	<	iSuccessChance)
			{
				_lpDestItem->copy(&item);
				sendEnchantItemLog(_lpDestItem,eEIW_SUCCESS);

				return	eUJMER_OK;
			}

			_lpDestItem->m_isBroken	=	1;
			sendEnchantItemLog(_lpDestItem,eEIW_BROKEN);

			return	eUJMER_ITEM_BROKEN;
		}

		if (iResult	==	eAPR_PREFIX_FULL)
			return	eUJMER_CAN_NOT_ATTACH_PREFIX_BY_PREFIX_FULL;

		if (iResult	==	eAPR_CAN_NOT_ATTACH_PREFIX_AT_BROKEN_ITEM)
			return	eUJMER_CAN_NOT_ATTACH_PREFIX_AT_BROKEN_ITEM;

		if (iResult	==	eAPR_CAN_NOT_ATTACH_PREFIX_ITEM)
			return	eUJMER_CAN_NOT_ATTACH_PREFIX_ITEM;
	}

	return	eUJMER_FAILED;
}	//	cACTOR::jimMorysEnchantService(cItem *_lpEnchantItem,cItem *_lpDestItem)

//
//	유니크 아이템 인챈트
int
cACTOR::enchantItemByUniqueInfo(cItem *_lpItem,cItem *_lpDestItem)
{
	cBasicItem		*lpEnchantItemBasicItem	=	_lpItem->getBasicItem();
	cBasicItem		*lpTargetItemBasicData	=	_lpDestItem->getBasicItem();

	if	(_lpDestItem->m_isBroken)
		return	eUEUIR_CAN_NOT_ATTACH_PREFIX_AT_BROKEN_ITEM;

	if	(!_lpDestItem->isSpecialItem())
		if	(_lpDestItem->m_isCanNotAttachPrefixItem)
			return	eUJMER_CAN_NOT_ATTACH_PREFIX_ITEM;

	if	(_lpDestItem->isExceptionItem() || _lpDestItem->isExtraItem()	||	_lpDestItem->isShopSpecialItem() ||  lpTargetItemBasicData->isEternalItem())
		return	eUEUIR_CAN_NOT_ATTACH_PREFIX_ITEM;

	if	(_lpDestItem->getSetItem()	||	lpTargetItemBasicData->m_bf1IsBlockToEnchant)
		return	eUJMER_CAN_NOT_ATTACH_PREFIX_ITEM;

	BOOL	bIsNewPrefix	=	FALSE;

	cItem			item;
	cUniqueData		*lpUniqueInfo	=	&lpEnchantItemBasicItem->m_aUniqueData[0];
	CItemPrefixInfo	*lpDestPrefixInfo=	NULL;
	BOOL			bIsSuccess		=	FALSE;
	
	if	(!lpEnchantItemBasicItem	||	!lpTargetItemBasicData)
		return	eUEUIR_CAN_NOT_ATTACH_PREFIX_ITEM;

	if	(!lpTargetItemBasicData->isUnique())
		return	eUEUIR_CAN_NOT_ATTACH_PREFIX_ITEM;

	if(c_aItemEquipPlace[lpTargetItemBasicData->m_wKind] == dITEM_KIND_WASTING || c_aItemEquipPlace[lpTargetItemBasicData->m_wKind] == 0xffff)		//  변경..
		return	eUEUIR_CAN_NOT_ATTACH_PREFIX_ITEM;

	if	(lpEnchantItemBasicItem->m_attr.isExclusiveToNormalItem && _lpDestItem->isUniqueItem())	
		return	eUEUIR_IS_EXCLUSIVE_NOT_UNIQUE_ITEM;

	if	(lpEnchantItemBasicItem->m_attr.isExclusiveToUniqueItem && !_lpDestItem->isUniqueItem())
		return	eUEUIR_IS_EXCLUSIVE_UNIQUE_ITEM;

	int	iMinSuccessChance	=	lpEnchantItemBasicItem->m_wEnchantMinChance*100;
	int	iMaxSuccessChance	=	lpEnchantItemBasicItem->m_wEnchantMaxChance*100;
	int	iSuccessChance		=	min(iMinSuccessChance+ (getLuck() - 100)*6,iMaxSuccessChance);
	iSuccessChance			=	max(iSuccessChance,iMinSuccessChance);

	if	(random(10000)	<	iSuccessChance)	
		bIsSuccess	=	TRUE;

	item.copy(_lpDestItem);
	
	int	iSourcePrefixType	=	lpUniqueInfo->m_wEffect;
	int	iValue1				=	lpUniqueInfo->m_aValue[0];
	int	iValue2				=	lpUniqueInfo->m_aValue[1];
	int	iValue3				=	lpUniqueInfo->m_aValue[2];
	int	i;

	if	(lpEnchantItemBasicItem->m_attr.isUpgradePrefixLevel)
	{
		switch(iSourcePrefixType)
		{
			case	eIP_UPGRADE_FIRE_DAMAGE_ATTACK			: // "불 공격 1단계 상승":
				iSourcePrefixType	=	eIP_FIRE_DAMAGE;	//	불 속성 공격
				break;
			case	eIP_UPGRADE_WATER_DAMAGE_ATTACK			: // "물 공격 1단계 상승":
				iSourcePrefixType	=	eIP_WATER_DAMAGE;	//	물 속성 공격
				break;
			case	eIP_UPGRADE_WIND_DAMAGE_ATTACK			: // "바람 공격 1단계 상승":
				iSourcePrefixType	=	eIP_WIND_DAMAGE;	//	바람 속성 공격
				break;
			case	eIP_UPGRADE_EARTH_DAMAGE_ATTACK			: // "대지 공격 1단계 상승":
				iSourcePrefixType	=	eIP_TERRA_DAMAGE;	//	대지 속성 공격
				break;
			case	eIP_UPGRADE_LIGHT_DAMAGE_ATTACK			: // "빛 공격 1단계 상승":
				iSourcePrefixType	=	eIP_LIGHT_DAMAGE;	//	빛 속성 공격
				break;
			case	eIP_UPGRADE_DARK_DAMAGE_ATTACK			: // "어둠 공격 1단계 상승":
				iSourcePrefixType	=	eIP_DARK_DAMAGE;	//	어둠 속성 공격
				break;
		}
	}

	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		int	iDestPrefix	=	item.m_aPrefix[i].m_wPrefix;

		if (iDestPrefix	== 0xffff)	continue;

		int	iDestPrefixType		=	g_aItemPrefix[iDestPrefix].m_wType;

		if (iSourcePrefixType	==	iDestPrefixType)
		{
			lpDestPrefixInfo	=	&item.m_aPrefix[i];
			break;
		}
	}

	if	(lpDestPrefixInfo	==	NULL)
	{
		for (i=0;i<dITEM_PREFIX_COUNT;i++)
		{
			if (item.m_aPrefix[i].m_wPrefix	== 0xffff)
			{
				lpDestPrefixInfo				=	&item.m_aPrefix[i];
				lpDestPrefixInfo->m_aValue[0]	=	0;
				lpDestPrefixInfo->m_aValue[1]	=	0;
				bIsNewPrefix					=	TRUE;

				break;
			}
		}
	}

	if	(lpDestPrefixInfo	==	NULL)
		return	eUEUIR_CAN_NOT_ENCHANT_BY_PREFIX_FULL;

	if (lpEnchantItemBasicItem->m_attr.isUpgradePrefixLevel)
	{
		if (lpDestPrefixInfo->m_wPrefix	==	0xffff)	//	새로운 접두사 추가
		{
			int	iUpgradePrefix			=	g_itemPrefix.getPrefixByType(iSourcePrefixType,0);

			if (g_aItemPrefix[iUpgradePrefix].isStickAbleItem(lpTargetItemBasicData->m_wKind) == FALSE)
				return	eUEUIR_CAN_NOT_ATTACH_THAT_PREFIX_ITEM;

			lpDestPrefixInfo->m_wPrefix	=	iUpgradePrefix;
		}
		else	//	기존 접두사 업그레이드
		{
			int	iCurrentPrefix	=	lpDestPrefixInfo->m_wPrefix;
			int	iLevel			=	g_itemPrefix.getPrefixLevel(lpDestPrefixInfo->m_wPrefix);
			int	iMaxLevel		=	g_itemPrefix.getPrefixCountByType(iSourcePrefixType);

			if (iLevel	>=	iMaxLevel-1)
				return	eUEUIR_CAN_NOT_UPGRADE_PREFIX;

			int	iUpgradePrefix	=	g_itemPrefix.getPrefixByType(iSourcePrefixType,iLevel+1);

			if (g_aItemPrefix[iUpgradePrefix].m_aValue[0][0] < g_aItemPrefix[iCurrentPrefix].m_aValue[0][0])
				return	eUEUIR_CAN_NOT_UPGRADE_PREFIX;

			if (g_aItemPrefix[iUpgradePrefix].isStickAbleItem(lpTargetItemBasicData->m_wKind) == FALSE)
				return	eUEUIR_CAN_NOT_UPGRADE_PREFIX;

			if (g_aItemPrefix[iUpgradePrefix].m_wDiscernmentCode	>	lpEnchantItemBasicItem->m_wEnchantLimitPrefixDiscernmentCode)
				return	eUEUIR_CAN_NOT_UPGRADE_PREFIX;

			lpDestPrefixInfo->m_wPrefix	=	iUpgradePrefix;
		}

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[lpDestPrefixInfo->m_wPrefix];

		for (i=0;i<2;i++)
		{
			int	iMinValue,iMaxValue,iRange,iValue;

			iMinValue	=	lpPrefix->m_aValue[i][0];
			iMaxValue	=	lpPrefix->m_aValue[i][1];
			iMaxValue	=	max(iMaxValue,iMinValue);
			iRange		=	iMaxValue-iMinValue+1;
			iValue		=	min(random(iRange)+iMinValue,iMaxValue);

			lpDestPrefixInfo->m_aValue[i]	=	iValue;
		}

		if (bIsSuccess)
		{
			_lpDestItem->copy(&item);
			sendEnchantItemLog(_lpDestItem,eEIW_SUCCESS);

			return	eUEUIR_OK;
		}

		if (lpTargetItemBasicData->m_wKind == eIK_RING && lpEnchantItemBasicItem->m_attr.isDestroyRingAtFailedEnchant)
		{
			sendEnchantItemLog(_lpDestItem,eEIW_DESTROY);
			_lpDestItem->reset();

			return	eUEUIR_ITEM_DESTROYED;
		}

		_lpDestItem->m_isBroken	=	1;
		sendEnchantItemLog(_lpDestItem,eEIW_BROKEN);

		return	eUEUIR_ITEM_BROKEN;
	}

	int	iUpgradePrefix	=	g_itemPrefix.getPrefixByType(iSourcePrefixType,0);

	if (iUpgradePrefix	==	0xffff)
		return	eUEUIR_CAN_NOT_ATTACH_THAT_PREFIX_ITEM;

	lpDestPrefixInfo->m_wPrefix		=	iUpgradePrefix;

	int	iPValue1	=	lpDestPrefixInfo->m_aValue[0]+iValue1;
	int	iPValue2	=	lpDestPrefixInfo->m_aValue[1]+iValue2;
	int	iOldValue1	=	lpDestPrefixInfo->m_aValue[0];
	int	iOldValue2	=	lpDestPrefixInfo->m_aValue[1];

	lpDestPrefixInfo->m_aValue[0]	=	min(iPValue1,0xff);
	lpDestPrefixInfo->m_aValue[1]	=	min(iPValue2,0xff);

	g_itemPrefix.revisePrefixInfo(lpDestPrefixInfo,lpEnchantItemBasicItem->m_wEnchantLimitPrefixDiscernmentCode);

	if	(iPValue1	>	lpDestPrefixInfo->m_aValue[0]	||	iPValue2	>	lpDestPrefixInfo->m_aValue[1])
		return	eUEUIR_CAN_NOT_UPGRADE_PREFIX;

	if	(g_aItemPrefix[lpDestPrefixInfo->m_wPrefix].m_wDiscernmentCode	>	lpEnchantItemBasicItem->m_wEnchantLimitPrefixDiscernmentCode)
		return	eUEUIR_CAN_NOT_UPGRADE_PREFIX;

	if	(bIsNewPrefix	==	FALSE)
		if	(iOldValue1	==	lpDestPrefixInfo->m_aValue[0]	&&	iOldValue2	==	lpDestPrefixInfo->m_aValue[1])
			return	eUEUIR_CAN_NOT_UPGRADE_PREFIX;

	if	(g_aItemPrefix[lpDestPrefixInfo->m_wPrefix].isStickAbleItem(lpTargetItemBasicData->m_wKind) == FALSE)
	{
		if	(bIsNewPrefix)
			return	eUEUIR_CAN_NOT_ATTACH_THAT_PREFIX_ITEM;
		else
			return	eUEUIR_CAN_NOT_UPGRADE_PREFIX;
	}

	if	(bIsSuccess)
	{
		_lpDestItem->copy(&item);
		sendEnchantItemLog(_lpDestItem,eEIW_SUCCESS);

		return	eUEUIR_OK;
	}

	if	(lpTargetItemBasicData->m_wKind == eIK_RING && lpEnchantItemBasicItem->m_attr.isDestroyRingAtFailedEnchant)
	{
		sendEnchantItemLog(_lpDestItem,eEIW_DESTROY);
		_lpDestItem->reset();

		return	eUEUIR_ITEM_DESTROYED;
	}

	_lpDestItem->m_isBroken	=	1;
	sendEnchantItemLog(_lpDestItem,eEIW_BROKEN);

	return	eUEUIR_ITEM_BROKEN;
}

//
//	저주 아이템 복구
int
cACTOR::repairCursedItem(cItem *_lpItem,cItem *_lpDestItem)
{
	return	eRIR_IS_NOT_INCORRECT_ITEM;

	cBasicItem		*lpRepairItemBasicItem	=	_lpItem->getBasicItem();
	cBasicItem		*lpTargetItemBasicData	=	_lpDestItem->getBasicItem();

	if (lpRepairItemBasicItem->m_attr.isExclusiveToNormalItem && _lpDestItem->isUniqueItem())	
		return	eRIR_IS_EXCLUSIVE_NOT_UNIQUE_ITEM;

	if (lpRepairItemBasicItem->m_attr.isExclusiveToUniqueItem && !_lpDestItem->isUniqueItem())	
		return	eRIR_IS_EXCLUSIVE_UNIQUE_ITEM;

	sendEnchantItemLog(_lpDestItem,eEIW_REPAIR_CURSE);

	return	eRIR_OK;
}

//
//	접두사 막기 아이템 복구
int
cACTOR::repairReleaseBlockPrefixStatus(cItem *_lpItem,cItem *_lpDestItem)
{
	if	(!_lpDestItem->isSpecialItem() && !_lpDestItem->isExceptionItem())
	{
		if (_lpDestItem->m_isCanNotAttachPrefixItem	==	FALSE)
			return	eRIR_IS_NOT_INCORRECT_ITEM;

		return	eRIR_IS_USELESS_ITEM;
	}
	else
		return	eRIR_IS_NOT_INCORRECT_ITEM;

	cBasicItem		*lpRepairItemBasicItem	=	_lpItem->getBasicItem();
	cBasicItem		*lpTargetItemBasicData	=	_lpDestItem->getBasicItem();

	if	(lpRepairItemBasicItem->m_attr.isExclusiveToNormalItem && _lpDestItem->isUniqueItem())	
		return	eRIR_IS_EXCLUSIVE_NOT_UNIQUE_ITEM;

	if	(lpRepairItemBasicItem->m_attr.isExclusiveToUniqueItem && !_lpDestItem->isUniqueItem())	
		return	eRIR_IS_EXCLUSIVE_UNIQUE_ITEM;

	_lpDestItem->m_isCanNotAttachPrefixItem	=	FALSE;

	sendEnchantItemLog(_lpDestItem,eEIW_REPAIR_CAN_NOT_ATTACH_PREFIX_STATUS);

	return	eRIR_OK;
}

//
//	부숴진 아이템 복구
int
cACTOR::repairBrokenItem(cItem *_lpItem,cItem *_lpDestItem)
{
	if	(!_lpDestItem->m_isBroken	||	_lpDestItem->getSetItem()	||	_lpDestItem->isExceptionItem())
	{
		if	(_lpDestItem->m_isBroken	==	FALSE)
			return	eRIR_IS_NOT_INCORRECT_ITEM;

		return	eRIR_IS_USELESS_ITEM;
	}

	cBasicItem		*lpRepairItemBasicItem	=	_lpItem->getBasicItem();
	cBasicItem		*lpTargetItemBasicData	=	_lpDestItem->getBasicItem();

	if (lpRepairItemBasicItem->m_attr.isExclusiveToNormalItem && _lpDestItem->isUniqueItem())	
		return	eRIR_IS_EXCLUSIVE_NOT_UNIQUE_ITEM;
	if (lpRepairItemBasicItem->m_attr.isExclusiveToUniqueItem && !_lpDestItem->isUniqueItem())	
		return	eRIR_IS_EXCLUSIVE_UNIQUE_ITEM;

	_lpDestItem->m_isBroken	=	FALSE;

	if (lpRepairItemBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_REPAIR_BROKEN_ITEM)
		sendEnchantItemLog(_lpDestItem,eEIW_REPAIR_BROKEN);

	return	eRIR_OK;
}

int
cACTOR::unskilledRepairBrokenItem(cItem *_lpItem,cItem *_lpDestItem)
{
	int	iPrefixCount=	_lpDestItem->getPrefixCount();
	int	iResult		=	repairBrokenItem(_lpItem,_lpDestItem);

	if	(iResult	==	eRIR_OK && iPrefixCount)
	{
		for (int i=0;i<dITEM_PREFIX_COUNT;i++)
			_lpDestItem->m_aPrefix[i].m_wPrefix	=	0xffff;

		sendEnchantItemLog(_lpDestItem,eEIW_REPAIR_AND_REMOVE_PREFIX);

		return	eRIR_OK_BUT_REMOVE_PREFIX;
	}

	sendEnchantItemLog(_lpDestItem,eEIW_REPAIR_BROKEN);

	return	iResult;
}

int
cACTOR::attachPrefixToItem(cItem *_lpItem, cItem *_lpDestItem,int _iDestItemIndex, int _iValue1)
{

	if(!_lpDestItem || !_lpItem)
		return eCAPR_STRANGE_ITEM;

	cBasicItem	*lpTargetItemBasicData	=	_lpDestItem->getBasicItem();
	cBasicItem	*lpBasicUseItem			=	_lpItem->getBasicItem();
	BOOL		bIsCanNotTrade			=	FALSE;

	if(!lpTargetItemBasicData || !lpBasicUseItem)
		return eCAPR_STRANGE_ITEM;

	if(!lpTargetItemBasicData->isEquipmentItem())
		return eCAPR_CAN_NOT_ATTACH_PREFIX_ITEM;

	if	(_lpDestItem->getSetItem())
		return	eCAPR_CAN_NOT_ATTACH_PREFIX_ITEM;

	switch(lpBasicUseItem->m_aGenerateData[0].m_wEffect)
	{
		
	case eIE_ATTACH_PREFIX:
		bIsCanNotTrade = FALSE;
		break;
		
	case eIE_ATTACH_PREFIX_CAN_NOT_TRADE:
		bIsCanNotTrade = TRUE;
		break;
		
	default:
		return FALSE;
	}

	cItem item;
	item.copy(_lpDestItem);

	int		iPrefix = 0xffff;
	BYTE	bValue1 = 255;
	BYTE	bValue2 = 255;
	
	iPrefix = _lpItem->m_aPrefix[0].m_wPrefix;
	bValue1 = _lpItem->m_aPrefix[0].m_aValue[0];
	bValue2 = _lpItem->m_aPrefix[0].m_aValue[1];

	cITEM_PREFIX* lpPrefix	=	&g_aItemPrefix[iPrefix];

	if(!lpPrefix)
		return eCAPR_STRANGE_ITEM;

	if(lpPrefix->isStickAbleItem(lpTargetItemBasicData->m_wKind) == FALSE)
		return eCAPR_CAN_NOT_ATTACH_PREFIX_ITEM;

	int iResult = g_im.attachPrefix(&item,iPrefix, bValue1,bValue2);
	
	if(iResult != eAPR_SUCCESS)
	{
		switch(iResult)
		{
			
		case eAPR_EXIST_SAME_PREFIX	:
			return  eCAPR_EXIST_SAME_PREFIX;
			
		case eAPR_CAN_NOT_ATTACH_PREFIX_AT_BROKEN_ITEM	:
			return	eCAPR_CAN_NOT_ATTACH_PREFIX_AT_BROKEN_ITEM;
			
		case eAPR_CAN_NOT_ATTACH_PREFIX_ITEM	:
			return eCAPR_CAN_NOT_ATTACH_PREFIX_ITEM;
			
		case eAPR_PREFIX_FULL	:
			return eCAPR_PREFIX_FULL;
			
		case eAPR_FAILED	:
			return eCAPR_CAN_NOT_ATTACH_PREFIX;
		}
	}

	if	(random(100)	>=	_iValue1)
	{	
		sendUseItemLog(_lpItem);
		_lpItem->reset();
		sendEnchantItemLog(_lpDestItem,eEIW_FAILED_ATTACH_PREFIX);
		_lpDestItem->reset();
		
		arrangeInventory();

		return	eCAPR_FAILED;
	}
	
	if(bIsCanNotTrade)
	{
		cSpecialItem	*lpSpItem		=	(cSpecialItem	*)_lpDestItem;
		
		if	(_lpDestItem->isSpecialItem()==	FALSE)
			lpSpItem->reset();
		
		lpSpItem->m_bf1IsReversionItem	=	TRUE;
	}
	
	iResult = g_im.attachPrefix(_lpDestItem,iPrefix, bValue1,bValue2);
	
	switch(iResult)
	{
	case eAPR_SUCCESS	:		
		{

			sendUseItemLog(_lpItem);
			_lpItem->reset();
			arrangeInventory();
			sendUpdateItemData(_iDestItemIndex,TRUE);
			sendEnchantItemLog(_lpDestItem,eEIW_SUCCESS_ATTACH_PREFIX);
			iResult = eCAPR_OK;		
			break;
		}
		
	case eAPR_EXIST_SAME_PREFIX	:
		return  eCAPR_EXIST_SAME_PREFIX;
		
	case eAPR_CAN_NOT_ATTACH_PREFIX_AT_BROKEN_ITEM	:
		return	eCAPR_CAN_NOT_ATTACH_PREFIX_AT_BROKEN_ITEM;
		
	case eAPR_CAN_NOT_ATTACH_PREFIX_ITEM	:
		return eCAPR_CAN_NOT_ATTACH_PREFIX_ITEM;
		
	case eAPR_PREFIX_FULL	:
		return eCAPR_PREFIX_FULL;
		
	case eAPR_FAILED	:
		return eCAPR_CAN_NOT_ATTACH_PREFIX;
		
	}

	return	iResult;
}	
	
int
cACTOR::askOpenCopyOrCutPrefix(cItem *_lpItem,cItem *_lpDestItem,BOOL _bIsCut)
{
	if	(_lpDestItem->getPrefixCount()	==	0)
		return	eUIR_CAN_NOT_USE_ON_DEST_ITEM;

	if(_bIsCut == FALSE && _lpDestItem->getPrefixCount() ==	dITEM_PREFIX_COUNT)
		return	eCAPR_PREFIX_FULL;

	if	(_lpDestItem->getSetItem())
		return	eUIR_CAN_NOT_USE_ON_DEST_ITEM;


	cBasicItem	*lpBasicItem		=	_lpItem->getBasicItem();
	cBasicItem	*lpBasicDestItem	=	_lpDestItem->getBasicItem();
	if	(!lpBasicItem || !lpBasicDestItem)
		return	eRDIR_FAILED;
	
	if(!lpBasicDestItem->isEquipmentItem())
		return eCAPR_CAN_NOT_ATTACH_PREFIX_ITEM;

	int	iSrcSlot	= _lpItem		-	m_aItems;
	int	iDestSlot	= _lpDestItem	-	m_aItems;
	
	sendEtcWork(eEW_OPEN_COPY_OR_CUT_PREFIX_WINDOW,iSrcSlot,iDestSlot, _bIsCut);

	return	eRIPR_OK;
}

int
cACTOR::increaseCarvingLevel(cItem *_lpItem, cItem *_lpDestItem,int _iSrcItemIndex,int _iDestItemIndex)
{
	if(!_lpDestItem || !_lpItem)
		return eCAPR_STRANGE_ITEM;

	cBasicItem*	lpBasicItem		=	_lpItem->getBasicItem();
	cBasicItem*	lpDestBasicItem	=	_lpDestItem->getBasicItem();

	if(!lpBasicItem || !lpDestBasicItem)
		return eCAPR_STRANGE_ITEM;

	if(!_lpDestItem->isExistSpecificEffect(eIE_CARVING_LEVEL) ||  !lpDestBasicItem->m_bf1IsRingOfInfinity)
	{
		return eCLR_CAN_NOT_CARVE_TO_ITEM;
	}
	if(lpDestBasicItem->m_bf5LimitUpgradeRingOfInfinity  <= _lpDestItem->getCarvingLevel())
	{
		return eCLR_CAN_NOT_CARVE_TO_ITEM;
	}

	int iCarvingLevel	= _lpDestItem->getCarvingLevel();	
	if(!iCarvingLevel)
		return eCLR_CAN_NOT_CARVE_TO_ITEM;
	int iCristalCount	= lpDestBasicItem->getCarveNeedCristalCount(iCarvingLevel - 1);
	LONGLONG llMoney	= lpDestBasicItem->getCarveNeedMoney(iCarvingLevel - 1) * 10000;

	int iCristalCountInInventory = getItemCount(dITEM_INDEX_CRYSTAL);

	if(iCristalCount)
	{
		if( iCristalCount > iCristalCountInInventory)
		{
			return eCLR_NOT_ENOUGH_TO_METERIALS;
		}
	}

	if(llMoney)
	{
		if(m_iGold < llMoney)
			return eCLR_NOT_ENOUGH_TO_METERIALS;
	}

	if(iCristalCount)
	{
		removeItem(dITEM_INDEX_CRYSTAL,iCristalCount, FALSE,eRI_BY_ETERNAL,eSPENT_GOLD_IGNORE);
	}

	m_iGold				-=	llMoney;
	updateGoldLog(-llMoney,eSPENT_GOLD_BY_UPGRADE_CRAVING_LEVEL);

	sendAddGold(-llMoney);
	
	int iResult = eCLR_INCREASE_FAILED;
	float fLuckChance	= getLuck() *  dCARVING_LUCK_FACTOR;
	int iChance			= (_lpItem->getIncreaseCarvingChance() + fLuckChance) * 100;

	int	iIncreaseValue	= 0;	
	if(_lpItem->getIncreaseCarvingValue())
		iIncreaseValue	=	random(_lpItem->getIncreaseCarvingValue())+1;

	int	iRand			=	largeRandom(10000);
	if(isOperator())
	{
		if(m_bf1DevIsCarvingSuccess)
		{
			iChance			= 10000;
			iRand			= 0;
		}
		else if(m_bf1DevIsCarvingFail)
		{
			iChance			= 0;
			iRand			= 10000;
		}
	}
	int	iResultCarvingLevel	= _lpDestItem->getCarvingLevel();
	int	iDestItem			=	_lpDestItem->m_wBaseItem;
	int	iLogResult			=	eEIW_FAILED_INCREASE_CARVING_LEVEL;
	int	iLogUseItemSerial	=	_lpItem->m_wBaseItem;
	_lpItem->decreaseCount(1);
	cItem	logItem;
	logItem.copy(_lpDestItem);

	if(iRand<iChance)
	{
		_lpDestItem->increaseCarvingLevel(iIncreaseValue);
		sendEtcWork(eEW_SUCCESS_INCREASE_CARVING_LEVEL,_iSrcItemIndex,_iDestItemIndex,iIncreaseValue,lpDestBasicItem->getCarveNeedMoney(iCarvingLevel),iCristalCount);
		iResult				=	eCLR_INCREASE_SUCCESS;	
		iResultCarvingLevel =  _lpDestItem->getCarvingLevel();
		iLogResult			=	eEIW_SUCCESS_INCREASE_CARVING_LEVEL;
	}
	else
	{
		_lpDestItem->decreaseCount(1);
	}

	if(iResultCarvingLevel>= 10)
	{
		m_lpField->addSendEtcWork(0xffff,eEW_RESULT_CARVE_LEVEL_MESSAGE, m_wSerialInField ,iDestItem, iResultCarvingLevel, iResult-eCLR_INCREASE_SUCCESS);
		immediatelySendSaveDataToDBMemory();
	}

	sendCarveItemLog(iLogResult,&logItem, iLogUseItemSerial ,logItem.getCarvingLevel(),iResultCarvingLevel,llMoney,iCristalCount);
	
	return iResult;
}

int
cACTOR::setCarvingLevel(cItem *_lpItem, cItem *_lpDestItem)
{

	if(!_lpDestItem || !_lpItem)
		return eCAPR_STRANGE_ITEM;

	cBasicItem*	lpBasicItem		=	_lpItem->getBasicItem();
	cBasicItem*	lpDestBasicItem	=	_lpDestItem->getBasicItem();

	if(!lpBasicItem || !lpDestBasicItem)
		return eCAPR_STRANGE_ITEM;

	if(!_lpDestItem->isExistSpecificEffect(eIE_CARVING_LEVEL) ||  !lpDestBasicItem->m_bf1IsRingOfInfinity)
		return eCLR_CAN_NOT_CARVE_TO_ITEM;

	if(lpDestBasicItem->m_bf5LimitUpgradeRingOfInfinity  < _lpDestItem->getCarvingLevel())
		return eCLR_CAN_NOT_CARVE_TO_ITEM;

	if(lpDestBasicItem->m_bf5LimitUpgradeRingOfInfinity  < _lpItem->getSetCarvingLevel())
		return eCLR_CAN_NOT_CARVE_TO_ITEM;


	if(_lpItem->getSetCarvingLevel() <= _lpDestItem->getCarvingLevel())
		return eCLR_CAN_NOT_CARVE_TO_ITEM;

	int iSetCarvingLevel	= 	_lpItem->getSetCarvingLevel();
	int	iResultCarvingLevel	=	_lpDestItem->getCarvingLevel();
	int	iRand				=	largeRandom(10000);
	int iChance				=	_lpItem->getSetCarvingChance() * 100;
	int iResult				=	eCLR_SET_FAILED;
	int	iLogResult			=	eEIW_FAILED_SET_CARVING_LEVEL;	// 각인 레벨설정 실패
	cItem	logItem;
	logItem.copy(_lpDestItem);

	_lpItem->decreaseCount(1);
	if(iRand < iChance)
	{
		_lpDestItem->setCarvingLevel(iSetCarvingLevel);
		iResult				=	eCLR_SET_SUCCESS;
		iLogResult			=	eEIW_SUCCESS_SET_CARVING_LEVEL;	// 각인 레벨설정 성공
		iResultCarvingLevel =	_lpDestItem->getCarvingLevel(); 
	}
	else
	{		
		_lpDestItem->decreaseCount(1);	

	}

	sendCarveItemLog(iLogResult,&logItem, _lpItem->m_wBaseItem ,logItem.getCarvingLevel(),iResultCarvingLevel,0,0);

	return iResult;
}
//
//	인챈트 결과 처리
int
cACTOR::operateEnchantWorkResult(int _iResult,cItem *_lpItem,cItem *_lpDestItem,int _iSrcItemIndex,int _iDestItemIndex)
{
	if	(	(_iResult >= eUJMER_OK && _iResult <= eUJMER_ITEM_DESTROYED)	||
			(_iResult >= eUEUIR_OK && _iResult <= eUEUIR_ITEM_DESTROYED)	||
			(_iResult >= eRIR_OK   && _iResult <= eRIR_OK_BUT_REMOVE_PREFIX)||
			(_iResult == eUCIR_SUCCESS)	||
			(_iResult == eRDIR_SUCCESS)
		 )
	{
		_lpItem->decreaseCount(1);
		arrangeInventory();

		sendUpdateItemData(_iDestItemIndex,TRUE);
	}

	return	_iResult;
}

//
//	아이템 사용
int
cACTOR::useItemToItem(cItem *_lpItem,cItem *_lpDestItem,int _iSrcItemIndex,int _iDestItemIndex)
{
	cBasicItem		*lpBasicItem	=	_lpItem->getBasicItem();

	if	(!lpBasicItem				)
		return	eUIR_FAILED;
	if	(!isUseAbleItem(_lpItem)	)
		return	eUIR_LACK_ABILITY;
	if	(!_lpItem->isExceptionItem()	&&	_lpItem->m_bCount		<	1	)
		return	eUIR_FAILED;
	if	(_lpDestItem->getSetItem())
		return	eUIR_CAN_NOT_USE_ON_DEST_ITEM;
	if	(g_iWorldServerType	==	eSERVER_TYPE_GVG && lpBasicItem->m_bf1IsNotUseToGVG)
		return	eUIR_CAN_NOT_USE_ITEM;
	if	(checkItemCoolTime(lpBasicItem)	==	FALSE)
	{
		int	iSlot	=	lpBasicItem->m_bCoolTimeIndex;

		sendEtcWork(eEW_SET_ITEM_COOL_TIME,iSlot,m_awItemCoolTime[iSlot]);

		return	eUIR_FAILED_BY_ITEM_COOLTIME;
	}

	if	(lpBasicItem->m_attr.isDurabilityToUseCount		)	//	내구력을 아이템 카운트로 쓰는 경우
	{
		if (_lpItem->m_bDurability	==	0)
		{
			_lpItem->decreaseCount(1,FALSE);

			return	eUIR_FAILED;
		}
	}

	if (!_lpItem->isConsumeItem()						)
		return	eUIR_IS_NOT_CONSUME_ITEM;		//	소비형 아이템이 아니다.

	int		iResult;
	BOOL	bIsLoop	=	TRUE;

	for (int i=0;i<1;i++)
	{
		if (lpBasicItem->m_aGenerateData[i].m_wEffect	==	0xffff)
			break;

		int	iReferenceIndex1	=	lpBasicItem->m_aGenerateData[i].m_aValue[0];
		int	iReferenceIndex2	=	lpBasicItem->m_aGenerateData[i].m_aValue[1];
		int	iValue1				=	_lpItem->m_aOption[iReferenceIndex1];
		int	iValue2				=	_lpItem->m_aOption[iReferenceIndex2];

		switch(lpBasicItem->m_aGenerateData[i].m_wEffect)
		{
			case	eIE_SEAL_SPHERE_OF_DAWN				:
				iResult	=	sealSphereOfDawn(_lpDestItem,_lpItem,_iSrcItemIndex,_iDestItemIndex);
				break;

			case	eIE_ADD_CATEGORY					:
				iResult	=	addCategoryToBook(_lpDestItem,_lpItem);
				break;

			case	eIE_ADD_PAGE						:
				iResult	=	addPageToBook(_lpDestItem,_lpItem,_iSrcItemIndex,_iDestItemIndex);
				break;

			case	eIE_REMOVE_PREFIX					:
			case	eIE_CH5_REMOVE_PREFIX					:
				return	askRemoveItemPrefix(_lpItem,_lpDestItem);

			case	eIE_REPAIR_DX_ITEM_KIT				:
			{
				iResult	=	repairDxItem(_lpDestItem,iValue1);

				return	operateEnchantWorkResult(iResult,_lpItem,_lpDestItem,_iSrcItemIndex,_iDestItemIndex);
			}
			case	eIE_CHARGE_EXTRA_ITEM_POWER			:	//	엑스트라 아이템 충전
			{
				iResult	=	chargeExtraItem(_lpItem,_lpDestItem);

				return	operateEnchantWorkResult(iResult,_lpItem,_lpDestItem,_iSrcItemIndex,_iDestItemIndex);
			}

			case	eIE_JIM_MORYS_ENCHANT				:	//	짐 모리의 인챈트
			{
				iResult	=	jimMorysEnchantService(_lpItem,_lpDestItem);

				return	operateEnchantWorkResult(iResult,_lpItem,_lpDestItem,_iSrcItemIndex,_iDestItemIndex);
			}

			case	eIE_ENCHANT_ITEM_BY_UNIQUE_INFO		:	//	인챈트
			{
				iResult	=	enchantItemByUniqueInfo(_lpItem,_lpDestItem);

				return	operateEnchantWorkResult(iResult,_lpItem,_lpDestItem,_iSrcItemIndex,_iDestItemIndex);
			}

			case	eIE_UNSKILLED_REPAIR_BROKEN_ITEM	:	//	"부숴진 아이템 수리",
			{
				iResult	=	unskilledRepairBrokenItem(_lpItem,_lpDestItem);

				return	operateEnchantWorkResult(iResult,_lpItem,_lpDestItem,_iSrcItemIndex,_iDestItemIndex);
			}

			case	eIE_REPAIR_BROKEN_ITEM				:	//	"부숴진 아이템 수리",
			{
				iResult	=	repairBrokenItem(_lpItem,_lpDestItem);

				return	operateEnchantWorkResult(iResult,_lpItem,_lpDestItem,_iSrcItemIndex,_iDestItemIndex);
			}
			case	eIE_CURE_ITEM_EQUIP_CURSE			:	//	"장비에 붙어 있는 저주를 풀어준다.
			{
				iResult	=	repairCursedItem(_lpItem,_lpDestItem);

				return	operateEnchantWorkResult(iResult,_lpItem,_lpDestItem,_iSrcItemIndex,_iDestItemIndex);
			}

			case	eIE_RELEASE_ATTACH_PREFIX_CURSE		:	//	"장비에 붙어 있는 저주를 풀어준다.
			{
				iResult	=	repairReleaseBlockPrefixStatus(_lpItem,_lpDestItem);

				return	operateEnchantWorkResult(iResult,_lpItem,_lpDestItem,_iSrcItemIndex,_iDestItemIndex);
			}

			case	eIE_INCREASE_WEAPON_DAMAGE					://	"무기 데미지 증가"
			{
				iResult	=	improveWeaponDamage(iValue1,_lpDestItem);
				break;
			}

			case	eIE_INCREASE_ARMOR_DEFENSE					://	"갑옷 방어력 증가"
			{
				iResult	=	improveWeaponDamage(iValue1,_lpDestItem);
				break;
			}

			case	eIE_MAGIC_JEWEL								://	"마법 보석"
			{
				iResult	=	attachPrefix(iValue1,iValue2,_lpDestItem);
				break;
			}

			case	eIE_CHANGE_ITEM_PREFIX_2003_11_01				://	아이템 접두사 변환(마법 대미지->물리 대미지)
			{
				iResult	=	changeDamagePrefix20031101(_lpDestItem,_iSrcItemIndex,_iDestItemIndex);
				break;
			}

			case	eIE_CHANGE_ITEM_PREFIX_2003_11_17				:
			{
				iResult	=	changeDamagePrefix20031117(_lpDestItem,_iSrcItemIndex,_iDestItemIndex);
				break;
			}

			case	eIE_CH5_COPY_ITEM	:
			{
				iResult	=	copyItem(_lpDestItem);
				break;
			}

			case	eIE_ATTACH_PREFIX						:	// %의 확률로 접두사 붙이기
			case	eIE_ATTACH_PREFIX_CAN_NOT_TRADE						:	// %의 확률로 접두사 붙이기
				return attachPrefixToItem(_lpItem, _lpDestItem, _iDestItemIndex, iValue1);

			case	eIE_COPY_PREFIX						:	// 접두사 복사 창 열기
			case	eIE_COPY_PREFIX_CAN_NOT_TRADE		:	// 접두사 복사 창 열기
				return askOpenCopyOrCutPrefix(_lpItem, _lpDestItem, FALSE);

			case	eIE_CUT_PREFIX						:	// 접두사 잘라내기 창 열기
			case	eIE_CUT_PREFIX_CAN_NOT_TRADE		:	// 접두사 잘라내기 창 열기
				return askOpenCopyOrCutPrefix(_lpItem, _lpDestItem, TRUE);

			case	eIE_INCREASE_CARVING_LEVEL		:			//	각인 레벨증가
				return increaseCarvingLevel(_lpItem, _lpDestItem,_iSrcItemIndex,_iDestItemIndex);

			case	eIE_SET_CARVING_LEVEL:						//	 각인 레벨 설정
				return setCarvingLevel(_lpItem, _lpDestItem);

			case	eIE_CH5_IMPROVE_NORMAL_ITEM_POWER		:	//	일반 아이템 파워 증가
			case	eIE_CH5_IMPROVE_UNIQUE_ITEM_POWER		:	//	일반 아이템 파워 증가
			
			default													:
				return	eUIR_NOT_EMBODY_ITEM;
		}
	}

	setLastUseItem(lpBasicItem->m_iSerial);

	if	(iResult	<=	eUIR_DECREASE_ITEM_COUNT_BORDER)
	{
		_lpItem->decreaseCount(1);

		if	(iResult	!=	eCIR_CH5_REWARD_BROKEN	&&	iResult	!=	eCIR_CH5_REWARD_COPY_ITEM)
			sendUpdateItemData(_iSrcItemIndex,TRUE);

		arrangeInventory();
	}

	return	iResult;
}
