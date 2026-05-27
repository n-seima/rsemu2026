#include <math.h>
#include "cACTOR.H"
#include "cFIELD.H"
#include "CGame.H"
#include "debugCode.H"
#include "arca.H"
#include "CEvent.H"
#include "secretDungeon.H"

int
cACTOR::getBuffedEffectCount(cAbility *_lpAbility)
{
	int		iCount		=	0;
	CSkill	*lpSkill	=	_lpAbility->getSkill();

	if (!lpSkill)
		return	FALSE;

	for (int i=0;i<dEXTRA_EFFECT_COUNT;i++)
	{
		CSkillExtraEffect		*lpExtraEffect	=	&lpSkill->m_aExtraEffect[i];

		if	(lpExtraEffect->m_wEffect	==	0xffff)
			break;

		iCount	+=	isBuffed(lpExtraEffect,_lpAbility);
	}

	return	iCount;
}


BOOL
cACTOR::isLightAdditionalEffect()
{

	if(m_wBlindTime || m_hitChance.isWrong() ||	m_dodgeChance.isWrong())
		return TRUE;

	return FALSE;
}


//
//	스킬 효과를 적용 시킨다.
BOOL
cACTOR::isBuffed(CSkillExtraEffect	*_lpExtraEffect,cAbility *_lpAbility)
{
	switch(_lpExtraEffect->m_wEffect)
	{
		case	eSKILL_EE_TRANS_FROG			:	//	{	"개구리변신"		
		case	eSKILL_EE_TRANS_FROG2			:	//	{	"개구리변신"		
			if	(m_wTransToFrogTime)
				return	TRUE;

			return	FALSE;

		case	eSKILL_EE_FLY					:
			if	(m_wFlyTime)
				return	TRUE;
			return	FALSE;

		case	eSKILL_EE_DODGE_MAGIC_DAMAGE	:
			if	(m_wDodgeMagicDamageTime)
				return	TRUE;
			return	FALSE;

		case	eSKILL_EE_INVISIBLE							:
			if	(m_sInvisivilityTime)
				return	TRUE;

			return	FALSE;

		case	eSKILL_EE_BREAK_ARMOR			:	//	갑옷파괴
			if	(m_wBreakedArmorTime)
				return	TRUE;

			return	FALSE;

		case	eSKILL_EE_BREAK_WEAPON			:	//	무기파괴
			if (m_wBreakedWeaponTime)
				return	TRUE;

			return	FALSE;

		case	eSKILL_EE_DECLINE_HIT_CHANCE			:	//	명중률 저하
			return	m_hitChance.isWrong();

		case	eSKILL_EE_DECLINE_DODGE_CHANCE			:	//	회피율 저하
			return	m_dodgeChance.isWrong();

		case	eSKILL_EE_BLIND							:	//	장님
			if	(m_wBlindTime)
				return	TRUE;

			return	FALSE;

		case	eSKILL_EE_DECLINE_ATTACK_SPEED			:
			return	m_actionSpeed.isWrong();

		case	eSKILL_EE_DECLINE_MOVE_SPEED			:	//	이동속도저하
			return	m_moveSpeed.isWrong();

		case	eSKILL_EE_DECLINE_ATTACK_POWER				:	//	공격력 저하
		case	eSKILL_EE_DECLINE_ATTACK_POWER_PERCENTAGE	:
			return	m_attackPower.isWrong();

		case	eSKILL_EE_INCLINE_MOVE_SPEED			:	//	이동속도증가
			return	m_moveSpeed.isGood();

		case	eSKILL_EE_INCLINE_ATTACK_SPEED			:	//	공격속도증가
			return	m_actionSpeed.isGood();

		case	eSKILL_EE_DECLINE_WEAPON_ATTACK_POWER_PERCENTAGE	:
			return	m_weaponPower.isWrong();

		case	eSKILL_EE_INCLINE_WEAPON_ATTACK_POWER				:
			return	m_weaponPower.isGood();

		case	eSKILL_EE_INCLINE_ARMOR_DEFENSE_POWER			:	//	기본 방어력증가
			return	m_armorPower.isGood();

		case	eSKILL_EE_INCLINE_ATTACK_POWER			:	//	공격력증가
			return	m_attackPower.isGood();

		case	eSKILL_EE_INCLINE_ATTACK_POWER_PERCENTAGE	:
			return	m_attackPower.isGood();

		case	eSKILL_EE_DECREASE_DEFENSE_POWER			:
		case	eSKILL_EE_DECREASE_DEFENSE_POWER_PERCENTAGE	:
			return	m_defensePower.isWrong();

		case	eSKILL_EE_INCREASE_DEFENSE_POWER			:	//	방어력 증가
		case	eSKILL_EE_INCREASE_DEFENSE_POWER_PERCENTAGE	:	//	방어력 퍼센티지 증가
			return	m_defensePower.isGood();

		case	eSKILL_EE_LEVITATE						:	//	공중부양
			if	(m_sFloatTime)
				return	TRUE;

			return	FALSE;

		case	eSKILL_EE_INCLINE_DODGE_CHANCE			:	//	회피율 증가
			return	m_dodgeChance.isGood();

		case	eSKILL_EE_DECLINE_CONCENTRATION			:
			return	m_concentration.isWrong();

		case	eSKILL_EE_INCLINE_CONCENTRATION			:	//	집중력 증가
			return	m_concentration.isGood();

		case	eSKILL_EE_INCLINE_POWER						:	//	힘증가"	
			return	m_strength.isGood();

		case	eSKILL_EE_INCLINE_AGILITY					:	//	민첩성증가"
			return	m_agility.isGood();

		case	eSKILL_EE_INCLINE_CONSTITUTION				:	//	건강증가"	
			return	m_constitution.isGood();

		case	eSKILL_EE_INCLINE_INTELIGENCE				:	//	지식증가"	
			return	m_intelligence.isGood();

		case	eSKILL_EE_INCLINE_WISDOM					:	//	지혜증가"	
			return	m_wisdom.isGood();

		case	eSKILL_EE_INCLINE_LUCK						:	//	운증가"	
			return	m_luck.isGood();

		case	eSKILL_EE_INCLINE_STRANGE_STATUS_RESISTANCE	:	//	이상상태 저항 증가
			return	m_strangeStatusResistance.isGood();

		case	eSKILL_EE_INCLINE_DECLINE_POWER_RESISTANCE	:	//	능력치 저하 저항 증가
			return	m_declinePowerResistance.isGood();

		case	eSKILL_EE_INCLINE_CURSE_RESISTANCE			:	//	저주 저항 증가
			return	m_curseResistance.isGood();

		case	eSKILL_EE_INCREASE_STUN_RESISTANCE	:
			return	m_stunResistance.isGood();

		case	eSKILL_EE_DECREASE_PHYSICAL_DAMAGE	:
			if	(m_wReducePhysicalDamage)
				return	TRUE;

			return	FALSE;

		case	eSKILL_EE_INVINCIBLE				:
			if	(m_wInvincibleTime)
				return	TRUE;

			return	FALSE;

		case	eSKILL_EE_DECLINE_MAGIC_RESISTANCE	:
		case	eSKILL_EE_DECLINE_FIRE_RESISTANCE	:	//	불저항 증가
		case	eSKILL_EE_DECLINE_WATER_RESISTANCE	:	//	물저항 증가
		case	eSKILL_EE_DECLINE_EARTH_RESISTANCE	:	//	대지저항 증가
		case	eSKILL_EE_DECLINE_WIND_RESISTANCE	:	//	바람저항 증가
		case	eSKILL_EE_DECLINE_LIGHT_RESISTANCE	:	//	빛저항 증가
		case	eSKILL_EE_DECLINE_DARK_RESISTANCE	:	//	암흑저항 증가
		{
			switch(_lpExtraEffect->m_wEffect)
			{
				case	eSKILL_EE_DECLINE_MAGIC_RESISTANCE	:
					return	m_allMagicResistance.isWrong();

				case	eSKILL_EE_DECLINE_FIRE_RESISTANCE	:	//	불저항 감소
					return	m_fireResistance.isWrong();

				case	eSKILL_EE_DECLINE_WATER_RESISTANCE	:	//	물저항 감소
					return	m_waterResistance.isWrong();

				case	eSKILL_EE_DECLINE_EARTH_RESISTANCE	:	//	대지저항 감소
					return	m_earthResistance.isWrong();

				case	eSKILL_EE_DECLINE_WIND_RESISTANCE	:	//	바람저항 감소
					return	m_windResistance.isWrong();

				case	eSKILL_EE_DECLINE_LIGHT_RESISTANCE	:	//	빛저항 감소
					return	m_lightResistance.isWrong();

				case	eSKILL_EE_DECLINE_DARK_RESISTANCE	:	//	암흑저항 감소
					return	m_darkResistance.isWrong();
			}

			return	FALSE;
		}


		case	eSKILL_EE_INCLINE_MAGIC_RESISTANCE	:
		case	eSKILL_EE_INCLINE_FIRE_RESISTANCE	:	//	불저항 증가
		case	eSKILL_EE_INCLINE_WATER_RESISTANCE	:	//	물저항 증가
		case	eSKILL_EE_INCLINE_EARTH_RESISTANCE	:	//	대지저항 증가
		case	eSKILL_EE_INCLINE_WIND_RESISTANCE	:	//	바람저항 증가
		case	eSKILL_EE_INCLINE_LIGHT_RESISTANCE	:	//	빛저항 증가
		case	eSKILL_EE_INCLINE_DARK_RESISTANCE	:	//	암흑저항 증가
		{
			switch(_lpExtraEffect->m_wEffect)
			{
				case	eSKILL_EE_INCLINE_MAGIC_RESISTANCE	:
					return	m_allMagicResistance.isGood();

				case	eSKILL_EE_INCLINE_FIRE_RESISTANCE	:	//	불저항 감소
					return	m_fireResistance.isGood();

				case	eSKILL_EE_INCLINE_WATER_RESISTANCE	:	//	물저항 감소
					return	m_waterResistance.isGood();

				case	eSKILL_EE_INCLINE_EARTH_RESISTANCE	:	//	대지저항 감소
					return	m_earthResistance.isGood();

				case	eSKILL_EE_INCLINE_WIND_RESISTANCE	:	//	바람저항 감소
					return	m_windResistance.isGood();

				case	eSKILL_EE_INCLINE_LIGHT_RESISTANCE	:	//	빛저항 감소
					return	m_lightResistance.isGood();

				case	eSKILL_EE_INCLINE_DARK_RESISTANCE	:	//	암흑저항 감소
					return	m_darkResistance.isGood();
			}

			return	FALSE;
		}

		case	eSKILL_EE_INCLINE_MAX_CP						:	//	CP증가
			return	m_maxCP.isGood();

		case	eSKILL_EE_INCLINE_MAX_HP						:	//	체력증가"	
			return	m_maxHP.isGood();

		case	eSKILL_EE_INCLINE_MAX_HP_PERCENTAGE				:	//	체력증가"	
			return	m_maxHP.isGood();

		case	eSKILL_EE_REGEN_HP							:	//	체력자동회복"	
			return	m_regenHPPerSecond.isGood();

		case	eSKILL_EE_ADDITIONAL_FIRE_DAMAGE			:	//	추가불대미지	
		case	eSKILL_EE_ADDITIONAL_WATER_DAMAGE			:	//	추가물대미지	
		case	eSKILL_EE_ADDITIONAL_WIND_DAMAGE			:	//	추가바람대미지	
		case	eSKILL_EE_ADDITIONAL_EARTH_DAMAGE			:	//	추가대지대미지	
		case	eSKILL_EE_ADDITIONAL_LIGHT_DAMAGE			:	//	추가빛대미지	
		case	eSKILL_EE_ADDITIONAL_DARK_DAMAGE			:	//	추가어둠대미지	
		{
			if (_lpExtraEffect->m_wEffect	==	eSKILL_EE_ADDITIONAL_FIRE_DAMAGE	)
				return	m_addFireDamage.isGood();

			if (_lpExtraEffect->m_wEffect	==	eSKILL_EE_ADDITIONAL_WATER_DAMAGE	)	
				return	m_addWaterDamage.isGood();

			if (_lpExtraEffect->m_wEffect	==	eSKILL_EE_ADDITIONAL_WIND_DAMAGE	)	
				return	m_addWindDamage.isGood();

			if (_lpExtraEffect->m_wEffect	==	eSKILL_EE_ADDITIONAL_EARTH_DAMAGE	)	
				return	m_addEarthDamage.isGood();

			if (_lpExtraEffect->m_wEffect	==	eSKILL_EE_ADDITIONAL_LIGHT_DAMAGE	)
				return	m_addLightDamage.isGood();

			if (_lpExtraEffect->m_wEffect	==	eSKILL_EE_ADDITIONAL_DARK_DAMAGE	)
				return	m_addDarkDamage.isGood();
			
			return	TRUE;
		}

		case	eSKILL_EE_DECLINE_MOVE_SPEED_BY_EARTH		:	//	이동속도 저하 by 대지
			return	m_moveSpeed.isWrong();

		case	eSKILL_EE_STONE_BY_EARTH					:	//	석화 by 대지
			if	(m_wStoneTime)
				return	TRUE;

			return	FALSE;

		case	eSKILL_EE_FLEE					:	//도망
			if	(m_wFleeTime)
				return	TRUE;

			return	FALSE;

		case	eSKILL_EE_POISON							:
			if	(m_wPoisonTime)
				return	TRUE;

			return	FALSE;

		case	eSKILL_EE_INCLINE_BY_OWNER_FORCE			:
			return	m_attackPower.isGood();

		case	eSKILL_EE_DECLINE_HIT_CHANCE_BY_LIGHT	:	//	빛에 의한 명중률 저하
			return	m_hitChance.isWrong();

		case	eSKILL_EE_DECLINE_DODGE_CHANCE_BY_LIGHT	:	//	빛에 의한 회피율 저하
			return	m_dodgeChance.isWrong();

		case	eSKILL_EE_BLIND_BY_LIGHT				:	//	빛에 의한 장님
			if	(m_wBlindTime)
				return	TRUE;

			return	FALSE;

		case	eSKILL_EE_LIGHT_ATTACK_ADDITIONAL_EFFECT	:
			return	m_hitChance.isWrong() + m_dodgeChance.isWrong();

		case	eSKILL_EE_CURSE_AMPLIFICATION				:
			if	(m_wCurseAmplificationTime)
				return	TRUE;

			return	FALSE;

		case	eSKILL_EE_SLEEP								:
			if	(m_wSleepTime)
				return	TRUE;

			return	FALSE;

		case	eSKILL_EE_INCLINE_HIT_CHANCE:
			return	m_hitChance.isGood();
		
		case	eSKILL_EE_INCREASE_LEVEL	:
			return	m_level.isGood();

		case	eSKILL_EE_LEVEL_DRAIN		:
			return	m_level.isWrong();

		case	eSKILL_EE_DECREASE_BLOCKING_CHANCE	:
			return	m_blockingChance.isWrong();

		case	eSKILL_EE_FIRST_AID					:
			if	(m_wFirstAidTime)
				return	TRUE;

			return	FALSE;

		case	eSKILL_EE_STRENGTHENING_FIRE_DAMAGE		:
			return	m_strengtheningFireDamage.isGood();

		case	eSKILL_EE_STRENGTHENING_WATER_DAMAGE	:
			return	m_strengtheningFireDamage.isGood();

		case	eSKILL_EE_STRENGTHENING_WIND_DAMAGE		:
			return	m_strengtheningWindDamage.isGood();

		case	eSKILL_EE_STRENGTHENING_EARTH_DAMAGE	:
			return	m_strengtheningEarthDamage.isGood();

		case	eSKILL_EE_STRENGTHENING_LIGHT_DAMAGE	:
			return	m_strengtheningLightDamage.isGood();

		case	eSKILL_EE_STRENGTHENING_DARK_DAMAGE		:
			return	m_strengtheningDarkDamage.isGood();

		case	eSKILL_EE_STRENGTHENING_MAGIC_DAMAGE	:
			return	m_strengtheningMagicDamage.isGood();

		case	eSKILL_EE_INCREASE_SKILL_LEVEL		:
			return	m_correctAllSkillLevel.isGood();

		case	eSKILL_EE_ADD_THORN					:
			return	m_returnDamageRate.isGood();

		case	eSKILL_EE_INCLINE_CRITICAL_CHANCE	:
			return	m_correctCriticalHitChance.isGood();

		case	eSKILL_EE_MAKE_THE_OTHER_SELF		:
			if	(m_wUpkeepTheOtherSelfTime)
				return	TRUE;

			return	FALSE;

		case	eSKILL_NOTIFICATION_OF_DEATH		:
			if	(m_wNotificationOfDeathCounter)
				return	TRUE;

			return	FALSE;

		case	eSKILL_EE_TORTURE					:
			if	(m_wTortureTime)
				return	TRUE;

			return	FALSE;

		case		eSKILL_EE_MAKE_SKILL_ZONE				:
		{
			cAbility	ability;

			ability.set(_lpExtraEffect->m_aiValue[0],_lpAbility->m_wLevel);

			int		iRange	=	ability.getHitRange()/2;

			if	(m_lpField->m_skillManager.findActiveSkill(m_pos.x,m_pos.y,ability.m_wSkill,iRange)	!=	0xffff)
				return	TRUE;

			return	FALSE;
		}
	}

	return	FALSE;
}

BOOL
cACTOR::setInverseBuff(float _fValuePercent, int _iKeepTime)
{


	if(m_hitChance.isGood())
	{
		int iValue = -(m_hitChance.m_sCorrectValue * _fValuePercent );
		m_hitChance.dispell();
		m_hitChance.correctValue(iValue, _iKeepTime);
	}
	if(m_dodgeChance.isGood())
	{
		int iValue = -(m_dodgeChance.m_sCorrectValue * _fValuePercent );
		m_dodgeChance.dispell();
		m_dodgeChance.correctValue(iValue, _iKeepTime);
	}
	if(m_actionSpeed.isGood())
	{
		int iValue = -(m_actionSpeed.m_sCorrectValue * _fValuePercent );
		m_actionSpeed.dispell();
		m_actionSpeed.correctValue(iValue, _iKeepTime);
	}
	if(m_moveSpeed.isGood())
	{
		_iKeepTime			*=	dSYNC_FPS;
		int iValue = -(m_moveSpeed.m_sCorrectValue * _fValuePercent );
		m_moveSpeed.dispell();
		m_moveSpeed.correctValue(iValue, _iKeepTime);
	}

	if(m_attackPower.isGood())
	{
		int iValue = -(m_attackPower.m_sCorrectValue * _fValuePercent );
		m_attackPower.dispell();
		m_attackPower.correctValue(iValue, _iKeepTime);
	}
	if(m_weaponPower.isGood())
	{
		int iValue = -(m_weaponPower.m_sCorrectValue * _fValuePercent );
		m_weaponPower.dispell();
		m_weaponPower.correctValue(iValue, _iKeepTime);	
	}

	if(m_armorPower.isGood())
	{
		int iValue = -(m_armorPower.m_sCorrectValue * _fValuePercent );
		m_armorPower.dispell();
		m_armorPower.correctValue(iValue, _iKeepTime);	
	}


	if(m_defensePower.isGood())
	{
		int iValue = -(m_defensePower.m_sCorrectValue * _fValuePercent );
		int iPercentageValue = -(m_defensePower.m_sCorrectPercentageValue *_fValuePercent );
		
		m_defensePower.dispell();
		if(iValue < 0)
			m_defensePower.correctValue(iValue, _iKeepTime);	
		if(iPercentageValue < 0)
			m_defensePower.correctPercentageValue(iPercentageValue,_iKeepTime);
	}

	if(m_concentration.isGood())
	{
		int iValue = -(m_concentration.m_sCorrectValue * _fValuePercent );
		m_concentration.dispell();
		m_concentration.correctValue(iValue, _iKeepTime);	
	}

	if(m_strength.isGood())
	{
		int iValue = -(m_strength.m_sCorrectValue * _fValuePercent );
		m_strength.dispell();
		m_strength.correctValue(iValue, _iKeepTime);
	}
	if(m_agility.isGood())
	{
		int iValue = -(m_agility.m_sCorrectValue * _fValuePercent );
		m_agility.dispell();
		m_agility.correctValue(iValue, _iKeepTime);
	}
	if(m_constitution.isGood())
	{
		int iValue = -(m_constitution.m_sCorrectValue * _fValuePercent );
		m_constitution.dispell();
		m_constitution.correctValue(iValue, _iKeepTime);
	}
	if(m_intelligence.isGood())
	{
		int iValue = -(m_intelligence.m_sCorrectValue * _fValuePercent );
		m_intelligence.dispell();
		m_intelligence.correctValue(iValue, _iKeepTime);
	}
	if(m_wisdom.isGood())
	{
		int iValue = -(m_wisdom.m_sCorrectValue * _fValuePercent );
		m_wisdom.dispell();
		m_wisdom.correctValue(iValue, _iKeepTime);
	}
	if(m_luck.isGood())
	{
		int iValue = -(m_luck.m_sCorrectValue * _fValuePercent );
		m_luck.dispell();
		m_luck.correctValue(iValue, _iKeepTime);
	}
	if(m_level.isGood())
	{
		int iValue = -(m_level.m_sCorrectValue * _fValuePercent );
		m_level.dispell();
		if(iValue<0)
			m_iMaxHPForDrainLevel	=	getMaxHP();
		m_level.correctValue(iValue, _iKeepTime);	
		{
			m_strength.correctValue(iValue,_iKeepTime);
			m_constitution.correctValue(iValue,_iKeepTime);
			m_agility.correctValue(iValue,_iKeepTime);
			m_charisma.correctValue(iValue,_iKeepTime);
			m_intelligence.correctValue(iValue,_iKeepTime);
		}
		sendEtcWork(eEW_LEVEL_DRAIN,m_wSerialInField,m_level.m_sCorrectValue,m_level.m_wTime,TRUE,getLevel(),0);				
		if	(isPlayer())
			buildPower();
		else
			rebuildForUpdateLevel();
	}

	if(m_strangeStatusResistance.isGood())
	{
			int iValue = -(m_strangeStatusResistance.m_sCorrectValue * _fValuePercent );
		m_strangeStatusResistance.dispell();
		m_strangeStatusResistance.correctValue(iValue, _iKeepTime);
	}
	if(m_declinePowerResistance.isGood())
	{
		int iValue = -(m_declinePowerResistance.m_sCorrectValue * _fValuePercent );
		m_declinePowerResistance.dispell();
		m_declinePowerResistance.correctValue(iValue, _iKeepTime);
	}
	if(m_curseResistance.isGood())
	{
			int iValue = -(m_curseResistance.m_sCorrectValue * _fValuePercent );
		m_curseResistance.dispell();
		m_curseResistance.correctValue(iValue, _iKeepTime);	
	}
	if(m_stunResistance.isGood())
	{
		int iValue = -(m_stunResistance.m_sCorrectValue * _fValuePercent );
		m_stunResistance.dispell();
		m_stunResistance.correctValue(iValue, _iKeepTime);	
	}
	if(m_allMagicResistance.isGood())
	{
		int iValue = -(m_allMagicResistance.m_sCorrectValue * _fValuePercent );
		m_allMagicResistance.dispell();
		m_allMagicResistance.correctValue(iValue, _iKeepTime);
	}
				
	if(m_fireResistance.isGood())
	{
		int iValue = -(m_fireResistance.m_sCorrectValue * _fValuePercent );
		m_fireResistance.dispell();
		m_fireResistance.correctValue(iValue, _iKeepTime);
	}
	if(m_waterResistance.isGood())
	{
		int iValue = -(m_waterResistance.m_sCorrectValue * _fValuePercent );
		m_waterResistance.dispell();
		m_waterResistance.correctValue(iValue, _iKeepTime);
	}

	if(m_earthResistance.isGood())
	{
		int iValue = -(m_earthResistance.m_sCorrectValue * _fValuePercent );
		m_earthResistance.dispell();
		m_earthResistance.correctValue(iValue, _iKeepTime);
	}
			
	if(m_windResistance.isGood())
	{
		int iValue = -(m_windResistance.m_sCorrectValue * _fValuePercent );
		m_windResistance.dispell();
		m_windResistance.correctValue(iValue, _iKeepTime);
	}	
	if(m_lightResistance.isGood())
	{
		int iValue = -(m_lightResistance.m_sCorrectValue * _fValuePercent );
		m_lightResistance.dispell();
		m_lightResistance.correctValue(iValue, _iKeepTime);
	}
	if(m_darkResistance.isGood())
	{
		int iValue = -(m_darkResistance.m_sCorrectValue * _fValuePercent );
		m_darkResistance.dispell();
		m_darkResistance.correctValue(iValue, _iKeepTime);
	}

	return TRUE;
}