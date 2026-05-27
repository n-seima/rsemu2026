#include "hero_mini_pet.h"
#include "cSound.h"
#include "CWindowInterface.H"
#include "cMESSAGE.H"

int
cHero_mini_pet::getFistAvailMiniPetIndex()
{
	for (int i=0;i<c_iMiniPetCount;i++)
	{
		if	(m_aMiniPet[i].m_bf7Level)
			return	i;
	}

	return	c_iMiniPetCount;
}

int
cHero_mini_pet::getMiniPetCount()
{
	int	iCount	=	0;

	for (int i=c_iMiniPetCount;i;)
	{
		--i;

		if	(m_aMiniPet[i].m_bf7Level	==	0)
			continue;

		iCount++;
	}

	return	iCount;
}

cMiniPet*
cHero_mini_pet::getMiniPet(int _iSlot)
{
	if	(_iSlot	<	0	||	_iSlot	>=	c_iMiniPetCount)
		return	NULL;

	return	&m_aMiniPet[_iSlot];
}


int
cHero_mini_pet::getBlockLightPocketCount()
{
	int	i,iCount=0;

	for (i=0;i<getItemSlotCount();i++)
		if	(m_aItems[i].isBlockLightPocket())
			iCount++;

	for (i=0;i<m_bf6ExtraInventorySize;i++)
		if (m_aExtraInventory[i].isBlockLightPocket())
			iCount++;

	return	iCount;
}

void
cHero_mini_pet::applyMiniPetBuffSkillEffects(cAbility *_lpAbility)
{
	CSkill	*lpSkill	=	_lpAbility->getSkill();

	if	(!lpSkill)
		return;

	for (int i=0;i<dEXTRA_EFFECT_COUNT;i++)
	{
		CSkillExtraEffect		*lpExtraEffect	=	&lpSkill->m_aExtraEffect[i];
		CSkillExtraEffectInfo	skillEE;

		if	(!_lpAbility->getExtraEffect(i,&skillEE))
			break;

		int	iValue		=	(skillEE.m_aiValue[0]+skillEE.m_aiValue[1]*_lpAbility->m_wLevel)/100;

		int iValueForIncPowerLevel	=	0;
		
		//JBC	미니펫 관련 스텟 증가 수식 변경.	08-11-03
		switch (g_iSeasonVariable)
		{
		case	0:			//0시즌	결국 기존 공식이라는 뜻임...
			iValueForIncPowerLevel		=	0;
			break;
		default:			// 미니펫 2차 강화(시즌 1) 참조.
			iValueForIncPowerLevel		=	g_hero.m_iLevel / 10 + (skillEE.m_aiValue[2])/100;
			break;
		}

		switch(skillEE.m_wEffect)
		{
			case	eSKILL_EE_CORRECT_ITEM_EQUIP_LEVEL	:
				m_wLimitEquipmentLevelOverPoint	+=	iValue;
				break;

			case	eSKILL_EE_INCREASE_SKILL_LEVEL		:
				m_correctAllSkillLevel.addValue(iValue);
				break;

			case	eSKILL_EE_INCREASE_DEFENSE_POWER_PERCENTAGE	:
				m_defensePower.m_sCorrectPercentageValueByPrefix	+=	iValue;
				break;

			case	eSKILL_EE_INCLINE_MOVE_SPEED				:
				m_moveSpeed.addValue(iValue);
				break;

			case	eSKILL_EE_INCLINE_MAX_CP					:
				m_maxCP.m_sCorrectValueByPrefix	+=	iValue;
				break;

			case	eSKILL_EE_INCLINE_MAX_HP					:
				m_maxHP.m_sCorrectValueByPrefix	+=	iValue;
				break;

			case	eSKILL_EE_CORRECT_FIELD_FIRE_PENELTY		:
				m_bCorrectFieldFirePenelty		+=	iValue;
				break;

			case	eSKILL_EE_CORRECT_FIELD_WATER_PENELTY		:
				m_bCorrectFieldWaterPenelty		+=	iValue;
				break;

			case	eSKILL_EE_CORRECT_FIELD_WIND_PENELTY		:
				m_bCorrectFieldWindPenelty		+=	iValue;
				break;

			case	eSKILL_EE_CORRECT_FIELD_EARTH_PENELTY		:
				m_bCorrectFieldEarthPenelty		+=	iValue;
				break;

			case	eSKILL_EE_CORRECT_FIELD_LIGHT_PENELTY		:
				m_bCorrectFieldLightPenelty		+=	iValue;
				break;

			case	eSKILL_EE_CORRECT_FIELD_DARK_PENELTY		:
				m_bCorrectFieldDarkPenelty		+=	iValue;
				break;

			case	eSKILL_EE_STRENGTHENING_MAGIC_DAMAGE			:
				m_strengtheningMagicDamage.addValue(iValue);
				break;
			case	eSKILL_EE_INCLINE_HIT_CHANCE					:
				m_hitChance.addValue(iValue);
				break;
			case	eSKILL_EE_INCLINE_ATTACK_SPEED					:
				m_actionSpeed.addValue(iValue);
				break;
			case	eSKILL_EE_INCLINE_DODGE_CHANCE					:
				m_dodgeChance.addValue(iValue);
				break;
			case	eSKILL_EE_DECLINE_TARGET_MAGIC_RESISTANCE		:
				m_weakningTargetMagicResistance.addValue(iValue);
				break;
			case	eSKILL_EE_INCREASE_FINAL_PHYSICAL_ATTACK_POWER	:
				m_wIncreaseFinalPhysicalAttackPower	+=	iValue;
				break;

			case	eSKILL_EE_INCLINE_MAGIC_RESISTANCE			:
				m_allMagicResistance.addValue(iValue);
				break;

			case	eSKILL_EE_INCLINE_FIRE_RESISTANCE			:	//	불저항 증가
				m_fireResistance.addValue(iValue);
				break;
			case	eSKILL_EE_INCLINE_WATER_RESISTANCE			:	//	물저항 증가
				m_waterResistance.addValue(iValue);
				break;
			case	eSKILL_EE_INCLINE_EARTH_RESISTANCE			:	//	대지저항 증가
				m_earthResistance.addValue(iValue);
				break;
			case	eSKILL_EE_INCLINE_WIND_RESISTANCE			:	//	바람저항 증가
				m_windResistance.addValue(iValue);
				break;
			case	eSKILL_EE_INCLINE_LIGHT_RESISTANCE			:	//	빛저항 증가
				m_lightResistance.addValue(iValue);
				break;
			case	eSKILL_EE_INCLINE_DARK_RESISTANCE			:	//	어둠저항 증가
				m_darkResistance.addValue(iValue);
				break;

			case	eSKILL_EE_INCLINE_POWER						:	//	힘증가"	
				m_strength.addValue(iValue + iValueForIncPowerLevel);
				break;
			case	eSKILL_EE_INCLINE_AGILITY					:	//	민첩성증가"
				m_agility.addValue(iValue + iValueForIncPowerLevel);
				break;
			case	eSKILL_EE_INCLINE_CONSTITUTION				:	//	건강증가"	
				m_constitution.addValue(iValue + iValueForIncPowerLevel);
				break;
			case	eSKILL_EE_INCLINE_CHARISMA					:	//	건강증가"	
				m_charisma.addValue(iValue + iValueForIncPowerLevel);
				break;
			case	eSKILL_EE_INCLINE_INTELIGENCE				:	//	지식증가"	
				m_intelligence.addValue(iValue+ iValueForIncPowerLevel);
				break;
			case	eSKILL_EE_INCLINE_WISDOM					:	//	지혜증가"	
				m_wisdom.addValue(iValue + iValueForIncPowerLevel);
				break;
		}
	}
}

//	미니펫 버프 스킬 처리
void
cHero_mini_pet::operateBuffSkill()
{
	operateComboCondition();

	for	(int i=0;i<2;i++)
	{
		int	iPetIndex	=	m_wActiveMiniPet;

		if	(i)
			iPetIndex	=	m_wActiveMiniPet2;

		if	(iPetIndex	>=	c_iMiniPetCount)
			continue;

		cMiniPet	*lpMiniPet	=	&m_aMiniPet[iPetIndex];

		if	(lpMiniPet->m_bf7Level	==	0)
			return;

		operateEvolutionGradeBonus(iPetIndex);

		cAbility	ability;

		for	(int iSkill=0;iSkill<c_iMiniPetSkillCount;iSkill++)
		{
			cMiniPetAbilityInfo	*lpAbility	=	&lpMiniPet->m_aAbility[iSkill];

			if	(lpAbility->m_bf4Level	==	0)
				continue;
			if	(lpAbility->m_bf1IsOFF)
				continue;
			CSkill	*lpSkill	=	&g_aSkill[lpAbility->m_bf11Skill];

			int	iLevel			=	lpAbility->m_bf4Level;

			if	(lpSkill->m_bf1IsNormalMiniPetSkill)
			{
				iLevel			+=	m_bIncreaseMiniPetBasicSkillLevel;
				iLevel			+=	m_bCorrectMiniPetBasicSkillLevel;
			}

			iLevel			+=	m_bIncreaseMiniPetSkillLevel;
			iLevel			+=	m_bCorrectMiniPetSkillLevel;

			ability.set(lpAbility->m_bf11Skill,iLevel);

			switch(lpSkill->m_bf5MiniPetSkillType)
			{
				case	nsMiniPetST::LordPowerUp	:
				{
					applyMiniPetBuffSkillEffects(&ability);
					break;
				}
			}
		}
	}
}

void
cHero_mini_pet::restraintMiniPet(int _iAttr)
{
	char	*lpstrState;

	switch(_iAttr)
	{
		case	nsMiniPetType::SpritOfFire		:
			m_bf4EatFireMiniPetCount++;
			lpstrState	=	g_alpstrStateName[eSTATE_STRENGTH	];
			break;
		case	nsMiniPetType::SpritOfWater		:
			m_bf4EatWaterMiniPetCount++;
			lpstrState	=	g_alpstrStateName[eSTATE_WISDOM	];
			break;
		case	nsMiniPetType::SpritOfWind		:
			m_bf4EatWindMiniPetCount++;
			lpstrState	=	g_alpstrStateName[eSTATE_AGILITY	];
			break;
		case	nsMiniPetType::SpritOfEarth		:
			m_bf4EatEarthMiniPetCount++;
			lpstrState	=	g_alpstrStateName[eSTATE_CONSTITUTION	];
			break;
		case	nsMiniPetType::SpritOfLight		:
			m_bf4EatLightMiniPetCount++;
			lpstrState	=	g_alpstrStateName[eSTATE_INTELLIGENCE	];
			break;
		case	nsMiniPetType::SpritOfDark		:
			m_bf4EatDarkMiniPetCount++;
			lpstrState	=	g_alpstrStateName[eSTATE_CHARISMA	];
			break;
	}

	buildPower();

	g_msgBox.cPopup("",_ms(dMSG_ABSORB_MINIPET_AND_FEEL_SO_POWERFUL_FORM,lpstrState,dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET),dMSG_OK);
}

void
cHero_mini_pet::changeMiniPetPlace(int _iMiniPet1,int _iMiniPet2)
{
	cMiniPet	miniPet;

	memcpy(&miniPet,&m_aMiniPet[_iMiniPet2],sizeof(cMiniPet));
	memcpy(&m_aMiniPet[_iMiniPet2],&m_aMiniPet[_iMiniPet1],sizeof(cMiniPet));
	memcpy(&m_aMiniPet[_iMiniPet1],&miniPet,sizeof(cMiniPet));

	int	iOldPet1=m_wActiveMiniPet,iOldPet2=m_wActiveMiniPet2;

	if	(_iMiniPet1	==	iOldPet1)
		m_wActiveMiniPet	=	_iMiniPet2;
	if	(_iMiniPet2	==	iOldPet1)
		m_wActiveMiniPet	=	_iMiniPet1;
	if	(_iMiniPet1	==	iOldPet2)
		m_wActiveMiniPet2	=	_iMiniPet2;
	if	(_iMiniPet2	==	iOldPet2)
		m_wActiveMiniPet2	=	_iMiniPet1;

	if	(g_gwMiniPetStatus.m_iSelectMiniPet	==	_iMiniPet1)
		g_gwMiniPetStatus.m_iSelectMiniPet	=	_iMiniPet2;
	else
	if	(g_gwMiniPetStatus.m_iSelectMiniPet	==	_iMiniPet2)
		g_gwMiniPetStatus.m_iSelectMiniPet	=	_iMiniPet1;

	g_gwMiniPetStatus.m_lpSelectMiniPet	=	getMiniPet(g_gwMiniPetStatus.m_iSelectMiniPet);
	g_gwMiniPetStatus.resetButtonsPos();

	syncHeroApprearance();
	operateComboCondition();
}

BOOL
cHero_mini_pet::playFX(int _iFX,int _iMiniPetIndex,BOOL _bIsTest)
{

	if( g_config.m_bf1IsMiniPetSoundOff)
		return FALSE;

	if	(_iMiniPetIndex	>=	c_iMiniPetCount	||	_iMiniPetIndex	<	0)
		return	FALSE;

	cMiniPet	*lpMiniPet	=	&m_aMiniPet[_iMiniPetIndex];

	if	(lpMiniPet->m_bf7Level	==	0)
		return	FALSE;

	int	iType		=	lpMiniPet->m_bf5Type;
	int	iWaveIndex	=	iType*c_iMiniPetVoiceCount+_iFX;

	if	(_iFX	==	eMPFX_USE_SKILL)
	{
		if	(random(100)	>=	10+_bIsTest*1000)
			return	FALSE;

		iWaveIndex	+=	random(2);
	}

	if	(_iFX	==	eMPFX_IDLE)
	{
		if	(random(100)	>=	10+_bIsTest*1000)
			return	FALSE;

		iWaveIndex	+=	random(3);
	}

	if	(_iFX	>=	eMPFX_EAT_LQ_ITEM	&&	_iFX	<=	eMPFX_EAT_EX_ITEM)
	{
		if	(random(100)	>=	20+_bIsTest*1000)
			return	FALSE;
	}

#ifndef	_FOR_KOREA
	if	(iWaveIndex	>=	84)
		return	FALSE;
#endif

	char	wavFName[64];

	sprintf(wavFName,"miniPet/%s.wav",g_strMiniPetWav[iWaveIndex]);

	g_esm.play(wavFName);

	return	TRUE;
}

int
cHero_mini_pet::getMiniPetMagicDamage(cAbility *_lpAbility,int _iPetLevel,int _iValue,int _iValuePerLevel,int _iValueRange,int _iValueRangePerLevel,BOOL _bWantMax)
{
	CSkill	*lpSkill	=	_lpAbility->getSkill();

	int		iValue;
	int		iSkillLevel		=	_lpAbility->m_wLevel;
	float	fPet_slash_2	=	(float)_iPetLevel/2;
	float	fSkill_slash_2	=	(float)iSkillLevel/2;
	int		iDamageExpressionType = lpSkill->m_bf4PetSkillDamageExpressionType;

	//JBC	미니펫 3차 강화(각성의 씨앗 열매)를 위한 작업들...09-01-08
	//기존 데미지 수식의(고정값) 강화
	if(g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet].m_bf1MinipetAwaken100)	
		_iValue	=	2 * lpSkill->m_wAwakenSkillBonusDamageForOrigin;
	else if(g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet].m_bf1MinipetAwaken50)
		_iValue	=  lpSkill->m_wAwakenSkillBonusDamageForOrigin;
	
	//기존 데미지 수식의 레벨당 앞에 붙는 값을 변화
	if(g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet].m_bf1MinipetAwaken100)	
		_iValuePerLevel	=	2 * lpSkill->m_wAwakenSkillBonusDamagePerLevelForOrigin;
	else if(g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet].m_bf1MinipetAwaken50)
		_iValuePerLevel	=  lpSkill->m_wAwakenSkillBonusDamagePerLevelForOrigin;
	
	//기존 데미지 수식의 범위 값의 변화를 변화.-_-;
	if(g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet].m_bf1MinipetAwaken100)	
		_iValueRange	=	2 * lpSkill->m_wAwakenSkillBonusDamageValueRangeForOrigin;
	else if(g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet].m_bf1MinipetAwaken50)
		_iValueRange	=  lpSkill->m_wAwakenSkillBonusDamageValueRangeForOrigin;
	
	//기존 데미지 수식의 레벨당 범위 값의 변화를 변화.-_-;
	if(g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet].m_bf1MinipetAwaken100)	
		_iValueRangePerLevel	=	2 * lpSkill->m_wAwakenSkillBonusDamageValueRangePerLevelForOrigin;
	else if(g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet].m_bf1MinipetAwaken50)
		_iValueRangePerLevel	=  lpSkill->m_wAwakenSkillBonusDamageValueRangePerLevelForOrigin;

	if(g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet].m_bf1MinipetAwaken100 || g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet].m_bf1MinipetAwaken50)
		iDamageExpressionType = lpSkill->m_bf4PetSkillDamageExpressionTypeForAwaken;

	switch(iDamageExpressionType)
	{
		case	0	:
		{
			int	iValueRange	=	_iValueRange+_iValueRangePerLevel*iSkillLevel;

			if	(_bWantMax)
				iValue	=	_iValue+iSkillLevel*_iValuePerLevel+iValueRange;
			else
				iValue	=	_iValue+iSkillLevel*_iValuePerLevel-iValueRange;
			break;
		}
		case	1	:
		{
	//		불 : (10+펫Lv/2±5) * 스킬Lv/2
			int	iValueRange	=	_iValueRange+_iValueRangePerLevel*iSkillLevel;

			if	(_bWantMax)
				iValue	=	(int)(_iValue+(fPet_slash_2)*_iValuePerLevel+iValueRange);
			else
				iValue	=	(int)(_iValue+(fPet_slash_2)*_iValuePerLevel-iValueRange);

			iValue	=	(int)(iValue*fSkill_slash_2);
			break;
		}
		case	2	:
		{
	//		바람 : (2+펫Lv/2±펫Lv/4) * 스킬Lv/2

			int	iValueRange	=	_iValueRange+_iValueRangePerLevel*_iPetLevel/4;

			if	(_bWantMax)
				iValue	=	(int)(_iValue+fPet_slash_2*_iValuePerLevel+iValueRange);
			else
				iValue	=	(int)(_iValue+fPet_slash_2*_iValuePerLevel-iValueRange);

			iValue	=	(int)(iValue*fSkill_slash_2);
			break;
		}
		case	3	:
		{
	//	불 : (50+25*펫Lv/2±25)*스킬Lv
			int	iValueRange	=	_iValueRange+_iValueRangePerLevel*iSkillLevel;

			if	(_bWantMax)
				iValue	=	(int)(_iValue+fPet_slash_2*_iValuePerLevel+iValueRange);
			else
				iValue	=	(int)(_iValue+fPet_slash_2*_iValuePerLevel-iValueRange);
			iValue	*=	iSkillLevel;
			break;
		}
		case	4	:
		{
	//	바람 : (50+25*펫Lv/2±12*펫Lv/2)*스킬Lv
			int	iValueRange	=	(int)(_iValueRange+_iValueRangePerLevel*fPet_slash_2);

			if	(_bWantMax)
				iValue	=	(int)(_iValue+fPet_slash_2*_iValuePerLevel+iValueRange);
			else
				iValue	=	(int)(_iValue+fPet_slash_2*_iValuePerLevel-iValueRange);

			iValue	*=	iSkillLevel;
			break;
		}
		case	5	:
		{
	//	"불:(5+1*펫Lv/2±3) 연속 공격: 5Lv 번
			int	iValueRange	=	_iValueRange+_iValueRangePerLevel*iSkillLevel;

			if	(_bWantMax)
				iValue	=	(int)(_iValue+fPet_slash_2*_iValuePerLevel+iValueRange);
			else
				iValue	=	(int)(_iValue+fPet_slash_2*_iValuePerLevel-iValueRange);
			break;
		}
		case	6	:
		{
	//		빛 : (2+펫Lv/2±펫Lv/2) * 스킬Lv/2
			int	iValueRange	=	(int)(_iValueRange+_iValueRangePerLevel*fPet_slash_2);

			if	(_bWantMax)
				iValue	=	(int)(_iValue+fPet_slash_2*_iValuePerLevel+iValueRange);
			else
				iValue	=	(int)(_iValue+fPet_slash_2*_iValuePerLevel-iValueRange);

			iValue	=	(int)(iValue*fSkill_slash_2);
			break;
		}
		case	7	:
		{
	//	물:(3+0.9펫Lv/2±2)*2Lv
			int	iValueRange	=	_iValueRange+_iValueRangePerLevel;

			if	(_bWantMax)
				iValue	=	(int)(_iValue+fPet_slash_2*_iValuePerLevel+iValueRange);
			else
				iValue	=	(int)(_iValue+fPet_slash_2*_iValuePerLevel-iValueRange);

			iValue	*=	iSkillLevel*2;
			break;
		}
		case	8	:
		{
	//	바람:(3+1펫Lv/2±0.7펫Lv/2)
			int	iValueRange	=	(int)(_iValueRange+_iValueRangePerLevel*fPet_slash_2);

			if	(_bWantMax)
				iValue	=	(int)(_iValue+fPet_slash_2*_iValuePerLevel+iValueRange);
			else
				iValue	=	(int)(_iValue+fPet_slash_2*_iValuePerLevel-iValueRange);
			break;
		}
		case	9	:
		{
			int	iValueRange	=	(int)(_iValueRange+_iValueRangePerLevel*fPet_slash_2);

			if	(_bWantMax)
				iValue	=	(int)(_iValue+fPet_slash_2*_iValuePerLevel+iValueRange);
			else
				iValue	=	(int)(_iValue+fPet_slash_2*_iValuePerLevel-iValueRange);

			iValue	*=	iSkillLevel*2;
			break;
		}
		case	10	:
		{
			int	iValueRange	=	_iValueRange+_iValueRangePerLevel*_iPetLevel/2;

			if	(_bWantMax)
				iValue	=	_iValue+_iPetLevel/2*_iValuePerLevel+iValueRange;
			else
				iValue	=	_iValue+_iPetLevel/2*_iValuePerLevel-iValueRange;
			break;
		}
		case	11	:		//이 이후의 공식은 미니펫 3차 각성후 적용됩니다.
		{
			int iValueRange	= _iValueRange + _iValueRangePerLevel * _iPetLevel/2;

			if	(_bWantMax)
				iValue = _iValue + _iValuePerLevel * _iPetLevel/2 + iValueRange;
			else
				iValue = _iValue + _iValuePerLevel * _iPetLevel/2 - iValueRange;

			break;
		}
		case	12	:
		{
			int iValueRange	= _iValueRange + _iValueRangePerLevel * _iPetLevel/2;

			if	(_bWantMax)
				iValue = _iValue + _iValuePerLevel * _iPetLevel/2 + iValueRange;
			else
				iValue = _iValue + _iValuePerLevel * _iPetLevel/2 - iValueRange;

			iValue = iValue * iSkillLevel;
			break;
		}
		case	13	:
		{
			int iValueRange	= _iValueRange + _iValueRangePerLevel * _iPetLevel/2;

			if	(_bWantMax)
				iValue = _iValue + _iValuePerLevel * _iPetLevel/2 + iValueRange;
			else
				iValue = _iValue + _iValuePerLevel * _iPetLevel/2 - iValueRange;
			break;
		}
		case	14	:
		{
			int iValueRange	= _iValueRange + _iValueRangePerLevel * _iPetLevel/2;

			if	(_bWantMax)
				iValue = _iValue + _iValuePerLevel * _iPetLevel/2 + iValueRange;
			else
				iValue = _iValue + _iValuePerLevel * _iPetLevel/2 - iValueRange;

			iValue = iValue * 2 * iSkillLevel;
			break;
		}
		case	15	:		//이 이후의 공식은 미니펫 3차 각성후 적용됩니다.
		{
			int iValueRange	= _iValueRange + _iValueRangePerLevel * _iPetLevel/4;
			
			if	(_bWantMax)
				iValue = _iValue + _iValuePerLevel * _iPetLevel/2 + iValueRange;
			else
				iValue = _iValue + _iValuePerLevel * _iPetLevel/2 - iValueRange;
			
			iValue = iValue * iSkillLevel / 2;
			break;
		}
	}

	if(g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet].m_bf1MinipetAwaken100)
		iValue *= (2 * lpSkill->m_wAwakenSkillBonusDamage + 2 * lpSkill->m_wAwakenSkillBonusDamagePerLevel * g_hero.m_iLevel)/100;
	else if(g_hero.m_aMiniPet[g_gwMiniPetStatus.m_iSelectMiniPet].m_bf1MinipetAwaken50)
		iValue *= (lpSkill->m_wAwakenSkillBonusDamage + lpSkill->m_wAwakenSkillBonusDamagePerLevel * g_hero.m_iLevel)/100;

	return	iValue;
}

BOOL
cHero_mini_pet::isAvailReversibleFrame()
{
	CTimeInfo	expireTime,currentTime;

	currentTime.update();

	expireTime.m_wYear	=	m_wReversalFrameYear+2000;
	expireTime.m_wMonth	=	m_wReversalFrameMonth;
	expireTime.m_wDay	=	m_wReversalFrameDay;
	expireTime.m_wHour	=	m_wReversalFrameHour;
	expireTime.m_wMinute=	m_wReversalFrameMinute;	//	25

	if	(expireTime.getMinuteValue(2000)	>=	currentTime.getMinuteValue(2000))
		return	TRUE;

	return	FALSE;
}

int
cHero_mini_pet::getMinipetIncreaseActiveChance(cAbility *_lpAbility , WORD _iSkillIndex)
{
	CSkill	*lpSkill		=	_lpAbility->getSkill();

	if(lpSkill->m_aValue[0] == _iSkillIndex)
		return	_lpAbility->getActiveChance();

	return NULL;
}

int
cHero_mini_pet::getMinipetIncreaseHitRange(cAbility *_lpAbility , WORD _iSkillIndex)
{
	CSkill	*lpSkill		=	_lpAbility->getSkill();
	
	if(lpSkill->m_aValue[0] == _iSkillIndex)
		return	_lpAbility->getHitRange(NULL);
	
	return NULL;
}

enum
{
	eIncrease_MiniPet_FireDamage,
	eIncrease_MiniPet_WaterDamage,
	eIncrease_MiniPet_WindDamage,
	eIncrease_MiniPet_EarthDamage,
	eIncrease_MiniPet_LightDamage,
	eIncrease_MiniPet_DarkDamage,
	eIncrease_MiniPet_MagicDamage,
	eCorrect_MiniPet_SkillActiveChance,
	eIncrease_MiniPet_BasicSkillLevel,
	eIncrease_MiniPet_SkillLevel,
	eIncrease_MiniPet_MagicDamage_vsBoss,
};

BOOL
cHero_mini_pet::operateComboCondition()
{
	m_wMiniPetCombiSkill				=	0xffff;

	m_bIncreaseMiniPetFireDamage		=	0;
	m_bIncreaseMiniPetWaterDamage		=	0;
	m_bIncreaseMiniPetWindDamage		=	0;
	m_bIncreaseMiniPetEarthDamage		=	0;
	m_bIncreaseMiniPetLightDamage		=	0;
	m_bIncreaseMiniPetDarkDamage		=	0;
	m_bIncreaseMiniPetMagicDamage		=	0;
	m_bIncreaseMiniPetSkillActiveChance	=	0;
	m_bIncreaseMiniPetBasicSkillLevel	=	0;
	m_bIncreaseMiniPetSkillLevel		=	0;
	m_bIncreaseMiniPetMagicDamageVsBoss	=	0;

	if	(getMiniPetCount()	<	16)
		return	FALSE;

	int	iMaxValue	=	0;

	for	(int i=g_sm.m_iMiniPetCombiSkillCount;i;)
	{
		--i;
		int		iSkill	=	g_sm.m_aiMiniPetCombiSkill[i];
		CSkill	*lpSkill=	&g_aSkill[iSkill];

		BOOL	bIsCorrect=	TRUE;

		for	(int i=0;i<16;i++)
		{
			if	(lpSkill->m_aValue[i]	==	-1)	//	아무거나
				continue;

			if	(lpSkill->m_aValue[i]	>=	100)
			{
				if	((m_aMiniPet[i].m_bf5Type%6)	!=	lpSkill->m_aValue[i]-100)
				{
					bIsCorrect	=	FALSE;
					break;
				}
			}
			else
			if	(m_aMiniPet[i].m_bf5Type	!=	lpSkill->m_aValue[i])
			{
				bIsCorrect	=	FALSE;
				break;
			}
		}

		if	(bIsCorrect	==	FALSE)
			continue;

		int	iValue	=	lpSkill->m_aValue[17];
		int	iEffect	=	lpSkill->m_aValue[16];

		if	(iValue	>	iMaxValue)
		{
			iMaxValue				=	iValue;
			m_wMiniPetCombiSkill	=	iSkill;
		}

		switch(iEffect)
		{
			case	eIncrease_MiniPet_FireDamage:
				m_bIncreaseMiniPetFireDamage	=	max(m_bIncreaseMiniPetFireDamage,iValue);
				break;
			case	eIncrease_MiniPet_WaterDamage:
				m_bIncreaseMiniPetWaterDamage	=	max(m_bIncreaseMiniPetWaterDamage,iValue);
				break;
			case	eIncrease_MiniPet_WindDamage:
				m_bIncreaseMiniPetWindDamage	=	max(m_bIncreaseMiniPetWindDamage,iValue);
				break;
			case	eIncrease_MiniPet_EarthDamage:
				m_bIncreaseMiniPetEarthDamage	=	max(m_bIncreaseMiniPetEarthDamage,iValue);
				break;
			case	eIncrease_MiniPet_LightDamage:
				m_bIncreaseMiniPetLightDamage	=	max(m_bIncreaseMiniPetLightDamage,iValue);
				break;
			case	eIncrease_MiniPet_DarkDamage:
				m_bIncreaseMiniPetDarkDamage	=	max(m_bIncreaseMiniPetDarkDamage,iValue);
				break;
			case	eIncrease_MiniPet_MagicDamage:
				m_bIncreaseMiniPetMagicDamage	=	max(m_bIncreaseMiniPetMagicDamage,iValue);
				break;
			case	eCorrect_MiniPet_SkillActiveChance:
				m_bIncreaseMiniPetSkillActiveChance	=	max(m_bIncreaseMiniPetSkillActiveChance,iValue);
				break;
			case	eIncrease_MiniPet_BasicSkillLevel:
				m_bIncreaseMiniPetBasicSkillLevel	=	max(m_bIncreaseMiniPetBasicSkillLevel,iValue);
				break;
			case	eIncrease_MiniPet_SkillLevel:
				m_bIncreaseMiniPetSkillLevel		=	max(m_bIncreaseMiniPetSkillLevel	,iValue);
				break;
			case	eIncrease_MiniPet_MagicDamage_vsBoss:
				m_bIncreaseMiniPetMagicDamageVsBoss	=	max(m_bIncreaseMiniPetMagicDamageVsBoss,iValue);
				break;
		}

		break;
	}

	if	(m_wMiniPetCombiSkill	!=	0xffff)
		return	TRUE;

	return	FALSE;
}

void
cHero_mini_pet::operateEvolutionGradeBonus(int _iPetIndex)
{
	m_bCorrectMiniPetBasicSkillLevel	=	0;
	m_bCorrectMiniPetSkillLevel			=	0;
	m_bCorrectMiniPetSkillActiveChance	=	0;
	m_bCorrectMiniPetSkillDamage		=	0;

	if	(m_aMiniPet[_iPetIndex].m_bf7Level	==	0)
		return;

	cMiniPet	*lpMiniPet	=	&m_aMiniPet[_iPetIndex];

	int	iRate;

	if	(lpMiniPet->getStyle()	==	nsMiniPetStyle::Bio)
	{
		iRate	=	lpMiniPet->getGrowBioFactor();

		if	(iRate	>=	99)
		{
			m_bCorrectMiniPetSkillDamage	=	50;
		}
		else
		if	(iRate	>=	80)
		{
			m_bCorrectMiniPetSkillDamage	=	20;
		}
		else
		if	(iRate	>=	50)
		{
			m_bCorrectMiniPetSkillDamage	=	10;
		}
	}
	else
	if	(lpMiniPet->getStyle()	==	nsMiniPetStyle::Energy)
	{
		iRate	=	lpMiniPet->getGrowEnergyFactor();

		if	(iRate	>=	99)
		{
			m_bCorrectMiniPetSkillActiveChance	=	10;
		}
		else
		if	(iRate	>=	80)
		{
			m_bCorrectMiniPetSkillActiveChance	=	3;
		}
		else
		if	(iRate	>=	50)
		{
			m_bCorrectMiniPetSkillActiveChance	=	1;
		}
	}
	else
	if	(lpMiniPet->getStyle()	==	nsMiniPetStyle::Mineral)
	{
		iRate	=	lpMiniPet->getGrowMineralFactor();

		if	(iRate	>=	99)
		{
			m_bCorrectMiniPetSkillLevel			=	1;
		}
		else
		if	(iRate	>=	80)
		{
			m_bCorrectMiniPetBasicSkillLevel	=	1;
		}
	}
}