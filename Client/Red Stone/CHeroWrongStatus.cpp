#include "CHero.H"
#include "CActor.H"

//
//	상태 이상 상태
BOOL
CHero::isStrangeStatus()
{
	if (m_wBlindTime)		return	TRUE;	//	장님 상태 남은 시간
	if (m_wPoisonTime)		return	TRUE;	//	초당 데미지/10 , 중독 상태 남은 시간
	if (m_wSleepTime)		return	TRUE;	//	잠이 깰때까지 남은 시간
	if (m_wFreezeTime)		return	TRUE;	//	프리즈 상태 남은 시간
	if (m_wColdTime)		return	TRUE;	//	콜드 상태 남은 시간
	if (m_wStunTime)		return	TRUE;	//	스턴 상태 남은 시간
	if (m_wStoneTime)		return	TRUE;	//	석화 남은 시간
	if (m_wConfuseTime)		return	TRUE;	//	혼란 상태 남은 시간
	if (m_wCharmedTime)		return	TRUE;	//	차밍 상태 남은 시간
//	if (m_wNightMareTime)	return	TRUE;	//	악몽
//	if (m_wParalysisTime)	return	TRUE;	//	전신 마비

	return	FALSE;
}

//
//	능력치 저하 상태
BOOL
CHero::isDeclinePowerStatus()
{
	if (m_attackPower.isWrong()			)	return	TRUE;
	if (m_defensePower.isWrong()		)	return	TRUE;
	if (m_hitChance.isWrong()			)	return	TRUE;
	if (m_dodgeChance.isWrong()			)	return	TRUE;
	if (m_actionSpeed.isWrong()			)	return	TRUE;
	if (m_moveSpeed.isWrong()			)	return	TRUE;
	if (m_concentration.isWrong()		)	return	TRUE;
	if (m_fireResistance.isWrong()		)	return	TRUE;
	if (m_waterResistance.isWrong()		)	return	TRUE;
	if (m_windResistance.isWrong()		)	return	TRUE;
	if (m_earthResistance.isWrong()		)	return	TRUE;
	if (m_lightResistance.isWrong()		)	return	TRUE;
	if (m_darkResistance.isWrong()		)	return	TRUE;
	if (m_allMagicResistance.isWrong()	)	return	TRUE;
	if (m_strength.isWrong()			)	return	TRUE;						//	힘 
	if (m_agility.isWrong()				)	return	TRUE;						//	민첩성
	if (m_constitution.isWrong()		)	return	TRUE;					//	건강
	if (m_intelligence.isWrong()		)	return	TRUE;					//	지식
	if (m_wisdom.isWrong()				)	return	TRUE;						//	지혜
	if (m_charisma.isWrong()			)	return	TRUE;						//	카리스마
	if (m_luck.isWrong()				)	return	TRUE;							//	운

	return	FALSE;
}

//
//	저주 상태
BOOL
CHero::isCurseStatus()
{
	if	(m_wFalloffMoraleTime)	return	TRUE;	//	사기 저하 상태 남은 시간
	if	(m_wTargetedTime	)	return	TRUE;	//	타겟 설정 상태 남은 시간
	if	(m_wBreakedArmorTime)	return	TRUE;	//	갑옷 파괴 상태 남은 시간
	if	(m_wBreakedWeaponTime)	return	TRUE;	//	무기 파괴 상태 남은 시간
	if	(m_wSimpletonTime	)	return	TRUE;	//	바보-_- 상태 남은 시간
	if	(m_wBerserkTime		)	return	TRUE;	//	버서크 상태 남은 시간
	if	(m_wTortureTime		)	return	TRUE;	//	고문률-_-??,고문 시간
	if	(m_wBloodDonorTime	)	return	TRUE;	//	피흡혈률,피흡혈 당하는 시간
//	if	(m_wContractTime	)	return	TRUE;	//	계약
	if	(m_level.isWrong()	)	return	TRUE;	//	계약
	if	(m_wReversalPowerTime)	return	TRUE;	//	계약
	if  (m_maxHP.isWrong()	)	return	TRUE;


	return	FALSE;
}

//
//	뭔가 이상한 상태
BOOL
CHero::isWrongStatus()
{
	if (isStrangeStatus())		return	TRUE;	//	상태 이상 상태
	if (isDeclinePowerStatus())	return	TRUE;	//	능력치 저하 상태
	if (isCurseStatus())		return	TRUE;	//	저주 상태

	return	FALSE;
}

//
//	이상상태 치료
void
CHero::cureStrangeStatus(int _iEfficient)
{
	m_wBlindTime	-=	m_wBlindTime*_iEfficient/100;	//	장님 상태 남은 시간
	m_wPoisonTime	-=	m_wPoisonTime*_iEfficient/100;	//	초당 데미지/10 , 중독 상태 남은 시간
	m_wSleepTime	-=	m_wSleepTime*_iEfficient/100;	//	잠이 깰때까지 남은 시간
	m_wFreezeTime	-=	m_wFreezeTime*_iEfficient/100;	//	프리즈 상태 남은 시간
	m_wColdTime		-=	m_wColdTime	*_iEfficient/100;	//	콜드 상태 남은 시간
	m_wStunTime		-=	m_wStunTime	*_iEfficient/100;	//	스턴 상태 남은 시간
	m_wStoneTime	-=	m_wStoneTime*_iEfficient/100;	//	석화 남은 시간
	m_wConfuseTime	-=	m_wConfuseTime*_iEfficient/100;	//	혼란 상태 남은 시간
	m_wCharmedTime	-=	m_wCharmedTime*_iEfficient/100;
//	m_wNightMareTime-=	m_wNightMareTime*_iEfficient/100;					//	악몽
//	m_wParalysisTime-=	m_wParalysisTime*_iEfficient/100;					//	전신 마비
}

//
//	저하계 치료
void
CHero::cureDeclinePowerStatus()
{
	m_attackPower.cure();
	m_defensePower.cure();
	m_hitChance.cure();
	m_dodgeChance.cure();
	m_actionSpeed.cure();
	m_moveSpeed.cure();
	m_concentration.cure();
	m_fireResistance.cure();
	m_waterResistance.cure();
	m_windResistance.cure();
	m_earthResistance.cure();
	m_lightResistance.cure();
	m_darkResistance.cure();
	m_allMagicResistance.cure();
	m_strength.cure();					//	힘 
	m_agility.cure();						//	민첩성
	m_constitution.cure();			//	건강
	m_intelligence.cure();			//	지식
	m_wisdom.cure();						//	지혜
	m_charisma.cure();					//	카리스마
	m_luck.cure();							//	운
}

//
//	저주 치료
void
CHero::cureCurseStatus()
{
	m_wFalloffMoraleTime=	0;	//	사기 저하 상태 남은 시간
	m_wTargetedTime		=	0;	//	타겟 설정 상태 남은 시간
	m_wBreakedArmorTime	=	0;	//	갑옷 파괴 상태 남은 시간
	m_wBreakedWeaponTime=	0;	//	무기 파괴 상태 남은 시간
	m_wSimpletonTime	=	0;	//	바보-_- 상태 남은 시간
	m_wBerserkTime		=	0;	//	버서크 상태 남은 시간
	m_wTortureTime		=	0;	//	고문률-_-??,고문 시간
	m_wBloodDonorTime	=	0;	//	피흡혈률,피흡혈 당하는 시간
	m_wReversalPowerTime=	0;

	m_maxHP.cure();
	m_level.cure();
}

//
//	상태이상 치료
void
CHero::cureWrongStatus()
{
	cureStrangeStatus();
	cureDeclinePowerStatus();
	cureCurseStatus();
}

//
//	부활
void
CHero::revive()
{
	if	(!g_lpHero->isDeath())
		return;

	m_iHP			=	1;
}

//
//	스킬에 의한 치료
BOOL
CHero::cure(int _iSkill,int _iHealHP)
{
	CSkill *lpSkill		=	&g_aSkill[_iSkill];

	BOOL	bIsEffected	=	FALSE;

	if (lpSkill->isCureStrangeStatus())
	{
		bIsEffected		=	TRUE;
		cureStrangeStatus();
	}
	if (lpSkill->isCureDeclinePowerStatus())
	{
		bIsEffected		=	TRUE;
		cureDeclinePowerStatus();
	}
	if (lpSkill->isCureCurseStatus())
	{
		bIsEffected		=	TRUE;
		cureCurseStatus();
	}
	if (lpSkill->isCureAllStrangeStatus())
	{
		bIsEffected		=	TRUE;
		cureWrongStatus();
	}
	if (lpSkill->isResurrection())
	{
		bIsEffected		=	TRUE;
		revive();
	}

	if (_iHealHP > 0 && m_iHP < getMaxHP())
		bIsEffected = TRUE;

	if (lpSkill->m_bf1IsInstanceHeal)
	{
		m_iHP	+=	_iHealHP;
		m_iHP	=	min(g_hero.m_iHP,g_hero.getMaxHP());
	}
	else
	{
		if(m_wInstanceHealEffectTime)
		{
			int iInstanceHealHP	=	_iHealHP * m_wInstanceHealEffectPercent/10000;
			iInstanceHealHP	=	min(iInstanceHealHP,_iHealHP);
			m_iHP	+=	iInstanceHealHP;
			m_iHP	=	min(g_hero.m_iHP,g_hero.getMaxHP());
			_iHealHP  = max(_iHealHP - iInstanceHealHP,0);
		}
		healHP(_iHealHP);
	}

	return	bIsEffected;
}


//
//	대미지 흡수
BOOL
CHero::absorbDamage(int _iHealHP)
{
	BOOL	bIsEffected	=	FALSE;

	if (_iHealHP > 0 && m_iHP < getMaxHP())
		bIsEffected = TRUE;

	healHP(_iHealHP);

	return	bIsEffected;
}