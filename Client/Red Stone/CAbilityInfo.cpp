#include "CSkill.H"
#include "CHero.H"


int
cAbility::getApplyLimitHP(int _iLevel)
{
	CSkill	*lpSkill	=	getSkill();

	if (!lpSkill)
		return	-1;

	return	(lpSkill->m_sApplyLimitHPPercentage+lpSkill->m_sApplyLimitHPPercentagePerLevel*m_wLevel)/100;
}

int
cAbility::getApplyLimitLevel(int _iLevel,int _iType)
{
	CSkill	*lpSkill	=	getSkill();

	if (!lpSkill)
		return	-1;

	switch(_iType)
	{
		case	0xffff			:
			if (lpSkill->m_sTargetLevelLimitType1	!=	0x7fff)
				return	_iLevel+m_wLevel+lpSkill->m_sTargetLevelLimitType1;
			break;
		case	eJT_UNDEAD		:
			if (lpSkill->m_sUndeadTargetLevelLimitType1	!=	0x7fff)
				return	_iLevel+m_wLevel+lpSkill->m_sUndeadTargetLevelLimitType1;
			break;
		case	eJT_HUMAN		:
			if (lpSkill->m_sHumanTargetLevelLimitType1	!=	0x7fff)
				return	_iLevel+m_wLevel+lpSkill->m_sHumanTargetLevelLimitType1;
			break;
		case	eJT_DEVIL		:
			if (lpSkill->m_sDemonTargetLevelLimitType1	!=	0x7fff)
				return	_iLevel+m_wLevel+lpSkill->m_sDemonTargetLevelLimitType1;
			break;
		case	eJT_ANIMAL		:
			if (lpSkill->m_sAnimalTargetLevelLimitType1	!=	0x7fff)
				return	_iLevel+m_wLevel+lpSkill->m_sAnimalTargetLevelLimitType1;
			break;
		case	eJT_HOLY_BEAST	:
			if (lpSkill->m_sHolyBeastTargetLevelLimitType1	!=	0x7fff)
				return	_iLevel+m_wLevel+lpSkill->m_sHolyBeastTargetLevelLimitType1;
			break;
	}

	return	-1;
}

//
//	불 대미지
int
cAbility::getFireDamage(int _iPhysicalDamage,BOOL _isMin)
{
	CSkill		*lpSkill	=	getSkill();
	LONGLONG	llDamage	=	_iPhysicalDamage;

	llDamage	=	(lpSkill->m_sFireDamagePercentage+lpSkill->m_sFireDamagePercentagePerLevel*m_wLevel)*llDamage;

//		각 마법 공격력 = 마법, 기술의 각 속성 기본 공격력 * (1 + 지식 * 0.01) + 아이템에 의한 각 속성 마법 공격력
	if (llDamage)
	{
		llDamage		/=	10000;
		llDamage		=	(LONGLONG)((LONGLONG)(llDamage+llDamage*g_hero.getIntelligence()*g_hero.getCorrectIntelligenceFactorForMagicDamage(lpSkill)));

		return	(int)llDamage;
	}

	_iPhysicalDamage		=	(lpSkill->m_sFireDamage+lpSkill->m_sFireDamagePerLevel*m_wLevel);

	if (_isMin)
	{
		_iPhysicalDamage	-=	lpSkill->m_sFireDamageRangePerLevel*m_wLevel;
		_iPhysicalDamage	-=	lpSkill->m_sFireDamageRange;
	}
	else
	{
		_iPhysicalDamage	+=	lpSkill->m_sFireDamageRangePerLevel*m_wLevel;
		_iPhysicalDamage	+=	lpSkill->m_sFireDamageRange;
	}

	_iPhysicalDamage		=	(int)(_iPhysicalDamage+_iPhysicalDamage*g_hero.getIntelligence()*g_hero.getCorrectIntelligenceFactorForMagicDamage(lpSkill));

	return	_iPhysicalDamage;
}

//
//	물 대미지
int
cAbility::getWaterDamage(int _iPhysicalDamage,BOOL _isMin)
{
	CSkill		*lpSkill	=	getSkill();
	LONGLONG	llDamage	=	_iPhysicalDamage;

	llDamage	=	(lpSkill->m_sWaterDamagePercentage+lpSkill->m_sWaterDamagePercentagePerLevel*m_wLevel)*llDamage;

//		각 마법 공격력 = 마법, 기술의 각 속성 기본 공격력 * (1 + 지식 * 0.01) + 아이템에 의한 각 속성 마법 공격력
	if (llDamage)
	{
		llDamage		/=	10000;
		llDamage		=	(LONGLONG)(llDamage+llDamage*g_hero.getIntelligence()*g_hero.getCorrectIntelligenceFactorForMagicDamage(lpSkill));

		return	(int)llDamage;
	}

	_iPhysicalDamage		=	(lpSkill->m_sWaterDamage+lpSkill->m_sWaterDamagePerLevel*m_wLevel);

	if (_isMin)
	{
		_iPhysicalDamage	-=	lpSkill->m_sWaterDamageRangePerLevel*m_wLevel;
		_iPhysicalDamage	-=	lpSkill->m_sWaterDamageRange;
	}
	else
	{
		_iPhysicalDamage	+=	lpSkill->m_sWaterDamageRangePerLevel*m_wLevel;
		_iPhysicalDamage	+=	lpSkill->m_sWaterDamageRange;
	}

	_iPhysicalDamage		=	(int)(_iPhysicalDamage+_iPhysicalDamage*g_hero.getIntelligence()*g_hero.getCorrectIntelligenceFactorForMagicDamage(lpSkill));

	return	_iPhysicalDamage;
}

//
//	바람 대미지
int
cAbility::getWindDamage(int _iPhysicalDamage,BOOL _isMin)
{
	CSkill	*lpSkill		=	getSkill();

	LONGLONG	llDamage	=	_iPhysicalDamage;

	llDamage	=	(lpSkill->m_sWindDamagePercentage+lpSkill->m_sWindDamagePercentagePerLevel*m_wLevel)*llDamage;

//		각 마법 공격력 = 마법, 기술의 각 속성 기본 공격력 * (1 + 지식 * 0.01) + 아이템에 의한 각 속성 마법 공격력
	if (llDamage)
	{
		llDamage		/=	10000;
		llDamage		=	(LONGLONG)(llDamage+llDamage*g_hero.getIntelligence()*g_hero.getCorrectIntelligenceFactorForMagicDamage(lpSkill));

		return	(int)llDamage;
	}

	_iPhysicalDamage		=	(lpSkill->m_sWindDamage+lpSkill->m_sWindDamagePerLevel*m_wLevel);

	if (_isMin)
	{
		_iPhysicalDamage	-=	lpSkill->m_sWindDamageRangePerLevel*m_wLevel;
		_iPhysicalDamage	-=	lpSkill->m_sWindDamageRange;
	}
	else
	{
		_iPhysicalDamage	+=	lpSkill->m_sWindDamageRangePerLevel*m_wLevel;
		_iPhysicalDamage	+=	lpSkill->m_sWindDamageRange;
	}

	_iPhysicalDamage		=	(int)(_iPhysicalDamage+_iPhysicalDamage*g_hero.getIntelligence()*g_hero.getCorrectIntelligenceFactorForMagicDamage(lpSkill));

	return	_iPhysicalDamage;
}

//
//	대지 대미지
int
cAbility::getEarthDamage(int _iPhysicalDamage,BOOL _isMin)
{
	CSkill		*lpSkill	=	getSkill();
	LONGLONG	llDamage	=	_iPhysicalDamage;

	llDamage	=	(lpSkill->m_sEarthDamagePercentage+lpSkill->m_sEarthDamagePercentagePerLevel*m_wLevel)*llDamage;

//		각 마법 공격력 = 마법, 기술의 각 속성 기본 공격력 * (1 + 지식 * 0.01) + 아이템에 의한 각 속성 마법 공격력
	if (llDamage)
	{
		llDamage		/=	10000;
		llDamage		=	(LONGLONG)(llDamage+llDamage*g_hero.getIntelligence()*g_hero.getCorrectIntelligenceFactorForMagicDamage(lpSkill));

		return	(int)llDamage;
	}

	_iPhysicalDamage		=	(lpSkill->m_sEarthDamage+lpSkill->m_sEarthDamagePerLevel*m_wLevel);

	if (_isMin)
	{
		_iPhysicalDamage	-=	lpSkill->m_sEarthDamageRangePerLevel*m_wLevel;
		_iPhysicalDamage	-=	lpSkill->m_sEarthDamageRange;
	}
	else
	{
		_iPhysicalDamage	+=	lpSkill->m_sEarthDamageRangePerLevel*m_wLevel;
		_iPhysicalDamage	+=	lpSkill->m_sEarthDamageRange;
	}

	_iPhysicalDamage		=	(int)(_iPhysicalDamage+_iPhysicalDamage*g_hero.getIntelligence()*g_hero.getCorrectIntelligenceFactorForMagicDamage(lpSkill));

	return	_iPhysicalDamage;
}

//
//	빛 대미지
int
cAbility::getLightDamage(int _iPhysicalDamage,BOOL _isMin)
{
	CSkill		*lpSkill	=	getSkill();
	LONGLONG	llDamage	=	_iPhysicalDamage;

	llDamage	=	(lpSkill->m_sLightDamagePercentage+lpSkill->m_sLightDamagePercentagePerLevel*m_wLevel)*llDamage;

//		각 마법 공격력 = 마법, 기술의 각 속성 기본 공격력 * (1 + 지식 * 0.01) + 아이템에 의한 각 속성 마법 공격력
	if (llDamage)
	{
		llDamage		/=	10000;
		llDamage		=	(LONGLONG)(llDamage+llDamage*g_hero.getIntelligence()*g_hero.getCorrectIntelligenceFactorForMagicDamage(lpSkill));

		return	(int)llDamage;
	}

	_iPhysicalDamage		=	(lpSkill->m_sLightDamage+lpSkill->m_sLightDamagePerLevel*m_wLevel);

	if (_isMin)
	{
		_iPhysicalDamage	-=	lpSkill->m_sLightDamageRangePerLevel*m_wLevel;
		_iPhysicalDamage	-=	lpSkill->m_sLightDamageRange;
	}
	else
	{
		_iPhysicalDamage	+=	lpSkill->m_sLightDamageRangePerLevel*m_wLevel;
		_iPhysicalDamage	+=	lpSkill->m_sLightDamageRange;
	}

	_iPhysicalDamage		=	(int)(_iPhysicalDamage+_iPhysicalDamage*g_hero.getIntelligence()*g_hero.getCorrectIntelligenceFactorForMagicDamage(lpSkill));

	return	_iPhysicalDamage;
}

//
//	어둠 대미지
int
cAbility::getDarkDamage(int _iPhysicalDamage,BOOL _isMin)
{
	CSkill		*lpSkill	=	getSkill();
	LONGLONG	llDamage	=	_iPhysicalDamage;

	llDamage	=	(lpSkill->m_sDarkDamagePercentage+lpSkill->m_sDarkDamagePercentagePerLevel*m_wLevel)*llDamage;

//		각 마법 공격력 = 마법, 기술의 각 속성 기본 공격력 * (1 + 지식 * 0.01) + 아이템에 의한 각 속성 마법 공격력
	if (llDamage)
	{
		llDamage		/=	10000;
		llDamage		=	(LONGLONG)(llDamage+llDamage*g_hero.getIntelligence()*g_hero.getCorrectIntelligenceFactorForMagicDamage(lpSkill));

		return	(int)llDamage;
	}

	_iPhysicalDamage		=	(lpSkill->m_sDarkDamage+lpSkill->m_sDarkDamagePerLevel*m_wLevel);

	if (_isMin)
	{
		_iPhysicalDamage	-=	lpSkill->m_sDarkDamageRangePerLevel*m_wLevel;
		_iPhysicalDamage	-=	lpSkill->m_sDarkDamageRange;
	}
	else
	{
		_iPhysicalDamage	+=	lpSkill->m_sDarkDamageRangePerLevel*m_wLevel;
		_iPhysicalDamage	+=	lpSkill->m_sDarkDamageRange;
	}

	_iPhysicalDamage		=	(int)(_iPhysicalDamage+_iPhysicalDamage*g_hero.getIntelligence()*g_hero.getCorrectIntelligenceFactorForMagicDamage(lpSkill));

	return	_iPhysicalDamage;
}

//
//	근접 대미지 입히는 공격이냐?
BOOL
cAbility::isCloseRangeDamageAttack()
{
	CSkill	*lpSkill	=	getSkill();

	if (lpSkill == NULL) return	FALSE;

	if (lpSkill->m_wType	==	eSKILL_TYPE_NORMAL_HIT					)	return	TRUE;//	가장 일반적인 물리 공격
	if (lpSkill->m_wType	==	eSKILL_TYPE_CONTINUOUS_HIT_BY_CHARACTER	)	return	TRUE;//	캐릭터의 대미지 프레임을 체크해서 2대 이상 때린다.
	if (lpSkill->m_wType	==	eSKILL_TYPE_CONTINUOUS_HIT_VALUE		)	return	TRUE;//	트리거 프레임 이후에 수치에 의거해서 순차적으로 대미지를 입힌다.
	if (lpSkill->m_wType	==	eSKILL_TYPE_CONTINUOUS_HIT_BY_ILLUSION	)	return	TRUE;//	잔상에 의한 연속 타격
	if (lpSkill->m_wType	==	eSKILL_TYPE_ATTACK_BY_BUNSHIN			)	return	TRUE;//	분신에 의한 공격
	if (lpSkill->m_wType	==	eSKILL_TYPE_SCIMITAR_CUTTING			)	return	TRUE;//	시미터 커팅
	if (lpSkill->m_wType	==	eSKILL_TYPE_RUSH						)	return	TRUE;//	러쉬

	return	FALSE;
}

//
//부가효과 얻기
BOOL
cAbility::getExtraEffect(int _iIndex,CSkillExtraEffectInfo *_lpInfo)
{
	CSkill	*lpSkill	=	getSkill();

	_lpInfo->m_wEffect	=	lpSkill->m_aExtraEffect[_iIndex].m_wEffect;

	if (_lpInfo->m_wEffect	==	0xffff)
		return	FALSE;

	_lpInfo->m_sActiveChance=	(lpSkill->m_aExtraEffect[_iIndex].m_sActiveChance+lpSkill->m_aExtraEffect[_iIndex].m_sActiveChancePerLevel*m_wLevel);
	_lpInfo->m_sActiveChance=	min(_lpInfo->m_sActiveChance,lpSkill->m_aExtraEffect[_iIndex].m_wLimitActiveChance);
	_lpInfo->m_sActiveChance/=	100;

	_lpInfo->m_iUpkeepTime	=	(lpSkill->m_aExtraEffect[_iIndex].m_sUpkeepTime+lpSkill->m_aExtraEffect[_iIndex].m_sUpkeepTimePerLevel*m_wLevel);
	memcpy(_lpInfo->m_aiValue,lpSkill->m_aExtraEffect[_iIndex].m_aiValue,sizeof(_lpInfo->m_aiValue));

	return	TRUE;
}

//
//	지속시간
int
cAbility::getUpkeepTime()
{
	CSkill	*lpSkill	=	getSkill();

	int		iTime		=	(lpSkill->m_wUpkeepTime+lpSkill->m_wUpkeepTimePerLevel*m_wLevel)/100;

	return	iTime;
}

//
//	지속시간
int
cAbility::getExtraUpkeepTime(int _iIndex)
{
	CSkill	*lpSkill=	getSkill();

	if (!lpSkill || _iIndex >= dEXTRA_EFFECT_COUNT || _iIndex < 0)
		return 0;

	int		iTime;

	if (lpSkill->m_aExtraEffect[_iIndex].m_bf1IsUpkeepTimeByFrame)
			iTime	=	(lpSkill->m_aExtraEffect[_iIndex].m_sUpkeepTime+lpSkill->m_aExtraEffect[_iIndex].m_sUpkeepTimePerLevel*m_wLevel)*dSYNC_FPS/100;
	else	iTime	=	(lpSkill->m_aExtraEffect[_iIndex].m_sUpkeepTime+lpSkill->m_aExtraEffect[_iIndex].m_sUpkeepTimePerLevel*m_wLevel)/100;
	
	return	iTime;
}


CSkillExtraEffect*
cAbility::getAdditionalDamageInfo(int _iIndex)
{
	CSkill	*lpSkill	=	getSkill();

	if (!lpSkill)	return	NULL;

	int		i,iCount=0;
	BOOL	bIsOk	=	FALSE;

	for (i=0;i<dEXTRA_EFFECT_COUNT;i++)
	{
		if (lpSkill->m_aExtraEffect[i].m_wEffect == eSKILL_EE_POISON)
			bIsOk	=	TRUE;

		if (lpSkill->m_aExtraEffect[i].m_wEffect >= eSKILL_EE_ADDITIONAL_FIRE_DAMAGE && lpSkill->m_aExtraEffect[i].m_wEffect <= eSKILL_EE_ADDITIONAL_DARK_DAMAGE)
			bIsOk	=	TRUE;

		if (bIsOk)
		{
			if (iCount == _iIndex) return &lpSkill->m_aExtraEffect[i];

			iCount++;
		}
	}

	return	NULL;
}

//
//	힐 포인트
int
cAbility::getHealPoint(BOOL _IsPetResurrection , int _iLevel)
{
	CSkill	*lpSkill	=	getSkill();

	if (!lpSkill)
		return	0;

	int	iHealPoint		=	lpSkill->m_wHealPoint+lpSkill->m_wHealPointPerLevel*m_wLevel;

	if(_IsPetResurrection)
	{
		int iAddHealPointForRevive = 2 + _iLevel / 10;

		iHealPoint += iAddHealPointForRevive * 100;
	}

	if(!lpSkill->m_wHealPoint && !lpSkill->m_wHealPointPerLevel) 
		return 0;
	else
		return	iHealPoint;
}

//
//	남은 체력을 기반으로 회복할 퍼센티지
int
cAbility::getHealPercentageBasedRemainHP()
{
	CSkill	*lpSkill	=	getSkill();

	if (!lpSkill)
		return	0;

	int	iHealPoint		=	lpSkill->m_sHealPercentageBasedCurrentHP+lpSkill->m_sHealPercentageBasedCurrentHPperLevel*m_wLevel;

	g_hero.correctValueEffectDuelServer(&iHealPoint , lpSkill);

	return	iHealPoint;
}

//
//	남은 체력을 기반으로 회복할 퍼센티지
int
cAbility::getFirstAidHP()
{
	CSkill	*lpSkill	=	getSkill();

	if	(!lpSkill)
		return	0;

	for (int i=0;i<dEXTRA_EFFECT_COUNT;i++)
	{
		CSkillExtraEffectInfo	skillEE;

		if (!getExtraEffect(i,&skillEE))
			break;

		if (skillEE.m_wEffect	==	eSKILL_EE_FIRST_AID)
		{
			int		iValue1		=	skillEE.m_aiValue[0];
			int		iValue2		=	skillEE.m_aiValue[1];

			BOOL	bIsProportionToRemainHPAndHitDamage	=	skillEE.m_aiValue[4];

			if	(bIsProportionToRemainHPAndHitDamage)
				return	0;

			return	iValue1+iValue2*m_wLevel;
		}
	}

	return	0;
}

int
cAbility::getRestoreExpPercentage()
{
	CSkill	*lpSkill	=	getSkill();

	if (!lpSkill || !lpSkill->isResurrection())
		return	0;

	int	iRestoreExpPercentage	=	(short)lpSkill->m_aValue[1]+(short)lpSkill->m_aValue[0]*m_wLevel;
	iRestoreExpPercentage		=	max(iRestoreExpPercentage,0);
	iRestoreExpPercentage		=	min(iRestoreExpPercentage,100);

	return	iRestoreExpPercentage;
}

