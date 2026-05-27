#include "cITEM_PREFIX.H"
#include "cJOB.H"
#include "cDECODER.H"

cITEM_PREFIX_MANAGER	g_itemPrefix;

BOOL					cITEM_PREFIX_MANAGER::s_isVirgin	=	TRUE;

cITEM_PREFIX::cTYPE		g_aItemPrefixType[dMAX_ITEM_PREFIX_TYPE_COUNT]	=
{
eIP_ADD_STRENGTH, "strength increase/decrease",
eIP_ADD_AGILITY, "Increase/decrease agility",
eIP_ADD_CONSTITUTION, "Health Increase",
eIP_ADD_WISDOM, "wisdom increase/decrease",
eIP_ADD_INTELIGENCE, "Increase or decrease knowledge",
eIP_ADD_CHARISMA, "Charisma increase/decrease",
eIP_ADD_LUCK, "Luck increase/decrease",
eIP_FIXING_STRENGTH, "Fixing strength",
eIP_FIXING_AGILITY, "Fixing agility",
eIP_FIXING_CONSTITUTION, "Fixing Health",
eIP_FIXING_WISDOM, "Wisdom fixing",
eIP_FIXING_INTELLIGENCE, "Knowledge Fixing",
eIP_FIXING_CHARISMA, "Fixing Charisma",
eIP_FIXING_LUCK, "Fixing luck",
eIP_ADD_STRENGTH_BY_LEVEL, "Strength increase/decrease proportional to level",
eIP_ADD_AGILITY_BY_LEVEL, "Increases or decreases agility proportional to level",
eIP_ADD_CONSTITUTION_BY_LEVEL, "Health increases or decreases in proportion to level",
eIP_ADD_WISDOM_BY_LEVEL, "Increase or decrease wisdom proportional to level",
eIP_ADD_INTELIGENCE_BY_LEVEL, "Knowledge increases or decreases in proportion to level",
eIP_ADD_CHARISMA_BY_LEVEL, "Charisma increases or decreases in proportion to level",
eIP_ADD_LUCK_BY_LEVEL, "Luck increases or decreases in proportion to level",
eIP_ADD_DAMAGE_PERCENTAGE, "Weapon damage increase/decrease (percentage)",
eIP_ADD_MIN_DAMAGE, "Weapon damage increase/decrease (minimum value)",
eIP_ADD_MAX_DAMAGE, "Weapon damage increase/decrease (maximum value)",
eIP_ADD_DEFENSE_PERCENTAGE, "Defense increase/decrease (percentage)",
eIP_ADD_DEFENSE, "Defense increase/decrease (value)",
eIP_ADD_MAX_HP_PERCENTAGE, "Max HP increase/decrease (percentage)",
eIP_ADD_MAX_HP, "Maximum health increase/decrease (value)",
eIP_ADD_MAX_CP_PERCENTAGE, "Maximum CP increase/decrease (percentage)",
eIP_ADD_MAX_CP, "Maximum CP increase/decrease (numerical value)",
eIP_LOW_CP, "CP reduction limit when moving",
eIP_CORRECT_HIT_RATE, "Attack success probability (hit rate)",
eIP_CORRECT_DODGE_RATE, "Probability to avoid attack (evasion rate)",
eIP_SHIELD_BLOCK_RATE, "Shield activation probability",
eIP_CRITICAL_HIT, "Critical hit chance",
eIP_CRUSH_SHOT, "Probability of triggering a final blow",
eIP_IGNORE_TARGET_DODGE_ABILITY, "Ignore enemy defender's evasion modifier",
eIP_IGNORE_ATTACKER_CORRECT_HIT_VALUE, "Ignore enemy attacker's hit modifier",
eIP_PERFECT_HIT, "Always hit",
eIP_BLOCKING_SPEED, "Blocking speed",
eIP_MOVE_SPEED, "movement speed",
eIP_ACTION_SPEED, "Attack Speed",
eIP_POTION_HEAL_SPEED, "Adjust potion recovery speed [+50]",
eIP_ADD_CONCENTRATION , "Control CP reduction rate (Concentration)",
eIP_FIRE_DAMAGE, "Fire attack",
eIP_WATER_DAMAGE, "Water attribute attack",
eIP_WIND_DAMAGE, "Wind attribute attack",
eIP_TERRA_DAMAGE, "Earth attribute attack",
eIP_LIGHT_DAMAGE, "Light attribute attack",
eIP_DARK_DAMAGE, "Dark attribute attack",
eIP_STEAL_HP, "Health Steal",
eIP_BONUS_CP, "Bonus CP",
eIP_FLEE, "flee",
eIP_KNOCK_OUT, "knockout",
eIP_INSTANT_KILL, "Instant death",
eIP_WEAPON_BREAKER, "Weapon Break",
eIP_ARMOR_BREAKER, "Armor Break",
eIP_MAKE_COLD, "cold state",
eIP_MAKE_FREEZE, "freeze state",
eIP_MAKE_BLIND, "blind state",
eIP_MAKE_STUN, "Stun state",
eIP_MAKE_STONE, "petrified state",
eIP_MAKE_CONFUSE, "Confused",
eIP_MAKE_SLEEP, "Sleep state",
eIP_MAKE_CHARMING, "Charming state",
eIP_MAKE_FIRST_TARGET, "First target state",
eIP_MAKE_AI_DOWN , "AI downgraded state",
eIP_MAKE_BERSERK, "Berserk state",
eIP_RESISTANCE_FIRE, "Resistance to fire attribute attacks",
eIP_RESISTANCE_WATER, "Water property attack resistance",
eIP_RESISTANCE_WIND, "Wind attribute attack resistance",
eIP_RESISTANCE_TERRA, "Resistance to earth attribute attacks",
eIP_RESISTANCE_LIGHT, "Light attribute attack resistance",
eIP_RESISTANCE_DARK, "Dark attribute attack resistance",
eIP_RESISTANCE_MAGIC, "Resistance to all magical attacks",
eIP_ABSORB_FIRE, "Absorb fire attribute attacks",
eIP_ABSORB_WATER, "Absorb water attack",
eIP_ABSORB_WIND, "Absorb wind attribute attacks",
eIP_ABSORB_TERRA, "Absorbs earth attribute attacks",
eIP_ABSORB_LIGHT, "Absorb light attribute attacks",
eIP_ABSORB_DARK, "Absorb dark attribute attacks",
eIP_ABSORB_MAGIC, "Absorbs all magical attacks",
eIP_RESISTANCE_FREEZE, "Resistance to freeze attacks",
eIP_RESISTANCE_COLD, "Resistance to cold attacks",
eIP_RESISTANCE_STUN, "Resistance to stun attacks",
eIP_RESISTANCE_CONFUSE, "Resistance to confusion attacks",
eIP_RESISTANCE_CHARMING, "Resistance to charming attacks",
eIP_RESISTANCE_STONE, "Resistance to petrification attacks",
eIP_RESISTANCE_INSTANCE_KILL, "Resistance to instant kill attacks",
eIP_RESISTANCE_POISON, "Resistance to poisoning attacks",
eIP_RESISTANCE_SLEEP, "Resistance to sleep attacks",
eIP_RESISTANCE_STRANGE_STATUS, "Resistance to status attacks",
eIP_RESISTANCE_DECLINE_STATUS, "Resistance to lower-world attacks",
eIP_RESISTANCE_CURSE_STATUS, "Resistance to underworld attacks",
eIP_RESISTANCE_ALL_BAD_STATUS, "Resistance to all abnormal attacks",
eIP_RESISTANCE_KNOCK_BACK, "Resistance to knockdown/tumble attacks",
eIP_RESISTANCE_CRITICAL_SHOT, "Resistance to critical hits",
eIP_RESISTANCE_CRUSH_SHOT, "Resistance to final blow attacks",
eIP_RETURN_DAMAGE, "Returns some of the damage received",
eIP_DAMAGE_TO_CP, "Convert received damage CP",
eIP_MAKE_FREEZE_ATTACKER, "Attacker Freeze",
eIP_MAKE_COLD_ATTACKER, "attacker cold",
eIP_CHARGE_SKILL, "Skill/Magic Charge",
eIP_JOB_SKILL_LEVEL, "Series skill/magic level adjustment",
eIP_ALL_SKILL_LEVEL, "Adjust all skill/magic levels",
eIP_FIRST_AID, "Strike First Aid",
eIP_CHANGE_WEAPON_SPEED, "Weapon exchange speed",
eIP_REGENERATION_HP, "Health Regeneration",
eIP_RESURRECTION_CHANCE, "Resurrection Chance",
eIP_BLURRED, "Blurred",
eIP_INVISIBLE, "Invisible",
eIP_LEVITATE, "Levitation",
eIP_MAGIC_BULLET, "Magic Bullet",
eIP_INFINITY_BULLET, "Infinite Bullets",
eIP_NOT_AIMED, "Excluded from target",
eIP_INCREASE_RARE_ITEM_DROP_CHANCE, "Increase Magic Item Chance",
eIP_INCREASE_UNIQUE_ITEM_DROP_CHANCE, "Increase chance of unique item",
eIP_DECREASE_RELOAD_TIME, "Reduce belt refill time",
eIP_AUTO_RELOAD, "Belt auto fill",
eIP_DISABLE, "disabled",
eIP_CAN_NOT_STRIP, "Cannot be taken off after wearing",
eIP_CAN_NOT_ADD_PREFIX, "Additional enchantments not possible",
eIP_UPGRADE_FIRE_DAMAGE_ATTACK, "Increases fire attack by 1 level",
eIP_UPGRADE_WATER_DAMAGE_ATTACK, "Water attack increases by 1 level",
eIP_UPGRADE_WIND_DAMAGE_ATTACK, "Wind attack increases by 1 level",
eIP_UPGRADE_EARTH_DAMAGE_ATTACK, "Earth attack increases by 1 level",
eIP_UPGRADE_LIGHT_DAMAGE_ATTACK, "Light attack increases by 1 level",
eIP_UPGRADE_DARK_DAMAGE_ATTACK, "Dark attack increases by 1 level",

eIP_CONTROL_SKILL_LEVEL_BY_DIFFICULTY, "Adjust skill/magic level by level",
eIP_INCREASE_SUMMON_BEAST_STATUS, "Increase summons stats",
eIP_INCREASE_GAIN_EXP_FOR_PET, "Increase pet experience gain",
eIP_IS_SHOP_SPECIAL_ITEM, "Use period",

eIP_WEAKNING_TARGET_FIRE_RESISTANCE, "Weaken target fire resistance",
eIP_WEAKNING_TARGET_WATER_RESISTANCE, "Weaken target water resistance",
eIP_WEAKNING_TARGET_WIND_RESISTANCE, "Weaken target wind resistance",
eIP_WEAKNING_TARGET_EARTH_RESISTANCE, "Weaken target ground resistance",
eIP_WEAKNING_TARGET_LIGHT_RESISTANCE, "Weaken target light resistance",
eIP_WEAKNING_TARGET_DARK_RESISTANCE, "Weaken target darkness resistance",
eIP_WEAKNING_TARGET_MAGIC_RESISTANCE, "Weaken target magic resistance",
eIP_STRENGTHENING_FIRE_DAMAGE, "Strengthens fire attribute attack power",
eIP_STRENGTHENING_WATER_DAMAGE, "Strengthens water attribute attack power",
eIP_STRENGTHENING_WIND_DAMAGE, "Strengthens wind attribute attack power",
eIP_STRENGTHENING_EARTH_DAMAGE, "Enhanced earth attribute attack power",
eIP_STRENGTHENING_LIGHT_DAMAGE, "Strengthens light attribute attack power",
eIP_STRENGTHENING_DARK_DAMAGE, "Strengthens dark attribute attack power",
eIP_STRENGTHENING_MAGIC_DAMAGE, "Strengthens magic attack power",

eIP_DECREASE_STRENGTH, "Reduce strength",
eIP_DECREASE_AGILITY, "Decreased agility",
eIP_DECREASE_CONSTITUTION, "Decreased health",
eIP_DECREASE_WISDOM, "Decrease wisdom",
eIP_DECREASE_INTELIGENCE, "Decrease knowledge",
eIP_DECREASE_CHARISMA, "Decrease charisma",
eIP_DECREASE_LUCK, "Decrease luck",

eIP_DECLINE_MOVE_SPEED, "Reduce movement speed",
eIP_DECLINE_ACTION_SPEED, "Reduce attack speed",

eIP_ADD_DAMAGE_TO_UNDEAD, "Undead Hunter",
eIP_ADD_DAMAGE_TO_DEMON, "Demon Hunter",
eIP_ADD_DAMAGE_TO_HUMAN, "Human Hunter",
eIP_ADD_DAMAGE_TO_ANIMAL, "Animal Hunter",
eIP_ADD_DAMAGE_TO_HOLY_ANIMAL, "Holy Beast Hunter",
eIP_POTION_HEAL_SPEED2, "Adjust potion recovery speed",

eIP_DECLINE_DEFENSE_PERCENTAGE, "Reduce defense percentage",
eIP_DECLINE_DEFENSE, "Decreased defense",
eIP_DECLINE_HIT_RATE, "Decreased hit rate",
eIP_DECLINE_DODGE_RATE, "Reduce evasion rate",
// VV 09.08.07 Magic Chudem DX+_+
eIP_FIRE_DAMAGE_DX, "Fire attribute attack (DX)",
eIP_WATER_DAMAGE_DX, "Water attribute attack (DX)",
eIP_WIND_DAMAGE_DX, "Wind attribute attack (DX)",
eIP_TERRA_DAMAGE_DX, "Earth attribute attack (DX)",
eIP_LIGHT_DAMAGE_DX, "Light attribute attack (DX)",
eIP_DARK_DAMAGE_DX, "Dark attribute attack (DX)",
// AA 09.08.07 Magic Chudem DX+_+
eIP_BUFF_SKILL_CASTING, "Use buff skill", // Buff skill casting ~ 09.10.12
eIP_INCREASE_ALL_STATUS, "Increase all stats", // Increase all stats.
eIP_ACL_BUFF_SKILL_CASTING, "Use imprint buff skill", // Cast imprint buff skill ~
eIP_INCREASE_HARD_BLOW, "Increased hard hit chance",
0xffff,
};

//
//	초기화
void
cITEM_PREFIX::reset()
{
	memset(this,0,sizeof(cITEM_PREFIX));
	strcpy(m_str1stPrefix,"");
	strcpy(m_str2stPrefix,"");
	m_wSerial				=	0xffff;
	m_aEnableSet[0]	=	1;
	m_aEnableSet[1]	=	1;
	m_aEnableSet[2]	=	1;

	memset(m_aStickableItem,0,sizeof(m_aStickableItem));
	m_bf1IsStickableItemScythe		= FALSE;
	m_bf1IsStickAbleItemArmWeapon	= FALSE;
	m_bf1IsStickAbleItemBook		= FALSE;
}

//
//	복사
void
cITEM_PREFIX::copy(cITEM_PREFIX *_lpPrefix)
{
	memcpy(this,_lpPrefix,sizeof(cITEM_PREFIX));
}

//
//	_lpValue에 이 접두사의 최대값들을 담아 준다.
void
cITEM_PREFIX::getMaxValue(short *_lpValues)
{
	_lpValues[0]	=	m_aValue[0][1];
	_lpValues[1]	=	m_aValue[1][1];
	_lpValues[2]	=	m_sValue3;
}

//
//	생성자
cITEM_PREFIX_MANAGER::cITEM_PREFIX_MANAGER()
{
	s_isVirgin	=	FALSE;
	
	init();
}
BOOL
cITEM_PREFIX::isSame(cITEM_PREFIX* _lpItemPrefix)
{
	if(m_wSerial != _lpItemPrefix->m_wSerial)
		return FALSE;

	if(m_wDiscernmentCode != _lpItemPrefix->m_wDiscernmentCode)
		return FALSE;

	if(m_wType != _lpItemPrefix->m_wType)
		return FALSE;

	if(memcmp(m_aValue, _lpItemPrefix->m_aValue, sizeof(short)*4) != 0)
		return FALSE;

	if(m_sValue3 != _lpItemPrefix->m_sValue3)
		return FALSE;

	if(memcmp(m_str1stPrefix,_lpItemPrefix->m_str1stPrefix,sizeof(char) * 20) != 0)
		return FALSE;

	if(memcmp(m_str2stPrefix,_lpItemPrefix->m_str2stPrefix,sizeof(char) * 20) != 0)
		return FALSE;

	if(m_wDropLevel != _lpItemPrefix->m_wDropLevel)
		return FALSE;

	if(m_wPrefixForm != _lpItemPrefix->m_wPrefixForm)
		return FALSE;

	if(m_dwPrice != _lpItemPrefix->m_dwPrice)
		return FALSE;

	if(m_wComputePriceMethod != _lpItemPrefix->m_wComputePriceMethod)
		return FALSE;

	if(m_wPriceFactor != _lpItemPrefix->m_wPriceFactor)
		return FALSE;

	if(memcmp(m_aEnableSet,_lpItemPrefix->m_aEnableSet,sizeof(BYTE)*4) != 0)
		return FALSE;

	if(memcmp(m_aStickableItem,_lpItemPrefix->m_aStickableItem,sizeof(BYTE) * dITEM_PREFIX_STICK_ABLE_ITEM_COUNT) != 0)
		return FALSE;

	if(m_bWeaponEffect != _lpItemPrefix->m_bWeaponEffect)
		return FALSE;

	if(m_bArmorEffect != _lpItemPrefix->m_bArmorEffect)
		return FALSE;

	if(m_wIsDXPrefix != _lpItemPrefix->m_wIsDXPrefix)
		return FALSE;

	if(m_wCorrectDropChance != _lpItemPrefix->m_wCorrectDropChance)
		return FALSE;

	if(memcmp(m_saCorrectQualityPrefix, _lpItemPrefix->m_saCorrectQualityPrefix, sizeof(short) * 2) != 0)
		return FALSE;

	if(m_wIsUltimatePrefix != _lpItemPrefix->m_wIsUltimatePrefix)
		return FALSE;

	if(m_bf1IsStickableItemScythe != _lpItemPrefix->m_bf1IsStickableItemScythe)
		return FALSE;

	if(m_bf1IsStickAbleItemArmWeapon != _lpItemPrefix->m_bf1IsStickAbleItemArmWeapon)
		return FALSE;

	if(m_bf1IsStickAbleItemBook != _lpItemPrefix->m_bf1IsStickAbleItemBook)
		return FALSE;

	return TRUE;
}

//
//	중복되는 데이터를 찾는다.
cITEM_PREFIX*
cITEM_PREFIX_MANAGER::findOverlappedData(char *_strPrefix1,char *_strPrefix2,WORD _wDiscernmentCode,int _iExcept)
{
	int	iIndex,iCount;

	for (iIndex=0,iCount=0;iIndex<dMAX_ITEM_PREFIX_COUNT && iCount < getCount();iIndex++)
	{
		cITEM_PREFIX	*lpPrefix	=	get(iIndex);

		if (!lpPrefix)	continue;

		iCount++;

		if (iIndex	==	_iExcept)	continue;

		if (stricmp(lpPrefix->m_str1stPrefix,_strPrefix1)==0)	return	lpPrefix;
		if (stricmp(lpPrefix->m_str1stPrefix,_strPrefix2)==0)	return	lpPrefix;
		if (stricmp(lpPrefix->m_str2stPrefix,_strPrefix1)==0)	return	lpPrefix;
		if (stricmp(lpPrefix->m_str2stPrefix,_strPrefix2)==0)	return	lpPrefix;

		if (lpPrefix->m_wDiscernmentCode	==	_wDiscernmentCode)	return	lpPrefix;
	}

	return	NULL;
}

//	컬러라이즈 효과들 모두 초기화
void
cITEM_PREFIX_MANAGER::resetColorizeEffect()
{
	int	iIndex,iCount;

	for (iIndex=0,iCount=0;iIndex<dMAX_ITEM_PREFIX_COUNT && iCount < getCount();iIndex++)
	{
		cITEM_PREFIX	*lpPrefix	=	get(iIndex);

		if (!lpPrefix)	continue;

		iCount++;

		lpPrefix->m_bArmorEffect	=	0xff;
		lpPrefix->m_bWeaponEffect	=	0xff;
	}
}

//
//	_strPrefix라는 이름을 가진 접두사의 기본 타입을 찾는다.
int
cITEM_PREFIX_MANAGER::getPrefixType(char *_strPrefix)
{
	int	iIndex=0;

	while(1)
	{
		if (g_aItemPrefixType[iIndex].m_wType	==	0xffff)
			break;

		if (stricmp(g_aItemPrefixType[iIndex].m_strName,_strPrefix)==0)
			return	g_aItemPrefixType[iIndex].m_wType;

		iIndex++;
	}

	return	0xffff;
}

cITEM_PREFIX*
cITEM_PREFIX_MANAGER::getPrefix(char *_strPrefix)
{
	int	iIndex,iCount;

	for (iIndex=0,iCount=0;iIndex<dMAX_ITEM_PREFIX_COUNT && iCount < getCount();iIndex++)
	{
		cITEM_PREFIX	*lpPrefix	=	get(iIndex);

		if (!lpPrefix)
			continue;

		iCount++;

		if (stricmp(lpPrefix->m_str1stPrefix,_strPrefix)==0)
			return	lpPrefix;
		if (stricmp(lpPrefix->m_str2stPrefix,_strPrefix)==0)
			return	lpPrefix;
	}

	return	NULL;
}


//
//식별 코드로 검색한다.
cITEM_PREFIX*
cITEM_PREFIX_MANAGER::getIndexByDiscernmentCode(int _iDiscernmentCode)
{
	int	iIndex,iCount;

	for (iIndex=0,iCount=0;iIndex<dMAX_ITEM_PREFIX_COUNT && iCount < getCount();iIndex++)
	{
		cITEM_PREFIX	*lpPrefix	=	get(iIndex);

		if (!lpPrefix)
			continue;

		iCount++;

		if (lpPrefix->m_wDiscernmentCode	==	_iDiscernmentCode)	return	lpPrefix;
	}

	return	NULL;
}


class cPREFIX_EFFECT
{
public:
	int							m_iSerial;
	char						m_strReferenceText[256];
};



cPREFIX_EFFECT			l_aPrefixHyperText[dMAX_ITEM_PREFIX_TYPE_COUNT]	=
{
//////////////////////////////////////////////////
//	기본 능력치 조절
//////////////////////////////////////////////////
eIP_ADD_STRENGTH,"Strength +[0]",
eIP_ADD_AGILITY,"Agility +[0]",
eIP_ADD_CONSTITUTION,"Health +[0]",
eIP_ADD_WISDOM,"Wisdom +[0]",
eIP_ADD_INTELIGENCE,"Knowledge +[0]",
eIP_ADD_CHARISMA,"Charisma +[0]",
eIP_ADD_LUCK,"Luck +[0]",
eIP_FIXING_STRENGTH,"Fixing strength [0]",
eIP_FIXING_AGILITY,"Fixing agility [0]",
eIP_FIXING_CONSTITUTION,"Fixing health [0]",
eIP_FIXING_WISDOM,"Fixing Wisdom [0]",
eIP_FIXING_INTELLIGENCE,"Knowledge Fixing [0]",
eIP_FIXING_CHARISMA,"Fixing Charisma [0]",
eIP_FIXING_LUCK,"Luck fixing [0]",
eIP_ADD_STRENGTH_BY_LEVEL,"Strength +[1]/Level [0]",
eIP_ADD_AGILITY_BY_LEVEL,"Agility +[1]/level [0]",
eIP_ADD_CONSTITUTION_BY_LEVEL,"Health +[1]/Level [0]",
eIP_ADD_WISDOM_BY_LEVEL,"Wisdom+[1]/level [0]",
eIP_ADD_INTELIGENCE_BY_LEVEL,"Knowledge +[1]/Level [0]",
eIP_ADD_CHARISMA_BY_LEVEL,"Charisma +[1]/level [0]",
eIP_ADD_LUCK_BY_LEVEL,"Luck +[1]/Level [0]",
eIP_ADD_DAMAGE_PERCENTAGE,"Damage +[0]%",
eIP_ADD_MIN_DAMAGE,"Minimum damage +[0]",
eIP_ADD_MAX_DAMAGE,"Maximum damage +[0]",
eIP_ADD_DEFENSE_PERCENTAGE,"Defense +[0]%",
eIP_ADD_DEFENSE,"Defense +[0]",
eIP_ADD_MAX_HP_PERCENTAGE,"Max HP +[0]%",
eIP_ADD_MAX_HP,"Maximum Health +[0]",
eIP_ADD_MAX_CP_PERCENTAGE,"Max CP +[0]%",
eIP_ADD_MAX_CP,"Max CP +[0]",
eIP_LOW_CP,"Reduction limit CP +[0]",

/////////////////////////////////////////////////////
// Various probability corrections
/////////////////////////////////////////////////////
eIP_CORRECT_HIT_RATE,"Hit rate +[0]％",
eIP_CORRECT_DODGE_RATE,"Dodge rate +[0]%",
eIP_SHIELD_BLOCK_RATE,"Block rate +[0]％",
eIP_CRITICAL_HIT,"Critical hit activation probability +[0]%",
eIP_CRUSH_SHOT,"Probability of triggering a final blow +[0]%",
eIP_IGNORE_TARGET_DODGE_ABILITY,"Ignore target's evasion rate correction",
eIP_IGNORE_ATTACKER_CORRECT_HIT_VALUE,"Ignore attacker's hit rate modifier",
eIP_PERFECT_HIT,"Hit rate <c:LTYELLOW>100%<n>",
eIP_INCREASE_HARD_BLOW,"Hard hit probability +[0]％", // Hard hit probability
///////////////////////////////////////////////////// ////
// Speed/Time Correction
///////////////////////////////////////////////////// ////
eIP_BLOCKING_SPEED,"Blocking speed +[0]%",
eIP_MOVE_SPEED,"Movement speed +[0]%",
eIP_ACTION_SPEED,"Attack speed +[0]%",
eIP_POTION_HEAL_SPEED,"Potion recovery speed +([0]+50)%",
eIP_ADD_CONCENTRATION,"Concentration +[0]%",

//////////////////////////////////////////////////////////////////////////////
//	공격 성공시 추가 효과
//////////////////////////////////////////////////////////////////////////////
eIP_FIRE_DAMAGE,"Fire damage [0]~[1]",
eIP_WATER_DAMAGE,"Water Damage [0]~[1]\n- Cold [2]Frame",
eIP_WIND_DAMAGE,"Wind Damage [0]~[1]\n- Stun [2]Frame",
eIP_TERRA_DAMAGE," [0] to [1] poison damage per second for [2] seconds",
eIP_LIGHT_DAMAGE,"Light damage [0]~[1]\n- Reduces accuracy and evasion for [2] seconds",
eIP_DARK_DAMAGE,"Dark damage [0]~[1]\n- Curse [2] seconds",
eIP_STEAL_HP,"Absorbs [0]% of the damage inflicted on the enemy as stamina.",
eIP_BONUS_CP,"CP acquisition bonus [0]%",
eIP_FLEE,"Enemy Flee [0]%",
eIP_KNOCK_OUT,"Knockout attack +[0]%",
eIP_INSTANT_KILL,"Instant death attack +[0]%",
eIP_WEAPON_BREAKER,"Weapon Break Attack +[0]% ([1] seconds)",
eIP_ARMOR_BREAKER,"Armor Breaking Attack +[0]% ([1] seconds)",
eIP_MAKE_COLD,"Cold Attack+ [0]% ([1] seconds)",
eIP_MAKE_FREEZE,"Freeze Attack +[0]% ([1] seconds)",
eIP_MAKE_BLIND,"Blind Attack +[0]% ([1] seconds)",
eIP_MAKE_STUN,"Stun attack +[0]% ([1] seconds)",
eIP_MAKE_STONE,"Petrification Attack +[0]% ([1] seconds)",
eIP_MAKE_CONFUSE,"Confusion Attack +[0]% ([1] seconds)",
eIP_MAKE_SLEEP,"Sleep Attack +[0]% ([1] seconds)",
eIP_MAKE_CHARMING,"Charming attack +[0]% ([1] seconds)",
eIP_MAKE_FIRST_TARGET,"Priority target attack +[0]% ([1] seconds)",
eIP_MAKE_AI_DOWN ,"AI lowering attack +[0]% ([1] seconds)",
eIP_MAKE_BERSERK,"Berserk Attack +[0]% ([1] seconds)",

///////////////////////////////////////////////////// ///////////////////////////////////////////////////// //////////////////
// Magic attack resistance
///////////////////////////////////////////////////// ///////////////////////////////////////////////////// //////////////////
eIP_RESISTANCE_FIRE,"Fire resistance +[0]%",
eIP_RESISTANCE_WATER,"Water attribute resistance +[0]%",
eIP_RESISTANCE_WIND,"Wind attribute resistance +[0]％",
eIP_RESISTANCE_TERRA,"Earth attribute resistance +[0]%",
eIP_RESISTANCE_LIGHT,"Light attribute resistance +[0]％",
eIP_RESISTANCE_DARK,"Dark attribute resistance +[0]%",
eIP_RESISTANCE_MAGIC,"Magic Resistance +[0]%",
eIP_ABSORB_FIRE,"Absorb fire attribute damage [0]%",
eIP_ABSORB_WATER,"Absorb water attribute damage [0]%",
eIP_ABSORB_WIND,"Absorb wind attribute damage [0]％",
eIP_ABSORB_TERRA,"Earth attribute damage absorption [0]%",
eIP_ABSORB_LIGHT,"Absorb light attribute damage [0]％",
eIP_ABSORB_DARK,"Absorbs dark attribute damage [0]%",
eIP_ABSORB_MAGIC,"Absorb magic attribute damage [0]%",

///////////////////////////////////////////////////// ///////////////////////////////////////////////////// ///////////////////
// Various resistances, immunity
///////////////////////////////////////////////////// ///////////////////////////////////////////////////// ///////////////////
eIP_RESISTANCE_FREEZE,"Freeze resistance +[0]％",
eIP_RESISTANCE_COLD,"Cold Resistance +[0]%",
eIP_RESISTANCE_STUN,"Stun resistance +[0]%",
eIP_RESISTANCE_CONFUSE,"Confusion Resistance +[0]%",
eIP_RESISTANCE_CHARMING,"Charming Resistance +[0]％",
eIP_RESISTANCE_STONE,"Petrification Resistance +[0]%",
eIP_RESISTANCE_INSTANCE_KILL,"Instant kill resistance +[0]%",
eIP_RESISTANCE_POISON,"Poison Resistance +[0]％",
eIP_RESISTANCE_SLEEP,"Sleep Resistance +[0]％",
eIP_RESISTANCE_STRANGE_STATUS,"Status resistance +[0]%",
eIP_RESISTANCE_DECLINE_STATUS,"Degraded resistance +[0]%",
eIP_RESISTANCE_CURSE_STATUS,"Low frequency resistance +[0]%",
eIP_RESISTANCE_ALL_BAD_STATUS,"All ideal resistance +[0]%",
eIP_RESISTANCE_KNOCK_BACK,"Green White Resistance +[0]％",
eIP_RESISTANCE_CRITICAL_SHOT,"Critical Resistance +[0]%",
eIP_RESISTANCE_CRUSH_SHOT,"Closing blow resistance +[0]%",

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	반격,스킬 관련
/////////////////////////////////////////////////////////////////////////////////////////////////////////
eIP_RETURN_DAMAGE, "Return damage [0]%",
eIP_DAMAGE_TO_CP, "Convert damage to CP [0]%",
eIP_MAKE_FREEZE_ATTACKER, "Counter Freeze [0]％([1] seconds)",
eIP_MAKE_COLD_ATTACKER, "Counter cold [0]% ([1] seconds)",
eIP_CHARGE_SKILL, " ",
eIP_JOB_SKILL_LEVEL, "Skill level +[0] ([1] series job)",
eIP_ALL_SKILL_LEVEL, "Skill level +[0]",

///////////////////////////////////////////////////// ///////////////////////////////////////////////////// /////
// Other/Special Abilities
///////////////////////////////////////////////////// ///////////////////////////////////////////////////// /////
eIP_FIRST_AID, "First aid [0]%",
eIP_CHANGE_WEAPON_SPEED, "Weapon exchange speed +[0]%",
eIP_REGENERATION_HP, "Health Recovery +([0]/10 seconds)",
eIP_RESURRECTION_CHANCE, "Resurrection [0]%",
eIP_BLURRED, "<c:LTYELLOW>Blur<n>",
eIP_INVISIBLE, "<c:LTYELLOW>Transparent<n>",
eIP_LEVITATE, "<c:LTYELLOW>Levitate<n>",
eIP_MAGIC_BULLET, "<c:LTYELLOW>Magic Bullet<n>",
eIP_INFINITY_BULLET, "<c:LTYELLOW>Infinite Bullet<n>",
eIP_NOT_AIMED, "<c:LTYELLOW>Do not attack first.<n>",
eIP_INCREASE_RARE_ITEM_DROP_CHANCE, "Magic item drop probability +[0]%",
eIP_INCREASE_UNIQUE_ITEM_DROP_CHANCE, "Unique item drop probability +[0]%",
eIP_DECREASE_RELOAD_TIME, "Item reload time -[0]%",
eIP_AUTO_RELOAD, "<c:LTYELLOW>Automatically reload items<n>",
eIP_DISABLE, "<c:LTYELLOW>Disabled<n>",
eIP_CAN_NOT_STRIP, "<c:LTYELLOW>Cannot take off after wearing<n>",
eIP_CAN_NOT_ADD_PREFIX, "<c:LTYELLOW>Additional enchantment not possible<n>",

eIP_UPGRADE_FIRE_DAMAGE_ATTACK, "<c:LTYELLOW>Increases fire attack by 1 level<n>",
eIP_UPGRADE_WATER_DAMAGE_ATTACK, "<c:LTYELLOW>Water attack increases by 1 level<n>",
eIP_UPGRADE_WIND_DAMAGE_ATTACK, "<c:LTYELLOW>Wind attack increases by 1 level<n>",
eIP_UPGRADE_EARTH_DAMAGE_ATTACK, "<c:LTYELLOW>Earth attack increases by 1 level<n>",
eIP_UPGRADE_LIGHT_DAMAGE_ATTACK, "<c:LTYELLOW>Light attack increases by 1 level<n>",
eIP_UPGRADE_DARK_DAMAGE_ATTACK, "<c:LTYELLOW>Dark attack increases by 1 level<n>",

eIP_CONTROL_SKILL_LEVEL_BY_DIFFICULTY, "Skill difficulty level below [0] level increases by [1].",
eIP_INCREASE_SUMMON_BEAST_STATUS, "All stats of the summoned animal increase by [0]",
eIP_INCREASE_GAIN_EXP_FOR_PET, "[0]% pet experience bonus",
eIP_IS_SHOP_SPECIAL_ITEM, "Use period",

eIP_WEAKNING_TARGET_FIRE_RESISTANCE, "Weakens the target's fire resistance by <c:LTYELLOW>[0]%<n>.",
eIP_WEAKNING_TARGET_WATER_RESISTANCE, "Weakens the target's water resistance by <c:LTYELLOW>[0]%<n>.",
eIP_WEAKNING_TARGET_WIND_RESISTANCE, "Weakens the target's wind resistance by <c:LTYELLOW>[0]%<n>.",
eIP_WEAKNING_TARGET_EARTH_RESISTANCE, "Weakens the target's Earth Resistance by <c:LTYELLOW>[0]%<n>.",
eIP_WEAKNING_TARGET_LIGHT_RESISTANCE, "Weakens the target's light resistance by <c:LTYELLOW>[0]%<n>.",
eIP_WEAKNING_TARGET_DARK_RESISTANCE, "Weakens the target's darkness resistance by <c:LTYELLOW>[0]%<n>.",
eIP_WEAKNING_TARGET_MAGIC_RESISTANCE, "Weakens the target's magic resistance by <c:LTYELLOW>[0]%<n>.",
eIP_STRENGTHENING_FIRE_DAMAGE, "Strengthens fire attribute attack power by <c:LTYELLOW>[0]%<n>.",
eIP_STRENGTHENING_WATER_DAMAGE, "Strengthens water attribute attack power by <c:LTYELLOW>[0]%<n>.",
eIP_STRENGTHENING_WIND_DAMAGE, "Strengthens wind attribute attack power by <c:LTYELLOW>[0]%<n>.",
eIP_STRENGTHENING_EARTH_DAMAGE, "Enhances earth attribute attack power by <c:LTYELLOW>[0]%<n>.",
eIP_STRENGTHENING_LIGHT_DAMAGE, "Strengthens light attribute attack power by <c:LTYELLOW>[0]%<n>.",
eIP_STRENGTHENING_DARK_DAMAGE, "Strengthens dark attribute attack power by <c:LTYELLOW>[0]%<n>.",
eIP_STRENGTHENING_MAGIC_DAMAGE, "Enhances magic attack power by <c:LTYELLOW>[0]%<n>.",

eIP_DECREASE_STRENGTH, "strength -[0]",
eIP_DECREASE_AGILITY, "Agility -[0]",
eIP_DECREASE_CONSTITUTION, "Health -[0]",
eIP_DECREASE_WISDOM, "wisdom -[0]",
eIP_DECREASE_INTELIGENCE, "Knowledge-[0]",
eIP_DECREASE_CHARISMA, "Charisma -[0]",
eIP_DECREASE_LUCK, "Lucky -[0]",

eIP_DECLINE_MOVE_SPEED, "Movement speed -[0]％",
eIP_DECLINE_ACTION_SPEED, "Attack Speed -[0]%",

eIP_ADD_DAMAGE_TO_UNDEAD, "Gives additional <c:LTYELLOW>[0]%<n> damage to undead characters.",
eIP_ADD_DAMAGE_TO_DEMON, "Gives additional <c:LTYELLOW>[0]%<n> damage to devil-type characters.",
eIP_ADD_DAMAGE_TO_HUMAN, "Gives additional <c:LTYELLOW>[0]%<n> damage to humanoid characters.",
eIP_ADD_DAMAGE_TO_ANIMAL, "Gives additional <c:LTYELLOW>[0]%<n> damage to animal-type characters.",
eIP_ADD_DAMAGE_TO_HOLY_ANIMAL, "Gives additional <c:LTYELLOW>[0]%<n> damage to divine beast type characters.",
eIP_POTION_HEAL_SPEED2, "Potion recovery speed +([0])％",

eIP_DECLINE_DEFENSE_PERCENTAGE, "Defense -[0]%",
eIP_DECLINE_DEFENSE, "Defense -[0]",
eIP_DECLINE_HIT_RATE, "Hit rate -[0]％",
eIP_DECLINE_DODGE_RATE, "Escape rate -[0]％",
// VV 09.08.07 Unique option Magic Chudam DX+_+
eIP_FIRE_DAMAGE_DX, "Fire damage [0]~[1]",
eIP_WATER_DAMAGE_DX, "Water Damage [0]~[1]\n- Cold [2]Frame",
eIP_WIND_DAMAGE_DX, "Wind Damage [0]~[1]\n- Stun [2]Frame",
eIP_TERRA_DAMAGE_DX, " [0] to [1] poison damage per second for [2] seconds",
eIP_LIGHT_DAMAGE_DX, "Light damage [0]~[1]\n- Decreases hit and evasion for [2] seconds",
eIP_DARK_DAMAGE_DX, "Dark damage [0]~[1]\n- Curse [2] seconds",
// AA 09.08.07 Unique Option Magic Chudam DX+_+
eIP_BUFF_SKILL_CASTING, "Casts [0] skill with [1] level related stat [2]. (Duration [3] seconds)", // Buff skill casting
eIP_INCREASE_ALL_STATUS, "All abilities of the wearer increase by [0]", // Increase all abilities
eIP_ACL_BUFF_SKILL_CASTING, "[0]Cast the skill at the imprint level", // Imprint buff skill casting
0xffff, "End"
};

//
//	아이템 효과들의 인덱스를 얻어 온다.(시리얼 참조)
int
cITEM_PREFIX_MANAGER::getIndex(int _iSerial)
{
	int	iIndex	=	0;

	while(1)
	{
		if	(l_aPrefixHyperText[iIndex].m_iSerial	==	0xffff	)	break;
		if	(l_aPrefixHyperText[iIndex].m_iSerial	==	_iSerial)	return	iIndex;

		iIndex++;
	}

	return	0xffff;
}

//
//	아이템 부가 효과 설명
char*
cITEM_PREFIX_MANAGER::getComment(cITEM_PREFIX_INFO *_lpPrefixInfo)
{
	cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(_lpPrefixInfo->m_wPrefix);

	int		iIndex	=	getIndex(lpPrefix->m_wType);
	if		(iIndex	==	0xffff)	return	NULL;

	char	*lpText	=	l_aPrefixHyperText[iIndex].m_strReferenceText;
	char	*lpStream;

	m_string.Reset();

	if (lpPrefix->m_wType	==	eIP_JOB_SKILL_LEVEL)
	{
		m_string.Add("%c%s%c skill level +%c%d%c", 1, g_aDefaultJobName[_lpPrefixInfo->m_aValue[1]], 2, 1, _lpPrefixInfo->m_aValue[0], 2);

		return	m_string.String;
	}

	m_decoder.UploadBuffer(lpText,NULL);

	while(1)
	{
		lpStream	=	m_decoder.GetStream('[');

		if (!lpStream)	break;

		m_string.Add(lpStream);
		if (m_decoder.m_isDone	)	break;		//	다 읽어 들였다.

		iIndex		=	m_decoder.GetNumber();

		if (iIndex	==	2)
				iIndex	=	lpPrefix->m_sValue3;
		else	iIndex	=	_lpPrefixInfo->m_aValue[iIndex];

		lpStream	=	m_decoder.GetStream(']');
		m_string.Add(_ms("%c%d%c",1,iIndex,2));

		if (m_decoder.m_isDone	)	break;		//	다 읽어 들였다.
	}

	return	m_string.String;
}
