#include "cITEM.H"
#include "cMESSAGE.H"
#include "cITEM_PREFIX_EFFECT.H"
#include "cITEM_EFFECT.H"
#include "cJOB.H"

int	g_aStackableItem[]=
{	
	eIK_KNIFE						,//"단도",
	eIK_ARROW						,//"화살",
	eIK_BULLET						,//"탄환",
	eIK_HEAL_POTION					,//"체력 포션",
	eIK_CHARGING_POTION				,//"차징 포션",
	eIK_DRUG						,//"치료약",
	eIK_STATUS_DRUG					,//"상태 회복제",
	eIK_KEY							,//"열쇠",
	eIK_PORTAL_STONE				,//"포탈 스톤",
	eIK_FOOD_AND_ETC				,//"먹을 것/기타",
	eIK_SPECIAL						,
	eIK_QUEST_ITEM					,//"퀘스트 아이템",
	eIK_PREMIUM_ITEM				,//"프리미엄 아이템",
	eIK_ITEM_PACK					,
	0xffff
};


cBASIC_ITEM			g_aBasicItem[dBASIC_ITEM_COUNT];
cSPRITE				g_sprFieldShape;
cSMI				g_smiIconItem;
CCustomItemManager	g_premiumItem;
CCustomItemManager	g_customItem;

//◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
//	아이템 효과
//◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
char*
cUniqueData::getComment(BOOL _isSpecial)
{
	return	cPREFIX_EFFECT::GetComment((cITEM_EFFECT*)this,_isSpecial);
}	//	cITEM_EFFECT::getComment(BOOL _isSpecial)


//◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
//	아이템
//◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
//
//	기본 아이템에 설정되어 있는 내구력을 리턴한다.
int
cITEM::getMaxDurability()
{
	return	g_aBasicItem[m_wBaseItem].m_wDurability;
}	//	cITEM::getMaxDurability()


/////////////////////////////////////////////////////////////////////////////////
//	정적 멤버
/////////////////////////////////////////////////////////////////////////////////
//
//	아이템에 관련된것들 초기화
void
cITEM::Init()
{
	Reset();
	g_smiIconItem.load("data\\misc\\iconItem.smi");
	g_sprFieldShape.Load("data\\misc\\ItemFieldShape.sd");
}	//	cITEM::Init()

//
//	아이템에 관련된것들 리셋
void
cITEM::Reset()
{
	cBASIC_ITEM::Reset();
}

BOOL
cITEM::isNormalItem()//이건 보통 아이템이냐
{	
	if (isBreedingRecordBook())
		return	TRUE;

	if (m_aPrefix[0].m_wPrefix	!=	0xffff)
		return	FALSE;	//	레어.. 다 -o-

	cBASIC_ITEM	*lpItem		=	getBasicItem();
	CRY(!lpItem, "cry in cITEM::isNormalItem - Wow!! Why are you searching for an empty item!!");

	if (lpItem->m_aUniqueData[0].m_wEffect	!=	0xffff)
		return	FALSE;	//	유니크... 다 -o-

	return	FALSE;
}

BOOL	
cBASIC_ITEM::isSame(cBASIC_ITEM* _lpBasicItemData)
{

	if(m_iSerial != _lpBasicItemData->m_iSerial)
		return FALSE;

	if(strcmp(m_strName,_lpBasicItemData->m_strName) != 0)
		return FALSE;

	if(strcmp(m_strOwnerGuild,_lpBasicItemData->m_strOwnerGuild) != 0)
		return FALSE;

	if(m_dwCommentAddress != _lpBasicItemData->m_dwCommentAddress)
		return FALSE;

	if(m_wKind != _lpBasicItemData->m_wKind)
		return FALSE;

	if(memcmp(m_aEnableJob,_lpBasicItemData->m_aEnableJob,18) != 0)
		return FALSE;

	if(m_dwPrice != _lpBasicItemData->m_dwPrice)
		return FALSE;

	if(m_wPriceComputeMethod != m_wPriceComputeMethod)
		return FALSE;

	if(m_wRange != _lpBasicItemData->m_wRange)
		return FALSE;

	if(m_wDamageRange != _lpBasicItemData->m_wDamageRange)
		return FALSE;	
	
	if(m_wSpeed != _lpBasicItemData->m_wSpeed)
		return FALSE;

	if(m_wMinDamage != _lpBasicItemData->m_wMinDamage)
		return FALSE;

	if(m_wMaxDamage != _lpBasicItemData->m_wMaxDamage)
		return FALSE;

	if(m_wDurability != _lpBasicItemData->m_wDurability)
		return FALSE;

	if(m_wEquipUseLimitValue != _lpBasicItemData->m_wEquipUseLimitValue)
		return FALSE;

	if(m_wEquipUseLimitContents != _lpBasicItemData->m_wEquipUseLimitContents)
		return FALSE;

	if(m_wEquipUseLimitMethod != _lpBasicItemData->m_wEquipUseLimitMethod)
		return FALSE;

	if(m_wRequireLevel != _lpBasicItemData->m_wRequireLevel)
		return FALSE;
	
	if(m_wRequireStrength != _lpBasicItemData->m_wRequireStrength)
		return FALSE;

	if(m_wRequireDexterity != _lpBasicItemData->m_wRequireDexterity)
		return FALSE;

	if(m_wRequireConstitution != _lpBasicItemData->m_wRequireConstitution)
		return FALSE;

	if(m_wRequireWisdom != _lpBasicItemData->m_wRequireWisdom)
		return FALSE;

	if(m_wRequireIntelligence != _lpBasicItemData->m_wRequireIntelligence)
		return FALSE;

	if(m_wRequireCharisma != _lpBasicItemData->m_wRequireCharisma)
		return FALSE;

	if(m_wRequireLuck != _lpBasicItemData->m_wRequireLuck)
		return FALSE;

	if(m_wRequireAllignment != _lpBasicItemData->m_wRequireAllignment)
		return FALSE;

	if(m_wIconShape != _lpBasicItemData->m_wIconShape)
		return FALSE;

	if(m_wFieldShape != _lpBasicItemData->m_wFieldShape)
		return FALSE;

	if(m_wEquippedShape != _lpBasicItemData->m_wEquippedShape)
		return FALSE;

	if(m_questKind != _lpBasicItemData->m_questKind)
		return FALSE;

	if(m_questIndex != _lpBasicItemData->m_questIndex)
		return FALSE;

	if(m_wStackLimit != _lpBasicItemData->m_wStackLimit)
		return FALSE;

	if(m_wDropLevel != _lpBasicItemData->m_wDropLevel)
		return FALSE;

	if(memcmp(m_aValue,_lpBasicItemData->m_aValue,sizeof(WORD)*4) != 0)
		return FALSE;

	if(memcmp(m_aGenerateData,_lpBasicItemData->m_aGenerateData,sizeof(cITEM_GENERATE_DATA)*dGENERATE_ITEM_DATA_COUNT) != 0)
		return FALSE;

	if(memcmp(m_aUniqueData,_lpBasicItemData->m_aUniqueData, sizeof(cUniqueData)*dITEM_UNIQUE_DATA_COUNT) != 0)
		return FALSE;

	if(m_wPriceValue != _lpBasicItemData->m_wPriceValue)
		return FALSE;

	if(memcmp(&m_attr,&_lpBasicItemData->m_attr, sizeof(DWORD)) != 0)
		return FALSE;

	if(m_wExpireYear != _lpBasicItemData->m_wExpireYear)
		return FALSE;

	if(m_wExpireMonth != _lpBasicItemData->m_wExpireMonth)
		return FALSE;

	if(m_wExpireDay != _lpBasicItemData->m_wExpireDay)
		return FALSE;

	if(m_wExpireHour != _lpBasicItemData->m_wExpireHour)
		return FALSE;

	if(m_wEnchantMinChance != _lpBasicItemData->m_wEnchantMinChance)
		return FALSE;

	if(m_wEnchantMaxChance != _lpBasicItemData->m_wEnchantMaxChance)
		return FALSE;

	if(m_wEnchantLimitPrefixDiscernmentCode != _lpBasicItemData->m_wEnchantLimitPrefixDiscernmentCode)
		return FALSE;

	if(m_wPaletteIndex != _lpBasicItemData->m_wPaletteIndex)
		return FALSE;

	if(m_wBoostDurability != _lpBasicItemData->m_wBoostDurability)
		return FALSE;

	if(m_wCorrectDropChance != _lpBasicItemData->m_wCorrectDropChance)
		return FALSE;

	if(m_wBaseItem != _lpBasicItemData->m_wBaseItem)
		return FALSE;

	if(m_wExtraGrade != _lpBasicItemData->m_wExtraGrade)
		return FALSE;

	if(m_wPremiumLevel != _lpBasicItemData->m_wPremiumLevel)
		return FALSE;

	if(m_wIsIDPublicItem != _lpBasicItemData->m_wIsIDPublicItem)
		return FALSE;

	if(m_bf1IsDestroyWhenMoveField != _lpBasicItemData->m_bf1IsDestroyWhenMoveField)
		return FALSE;

	if(m_bf1IsBuyOnlyGuildMaster != _lpBasicItemData->m_bf1IsBuyOnlyGuildMaster)
		return FALSE;

	if(m_bf1IsUseOnlyGuildMaster != _lpBasicItemData->m_bf1IsUseOnlyGuildMaster)
		return FALSE;

	if(m_bf1IsUseAbleByThrowPotionSkill != _lpBasicItemData->m_bf1IsUseAbleByThrowPotionSkill)
		return FALSE;

	if(m_bf1IsUseAbleByThrowFlowerSkill != _lpBasicItemData->m_bf1IsUseAbleByThrowFlowerSkill)
		return FALSE;

	if(m_bf1IsUseAbleByThrowDrugSkill != _lpBasicItemData->m_bf1IsUseAbleByThrowDrugSkill)
		return FALSE;

	if(m_bf1IsUseAbleByThrowCandySkill != _lpBasicItemData->m_bf1IsUseAbleByThrowCandySkill)
		return FALSE;

	if(m_bf1IsUltimate != _lpBasicItemData->m_bf1IsUltimate)
		return FALSE;

	if(m_bf1IsBlockToEnchant != _lpBasicItemData->m_bf1IsBlockToEnchant)
		return FALSE;

	if(m_bf1IsBlockToFeedPet != _lpBasicItemData->m_bf1IsBlockToFeedPet)
		return FALSE;

	if(m_bf1IsFreeTeleport != _lpBasicItemData->m_bf1IsFreeTeleport)
		return FALSE;

	if(m_bf1IsRequestSummonCarpet != _lpBasicItemData->m_bf1IsRequestSummonCarpet)
		return FALSE;

	if(m_bf1IsCanSummonMagicCarpet != _lpBasicItemData->m_bf1IsCanSummonMagicCarpet)
		return FALSE;

	if(m_bf1RemeberPlace0 != _lpBasicItemData->m_bf1RemeberPlace0)
		return FALSE;

	if(m_bf1RemeberPlace1 != _lpBasicItemData->m_bf1RemeberPlace1)
		return FALSE;

	if(m_bf1RemeberPlace2 != _lpBasicItemData->m_bf1RemeberPlace2)
		return FALSE;

	if(m_bf1AddEntryGuildDungeon != _lpBasicItemData->m_bf1AddEntryGuildDungeon)
		return FALSE;

	if(m_bf1IsAddGetMysticStone != _lpBasicItemData->m_bf1IsAddGetMysticStone)
		return FALSE;

	if(m_bf1IsAddGetTantalissRelic != _lpBasicItemData->m_bf1IsAddGetTantalissRelic)
		return FALSE;

	if(m_bf1IsIgnoreGuildHallLevelForEntryTantalisExile != _lpBasicItemData->m_bf1IsIgnoreGuildHallLevelForEntryTantalisExile)
		return FALSE;

	if(m_bf1IsInfinitySeriousUpgradeChanceUp != _lpBasicItemData->m_bf1IsInfinitySeriousUpgradeChanceUp)
		return FALSE;

	if(m_bf1IsRequireLogWithGuildInfo != _lpBasicItemData->m_bf1IsRequireLogWithGuildInfo)
		return FALSE;

	if(m_bf1IsFreePastPortal != _lpBasicItemData->m_bf1IsFreePastPortal)
		return FALSE;

	if(m_bf1IsHalfPastPortal != _lpBasicItemData->m_bf1IsHalfPastPortal)
		return FALSE;

	if(m_bf1IsExcludeExpBonus != _lpBasicItemData->m_bf1IsExcludeExpBonus)
		return FALSE;

	if(m_bf3WearLimit != _lpBasicItemData->m_bf3WearLimit)
		return FALSE;

	if(m_bf1IsEternalItem != _lpBasicItemData->m_bf1IsEternalItem)
		return FALSE;

	if(m_bf1EternalItemUpgradeNotPenalty != _lpBasicItemData->m_bf1EternalItemUpgradeNotPenalty)
		return FALSE;

	if(m_bf1IsUpgradeMaterials != _lpBasicItemData->m_bf1IsUpgradeMaterials)
		return FALSE;

	if(m_bf1IsItemNotUsePet != _lpBasicItemData->m_bf1IsItemNotUsePet)
		return FALSE;

	if(m_bIsExceptionItem != _lpBasicItemData->m_bIsExceptionItem)
		return FALSE;

	if(m_wPremiumItemLevelForWithdraw != _lpBasicItemData->m_wPremiumItemLevelForWithdraw)
		return FALSE;

	if(m_bCompulsionColorizeEffect != _lpBasicItemData->m_bCompulsionColorizeEffect)
		return FALSE;

	if(m_bCoolTimeIndex != _lpBasicItemData->m_bCoolTimeIndex)
		return FALSE;

	if(m_wCoolTimeSecond != _lpBasicItemData->m_wCoolTimeSecond)
		return FALSE;

	if(m_wNextEternalItemSerial != _lpBasicItemData->m_wNextEternalItemSerial)
		return FALSE;

	if(m_bf4EternalItemCurrentLevel != _lpBasicItemData->m_bf4EternalItemCurrentLevel)
		return FALSE;	

	if(m_bf1IsMaleJobEquipAble != _lpBasicItemData->m_bf1IsMaleJobEquipAble)
		return FALSE;		

	if(m_bf1IsFemaleJobEquipAble != _lpBasicItemData->m_bf1IsFemaleJobEquipAble)
		return FALSE;

	if(m_bf7UpgradeBaseSuccessPercent != _lpBasicItemData->m_bf7UpgradeBaseSuccessPercent)
		return FALSE;

	if(m_bf1IsOccurEatPotionEffect != _lpBasicItemData->m_bf1IsOccurEatPotionEffect)
		return FALSE;

	if(m_bf1IsUseShop != _lpBasicItemData->m_bf1IsUseShop)
		return FALSE;

	if(m_bf1IsUseBank != _lpBasicItemData->m_bf1IsUseBank)
		return FALSE;

	if(m_uiEquipAbleJobMask != _lpBasicItemData->m_uiEquipAbleJobMask)
		return FALSE;

	if(m_wUpgradePenaltyItemSerial != _lpBasicItemData->m_wUpgradePenaltyItemSerial)
		return FALSE;

	if(m_bf7UpgradeMaterialsSuccessPercent != _lpBasicItemData->m_bf7UpgradeMaterialsSuccessPercent)
		return FALSE;

	if(m_bf7UpgradeCosmicPowerSuccessPercent != _lpBasicItemData->m_bf7UpgradeCosmicPowerSuccessPercent)
		return FALSE;

	if(m_bf1IsNotConsume != _lpBasicItemData->m_bf1IsNotConsume)
		return FALSE;

	if(m_bf1IsUseItemAfterTransformation != _lpBasicItemData->m_bf1IsUseItemAfterTransformation)
		return FALSE;

	if(m_bf4ItemRebirthCount != _lpBasicItemData->m_bf4ItemRebirthCount)				
		return FALSE;

	if(m_bf5LimitUpgradeRingOfInfinity != _lpBasicItemData->m_bf5LimitUpgradeRingOfInfinity)		
		return FALSE;

	if(m_bf1IsRingOfInfinity != _lpBasicItemData->m_bf1IsRingOfInfinity)				
		return FALSE;

	if(m_bf1IsUseToEnemy != _lpBasicItemData->m_bf1IsUseToEnemy)					
		return FALSE;

	if(m_bf1IsUseAfterWear != _lpBasicItemData->m_bf1IsUseAfterWear)					
		return FALSE;

	if(m_bf1IsUsePremiumInventory != _lpBasicItemData->m_bf1IsUsePremiumInventory)			
		return FALSE;

	if(m_bf1IsNotApplyCopyEffect != _lpBasicItemData->m_bf1IsNotApplyCopyEffect)			
		return FALSE;

	if(m_bf1IsNotApplyReversionEffect != _lpBasicItemData->m_bf1IsNotApplyReversionEffect)		
		return FALSE;

	if(m_bf1IsNotGetUsedSkill != _lpBasicItemData->m_bf1IsNotGetUsedSkill)				
		return FALSE;

	if(m_wMoveFieldSerial != _lpBasicItemData->m_wMoveFieldSerial)				
		return FALSE;

	if(m_wRequireMaxLevel != _lpBasicItemData->m_wRequireMaxLevel)
		return FALSE;

	if(m_wItemEffect != _lpBasicItemData->m_wItemEffect)
		return FALSE;

	if(m_bf1IsNotUseToGVG != _lpBasicItemData->m_bf1IsNotUseToGVG)
		return FALSE;


	return TRUE;
}

BOOL
cITEM::isLittleGoodItem()
{
	if (isBreedingRecordBook())
		return	FALSE;

	if (m_aPrefix[0].m_wPrefix	==	0xffff)	
		return	FALSE;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (m_aPrefix[i].m_wPrefix	==	0xffff)	
			break;

	}
	return	FALSE;
}
	
BOOL
cITEM::isItemPack()
{
	cBASIC_ITEM	*lpItem	=	getBasicItem();

	if (!lpItem)	
		return	FALSE;

	if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_LOTTERY_ITEM)
		return	TRUE;
	if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_SELECT_ITEM_IN_ITEM_PACK)
		return	TRUE;

	return	FALSE;
}

BOOL
cITEM::isRareItem()	
{
	if (isBreedingRecordBook())
		return	FALSE;

	if (m_aPrefix[0].m_wPrefix	==	0xffff)	
		return	FALSE;

	if (isLittleGoodItem())
		return	FALSE;

	return	TRUE;
}
BOOL
cITEM::isUniqueItem()//이건 유니크냐?
{
	if (isBreedingRecordBook())
		return	FALSE;
	if (isExtraItem())
		return	FALSE;
	
	cBASIC_ITEM	*lpItem		=	getBasicItem();
	CRY(!lpItem,"cry in cITEM::isNormalItem - Wow!! Why are you searching for empty items?");
	if (c_aItemEquipPlace[lpItem->m_wKind] == dITEM_KIND_WASTING || c_aItemEquipPlace[lpItem->m_wKind] == 0xffff)		//  변경..
		return	FALSE;
	if (lpItem->m_aUniqueData[0].m_wEffect	!=	0xffff)	return	TRUE;	//	유니크... 다 -o-

	return	FALSE;
}
BOOL
cITEM::isSuperUniqueItem()//이건 유니크냐?
{
	if (isBreedingRecordBook())
		return	FALSE;
	
	cBASIC_ITEM	*lpItem		=	getBasicItem();
	CRY(!lpItem,"cry in cITEM::isNormalItem - Wow!! Why are you searching for empty items?");

	if (c_aItemEquipPlace[lpItem->m_wKind] == dITEM_KIND_WASTING || c_aItemEquipPlace[lpItem->m_wKind] == 0xffff)		//  변경..
		return	FALSE;

	if (lpItem->m_aUniqueData[0].m_wEffect	==	0xffff)
		return	FALSE;	//	유니크... 다 -o-
	if (m_aPrefix[0].m_wPrefix	==	0xffff)	
		return	FALSE;

	return	TRUE;
}

cBASIC_ITEM*
cITEM::getBasicItem()
{
	if(m_wBaseItem >= dBASIC_ITEM_COUNT)	return	NULL;

	return &g_aBasicItem[m_wBaseItem];
}

//
//	명사의 수
int
cITEM::getNounPrefixCount()
{
	int	iPrefixCount	=	0;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (m_aPrefix[i].m_wPrefix	==	0xffff)		break;

		cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(m_aPrefix[i].m_wPrefix);

		if (lpPrefix->m_wPrefixForm	==	0)	iPrefixCount++;
	}

	return	iPrefixCount;
}	//	cITEM::getNounPrefixCount()

//
//	형용사의 수
int
cITEM::getAdjectivePrefixCount()
{
	int	iPrefixCount	=	0;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (m_aPrefix[i].m_wPrefix	==	0xffff)		break;

		cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(m_aPrefix[i].m_wPrefix);

		if (lpPrefix->m_wPrefixForm	==	1)	iPrefixCount++;
	}

	return	iPrefixCount;
}	//	cITEM::getAdjectivePrefixCount()


//
//	아이템 이름
char*
cITEM::getName(BOOL _bIsWantColor,BOOL _bIsWantIcon)
{
	static	cSTRING		s_strItemName;

	cBASIC_ITEM	*lpBasicItem	=	getBasicItem();

	if (!lpBasicItem)	return	NULL;

	if (lpBasicItem->m_wKind == eIK_PREMIUM_ITEM || (!isRareItem() && !isUniqueItem() && !isExtraItem()))
		return	lpBasicItem->m_strName;

	s_strItemName.Reset();

	if (_bIsWantIcon)
	{
				if (isSuperUniqueItem())	s_strItemName.Add("(S)");
		else	if (isUniqueItem())		s_strItemName.Add("(U)");
		else	if (isExtraItem())		s_strItemName.Add("(E)");
		else	if (isRareItem())		s_strItemName.Add("(R)");
	}

	if (_bIsWantColor)	s_strItemName.Add("%c",1);

//	형용사 2차 + 형용사 2차 + 형용사 1차 + 아이템
//	형용사 2차 + 형용사 1차 + 명사 1차 + 아이템
//	형용사 1차 + 명사 2차 + 명사 1차 + 아이템
//	명사 2차 + 명사 2차 + 명사 1차 + 아이템
	int	iPrefixCount	=	getPrefixCount();
	int	iNounCount		=	getNounPrefixCount();

//	접두사가 하나 일때
	if (iPrefixCount	==	1)
	{
		cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(m_aPrefix[0].m_wPrefix);

		s_strItemName.Add(lpPrefix->m_str1stPrefix);
	}

//	접두사가 둘 일때
	if (iPrefixCount	==	2)
	{
		cITEM_PREFIX	*lpPrefix1	=	g_itemPrefix.get(m_aPrefix[0].m_wPrefix);
		cITEM_PREFIX	*lpPrefix2	=	g_itemPrefix.get(m_aPrefix[1].m_wPrefix);

		if (iNounCount	==	0)	//	둘다 형용사
		{
			s_strItemName.Add("%s ",lpPrefix1->m_str2stPrefix);
			s_strItemName.Add(lpPrefix2->m_str1stPrefix);
		}

		if (iNounCount	==	1)
		{
			if (lpPrefix1->m_wPrefixForm	==	0)	//	명사
			{
				s_strItemName.Add("%s ",lpPrefix2->m_str1stPrefix);
				s_strItemName.Add(lpPrefix1->m_str1stPrefix);
			}
			else	//	형용사
			{
				s_strItemName.Add("%s ",lpPrefix1->m_str1stPrefix);
				s_strItemName.Add(lpPrefix2->m_str1stPrefix);
			}
		}

		if (iNounCount	==	2)	//	둘다 명사
		{
			s_strItemName.Add("%s ",lpPrefix1->m_str2stPrefix);
			s_strItemName.Add(lpPrefix2->m_str1stPrefix);
		}
	}

//	접두사가 셋일때
	if (iPrefixCount	==	3)
	{
		cITEM_PREFIX	*lpPrefix1	=	g_itemPrefix.get(m_aPrefix[0].m_wPrefix);
		cITEM_PREFIX	*lpPrefix2	=	g_itemPrefix.get(m_aPrefix[1].m_wPrefix);
		cITEM_PREFIX	*lpPrefix3	=	g_itemPrefix.get(m_aPrefix[2].m_wPrefix);

		if (iNounCount	==	0)	//	셋 다 형용사
		{
			s_strItemName.Add("%s ",lpPrefix1->m_str2stPrefix);
			s_strItemName.Add("%s ",lpPrefix2->m_str2stPrefix);
			s_strItemName.Add(lpPrefix3->m_str1stPrefix);
		}

		if (iNounCount	==	1)	//	명사가 하나
		{
			if (lpPrefix1->m_wPrefixForm	==	0)	//	명사
			{
				s_strItemName.Add("%s ",lpPrefix3->m_str2stPrefix);
				s_strItemName.Add("%s ",lpPrefix2->m_str1stPrefix);
				s_strItemName.Add(lpPrefix1->m_str1stPrefix);
			}

			if (lpPrefix2->m_wPrefixForm	==	0)	//	명사
			{
				s_strItemName.Add("%s ",lpPrefix3->m_str2stPrefix);
				s_strItemName.Add("%s ",lpPrefix1->m_str1stPrefix);
				s_strItemName.Add(lpPrefix2->m_str1stPrefix);
			}

			if (lpPrefix3->m_wPrefixForm	==	0)	//	명사
			{
				s_strItemName.Add("%s ",lpPrefix1->m_str2stPrefix);
				s_strItemName.Add("%s ",lpPrefix2->m_str1stPrefix);
				s_strItemName.Add(lpPrefix3->m_str1stPrefix);
			}
		}

		if (iNounCount	==	2)	//	형용사가 하나
		{
			if (lpPrefix1->m_wPrefixForm	==	1)	//	형용사
			{
				s_strItemName.Add("%s ",lpPrefix1->m_str1stPrefix);
				s_strItemName.Add("%s ",lpPrefix2->m_str2stPrefix);
				s_strItemName.Add(lpPrefix3->m_str1stPrefix);
			}

			if (lpPrefix2->m_wPrefixForm	==	1)	//	형용사
			{
				s_strItemName.Add("%s ",lpPrefix2->m_str1stPrefix);
				s_strItemName.Add("%s ",lpPrefix1->m_str2stPrefix);
				s_strItemName.Add(lpPrefix3->m_str1stPrefix);
			}

			if (lpPrefix3->m_wPrefixForm	==	1)	//	형용사
			{
				s_strItemName.Add("%s ",lpPrefix3->m_str1stPrefix);
				s_strItemName.Add("%s ",lpPrefix1->m_str2stPrefix);
				s_strItemName.Add(lpPrefix2->m_str1stPrefix);
			}
		}

		if (iNounCount	==	3)	//	셋 다 명사
		{
			s_strItemName.Add("%s ",lpPrefix1->m_str2stPrefix);
			s_strItemName.Add("%s ",lpPrefix2->m_str2stPrefix);
			s_strItemName.Add(lpPrefix3->m_str1stPrefix);
		}
	}

	if (_bIsWantColor)
		s_strItemName.Add("%c ",2);
	else
		s_strItemName.Add(" ");

	s_strItemName.Add("%s",lpBasicItem->m_strName);

	return	s_strItemName.String;
}	//	cITEM::getName()

//◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
//	기본 아이템
//◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆

//
//	아이템 초기화
void
cBASIC_ITEM::reset()
{
	pKILL(m_pItemComment);

	memset(this,0,sizeof(cBASIC_ITEM)-4);

	strcpy(m_strName,"valid name");			//	이름

	m_iSerial				=	dVALID_ITEM;
	m_questKind				=	0;
	m_questIndex			=	dNOT_QUEST_ITEM;
	m_wDurability			=	10;
	m_wStackLimit			=	1;
	m_wEquipUseLimitMethod	=	0xffff;
	m_wEquipUseLimitContents=	0xffff;
	m_wIconShape			=	0xffff;
	m_wFieldShape			=	0xffff;
	m_wEquippedShape		=	0xffff;
	m_wPriceValue			=	1;
	m_wCorrectDropChance	=	1000;
	m_wBaseItem				=	0xffff;
	m_wExtraGrade			=	0xffff;
	m_wDropLevel			=	10000;
	m_bCoolTimeIndex		=	0;

	memset(m_aUniqueData,0xff,sizeof(m_aUniqueData));
	memset(m_aGenerateData,0xff,sizeof(m_aGenerateData));
	memset(m_aEnableJob,0,sizeof(m_aEnableJob));
	
	for (int iJob=0;iJob<dPLAYER_JOB_COUNT;iJob++)
	{
		m_uiEquipAbleJobMask	+=	(1<<iJob);
	}
}

//
//data 코멘트
char*
cBASIC_ITEM::getDataComment(int _iIndex,BOOL _isSpecial,int _iValue1,int _iValue2)
{
	return	cITEM_GENERATE_EFFECT::GetComment(&m_aGenerateData[_iIndex],this,_isSpecial,_iValue1,_iValue2);
}

//
//	아이템 복사
void
cBASIC_ITEM::copy(cBASIC_ITEM	*lpItem)
{
	reset();

	memcpy(this,lpItem,sizeof(cBASIC_ITEM)-4);

	setComment(lpItem->m_pItemComment);
}

//
//	아이템 복사
void
cBASIC_ITEM::copy(int _iIndex)
{
	if (!IsExist(_iIndex))	return;

	copy(&g_aBasicItem[_iIndex]);
}


//
//	아이템에 대한 설명 설정
void
cBASIC_ITEM::setComment(char *_strComment)
{
	pKILL(m_pItemComment);

	if (!_strComment)
	{
		m_dwCommentSize	=	strlen(dMSG_VALID_COMMENT)+1;

		m_pItemComment	=	new char [m_dwCommentSize];

		strcpy(m_pItemComment,dMSG_VALID_COMMENT);

		return;
	}

	m_dwCommentSize	=	strlen(_strComment)+1;
	m_pItemComment	=	new char [m_dwCommentSize];

	strcpy(m_pItemComment,_strComment);
}


///////////////////////////////////////////////////////////////////////////////////////////////
//	정적 멤버
///////////////////////////////////////////////////////////////////////////////////////////////
int		cBASIC_ITEM::s_aCountOfCategory[dCOUNT_OF_ITEM_KIND];
int		cBASIC_ITEM::s_iCount	=	0;

//
//	기본 아이템 초기화
void
cBASIC_ITEM::Reset(cBASIC_ITEM *_lpBI)
{
	if	(_lpBI	==	NULL)
		_lpBI	=	g_aBasicItem;

	int	i;

	if	(_lpBI	==	g_aBasicItem)
		s_iCount	=	0;

	for (i=0;i<dBASIC_ITEM_COUNT;i++)
		_lpBI[i].reset();

	if	(_lpBI	==	g_aBasicItem)
		for (i=0;i<dCOUNT_OF_ITEM_KIND;i++)
			s_aCountOfCategory[i]	=	0;
}

//
//	아이템을 정렬한다.
void
cBASIC_ITEM::Arrange(cBASIC_ITEM *_lpBI,int _iVersion)
{
	if	(_lpBI	==	NULL)
		_lpBI	=	g_aBasicItem;

	int		i,iIndex	=	0;

	if	(_lpBI	==	g_aBasicItem)
	{
		s_iCount	=	0;

		memset(s_aCountOfCategory,0,sizeof(s_aCountOfCategory));
	}

	for (i=0;i<dBASIC_ITEM_COUNT;i++)
	{
		if	(_lpBI[i].m_iSerial	>=	dBASIC_ITEM_COUNT	||	_lpBI[i].m_iSerial	<	0)
			_lpBI[i].m_iSerial	=	dVALID_ITEM;

		if	(_lpBI[i].m_iSerial	==	dVALID_ITEM)
			continue;

		if	(_lpBI	==	g_aBasicItem)
		{
			s_aCountOfCategory[g_aBasicItem[i].m_wKind]++;
			s_iCount	++;
		}
	}

	for (i=0;i<dBASIC_ITEM_COUNT;i++)
	{
		if	(_lpBI[i].m_iSerial			==	dVALID_ITEM)
			continue;
/*
		if	(_lpBI[i].m_attr.isDXItem	==	FALSE	||	_lpBI[i].isUnique())
			continue;

		char	strName[64];

		strcpy(strName,_lpBI[i].m_strName);

		if	(strstr(strName,"GDX"))
			strName[strlen(strName)-4]	=	NULL;
		else
		if	(strstr(strName,"LDX"))
			strName[strlen(strName)-4]	=	NULL;
		else
		if	(strstr(strName,"XLS"))
			strName[strlen(strName)-4]	=	NULL;
		else
		if	(strstr(strName,"LX"))
			strName[strlen(strName)-3]	=	NULL;
		else
		if	(strstr(strName,"DX"))
			strName[strlen(strName)-3]	=	NULL;
		else
			cMSG::Put("cBASIC_ITEM::Arrange","%s\n%d",strName,i);

		cBASIC_ITEM*	lpItem	=	Get(strName,_lpBI);

		if	(lpItem)
			lpItem->m_wDurability	=	_lpBI[i].m_wDurability;
*/
	}
}

//
//	마지막에 있는 아이템의 인덱스를 얻어 온다.
int
cBASIC_ITEM::GetLastIndex(cBASIC_ITEM *_lpBI)
{
	if	(_lpBI	==	NULL)
		_lpBI	=	g_aBasicItem;

	for (int i=dBASIC_ITEM_COUNT;i;)
	{
		--i;

		if	(_lpBI[i].m_iSerial	!=	dVALID_ITEM)
			return	i;
	}

	return	-1;
}

//
//	기본 아이템을 추가한다. 나중에.. 이런거 저런거 므흣*-_-* 한거 까지 추가하자.
int
cBASIC_ITEM::Add(cBASIC_ITEM *_lpItem)
{
	if	(s_iCount	>=	dBASIC_ITEM_COUNT)
		return	dVALID_ITEM;

	if	(Get(_lpItem->m_strName))
		return	dVALID_ITEM;

	int	iNewIndex	=	dVALID_ITEM,i;

	for (i=0;i<dBASIC_ITEM_COUNT;i++)
	{
		if (g_aBasicItem[i].m_iSerial	==	dVALID_ITEM)
		{
			iNewIndex	=	i;

			break;
		}
	}

	if	(iNewIndex	==	dVALID_ITEM)
	{
		s_iCount	=	dBASIC_ITEM_COUNT;

		return	dVALID_ITEM;
	}

	s_aCountOfCategory[_lpItem->m_wKind]++;
	s_iCount++;

	memcpy(&g_aBasicItem[iNewIndex],_lpItem,sizeof(cBASIC_ITEM)-4);

	g_aBasicItem[iNewIndex].setComment(_lpItem->m_pItemComment);

	g_aBasicItem[iNewIndex].m_iSerial	=	iNewIndex;

	return	iNewIndex;
}

//
//	기본 아이템을 제거한다.
BOOL
cBASIC_ITEM::Remove(int _iIndex)
{

	if (s_iCount<=	0									)	return	FALSE;
	if (_iIndex	>=	dBASIC_ITEM_COUNT					)	return	FALSE;
	if (_iIndex	<	0									)	return	FALSE;
	if (g_aBasicItem[_iIndex].m_iSerial	==	dVALID_ITEM	)	return	FALSE;

	g_aBasicItem[_iIndex].m_iSerial	=	dVALID_ITEM;
	s_aCountOfCategory[g_aBasicItem[_iIndex].m_wKind]--;
	s_iCount--;

	return	TRUE;
}	//	cBASIC_ITEM::RemoveItem(int _iIndex)

//
//	기본 아이템을 제거한다.
BOOL
cBASIC_ITEM::Remove(char *_strName)
{
	cBASIC_ITEM	*lpItem	=	Get(_strName);

	if (!lpItem)	return	FALSE;

	return	Remove(lpItem->m_iSerial);
}	//	cBASIC_ITEM::Remove(char *_strName)

//
//	이름을 참조해서 아이템을 얻어 온다.
cBASIC_ITEM*
cBASIC_ITEM::Get(char *_strName,cBASIC_ITEM *_lpBI)
{
	if	(_lpBI	==	NULL)
		_lpBI	=	g_aBasicItem;

	for (int i=0;i<dBASIC_ITEM_COUNT;i++)
	{
		if (_lpBI[i].m_iSerial	!=	dVALID_ITEM	)
		{
			if (stricmp(_strName,_lpBI[i].m_strName)	==	0)
				return	&_lpBI[i];
		}
	}

	return	NULL;
}

//
//	종류별로 아이템을 얻어 오기 위해서..
cBASIC_ITEM*
cBASIC_ITEM::GetItemByKind(int _iKind,int *_iBaseIndex)
{
	for (;*_iBaseIndex<dBASIC_ITEM_COUNT;(*_iBaseIndex)++)
	{
		int index	=	*_iBaseIndex;
		
		if (g_aBasicItem[index].m_iSerial	!=	dVALID_ITEM	)
		{
			if (_iKind	==	dITEM_KIND_ALL)
			{
				(*_iBaseIndex)++;					//	다시 얻으려면.. -_-a
				return	&g_aBasicItem[index];
			}
			
			if (_iKind	==	dITEM_KIND_WASTING)
			{
				
				if (c_aItemEquipPlace[g_aBasicItem[index].m_wKind] == dITEM_KIND_WASTING || c_aItemEquipPlace[g_aBasicItem[index].m_wKind] == 0xffff)		//  변경..
				{
					(*_iBaseIndex)++;					//	다시 얻으려면.. -_-a
					return	&g_aBasicItem[index];
				}
			}
			else
			{
				if (g_aBasicItem[index].m_wKind	==	_iKind)
				{
					(*_iBaseIndex)++;					//	다시 얻으려면.. -_-a
					return	&g_aBasicItem[index];
				}
			}
		}
	}
	
	return	NULL;
}	//	cBASIC_ITEM::GetItemByKind(int kind,int *_iBaseIndex)

//
//	종류별로 아이템을 얻어 오기 위해서..
cBASIC_ITEM*
cBASIC_ITEM::GetItemByKind(int _iKind,int _iJob,int *_iBaseIndex)
{
	for (;*_iBaseIndex<dBASIC_ITEM_COUNT;(*_iBaseIndex)++)
	{
		int index	=	*_iBaseIndex;

		if (g_aBasicItem[index].m_iSerial	!=	dVALID_ITEM	)
		{
			if (_iJob	>=	dBODY_NPC_START)
			{
				if (_iKind	==	dITEM_KIND_ALL)
				{
					(*_iBaseIndex)++;					//	다시 얻으려면.. -_-a
					return	&g_aBasicItem[index];
				}

				if (_iKind	==	dITEM_KIND_WASTING)
				{
					if (c_aItemEquipPlace[g_aBasicItem[index].m_wKind] == dITEM_KIND_WASTING || c_aItemEquipPlace[g_aBasicItem[index].m_wKind] == 0xffff)		//  변경..
					{
						(*_iBaseIndex)++;					//	다시 얻으려면.. -_-a
						return	&g_aBasicItem[index];
					}
				}
				else
				if (g_aBasicItem[index].m_wKind	==	_iKind)
				{
					(*_iBaseIndex)++;					//	다시 얻으려면.. -_-a
					return	&g_aBasicItem[index];
				}
			}

			else

			if	(g_aBasicItem[index].isEquipAbleJob(_iJob))
			{
				if	(_iKind	==	dITEM_KIND_ALL)
				{
					(*_iBaseIndex)++;					//	다시 얻으려면.. -_-a
					return	&g_aBasicItem[index];
				}

				if (_iKind	==	dITEM_KIND_WASTING)
				{
					if (c_aItemEquipPlace[g_aBasicItem[index].m_wKind] == dITEM_KIND_WASTING || c_aItemEquipPlace[g_aBasicItem[index].m_wKind] == 0xffff)		//  변경..
					{
						(*_iBaseIndex)++;					//	다시 얻으려면.. -_-a
						return	&g_aBasicItem[index];
					}
				}
				else
				if (g_aBasicItem[index].m_wKind	==	_iKind)
				{
					(*_iBaseIndex)++;					//	다시 얻으려면.. -_-a
					return	&g_aBasicItem[index];
				}
			}
		}
	}

	return	NULL;
}	//	cBASIC_ITEM::GetItemByKind(int kind,int *_iBaseIndex)

//
//	특정 분류에 특정 아이템이 존재하는지 확인.
BOOL
cBASIC_ITEM::IsExist(int _iIndex,int _iKind)
{
	if (_iIndex	>=	dBASIC_ITEM_COUNT	||	_iIndex	<	0)	return	FALSE;

	if (g_aBasicItem[_iIndex].m_iSerial	==	dVALID_ITEM	)	return	FALSE;

	if (_iKind							==	dITEM_KIND_ALL)	return	TRUE;
	if (g_aBasicItem[_iIndex].m_wKind	==	_iKind		)	return	TRUE;

	return	FALSE;
}	//	cBASIC_ITEM::IsExist(int _iIndex,int _iKind)

//
//	분류별 아이템 수를 얻어 온다.
int
cBASIC_ITEM::GetCountByKind(int _iKind)
{
	int	iCount	=	0;

	for (int i=0;i<dBASIC_ITEM_COUNT;i++)
		if (g_aBasicItem[i].m_iSerial	!=	dVALID_ITEM	)
		{
			if (_iKind	==	dITEM_KIND_ALL)				iCount++;
			else
			if (g_aBasicItem[i].m_wKind	==	_iKind)		iCount++;
		}

	return	iCount;
}	//	cBASIC_ITEM::GetCountByKind(int _iKind)

//
//유니크 아이템의 데이터만 초기화
void
cBASIC_ITEM::ResetUniqueItem()
{
	int	i;

	for (i=0;i<dBASIC_ITEM_COUNT;i++)
	{
		if (g_aBasicItem[i].m_iSerial	==	dVALID_ITEM)	continue;
		if (!g_aBasicItem[i].isUnique())					continue;

		g_aBasicItem[i].reset();
	}

	Arrange();
}

//
//	_strName의 분류
int
cBASIC_ITEM::GetItemKind(char *_strName)
{
	int	iIndex	=	0;

	while(1)
	{
		if (stricmp(g_strItemCategory[iIndex],"end")	==0)	break;

		if (stricmp(g_strItemCategory[iIndex],_strName)	==0)	return	iIndex;

		iIndex++;
	}

	return	0xffff;
}

//
//	유니크 아이템의 갯수 리턴
int
cBASIC_ITEM::GetUniqueCount()
{
	int	iCount	=	0;

	for (int i=0;i<dBASIC_ITEM_COUNT;i++)
		if (g_aBasicItem[i].m_iSerial	!=	dVALID_ITEM	)
			if (g_aBasicItem[i].isUnique())
				iCount++;

	return	iCount;
}	//	cBASIC_ITEM::GetUniqueCount()


BOOL
cBASIC_ITEM::isInfinityBullet()
{
	int		i;

	for (i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
		if (m_aGenerateData[i].m_wEffect	==	eIE_INFINITY_BULLET)	
			return	TRUE;

	for (i=0;i<dITEM_UNIQUE_DATA_COUNT;i++)
		if (m_aUniqueData[i].m_wEffect		==	eIP_INFINITY_BULLET)	
			return	TRUE;

	return	FALSE;
}	//	cBASIC_ITEM::isInfinityBullet()

BOOL
cBASIC_ITEM::addInfinityBulletOption()
{
	int		i;

	for (i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
	{
		if (m_aGenerateData[i].m_wEffect	==	0xffff)
		{
			m_aGenerateData[i].m_wEffect	=	eIE_INFINITY_BULLET;
			memset(m_aGenerateData[i].m_aValue,0,sizeof(m_aGenerateData[i].m_aValue));

			return	TRUE;
		}
	}

	return	FALSE;
}	//	cBASIC_ITEM::addInfinityBulletOption()


int
CCustomItemManager::insert(int _iIndex,CCustomItem *_lpItem)
{
	if (m_iCount >= m_iSize)
		return	0xffff;

	int	i;

	for (i=m_iCount-1;i>=_iIndex;i--)
		memcpy(&m_pItems[i+1],&m_pItems[i],sizeof(CCustomItem));

	memcpy(&m_pItems[_iIndex],_lpItem,sizeof(CCustomItem));

	m_iCount++;

	if (_iIndex	==	m_iRookie)
		for (i=0;i<m_iSize;i++)
			if (m_pItems[i].m_wBaseItem == 0xffff)
			{
				m_iRookie	=	i;
				break;
			}

	return	_iIndex;
}