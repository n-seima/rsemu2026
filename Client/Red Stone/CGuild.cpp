#include "CGuild.h"
#include "CGuildSkill.h"
#include "CMessage.h"
#include "CTitle.h"
#include "CHero.h"
#include "CWindowInterface.h"

CGuild				g_guild;
CGuildManager		g_guildManager;
cGuildBattleInfo	g_guildBattleInfo;
int					g_iRemainGuildBattleTime	=	-1;

UINT	
l_auiGuildPetExp[]	=	//	길드펫 누적 경험치
{
	0 ,
	5 ,
	17 ,
	47 ,
	111 ,
	231 ,
	433 ,
	749 ,
	1215 ,
	1873 ,
	2769 ,
	3954 ,
	5485 ,
	7422 ,
	9831 ,
	12783 ,
	16353 ,
	20622 ,
	25674 ,
	31599 ,
	38491 ,
	46449 ,
	55577 ,
	65983 ,
	77780 ,
	91087 ,
	106025 ,
	122722 ,
	141310 ,
	161924 ,
	184706 ,
	209801 ,
	237360 ,
	267537 ,
	300492 ,
	336389 ,
	375396 ,
	417686 ,
	463437 ,
	512832 ,
	566057 ,
	623303 ,
	684766 ,
	750647 ,
	821151 ,
	896487 ,
	976869 ,
	1062516 ,
	1153651 ,
	1250501 ,
	1353298 ,
	1462279 ,
	1577685 ,
	1699761 ,
	1828757 ,
	1964928 ,
	2108533 ,
	2259835 ,
	2419102 ,
	2586606 ,
	2762624 ,
	2947437 ,
	3141332 ,
	3344598 ,
	3557530 ,
	3780426 ,
	4013591 ,
	4257332 ,
	4511962 ,
	4777797 ,
	5055159 ,
	5344373 ,
	5645769 ,
	5959682 ,
	6286450 ,
	6626417 ,
	6979930 ,
	7347342 ,
	7729009 ,
	8125291 ,
	8536555 ,
	8963169 ,
	9405508 ,
	9863950 ,
	10338878 ,
	10830679 ,
	11339745 ,
	11866472 ,
	12411260 ,
	12974514 ,
	13556642 ,
	14158059 ,
	14779181 ,
	15420432 ,
	16082237 ,
	16765028 ,
	17469239 ,
	18195310 ,
	18943685 ,
	19714813 ,
	0xffffffff,
};

CGuild::CGuild()
{
	reset();
}	//	CGuild::CGuild()

void
CGuild::resetGBS()
{
	m_battleInfo.reset();
}

void
CGuild::reset()
{
	m_wGuildInventorySize		=	0;
	m_wIsNotReceivedMemberList	=	TRUE;
	m_wMemberSortMethod			=	eGMLSM_BY_RANK;
	m_iGuildHonorPoint			=	0;

	resetGBS();

	memset(&m_memberList,0,sizeof(m_memberList));

	CGuildAdvanceInfo::reset();
}

int
CGuild::getInventorySlotIndex(int _iStorage,int _iSlot)
{
	int	iSlotIndex	=	0;

	for	(int i=0;i<_iStorage;i++)
		iSlotIndex	+=	m_abInventorySize[i];

	iSlotIndex		+=	_iSlot;

	return	iSlotIndex;
}

int
CGuild::getSlotIndexInStorage(int _iSlot)
{
	int	iCount	=	0;

	for	(int i=0;i<4;i++)
	{
		if	(_iSlot	<	iCount+m_abInventorySize[i])
			return	_iSlot-iCount;

		iCount	+=	m_abInventorySize[i];
	}

	return	0xffff;
}


bool
CGuild::isValidInventorySlot(int _iSlot)
{
	if	(_iSlot	>=	m_wGuildInventorySize	||	_iSlot	<	0)
		return	false;

	if	(m_aInventory[_iSlot].m_wBaseItem	==	0xffff)
		return	true;

	return	false;
}

void
CGuild::operateGuildSkillData()
{
	int	i;

	m_wMaxCongressMan		=	dGUILD_MIN_CONGRESSMAN;
	m_wMaxSubMaster			=	dGUILD_MIN_SUBMASTER;
	m_wMaxMember			=	dGUILD_MIN_PLAYER;
	m_wSellEnchantItemCount	=	0;

	m_wMaxGuardianCount					=	0;
	m_wIncreaseGuardianSight			=	0;
	m_wIncreaseDamageUnderSiegeWarfare	=	0;
	m_wReduceDamageUnderSiegeWarfare	=	0;
	m_wIncreaeGuildPetSight				=	0;
	m_wIncreaseGuildCrestEfficient		=	0;
	m_wIncreaseGuildDoorEfficient		=	0;
	m_wIncreaseGuildPetAttackSpeed		=	0;
	m_wDecreaseGuildShopItemPrice		=	0;
	m_wIncreaseDashBladeDamage			=	0;
	m_wIncreaseGuildFlagEfficient		=	0;
	m_wIncreaseAstroBowDamage			=	0;
	
	m_wDecreasePeneltyOfFireResistance	=	0;
	m_wDecreasePeneltyOfWaterResistance	=	0;
	m_wDecreasePeneltyOfWindResistance	=	0;
	m_wDecreasePeneltyOfEarthResistance	=	0;
	m_wDecreasePeneltyOfLightResistance	=	0;
	m_wDecreasePeneltyOfDarkResistance	=	0;
	m_wObitianCount						=	0;
	m_wDashBladeCount					=	0;
	m_wAstralBowCount					=	0;
	m_wCristalWaterCount				=	0;
	m_wSellEnchantItemCount				=	0;
	m_wDecreaseStructurePrice			=	0;
	m_wDecreaseDungeonMagicPenelty		=	0;
	m_wDecreaseDeathPenelty				=	0;
	m_wGuildInventorySize				=	0;
	m_wRelatedPlaceCount				=	c_iMinCountForRelatedPlaceOfGuiild;

	for (i=0;i<c_iNormalGuildSkillCount;i++)
	{
		if	(m_abGuildSkill[i]	==	0)		
			continue;

		int	iLevel	=	getApplySkillLevel(i);

		if	(iLevel)
			operateGuildSkillData(i,iLevel);
	}


	for	(int iStatueSkill=0;iStatueSkill<c_iGuildStatueCount;iStatueSkill++)
	{
		int	iSkillIndex	=	c_iGuildStatueSkillBeginIndex+iStatueSkill*c_iGuildStatuePartsCount;
		int	iPartsCount	=	0;

		for (i=iSkillIndex;i<iSkillIndex+c_iGuildStatuePartsCount;i++)
		{
			if	(m_abGuildSkill[i])
				iPartsCount++;
		}

		int	iEffectLimitCount	=	dMAX_GUILD_SKILL_EFFECT_COUNT;

		if	(iPartsCount	<	c_iGuildStatueSetEffectRequireCount)
			iEffectLimitCount	=	1;

		for (i=iSkillIndex;i<iSkillIndex+c_iGuildStatuePartsCount;i++)
		{
			if	(m_abGuildSkill[i]	==	0)
				continue;

			int	iLevel	=	getApplySkillLevel(i);

			if	(iLevel)
				operateGuildSkillData(i,iLevel,iEffectLimitCount);
		}
	}

	if	(m_wGuildInventorySize	<=	0)
		memset(m_abInventorySize,0,sizeof(m_abInventorySize));
}

//
//	업그레이드 가능한 스킬인가?
BOOL
CGuild::isUpgradeAbleSkill(int _iSkill)
{
	if	(m_wGuildPoint	==	0)
		return	FALSE;

	CGuildSkill	*lpSkill=	g_gsManager.get(_iSkill);
	int			i;

	if	(!lpSkill	||	lpSkill->m_bf1IsStatueSkill)		//	스킬 없수?
		return	FALSE;

	int	iCurrentLevel	=	m_abGuildSkill[_iSkill];

	if	(iCurrentLevel	>=	0x8000)	
		return	FALSE;

	if	(iCurrentLevel	>=	lpSkill->m_wTopLevel)	//	한계다.
		return	FALSE;

	if	(iCurrentLevel	>=	getSkillLimitLevel(_iSkill))	//	레벨 한계다.
		return	FALSE;

	if	(lpSkill->m_bf1IsRequireSkillMasteryTitle)
	{
		if	(g_hero.getTitleByTitleEffect(eTE_SKILL_MASTER)	==	NULL)
			return	FALSE;
	}

	if	(lpSkill->m_bf3RequireGuildHallLevel	>	m_wHallLevel)
		return	FALSE;

	if	(g_hero.m_iGold	<	lpSkill->m_iRequireGold)
		return	FALSE;

	for (i=0;i<dGUILD_SKILL_REQUIRE_SKILL_COUNT;i++)
	{
		if	(lpSkill->m_awRequireSkill[i]	==	0xffff)
			break;

		if	(m_abGuildSkill[lpSkill->m_awRequireSkill[i]]	<	lpSkill->m_awRequireSkillLevel[i])
			return	FALSE;
	}

	return	TRUE;
}

//
//	길드 스킬 데이터 처리
void
CGuild::operateGuildSkillData(int _iSkill,int _iLevel,int _iEffectLimitCount)
{
	int			i;
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);

	if	(lpSkill	==	NULL	||	_iLevel	>	lpSkill->m_wTopLevel)	
		return;

	for (i=0;i<_iEffectLimitCount;i++)
	{
		CGuildSkillEffectInfo	*lpEffect	=	&lpSkill->m_aEffect[i];

		if	(lpEffect->m_wEffect	==	0xffff)	
			continue;

		switch(lpEffect->m_wEffect)
		{
			case	eGSE_INCREASE_GUILD_VILLAGE				:
				operateGSIncreaseGuildVillage(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_INCREASE_MAX_GUILD_MEMBER_COUNT	:	//	최대길드원증가
				operateGSIncreaseMaxGuildMemberCount(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_INCREASE_MAX_GUILD_SUB_MASTER_COUNT:	//최대서브마스터수증가
				operateGSIncreaseMaxSubGuildMasterCount(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_INCREASE_MAX_GUILD_CONGRESS_COUNT	:	//	최대길드의원인원수증가
				operateGSIncreaseMaxGuildCongressCount(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_INCREASE_GUILD_INVENTORY_SIZE		:	//	길드인벤토리사이즈증가
				operateGSIncreaseGuildInventorySize(_iSkill,_iLevel,lpEffect);
				break;

//	길드에 설정될 데이터들
			case	eGSE_INCREASE_GUARDIAN_COUNT_PER_LEVEL	:	//{	"가디언수증가per레벨"					:	
				operateGSIncreaseGuardianCount(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_INCREASE_GUARDIAN_SIGHT_PER_LEVEL	:	//{	"가디언시야증가"						:	
				operateGSIncreaseGuardianSight(_iSkill,_iLevel,lpEffect);
				break;

			case	eGSE_DECREASE_FIELD_FIRE_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드불저항페널티감소per레벨"		:	
			case	eGSE_DECREASE_FIELD_WATER_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드물저항페널티감소per레벨"		:	
			case	eGSE_DECREASE_FIELD_WIND_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드바람저항페널티감소per레벨"		:	
			case	eGSE_DECREASE_FIELD_EARTH_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드대지저항페널티감소per레벨"		:	
			case	eGSE_DECREASE_FIELD_LIGHT_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드빛저항페널티감소per레벨"		:	
			case	eGSE_DECREASE_FIELD_DARK_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드어둠저항페널티감소per레벨"		:	
				operateGSDecreaseMagicResistanceFieldPenelty(_iSkill,_iLevel,lpEffect);
				break;

			case	eGSE_INCREASE_OBITIAN_COUNT_PER_LEVEL		:	//{	"오비티안개수증가per레벨"				:	
			case	eGSE_INCREASE_CRISTAL_WATER_COUNT_PER_LEVEL	:	//{	"크리스탈워터개수증가per레벨"			:	
			case	eGSE_INCREASE_ASTRAL_BOW_COUNT_PER_LEVEL	:	//{	"아스트로보우개수증가per레벨"			:	
			case	eGSE_INCREASE_DASH_BLADE_COUNT_PER_LEVEL	:	//{	"대쉬블래이드개수증가per레벨"			:	
				operateGSIncreasePetCount(_iSkill,_iLevel,lpEffect);
				break;

			case	eGSE_SELL_MAGIC_ENCHANT_ITEM						:	//{	"마법인챈트용품판매"				:	
				operateGSAddSellMagicEnchantItem(_iSkill,_iLevel,lpEffect);
				break;

			case	eGSE_DECREASE_DUNGEON_MAGIC_ATTR_PENELTY_PER_LEVEL	:	//{	"던젼속성페널티저하per레벨"						:	
				operateGSDecreaseDungeonMagicPeneltyPerLevel(_iSkill,_iLevel,lpEffect);
				break;

			case	eGSE_DECREASE_DEATH_PENELTY_PER_LEVEL				:	//{	"사망페널티감소per레벨"					:	
				operateGSDecreaseDeathPeneltyPerLevel(_iSkill,_iLevel,lpEffect);
				break;

//	추가된거
			case	eGSE_INCREASE_DAMAGE_UNDER_SIEGE_WARFARE	:	//{	"공성전시대미지감소
				operateGSIncreaseDamageUnderSiegeWarfare(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_REDUCE_DAMAGE_UNDER_SIEGE_WARFARE	:	//{	"공성전시대미지감소
				operateGSReduceDamageUnderSiegeWarfare(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_INCREASE_GUILD_PET_SIGHT			:
				operateGSIncreaseGuildPetSight(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_INCREASE_GUILD_CREST_EFFICIENT		:	//	"길드문장효율증가",
				operateGSIncreaseGuildCrestEfficient(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_INCREASE_GUILD_FLAG_EFFICIENT		:	//	"길드깃발효율증가",
				operateGSIncreaseGuildFlagEfficient(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_INCREASE_ASTRO_BOW_DAMAGE			:	//	"아스트로보우대미지증가",
				operateGSIncreaseAstroBowDamage(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_INCREASE_GUILD_DOOR_EFFICIENT		:	//	"길드문효율증가",
				operateGSIncreaseGuildDoorEfficient(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_INCREASE_GUILD_PET_ATTACK_SPEED	:	//	"길드펫액션속도증가",
				operateGSIncreaseGuildPetAttackSpeed(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_DECREASE_GUILD_SHOP_ITEM_PRICE		:	//	"길드상점아이템가격감소",
				operateGSDecreaseGuildShopItemPrice(_iSkill,_iLevel,lpEffect);
				break;
			case	eGSE_INCREASE_DASH_BLADE_DAMAGE			:	//	"대쉬블레이드대미지증가",
				operateGSIncreaseDashBladeDamage(_iSkill,_iLevel,lpEffect);
				break;
//	추가된거
				
		}
	}
}

//
//	길드 연고지 수 증가
int
CGuild::operateGSIncreaseGuildVillage(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue)
{
	int	iIncreaseCount	=	_iLevel*_lpEffect->m_aiValue[0][0]/100;

	if	(_bIsGetValue)
		return	iIncreaseCount;

	m_wRelatedPlaceCount=	min(m_wRelatedPlaceCount+iIncreaseCount,dMAX_GUILD_VILLAGE_COUNT);

	return	0xffff;	
}

//
//	최대길드원증가
int
CGuild::operateGSIncreaseMaxGuildMemberCount(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue)
{
	int	iIncreaseCount	=	_iLevel*_lpEffect->m_aiValue[0][0]/100;

	if	(_bIsGetValue)
		return	iIncreaseCount;

	m_wMaxMember		=	min(m_wMaxMember+iIncreaseCount,c_iMaxGuildMemberCount);

	return	0xffff;	
}

//
//	최대서브마스터수증가
int
CGuild::operateGSIncreaseMaxSubGuildMasterCount(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue)
{
	int	iIncreaseCount	=	_iLevel*_lpEffect->m_aiValue[0][0]/100;

	if	(_bIsGetValue)
		return	iIncreaseCount;

	m_wMaxSubMaster		=	min(m_wMaxSubMaster+iIncreaseCount,c_iMaxSubGuildMasterCount);

	return	0xffff;	
}

//
//	최대길드의원인원수증가
int
CGuild::operateGSIncreaseMaxGuildCongressCount(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue)
{
	int	iIncreaseCount	=	_iLevel*_lpEffect->m_aiValue[0][0]/100;

	if	(_bIsGetValue)
		return	iIncreaseCount;

	m_wMaxCongressMan	=	min(m_wMaxCongressMan+iIncreaseCount,c_iMaxSubGuildCongressCount);

	return	0xffff;	
}

//
//	길드인벤토리사이즈증가
int
CGuild::operateGSIncreaseGuildInventorySize(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue)
{
	int	iIncreaseCount	=	_iLevel*_lpEffect->m_aiValue[0][0]/100;

	if	(_bIsGetValue)
		return	iIncreaseCount;

	if	(m_wHallLevel	==	0)
		return	0xffff;

	m_wGuildInventorySize=	min(m_wGuildInventorySize+iIncreaseCount,dGUILD_MAX_INVEN);

	return	0xffff;
}

int
CGuild::operateGSIncreaseGuardianCount(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	m_wMaxGuardianCount		+=	iValue;

	return	0xffff;
}

int
CGuild::operateGSIncreaseGuardianSight(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	m_wIncreaseGuardianSight+=	iValue;

	return	0xffff;
}

int
CGuild::operateGSDecreaseMagicResistanceFieldPenelty(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	switch(_lpEffect->m_wEffect)
	{
		case	eGSE_DECREASE_FIELD_FIRE_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드불저항페널티감소per레벨"		:	
			m_wDecreasePeneltyOfFireResistance	+=	iValue;
			break;
		case	eGSE_DECREASE_FIELD_WATER_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드물저항페널티감소per레벨"		:	
			m_wDecreasePeneltyOfWaterResistance	+=	iValue;
			break;
		case	eGSE_DECREASE_FIELD_WIND_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드바람저항페널티감소per레벨"		:	
			m_wDecreasePeneltyOfWindResistance	+=	iValue;
			break;
		case	eGSE_DECREASE_FIELD_EARTH_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드대지저항페널티감소per레벨"		:	
			m_wDecreasePeneltyOfEarthResistance	+=	iValue;
			break;
		case	eGSE_DECREASE_FIELD_LIGHT_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드빛저항페널티감소per레벨"		:	
			m_wDecreasePeneltyOfLightResistance	+=	iValue;
			break;
		case	eGSE_DECREASE_FIELD_DARK_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드어둠저항페널티감소per레벨"		:	
			m_wDecreasePeneltyOfDarkResistance	+=	iValue;
			break;
	}

	return	0xffff;
}

int
CGuild::operateGSIncreasePetCount(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	switch(_lpEffect->m_wEffect)
	{
		case	eGSE_INCREASE_OBITIAN_COUNT_PER_LEVEL		:	//{	"오비티안개수증가per레벨"				:	
			m_wObitianCount		+=	iValue;
			break;
		case	eGSE_INCREASE_CRISTAL_WATER_COUNT_PER_LEVEL	:	//{	"크리스탈워터개수증가per레벨"			:	
			m_wCristalWaterCount+=	iValue;
			break;
		case	eGSE_INCREASE_ASTRAL_BOW_COUNT_PER_LEVEL	:	//{	"아스트로보우개수증가per레벨"			:	
			m_wAstralBowCount	+=	iValue;
			break;
		case	eGSE_INCREASE_DASH_BLADE_COUNT_PER_LEVEL	:	//{	"대쉬블래이드개수증가per레벨"			:	
			m_wDashBladeCount	+=	iValue;
			break;
	}

	return	0xffff;
}

int
CGuild::operateGSAddSellMagicEnchantItem(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue)
{
	for	(int i=0;i<dGUILD_SKILL_EFFECT_VARIATION_COUNT;i++)
	{
		if	(m_wSellEnchantItemCount	>=	c_iGuildHallSellEnchantItemCount)
			break;

		if	(_lpEffect->m_aiValue[0][i])
			m_awSellEnchantItem[m_wSellEnchantItemCount++]	+=	_lpEffect->m_aiValue[0][i];
	}

	return	0xffff;
}

int
CGuild::operateGSDecreaseDungeonMagicPeneltyPerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	m_wDecreaseDungeonMagicPenelty	+=	iValue;

	return	0xffff;
}

int
CGuild::operateGSDecreaseDeathPeneltyPerLevel(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	m_wDecreaseDeathPenelty	+=	iValue;

	return	0xffff;
}

int
CGuild::operateGSIncreaseDamageUnderSiegeWarfare(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	m_wIncreaseDamageUnderSiegeWarfare	+=	iValue;
	
	return	0xffff;
}

int
CGuild::operateGSReduceDamageUnderSiegeWarfare(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	m_wReduceDamageUnderSiegeWarfare	+=	iValue;

	return	0xffff;
}

int
CGuild::operateGSIncreaseGuildPetSight(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue)
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	m_wIncreaeGuildPetSight	+=	iValue;

	return	0xffff;
}

int		
CGuild::operateGSIncreaseGuildCrestEfficient(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue)	//	길드문장효율증가",
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	m_wIncreaseGuildCrestEfficient	+=	iValue;

	return	0xffff;
}

int		
CGuild::operateGSIncreaseGuildFlagEfficient(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue)	//	"길드깃발효율증가",
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	m_wIncreaseGuildFlagEfficient	+=	iValue;

	return	0xffff;
}

int		
CGuild::operateGSIncreaseAstroBowDamage(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue)	//	"아스트로보우대미지증가",
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	m_wIncreaseAstroBowDamage	+=	iValue;

	return	0xffff;
}

int		
CGuild::operateGSIncreaseGuildDoorEfficient(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue)	//	"길드문효율증가",
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	m_wIncreaseGuildDoorEfficient	+=	iValue;

	return	0xffff;
}

int		
CGuild::operateGSIncreaseGuildPetAttackSpeed(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue)	//	"길드펫액션속도증가",
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	m_wIncreaseGuildPetAttackSpeed	+=	iValue;

	return	0xffff;
}

int		
CGuild::operateGSDecreaseGuildShopItemPrice(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue)	//	"길드상점아이템가격감소",
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	m_wDecreaseGuildShopItemPrice	+=	iValue;

	return	0xffff;
}

int		
CGuild::operateGSIncreaseDashBladeDamage(int _iSkill,int _iLevel,CGuildSkillEffectInfo	*_lpEffect,BOOL _bIsGetValue)	//	"대쉬블레이드대미지증가",
{
	int			iValue		=	_iLevel*_lpEffect->m_aiValue[0][0]/100;;

	if	(_bIsGetValue)
		return	iValue;

	m_wIncreaseDashBladeDamage	+=	iValue;

	return	0xffff;
}

//
//	최고레벨(이 이상 못 올라간다.)
int
CGuild::getSkillTopLevel(int _iSkill)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);

	if	(lpSkill	==	NULL)	
		return	0;

	return	lpSkill->m_wTopLevel;
}

//
//	이런저런 상황을 고려해 계산한 한계 레벨
int
CGuild::getSkillLimitLevel(int _iSkill)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);

	if (lpSkill	==	NULL)	return	0;

	int	iLevel	=	lpSkill->m_wTopLevel;

	if (lpSkill->m_sMaxLevelByGuildHallLevel != 0)
		iLevel		=	min(iLevel,m_wHallLevel-lpSkill->m_sMaxLevelByGuildHallLevel);

	switch (lpSkill->m_wTopLevelLimitExpression)
	{
		case	1	:	//	(10+[스킬0] Lv*10-[스킬1]Lv*5-1)까지만 레벨 업 가능
		{
			int	iValue1,iValue2,iLimitLevel;

			iValue1		=	lpSkill->m_aTopLevelLimitExpressionValue[0];
			iValue1		=	m_abGuildSkill[iValue1];
			iValue2		=	lpSkill->m_aTopLevelLimitExpressionValue[1];
			iValue2		=	m_abGuildSkill[iValue2];
			iLimitLevel	=	10+iValue1*10-iValue2*5-1;
			iLevel		=	min(iLevel,iLimitLevel);
			break;
		}
		
		case	2	:	//	(10+[스킬0]Lv*10-[스킬1]Lv-1)/5까지만 레벨 업 가능
		{
			int	iValue1,iValue2,iLimitLevel;

			iValue1		=	lpSkill->m_aTopLevelLimitExpressionValue[0];
			iValue1		=	m_abGuildSkill[iValue1];
			iValue2		=	lpSkill->m_aTopLevelLimitExpressionValue[1];
			iValue2		=	m_abGuildSkill[iValue2];
			iLimitLevel	=	(10+iValue1*10-iValue2-1)/5;
			iLevel		=	min(iLevel,iLimitLevel);
			break;
		}
	}

	return	iLevel;
}

//
//	적용 할때 레벨
int
CGuild::getApplySkillLevel(int _iSkill)
{
	CGuildSkill	*lpSkill	=	g_gsManager.get(_iSkill);

	if (lpSkill	==	NULL)
		return	0;

	int	iLevel	=	m_abGuildSkill[_iSkill];

	if (iLevel	>=	0x8000)
		return	0;

	if (iLevel	==	0)
		return	0;

	int	iLimitLevel	=	getSkillLimitLevel(_iSkill);

	iLevel		=	min(iLevel,iLimitLevel);
	//iLevel		=	min(iLevel, m_wHallLevel);

	return	iLevel;
}

//
//
char*
CGuild::getSkillComment(int _iSkill)
{
	static	cSTRING	s_strTooltip;

	s_strTooltip.Reset();

	int			iLevel			=	m_abGuildSkill[_iSkill],i;
	CGuildSkill	*lpSkill		=	g_gsManager.get(_iSkill);
	int			iEffectLimitCount	=	dMAX_GUILD_SKILL_EFFECT_COUNT;

	if	(lpSkill	==	NULL)	
		return	NULL;

	if	(lpSkill->m_wTopLevel	==	0)
		s_strTooltip.Add("<c:LTRED> !!! %s<n>\n\n",dMSG_NOT_EMBODED_SKILL);

	if	(iLevel)
	{
		s_strTooltip.Add("<c:LTYELLOW>%s<n><Lv %d>\n\n",lpSkill->m_strName,iLevel);
// 		if(m_wHallLevel > iLevel)
// 			s_strTooltip.Add("<c:LTYELLOW>%s<n><c:LTRED><Lv %d><n>\n\n",lpSkill->m_strName,iLevel);
// 		else 
// 			s_strTooltip.Add("<c:LTYELLOW>%s<n><Lv %d>\n\n",lpSkill->m_strName,iLevel);

	}
	else		
		s_strTooltip.Add("<c:LTYELLOW>%s<n>\n\n",lpSkill->m_strName);

	int	iRealLevel	=	getApplySkillLevel(_iSkill);

	if	(lpSkill->m_wTopLevel)
	{
		s_strTooltip.Add("<c:LTCYAN><%s><n>\n",dMSG_INFORMATION);
		int	iLimitLevel	=	getSkillLimitLevel(_iSkill);

		if (iLimitLevel	!=	lpSkill->m_wTopLevel)
			s_strTooltip.Add("- %s [Lv <c:LTRED>%d<n>/<c:LTYELLOW>%d<n>]<n>\n",dMSG_LITMIT_LEVEL,iLimitLevel,lpSkill->m_wTopLevel);
		else
			s_strTooltip.Add("- %s [Lv <c:LTYELLOW>%d<n>]<n>\n",dMSG_LITMIT_LEVEL,lpSkill->m_wTopLevel);

		if	(iLevel		!=	iRealLevel)
			s_strTooltip.Add("- %s <c:LTYELLOW>[Lv %d]<n>\n",dMSG_REAL_APPLY_LEVEL,iRealLevel);
	}

	if	(_iSkill	>=	c_iNormalGuildSkillCount)
	{
		int	iBegin		=	c_iNormalGuildSkillCount+((_iSkill-c_iNormalGuildSkillCount)/c_iGuildStatuePartsCount)*c_iGuildStatuePartsCount;
		int	iPartsCount	=	0;

		for	(int iStatueSkill=iBegin;iStatueSkill<iBegin+c_iGuildStatuePartsCount;iStatueSkill++)
		{
			if	(m_abGuildSkill[iStatueSkill])
				iPartsCount++;
		}

		if	(iPartsCount	<	c_iGuildStatueSetEffectRequireCount)
			iEffectLimitCount	=	1;
	}

	for (i=0;i<dMAX_GUILD_SKILL_EFFECT_COUNT;i++)
	{
		CGuildSkillEffectInfo	*lpEffectInfo	=	&lpSkill->m_aEffect[i];

		if	(lpEffectInfo->m_wEffect	==	0xffff || iRealLevel == 0)
			break;

		int	iReturnValue;
		int	iValue1	=	lpEffectInfo->m_aiValue[0][3];	//	신입 길드원
		int	iValue2	=	lpEffectInfo->m_aiValue[0][2];	//	길드 원로,길드 의원,정식 길드
		int	iValue3	=	lpEffectInfo->m_aiValue[0][1];	//	부길마
		int	iValue4	=	lpEffectInfo->m_aiValue[0][0];	//	길마

		switch(lpEffectInfo->m_wEffect)
		{
		case	eGSE_INCREASE_HP						:	//체력증가	
			iReturnValue	=	g_hero.operateGSIncreaseHP(_iSkill,iRealLevel,TRUE);
			
			s_strTooltip.Add("- %s <c:LTYELLOW>+%d<n> [Lv x %d %d %d %d]\n",dMSG_HP,iReturnValue,iValue1,iValue2,iValue3,iValue4);
			break;
		case	eGSE_INCREASE_CP						:	//CP증가					
			iReturnValue	=	g_hero.operateGSIncreaseCP(_iSkill,iRealLevel,TRUE);
			s_strTooltip.Add("- %s <c:LTYELLOW>+%d<n> [Lv x %d %d %d %d]\n",dMSG_CP,iReturnValue,iValue1,iValue2,iValue3,iValue4);
			break;
		case	eGSE_INCREASE_HIT_CHANCE				:	//정확도증가				
			iReturnValue	=	g_hero.operateGSIncreaseHitChance(_iSkill,iRealLevel,TRUE);
#ifdef	_FOR_KOREA
			s_strTooltip.Add("- %s <c:LTYELLOW>+%d％<n> [Lv x %d.%.1d %d.%.1d %d.%.1d %d.%.1d]\n",dMSG_HIT_CHANCE,iReturnValue,iValue1/100,(iValue1%100)/10,iValue2/100,(iValue2%100)/10,iValue3/100,(iValue3%100)/10,iValue4/100,(iValue4%100)/10);
#elif	_FOR_JAPAN
			s_strTooltip.Add("- %s <c:LTYELLOW>+%d걪<n> [Lv x %d.%.1d %d.%.1d %d.%.1d %d.%.1d]\n",dMSG_HIT_CHANCE,iReturnValue,iValue1/100,(iValue1%100)/10,iValue2/100,(iValue2%100)/10,iValue3/100,(iValue3%100)/10,iValue4/100,(iValue4%100)/10);
#else
			s_strTooltip.Add("- %s <c:LTYELLOW>+%d<n>percent [Lv x %d.%.1d %d.%.1d %d.%.1d %d.%.1d]\n",dMSG_HIT_CHANCE,iReturnValue,iValue1/100,(iValue1%100)/10,iValue2/100,(iValue2%100)/10,iValue3/100,(iValue3%100)/10,iValue4/100,(iValue4%100)/10);
#endif
			break;
		case	eGSE_INCREASE_DODGE_CHANCE				:	//회피율증가				
			iReturnValue	=	g_hero.operateGSIncreaseDodgeChance(_iSkill,iRealLevel,TRUE);
#ifdef	_FOR_KOREA
			s_strTooltip.Add("- %s <c:LTYELLOW>+%d％<n> [Lv x %d.%.1d %d.%.1d %d.%.1d %d.%.1d]\n",dMSG_DODGE_CHANCE,iReturnValue,iValue1/100,(iValue1%100)/10,iValue2/100,(iValue2%100)/10,iValue3/100,(iValue3%100)/10,iValue4/100,(iValue4%100)/10);
#elif	_FOR_JAPAN
			s_strTooltip.Add("- %s <c:LTYELLOW>+%d걪<n> [Lv x %d.%.1d %d.%.1d %d.%.1d %d.%.1d]\n",dMSG_DODGE_CHANCE,iReturnValue,iValue1/100,(iValue1%100)/10,iValue2/100,(iValue2%100)/10,iValue3/100,(iValue3%100)/10,iValue4/100,(iValue4%100)/10);
#else
			s_strTooltip.Add("- %s <c:LTYELLOW>+%d<n>percent [Lv x %d.%.1d %d.%.1d %d.%.1d %d.%.1d]\n",dMSG_DODGE_CHANCE,iReturnValue,iValue1/100,(iValue1%100)/10,iValue2/100,(iValue2%100)/10,iValue3/100,(iValue3%100)/10,iValue4/100,(iValue4%100)/10);
#endif
			break;
		case	eGSE_INCREASE_STRENGTH					:	//힘증가					
			iReturnValue	=	g_hero.operateGSIncreaseStrength(_iSkill,iRealLevel,TRUE);
			s_strTooltip.Add("- %s <c:LTYELLOW>+%d<n> [Lv x %d %d %d %d]\n",dMSG_STRENGTH,iReturnValue,iValue1,iValue2,iValue3,iValue4);
			break;
		case	eGSE_INCREASE_AGILITY					:	//민첩성증가				
			iReturnValue	=	g_hero.operateGSIncreaseAgility(_iSkill,iRealLevel,TRUE);
			s_strTooltip.Add("- %s <c:LTYELLOW>+%d<n> [Lv x %d %d %d %d]\n",dMSG_AGILITY,iReturnValue,iValue1,iValue2,iValue3,iValue4);
			break;
		case	eGSE_INCREASE_CONSTITUTION				:	//건강증가				
			iReturnValue	=	g_hero.operateGSIncreaseConstitution(_iSkill,iRealLevel,TRUE);
			s_strTooltip.Add("- %s <c:LTYELLOW>+%d<n> [Lv x %d %d %d %d]\n",dMSG_HEALTH,iReturnValue,iValue1,iValue2,iValue3,iValue4);
			break;
		case	eGSE_INCREASE_INTELLIGENCE				:	//지식증가				
			iReturnValue	=	g_hero.operateGSIncreaseIntelligence(_iSkill,iRealLevel,TRUE);
			s_strTooltip.Add("- %s <c:LTYELLOW>+%d<n> [Lv x %d %d %d %d]\n",dMSG_INTELLIGENCE,iReturnValue,iValue1,iValue2,iValue3,iValue4);
			break;
		case	eGSE_INCREASE_WISDOM					:	//지혜증가				
			iReturnValue	=	g_hero.operateGSIncreaseWisdom(_iSkill,iRealLevel,TRUE);
			s_strTooltip.Add("- %s <c:LTYELLOW>+%d<n> [Lv x %d %d %d %d]\n",dMSG_WISDOM,iReturnValue,iValue1,iValue2,iValue3,iValue4);
			break;
		case	eGSE_INCREASE_CHARISMA					:	//카리스마증가			
			iReturnValue	=	g_hero.operateGSIncreaseCharisma(_iSkill,iRealLevel,TRUE);
			s_strTooltip.Add("- %s <c:LTYELLOW>+%d<n> [Lv x %d %d %d %d]\n",dMSG_CHARISMA,iReturnValue,iValue1,iValue2,iValue3,iValue4);
			break;
		case	eGSE_INCREASE_LUCK						:	//운증가					
			iReturnValue	=	g_hero.operateGSIncreaseLuck(_iSkill,iRealLevel,TRUE);
			s_strTooltip.Add("- %s <c:LTYELLOW>+%d<n> [Lv x %d %d %d %d]\n",dMSG_LUCK,iReturnValue,iValue1,iValue2,iValue3,iValue4);
				break;

			case	eGSE_INCREASE_KNIGHT_SKILL_LEVEL		:	//기사스킬레벨증가		
			case	eGSE_INCREASE_WARRIOR_SKILL_LEVEL		:	//전사스킬레벨증가		
			case	eGSE_INCREASE_WIZARD_SKILL_LEVEL		:	//마법사스킬레벨증가		
			case	eGSE_INCREASE_WEREWOLF_SKILL_LEVEL		:	//늑대인간스킬레벨증가	
			case	eGSE_INCREASE_PRIEST_SKILL_LEVEL		:	//사제스킬레벨증가		
			case	eGSE_INCREASE_FALLEN_ANGEL_SKILL_LEVEL	:	//타락천사스킬레벨증가	
			case	eGSE_INCREASE_ROGUE_SKILL_LEVEL			:	//도둑스킬레벨증가		
			case	eGSE_INCREASE_FIGHTER_SKILL_LEVEL		:	//무도가스킬레벨증가		
			case	eGSE_INCREASE_MAGIC_ARCHER_SKILL_LEVEL	:	//마법궁수스킬레벨증가	
			case	eGSE_INCREASE_MAGIC_LANCER_SKILL_LEVEL	:	//마법창병스킬레벨증가	
			case	eGSE_INCREASE_TAMER_SKILL_LEVEL			:	//조련사스킬레벨증가		
			case	eGSE_INCREASE_SUMMONER_SKILL_LEVEL		:	//소환사스킬레벨증가		
			case	eGSE_INCREASE_PRINCESS_SKILL_LEVEL		:	//공주스킬레벨증가		
			case	eGSE_INCREASE_MAGIC_GIRL_SKILL_LEVEL	:	//마법소녀스킬레벨증가	
			case	eGSE_INCREASE_NECROMANCER_SKILL_LEVEL	:	//강신술사스킬레벨증가	
			case	eGSE_INCREASE_DEVIL_SKILL_LEVEL			:	//악마스킬레벨증가		
			{
				int	iJob		=	lpEffectInfo->m_wEffect-eGSE_INCREASE_KNIGHT_SKILL_LEVEL;
				iReturnValue	=	g_hero.operateGSIncreaseJobSkillLevel(_iSkill,iRealLevel,iJob,TRUE);
				s_strTooltip.Add("- %s %s <c:LTYELLOW>+%d<n> [Lv / %d %d %d %d]\n",g_pstrHeroJobName[iJob],dMSG_SKILL_LEVEL,iReturnValue,iValue1,iValue2,iValue3,iValue4);
				break;
			}
			case	eGSE_INCREASE_SOUL_BRINGER_SKILL_LEVEL			:	//영술사스킬레벨증가	
			case	eGSE_INCREASE_CHAMPION_SKILL_LEVEL				:	// 투사 스킬레벨증가
			case	eGSE_INCREASE_OPTICALIST_SKILL_LEVEL			:	// 광학사 스킬레벨증가		
			{
				int	iJob		=	dJOB_SOUL_BRINGER + lpEffectInfo->m_wEffect-eGSE_INCREASE_SOUL_BRINGER_SKILL_LEVEL;
				iReturnValue	=	g_hero.operateGSIncreaseJobSkillLevel(_iSkill,iRealLevel,iJob,TRUE);
				s_strTooltip.Add("- %s %s <c:LTYELLOW>+%d<n> [Lv / %d %d %d %d]\n",g_pstrHeroJobName[iJob],dMSG_SKILL_LEVEL,iReturnValue,iValue1,iValue2,iValue3,iValue4);
				break;
			}
		}
	}

	CGuildSkillEffectInfo	*lpCheckEffectInfo = &lpSkill->m_aEffect[0];

	if(lpCheckEffectInfo->m_wEffect >= eGSE_ABSORB_FIRE_PER_LEVEL)
		s_strTooltip.Add(_ms("- %s\n" , dMSG_COMPLATE_BONUS));

	for (i=0;i<dMAX_GUILD_SKILL_EFFECT_COUNT;i++)
	{
		CGuildSkillEffectInfo	*lpEffectInfo	=	&lpSkill->m_aEffect[i];

		if	(lpEffectInfo->m_wEffect	==	0xffff)
			break;

		int		iReturnValue;
		int		iValue1	=	lpEffectInfo->m_aiValue[0][3];	//	신입 길드원
		int		iValue2	=	lpEffectInfo->m_aiValue[0][2];	//	길드 원로,길드 의원,정식 길드
		int		iValue3	=	lpEffectInfo->m_aiValue[0][1];	//	부길마
		int		iValue4	=	lpEffectInfo->m_aiValue[0][0];	//	길마

		char	strTextColor[32]	=	"<c:WHITE>";
		char	strValueColor[32]	=	"<c:LTYELLOW>";

		if	(i	>=	iEffectLimitCount	||	iRealLevel == 0)
		{
			strcpy(strTextColor,"<c:GRAY>");
			strcpy(strValueColor,"<c:GRAY>");
		}

		switch(lpEffectInfo->m_wEffect)
		{
			case	eGSE_ABSORB_FIRE_PER_LEVEL	:	//{	"불흡수per레벨"			:	
			case	eGSE_ABSORB_WATER_PER_LEVEL	:	//{	"물흡수per레벨"			:	
			case	eGSE_ABSORB_WIND_PER_LEVEL	:	//{	"바람흡수per레벨"		:	
			case	eGSE_ABSORB_EARTH_PER_LEVEL	:	//{	"대지흡수per레벨"		:	
			case	eGSE_ABSORB_LIGHT_PER_LEVEL	:	//{	"빛흡수per레벨"			:	
			case	eGSE_ABSORB_DARK_PER_LEVEL	:	//{	"어둠흡수per레벨"		:
			{
				int	iMessage	=	dWM_ABSORB_MAGIC_DAMAGE_MESSAGE_BEGIN+lpEffectInfo->m_wEffect-eGSE_ABSORB_FIRE_PER_LEVEL;
				
				iReturnValue	=	g_hero.operateGSAbsorbMagicDamagePerLevel(_iSkill,iRealLevel,lpEffectInfo,TRUE);

				s_strTooltip.Add("%s- %s %s+%d%s<n>\n",strTextColor,g_pstrCommonMessage[iMessage],strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;
			}

			case	eGSE_INCREASE_RESISTANCE_OF_FIRE_PER_LEVEL	:	//{	"불저항력증가per레벨"		:	
			case	eGSE_INCREASE_RESISTANCE_OF_WATER_PER_LEVEL	:	//{	"물저항력증가per레벨"		:	
			case	eGSE_INCREASE_RESISTANCE_OF_WIND_PER_LEVEL	:	//{	"바람저항력증가per레벨"		:	
			case	eGSE_INCREASE_RESISTANCE_OF_EARTH_PER_LEVEL	:	//{	"대지저항력증가per레벨"		:	
			case	eGSE_INCREASE_RESISTANCE_OF_LIGHT_PER_LEVEL	:	//{	"빛저항력증가per레벨"		:	
			case	eGSE_INCREASE_RESISTANCE_OF_DARK_PER_LEVEL	:	//{	"어둠저항력증가per레벨"		:	
			{
				int	iMessage	=	dWM_INCREASE_MAGIC_RESISTANCE_MESSAGE_BEGIN+lpEffectInfo->m_wEffect-eGSE_INCREASE_RESISTANCE_OF_FIRE_PER_LEVEL;

				iReturnValue	=	g_hero.operateGSIncreaseMagicResistancePerLevel(_iSkill,iRealLevel,lpEffectInfo,TRUE);

				s_strTooltip.Add("%s- %s %s+%d%s<n>\n",strTextColor,g_pstrCommonMessage[iMessage],strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;
			}

			case	eGSE_INCREASE_CRITICAL_CRUSH_RESISTANCE_PER_LEVEL	:	//{	"치명타결정타저항증가per레벨"			:
			{
				iReturnValue	=	g_hero.operateGSIncreaseCrushResistancePerLevel(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d%s<n>\n",strTextColor,dMSG_CRUSH_RESISTANCE,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;
			}

			case	eGSE_INCREASE_CURSE_RESISTANCE_PER_LEVEL	:	//{	"저주계저항증가per레벨"					:	
			{
				iReturnValue	=	g_hero.operateGSIncreaseCurseResistancePerLevel(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d%s<n>\n",strTextColor,dMSG_CURSE_RESISTANCE,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;
			}

			case	eGSE_INCREASE_STRANGE_STATUS_RESISTANCE_PER_LEVEL	:	//{	"이상계저항증가per레벨"					:	
			{
				iReturnValue	=	g_hero.operateGSIncreaseStrangeStatusResistancePerLevel(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d%s<n>\n",strTextColor,dMSG_STRANGE_STATUS_RESISTANCE,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;
			}

			case	eGSE_INCREASE_DECLINE_POWER_RESISTANCE_PER_LEVEL	:	//{	"저하계저항증가per레벨"					:	
			{
				iReturnValue	=	g_hero.operateGSIncreaseDeclinePowerResitancePerLevel(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d%s<n>\n",strTextColor,dMSG_DECLINE_POWER_RESISTANCE,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;
			}

			case	eGSE_INCREASE_PHYSICAL_MAGIC_DAMAGE_PER_LEVEL		:	//{	"물리공격력증가per레벨"					:	
			{
				iReturnValue	=	g_hero.operateGSIncreasePhysicalMagicDamagePerLevel(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d%s<n>\n",strTextColor,dMSG_PHYSICAL_MAGIC_DAMAGE_RESISTANCE,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;
			}
			case	eGSE_DEFENSIVE_POWER_PER_LEVEL	:	//{	"방어력증가퍼센티지per레벨"				:	
			{
				iReturnValue	=	g_hero.operateGSIncreaseDefensivePowerPerLevel(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d%s<n>\n",strTextColor,dMSG_DEFENSIVE_POWER,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;
			}

			case	eGSE_INCREASE_ATTACK_SPEED_PER_LEVEL	:	//{	"공격속도증가per레벨"					:	
			{
				iReturnValue	=	g_hero.operateGSIncreaseActionSpeedPerLevel(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d%s<n>\n",strTextColor,dMSG_ATTACK_SPEED,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;
			}
			case	eGSE_INCREASE_MOVE_SPEED_PER_LEVEL	:	//{	"이동속도증가per레벨"					:	
			{
				iReturnValue	=	g_hero.operateGSIncreaseMoveSpeedPerLevel(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d%s<n>\n",strTextColor,dMSG_MOVE_SPEED,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;
			}

			case	eGSE_INCREASE_HIT_CHANCE_PER_LEVEL	:	//{	"명중률증가per레벨"						:	
			{
				iReturnValue	=	g_hero.operateGSIncreaseHitChancePerLevel(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d%s<n>\n",strTextColor,dMSG_HIT_CHANCE,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;
			}
			case	eGSE_INCREASE_DODGE_CHANCE_PER_LEVEL	:	//{	"회피율증가per레벨"						:	
			{
				iReturnValue	=	g_hero.operateGSIncreaseDodgeChancePerLevel(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d%s<n>\n",strTextColor,dMSG_DODGE_CHANCE,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;
			}

			case	eGSE_INCREASE_MAX_GUILD_MEMBER_COUNT	:	//	최대길드원증가
				iReturnValue	=	operateGSIncreaseMaxGuildMemberCount(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d<n>\n",strTextColor,dMSG_GUILD_MEMBER_COUNT,strValueColor,iReturnValue);
				break;
			case	eGSE_INCREASE_MAX_GUILD_SUB_MASTER_COUNT:	//최대서브마스터수증가
				iReturnValue	=	operateGSIncreaseMaxSubGuildMasterCount(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d<n>\n",strTextColor,dMSG_SUB_GUILD_MASTER_COUNT,strValueColor,iReturnValue);
				break;
			case	eGSE_INCREASE_MAX_GUILD_CONGRESS_COUNT	:	//	최대길드의원인원수증가
				iReturnValue	=	operateGSIncreaseMaxGuildCongressCount(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d<n>\n",strTextColor,dMSG_GUILD_GONGRESS_COUNT,strValueColor,iReturnValue);
				break;
			case	eGSE_INCREASE_GUILD_INVENTORY_SIZE		:	//	길드인벤토리사이즈증가
				iReturnValue	=	operateGSIncreaseGuildInventorySize(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d<n>\n",strTextColor,dMSG_GUILD_INVENTORY_SIZE,strValueColor,iReturnValue);
				break;
			case	eGSE_INCREASE_OBITIAN_COUNT_PER_LEVEL		:	//{	"오비티안개수증가per레벨"				:	
			{
				iReturnValue	=	g_guild.operateGSIncreasePetCount(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d<n>\n",strTextColor,dMSG_GUILD_PET_OBITIAN_COUNT,strValueColor,iReturnValue);
				break;
			}
			case	eGSE_INCREASE_CRISTAL_WATER_COUNT_PER_LEVEL	:	//{	"크리스탈워터개수증가per레벨"			:	
			{
				iReturnValue	=	g_guild.operateGSIncreasePetCount(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d<n>\n",strTextColor,dMSG_GUILD_PET_CRISTAL_WATER_COUNT,strValueColor,iReturnValue);
				break;
			}
			case	eGSE_INCREASE_ASTRAL_BOW_COUNT_PER_LEVEL	:	//{	"아스트로보우개수증가per레벨"			:	
			{
				iReturnValue	=	g_guild.operateGSIncreasePetCount(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d<n>\n",strTextColor,dMSG_GUILD_PET_ASTRAL_BOW_COUNT,strValueColor,iReturnValue);
				break;
			}
			case	eGSE_INCREASE_DASH_BLADE_COUNT_PER_LEVEL	:	//{	"대쉬블래이드개수증가per레벨"			:	
			{
				iReturnValue	=	g_guild.operateGSIncreasePetCount(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d<n>\n",strTextColor,dMSG_GUILD_PET_DASH_BLADE_COUNT,strValueColor,iReturnValue);
				break;
			}

//	길드에 설정될 데이터들
			case	eGSE_INCREASE_GUARDIAN_COUNT_PER_LEVEL	:	//{	"가디언수증가per레벨"					:	
				iReturnValue	=	g_guild.operateGSIncreaseGuardianCount(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d<n>\n",strTextColor,dMSG_GUILD_GUARDIAN_COUNT,strValueColor,iReturnValue);
				break;
			case	eGSE_INCREASE_GUARDIAN_SIGHT_PER_LEVEL	:	//{	"가디언시야증가"						:	
				iReturnValue	=	g_guild.operateGSIncreaseGuardianSight(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d<n>\n",strTextColor,dMSG_GUILD_GUARDIAN_SIGHT,strValueColor,iReturnValue);
				break;


			case	eGSE_DECREASE_FIELD_FIRE_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드불저항페널티감소per레벨"		:	
				iReturnValue	=	g_guild.operateGSDecreaseMagicResistanceFieldPenelty(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s%d%s<n>\n",strTextColor,dMSG_DECREASE_FIELD_FIRE_RESISTANCE_PENELTY,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;
			case	eGSE_DECREASE_FIELD_WATER_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드물저항페널티감소per레벨"		:	
				iReturnValue	=	g_guild.operateGSDecreaseMagicResistanceFieldPenelty(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s%d%s<n>\n",strTextColor,dMSG_DECREASE_FIELD_WATER_RESISTANCE_PENELTY,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;
			case	eGSE_DECREASE_FIELD_WIND_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드바람저항페널티감소per레벨"		:	
				iReturnValue	=	g_guild.operateGSDecreaseMagicResistanceFieldPenelty(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s%d%s<n>\n",strTextColor,dMSG_DECREASE_FIELD_WIND_RESISTANCE_PENELTY,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;
			case	eGSE_DECREASE_FIELD_EARTH_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드대지저항페널티감소per레벨"		:	
				iReturnValue	=	g_guild.operateGSDecreaseMagicResistanceFieldPenelty(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s%d%s<n>\n",strTextColor,dMSG_DECREASE_FIELD_EARTH_RESISTANCE_PENELTY,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;
			case	eGSE_DECREASE_FIELD_LIGHT_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드빛저항페널티감소per레벨"		:	
				iReturnValue	=	g_guild.operateGSDecreaseMagicResistanceFieldPenelty(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s%d%s<n>\n",strTextColor,dMSG_DECREASE_FIELD_LIGHT_RESISTANCE_PENELTY,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;
			case	eGSE_DECREASE_FIELD_DARK_RESISTANCE_PENELTY_PER_LEVEL	:	//{	"필드어둠저항페널티감소per레벨"		:	
				iReturnValue	=	g_guild.operateGSDecreaseMagicResistanceFieldPenelty(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s%d%s<n>\n",strTextColor,dMSG_DECREASE_FIELD_DARK_RESISTANCE_PENELTY,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;

			case	eGSE_SELL_MAGIC_ENCHANT_ITEM						:	//{	"마법인챈트용품판매"				:	
			{
				s_strTooltip.Add("%s- %s %s\n",strTextColor,dMSG_SELL_MAGIC_ENCHANT_ITEM,strValueColor);

				for	(int i=0;i<dGUILD_SKILL_EFFECT_VARIATION_COUNT;i++)
				{
					int	iBasicItem	=	lpEffectInfo->m_aiValue[0][i];

					if	(iBasicItem	==	0)
						continue;

					s_strTooltip.Add("   %s\n",g_aBasicItem[iBasicItem].m_strName);
				}
				break;
			}

			case	eGSE_DECREASE_DUNGEON_MAGIC_ATTR_PENELTY_PER_LEVEL	:	//{	"던젼속성페널티저하per레벨"						:	
				iReturnValue	=	g_guild.operateGSDecreaseDungeonMagicPeneltyPerLevel(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d%s<n>\n",strTextColor,dMSG_DECREASE_DUNGEON_MAGIC_ATTR_PENELTY,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;

			case	eGSE_DECREASE_DEATH_PENELTY_PER_LEVEL				:	//{	"사망페널티감소per레벨"					:	
				iReturnValue	=	g_guild.operateGSDecreaseDeathPeneltyPerLevel(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d%s<n>\n",strTextColor,dMSG_DECREASE_DEATH_PENELTY,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;

//	추가된거
			case	eGSE_INCREASE_DAMAGE_UNDER_SIEGE_WARFARE	:	//{	"공성전시대미지감소
				iReturnValue	=	g_guild.operateGSIncreaseDamageUnderSiegeWarfare(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d%s<n>\n",strTextColor,dMSG_INCREASE_DAMAGE_UNDER_SIEGE_WARFARE,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;
			case	eGSE_REDUCE_DAMAGE_UNDER_SIEGE_WARFARE	:	//{	"공성전시대미지감소
				iReturnValue	=	g_guild.operateGSReduceDamageUnderSiegeWarfare(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d%s<n>\n",strTextColor,dMSG_REDUCE_DAMAGE_UNDER_SIEGE_WARFARE,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;
			case	eGSE_INCREASE_GUILD_PET_SIGHT			:
				iReturnValue	=	g_guild.operateGSIncreaseGuildPetSight(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d<n>\n",strTextColor,dMSG_INCREASE_GUILD_PET_SIGHT,strValueColor,iReturnValue);
				break;
			case	eGSE_INCREASE_GUILD_CREST_EFFICIENT		:	//	"길드문장효율증가",
				iReturnValue	=	g_guild.operateGSIncreaseGuildCrestEfficient(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d%s<n>\n",strTextColor,dMSG_INCREASE_GUILD_CREST_EFFICIENT,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;
			case	eGSE_INCREASE_GUILD_FLAG_EFFICIENT		:	//	"길드깃발효율증가",
				iReturnValue	=	g_guild.operateGSIncreaseGuildFlagEfficient(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d%s<n>\n",strTextColor,dMSG_INCREASE_GUILD_FLAG_EFFICIENT,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;
			case	eGSE_INCREASE_ASTRO_BOW_DAMAGE			:	//	"아스트로보우대미지증가",
				iReturnValue	=	g_guild.operateGSIncreaseAstroBowDamage(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d%s<n>\n",strTextColor,dMSG_INCREASE_ASTRO_BOW_DAMAGE,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;
			case	eGSE_INCREASE_GUILD_DOOR_EFFICIENT		:	//	"길드문효율증가",
				iReturnValue	=	g_guild.operateGSIncreaseGuildDoorEfficient(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d%s<n>\n",strTextColor,dMSG_INCREASE_GUILD_DOOR_EFFICIENT,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;
			case	eGSE_INCREASE_GUILD_PET_ATTACK_SPEED	:	//	"길드펫액션속도증가",
				iReturnValue	=	g_guild.operateGSIncreaseGuildPetAttackSpeed(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+0.%d%s<n>\n",strTextColor,dMSG_INCREASE_GUILD_PET_ATTACK_SPEED,strValueColor,iReturnValue,dMSG_SECOND);
				break;
			case	eGSE_DECREASE_GUILD_SHOP_ITEM_PRICE		:	//	"길드상점아이템가격감소",
				iReturnValue	=	g_guild.operateGSDecreaseGuildShopItemPrice(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d%s<n>\n",strTextColor,dMSG_DECREASE_GUILD_SHOP_ITEM_PRICE,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;
			case	eGSE_INCREASE_DASH_BLADE_DAMAGE			:	//	"대쉬블레이드대미지증가",
				iReturnValue	=	g_guild.operateGSIncreaseDashBladeDamage(_iSkill,iRealLevel,lpEffectInfo,TRUE);
				s_strTooltip.Add("%s- %s %s+%d%s<n>\n",strTextColor,dMSG_INCREASE_DASH_BLADE_DAMAGE,strValueColor,iReturnValue,dMSG_PERCENTAGE);
				break;
//	추가된거

		}
	}

	s_strTooltip.Add("\n");

	if	(iLevel	<	getSkillTopLevel(_iSkill) || lpSkill->m_wTopLevel == 0)
	{
		if	(lpSkill->m_bf1IsRequireSkillMasteryTitle || lpSkill->m_bf3RequireGuildHallLevel || lpSkill->m_awRequireSkill[0] != 0xffff || lpSkill->m_iRequireGold)
		{
			s_strTooltip.Add("<c:LTCYAN><%s><n>\n",dMSG_REQUIRE_INFO_FOR_INCREASE_GUILD_SKILL);

			if	(lpSkill->m_bf1IsRequireSkillMasteryTitle)
				s_strTooltip.Add("- <c:LTYELLOW>%s<n>\n",dMSG_REQUIRE_SKILL_MASTERY_TITLE);

			for (i=0;i<dGUILD_SKILL_REQUIRE_SKILL_COUNT;i++)
			{
				CGuildSkill	*lpRequireSkill	=	g_gsManager.get(lpSkill->m_awRequireSkill[i]);

				if	(lpRequireSkill	==	NULL)
					break;

				s_strTooltip.Add("- %s <c:LTYELLOW>%s<n>(<c:LTYELLOW>Lv %d<n>)\n",dMSG_REQUIRE_SKILL,lpRequireSkill->m_strName,lpSkill->m_awRequireSkillLevel[i]);
			}

			if	(lpSkill->m_iRequireGold)
				s_strTooltip.Add("- %s <c:LTYELLOW>%s<n> Gold\n",dMSG_REQUIRE_GOLD,CGamePlay::GetMoneyString(lpSkill->m_iRequireGold));
			if	(lpSkill->m_bf3RequireGuildHallLevel)
				s_strTooltip.Add("- %s <c:LTYELLOW>%d<n>\n",dMSG_GUILD_HALL_LEVEL,lpSkill->m_bf3RequireGuildHallLevel);
			s_strTooltip.Add("\n");
		}
	}

	s_strTooltip.Add("<c:LTCYAN><%s><n>\n",dMSG_COMMENT);
	s_strTooltip.Add("- %s\n\n",lpSkill->m_strComment);

	return	s_strTooltip.String;
}

//
//	내일 길드전이 예약되어 있다.
BOOL
CGuild::isBookedBattle()
{
	if	(m_battleInfo.m_bBSW_Hour	==	0xff)
		return	FALSE;

	return	TRUE;
}

BOOL
CGuild::isBookedIGB()
{
	if	(m_battleInfo.m_wIGB_FieldSerial	!=	0xffff)
		return	TRUE;

	return	FALSE;
}

//
//	내일 길드전이 정해져 있다.
BOOL
CGuild::isEngagedIGB()
{
	if	(m_battleInfo.m_wIGB_FieldSerial			==	0xffff	)
		return	FALSE;
	if	(m_battleInfo.m_strIGB_TargetGuildName[0]	==	0		)
		return	FALSE;

	return	TRUE;
}

void
CGuild::receiveInventoryInfo(cGuildInventoryItemForPacket	*_lpItemList,int _iCount)
{
	for	(int i=c_iGuildInventorySize;i;)
	{
		--i;

		m_aInventory[i].reset();
	}

	for	(int iSlot=_iCount;iSlot;)
	{
		--iSlot;

		cGuildInventoryItemForPacket	*lpItem	=	&_lpItemList[iSlot];

		memcpy(&m_aInventory[lpItem->m_wSlot],lpItem,sizeof(cItem));
	}
}

CSimpleGuildMemberInfo*
CGuild::getMember(int _iMember)
{
	if	(_iMember<0 || _iMember >= m_memberList.m_wCount)
		return	NULL;

	return	&m_memberList.m_aMembers[_iMember];
}

CSimpleGuildMemberInfo*
CGuild::getMember(char *_lpstrName)
{
	int	iMemberIndex	=	getMemberIndex(_lpstrName);

	if	(iMemberIndex	==	0xffff)
		return	NULL;

	return	&m_memberList.m_aMembers[iMemberIndex];
}

int
CGuild::getMemberIndex(char *_lpstrName)
{
	for (int i=0;i<m_memberList.m_wCount;i++)
	{
		if (STRICMP(m_memberList.m_aMembers[i].getName(),_lpstrName) == 0)
			return	i;
	}

	return	0xffff;
}

void
CGuild::sortMemberList(int _iMethod)
{
	CSimpleGuildMemberList		memberList;
	int							i;

	if (_iMethod	==	-1)
		_iMethod	=	m_wMemberSortMethod;

	m_wMemberSortMethod	=	_iMethod;

	memcpy(&memberList,&m_memberList,sizeof(memberList));
	
	if (m_wMemberSortMethod	==	eGMLSM_BY_NAME)
	{
		cSTRINGS	strings;

		for (i=0;i<m_memberList.m_wCount;i++)
			strings.Add(m_memberList.m_aMembers[i].getName(),i);

		strings.Sort();

		cSTRING	*lpName	=	strings.Pick(0);

		for (i=0;i<m_memberList.m_wCount;i++)
		{
			memcpy(&m_memberList.m_aMembers[i],&memberList.m_aMembers[lpName->Value],sizeof(CSimpleGuildMemberInfo));
			lpName	=	lpName->pNEXT;
		}

		return;
	}

	{
		cINDEX	aSortData[dGUILD_MAX_PLAYER];

		if (_iMethod	==	eGMLSM_BY_RANK)
			for (i=0;i<m_memberList.m_wCount;i++)
			{
				aSortData[i].Index	=	i;
				aSortData[i].Value	=	m_memberList.m_aMembers[i].m_bf4Rank;
			}

		if (_iMethod	==	eGMLSM_BY_LEVEL)
			for (i=0;i<m_memberList.m_wCount;i++)
			{
				aSortData[i].Index	=	i;
				aSortData[i].Value	=	m_memberList.m_aMembers[i].m_bf11Level;
			}

		if (_iMethod	==	eGMLSM_BY_JOB)
			for (i=0;i<m_memberList.m_wCount;i++)
			{
				aSortData[i].Index	=	i;
				aSortData[i].Value	=	m_memberList.m_aMembers[i].m_bf8Job;
			}

		qsort((void *)aSortData,m_memberList.m_wCount,sizeof(cINDEX),(int(*)(const void *,const void *))Compare);

		if (_iMethod	!=	eGMLSM_BY_JOB)
			for (i=0;i<m_memberList.m_wCount;i++)
				memcpy(&m_memberList.m_aMembers[m_memberList.m_wCount-i-1],&memberList.m_aMembers[aSortData[i].Index],sizeof(CSimpleGuildMemberInfo));
		else
			for (i=0;i<m_memberList.m_wCount;i++)
				memcpy(&m_memberList.m_aMembers[i],&memberList.m_aMembers[aSortData[i].Index],sizeof(CSimpleGuildMemberInfo));
	}
}

void
CGuild::updateMember(char *_lpstrName,int _iLevel,int _iRank,int _iJob,BOOL _bIsConnected,int _iReserverPet)
{
	if	(m_wIsNotReceivedMemberList)
		return;

	int	iSlot	=	getMemberIndex(_lpstrName);

	if	(iSlot	==	0xffff)
		iSlot	=	m_memberList.m_wCount;

	CSimpleGuildMemberInfo	*lpMember	=	&m_memberList.m_aMembers[iSlot];

	lpMember->m_bf11Level		=	_iLevel;
	lpMember->m_bf4Rank			=	_iRank;
	lpMember->m_bf1IsConnected	=	_bIsConnected;
	lpMember->m_bf8Job			=	_iJob;
	lpMember->m_bf4ReservePet	=	_iReserverPet;

	lpMember->setName(_lpstrName);

	if	(_iRank	==	dGUILD_CLASS_MASTER		)	
		strcpy(m_strMasterName,_lpstrName);
	
	sortMemberList();

	g_gwGuild.changeTab();
}

void
CGuild::removeMember(char *_lpstrName)
{
	if (m_wIsNotReceivedMemberList)
		return;

	int	iSlot	=	getMemberIndex(_lpstrName);

	if (iSlot	==	0xffff)
		return;

	CSimpleGuildMemberList		memberList;

	memcpy(&memberList,&m_memberList,sizeof(memberList));

	if (iSlot	>	0)
		memcpy(&m_memberList.m_aMembers,&m_memberList.m_aMembers,sizeof(CSimpleGuildMemberInfo)*iSlot);

	if (iSlot	<	dGUILD_MAX_PLAYER-1)
		memcpy(&m_memberList.m_aMembers[iSlot],&m_memberList.m_aMembers[iSlot+1],sizeof(CSimpleGuildMemberInfo)*(dGUILD_MAX_PLAYER-iSlot-1));

	sortMemberList();

	g_gwGuild.changeTab();
}

int
CGuild::getCheckSum()
{
	int	iCheckSum	=	0,i;

//	iCheckSum	+=	m_wGuildInventorySize;										//	길드 인벤토리 사이즈
	iCheckSum	+=	m_wLevel;								//	길드 레벨
//	iCheckSum	+=	m_dwExp;								//	길드 경험치
	iCheckSum	+=	m_iVictoryPoint;						//	길드 승점
	iCheckSum	+=	m_wVictoryContinue;
	iCheckSum	+=	m_wVictoryCount;
	iCheckSum	+=	m_wDefeatCount;
	iCheckSum	+=	m_wDrawCount;
	iCheckSum	+=	m_wCancelCount;

	iCheckSum	+=	(int)m_llGuildGold;								//	길드 소유 금액
	iCheckSum	+=	m_wHallLevel;							//	길드홀 레벨
//	iCheckSum	+=	m_wMaxMember;
	iCheckSum	+=	m_wMemberCount;			//	최대 길드원 수,현재 길드원 수
//	iCheckSum	+=	m_wMaxSubMaster;
	iCheckSum	+=	m_wSubMasterCount;		//	최대 서브 길마 수,현재 서브 길마 수
//	iCheckSum	+=	m_wMaxCongressMan;
	iCheckSum	+=	m_wCongressManCount;	//	최대 길드 의원 수,현재 서길드 의원 수
	iCheckSum	+=	m_wExpTaxRate;			//	길드 경험치를 걷을것인가? 길드 경험치 세율-_-
	iCheckSum	+=	m_wGuildPoint;
	iCheckSum	+=	m_bf6BonusGuildPoint;
	iCheckSum	+=	m_bf6PeneltyBonusGuildPoint;

	for (i=0;i<dMAX_GUILD_SKILL_COUNT;i++)
		iCheckSum	+=	m_abGuildSkill[i];

	iCheckSum	+=	GetHashCode(m_strName);			//	길드 이름

	return	iCheckSum;
}

int
CGuild::getRentedPetCount(int _iPetType)
{
	int	iPetCount=	0;

	for (int i=0;i<m_wMemberCount;i++)
	{
		if	(m_memberList.m_aMembers[i].m_bf4ReservePet	==	_iPetType)
			iPetCount++;
	}

	return	iPetCount;
}

int
CGuild::getPetLevelByExp(UINT	_uiExp)
{
	for(int iLevel=0;iLevel<100;iLevel++)
	{
		if	(_uiExp	>=	l_auiGuildPetExp[iLevel]	&&	_uiExp	<	l_auiGuildPetExp[iLevel+1])
			return	iLevel+1;
	}

	return	100;
}

int
CGuild::getPetLevel(int _iPetType)
{
	return	getPetLevelByExp(getPetExp(_iPetType));
}

int
CGuild::getGuardianLevel(int _iGuardianType)
{
	return	getPetLevelByExp(getGuardianExp(_iGuardianType));
}

void
CGuild::feedItemToGuildPet(int _iPetType,int _iItemSlot,int _iFeedExp,int _iExp,int _iExpDuringWeek)
{
	g_hero.m_aItems[_iItemSlot].reset();

	if	(_iPetType	>=	dGUILD_PET_TYPE_COUNT)
	{
		_iPetType	-=	dGUILD_PET_TYPE_COUNT;

		int	iOldLevel=	getGuardianLevel(_iPetType+1);

		setGuardianExp(_iPetType+1,_iExp,_iExpDuringWeek);

		CGamePlay::AddSystemMessage(WHITE,dMSG_INCREASE_GUILD_GUARDIAN_EXP_BY_FEED_ITEM_FORM,g_lpstrGuildGuardianName[_iPetType+1],_iFeedExp);

		int	iLevel	=	getGuardianLevel(_iPetType+1);

		if	(iLevel	>	iOldLevel)
			CGamePlay::AddSystemMessage(WHITE,dMSG_INCREASE_GUILD_GUARDIAN_LEVEL_FORM,g_lpstrGuildGuardianName[_iPetType+1],iLevel);
	}
	else
	{
		int	iOldLevel=	getPetLevel(_iPetType+1);

		setPetExp(_iPetType+1,_iExp,_iExpDuringWeek);

		CGamePlay::AddSystemMessage(WHITE,dMSG_INCREASE_GUILD_PET_EXP_BY_FEED_ITEM_FORM,g_lpstrGuildPetName[_iPetType+1],_iFeedExp);

		int	iLevel	=	getPetLevel(_iPetType+1);

		if	(iLevel	>	iOldLevel)
			CGamePlay::AddSystemMessage(WHITE,dMSG_INCREASE_GUILD_PET_LEVEL_FORM,g_lpstrGuildPetName[_iPetType+1],iLevel);
	}
}

int
CGuild::getPetRemainExpForLevelUp(int _iPetType)
{
	int	iCurLevel	=	getPetLevel(_iPetType);

	if	(iCurLevel	==	100)
		return	0;

	return	l_auiGuildPetExp[iCurLevel]	-	getPetExp(_iPetType);
}

int
CGuild::getGuardianRemainExpForLevelUp(int _iGuardianType)
{
	int	iCurLevel	=	getGuardianLevel(_iGuardianType);

	if	(iCurLevel	==	100)
		return	0;

	return	l_auiGuildPetExp[iCurLevel]	-	getGuardianExp(_iGuardianType);
}

char*
CGuild::getGuildGuardianToolTip(int _iType)
{
	int		iLevel	=	getGuardianLevel(_iType);
	static	char	strText[512]	=	"";

	switch(_iType)
	{
		case	eGUILD_GUARDIAN_SHIELD_CRAFT	:
		{
			int	iRange		=	100+2*iLevel;
			int	iUpkeepTime	=	500;
			int	iPeriod		=	1500-5*iLevel;

			if	(iLevel	>=	100)
				iUpkeepTime	+=	1000;
			else
			if	(iLevel	>=	80)
				iUpkeepTime	+=	600;
			else
			if	(iLevel	>=	60)
				iUpkeepTime	+=	300;
			else
			if	(iLevel	>=	30)
				iUpkeepTime	+=	100;

			sprintf(strText,dMSG_TOOLTIP_FOR_SHIELD_CRAFT_FORM,
				iRange,iUpkeepTime/100,iUpkeepTime%100,iPeriod/100,iPeriod%100);
//	"- 효과 범위 <c:LTGREEN>%d Pixel<n>\n- 아군을 <c:LTGREEN>%d.%.2d초<n>간 물리 대미지 면역 상태로 만든다.\n- 발동 주기 <c:LTGREEN>%d.%2d초<n>"
			break;
		}
		case	eGUILD_GUARDIAN_ELEMENTAL_POST	:
		{
			int	iRange		=	100+2*iLevel;
			int	iUpkeepTime	=	500;
			int	iPeriod		=	1500-5*iLevel;

			if	(iLevel	>=	100)
				iUpkeepTime	+=	1000;
			else
			if	(iLevel	>=	80)
				iUpkeepTime	+=	600;
			else
			if	(iLevel	>=	60)
				iUpkeepTime	+=	300;
			else
			if	(iLevel	>=	30)
				iUpkeepTime	+=	100;
//	"- 효과 범위 <c:LTGREEN>%d Pixel<n>\n- 아군을 <c:LTGREEN>%d.%.2d초<n>간 마법 대미지 면역 상태로 만든다.\n- 발동 주기 <c:LTGREEN>%d.%2d초<n>"

			sprintf(strText,dMSG_TOOLTIP_FOR_ELEMENTAL_POST_FORM,
				iRange,iUpkeepTime/100,iUpkeepTime%100,iPeriod/100,iPeriod%100);
			break;
		}
		case	eGUILD_GUARDIAN_ETHER_SHELL		:
		{
//	"- 효과 범위 <c:LTGREEN>%d Pixel<n>\n- 아군을 <c:LTGREEN>%d.%.2d초<n>간 상태 이상 공격 면역 상태로 만든다.\n- 발동 주기 <c:LTGREEN>%d.%2d초<n>"
			int	iRange		=	100+2*iLevel;
			int	iUpkeepTime	=	500;
			int	iPeriod		=	1500-5*iLevel;

			if	(iLevel	>=	100)
				iUpkeepTime	+=	1000;
			else
			if	(iLevel	>=	80)
				iUpkeepTime	+=	600;
			else
			if	(iLevel	>=	60)
				iUpkeepTime	+=	300;
			else
			if	(iLevel	>=	30)
				iUpkeepTime	+=	100;
//	- 사정거리 안에 아군을 상태 이상 공격
			sprintf(strText,dMSG_TOOLTIP_FOR_ETHER_SHELL_FORM,
				iRange,iUpkeepTime/100,iUpkeepTime%100,iPeriod/100,iPeriod%100);
			break;
		}
		case	eGUILD_GUARDIAN_HOLLOW_KNIGHT	:
		{
			int	iMinDamage	=	(20+18*iLevel-2*iLevel)*100;
			int	iMaxDamage	=	(20+18*iLevel+2*iLevel)*100;
			int	iHitChance	=	5000;
			int	iAttackSpeed=	200;
			int	iMoveSpeed	=	70;

			if	(iLevel	>=	100)
				iAttackSpeed	-=	(iAttackSpeed*25)/100;

			if	(iLevel	>=	80)
				iMoveSpeed	+=	75;
			else
			if	(iLevel	>=	60)
				iMoveSpeed	+=	25;

			if	(iLevel	>=	30)
				iHitChance	+=	1000;

//	명중률 +10%	이동 속도+25	이동 속도+75	공격 속도 +25%
//	"- 시야 안에 적을 따라다니며 공격한다.\n- 공격력 <c:LTYELLOW>%d.%.2d~%d.%.2d<n>\n- 명중률 <c:LTGREEN>%d.%.2d％<n>\n- 공격 속도 <c:LTGREEN>%d.%.2d초<n>\n- 이동 속도 초당 <c:LTGREEN>%d Pixel<n>"

			sprintf(strText,dMSG_TOOLTIP_FOR_HOLLOW_KNIGHT_FORM,
				iMinDamage/100,iMinDamage%100,iMaxDamage/100,iMaxDamage%100,
				iHitChance/100,iHitChance%100,iAttackSpeed/100,iAttackSpeed%100,iMoveSpeed);
			break;
		}
	}

	return	strText;
}

char*
CGuild::getGuildPetToolTip(int _iType)
{
	int		iLevel	=	getPetLevel(_iType);
	static	char	strText[512]	=	"";

	switch(_iType)
	{
		case	eGUILD_PET_OBITIAN		:
		{
			int	iMinDamage			=	(5+12*iLevel-3*iLevel)*100;
			int	iMaxDamage			=	(5+12*iLevel+3*iLevel)*100;
			int	iRange				=	120;
			int	iAttackSpeed		=	400;

			if	(iLevel	>=	100)
				iRange	+=	64;
			else
			if	(iLevel	>=	80)
				iRange	+=	40;
			else
			if	(iLevel	>=	60)
				iRange	+=	25;
			else
			if	(iLevel	>=	30)
				iRange	+=	10;

			iAttackSpeed	-=	iAttackSpeed*m_wIncreaseGuildPetAttackSpeed/100;
			iRange			+=	iRange*m_wIncreaeGuildPetSight/100;

			sprintf(strText,dMSG_TOOLTIP_FOR_OBITIAN_FORM,iMinDamage/100,iMinDamage%100,iMaxDamage/100,iMaxDamage%100,iRange,iAttackSpeed/100,iAttackSpeed%100);
			break;
		}

		case	eGUILD_PET_DASH_BLADE	:
		{

//	"- 물리 대미지 <c:LTYELLOW>%d.%2d~%d.%2d<n>\n- 사정거리 <c:LTGREEN>%d Pixel<n>\n- 명중률 <c:LTGREEN>%d.%.2d％<n>\n- 공격 속도 <c:LTGREEN>%d.%2d초<n>"
			int	iMinDamage			=	(7+9*iLevel-1*iLevel)*100;
			int	iMaxDamage			=	(7+9*iLevel+1*iLevel)*100;

			iMinDamage				+=	iMinDamage*m_wIncreaseDashBladeDamage/100;
			iMaxDamage				+=	iMaxDamage*m_wIncreaseDashBladeDamage/100;

			int	iRange				=	150;
			int	iHitChance			=	7500;
			int	iAttackSpeed		=	200;
			int	iIgnoreTargetDefensivePowerChance	=	0;

			if	(iLevel	>=	100)
				iIgnoreTargetDefensivePowerChance	=	25;
			else
			if	(iLevel	>=	60)
				iIgnoreTargetDefensivePowerChance	=	10;

			if	(iLevel	>=	80)
				iHitChance	+=	1500;
			else
			if	(iLevel	>=	30)
				iHitChance	+=	500;

			iRange			+=	iRange*m_wIncreaeGuildPetSight/100;
			iAttackSpeed	-=	iAttackSpeed*m_wIncreaseGuildPetAttackSpeed/100;

			sprintf(strText,dMSG_TOOLTIP_FOR_DASH_BLADE_FORM,iMinDamage/100,iMinDamage%100,iMaxDamage/100,iMaxDamage%100,iRange,
															iHitChance/100,iHitChance%100,iAttackSpeed/100,iAttackSpeed%100,iIgnoreTargetDefensivePowerChance);
			break;
		}
		case	eGUILD_PET_ASTRO_BOW	:
		{
//	"- 물리 대미지 <c:LTYELLOW>%d.%2d~%d.%2d<n>\n- 사정거리 <c:LTGREEN>%d Pixel<n>\n- 명중률 <c:LTGREEN>%d.%.2d％<n>\n- 공격 속도 <c:LTGREEN>%d.%2d초<n>"
			int	iMinDamage			=	(3+7*iLevel-3*iLevel)*100;
			int	iMaxDamage			=	(3+7*iLevel+3*iLevel)*100;

			iMinDamage				+=	iMinDamage*m_wIncreaseAstroBowDamage/100;
			iMaxDamage				+=	iMaxDamage*m_wIncreaseAstroBowDamage/100;

			int	iRange				=	300;
			int	iHitChance			=	7500;
			int	iAttackSpeed		=	300;

			if	(iLevel	>=	100)
				iHitChance	+=	1500;
			else
			if	(iLevel	>=	60)
				iHitChance	+=	500;

			if	(iLevel	>=	80)
				iAttackSpeed	-=	(iAttackSpeed*15)/100;
			else
			if	(iLevel	>=	30)
				iAttackSpeed	-=	(iAttackSpeed*5)/100;

			iRange			+=	iRange*m_wIncreaeGuildPetSight/100;
			iAttackSpeed	-=	iAttackSpeed*m_wIncreaseGuildPetAttackSpeed/100;

			sprintf(strText,dMSG_TOOLTIP_FOR_ASTRO_BOW_FORM,iMinDamage/100,iMinDamage%100,iMaxDamage/100,iMaxDamage%100,iRange,
															iHitChance/100,iHitChance%100,iAttackSpeed/100,iAttackSpeed%100);
			break;
		}
		case	eGUILD_PET_CRISTAL_WATER:
		{
//	"- <c:LTGREEN>%d.%2d포인트<n>의 체력을 회복 시켜 준다.\n- <c:LTGREEN>%d％<n>의 확률로 <c:LTGREEN>저주계 상태이상<n> 치료\n- <c:LTGREEN>%d％<n>의 확률로 <c:LTGREEN>이상계 상태이상<n> 치료\n- <c:LTGREEN>%d％<n>의 확률로 <c:LTGREEN>능력치 저하계 상태이상<n> 치료\n- 공격 속도 <c:LTGREEN>%d.%2d초<n>"
			int	iHP						=	100+20*iLevel;
			int	iCureCurseChance		=	0;
			int	iCureStrangeStatusChance=	0;
			int	iCureDeclineStatusChance=	0;
			int	iCurePeriod				=	500;

			if	(iLevel	>=	100)
				iCurePeriod	-=	(iCurePeriod*10)/100;
			if	(iLevel	>=	80)
				iCureDeclineStatusChance	=	10;
			if	(iLevel	>=	60)
				iCureStrangeStatusChance	=	10;
			if	(iLevel	>=	30)
				iCureCurseChance			=	10;

			sprintf(strText,dMSG_TOOLTIP_FOR_CRISTAL_WATER_FORM,
							iHP/100,iHP%100,
							iCureCurseChance,iCureStrangeStatusChance,iCureDeclineStatusChance,
							iCurePeriod/100,iCurePeriod%100);
			break;
		}
	}

	return	strText;
}


void
CGuildManager::init()
{
	for (int i=0;i<dGUILD_MAX_COUNT;i++)
		m_aGuildMark[i].m_wSerial	=	0xffff;
}

void
CGuildManager::setGuildMark(int _iGuild,CGuildMarkInfo *_lpMark,BOOL _bIsTwinkle,int _iGuildLevel,int _iHallLevel)
{
	m_aGuildMark[_iGuild].m_wSerial			=	_iGuild;
	m_aGuildMark[_iGuild].m_bf1IsTwinkle	=	_bIsTwinkle;
	m_aGuildMark[_iGuild].m_bf3HallLevel	=	_iHallLevel;
	m_aGuildMark[_iGuild].m_bf8Level		=	_iGuildLevel;

	memcpy(&m_aGuildMark[_iGuild],_lpMark,sizeof(CGuildMarkInfo));
}

CGuildMarkInfo2*
CGuildManager::getGuildMark(int _iGuild)
{
	if	(m_aGuildMark[_iGuild].m_wSerial	==	0xffff)
		return	NULL;

	return	&m_aGuildMark[_iGuild];
}


