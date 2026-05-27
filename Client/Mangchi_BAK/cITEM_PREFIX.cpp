#include "cITEM_PREFIX.H"
#include "cJOB.H"
#include "cDECODER.H"

cITEM_PREFIX_MANAGER	g_itemPrefix;

BOOL					cITEM_PREFIX_MANAGER::s_isVirgin	=	TRUE;

cITEM_PREFIX::cTYPE		g_aItemPrefixType[dMAX_ITEM_PREFIX_TYPE_COUNT]	=
{
	eIP_ADD_STRENGTH,	"힘 증감",
	eIP_ADD_AGILITY,	"민첩성 증감",
	eIP_ADD_CONSTITUTION,	"건강 증감",
	eIP_ADD_WISDOM,	"지혜 증감",
	eIP_ADD_INTELIGENCE,	"지식 증감",
	eIP_ADD_CHARISMA,	"카리스마 증감",
	eIP_ADD_LUCK,	"운 증감",
	eIP_FIXING_STRENGTH,	"힘 고정",
	eIP_FIXING_AGILITY,	"민첩성 고정",
	eIP_FIXING_CONSTITUTION,	"건강 고정",
	eIP_FIXING_WISDOM,	"지혜 고정",
	eIP_FIXING_INTELLIGENCE,	"지식 고정",
	eIP_FIXING_CHARISMA,	"카리스마 고정",
	eIP_FIXING_LUCK,	"운 고정",
	eIP_ADD_STRENGTH_BY_LEVEL,	"레벨 비례 힘 증감",
	eIP_ADD_AGILITY_BY_LEVEL,	"레벨 비례 민첩성 증감",
	eIP_ADD_CONSTITUTION_BY_LEVEL,	"레벨 비례 건강 증감",
	eIP_ADD_WISDOM_BY_LEVEL,	"레벨 비례 지혜 증감",
	eIP_ADD_INTELIGENCE_BY_LEVEL,	"레벨 비례 지식 증감",
	eIP_ADD_CHARISMA_BY_LEVEL,	"레벨 비례 카리스마 증감",
	eIP_ADD_LUCK_BY_LEVEL,	"레벨 비례 운 증감",
	eIP_ADD_DAMAGE_PERCENTAGE,	"무기 대미지 증감 (퍼센티지)",
	eIP_ADD_MIN_DAMAGE,	"무기 대미지 증감 (최소값)",
	eIP_ADD_MAX_DAMAGE,	"무기 대미지 증감 (최대값)",
	eIP_ADD_DEFENSE_PERCENTAGE,	"방어력 증감 (퍼센티지)",
	eIP_ADD_DEFENSE,	"방어력 증감 (수치)",
	eIP_ADD_MAX_HP_PERCENTAGE,	"최대 체력 증감 (퍼센티지)",
	eIP_ADD_MAX_HP,	"최대 체력 증감 (수치)",
	eIP_ADD_MAX_CP_PERCENTAGE,	"최대 CP 증감 (퍼센티지)",
	eIP_ADD_MAX_CP,	"최대 CP 증감 (수치)",
	eIP_LOW_CP,	"이동시 감소 한계 CP",
	eIP_CORRECT_HIT_RATE,	"공격 성공 확률 (명중률)",
	eIP_CORRECT_DODGE_RATE,	"공격을 피할 확률 (회피율)",
	eIP_SHIELD_BLOCK_RATE,	"방패 발동 확률",
	eIP_CRITICAL_HIT,	"치명타 발동 확률",
	eIP_CRUSH_SHOT,	"결정타 발동 확률",
	eIP_IGNORE_TARGET_DODGE_ABILITY,	"적 방어자의 회피 보정치 무시",
	eIP_IGNORE_ATTACKER_CORRECT_HIT_VALUE,	"적 공격자의 명중 보정치 무시",
	eIP_PERFECT_HIT,	"항상 명중",
	eIP_BLOCKING_SPEED,	"블러킹 속도",
	eIP_MOVE_SPEED,	"이동 속도",
	eIP_ACTION_SPEED,	"공격 속도",
	eIP_POTION_HEAL_SPEED,	"물약 회복 속도 조절[+50]",
	eIP_ADD_CONCENTRATION ,	"CP 감소 속도 조절 (집중력)",
	eIP_FIRE_DAMAGE,	"불 속성 공격",
	eIP_WATER_DAMAGE,	"물 속성 공격",
	eIP_WIND_DAMAGE,	"바람 속성 공격",
	eIP_TERRA_DAMAGE,	"대지 속성 공격",
	eIP_LIGHT_DAMAGE,	"빛 속성 공격",
	eIP_DARK_DAMAGE,	"어둠 속성 공격",
	eIP_STEAL_HP,	"체력 스틸",
	eIP_BONUS_CP,	"보너스 CP",
	eIP_FLEE,	"도망",
	eIP_KNOCK_OUT,	"녹아웃",
	eIP_INSTANT_KILL,	"즉사",
	eIP_WEAPON_BREAKER,	"무기 파괴",
	eIP_ARMOR_BREAKER,	"갑옷 파괴",
	eIP_MAKE_COLD,	"콜드 상태",
	eIP_MAKE_FREEZE,	"프리즈 상태",
	eIP_MAKE_BLIND,	"장님 상태",
	eIP_MAKE_STUN,	"스턴 상태",
	eIP_MAKE_STONE,	"석화 상태",
	eIP_MAKE_CONFUSE,	"혼란 상태",
	eIP_MAKE_SLEEP,	"잠 상태",
	eIP_MAKE_CHARMING,	"차밍 상태",
	eIP_MAKE_FIRST_TARGET,	"우선 타겟이 된 상태",
	eIP_MAKE_AI_DOWN ,	"AI 저하 상태",
	eIP_MAKE_BERSERK,	"버서크 상태",
	eIP_RESISTANCE_FIRE,	"불 속성 공격 저항",
	eIP_RESISTANCE_WATER,	"물 속성 공격 저항",
	eIP_RESISTANCE_WIND,	"바람 속성 공격 저항",
	eIP_RESISTANCE_TERRA,	"대지 속성 공격 저항",
	eIP_RESISTANCE_LIGHT,	"빛 속성 공격 저항",
	eIP_RESISTANCE_DARK,	"어둠 속성 공격 저항",
	eIP_RESISTANCE_MAGIC,	"모든 마법 속성 공격 저항",
	eIP_ABSORB_FIRE,	"불 속성 공격 흡수",
	eIP_ABSORB_WATER,	"물 속성 공격 흡수",
	eIP_ABSORB_WIND,	"바람 속성 공격 흡수",
	eIP_ABSORB_TERRA,	"대지 속성 공격 흡수",
	eIP_ABSORB_LIGHT,	"빛 속성 공격 흡수",
	eIP_ABSORB_DARK,	"어둠 속성 공격 흡수",
	eIP_ABSORB_MAGIC,	"모든 마법 속성 공격 흡수",
	eIP_RESISTANCE_FREEZE,	"프리즈 공격에 대한 저항",
	eIP_RESISTANCE_COLD,	"콜드 공격에 대한 저항",
	eIP_RESISTANCE_STUN,	"스턴 공격에 대한 저항",
	eIP_RESISTANCE_CONFUSE,	"혼란 공격에 대한 저항",
	eIP_RESISTANCE_CHARMING,	"차밍 공격에 대한 저항",
	eIP_RESISTANCE_STONE,	"석화 공격에 대한 저항",
	eIP_RESISTANCE_INSTANCE_KILL,	"즉사 공격에 대한 저항",
	eIP_RESISTANCE_POISON,	"중독 공격에 대한 저항",
	eIP_RESISTANCE_SLEEP,	"잠 공격에 대한 저항",
	eIP_RESISTANCE_STRANGE_STATUS,	"상태이상 공격에 대한 저항",
	eIP_RESISTANCE_DECLINE_STATUS,	"저하계 공격에 대한 저항",
	eIP_RESISTANCE_CURSE_STATUS,	"저주계 공격에 대한 저항",
	eIP_RESISTANCE_ALL_BAD_STATUS,	"모든 이상 공격에 대한 저항",
	eIP_RESISTANCE_KNOCK_BACK,	"녹백/넘어짐 공격에 대한 저항",
	eIP_RESISTANCE_CRITICAL_SHOT,	"치명타 공격에 대한 저항",
	eIP_RESISTANCE_CRUSH_SHOT,	"결정타 공격에 대한 저항",
	eIP_RETURN_DAMAGE,	"받은 대미지 일부 돌려 줌",
	eIP_DAMAGE_TO_CP,	"받은 대미지 CP 변환",
	eIP_MAKE_FREEZE_ATTACKER,	"공격자 프리즈",
	eIP_MAKE_COLD_ATTACKER,	"공격자 콜드",
	eIP_CHARGE_SKILL,	"스킬/마법 충전",
	eIP_JOB_SKILL_LEVEL,	"계열 스킬/마법 레벨 조절",
	eIP_ALL_SKILL_LEVEL,	"모든 스킬/마법 레벨 조절",
	eIP_FIRST_AID,	"타격 응급 처치",
	eIP_CHANGE_WEAPON_SPEED,	"무기 교환 속도",
	eIP_REGENERATION_HP,	"체력 리제네레이션",
	eIP_RESURRECTION_CHANCE,	"부활 확률",
	eIP_BLURRED,	"흐릿한 상태 (Blurred)",
	eIP_INVISIBLE,	"투명 상태 (Invisible)",
	eIP_LEVITATE,	"공중 부양",
	eIP_MAGIC_BULLET,	"마법 탄환",
	eIP_INFINITY_BULLET,	"무한 탄환",
	eIP_NOT_AIMED,	"타겟에서 제외",
	eIP_INCREASE_RARE_ITEM_DROP_CHANCE,	"마법 아이템 확률 증가",
	eIP_INCREASE_UNIQUE_ITEM_DROP_CHANCE,"유니크 아이템 확률 증가",
	eIP_DECREASE_RELOAD_TIME,	"벨트 채우는 시간 감소",
	eIP_AUTO_RELOAD,	"벨트 자동 채우기",
	eIP_DISABLE,	"사용 불가",
	eIP_CAN_NOT_STRIP,	"착용 후 탈의 불능",
	eIP_CAN_NOT_ADD_PREFIX,	"추가 인챈트 불능",
	eIP_UPGRADE_FIRE_DAMAGE_ATTACK,"불 공격 1단계 상승",
	eIP_UPGRADE_WATER_DAMAGE_ATTACK,"물 공격 1단계 상승",
	eIP_UPGRADE_WIND_DAMAGE_ATTACK,"바람 공격 1단계 상승",
	eIP_UPGRADE_EARTH_DAMAGE_ATTACK,"대지 공격 1단계 상승",
	eIP_UPGRADE_LIGHT_DAMAGE_ATTACK,"빛 공격 1단계 상승",
	eIP_UPGRADE_DARK_DAMAGE_ATTACK,"어둠 공격 1단계 상승",

	eIP_CONTROL_SKILL_LEVEL_BY_DIFFICULTY	,	"레벨별 스킬/마법 레벨 조절",
	eIP_INCREASE_SUMMON_BEAST_STATUS		,	"소환수 스탯 증가",
	eIP_INCREASE_GAIN_EXP_FOR_PET			,	"펫 경험치 획득 증가",
	eIP_IS_SHOP_SPECIAL_ITEM				,	"사용기간",

	eIP_WEAKNING_TARGET_FIRE_RESISTANCE			,	"타겟 불 저항 약화",
	eIP_WEAKNING_TARGET_WATER_RESISTANCE		,	"타겟 물 저항 약화",
	eIP_WEAKNING_TARGET_WIND_RESISTANCE			,	"타겟 바람 저항 약화",
	eIP_WEAKNING_TARGET_EARTH_RESISTANCE		,	"타겟 대지 저항 약화",
	eIP_WEAKNING_TARGET_LIGHT_RESISTANCE		,	"타겟 빛 저항 약화",
	eIP_WEAKNING_TARGET_DARK_RESISTANCE			,	"타겟 어둠 저항 약화",
	eIP_WEAKNING_TARGET_MAGIC_RESISTANCE		,	"타겟 마법 저항 약화",
	eIP_STRENGTHENING_FIRE_DAMAGE				,	"불 속성 공격력 강화",
	eIP_STRENGTHENING_WATER_DAMAGE				,	"물 속성 공격력 강화",
	eIP_STRENGTHENING_WIND_DAMAGE				,	"바람 속성 공격력 강화",
	eIP_STRENGTHENING_EARTH_DAMAGE				,	"대지 속성 공격력 강화",
	eIP_STRENGTHENING_LIGHT_DAMAGE				,	"빛 속성 공격력 강화",
	eIP_STRENGTHENING_DARK_DAMAGE				,	"어둠 속성 공격력 강화",
	eIP_STRENGTHENING_MAGIC_DAMAGE				,	"마법 공격력 강화",

	eIP_DECREASE_STRENGTH,	"힘 감소",
	eIP_DECREASE_AGILITY,	"민첩성 감소",
	eIP_DECREASE_CONSTITUTION,	"건강 감소",
	eIP_DECREASE_WISDOM,	"지혜 감소",
	eIP_DECREASE_INTELIGENCE,	"지식 감소",
	eIP_DECREASE_CHARISMA,	"카리스마 감소",
	eIP_DECREASE_LUCK,	"운 감소",

	eIP_DECLINE_MOVE_SPEED,	"이동 속도 감소",
	eIP_DECLINE_ACTION_SPEED,	"공격 속도 감소",

	eIP_ADD_DAMAGE_TO_UNDEAD					,	"언데드 사냥꾼",
	eIP_ADD_DAMAGE_TO_DEMON						,	"악마 사냥꾼",
	eIP_ADD_DAMAGE_TO_HUMAN						,	"인간 사냥꾼",
	eIP_ADD_DAMAGE_TO_ANIMAL					,	"동물 사냥꾼",
	eIP_ADD_DAMAGE_TO_HOLY_ANIMAL				,	"신수 사냥꾼",
	eIP_POTION_HEAL_SPEED2						,	"물약 회복 속도 조절",

	eIP_DECLINE_DEFENSE_PERCENTAGE				,"방어력 퍼센티지 감소",
	eIP_DECLINE_DEFENSE							,"방어력 감소",
	eIP_DECLINE_HIT_RATE						,"명중률 감소",
	eIP_DECLINE_DODGE_RATE						,"회피율 감소",
	// VV 09.08.07    마법추뎀DX+_+
	eIP_FIRE_DAMAGE_DX,	"불 속성 공격(DX)",
	eIP_WATER_DAMAGE_DX,	"물 속성 공격(DX)",
	eIP_WIND_DAMAGE_DX,	"바람 속성 공격(DX)",
	eIP_TERRA_DAMAGE_DX,	"대지 속성 공격(DX)",
	eIP_LIGHT_DAMAGE_DX,	"빛 속성 공격(DX)",
	eIP_DARK_DAMAGE_DX,	"어둠 속성 공격(DX)",
	// AA 09.08.07    마법추뎀DX+_+
	eIP_BUFF_SKILL_CASTING,"버프 스킬 사용",	// 버프 스킬 캐스팅 ~ 09.10.12
	eIP_INCREASE_ALL_STATUS,	"모든 능력치 증가", // 모든능력치증가.
	eIP_ACL_BUFF_SKILL_CASTING,"각인 버프 스킬 사용",	// 각인 버프 스킬 캐스팅 ~
	eIP_INCREASE_HARD_BLOW,"강타 확률 증가",
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
	eIP_ADD_STRENGTH,"힘 +[0]",
	eIP_ADD_AGILITY,"민첩성 +[0]",
	eIP_ADD_CONSTITUTION,"건강 +[0]",
	eIP_ADD_WISDOM,"지혜 +[0]",
	eIP_ADD_INTELIGENCE,"지식 +[0]",
	eIP_ADD_CHARISMA,"카리스마 +[0]",
	eIP_ADD_LUCK,"운 +[0]",
	eIP_FIXING_STRENGTH,"힘 고정 [0]",
	eIP_FIXING_AGILITY,"민첩성 고정 [0]",
	eIP_FIXING_CONSTITUTION,"건강 고정 [0]",
	eIP_FIXING_WISDOM,"지혜 고정 [0]",
	eIP_FIXING_INTELLIGENCE,"지식 고정 [0]",
	eIP_FIXING_CHARISMA,"카리스마 고정 [0]",
	eIP_FIXING_LUCK,"운 고정 [0]",
	eIP_ADD_STRENGTH_BY_LEVEL,"힘 +[1]/레벨 [0]",
	eIP_ADD_AGILITY_BY_LEVEL,"민첩성이 +[1]/레벨 [0]",
	eIP_ADD_CONSTITUTION_BY_LEVEL,"건강이 +[1]/레벨 [0]",
	eIP_ADD_WISDOM_BY_LEVEL,"지혜가+[1]/레벨 [0]",
	eIP_ADD_INTELIGENCE_BY_LEVEL,"지식이 +[1]/레벨 [0]",
	eIP_ADD_CHARISMA_BY_LEVEL,"카리스마가 +[1]/레벨 [0]",
	eIP_ADD_LUCK_BY_LEVEL,"운이 +[1]/레벨 [0]",
	eIP_ADD_DAMAGE_PERCENTAGE,"대미지 +[0]％",
	eIP_ADD_MIN_DAMAGE,"최소 대미지 +[0]",
	eIP_ADD_MAX_DAMAGE,"최대 대미지 +[0]",
	eIP_ADD_DEFENSE_PERCENTAGE,"방어력 +[0]％",
	eIP_ADD_DEFENSE,"방어력 +[0]",
	eIP_ADD_MAX_HP_PERCENTAGE,"최대 체력 +[0]％",
	eIP_ADD_MAX_HP,"최대 체력 +[0]",
	eIP_ADD_MAX_CP_PERCENTAGE,"최대 CP +[0]％",
	eIP_ADD_MAX_CP,"최대 CP +[0]",
	eIP_LOW_CP,"감소 한계 CP +[0]",

//////////////////////////////////////////////////
//	각종 확률 보정
//////////////////////////////////////////////////
	eIP_CORRECT_HIT_RATE,"명중률 +[0]％",
	eIP_CORRECT_DODGE_RATE,"회피율 +[0]％",
	eIP_SHIELD_BLOCK_RATE,"블럭률 +[0]％",
	eIP_CRITICAL_HIT,"치명타 발동 확률 +[0]％",
	eIP_CRUSH_SHOT,"결정타 발동 확률 +[0]％",
	eIP_IGNORE_TARGET_DODGE_ABILITY,"타겟의 회피율 보정치 무시",
	eIP_IGNORE_ATTACKER_CORRECT_HIT_VALUE,"공격자의 명중률 보정치 무시",
	eIP_PERFECT_HIT,"명중률 <c:LTYELLOW>100％<n>",
	eIP_INCREASE_HARD_BLOW,"강타 확률 +[0]％", // 강타확률
//////////////////////////////////////////////////////
//	속도/시간 보정
//////////////////////////////////////////////////////
	eIP_BLOCKING_SPEED,"블럭킹 속도 +[0]％",
	eIP_MOVE_SPEED,"이동 속도 +[0]％",
	eIP_ACTION_SPEED,"공격 속도 +[0]％",
	eIP_POTION_HEAL_SPEED,"물약 회복 속도 +([0]+50)％",
	eIP_ADD_CONCENTRATION,"집중력 +[0]％",

//////////////////////////////////////////////////////////////////////////////
//	공격 성공시 추가 효과
//////////////////////////////////////////////////////////////////////////////
	eIP_FIRE_DAMAGE,"불 대미지 [0]~[1]",
	eIP_WATER_DAMAGE,"물 대미지 [0]~[1]\n- 콜드 [2]Frame",
	eIP_WIND_DAMAGE,"바람 대미지 [0]~[1]\n- 스턴 [2]Frame",
	eIP_TERRA_DAMAGE," [2]초간 초당 [0]~[1]의 중독 대미지",
	eIP_LIGHT_DAMAGE,"빛 대미지 [0]~[1]\n- [2]초동안 명중,회피 저하 ",
	eIP_DARK_DAMAGE,"어둠 대미지 [0]~[1]\n- 저주 [2]초",
	eIP_STEAL_HP,"적에게 입힌 대미지의 [0]％를 체력으로 흡수한다.",
	eIP_BONUS_CP,"CP 획득 보너스 [0]％",
	eIP_FLEE,"적 도망 [0]％",
	eIP_KNOCK_OUT,"녹아웃 공격 +[0]％",
	eIP_INSTANT_KILL,"즉사 공격 +[0]％",
	eIP_WEAPON_BREAKER,"무기파괴 공격 +[0]％([1]초)",
	eIP_ARMOR_BREAKER,"갑옷파괴 공격 +[0]％([1]초)",
	eIP_MAKE_COLD,"콜드 공격+ [0]％([1]초)",
	eIP_MAKE_FREEZE,"프리즈 공격 +[0]％([1]초)",
	eIP_MAKE_BLIND,"장님 공격 +[0]％([1]초)",
	eIP_MAKE_STUN,"스턴 공격 +[0]％([1]초)",
	eIP_MAKE_STONE,"석화 공격 +[0]％([1]초)",
	eIP_MAKE_CONFUSE,"혼란 공격 +[0]％([1]초)",
	eIP_MAKE_SLEEP,"잠 공격 +[0]％([1]초)",
	eIP_MAKE_CHARMING,"차밍 공격 +[0]％([1]초)",
	eIP_MAKE_FIRST_TARGET,"우선 타겟공격 +[0]％([1]초)",
	eIP_MAKE_AI_DOWN ,"AI 저하 공격 +[0]％([1]초)",
	eIP_MAKE_BERSERK,"버서크 공격 +[0]％([1]초)",

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	마법 공격 저항
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	eIP_RESISTANCE_FIRE,"불 속성 저항 +[0]％",
	eIP_RESISTANCE_WATER,"물 속성 저항 +[0]％",
	eIP_RESISTANCE_WIND,"바람 속성 저항 +[0]％",
	eIP_RESISTANCE_TERRA,"대지 속성 저항 +[0]％",
	eIP_RESISTANCE_LIGHT,"빛 속성 저항 +[0]％",
	eIP_RESISTANCE_DARK,"어둠 속성 저항 +[0]％",
	eIP_RESISTANCE_MAGIC,"마법 저항 +[0]％",
	eIP_ABSORB_FIRE,"불 속성 대미지 흡수 [0]％",
	eIP_ABSORB_WATER,"물 속성 대미지 흡수 [0]％",
	eIP_ABSORB_WIND,"바람 속성 대미지 흡수 [0]％",
	eIP_ABSORB_TERRA,"대지 속성 대미지 흡수 [0]％",
	eIP_ABSORB_LIGHT,"빛 속성 대미지 흡수 [0]％",
	eIP_ABSORB_DARK,"어둠 속성 대미지 흡수 [0]％",
	eIP_ABSORB_MAGIC,"마법 속성 대미지 흡수 [0]％",

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	각종 저항, 면역
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	eIP_RESISTANCE_FREEZE,"프리즈 저항 +[0]％",
	eIP_RESISTANCE_COLD,"콜드 저항 +[0]％",
	eIP_RESISTANCE_STUN,"스턴 저항 +[0]％",
	eIP_RESISTANCE_CONFUSE,"혼란 저항 +[0]％",
	eIP_RESISTANCE_CHARMING,"차밍 저항 +[0]％",
	eIP_RESISTANCE_STONE,"석화 저항 +[0]％",
	eIP_RESISTANCE_INSTANCE_KILL,"즉사 저항 +[0]％",
	eIP_RESISTANCE_POISON,"중독 저항 +[0]％",
	eIP_RESISTANCE_SLEEP,"잠 저항 +[0]％",
	eIP_RESISTANCE_STRANGE_STATUS,"상태 이상계 저항 +[0]％",
	eIP_RESISTANCE_DECLINE_STATUS,"저하계 저항 +[0]％",
	eIP_RESISTANCE_CURSE_STATUS,"저주계 저항 +[0]％",
	eIP_RESISTANCE_ALL_BAD_STATUS,"모든 이상계 저항 +[0]％",
	eIP_RESISTANCE_KNOCK_BACK,"녹백 저항 +[0]％",
	eIP_RESISTANCE_CRITICAL_SHOT,"치명타 저항 +[0]％",
	eIP_RESISTANCE_CRUSH_SHOT,"결정타 저항 +[0]％",

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	반격,스킬 관련
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	eIP_RETURN_DAMAGE,"대미지 돌려주기 [0]％",
	eIP_DAMAGE_TO_CP,"대미지를 CP로 전환 [0]％",
	eIP_MAKE_FREEZE_ATTACKER,"카운터 프리즈 [0]％([1]초)",
	eIP_MAKE_COLD_ATTACKER,"카운터 콜드 [0]％([1]초)",
	eIP_CHARGE_SKILL," ",
	eIP_JOB_SKILL_LEVEL,"스킬 레벨 +[0]([1]계열 직업)",
	eIP_ALL_SKILL_LEVEL,"스킬 레벨 +[0]",

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	기타/특수 능력
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	eIP_FIRST_AID,"응급처치 [0]％",
	eIP_CHANGE_WEAPON_SPEED,"무기 교환 속도 +[0]％",
	eIP_REGENERATION_HP,"체력 회복 +([0]/10초)",
	eIP_RESURRECTION_CHANCE,"부활 [0]％",
	eIP_BLURRED,"<c:LTYELLOW>블러<n>",
	eIP_INVISIBLE,"<c:LTYELLOW>투명<n>",
	eIP_LEVITATE,"<c:LTYELLOW>공중 부양<n>",
	eIP_MAGIC_BULLET,"<c:LTYELLOW>마법 탄환<n>",
	eIP_INFINITY_BULLET,"<c:LTYELLOW>무한 탄환<n>",
	eIP_NOT_AIMED,"<c:LTYELLOW>선공을 당하지 않는다.<n>",
	eIP_INCREASE_RARE_ITEM_DROP_CHANCE,"마법 아이템 드랍 확률 +[0]％",
	eIP_INCREASE_UNIQUE_ITEM_DROP_CHANCE,"유니크 아이템 드랍 확률 +[0]％",
	eIP_DECREASE_RELOAD_TIME,"아이템 리로드 타임 -[0]％",
	eIP_AUTO_RELOAD,"<c:LTYELLOW>아이템 자동 리로드<n>",
	eIP_DISABLE,	"<c:LTYELLOW>사용 불가<n>",
	eIP_CAN_NOT_STRIP,	"<c:LTYELLOW>착용 후 탈의 불능<n>",
	eIP_CAN_NOT_ADD_PREFIX,	"<c:LTYELLOW>추가 인챈트 불능<n>",

	eIP_UPGRADE_FIRE_DAMAGE_ATTACK, "<c:LTYELLOW>불 공격 1단계 상승<n>",
	eIP_UPGRADE_WATER_DAMAGE_ATTACK, "<c:LTYELLOW>물 공격 1단계 상승<n>",
	eIP_UPGRADE_WIND_DAMAGE_ATTACK, "<c:LTYELLOW>바람 공격 1단계 상승<n>",
	eIP_UPGRADE_EARTH_DAMAGE_ATTACK, "<c:LTYELLOW>대지 공격 1단계 상승<n>",
	eIP_UPGRADE_LIGHT_DAMAGE_ATTACK, "<c:LTYELLOW>빛 공격 1단계 상승<n>",
	eIP_UPGRADE_DARK_DAMAGE_ATTACK, "<c:LTYELLOW>어둠 공격 1단계 상승<n>",

	eIP_CONTROL_SKILL_LEVEL_BY_DIFFICULTY	, "스킬 난이도 [0] 레벨 이하 스킬들 레벨이 [1] 증가 한다.",
	eIP_INCREASE_SUMMON_BEAST_STATUS		, "소환수의 모든 스탯이 [0] 증가",
	eIP_INCREASE_GAIN_EXP_FOR_PET			, "[0]％의 펫 경험치 보너스",
	eIP_IS_SHOP_SPECIAL_ITEM				,	"사용기간",

	eIP_WEAKNING_TARGET_FIRE_RESISTANCE			,	"타겟의 불 저항을 <c:LTYELLOW>[0]％<n> 약화 시킨다.",
	eIP_WEAKNING_TARGET_WATER_RESISTANCE		,	"타겟의 물 저항을 <c:LTYELLOW>[0]％<n> 약화 시킨다.",
	eIP_WEAKNING_TARGET_WIND_RESISTANCE			,	"타겟의 바람 저항을 <c:LTYELLOW>[0]％<n> 약화 시킨다.",
	eIP_WEAKNING_TARGET_EARTH_RESISTANCE		,	"타겟의 대지 저항을 <c:LTYELLOW>[0]％<n> 약화 시킨다.",
	eIP_WEAKNING_TARGET_LIGHT_RESISTANCE		,	"타겟의 빛 저항을 <c:LTYELLOW>[0]％<n> 약화 시킨다.",
	eIP_WEAKNING_TARGET_DARK_RESISTANCE			,	"타겟의 어둠 저항을 <c:LTYELLOW>[0]％<n> 약화 시킨다.",
	eIP_WEAKNING_TARGET_MAGIC_RESISTANCE		,	"타겟의 마법 저항을 <c:LTYELLOW>[0]％<n> 약화 시킨다.",
	eIP_STRENGTHENING_FIRE_DAMAGE				,	"불 속성 공격력을 <c:LTYELLOW>[0]％<n> 강화 시킨다.",
	eIP_STRENGTHENING_WATER_DAMAGE				,	"물 속성 공격력을 <c:LTYELLOW>[0]％<n> 강화 시킨다.",
	eIP_STRENGTHENING_WIND_DAMAGE				,	"바람 속성 공격력을 <c:LTYELLOW>[0]％<n> 강화 시킨다.",
	eIP_STRENGTHENING_EARTH_DAMAGE				,	"대지 속성 공격력을 <c:LTYELLOW>[0]％<n> 강화 시킨다.",
	eIP_STRENGTHENING_LIGHT_DAMAGE				,	"빛 속성 공격력을 <c:LTYELLOW>[0]％<n> 강화 시킨다.",
	eIP_STRENGTHENING_DARK_DAMAGE				,	"어둠 속성 공격력을 <c:LTYELLOW>[0]％<n> 강화 시킨다.",
	eIP_STRENGTHENING_MAGIC_DAMAGE				,	"마법 공격력을 <c:LTYELLOW>[0]％<n> 강화 시킨다.",

	eIP_DECREASE_STRENGTH,"힘 -[0]",
	eIP_DECREASE_AGILITY,"민첩성 -[0]",
	eIP_DECREASE_CONSTITUTION,"건강 -[0]",
	eIP_DECREASE_WISDOM,"지혜 -[0]",
	eIP_DECREASE_INTELIGENCE,"지식 -[0]",
	eIP_DECREASE_CHARISMA,"카리스마 -[0]",
	eIP_DECREASE_LUCK,"운 -[0]",

	eIP_DECLINE_MOVE_SPEED,"이동 속도 -[0]％",
	eIP_DECLINE_ACTION_SPEED,"공격 속도 -[0]％",

	eIP_ADD_DAMAGE_TO_UNDEAD					,	"언데드형 캐릭터에게 추가로 <c:LTYELLOW>[0]％<n>의 대미지를 준다.",
	eIP_ADD_DAMAGE_TO_DEMON						,	"악마형 캐릭터에게 추가로 <c:LTYELLOW>[0]％<n>의 대미지를 준다.",
	eIP_ADD_DAMAGE_TO_HUMAN						,	"인간형 캐릭터에게 추가로 <c:LTYELLOW>[0]％<n>의 대미지를 준다.",
	eIP_ADD_DAMAGE_TO_ANIMAL					,	"동물형 캐릭터에게 추가로 <c:LTYELLOW>[0]％<n>의 대미지를 준다.",
	eIP_ADD_DAMAGE_TO_HOLY_ANIMAL				,	"신수형 캐릭터에게 추가로 <c:LTYELLOW>[0]％<n>의 대미지를 준다.",
	eIP_POTION_HEAL_SPEED2						,	"물약 회복 속도 +([0])％",

	eIP_DECLINE_DEFENSE_PERCENTAGE				,"방어력 -[0]％",
	eIP_DECLINE_DEFENSE							,"방어력 -[0]",
	eIP_DECLINE_HIT_RATE						,"명중률 -[0]％",
	eIP_DECLINE_DODGE_RATE						,"회피율 -[0]％",
	// VV 09.08.07  유니크 옵션  마법추뎀DX+_+
	eIP_FIRE_DAMAGE_DX,"불 대미지 [0]~[1]",
	eIP_WATER_DAMAGE_DX,"물 대미지 [0]~[1]\n- 콜드 [2]Frame",
	eIP_WIND_DAMAGE_DX,"바람 대미지 [0]~[1]\n- 스턴 [2]Frame",
	eIP_TERRA_DAMAGE_DX," [2]초간 초당 [0]~[1]의 중독 대미지",
	eIP_LIGHT_DAMAGE_DX,"빛 대미지 [0]~[1]\n- [2]초동안 명중,회피 저하 ",
	eIP_DARK_DAMAGE_DX,"어둠 대미지 [0]~[1]\n- 저주 [2]초",
	// AA 09.08.07  유니크 옵션  마법추뎀DX+_+
	eIP_BUFF_SKILL_CASTING,"[0]스킬을 [1]레벨 관련스텟 [2]로 캐스팅한다.(지속시간 [3]초)", // 버프스킬캐스팅 
	eIP_INCREASE_ALL_STATUS,"착용자의 모든능력치가 [0] 증가한다", // 모든 능력치 증가
	eIP_ACL_BUFF_SKILL_CASTING,"[0]스킬을 각인 레벨로 캐스팅한다", // 각인버프스킬캐스팅 
	0xffff,"끝"
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
		m_string.Add("%c%s%c 스킬 레벨 +%c%d%c",1,g_aDefaultJobName[_lpPrefixInfo->m_aValue[1]],2,1,_lpPrefixInfo->m_aValue[0],2);

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
