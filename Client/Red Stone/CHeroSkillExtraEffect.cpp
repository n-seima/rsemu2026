#include <math.h>
#include "CHero.H"
#include "CActor.H"

//
//	스킬 부가 효과 적용
BOOL
CHero::applySkillExtraEffects(CActor *_lpAttacker,cAbility *_lpAbility, DWORD _dwKeepTime)
{		// 마지막 수정일 : 09.10.14
	int		iHitCount	=	0;
	CSkill	*lpSkill	=	_lpAbility->getSkill();

	for (int i=0;i<dEXTRA_EFFECT_COUNT;i++)
	{
		if	(lpSkill->m_aExtraEffect[i].m_bf4TargetMethod == eEEAT_ENEMY)
		{
			if (isOwnForce(_lpAttacker))
				continue;
		}
		else
		if	(lpSkill->m_aExtraEffect[i].m_bf4TargetMethod == eEEAT_OWN_FORCE)
		{
			if (isEnemy(_lpAttacker))
				continue;
		}
		else
		if	(lpSkill->m_aExtraEffect[i].m_bf4TargetMethod == eEEAT_PARTY_MEMBER)
		{
			if	(_lpAttacker->m_wSerial != m_iSerial)
			{
				if	(s_partyInfo.m_wMemberCount	==	0)
					continue;
				if	(!_lpAttacker->isPartyMember())
					continue;
			}
		}
		else
		if	(lpSkill->m_aExtraEffect[i].m_bf4TargetMethod == eEEAT_SELF)
		{
			if (_lpAttacker->m_wSerial != m_iSerial)
				continue;
		}

		if	(lpSkill->m_aExtraEffect[i].m_bf1TargetHuman	==	FALSE)
			continue;

		CSkillExtraEffectInfo	skillEE;

		if (!_lpAbility->getExtraEffect(i,&skillEE))	break;

		iHitCount	+=	applySkillExtraEffect(_lpAttacker,_lpAbility,&skillEE, _dwKeepTime);
	}
/*
	if (lpSkill->m_wHitImage	!=	0xffff)
	{
		CPos	pos;

		g_lpHero->getHitPointCorrectPos(lpSkill->m_wHitImageOutputPart,&pos);
		g_lpHero->addHitEffect(pos.x,pos.y,lpSkill->m_wHitImage);
	}
*/
	return	FALSE;
}


//
//	스킬 효과를 적용 시킨다.
BOOL
CHero::applySkillExtraEffect(CActor *_lpAttacker,cAbility *_lpAbility,CSkillExtraEffectInfo *_lpEffect, DWORD _dwKeepTime)
{		
	int		iEffect,iUpkeepTime;
	int		iSkillLevel	=	_lpAbility->m_wLevel;
	CSkill	*lpSkill	=	_lpAbility->getSkill();
	BOOL	bIsCanNotApplySameEffect	=	FALSE;

	if(lpSkill)
	{
		bIsCanNotApplySameEffect	=	lpSkill->m_bf1IsCanNotApplySameEffect;
	}

	iEffect				=	_lpEffect->m_wEffect;
	iUpkeepTime			=	_lpEffect->m_iUpkeepTime;
	
	if(_dwKeepTime)		// 강제시간이 있으면 강제 지속시간 적용.
		iUpkeepTime = _dwKeepTime;

	if	(iUpkeepTime	==	0)
		iUpkeepTime		=	_lpAbility->getUpkeepTime()*100;


	switch(_lpEffect->m_wEffect)
	{
		case	eSKILL_EE_DETECT_TRAP						:	//	"함정탐지"			::
		{
			int	iValue				=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;

			iUpkeepTime				/=	100;
			m_detectTrap.correctValue(iValue,iUpkeepTime);
			return	TRUE;
		}
		case	eSKILL_EE_DETECT_SECRET_OBJECT				:	//	"비밀오브젝트탐지"	::
		{
			int	iValue				=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;

			iUpkeepTime				/=	100;
			m_detectSecretObject.correctValue(iValue,iUpkeepTime);
			return	TRUE;
		}

		case	eSKILL_EE_INCLINE_MOVE_SPEED			:	//	이동속도증가
		{
			int	iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);

			iValue				=	iValue/100;

			iUpkeepTime			*=	dSYNC_FPS;
			iUpkeepTime			/=	100;

			m_moveSpeed.correctValue(iValue,iUpkeepTime,bIsCanNotApplySameEffect);
			return	TRUE;
		}

		case	eSKILL_EE_INCLINE_ATTACK_SPEED			:	//	공격속도증가
		{
			int	iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);

			iValue				=	iValue/100;

			m_actionSpeed.correctValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			return	TRUE;
		}

		case	eSKILL_EE_INCLINE_ATTACK_POWER			:	//	공격력증가
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];
			int	iValue3			=	_lpEffect->m_aiValue[2];
			int	iValue4			=	_lpEffect->m_aiValue[3];

			int	iLimitValue		=	(iValue3+iValue4*_lpAbility->m_wLevel)/100;
			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			if (iLimitValue)
				m_attackPower.addValue(iValue,iUpkeepTime/100,iLimitValue);
			else
				m_attackPower.correctValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			return	TRUE;
		}

		case	eSKILL_EE_INCLINE_ATTACK_POWER_PERCENTAGE	:
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			m_attackPower.correctPercentageValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			return	TRUE;
		}


		case	eSKILL_EE_INCREASE_DEFENSE_POWER			:	//	방어력 증가
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel);

			correctValueEffectDuelServer(&iValue , lpSkill);

			m_defensePower.correctValue(iValue/100,iUpkeepTime/100 ,bIsCanNotApplySameEffect);
			return	TRUE;
		}
		case	eSKILL_EE_INCREASE_DEFENSE_POWER_PERCENTAGE	:	//	방어력 퍼센티지 증가
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			correctValueEffectDuelServer(&iValue , lpSkill);

			m_defensePower.correctPercentageValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			return	TRUE;
		}

		case	eSKILL_EE_LEVITATE						:	//	공중부양
		{
			if (m_sFloatTime	==	-1)	return	FALSE;

			m_sFloatTime	=	max(m_sFloatTime,iUpkeepTime/100);

			return	TRUE;
		}

		case	eSKILL_EE_INCLINE_DODGE_CHANCE			:	//	회피율 증가
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			m_dodgeChance.correctValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			return	TRUE;
		}

		case	eSKILL_EE_INCLINE_CONCENTRATION			:	//	집중력 증가
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			m_concentration.correctValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			return	TRUE;
		}

		case	eSKILL_EE_INCLINE_CONCENTRATION_BY_STELLAR_PEER		:	//	집중력 증가
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			m_concentrationByStellarPeer.correctValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			return	TRUE;
		}

		case	eSKILL_EE_INCLINE_POWER						:	//	힘증가"	
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			m_strength.correctValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			buildPower();
			return	TRUE;
		}
		case	eSKILL_EE_INCLINE_AGILITY					:	//	민첩성증가"
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			m_agility.correctValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			buildPower();
			return	TRUE;
		}
		case	eSKILL_EE_INCLINE_CONSTITUTION				:	//	건강증가"	
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			m_constitution.correctValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			buildPower();
			return	TRUE;
		}
		case	eSKILL_EE_INCLINE_INTELIGENCE				:	//	지식증가"	
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			m_intelligence.correctValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			buildPower();
			return	TRUE;
		}
		case	eSKILL_EE_INCLINE_WISDOM					:	//	지혜증가"	
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			m_wisdom.correctValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			buildPower();
			return	TRUE;
		}
		case	eSKILL_EE_INCLINE_LUCK						:	//	운증가"	
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			m_luck.correctValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			return	TRUE;
		}
		case	eSKILL_EE_INCLINE_STRANGE_STATUS_RESISTANCE	:	//	이상상태 저항 증가
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			correctValueEffectDuelServer(&iValue , lpSkill);

			m_strangeStatusResistance.correctValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			return	TRUE;
		}
		case	eSKILL_EE_INCLINE_DECLINE_POWER_RESISTANCE	:	//	능력치 저하 저항 증가
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			m_declinePowerResistance.correctValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			return	TRUE;
		}
		case	eSKILL_EE_INCLINE_CURSE_RESISTANCE			:	//	저주 저항 증가
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			m_curseResistance.correctValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			return	TRUE;
		}

		case	eSKILL_EE_INCLINE_FIRE_RESISTANCE			:	//	불저항 증가
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			m_fireResistance.correctValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			return	TRUE;
		}
		case	eSKILL_EE_INCLINE_WATER_RESISTANCE			:	//	물저항 증가
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			m_waterResistance.correctValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			return	TRUE;
		}
		case	eSKILL_EE_INCLINE_EARTH_RESISTANCE			:	//	대지저항 증가
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			m_earthResistance.correctValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			return	TRUE;
		}
		case	eSKILL_EE_INCLINE_WIND_RESISTANCE			:	//	바람저항 증가
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			m_windResistance.correctValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			return	TRUE;
		}
		case	eSKILL_EE_INCLINE_LIGHT_RESISTANCE			:	//	빛저항 증가
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			m_lightResistance.correctValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			return	TRUE;
		}
		case	eSKILL_EE_INCLINE_DARK_RESISTANCE			:	//	암흑저항 증가
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			m_darkResistance.correctValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			return	TRUE;
		}
		case	eSKILL_EE_INCLINE_MAGIC_RESISTANCE	:	//	암흑저항 증가
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			correctValueEffectDuelServer(&iValue , lpSkill);
			m_allMagicResistance.correctValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			return	TRUE;
		}


		case	eSKILL_EE_INCLINE_MAX_CP						:	//	CP증가
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			m_maxCP.correctValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			return	TRUE;
		}
		case	eSKILL_EE_INCLINE_MAX_HP						:	//	체력증가"	
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel);

			m_maxHP.correctValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);

			return	TRUE;
		}

		case	eSKILL_EE_STRENGTHENING_FIRE_DAMAGE		:
		{
			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iValue3		=	_lpEffect->m_aiValue[2];
			int	iValue4		=	_lpEffect->m_aiValue[3];
			int	iValue		=	(iValue1+iValue2*iSkillLevel)/100;
			int	iLimit		=	(iValue3+iValue4*iSkillLevel)/100;

			if	(iLimit)
				iValue			=	min(iValue,iLimit);

			iUpkeepTime	/=	100;

			m_strengtheningFireDamage.correctValue(iValue,iUpkeepTime);
			return	TRUE;
		}
		case	eSKILL_EE_STRENGTHENING_WATER_DAMAGE	:
		{
			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iValue3		=	_lpEffect->m_aiValue[2];
			int	iValue4		=	_lpEffect->m_aiValue[3];
			int	iValue		=	(iValue1+iValue2*iSkillLevel)/100;
			int	iLimit		=	(iValue3+iValue4*iSkillLevel)/100;

			if (iLimit)
				iValue		=	min(iValue,iLimit);

			iUpkeepTime	/=	100;

			m_strengtheningWaterDamage.correctValue(iValue,iUpkeepTime);
			iUpkeepTime		=	m_strengtheningWaterDamage.m_wTime;
			return	TRUE;
		}
		case	eSKILL_EE_STRENGTHENING_WIND_DAMAGE		:
		{
			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iValue3		=	_lpEffect->m_aiValue[2];
			int	iValue4		=	_lpEffect->m_aiValue[3];
			int	iValue		=	(iValue1+iValue2*iSkillLevel)/100;
			int	iLimit		=	(iValue3+iValue4*iSkillLevel)/100;

			if (iLimit)
				iValue		=	min(iValue,iLimit);

			iUpkeepTime	/=	100;

			m_strengtheningWindDamage.correctValue(iValue,iUpkeepTime);
			return	TRUE;
		}
		case	eSKILL_EE_STRENGTHENING_EARTH_DAMAGE	:
		{
			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iValue3		=	_lpEffect->m_aiValue[2];
			int	iValue4		=	_lpEffect->m_aiValue[3];
			int	iValue		=	(iValue1+iValue2*iSkillLevel)/100;
			int	iLimit		=	(iValue3+iValue4*iSkillLevel)/100;

			if (iLimit)
				iValue		=	min(iValue,iLimit);

			iUpkeepTime	/=	100;

			m_strengtheningEarthDamage.correctValue(iValue,iUpkeepTime);
			return	TRUE;
		}
		case	eSKILL_EE_STRENGTHENING_LIGHT_DAMAGE	:
		{
			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iValue3		=	_lpEffect->m_aiValue[2];
			int	iValue4		=	_lpEffect->m_aiValue[3];
			int	iValue		=	(iValue1+iValue2*iSkillLevel)/100;
			int	iLimit		=	(iValue3+iValue4*iSkillLevel)/100;

			if (iLimit)
				iValue		=	min(iValue,iLimit);

			iUpkeepTime	/=	100;

			m_strengtheningLightDamage.correctValue(iValue,iUpkeepTime);
			iUpkeepTime		=	m_strengtheningLightDamage.m_wTime;
			return	TRUE;
		}
		case	eSKILL_EE_STRENGTHENING_DARK_DAMAGE		:
		{
			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iValue3		=	_lpEffect->m_aiValue[2];
			int	iValue4		=	_lpEffect->m_aiValue[3];
			int	iValue		=	(iValue1+iValue2*iSkillLevel)/100;
			int	iLimit		=	(iValue3+iValue4*iSkillLevel)/100;

			if (iLimit)
				iValue		=	min(iValue,iLimit);

			iUpkeepTime	/=	100;

			m_strengtheningDarkDamage.correctValue(iValue,iUpkeepTime);
			iUpkeepTime		=	m_strengtheningDarkDamage.m_wTime;
			return	TRUE;
		}
		case	eSKILL_EE_STRENGTHENING_MAGIC_DAMAGE	:
		{
			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];
			int	iValue3		=	_lpEffect->m_aiValue[2];
			int	iValue4		=	_lpEffect->m_aiValue[3];
			int	iValue		=	(iValue1+iValue2*iSkillLevel)/100;
			int	iLimit		=	(iValue3+iValue4*iSkillLevel)/100;

			if (iLimit)
				iValue		=	min(iValue,iLimit);

			iUpkeepTime	/=	100;

			m_strengtheningMagicDamage.correctValue(iValue,iUpkeepTime);
			return	TRUE;
		}

		case	eSKILL_EE_INCLINE_MAX_HP_PERCENTAGE				:	//	체력증가"	
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel)/100;

			correctValueEffectDuelServer(&iValue , lpSkill);

			m_maxHP.correctPercentageValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			return	TRUE;
		}
		case	eSKILL_EE_REGEN_HP							:	//	체력자동회복"	
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];

			int	iValue			=	(iValue1+iValue2*_lpAbility->m_wLevel);

			m_regenHPPerSecond.correctValue(iValue,iUpkeepTime/100,bIsCanNotApplySameEffect);
			return	TRUE;
		}
		case	eSKILL_EE_FIRST_AID					:
		{
			int	iValue1		=	_lpEffect->m_aiValue[0];
			int	iValue2		=	_lpEffect->m_aiValue[1];

			firstAid(iValue1+iValue2*_lpAbility->m_wLevel);
//	응급처치 지속시간 = 30 / (응급처치한 총 HP / MAX HP) 초	
//	→ 최대 180초이며 이 시간은 응급치료가 될 때마다 치료된 누적 HP를 기준으로 재계산하여 리셋함.
			return	TRUE;
		}

		case	eSKILL_EE_INCREASE_SKILL_LEVEL		:
		{
			int	iValue1			=	_lpEffect->m_aiValue[0];
			int	iValue2			=	_lpEffect->m_aiValue[1];
			int	iLimit			=	_lpEffect->m_aiValue[2]/100;

			int	iLevel			=	min((iValue1+iValue2*_lpAbility->m_wLevel)/100,iLimit);

			m_correctAllSkillLevel.correctValue(iLevel,iUpkeepTime/100);

			return	TRUE;
		}

		case	eSKILL_EE_INCREASE_RIGHT_PUNCH_SKILL_LEVEL	:
		{
			int	iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);
			iValue				=	iValue/100;
			m_correctRightPunchSkillLevel.correctValue(iValue,iUpkeepTime/100);
			return	TRUE;
		}

		case	eSKILL_EE_INCREASE_LEFT_PUNCH_SKILL_LEVEL	:
		{
			int	iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);
			iValue				=	iValue/100;
			m_correctLeftPunchSkillLevel.correctValue(iValue,iUpkeepTime/100);
			return	TRUE;
		}

		case	eSKILL_EE_INCREASE_RIGHT_KICK_SKILL_LEVEL	:
		{
			int	iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);
			iValue				=	iValue/100;
			m_correctRightKickSkillLevel.correctValue(iValue,iUpkeepTime/100);
			return	TRUE;
		}

		case	eSKILL_EE_INCREASE_LEFT_KICK_SKILL_LEVEL	:
		{
			int	iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);
			iValue				=	iValue/100;
			m_correctLeftKickSkillLevel.correctValue(iValue,iUpkeepTime/100);
			return	TRUE;
		}

		case	eSKILL_EE_INCREASE_DOWN_KICK_SKILL_LEVEL	:
		{
			int	iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);
			iValue				=	iValue/100;
			m_correctDownKickSkillLevel.correctValue(iValue,iUpkeepTime/100);
			return	TRUE;
		}
		case	eSKILL_EE_INTERRUPTING_WEAPON				:
			{
				setInterruptingWeapon(iUpkeepTime/100);
				
				return TRUE;
			}
		case eSKILL_EE_DECREASE_FINAL_ATTACK_POWER_PERCENTAGE		:
			{
				return TRUE;
			}
		case eSKILL_EE_NOT_USE_ACTIVITY_REACTION_SKILL :
			{	// 리액션사용금지.
				setReactionAllOff();
				m_wNotUseReactionSkillTime = iUpkeepTime/100;
				return TRUE;
			}			
		case eSKILL_EE_CONSENSUS:
			{	// 교감상태.
				
				m_wConsensusTime = iUpkeepTime/100;
				return TRUE;
			}
		case eSKILL_EE_INCREASE_ALL_STATUS:
			{		// 모든스탯증가상태.
				
			int iValue	=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/100;
			int iTime	=	iUpkeepTime/100;		
			m_strength.correctValue(iValue, iTime);
			m_agility.correctValue(iValue, iTime);	
			m_constitution.correctValue(iValue, iTime);			
			m_intelligence.correctValue(iValue, iTime);		
			m_wisdom.correctValue(iValue, iTime);
			m_charisma.correctValue(iValue, iTime);	
			m_luck.correctValue(iValue, iTime);
	
				return TRUE;
			}
		case eSKILL_EE_MADNESS:
			{	// 광란.

				int	iValue			=	(_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*iSkillLevel)/100;
				m_wMadnessTime = iUpkeepTime/100;
				m_wCorrectFinalDefensivePowerPercentageTime		= m_wMadnessTime;
				m_sCorrectFinalDefensivePowerPercentage			= -iValue;

				return TRUE;

			}
		case eSKILL_EE_INCLINE_CRITICAL_DAMAGE_PERCENTAGE:
			{	// 치명타대미지증가퍼센티지
				return TRUE;
			}
		case eSKILL_EE_DRAIN_CP:
			{	// CP 흡수.
				return TRUE;
			}

		case eSKILL_EE_SOUL_CONVERSION:
			{ // 소울 컨버전.
				

				float fValue = (_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel)/10000.0f;
				int iCorrectValue = -(m_defensePower.m_sCorrectValue *fValue );
				int iPercentageValue = -(m_defensePower.m_sCorrectPercentageValue *fValue );
				m_defensePower.dispell();
				
				if(iCorrectValue < 0)
				{
					m_defensePower.m_sCorrectValue = iCorrectValue;
					m_defensePower.m_wTimeForCorrectValue = iUpkeepTime/100;
				}				
				if(iPercentageValue< 0)
				{
					m_defensePower.m_wTimeForCorrectPercentageValue = iUpkeepTime/100;
					m_defensePower.m_sCorrectPercentageValue = iPercentageValue; 
					
				}

				iCorrectValue = -(m_maxHP.m_sCorrectValue *fValue );
				iPercentageValue = -(m_maxHP.m_sCorrectPercentageValue *fValue );
				iPercentageValue = max(-50,iPercentageValue);
				m_maxHP.m_sCorrectValue				= 0;
				m_maxHP.m_sCorrectPercentageValue	= 0;
				buildPower();

				return	TRUE;		

			}
		case eSKILL_EE_SOUL_BURN:
			{ // 소울 번.
				return TRUE;
			}
		case eSKILL_EE_RAGE_SOUL:
			{ // 타오르는영혼			
				return TRUE;
			}			
		case eSKILL_EE_BINDING_WORDS:
			{	// 언어구속.
				
				m_wBindingWordsTime				= iUpkeepTime/100;
				m_wBindingWordsOverLimitLevel	= _lpEffect->m_aiValue[0];
				return TRUE;
			}
		case eSKILL_EE_FIGHT_ENERGY:
			{	// 투기
				
				WORD	wFightEnergyTime	=  iUpkeepTime/100;
				WORD	wFightEnergyState	= _lpEffect->m_aiValue[9];			
				int		iFightEnergyValue	= (_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);
				if(_lpEffect->m_aiValue[2] != 0)
					iFightEnergyValue = min(iFightEnergyValue, _lpEffect->m_aiValue[2]);
				m_fightEnergy.set(wFightEnergyTime,wFightEnergyState, iFightEnergyValue);
				return TRUE;
			}

		case eSKILL_EE_RUSH:
			{	// 돌진

				m_wRushTime = iUpkeepTime/100;
				g_hero.m_isWalking = TRUE;
				if(g_lpHero)
					g_lpHero->m_isRunning	= TRUE;
				return FALSE;
			}
		case eSKILL_EE_FIGHTING_SPIRIT	:
			{	// 투지
				
				m_wFightingSpiritTime = iUpkeepTime/100;
				return TRUE;
			}
		case eSKILL_EE_IGNORE_NEED_STATE	:
			{	// 필요상태무시
				
				m_wIgnoreNeedStateTime = iUpkeepTime/100;
				return TRUE;
			}
		case eSKILL_EE_INSTANCE_HEAL_EFFECT:
			{	// 즉시힐효과
				
				m_wInstanceHealEffectTime	=  iUpkeepTime/100;
		
				m_wInstanceHealEffectPercent	= (_lpEffect->m_aiValue[0]+_lpEffect->m_aiValue[1]*_lpAbility->m_wLevel);
				if(_lpEffect->m_aiValue[2] != 0)
					m_wInstanceHealEffectPercent = min(m_wInstanceHealEffectPercent, _lpEffect->m_aiValue[2]);
				return TRUE;
			}
	}

	return	TRUE;
}

void
CHero::correctValueEffectDuelServer(int* _iValue , CSkill* _lpSkill)
{
	if(!g_bIsDuelServer)
		return;
	
	if (!*_iValue)
		return;
	
	if (!_lpSkill->m_bf7DuelServerpenaltyDiv)
		return;
	
	if (g_iSeasonVariable >= dSEASON_VARIABLE_PVP_REFORM)
	{
		*_iValue = *_iValue / _lpSkill->m_bf7DuelServerpenaltyDiv;
	}
}