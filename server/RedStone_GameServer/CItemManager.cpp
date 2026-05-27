#include	"cItem.H"
#include	"cFIELD.H"
#include	"CShop.H"
#include	"CGame.H"
#include	"debugCode.H"
#include	"itemPackList.H"
#include	"CEvent.H"
#include	"pieceItem.H"
#include	"treasureMapDefine.H"
#include	"book.H"

const	int	c_iDX_ItemDropChanceFactor		=	3;
const	int	c_iUltimateItemDropChanceFactor	=	9;

const	int	c_iUltimateItemGenerateDurability=	75;
const	int	c_iUltimateItemGenerateDurabilityRange=	25;

//
//	로딩된 아이템 데이터를 정리한다.
void
CItemManager::build()
{
	int	i;

	memset(m_aItemCountByType,0,sizeof(m_aItemCountByType));

	m_iBreedingRecordItem	=	-1;
	m_iPieceItem			=	-1;
	m_wSecretDungeonKey		=	0xffff;
	m_wTreasureMap			=	0xffff;
	m_wCoupleRing			=	0xffff;
	m_wPresentCoupleRing	=	0xffff;
	m_wNotDefineCoupleCoupleRing=	0xffff;
	m_wBook					=	0xffff;
	m_wBookCategory			=	0xffff;
	m_wBookPage				=	0xffff;
	m_wPrincessWeapon		=	0xffff;
	m_wCH5BonusHPAmulet		=	0xffff;
	m_wCH5BonusCPAmulet		=	0xffff;
	m_wCH5OverPowerItemAmulet=	0xffff;

	for (i=1;i<dBASIC_ITEM_COUNT;i++)
	{
		cBasicItem	*lpItem	=	&g_aBasicItem[i];

		if	(lpItem->m_iSerial	>=	dBASIC_ITEM_COUNT	||	lpItem->m_iSerial	<	0)
			continue;

		if	(lpItem->m_wBoostDurability	==	0)
			lpItem->m_wBoostDurability	=	100;

		int	iType	=	g_aBasicItem[i].m_wKind;
		int	iCount	=	m_aItemCountByType[iType];

		m_aItemListByType[iType][iCount]	=	i;

		m_aItemCountByType[iType]++;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_DUEL_BUFF_FIRE)
			m_awDuelBuffItem[0]	=	i;
		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_DUEL_BUFF_WATER)
			m_awDuelBuffItem[1]	=	i;
		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_DUEL_BUFF_WIND)
			m_awDuelBuffItem[2]	=	i;
		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_DUEL_BUFF_EARTH)
			m_awDuelBuffItem[3]	=	i;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_CH5_MIX_ITEM_BOX)
			m_wCH5RedBottomlessBox	=	i;
		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_CH5_BONUS_HP_AMULET)
			m_wCH5BonusHPAmulet	=	i;
		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_CH5_BONUS_CP_AMULET)
			m_wCH5BonusCPAmulet	=	i;
		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_CH5_OVER_POWER_ITEM_EQUIP)
			m_wCH5OverPowerItemAmulet	=	i;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_FLAKE_OF_DOING_ONE)
			m_wFlakeOfDoingOne			=	i;
		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_FEATHER_OF_THINKING_ONE)
			m_wFeatherOfThinnkingOne	=	i;
		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_TOY_OF_SIN_AND_RIN)
			m_wToyOfSinandRin	=	i;
		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_TREASURE_MAP)
			m_wTreasureMap			=	i;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_NAME_STONE)
			m_iNameStoneItem	=	i;
		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_BREEDING_RECORD)
			m_iBreedingRecordItem	=	i;
		
		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_MINIPET_POUCH)
			m_iBlockLightPocket	=	i;
		
		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_ITEM_PIECE)
			m_iPieceItem			=	i;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_SECRET_DUNGEON_KEY)
			m_wSecretDungeonKey		=	i;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_COUPLE_RING	)
			m_wCoupleRing			=	i;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_PRESENT_COUPLE_RING)
			m_wPresentCoupleRing	=	i;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_NOT_DEFINE_COUPLE_COUPLE_RING)
			m_wNotDefineCoupleCoupleRing=	i;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_BOOK	)
			m_wBook			=	i;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_PRINCESS_WEAPON	)
			m_wPrincessWeapon=	i;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_ADD_CATEGORY)
			m_wBookCategory	=	i;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_ADD_PAGE)
			m_wBookPage		=	i;

		if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_GUILD_STATUE_PIECE)
		{
			int	iKind	=	lpItem->m_aValue[0][0]-1;
			int	iLevel	=	lpItem->m_aValue[1][0]-1;

			if	(iKind	<	0	||	iLevel	<	0)
				cMSG::Error("error!!","다음 아이템의 수치가 잘못 되었습니다.\n\n%s",lpItem->m_strName);

			m_awGuildStatueElement[iKind][iLevel]	=	(WORD)i;
		}
	}

	for (m_iCommonDropItemCount=0;;m_iCommonDropItemCount++)
		if	(g_aCommonDropItemList[m_iCommonDropItemCount]	==	-1)
			break;
}

BOOL
CItemManager::generateBook(cItem *_lpItem,int _iBookIndex)
{
	if	(m_wBook	==	0xffff)
		return	FALSE;

	_lpItem->m_wBaseItem	=	m_wBook;
	_lpItem->m_dwSerial		=	largeRandom(0x7fffffff)+100;

	cBookItem	*lpBook		=	_lpItem->getBook();

	g_book.writeBook(lpBook,_iBookIndex);

	return	TRUE;
}

BOOL
CItemManager::generateCh5BonusRedBottomlessBox(cItem *_lpItem)
{
	cSpecialItem	*lpSpecialItem	=	(cSpecialItem	*)_lpItem;

	generateItemByBaseItem(_lpItem,m_wCH5RedBottomlessBox,1);

	lpSpecialItem->reset();
	lpSpecialItem->m_bf1IsReversionItem	=	TRUE;

	return	TRUE;
}

BOOL
CItemManager::generateCh5BonusHPAmulet(cItem *_lpItem,int _iTitleLevel)
{
	cSpecialItem	*lpSpecialItem	=	(cSpecialItem	*)_lpItem;

	generateItemByBaseItem(_lpItem,m_wCH5BonusHPAmulet,1);

	_lpItem->m_aOption[0]	=	_iTitleLevel*5;

	lpSpecialItem->reset();

	if	(_iTitleLevel	>=	4)
		lpSpecialItem->m_bf1IsReversionItem	=	TRUE;

	return	TRUE;
}

BOOL
CItemManager::generateCh5BonusCPAmulet(cItem *_lpItem,int _iTitleLevel)
{
	cSpecialItem	*lpSpecialItem	=	(cSpecialItem	*)_lpItem;

	generateItemByBaseItem(_lpItem,m_wCH5BonusCPAmulet,1);

	_lpItem->m_aOption[0]	=	_iTitleLevel*5;

	lpSpecialItem->reset();

	if	(_iTitleLevel	>=	4)
		lpSpecialItem->m_bf1IsReversionItem	=	TRUE;

	return	TRUE;
}

BOOL
CItemManager::generateCh5BonusOverEquipLevelAmulet(cItem *_lpItem,int _iTitleLevel)
{
	cSpecialItem	*lpSpecialItem	=	(cSpecialItem	*)_lpItem;

	generateItemByBaseItem(_lpItem,m_wCH5OverPowerItemAmulet,1);

	_lpItem->m_aOption[0]	=	_iTitleLevel;

	lpSpecialItem->reset();
	lpSpecialItem->m_bf1IsReversionItem	=	TRUE;

	return	TRUE;
}

BOOL
CItemManager::generateBookCategory(cItem *_lpItem,int _iBookIndex,int _iBeginCategory,int _iEndCategory)
{
	if	(m_wBookCategory	==	0xffff)
		return	FALSE;

	_lpItem->m_wBaseItem	=	m_wBookCategory;
	_lpItem->m_dwSerial		=	largeRandom(0x7fffffff)+100;

	cBookCategoryItemDefine	*lpCategory	=	_lpItem->getAddCategoryToBookItem();

	lpCategory->m_wBookIndex		=	_iBookIndex;
	lpCategory->m_wBeginCategory	=	_iBeginCategory;
	lpCategory->m_wEndCategory		=	_iEndCategory;

	return	TRUE;
}

BOOL
CItemManager::generateBookPage(cItem *_lpItem,int _iBookIndex,int _iCategory,int _iBeginPage,int _iEndPage)
{
	if	(m_wBookPage		==	0xffff)
		return	FALSE;

	_lpItem->m_wBaseItem	=	m_wBookPage;
	_lpItem->m_dwSerial		=	largeRandom(0x7fffffff)+100;

	cBookPageItemDefine	*lpPage		=	_lpItem->getAddPageToBookItem();

	lpPage->m_wBookIndex		=	_iBookIndex;
	lpPage->m_wCategory			=	_iCategory;
	lpPage->m_wBeginPage		=	_iBeginPage;
	lpPage->m_wEndPage			=	_iEndPage;

	return	TRUE;
}

int
CItemManager::getTopLevelNormalWeapon(int _iJob,int _iLevel)
{
	int	iMaxLevel	=	0;
	int	iItem		=	0xffff;

	for (int i=0;i<m_iItemCount;i++)
	{
		if	(g_aBasicItem[i].m_iSerial	==	0xffff)
			continue;

		int	iKind	=	g_aBasicItem[i].m_wKind;

		if	(!g_aBasicItem[i].isEnableJob(_iJob))
			continue;
		if	(g_aBasicItem[i].m_wRequireLevel	>	_iLevel)
			continue;
		if	(g_aBasicItem[i].isUnique())
			continue;
		if	(g_aBasicItem[i].m_attr.isExtraItem)
			continue;

		if	(g_aBasicItem[i].m_bf1IsUltimate)
		{
#ifdef	dBLOCKED_TO_DROP_ULTIMATE_ITEM
			continue;
#endif

#ifdef	dIS_DROP_ULTIMATE_ITEM_ONLY_TEST_SERVER	
			if	(g_bIsTestServer	==	FALSE)
				continue;
#endif
		}


		int	iPart	=	c_aItemEquipPlace[iKind];	//	장비 슬롯이 아닌 곳에 올렸다. 적당히 빈자리 찾아서 넣는다.

		if	(iPart	!=	dEQUIP_WEAPON	&&	iPart	!=	dEQUIP_WEAPON_2)
			continue;

		if	(g_aBasicItem[i].m_wRequireLevel	>	iMaxLevel)
		{
			iMaxLevel	=	g_aBasicItem[i].m_wRequireLevel;
			iItem		=	i;
		}
	}

	return	iItem;
}

BOOL
CItemManager::getCh5HeavenRewardUnique(cItem *_lpResultItem,int _iTitleLevel,int _iJob)
{
	cSpecialItem	*lpSpecialItem	=	(cSpecialItem	*)_lpResultItem;

	//	아이템 드롭 레벨 = 100 + 사냥꾼 레벨 * 90
	int	iDropLevel	=	100+_iTitleLevel*90;

	//	사냥꾼 레벨 1~2레벨은 노말 유니크, 3~5레벨은 슈퍼 노말 유니크, 6~7레벨은 딜럭스 유니크, 8~10레벨은 슈퍼 딜럭스 유니크가 된다.

	int	iPrefixLevel=	_iTitleLevel*40;
	int	iPrefixCount=	0;

	//	접두사 드롭 레벨 = 사냥꾼 레벨 * 40
	//	3,8레벨은 접두사 한 개, 4,9레벨은 접두사 두 개, 5,10레벨은 접두사 세 개가 붙는다.
	//	이때 얻은 아이템은 거래 불가능 아이템이다.

	BOOL	bIsDX	=	FALSE,bIsSuper=FALSE;

	if	(_iTitleLevel	>=	8)
	{
		bIsDX		=	TRUE;
		bIsSuper	=	TRUE;
		iPrefixCount=	_iTitleLevel-8+1;
	}
	else
	if	(_iTitleLevel	>=	6)
		bIsDX		=	TRUE;
	else
	if	(_iTitleLevel	>=	3)
	{
		iPrefixCount=	_iTitleLevel-3+1;
		bIsDX		=	FALSE;
		bIsSuper	=	TRUE;
	}

	int	iUniqueItem	=	getUniqueItemByDropLevel(iDropLevel,bIsDX,_iJob);

	if	(iUniqueItem==	-1)
		return	FALSE;

	generateItemByBaseItem(_lpResultItem,iUniqueItem,1,random(101));

	lpSpecialItem->m_dwSerial	=	largeRandom(0x7fffffff)+10;

	lpSpecialItem->reset();
	lpSpecialItem->m_bf1IsReversionItem	=	TRUE;

	if	(bIsSuper)
		if	(!attachCh5PrefixByDropLevel(_lpResultItem,iPrefixLevel,iPrefixCount))
			return	FALSE;

	return	TRUE;
}
/*
//m_bf4ReceiveBonusStateCount
//m_bf4ReceiveBonusSkillPointCount
//m_bf4ReceiveBonusHPPointCount
//m_bf4ReceiveBonusCPPointCount
 	보너스 스탯치:
5의 보너스 스탯 포인트를 준다. 10번의 회수 제한이 있다.
 	보너스 스킬 포인트:
100의 보너스 스킬 포인트를 준다. 10번의 회수 제한이 있다.
 	보너스 HP:
10의 보너스 최대 HP를 준다. 10번의 회수 제한이 있다.
 	보너스 CP:
10의 보너스 최대 CP를 준다. 10번의 회수 제한이 있다.
*/
//
//	드랍레벨과 타입을 기본으로 해서 아이템을 기본 아이템의 인덱스를 얻는다.
int
CItemManager::getItemByDropLevel(int _iType,int _iDropLevel,BOOL _isUnique)
{
	while(1)
	{
		int		i,iSlot,iItem;
		int		aEnableItem[dMAX_ITEM_COUNT_BY_KIND],iCount=0;

		for (i=0;i<m_aItemCountByType[_iType];i++)
		{
			iItem	=	m_aItemListByType[_iType][i];

			if	(g_aBasicItem[iItem].m_wDropLevel	<=	_iDropLevel)
			{
				if	(_isUnique)
					if	(!g_aBasicItem[iItem].isUnique())
						continue;	//	노말 아이템 제외

				if	(!_isUnique)
					if	(g_aBasicItem[iItem].isUnique())
						continue;	//	유니크 아이템 제외

				aEnableItem[iCount++]	=	iItem;
			}
		}

		if	(iCount	<=	0)	
			return	-1;

		iSlot	=	random(iCount);
		iItem	=	aEnableItem[iSlot];

		if	(g_aBasicItem[iItem].m_attr.isDXItem)
		{
			if	(random(c_iDX_ItemDropChanceFactor) != 0)
				continue;
		}

		if	(g_aBasicItem[iItem].m_bf1IsUltimate)
		{
#ifdef	dBLOCKED_TO_DROP_ULTIMATE_ITEM
			continue;
#endif

#ifdef	dIS_DROP_ULTIMATE_ITEM_ONLY_TEST_SERVER
			if	(g_bIsTestServer	==	FALSE)
				continue;
#endif
			if	(random(c_iUltimateItemDropChanceFactor) != 0)
				continue;
		}

		if	(random(1000) >= g_aBasicItem[iItem].m_wCorrectDropChance)
			return	-1;

		return	iItem;
	}

	return	-1;
}


int
CItemManager::getUniqueItemByDropLevel(int _iDropLevel,BOOL _bIsDX,int _iCheckJob)
{
	int		aEnableItem[1024],iCount=0;

	for (int i=0;i<dBASIC_ITEM_COUNT;i++)
	{
		cBasicItem	*lpBasicItem	=	&g_aBasicItem[i];

		if	(lpBasicItem->m_iSerial	==	-1)
			continue;

		if	(lpBasicItem->isUnique()	==	FALSE)
			continue;

		if	(lpBasicItem->m_wDropLevel	>	_iDropLevel)
			continue;

		if	(_iCheckJob	!=	-1)
			if	(lpBasicItem->isEnableJob(_iCheckJob) == FALSE)
				continue;

		if	(_bIsDX)
		{
			if	(lpBasicItem->m_attr.isDXItem	==	FALSE)
				continue;
		}
		else
		{
			if	(lpBasicItem->m_attr.isDXItem)
				continue;
		}

		aEnableItem[iCount++]	=	i;
	}

	if	(iCount	==	0)
		return	-1;

	return	aEnableItem[random(iCount)];
}

BOOL
CItemManager::generateTreasureMap(cItem *_lpItem)
{
	if	(m_wTreasureMap	==	0xffff)
		return	FALSE;

	_lpItem->reset();

	_lpItem->m_bCount	=	1;
	_lpItem->m_wBaseItem=	m_wTreasureMap;
	_lpItem->m_dwSerial	=	largeRandom(0x7fffffff)+2;

	_lpItem->m_year		=	g_currentTime.m_wYear-2000;
	_lpItem->m_month	=	g_currentTime.m_wMonth;
	_lpItem->m_day		=	g_currentTime.m_wDay;
	_lpItem->m_hour		=	g_currentTime.m_wHour;
	_lpItem->m_minute	=	g_currentTime.m_wMinute;
	
	_lpItem->increaseMinute(dTREASURE_MAP_UPKEEP_MINUTE);

	return	TRUE;
}

//
//	드랍레벨과 타입을 기본으로 해서 아이템을 생성한다.
BOOL
CItemManager::setPrincessWeaponOptionValue(cItem *_lpItem,int _iBaseItem)
{
	cBasicItem	*lpBasicItem	=	&g_aBasicItem[_iBaseItem];

	for (int i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
	{
		if	(lpBasicItem->m_aGenerateData[i].m_wEffect	==	0xffff)
			break;

		int	iReferenceIndex,iRange,iMinValue,iMaxValue;

//	옵션 1
		{
			iReferenceIndex	=	lpBasicItem->m_aGenerateData[i].m_aValue[0];
			iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
			iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue;

			_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+random(iRange+1),iMaxValue);
		}

//	옵션 2
		{
			iReferenceIndex	=	lpBasicItem->m_aGenerateData[i].m_aValue[1];
			iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
			iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue;

			_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+random(iRange+1),iMaxValue);
		}
	}

	return	TRUE;
}


//
//	드랍레벨과 타입을 기본으로 해서 아이템을 생성한다.
BOOL
CItemManager::generateItemByType(cItem *_lpItem,int _iType,int _iDropLevel,int _iCount,BOOL _isUnique)
{
	_lpItem->reset();

	if (_iType	==	eIK_ALL_WEAPON)
		_iType	=	c_aWeaponTypeItem[random(dWEAPON_TYPE_ITEM_COUNT)];

	if (_iType	==	eIK_KNIFE	||	_iType	==	eIK_ARROW	||	_iType	==	eIK_BULLET)	
		_iCount	=	random(51)+50;

	int		i,iItem	=	getItemByDropLevel(_iType,_iDropLevel,_isUnique);

	if (iItem	==	-1)
		return FALSE;

	cBasicItem	*lpBasicItem	=	&g_aBasicItem[iItem];

	_lpItem->m_wBaseItem		=	iItem;
	_lpItem->m_bCount			=	_iCount;
	_lpItem->m_bDurability		=	1;

	if (lpBasicItem->m_attr.isDurabilityToUseCount)	
		_lpItem->m_bDurability	=	(BYTE)lpBasicItem->getDurability();

	for (i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
	{
		if (lpBasicItem->m_aGenerateData[i].m_wEffect	==	0xffff)
			break;

		int	iReferenceIndex,iRange,iMinValue,iMaxValue;

//	옵션 1
		{
			iReferenceIndex	=	lpBasicItem->m_aGenerateData[i].m_aValue[0];
			iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
			iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue;

			_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+random(iRange+1),iMaxValue);
		}

//	옵션 2
		{
			iReferenceIndex	=	lpBasicItem->m_aGenerateData[i].m_aValue[1];
			iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
			iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue;

			_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+random(iRange+1),iMaxValue);
		}

		if (lpBasicItem->m_aGenerateData[i].m_wEffect	==	eIE_USE_JEWEL)
			_lpItem->m_bDurability	=	_lpItem->m_aOption[iReferenceIndex];
	}

	if	(_lpItem->isInfinityBullet())
		_lpItem->m_bCount	=	1;

	if	(_lpItem->isDXItem())
	{
		int	iDurability			=	random(25)+50;

		_lpItem->m_bDurability	=	lpBasicItem->getDurability()*iDurability/100;
	}

	if	(_lpItem->isUltimateItem())
	{
		int	iDurability			=	random(c_iUltimateItemGenerateDurability)+c_iUltimateItemGenerateDurabilityRange;

		_lpItem->m_bDurability	=	lpBasicItem->getDurability()*iDurability/100;
	}

	return	TRUE;
}

//	드랍레벨과 타입을 기본으로 해서 노말 아이템을 생성한다.

BOOL
CItemManager::generateArcaItem(cItem *_lpItem,int _iType,int _iDropLevel,int _iCount,int _iArcaLevel)
{
	_lpItem->reset();

	if	(!generateItemByType(_lpItem,_iType,_iDropLevel,_iCount,FALSE))
		return	FALSE;

//	확장 아이템 출현 확률 = 1.9% * (1 + 캐릭터의 장비에 의한 마법 아이템 & 희귀 아이템 확률 증가 보정치 합 + 운 * 0.01) * (몬스터 레벨 ^ 0.5) * 0.1  : [max 75%]
//	확장 아이템 출현 확률 = 2%   * (1 + 캐릭터의 장비에 의한 마법 아이템 & 희귀 아이템 확률 증가 보정치 합 + 운 * 0.01) * (몬스터 레벨 ^ 0.5) * 0.1  : [max 95%]
	{
		BOOL	isRare			=	FALSE;

		if	(random(100) <	3)
			isRare	=	TRUE;
		if	(_iType	==	eIK_JEWEL)
			isRare	=	TRUE;

		if	(isRare)	
			g_im.attachPrefixByDropLevel(_lpItem,_iDropLevel,FALSE,FALSE);
		else
		{
			cBasicItem* lpBasic = _lpItem->getBasicItem();
			if(!lpBasic)
				return FALSE;
			if	(lpBasic->isAttachAblePrefix())		// 수정.
			{	// 접두사를 붙일 수 있는 아이템이면..
				if	(random(100) < 25)
					makeGoodItem(_lpItem,dITEM_LITTLE_GOOD_PREFIX_DROP_LEVEL);
			}
		}
	}

	return	TRUE;
}



//
//	커스텀 아이템을 기반으로 아이템 생성
BOOL
CItemManager::generateItemByCustomItem(cItem *_lpItem,CCustomItem *_lpCustomItem)
{
	int		i,j;

	if (_lpCustomItem->m_wBaseItem	==	0xffff)
		return FALSE;
	
	if (_lpCustomItem->m_bf1IsCustomItem)
	{
		memcpy(_lpItem,_lpCustomItem,sizeof(CItemBaseInfo));

		return TRUE;
	}
	

	cBasicItem	*lpBasicItem	=	&g_aBasicItem[_lpCustomItem->m_wBaseItem];

	_lpItem->m_wBaseItem		=	_lpCustomItem->m_wBaseItem;
	_lpItem->m_dwSerial			=	1;
	_lpItem->m_bCount			=	max(_lpCustomItem->m_bCount,1);

	if	(lpBasicItem->m_attr.isExtraItem	&&	lpBasicItem->isChargeExtraItem() == FALSE)
		_lpCustomItem->m_bf20TermOfValidate	=	dITEM_FULL_CHARGE_TIME;

	if	(_lpCustomItem->m_bf20TermOfValidate)
	{
		_lpItem->m_year		=	g_currentTime.m_wYear-2000;
		_lpItem->m_month	=	g_currentTime.m_wMonth;
		_lpItem->m_day		=	g_currentTime.m_wDay;
		_lpItem->m_hour		=	g_currentTime.m_wHour;
		_lpItem->m_minute	=	g_currentTime.m_wMinute;

		_lpItem->increaseMinute(_lpCustomItem->m_bf20TermOfValidate);
	}

	if	(lpBasicItem->m_attr.isDurabilityToUseCount)
		_lpItem->m_bDurability	=	(BYTE)lpBasicItem->getDurability();

	if	(_lpItem->isDXItem())
	{
		int	iDurability			=	random(25)+50;

		_lpItem->m_bDurability	=	lpBasicItem->getDurability()*iDurability/100;
	}

	if	(_lpItem->isUltimateItem())
	{
		int	iDurability			=	random(c_iUltimateItemGenerateDurability)+c_iUltimateItemGenerateDurabilityRange;

		_lpItem->m_bDurability	=	lpBasicItem->getDurability()*iDurability/100;
	}

	if (_lpItem->isItemPack())
		_lpItem->m_aOption[0]	=	(BYTE)_lpCustomItem->m_wGenerateQuality;
	else
	for (i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
	{
		if (lpBasicItem->m_aGenerateData[i].m_wEffect	==	0xffff)
			break;

		int	iReferenceIndex,iRange,iMinValue,iMaxValue;

//	옵션 1
		{
			iReferenceIndex	=	lpBasicItem->m_aGenerateData[i].m_aValue[0];
			iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
			iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue;

			_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+iRange*_lpCustomItem->m_wGenerateQuality/100,iMaxValue);
		}

//	옵션 2
		{
			iReferenceIndex	=	lpBasicItem->m_aGenerateData[i].m_aValue[1];
			iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
			iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue;

			_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+iRange*_lpCustomItem->m_wGenerateQuality/100,iMaxValue);
		}

		if (lpBasicItem->m_aGenerateData[i].m_wEffect	==	eIE_USE_JEWEL)
			_lpItem->m_bDurability	=	_lpItem->m_aOption[iReferenceIndex];
	}

	if	(_lpCustomItem->m_wIsAttachRandomPrefix)
	{
		attachPrefixByDropLevel(_lpItem,_lpCustomItem->m_wDropLevel,TRUE);

		return	TRUE;
	}

	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		int	iPrefix	=	_lpCustomItem->m_aPrefix[i].m_wPrefix;

		if (iPrefix	==	0xffff)
			break;

		_lpItem->m_aPrefix[i].m_wPrefix		=	iPrefix;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[iPrefix];

		for (j=0;j<2;j++)
		{
			int	iMinValue,iMaxValue,iRange;
			
			iMinValue	=	lpPrefix->m_aValue[j][0];
			iMaxValue	=	lpPrefix->m_aValue[j][1];
			iMaxValue	=	max(iMaxValue,iMinValue);
			iRange		=	iMaxValue-iMinValue;

			_lpItem->m_aPrefix[i].m_aValue[j]	=	min(iMinValue+iRange*_lpCustomItem->m_aPrefixGenerateQuality[i]/100,iMaxValue);
		}
	}

	return	TRUE;
}

//
//	프리미엄 아이템 생성
BOOL
CItemManager::generatePremiumItem(cItem *_lpItem,int _iPremiumItemIndex)
{
	CCustomItem	*lpCustomItem	=	g_premiumItemManager.getItem(_iPremiumItemIndex);

	if	(!lpCustomItem)
		return	FALSE;
	if	(!generateItemByCustomItem(_lpItem,lpCustomItem))
		return	FALSE;
	
	_lpItem->m_bDurability	=	_lpItem->getBasicItem()->getDurability();

	return	TRUE;
}

BOOL
CItemManager::generateItemPiece(cItem *_lpItem,int _iSelectPieceItem,int _iPiece,BOOL _bIsInSecretDungeon,BOOL _bIsInBox,int _iFieldIndex,BOOL _bIsByQuest)
{
	cPieceItem			*lpPieceItem	=	g_pieceItemManager.get(_iSelectPieceItem);

	if	(!lpPieceItem)
		return	FALSE;

	if	(_iPiece	==	-1)
		_iPiece		=	random(lpPieceItem->m_wPieceCount);

	cItemPieceDefine	*lpItemPiece	=	&lpPieceItem->m_aPiece[_iPiece];

	BOOL				bIsDropAble		=	FALSE;

	if	(lpItemPiece->m_bf1IsDropInNormalFieldArca)
	{
		if	(_bIsInSecretDungeon	==	FALSE	&&	_bIsInBox	==	TRUE)
			bIsDropAble		=	TRUE;
	}
	if	(lpItemPiece->m_bf1IsDropInTempDungeonMonster)
	{
		if	(_bIsInSecretDungeon	==	TRUE	&&	_bIsInBox	==	FALSE)
			bIsDropAble		=	TRUE;
	}
	if	(lpItemPiece->m_bf1IsDropInTempDungeonArca)
	{
		if	(_bIsInSecretDungeon	==	TRUE	&&	_bIsInBox	==	TRUE)
			bIsDropAble		=	TRUE;
	}
	if	(lpItemPiece->m_bf1IsDropInNormalFieldMonster)
	{
		if	(_bIsInSecretDungeon	==	FALSE	&&	_bIsInBox	==	FALSE)
			bIsDropAble		=	TRUE;
	}

	if	(_iFieldIndex	==	28)
		bIsDropAble		=	TRUE;

	if	(_bIsByQuest)
		bIsDropAble		=	TRUE;

	if	(bIsDropAble	==	FALSE)
		return	FALSE;

	_lpItem->reset();

	_lpItem->m_bf1IsSpecialItem			=	TRUE;

	cSpecialItem	*lpSpecialItem		=	_lpItem->getSpecialItem();

	lpSpecialItem->reset();

	lpSpecialItem->m_wBaseItem			=	m_iPieceItem;
	lpSpecialItem->m_bCount				=	1;
	lpSpecialItem->m_aOption[0]			=	_iPiece;
	lpSpecialItem->m_bf11SValue			=	_iSelectPieceItem;
	lpSpecialItem->m_bDurability		=	1;
	lpSpecialItem->m_bf1IsUnknown		=	TRUE;
	lpSpecialItem->m_bf1IsReversionItem	=	lpPieceItem->m_aPiece[_iPiece].m_bf1IsReversion;

	return	TRUE;
}

//
//	드랍레벨과 타입을 기본으로 해서 아이템을 생성한다.
BOOL
CItemManager::generateItemByBaseItem(cItem *_lpItem,int _iBaseItem,int _iCount,int _iEfficiency, int* _iOption)
{
	int		i;

	if	(_iBaseItem	==	0xffff	||	_iBaseItem	<	0	||	_iBaseItem	>=	dBASIC_ITEM_COUNT)
		return	FALSE;

	cBasicItem	*lpBasicItem	=	&g_aBasicItem[_iBaseItem];

	_lpItem->m_wBaseItem		=	_iBaseItem;
	_lpItem->m_bCount			=	_iCount;

	if	(lpBasicItem->m_attr.isDurabilityToUseCount)	
		_lpItem->m_bDurability	=	(BYTE)lpBasicItem->getDurability();

	for (i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
	{
		if	(lpBasicItem->m_aGenerateData[i].m_wEffect	==	0xffff)
			break;

		int	iReferenceIndex,iRange,iMinValue,iMaxValue;

//	옵션 1
		{
			iReferenceIndex	=	lpBasicItem->m_aGenerateData[i].m_aValue[0];
			if(_iOption)
			{
				if(iReferenceIndex == 0)
				{
					iMinValue		=	_iOption[0];
					iMaxValue		=	_iOption[1];
				}
				else
				{
					iMinValue		=	_iOption[2];
					iMaxValue		=	_iOption[3];
				}
			}
			else
			{
				iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
				iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			}
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue;

			if	(_iEfficiency	==	-1)
				_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+random(iRange+1),iMaxValue);
			else
				_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+iRange*_iEfficiency/100,iMaxValue);
		}

//	옵션 2
		{
			iReferenceIndex	=	lpBasicItem->m_aGenerateData[i].m_aValue[1];

			if(_iOption)
			{
				if(iReferenceIndex == 0)
				{
					iMinValue		=	_iOption[0];
					iMaxValue		=	_iOption[1];
				}
				else
				{
					iMinValue		=	_iOption[2];
					iMaxValue		=	_iOption[3];
				}
			}
			else
			{		
				iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
				iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			}

			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue;

			if	(_iEfficiency	==	-1)
				_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+random(iRange+1),iMaxValue);
			else
				_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+iRange*_iEfficiency/100,iMaxValue);
		}

		if	(lpBasicItem->m_aGenerateData[i].m_wEffect	==	eIE_USE_JEWEL)
			_lpItem->m_bDurability	=	_lpItem->m_aOption[iReferenceIndex];
	}

	if	(_lpItem->isInfinityBullet())
		_lpItem->m_bCount	=	1;

	if	(_lpItem->isDXItem())
	{
		int	iDurability			=	random(25)+50;

		_lpItem->m_bDurability	=	lpBasicItem->getDurability()*iDurability/100;
	}

	if	(_lpItem->isUltimateItem())
	{
		int	iDurability			=	random(c_iUltimateItemGenerateDurability)+c_iUltimateItemGenerateDurabilityRange;

		_lpItem->m_bDurability	=	lpBasicItem->getDurability()*iDurability/100;
	}

	return	TRUE;
}
//
//	드랍레벨과 타입을 기본으로 해서 아이템을 생성한다.
BOOL
CItemManager::generateShopItem(cItem *_lpItem,CShopGenerateItemInfo *_lpGenerateItemInfo)
{
	int		i,j;

	if	(_lpGenerateItemInfo->m_wBaseItem==	0xffff)
		return FALSE;

	cBasicItem	*lpBasicItem	=	&g_aBasicItem[_lpGenerateItemInfo->m_wBaseItem];
	BOOL		bIsJewel		=	FALSE;

	_lpItem->m_wBaseItem		=	_lpGenerateItemInfo->m_wBaseItem;
	_lpItem->m_bCount			=	_lpGenerateItemInfo->m_bCount;

	if	(lpBasicItem->m_attr.isDurabilityToUseCount)	
		_lpItem->m_bDurability	=	(BYTE)lpBasicItem->getDurability();

	for (i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
	{
		if	(lpBasicItem->m_aGenerateData[i].m_wEffect	==	0xffff)
			break;

		int	iReferenceIndex,iRange,iMinValue,iMaxValue;

//	옵션 1
		{
			iReferenceIndex	=	lpBasicItem->m_aGenerateData[i].m_aValue[0];
			iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
			iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue;

			_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+iRange*_lpGenerateItemInfo->m_wGenerateQuality/100,iMaxValue);
		}

//	옵션 2
		{
			iReferenceIndex	=	lpBasicItem->m_aGenerateData[i].m_aValue[1];
			iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
			iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue;

			_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+iRange*_lpGenerateItemInfo->m_wGenerateQuality/100,iMaxValue);
		}

		if	(lpBasicItem->m_aGenerateData[i].m_wEffect	==	eIE_USE_JEWEL)
		{
			bIsJewel				=	TRUE;
			_lpItem->m_bDurability	=	_lpItem->m_aOption[iReferenceIndex];
		}
	}

	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		int	iPrefix	=	_lpGenerateItemInfo->m_aPrefix[i].m_wPrefix;

		if (iPrefix	==	0xffff)	break;

		_lpItem->m_aPrefix[i].m_wPrefix		=	iPrefix;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[iPrefix];

		for (j=0;j<2;j++)
		{
			int	iMinValue,iMaxValue,iRange;
			
			iMinValue	=	lpPrefix->m_aValue[j][0];
			iMaxValue	=	lpPrefix->m_aValue[j][1];
			iMaxValue	=	max(iMaxValue,iMinValue);
			iRange		=	iMaxValue-iMinValue;

			_lpItem->m_aPrefix[i].m_aValue[j]	=	min(iMinValue+iRange*_lpGenerateItemInfo->m_wPrefixQuality/100,iMaxValue);
		}
	}

	if	(_lpItem->isBreakdownAbleItem()	&&	bIsJewel	==	FALSE)
		_lpItem->m_bDurability	=	(BYTE)lpBasicItem->getDurability();

	if	(_lpGenerateItemInfo->m_bf10ChangeLimitLevel)
	{
		cSpecialItem	*lpSpecialItem	=	(cSpecialItem	*)_lpItem;

		lpSpecialItem->reset();

		lpSpecialItem->m_bf10EquipLevel	=	_lpGenerateItemInfo->m_bf10ChangeLimitLevel;
	}

	return	TRUE;
}

//
//	접두사 붙이기
void
CItemManager::attachPrefixByDropLevel(cItem *_lpItem,int _iDropLevel,BOOL _bIsRare,BOOL _bIsSpecialDropChance)
{
	int	iPrefixCount	=	0,i,j;
	int	iRandomValue	=	random(100);

	if (_lpItem->isUniqueItem())
	{
		if (iRandomValue<93							)	iPrefixCount	=	0;
		if (iRandomValue>=93&&	iRandomValue<	97	)	iPrefixCount	=	1;
		if (iRandomValue>=97&&	iRandomValue<	99	)	iPrefixCount	=	2;
		if (iRandomValue>=99						)	iPrefixCount	=	3;
	}
	else
	{
		if (iRandomValue>=0	&&	iRandomValue<	80	)	iPrefixCount	=	1;
		if (iRandomValue>=80&&	iRandomValue<	95	)	iPrefixCount	=	2;
		if (iRandomValue>=95						)	iPrefixCount	=	3;
	}

	cBasicItem	*lpBasicItem	=	_lpItem->getBasicItem();
	BOOL		bIsNoLimitPrefix=	FALSE;

//	if (g_bIsInnerTestServer)	
//		iPrefixCount	=	3;

	for (i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
	{
		if (lpBasicItem->m_aGenerateData[i].m_wEffect	==	eIE_FREE_RARE_OPTION)
		{
			bIsNoLimitPrefix	=	TRUE;
			iPrefixCount		=	min(iPrefixCount,_lpItem->m_aOption[0]);
		}
	}

	if (iPrefixCount	<=	0)
	{
		if (_bIsRare)
			iPrefixCount	=	1;
		else
			return;	//	접두사 붙을게 없다.
	}

	int	aOrder[6][3]	=	{	{0,1,2},{0,2,1},{1,0,2},{1,2,0},{2,0,1},{2,1,0}		};
	int	iOrder			=	random(6);
	int	iItemType		=	_lpItem->getItemType();
	int	iPrefixIndex	=	0;

	for (i=0;i<iPrefixCount;i++)
	{
		int	iPrefixOrder=	aOrder[iOrder][i];

		if	(bIsNoLimitPrefix)
			iPrefixOrder=	random(2);

		int	iPrefix	=	g_itemPrefix.getPrefixByDropLevel(_iDropLevel,iPrefixOrder,iItemType,_bIsSpecialDropChance);

		for (j=0;j<dITEM_PREFIX_COUNT;j++)
			if (iPrefix	==	_lpItem->m_aPrefix[j].m_wPrefix)
				iPrefix	=	0xffff;

		if (iPrefix	<	0	||	iPrefix	>=	g_itemPrefix.m_iCount)
			continue;

		_lpItem->m_aPrefix[iPrefixIndex].m_wPrefix		=	iPrefix;
		_lpItem->m_aPrefix[iPrefixIndex].m_aValue[0]	=	0;
		_lpItem->m_aPrefix[iPrefixIndex].m_aValue[1]	=	0;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[iPrefix];

		for (j=0;j<2;j++)
		{
			int	iMinValue,iMaxValue,iRange,iQuality,iCorrectQuality,iValue;

			iMinValue		=	lpPrefix->m_aValue[j][0];
			iMaxValue		=	lpPrefix->m_aValue[j][1];
			iMaxValue		=	max(iMaxValue,iMinValue);
			iRange			=	iMaxValue-iMinValue;
			iCorrectQuality	=	lpPrefix->m_saCorrectQualityPrefix[j];

			for (int iTryCount=0;iTryCount<10;iTryCount++)
			{
				iQuality		=	random(100)+1;

				if (random(100) < abs(iCorrectQuality)*100/iQuality)
					break;
				else	
					iQuality	=	1;
			}

			int	iAddValue	=	iQuality*iRange/100;

			if ((iQuality*iRange%50) >= 50)
				iAddValue++;

			if (iCorrectQuality	<	0)
				iAddValue	=	iRange-iAddValue;

			iValue			=	iMinValue + iAddValue;
			iValue			=	max(iValue,iMinValue);
			iValue			=	min(iValue,iMaxValue);

			_lpItem->m_aPrefix[iPrefixIndex].m_aValue[j]	=	iMinValue + iAddValue;
		}

		BOOL	bIsIncorrectPrefix	=	FALSE;

		if (lpBasicItem->m_wRequireStrength		>	_lpItem->getFixedStrength()		)
			bIsIncorrectPrefix	=	TRUE;
		if (lpBasicItem->m_wRequireDexterity	>	_lpItem->getFixedAgility()		)
			bIsIncorrectPrefix	=	TRUE;
		if (lpBasicItem->m_wRequireConstitution	>	_lpItem->getFixedConstitution()	)
			bIsIncorrectPrefix	=	TRUE;
		if (lpBasicItem->m_wRequireWisdom		>	_lpItem->getFixedWisdom()		)
			bIsIncorrectPrefix	=	TRUE;
		if (lpBasicItem->m_wRequireIntelligence	>	_lpItem->getFixedIntelligence()	)
			bIsIncorrectPrefix	=	TRUE;
		if (lpBasicItem->m_wRequireCharisma		>	_lpItem->getFixedCharisma()		)
			bIsIncorrectPrefix	=	TRUE;
		if (lpBasicItem->m_wRequireLuck			>	_lpItem->getFixedLuck()			)
			bIsIncorrectPrefix	=	TRUE;

		if (bIsIncorrectPrefix)
		{
			_lpItem->m_aPrefix[iPrefixIndex].m_wPrefix		=	0xffff;
			_lpItem->m_aPrefix[iPrefixIndex].m_aValue[0]	=	0;
			_lpItem->m_aPrefix[iPrefixIndex].m_aValue[1]	=	0;

			continue;
		}

		iPrefixIndex++;
	}

	if (_lpItem->isInfinityBullet())
		_lpItem->m_bCount	=	1;
}

BOOL
CItemManager::attachCh5PrefixByDropLevel(cItem *_lpItem,int _iDropLevel,int _iPrefixCount)
{
	cBasicItem	*lpBasicItem	=	_lpItem->getBasicItem();
	BOOL		bIsNoLimitPrefix=	FALSE;
	int			i,j;

	for (i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
	{
		if	(lpBasicItem->m_aGenerateData[i].m_wEffect	==	eIE_FREE_RARE_OPTION)
		{
			bIsNoLimitPrefix	=	TRUE;
		}
	}

	int	aOrder[6][3]	=	{	{0,1,2},{0,2,1},{1,0,2},{1,2,0},{2,0,1},{2,1,0}		};
	int	iOrder			=	random(6);
	int	iItemType		=	_lpItem->getItemType();
	int	iPrefixIndex	=	0;

	for (i=0;i<1000;i++)
	{
		if	(iPrefixIndex	>=	_iPrefixCount)
			break;

		int	iPrefixOrder=	aOrder[iOrder][iPrefixIndex];

		if	(bIsNoLimitPrefix)
			iPrefixOrder=	random(2);

		int	iPrefix	=	g_itemPrefix.getPrefixByDropLevel(_iDropLevel,iPrefixOrder,iItemType,FALSE);

		for (j=0;j<dITEM_PREFIX_COUNT;j++)
			if	(iPrefix	==	_lpItem->m_aPrefix[j].m_wPrefix)
				iPrefix		=	0xffff;

		if	(iPrefix	<	0	||	iPrefix	>=	g_itemPrefix.m_iCount)
			continue;

		_lpItem->m_aPrefix[iPrefixIndex].m_wPrefix		=	iPrefix;
		_lpItem->m_aPrefix[iPrefixIndex].m_aValue[0]	=	0;
		_lpItem->m_aPrefix[iPrefixIndex].m_aValue[1]	=	0;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[iPrefix];

		for (j=0;j<2;j++)
		{
			int	iMinValue,iMaxValue,iRange,iQuality,iCorrectQuality,iValue;

			iMinValue		=	lpPrefix->m_aValue[j][0];
			iMaxValue		=	lpPrefix->m_aValue[j][1];
			iMaxValue		=	max(iMaxValue,iMinValue);
			iRange			=	iMaxValue-iMinValue;
			iCorrectQuality	=	lpPrefix->m_saCorrectQualityPrefix[j];

			for (int iTryCount=0;iTryCount<10;iTryCount++)
			{
				iQuality		=	random(100)+1;

				if (random(100) < abs(iCorrectQuality)*100/iQuality)
					break;
				else	
					iQuality	=	1;
			}

			int	iAddValue	=	iQuality*iRange/100;

			if	((iQuality*iRange%50) >= 50)
				iAddValue++;

			if	(iCorrectQuality	<	0)
				iAddValue	=	iRange-iAddValue;

			iValue			=	iMinValue + iAddValue;
			iValue			=	max(iValue,iMinValue);
			iValue			=	min(iValue,iMaxValue);

			_lpItem->m_aPrefix[iPrefixIndex].m_aValue[j]	=	iMinValue + iAddValue;
		}

		BOOL	bIsIncorrectPrefix	=	FALSE;

		if	(lpBasicItem->m_wRequireStrength		>	_lpItem->getFixedStrength()		)
			bIsIncorrectPrefix	=	TRUE;
		if	(lpBasicItem->m_wRequireDexterity	>	_lpItem->getFixedAgility()		)
			bIsIncorrectPrefix	=	TRUE;
		if	(lpBasicItem->m_wRequireConstitution	>	_lpItem->getFixedConstitution()	)
			bIsIncorrectPrefix	=	TRUE;
		if	(lpBasicItem->m_wRequireWisdom		>	_lpItem->getFixedWisdom()		)
			bIsIncorrectPrefix	=	TRUE;
		if	(lpBasicItem->m_wRequireIntelligence	>	_lpItem->getFixedIntelligence()	)
			bIsIncorrectPrefix	=	TRUE;
		if	(lpBasicItem->m_wRequireCharisma		>	_lpItem->getFixedCharisma()		)
			bIsIncorrectPrefix	=	TRUE;
		if	(lpBasicItem->m_wRequireLuck			>	_lpItem->getFixedLuck()			)
			bIsIncorrectPrefix	=	TRUE;

		if	(bIsIncorrectPrefix)
		{
			_lpItem->m_aPrefix[iPrefixIndex].m_wPrefix		=	0xffff;
			_lpItem->m_aPrefix[iPrefixIndex].m_aValue[0]	=	0;
			_lpItem->m_aPrefix[iPrefixIndex].m_aValue[1]	=	0;

			continue;
		}

		iPrefixIndex++;

		if	(iPrefixIndex	>=	3)
			break;
	}

	if	(_lpItem->isInfinityBullet())
		_lpItem->m_bCount	=	1;

	if	(iPrefixIndex)
		return	TRUE;

	return	FALSE;
}

//
//	
int
CItemManager::attachPrefix(cItem *_lpDestItem,int _iPrefix, BYTE _bValue1, BYTE _bValue2 )
{
	int	iPrefixPos	=	0xffff;

	if (_lpDestItem->m_isBroken)
		return	eAPR_CAN_NOT_ATTACH_PREFIX_AT_BROKEN_ITEM;

	if	(!_lpDestItem->isSpecialItem())
		if	(_lpDestItem->m_isCanNotAttachPrefixItem)
			return	eAPR_CAN_NOT_ATTACH_PREFIX_ITEM;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (_lpDestItem->m_aPrefix[i].m_wPrefix	==	0xffff && iPrefixPos == 0xffff)
			iPrefixPos	=	i;

		if (_lpDestItem->m_aPrefix[i].m_wPrefix	==	_iPrefix)
			return	eAPR_EXIST_SAME_PREFIX;
	}

	if(iPrefixPos	==	0xffff)	
		return	eAPR_PREFIX_FULL;

	_lpDestItem->m_aPrefix[iPrefixPos].m_wPrefix	=	_iPrefix;
	_lpDestItem->m_aPrefix[iPrefixPos].m_aValue[0]	=	_bValue1;
	_lpDestItem->m_aPrefix[iPrefixPos].m_aValue[1]	=	_bValue2;

	cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[_iPrefix];
	if(_bValue1+_bValue2 == 0)
	{
		for (i=0;i<2;i++)
		{
			int	iMinValue,iMaxValue,iRange;
			
			iMinValue	=	lpPrefix->m_aValue[i][0];
			iMaxValue	=	lpPrefix->m_aValue[i][1];
			iMaxValue	=	max(iMaxValue,iMinValue);
			iRange		=	iMaxValue-iMinValue;
			
			_lpDestItem->m_aPrefix[iPrefixPos].m_aValue[i]	=	min(random(iRange)+iMinValue,iMaxValue);
		}
	}
	BOOL		bIsIncorrectPrefix	=	FALSE;
	cBasicItem	*lpBasicItem		=	_lpDestItem->getBasicItem();

	if (lpBasicItem->m_wRequireStrength		>	_lpDestItem->getFixedStrength()		)	bIsIncorrectPrefix	=	TRUE;
	if (lpBasicItem->m_wRequireDexterity	>	_lpDestItem->getFixedAgility()		)	bIsIncorrectPrefix	=	TRUE;
	if (lpBasicItem->m_wRequireConstitution	>	_lpDestItem->getFixedConstitution()	)	bIsIncorrectPrefix	=	TRUE;
	if (lpBasicItem->m_wRequireWisdom		>	_lpDestItem->getFixedWisdom()		)	bIsIncorrectPrefix	=	TRUE;
	if (lpBasicItem->m_wRequireIntelligence	>	_lpDestItem->getFixedIntelligence()	)	bIsIncorrectPrefix	=	TRUE;
	if (lpBasicItem->m_wRequireCharisma		>	_lpDestItem->getFixedCharisma()		)	bIsIncorrectPrefix	=	TRUE;
	if (lpBasicItem->m_wRequireLuck			>	_lpDestItem->getFixedLuck()			)	bIsIncorrectPrefix	=	TRUE;

	if (bIsIncorrectPrefix)
	{
		_lpDestItem->m_aPrefix[iPrefixPos].m_wPrefix	=	0xffff;
		_lpDestItem->m_aPrefix[iPrefixPos].m_aValue[0]	=	0;
		_lpDestItem->m_aPrefix[iPrefixPos].m_aValue[1]	=	0;

		return	eAPR_FAILED;
	}

	if (_lpDestItem->isInfinityBullet()) _lpDestItem->m_bCount	=	1;

	return	eAPR_SUCCESS;
}

//
//	접두사 붙이기
void
CItemManager::makeGoodItem(cItem *_lpItem,int _iDropLevel)
{
	int	i,j;
	int	iItemType		=	_lpItem->getItemType();

	for (i=0;i<10;i++)
	{
		int	iPrefix	=	g_itemPrefix.getLittleGoodPrefix(_iDropLevel,random(3),iItemType);

		if (iPrefix	<	0	||	iPrefix	>=	g_itemPrefix.m_iCount)	continue;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[iPrefix];

		if (lpPrefix->m_wDiscernmentCode == 1503 || lpPrefix->m_wDiscernmentCode == 2414 || lpPrefix->m_wDiscernmentCode == 4001)
			continue;

		_lpItem->m_aPrefix[0].m_wPrefix		=	iPrefix;
		_lpItem->m_aPrefix[0].m_aValue[0]	=	0;
		_lpItem->m_aPrefix[0].m_aValue[1]	=	0;

		for (j=0;j<2;j++)
		{
			int	iMinValue,iMaxValue,iRange;

			iMinValue	=	lpPrefix->m_aValue[j][0];
			iMaxValue	=	lpPrefix->m_aValue[j][1];
			iMaxValue	=	max(iMaxValue,iMinValue);
			iRange		=	iMaxValue-iMinValue;

			_lpItem->m_aPrefix[0].m_aValue[j]	=	min(random(iRange)+iMinValue,iMaxValue);
		}

		break;
	}

	if	(_lpItem->isInfinityBullet())
		_lpItem->m_bCount	=	1;

	if (_lpItem->isDXItem())
	{
		int	iDurability			=	random(25)+50;

		_lpItem->m_bDurability	=	_lpItem->getBasicItem()->getDurability()*iDurability/100;
	}

	if	(_lpItem->isUltimateItem())
	{
		int	iDurability			=	random(c_iUltimateItemGenerateDurability)+c_iUltimateItemGenerateDurabilityRange;

		_lpItem->m_bDurability	=	_lpItem->getBasicItem()->getDurability()*iDurability/100;
	}
}

//
//	접두사 붙이기
BOOL
CItemManager::attachRandomPrefix(cItem *_lpItem,int _iWhere)
{
	int	j;

	int	iPrefix			=	random(g_itemPrefix.m_iCount);

	_lpItem->m_aPrefix[_iWhere].m_wPrefix		=	iPrefix;

	cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[iPrefix];

	for (j=0;j<2;j++)
	{
		int	iMinValue,iMaxValue,iRange;
		
		iMinValue	=	lpPrefix->m_aValue[j][0];
		iMaxValue	=	lpPrefix->m_aValue[j][1];
		iMaxValue	=	max(iMaxValue,iMinValue);
		iRange		=	iMaxValue-iMinValue;

		_lpItem->m_aPrefix[_iWhere].m_aValue[j]	=	min(random(iRange+1)+iMinValue,iMaxValue);
	}

	if	(_lpItem->isInfinityBullet())
		_lpItem->m_bCount	=	1;

	cBasicItem	*lpBasicItem	=	_lpItem->getBasicItem();
	BOOL	bIsIncorrectPrefix	=	FALSE;

	if (lpBasicItem->m_wRequireStrength		>	_lpItem->getFixedStrength()		)	bIsIncorrectPrefix	=	TRUE;
	if (lpBasicItem->m_wRequireDexterity	>	_lpItem->getFixedAgility()		)	bIsIncorrectPrefix	=	TRUE;
	if (lpBasicItem->m_wRequireConstitution	>	_lpItem->getFixedConstitution()	)	bIsIncorrectPrefix	=	TRUE;
	if (lpBasicItem->m_wRequireWisdom		>	_lpItem->getFixedWisdom()		)	bIsIncorrectPrefix	=	TRUE;
	if (lpBasicItem->m_wRequireIntelligence	>	_lpItem->getFixedIntelligence()	)	bIsIncorrectPrefix	=	TRUE;
	if (lpBasicItem->m_wRequireCharisma		>	_lpItem->getFixedCharisma()		)	bIsIncorrectPrefix	=	TRUE;
	if (lpBasicItem->m_wRequireLuck			>	_lpItem->getFixedLuck()			)	bIsIncorrectPrefix	=	TRUE;

	if (bIsIncorrectPrefix)
	{
		_lpItem->m_aPrefix[_iWhere].m_wPrefix	=	0xffff;
		_lpItem->m_aPrefix[_iWhere].m_aValue[0]	=	0;
		_lpItem->m_aPrefix[_iWhere].m_aValue[1]	=	0;

		attachRandomPrefix(_lpItem,_iWhere);
	}

	return	TRUE;
}

//
//	_strItemName이란 이름을 가진 아이템을 찾는다.
cBasicItem*
CItemManager::getItem(char *_strItemName)
{
	for (int i=0;i<m_iItemCount;i++)
	{
		if (g_aBasicItem[i].m_iSerial	==	0xffff)
			continue;

		if (STRICMP(g_aBasicItem[i].m_strName,_strItemName)	==	0)
			return	&g_aBasicItem[i];
	}

	return	NULL;
}

//
//	_strItemName이란 이름을 가진 아이템을 찾는다.
int
CItemManager::getItemIndex(char *_strItemName)
{
	for (int i=0;i<m_iItemCount;i++)
	{
		if	(g_aBasicItem[i].m_iSerial	==	0xffff)
			continue;

		if (STRICMP(g_aBasicItem[i].m_strName,_strItemName)	==	0)
			return	i;
	}

	return	0xffff;
}

//
//	랜덤하게 특수 아이템 생성
BOOL
CItemManager::generateRandomSpecialItem(cItem *_lpItem,int _iDropLevel,int _iChance,BOOL _isRare,BOOL _isUnique,int _iItemType)
{
	if (random(100) >= _iChance)
		return	FALSE;

	if (_iItemType	==	-1)
	{
		_iItemType	=	random(dGENERATE_RANDOM_SPECIAL_ITEM_LIST_COUNT);
		_iItemType	=	g_aGenerateRandomSpecialItemList[_iItemType];
	}

	if (!g_im.generateItemByType(_lpItem,_iItemType,_iDropLevel,1,_isUnique))
		return	FALSE;

	if (_isRare)
		g_im.attachPrefixByDropLevel(_lpItem,_iDropLevel);

	if (_lpItem->isInfinityBullet())
		_lpItem->m_bCount	=	1;

	return	TRUE;
}
//
//
void
CCustomItemManager::build()
{
	memset(m_awTeleportStoneIndex,0xff,sizeof(m_awTeleportStoneIndex));

	for (int i=0;i<m_iSize;i++)
	{
		CCustomItem			*lpCustomInfo=	&m_pItems[i];

		if (lpCustomInfo->m_wBaseItem	==	0xffff)
			continue;

		cBasicItem			*lpBasicItem=	&g_aBasicItem[lpCustomInfo->m_wBaseItem];

		cITEM_GENERATE_DATA	*lpGenData	=	&lpBasicItem->m_aGenerateData[0];

		if	(lpGenData->m_wEffect		!=	eIE_TELEPORT_STONE	&&
			lpGenData->m_wEffect		!=	eIE_PORTAL_STONE	)
			continue;

		int	iIndex						=	lpBasicItem->m_aValue[0][0];

		m_awTeleportStoneIndex[iIndex]	=	i;
	}
}


BOOL
CCustomItemManager::generateTeleportStone(cItem *_lpItem,int _iWayPoint)
{
	if (_iWayPoint	>=	dWAY_POINT_COUNT)
		return	FALSE;
	if (m_awTeleportStoneIndex[_iWayPoint]	==	0xffff)	
		return	FALSE;

	return	g_im.generatePremiumItem(_lpItem,m_awTeleportStoneIndex[_iWayPoint]);
}

