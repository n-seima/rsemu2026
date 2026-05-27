#include "CAbility.H"
#include "cACTOR.H"
#include "CGame.H"
#include "cItem.H"

//
//	레벨 업까지 남은 경험치
int
cAbility::getRemainExpForLevelUp()
{
	CSkill		*lpSkill	=	getSkill();

	if (m_wLevel	==	0)
		return	g_aSkilExpTable[lpSkill->m_wDifficultyLevel-1]+lpSkill->m_wDifficultyLevel;

	return	lpSkill->m_wDifficultyLevel*(m_wLevel+1);
}

//
//	스킬 포인트?
int
cAbility::getSkillPoint()
{
	CSkill	*lpSkill=	getSkill();

	int	iPoint	=	g_aSkilExpTable[lpSkill->m_wDifficultyLevel-1]+lpSkill->m_wDifficultyLevel;

	for (int i=1;i<m_wLevel;i++)
		iPoint	+=	lpSkill->m_wDifficultyLevel*(i+1);

	return	iPoint;
}

int
cAbility::getSkillRangeForAI()
{
	return	getAttackRange(NULL,0);
}

//
//	사용가능하냐?
int
cAbility::isUseAbleToObject(int _iObjectType)
{
	if (m_wLevel		==	0)
		return	eAR_NOT_LEARNED_SKILL;

	CSkill	*lpSkill	=	getSkill();

	if (lpSkill->m_bf1IsTestSkill && !g_bIsOpenedTestServer)
		return	eAR_NOT_LEARNED_SKILL;

	switch(_iObjectType)
	{
		case	eTARGET_OBJECT_TRAP			:
		case	eTARGET_OBJECT_FIELD_TRAP	:
			if	(!lpSkill->isCastOnTrap())
				return	eAR_CAN_NOT_USE_SKILL_TO_TARGET_OBJECT;
			break;
		case	eTARGET_OBJECT_ARCA			:
			if (!lpSkill->isCastOnArca())
				return	eAR_CAN_NOT_USE_SKILL_TO_TARGET_OBJECT;
			break;
		case	eTARGET_OBJECT_DOOR			:
			if (!lpSkill->isCastOnDoor())
				return	eAR_CAN_NOT_USE_SKILL_TO_TARGET_OBJECT;
			break;
	}

	return	eAR_OK;
}

int
cAbility::getCoolTime()
{
	CSkill	*lpSkill	=	getSkill();

	if	(!lpSkill)
		return	0;
	
	if	(lpSkill->m_bf1IsDuelCoolTimeSkill	&&	g_bIsDuelServer	==	FALSE)
		return	0;

	int	iCoolTime	=	lpSkill->m_wCoolTime+(short)lpSkill->m_wCoolTimePerLevel*m_wLevel;

	iCoolTime		=	max(iCoolTime,lpSkill->m_wMinimumCoolTime);

	return	iCoolTime/100;
}

int
cAbility::getCoolTimeIndex()
{
	CSkill	*lpSkill	=	getSkill();

	if	(!lpSkill)
		return	0xffff;

	return	lpSkill->m_wCoolTimeIndex;
}

//
//	던지기 가능한 물약이냐?
BOOL
cAbility::isThrowAblePotion(cItem *_lpItem)
{
//	사용가능물약수				4;		//	2
//	사용가능물약	0	1		"스몰 힐링 포션";	//	3,4
//	사용가능물약	1	16		"힐링 포션";		//	5,6
//	사용가능물약	2	32		"라지 힐링 포션";	//	7,8
//	사용가능물약	3	50		"풀 힐링 포션";		//	9,10
//	iIndex					=	3+iIndex*2;

	CSkill	*lpSkill	=	getSkill();

	if	(!lpSkill)
		return	FALSE;

	int	iPotionCount	=	lpSkill->m_aValue[2];
	int	iItemGrade		=	0xffff;

	for	(int	i=0;i<iPotionCount;i++)
	{
		int	iRequireLevel=	lpSkill->m_aValue[3+i*2];
		int	iItem		=	lpSkill->m_aValue[3+i*2+1];

		if	(_lpItem->m_wBaseItem	==	iItem)
		{
			if	(m_wLevel>=	iRequireLevel)
				return	TRUE;

			return	FALSE;
		}
	}

	return	FALSE;
}

BOOL
cAbility::isIgnoreOptionAttackSpeed()
{		// 공격속도 아이템옵션무시? 
	CSkill	*lpSkill	=	getSkill();

	if	(!lpSkill)
		return	FALSE;
	return lpSkill->m_bf1IsIgnoreOptionAttackSpeed;
}
//
//	사용 가능한 스킬이냐?
BOOL
cAbility::isUseAble(int _iCP,BOOL _isEquipWeapon,BOOL _isEquipOtherWeapon,BOOL _isEquipShield,int _iBulletCount,int _iPetCount,int _iSummonBeastCount)
{
	if	(m_wLevel		==	0)
		return	eAR_NOT_LEARNED_SKILL;

	CSkill	*lpSkill	=	getSkill();

	if	(lpSkill->m_bf1IsTestSkill && !g_bIsOpenedTestServer)
		return	eAR_NOT_LEARNED_SKILL;
	
	if	(g_bIsDuelServer	&&	lpSkill->m_bf1IsBlockInDuelField)
		return	eAR_CAN_NOT_USE_IN_DUEL_FIELD_SKILL;

	if	(lpSkill->isRequireOtherJobWeapon())	
		if	(!_isEquipOtherWeapon)
			return	eAR_REQUIRE_WEAPON;
	if	(lpSkill->isRequireWeapon())	
		if	(!_isEquipWeapon)
			return	eAR_REQUIRE_WEAPON;
	if	(lpSkill->isRequireShield())	
		if	(!_isEquipShield)
			return	eAR_REQUIRE_SHIELD;
	if	(lpSkill->isRequireBullet())	
	{
		int	iShotCount	=	getContinuousShotCount();
		iShotCount		=	max(iShotCount,1);

		if	(_iBulletCount	<	iShotCount	)	
			return	eAR_REQUIRE_BULLET;
	}
	if	(lpSkill->isRequirePet()	)	
	{
		if	(_iPetCount	==	0)
			return	eAR_REQUIRE_PET;
	}
	if	(lpSkill->isRequireSummonBeast()	)	
	{
		if	(_iSummonBeastCount	==	0)
			return	eAR_REQUIRE_SUMMON_BEAST;
	}

	if	(getSpentCP()	==	0)
		return	eAR_OK;

	if	(_iCP	>=	c_aMinimumCP[lpSkill->m_wDifficultyLevel-1])
		return	eAR_OK;

	return	eAR_LOW_CP;
}

//
//	지속시간
int
cAbility::getUpkeepTime()
{
	CSkill	*lpSkill	=	getSkill();

	if	(lpSkill->m_wUpkeepTime	==	0xffff)
		return	65535;

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

	CSkillExtraEffect	*lpEE	=	&lpSkill->m_aExtraEffect[_iIndex];

	int		iTime;
	int		iLimitTime	=	lpEE->m_wLimitUpkeepTime;

	iTime	=	(lpEE->m_sUpkeepTime+lpEE->m_sUpkeepTimePerLevel*m_wLevel);

	if	(lpEE->m_wLimitUpkeepTime	!=	0xffff)
		iTime	=	min(iTime,iLimitTime);

	if	(lpEE->m_bf1IsCalcUpkeepTimeRoundOff)
		iTime	+=	50;
//	if	(lpEE->m_bf1IsUpkeepTimeByFrame)
//		iTime	=	iTime*dSYNC_FPS/100;
//	else
//		iTime	/=	100;
	
	return	iTime;
}

//
//	사용 가능한 스킬이냐?
int
cAbility::getGetCP()
{
	CSkill	*lpSkill	=	getSkill();

	return	lpSkill->m_wGetCP	+	lpSkill->m_wGetCPPerLevel*m_wLevel;
}

//
//	공격 속도
int
cAbility::getCorrectAttackSpeed()
{
	CSkill	*lpSkill	=	getSkill();

	return	100+(lpSkill->m_sCorrectAttackSpeed+lpSkill->m_sCorrectAttackSpeedPerLevel*m_wLevel)/100;
}

//
//	공격 속도
int
cAbility::getAttackSpeed()
{
	CSkill	*lpSkill	=	getSkill();

	int	iSpeed	=	lpSkill->m_sAttackSpeed+lpSkill->m_sAttackSpeedPerLevel*m_wLevel;
	iSpeed		=	max(iSpeed,lpSkill->m_sLimitAttackSpeed);

	return	iSpeed/100;
}

//
//	사거리 구함
int
cAbility::getAttackRange(cItem *_lpWeapon,int _iDefaultAttackRange)
{
	int	iRange	=	0;

	CSkill	*lpSkill	=	getSkill();

	if	(!lpSkill)
		return	iRange;

	if	(lpSkill->m_wShootRange	==	0	&&	lpSkill->m_wShootRangePerLevel	==	0)
	{
		if (_lpWeapon)
		{
			int	iCorrectRange	=	(lpSkill->m_wWeaponShootRangeCorrect+lpSkill->m_wWeaponShootRangeCorrectPerLevel*m_wLevel)/100;

			iRange	=	_lpWeapon->getBasicItem(TRUE)->m_wRange;
			iRange	=	iRange*iCorrectRange/100;
		}
	}
	else
		iRange		=	lpSkill->m_wShootRange + lpSkill->m_wShootRangePerLevel*m_wLevel;

	iRange		+=	_iDefaultAttackRange;

	iRange		=	iRange*lpSkill->m_wShootRangeCorrect/100;

	if	(iRange	==	0)
		if	(lpSkill->m_bf1ShootRangeZeroSkill	==	FALSE)
			return	getHitRange(_lpWeapon);

	return	iRange;
}

//	최소 사거리 구함
int
cAbility::getMinimumAttackRange()
{
	int	iRange	=	0;

	CSkill	*lpSkill	=	getSkill();

	if (!lpSkill)
		return	iRange;

	return	lpSkill->m_wMinimumShootRange;
}

//
//	타격 범위를 구한다.
int
cAbility::getHitRange(cItem *_lpWeapon)
{
	int	iRange	=	0;

	CSkill	*lpSkill	=	getSkill();

	if	(!lpSkill)
		return	iRange;

	if	(lpSkill->m_wHitRange	==	0	&&	lpSkill->m_wHitRangePerLevel	==	0)
	{
		if	(_lpWeapon)
			iRange	=	_lpWeapon->getBasicItem(TRUE)->m_wRange;

		iRange	=	iRange*lpSkill->m_wWeaponHitRangeCorrect/100;
	}
	else
	{
		if	(lpSkill->m_wHitRange	==	0xffff)
			iRange	=	lpSkill->m_wHitRangePerLevel;
		else
			iRange	=	(lpSkill->m_wHitRange + lpSkill->m_wHitRangePerLevel*m_wLevel)/100;
	}

	return	iRange;
}

int
cAbility::getMinimumHitRange()
{
	int	iRange	=	0;

	CSkill	*lpSkill	=	getSkill();

	if	(!lpSkill)
		return	0;

	return	lpSkill->m_wMinimumHitRange/100;
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

	return	iHealPoint;
}

//
//	명중률 보정 수치
int
cAbility::getCorrectHitChance()
{
	CSkill	*lpSkill	=	getSkill();

	return	(lpSkill->m_sHitChance+lpSkill->m_sHitChancePerLevel*m_wLevel)/100;
}

//
//	명중률
int
cAbility::getFixHitChance()
{
	return	getSkill()->m_sFixHitChance;
}

//
//	공격력
int
cAbility::getAttackPower()
{
	CSkill	*lpSkill	=	getSkill();

	int	iPower	=	lpSkill->m_sAttackPoint+lpSkill->m_sAttackPointPerLevel*m_wLevel;

	if (lpSkill->m_sAttackPointRangePerLevel)
	{
		int	iRange	=	lpSkill->m_sAttackPointRangePerLevel*m_wLevel;

		iRange	=	largeRandom(iRange*2)-iRange;
		iPower	+=	iRange;
	}

	return	iPower;
}

//
//	공격력 퍼센티지
int
cAbility::getAttackPowerPercentage(cACTOR *_lpTarget)
{
	CSkill	*lpSkill	=	getSkill();
	int		iPercentage;

	if	(lpSkill->m_bf1IsCatchAndThrowingTypeAttackPower	&&	_lpTarget)
	{
		iPercentage		=	lpSkill->m_sCorrectAttackPoint+lpSkill->m_sAttackPercentage;
		iPercentage		=	iPercentage+_lpTarget->getRemainHPRate()*lpSkill->m_sAttackPercentagePerLevel*m_wLevel/100;
//		+40%+(적의 현재HP/적의 최대HP)*10Lv%
	}
	else
		iPercentage		=	lpSkill->m_sCorrectAttackPoint+lpSkill->m_sAttackPercentage+lpSkill->m_sAttackPercentagePerLevel*m_wLevel;

	return	iPercentage/100;
}

int
cAbility::getStrikePeriod()
{
	CSkill	*lpSkill	=	getSkill();

	int	iPeriod	=	lpSkill->m_wStrikePeriod+lpSkill->m_sStrikePeriodPerLevel*m_wLevel;

	iPeriod		=	max(iPeriod,lpSkill->m_wMinimumStrikePeriod);

	return	iPeriod;
}


//
//크리티컬 확률 구하기
int
cAbility::getCriticalChance()
{
	CSkill	*lpSkill	=	getSkill();

	return	(lpSkill->m_sCriticalChance+lpSkill->m_sCriticalChancePerLevel*m_wLevel);
}

int
cAbility::getCriticalToUndead()
{
	CSkill	*lpSkill	=	getSkill();

	return	(lpSkill->m_sCriticalToUndead+lpSkill->m_sCriticalToUndeadPerLevel*m_wLevel)/100;
}

int
cAbility::getCriticalToHuman()
{
	CSkill	*lpSkill	=	getSkill();

	return	(lpSkill->m_sCriticalToHuman+lpSkill->m_sCriticalToHumanPerLevel*m_wLevel)/100;
}

int
cAbility::getCriticalToDemon()
{
	CSkill	*lpSkill	=	getSkill();

	return	(lpSkill->m_sCriticalToDemon+lpSkill->m_sCriticalToDemonPerLevel*m_wLevel)/100;
}
int
cAbility::getCriticalToAnimal()
{
	CSkill	*lpSkill	=	getSkill();

	return	(lpSkill->m_sCriticalToAnimal+lpSkill->m_sCriticalToAnimalPerLevel*m_wLevel)/100;
}

int
cAbility::getCriticalToHolyAnimal()
{
	CSkill	*lpSkill	=	getSkill();

	return	(lpSkill->m_sCriticalToHolyAnimal+lpSkill->m_sCriticalToHolyAnimalPerLevel*m_wLevel)/100;
}



int
cAbility::getInstanceKillToUndead()
{
	CSkill	*lpSkill	=	getSkill();

	return	(lpSkill->m_sInstanceKillToUndead+lpSkill->m_sInstanceKillToUndeadPerLevel*m_wLevel)/100;
}

int
cAbility::getInstanceKillToHuman()
{
	CSkill	*lpSkill	=	getSkill();

	return	(lpSkill->m_sInstanceKillToHuman+lpSkill->m_sInstanceKillToHumanPerLevel*m_wLevel)/100;
}

int
cAbility::getInstanceKillToDemon()
{
	CSkill	*lpSkill	=	getSkill();

	return	(lpSkill->m_sInstanceKillToDemon+lpSkill->m_sInstanceKillToDemonPerLevel*m_wLevel)/100;
}
int
cAbility::getInstanceKillToAnimal()
{
	CSkill	*lpSkill	=	getSkill();

	return	(lpSkill->m_sInstanceKillToAnimal+lpSkill->m_sInstanceKillToAnimalPerLevel*m_wLevel)/100;
}

int
cAbility::getInstanceKillToHolyAnimal()
{
	CSkill	*lpSkill	=	getSkill();

	return	(lpSkill->m_sInstanceKillToHolyAnimal+lpSkill->m_sInstanceKillToHolyAnimalPerLevel*m_wLevel)/100;
}


//
//고정된 크리티컬 확률 구하기
int
cAbility::getFixCriticalChance()
{
	CSkill	*lpSkill	=	getSkill();

	return	lpSkill->m_sFixCriticalChance;
}

//
//	결정타 찬스
int
cAbility::getCrushChance()
{
	CSkill	*lpSkill	=	getSkill();

	int	iChance			=	(lpSkill->m_sCrushChance+lpSkill->m_sCrushChancePerLevel*m_wLevel)/100;

	if	(lpSkill->m_wLimitCrushChance)
		iChance				=	min(iChance,lpSkill->m_wLimitCrushChance/100);

	return	iChance;
}

//
//고정된 결정타 확률 구하기
int
cAbility::getFixCrushChance()
{
	CSkill	*lpSkill	=	getSkill();

	return	lpSkill->m_sFixCrushChance;
}

//
//	대미지 일부를 체력으로 흡수
int
cAbility::getDamageToHP()
{
	CSkill	*lpSkill	=	getSkill();

	return	(lpSkill->m_wDamageToHP +lpSkill->m_wDamageToHPPerLevel * m_wLevel)/100;
}

//
//부가효과 얻기
BOOL
cAbility::getExtraEffect(int _iIndex,CSkillExtraEffectInfo *_lpInfo)
{
	CSkill	*lpSkill	=	getSkill();

	_lpInfo->m_wEffect	=	lpSkill->m_aExtraEffect[_iIndex].m_wEffect;

	if	(_lpInfo->m_wEffect	==	0xffff)
		return	FALSE;

	CSkillExtraEffect	*lpEE=	&lpSkill->m_aExtraEffect[_iIndex];

	int	iActiveChance		=	(lpSkill->m_aExtraEffect[_iIndex].m_sActiveChance+lpSkill->m_aExtraEffect[_iIndex].m_sActiveChancePerLevel*m_wLevel);
	iActiveChance			=	min(iActiveChance,lpSkill->m_aExtraEffect[_iIndex].m_wLimitActiveChance);
	_lpInfo->m_sActiveChance=	iActiveChance/100;

	if	(iActiveChance)
		_lpInfo->m_sActiveChance	=	max(_lpInfo->m_sActiveChance,1);

	_lpInfo->m_iUpkeepTime	=	(lpSkill->m_aExtraEffect[_iIndex].m_sUpkeepTime+lpSkill->m_aExtraEffect[_iIndex].m_sUpkeepTimePerLevel*m_wLevel);
	memcpy(_lpInfo->m_aiValue,lpSkill->m_aExtraEffect[_iIndex].m_aiValue,sizeof(_lpInfo->m_aiValue));
	
	int		iLimitTime	=	lpEE->m_wLimitUpkeepTime;
	int		iTime		=	(lpEE->m_sUpkeepTime+lpEE->m_sUpkeepTimePerLevel*m_wLevel);

	if	(lpEE->m_wLimitUpkeepTime	!=	0xffff)
		iTime	=	min(iTime,iLimitTime);

	if	(lpEE->m_bf1IsCalcUpkeepTimeRoundOff)
		iTime	+=	50;

	_lpInfo->m_iUpkeepTime	=	iTime;
	
	return	TRUE;
}

//
//	기본 공격 스킬이냐?
BOOL
cAbility::isBasicAttackSkill()
{
	CSkill	*lpSkill	=	getSkill();

	if (!lpSkill)	return	FALSE;

	if (lpSkill->m_wType	==	eSKILL_TYPE_NORMAL_HIT	||	lpSkill->m_wType	==	eSKILL_TYPE_MISSILE		)	return	TRUE;

	return	FALSE;
}

//
//즉사 확률
int
cAbility::getInstanceKillChance()
{
	CSkill	*lpSkill	=	getSkill();

	if (!lpSkill)	return	0;

	return	(lpSkill->m_sInstanceKillChance+lpSkill->m_sInstanceKillChancePerLevel*m_wLevel)/100;
}


//
//	탄수
int
cAbility::getShotCount(BOOL _bIsCheckDefaultLimit)
{
	CSkill	*lpSkill	=	getSkill();

	int	iCount	=	lpSkill->m_sShotCount+lpSkill->m_sShotCountPerLevel*m_wLevel;

	if (lpSkill->m_wShotCountDecimalMethod	==	eDECIMAL_METHOD_ROUND_OFF)	if (iCount%100>=50)	iCount+=	50;
	if (lpSkill->m_wShotCountDecimalMethod	==	eDECIMAL_METHOD_UP)			if (iCount%100 > 0)	iCount+=	100;

	if (_bIsCheckDefaultLimit)
		iCount	=	min(iCount,dMAX_CONTINUOUS_HIT_INFO_COUNT*100);

	if (lpSkill->m_wLimitShotCount)
		return	min(iCount/100,lpSkill->m_wLimitShotCount);

	return	iCount/100;
}

int
cAbility::getBulletCount()
{
	CSkill	*lpSkill	=	getSkill();

	int	iCount	=	lpSkill->m_sBulletCount+lpSkill->m_sBulletCountPerLevel*m_wLevel;

	if	(lpSkill->m_wBulletDecimalMethod	==	eDECIMAL_METHOD_ROUND_OFF)
		if	(iCount%100>=50)
			iCount+=	50;
	if	(lpSkill->m_wBulletDecimalMethod	==	eDECIMAL_METHOD_UP)
		if	(iCount%100 > 0)
			iCount+=	100;

	if	(lpSkill->m_sBulletLimitCount)
		return	min(iCount/100,lpSkill->m_sBulletLimitCount);

	return	iCount/100;
}

//
//	탄수
int
cAbility::getContinuousShotCount(BOOL _bIsCheckDefaultLimit)
{
	CSkill	*lpSkill	=	getSkill();

	int	iCount	=	lpSkill->m_sContiniousShotCount+lpSkill->m_sContiniousShotCountPerLevel*m_wLevel;

	if	(lpSkill->m_wContiniousShotDecimalMethod	==	eDECIMAL_METHOD_ROUND_OFF)
		if (iCount%100>=50)
			iCount+=	50;
	if	(lpSkill->m_wContiniousShotDecimalMethod	==	eDECIMAL_METHOD_UP)
		if (iCount%100 > 0)
			iCount+=	100;

	if	(_bIsCheckDefaultLimit	&&	lpSkill->m_sContiniousShotMaxCount	==	1000)
		iCount	=	min(iCount,dMAX_CONTINUOUS_HIT_INFO_COUNT*100);

	return	min(iCount/100,lpSkill->m_sContiniousShotMaxCount);
}



int
cAbility::getCorrectAwakenDamage(int _iAwakenType , int _iMiniPetLevel)
{
	CSkill	*lpSkill	=	getSkill();
	
	if	(!lpSkill)
		return	0;
	
	int iAddValue			= 0;
	int iAddValuePerLevel	= 0;
	
	if(!lpSkill->m_bf1IsNormalMiniPetSkill)
	{
		if(_iAwakenType == dAWAKENTYPE_100)
		{
			iAddValue			= 	lpSkill->m_wAwakenSkillBonusDamageForBoJoSkill * 2;
			iAddValuePerLevel	=	lpSkill->m_wAwakenSkillBonusDamageForBoJoSkillPerLevel * 2;
		}
		else if(_iAwakenType == dAWAKENTYPE_50)
		{
			iAddValue			= 	lpSkill->m_wAwakenSkillBonusDamageForBoJoSkill;
			iAddValuePerLevel	=	lpSkill->m_wAwakenSkillBonusDamageForBoJoSkillPerLevel;
		}
	}
	
	return (iAddValue + iAddValuePerLevel * _iMiniPetLevel) / 100;
}

int
cAbility::getAwakenActiveChance(int _iAwakenType , int _iMiniPetLevel)
{
	CSkill	*lpSkill	=	getSkill();
	
	if	(!lpSkill)
		return	0;
	
	int iAddValue			= 0;
	int iAddValuePerLevel	= 0;
	
	if(!lpSkill->m_bf1IsNormalMiniPetSkill)
	{
		if(_iAwakenType == dAWAKENTYPE_100)
		{
			iAddValue			= 	lpSkill->m_wAwakenSkillBonusActiveChanceForBoJoSKill * 2;
			iAddValuePerLevel	=	lpSkill->m_wAwakenSkillBonusActiveChanceForBoJoSKillPerLevel * 2;
		}
		else if(_iAwakenType == dAWAKENTYPE_50)
		{
			iAddValue			= 	lpSkill->m_wAwakenSkillBonusActiveChanceForBoJoSKill;
			iAddValuePerLevel	=	lpSkill->m_wAwakenSkillBonusActiveChanceForBoJoSKillPerLevel;
		}
	}
	
	return iAddValue + iAddValuePerLevel * _iMiniPetLevel;
}

/*
int
cAbility::getAwakenDamagePercentageForAidMagic(int _iMinipetLevel , BOOL _bISAwaken100)
{
	CSkill	*lpSkill		=	getSkill();
	
	if	(!lpSkill)
		return	0;

	if(lpSkill->m_wType == eSKILL_TYPE_AID_MAGIC)
	{
		if(_bISAwaken100)
			return	lpSkill->m_wAwakenSkillBonusActiveChance * 2 *_iMinipetLevel;
		else
			return	lpSkill->m_wAwakenSkillBonusActiveChance * _iMinipetLevel;
	}

	return 0;
}
*/


int
cAbility::getFireDamagePercentage()
{
	CSkill	*lpSkill		=	getSkill();

	if	(!lpSkill)
		return	0;

	return	(lpSkill->m_sFireDamagePercentage+lpSkill->m_sFireDamagePercentagePerLevel*m_wLevel)/100;
}

int
cAbility::getWaterDamagePercentage()
{
	CSkill	*lpSkill		=	getSkill();

	if	(!lpSkill)
		return	0;

	return	(lpSkill->m_sWaterDamagePercentage+lpSkill->m_sWaterDamagePercentagePerLevel*m_wLevel)/100;
}

int
cAbility::getWindDamagePercentage()
{
	CSkill	*lpSkill		=	getSkill();

	if	(!lpSkill)
		return	0;

	return	(lpSkill->m_sWindDamagePercentage+lpSkill->m_sWindDamagePercentagePerLevel*m_wLevel)/100;
}

int
cAbility::getEarthDamagePercentage()
{
	CSkill	*lpSkill		=	getSkill();

	if	(!lpSkill)
		return	0;

	return	(lpSkill->m_sEarthDamagePercentage+lpSkill->m_sEarthDamagePercentagePerLevel*m_wLevel)/100;
}

int
cAbility::getLightDamagePercentage()
{
	CSkill	*lpSkill		=	getSkill();

	if	(!lpSkill)
		return	0;

	return	(lpSkill->m_sLightDamagePercentage+lpSkill->m_sLightDamagePercentagePerLevel*m_wLevel)/100;
}

int
cAbility::getDarkDamagePercentage()
{
	CSkill	*lpSkill		=	getSkill();

	if	(!lpSkill)
		return	0;

	return	(lpSkill->m_sDarkDamagePercentage+lpSkill->m_sDarkDamagePercentagePerLevel*m_wLevel)/100;
}



int	
cAbility::getDamageRange(int _iKind, int _iDamageRangeType)
{
	int iDamageRange	=	0;
	int	iDamagePerLevel	=	0;
	int iValue			=	0;
	CSkill*	lpSkill		=	getSkill();

	if(!lpSkill)
		return 0;

	switch(_iKind)
	{
		
	case	eAM_ADD_FIRE_DAMAGE	:
		{
			iDamageRange	=	lpSkill->m_sFireDamageRange;
			iDamagePerLevel	=	lpSkill->m_sFireDamageRangePerLevel;
			break;
		}
	case	eAM_ADD_WATER_DAMAGE:
		{
			iDamageRange	=	lpSkill->m_sWaterDamageRange;
			iDamagePerLevel	=	lpSkill->m_sWaterDamageRangePerLevel;
			break;
		}
	case	eAM_ADD_WIND_DAMAGE:
		{
			iDamageRange	=	lpSkill->m_sWindDamageRange;
			iDamagePerLevel	=	lpSkill->m_sWindDamageRangePerLevel;
			break;
		}
	case	eAM_ADD_EARTH_DAMAGE:
		{
			iDamageRange	=	lpSkill->m_sEarthDamageRange;
			iDamagePerLevel	=	lpSkill->m_sEarthDamageRangePerLevel;
			break;
		}
	case	eAM_ADD_LIGHT_DAMAGE:
		{
			iDamageRange	=	lpSkill->m_sLightDamageRange;
			iDamagePerLevel	=	lpSkill->m_sLightDamageRangePerLevel;
			break;
		}
	case	eAM_ADD_DARK_DAMAGE:
		{
			iDamageRange	=	lpSkill->m_sDarkDamageRange;
			iDamagePerLevel	=	lpSkill->m_sDarkDamageRangePerLevel;
			break;
		}
	}

	iValue	=	getDamageRange(iDamageRange, iDamagePerLevel, m_wLevel, _iDamageRangeType);

	return iValue;
}

int
cAbility::getMagicDamage(int _iKind, int _iPhysicalDamage,int _iRemainHP, int _iDamageRangeType)
{

	CSkill	*lpSkill	=	getSkill();


	if (lpSkill->m_bf1IsMagicDamageBasedRemainHP)
	{
		int		iDamage		=	getValueMagicDamagePercent(_iKind)*_iRemainHP/100;

		return	iDamage;
	}

	LONGLONG	llDamage	=	_iPhysicalDamage;

	int	iDamagePercentage	=	getValueMagicDamagePercent(_iKind);

	llDamage				=	iDamagePercentage*llDamage/100;

	if (llDamage)
		return	(int)(llDamage/100);

	_iPhysicalDamage		=	getValueMagicDamage(_iKind);

	_iPhysicalDamage		+=	getDamageRange(_iKind, _iDamageRangeType);

	return	_iPhysicalDamage;

}

int	
cAbility::getDamageRange(int _iDamageRange, int _iDamagePerLevel, int _iLevel, int _iDamageRangeType)
{

	int iValue = 0;

	switch(_iDamageRangeType)
	{

	case eDAMAGE_NORMAL:
		{
			CSkill*	lpSkill	=	getSkill();
			int iDice		=	1;
			iValue			=	0x80000001;

			if(lpSkill)
			{
				int iAddDice = lpSkill->m_bf5MagicDamageDiceCount + lpSkill->m_bf5MagicDamageDiceCountLimit*m_wLevel/100;

				if(lpSkill->m_bf5MagicDamageDiceCountLimit)
					iAddDice	=	min(lpSkill->m_bf5MagicDamageDiceCountLimit,iAddDice);

				iAddDice		=	max(0,iAddDice);
				iDice			+=	iAddDice;
			}

			for(int i=0;i<iDice;++i)
			{
				int iDamage		=	(largeRandom(_iDamagePerLevel*m_wLevel*2)-_iDamagePerLevel*_iLevel);
				iDamage			+=	largeRandom(_iDamageRange*2)-_iDamageRange;
				iValue		=	max(iValue, iDamage);
			}
			
			break;
		}
	case eDAMAGE_MIN:
		iValue		=	-(_iDamagePerLevel*_iLevel);
		iValue		+=	-_iDamageRange;
		break;
	case eDAMAGE_MAX:
		iValue		=	_iDamagePerLevel*_iLevel;
		iValue		+=	_iDamageRange;
		break;
	}

	return iValue;
}

int
cAbility::getValueMagicDamagePercent(int _iKind)
{
	CSkill	*lpSkill	=	getSkill();
	int		iValue		=	0;
	if	(!lpSkill)
		return	iValue;
	

	switch(_iKind)
	{
		
	case	eAM_ADD_FIRE_DAMAGE	:
		{
			iValue	=	(lpSkill->m_sFireDamagePercentage+lpSkill->m_sFireDamagePercentagePerLevel*m_wLevel);
			if(lpSkill->m_sFireDamagePercentageLimit)
				iValue	=	min(iValue,lpSkill->m_sFireDamagePercentageLimit);
			break;
		}
	case	eAM_ADD_WATER_DAMAGE:
		{
			iValue	=	(lpSkill->m_sWaterDamagePercentage+lpSkill->m_sWaterDamagePercentagePerLevel*m_wLevel);
			if(lpSkill->m_sWaterDamagePercentageLimit)
				iValue	=	min(iValue,lpSkill->m_sWaterDamagePercentageLimit);
			break;
		}
	case	eAM_ADD_WIND_DAMAGE:
		{
			iValue	=	(lpSkill->m_sWindDamagePercentage+lpSkill->m_sWindDamagePercentagePerLevel*m_wLevel);
			if(lpSkill->m_sWindDamagePercentageLimit)
				iValue	=	min(iValue,lpSkill->m_sWindDamagePercentageLimit);
			break;
		}
	case	eAM_ADD_EARTH_DAMAGE:
		{
			iValue	=	(lpSkill->m_sEarthDamagePercentage+lpSkill->m_sEarthDamagePercentagePerLevel*m_wLevel);
			if(lpSkill->m_sFireDamagePercentageLimit)
				iValue	=	min(iValue,lpSkill->m_sEarthDamagePercentageLimit);
			break;
		}
	case	eAM_ADD_LIGHT_DAMAGE:
		{
			iValue	=	(lpSkill->m_sLightDamagePercentage+lpSkill->m_sLightDamagePercentagePerLevel*m_wLevel);
			if(lpSkill->m_sLightDamagePercentageLimit)
				iValue	=	min(iValue,lpSkill->m_sLightDamagePercentageLimit);
			break;
		}
	case	eAM_ADD_DARK_DAMAGE:
		{
			iValue	=	(lpSkill->m_sDarkDamagePercentage+lpSkill->m_sDarkDamagePercentagePerLevel*m_wLevel);
			if(lpSkill->m_sDarkDamagePercentageLimit)
				iValue	=	min(iValue,lpSkill->m_sDarkDamagePercentageLimit);
			break;
		}

	}

	return iValue;

}

int
cAbility::getValueMagicDamage(int _iKind)
{

	CSkill	*lpSkill	=	getSkill();
	int		iValue		=	0;

	if	(!lpSkill)
		return	iValue;
	
	switch(_iKind)
	{	
	case	eAM_ADD_FIRE_DAMAGE	:
		{
			iValue	=	(lpSkill->m_sFireDamage+lpSkill->m_sFireDamagePerLevel*m_wLevel);
			break;
		}
	case	eAM_ADD_WATER_DAMAGE:
		{
			iValue	=	(lpSkill->m_sWaterDamage+lpSkill->m_sWaterDamagePerLevel*m_wLevel);
			break;
		}
	case	eAM_ADD_WIND_DAMAGE:
		{
			iValue	=	(lpSkill->m_sWindDamage+lpSkill->m_sWindDamagePerLevel*m_wLevel);
			break;
		}
	case	eAM_ADD_EARTH_DAMAGE:
		{
			iValue	=	(lpSkill->m_sEarthDamage+lpSkill->m_sEarthDamagePerLevel*m_wLevel);
			break;
		}
	case	eAM_ADD_LIGHT_DAMAGE:
		{
			iValue	=	(lpSkill->m_sLightDamage+lpSkill->m_sLightDamagePerLevel*m_wLevel);
			break;
		}
	case	eAM_ADD_DARK_DAMAGE:
		{
			iValue	=	(lpSkill->m_sDarkDamage+lpSkill->m_sDarkDamagePerLevel*m_wLevel);
			break;
		}
	}

	return iValue;

}


//	미사일 공격 스킬이다.
BOOL
cAbility::isMissileSkill()
{
	CSkill	*lpSkill	=	getSkill();

	if (lpSkill == NULL) return	FALSE;

	if (lpSkill->m_wType==	eSKILL_TYPE_MISSILE			)	return	TRUE;
	if (lpSkill->m_wType==	eSKILL_TYPE_BOOMERANG		)	return	TRUE;
	if (lpSkill->m_wType==	eSKILL_TYPE_MACHINE_GUN		)	return	TRUE;
	if (lpSkill->m_wType==	eSKILL_TYPE_MACHINE_MISSILE	)	return	TRUE;
	if (lpSkill->m_wType==	eSKILL_TYPE_SPECIAL_MISSILE	)	return	TRUE;
	if (lpSkill->m_wType==	eSKILL_TYPE_TAIL_CHASER		)	return	TRUE;
	if (lpSkill->m_wType==	eSKILL_TYPE_INTERVAL_SHOOTER)	return	TRUE;
	if (lpSkill->m_wType==	eSKILL_TYPE_WATER_FALL		)	return	TRUE;
	if (lpSkill->m_wType==	eSKILL_TYPE_PIERCING_MISSILE)	return	TRUE;
	if (lpSkill->m_wType==	eSKILL_TYPE_ILLUSION_MISSILE)	return	TRUE;
	if (lpSkill->m_wType==	eSKILL_TYPE_SPREAD_ARROW	)	return	TRUE;
	if (lpSkill->m_wType==	eSKILL_TYPE_BIT_GLIDER		)	return	TRUE;
	if (lpSkill->m_wType==	eSKILL_TYPE_CONTINUOUS_SHOOT)	return	TRUE;
	if (lpSkill->m_wType==	eSKILL_TYPE_FIRE_BALL		)	return	TRUE;
	if (lpSkill->m_wType==	eSKILL_TYPE_MULTI_MISSILE	)	return	TRUE;
	if (lpSkill->m_wType==	eSKILL_TYPE_BOUNCING_MISSILE	)	return	TRUE;

	return	FALSE;
}

//
//	근접 대미지 입히는 공격이냐?
BOOL
cAbility::isCloseRangeDamageAttack()
{
	CSkill	*lpSkill	=	getSkill();

	if	(lpSkill == NULL)
		return	FALSE;

	if	(lpSkill->m_wType	==	eSKILL_TYPE_NORMAL_HIT					)	return	TRUE;//	가장 일반적인 물리 공격
	if	(lpSkill->m_wType	==	eSKILL_TYPE_CONTINUOUS_HIT_BY_CHARACTER	)	return	TRUE;//	캐릭터의 대미지 프레임을 체크해서 2대 이상 때린다.
	if	(lpSkill->m_wType	==	eSKILL_TYPE_CONTINUOUS_HIT_VALUE		)	return	TRUE;//	트리거 프레임 이후에 수치에 의거해서 순차적으로 대미지를 입힌다.
	if	(lpSkill->m_wType	==	eSKILL_TYPE_CONTINUOUS_HIT_BY_ILLUSION	)	return	TRUE;//	잔상에 의한 연속 타격
	if	(lpSkill->m_wType	==	eSKILL_TYPE_ATTACK_BY_BUNSHIN			)	return	TRUE;//	분신에 의한 공격
	if	(lpSkill->m_wType	==	eSKILL_TYPE_SCIMITAR_CUTTING			)	return	TRUE;//	시미터 커팅
	if	(lpSkill->m_wType	==	eSKILL_TYPE_RUSH						)	return	TRUE;//	러쉬
	if	(lpSkill->m_wType	==	eSKILL_TYPE_AWESOME_FORTRESS			)	return	TRUE;//	러쉬
	if	(lpSkill->m_wType	==	eSKILL_TYPE_CONTINUOUS_HIT_OF_DOPPELGANGER			)	return	TRUE;//	
	if	(lpSkill->m_wType	==	eSKILL_TYPE_BITE_HANGING				)	return	TRUE;//	러쉬

	return	FALSE;
}

//
//	공격하는 마법 쉴드냐?
BOOL
cAbility::isAttackMagicShield()
{
	CSkill	*lpSkill	=	getSkill();

	if (lpSkill == NULL) return	FALSE;

	for (int i=0;i<dEXTRA_EFFECT_COUNT;i++)
	{
		if (lpSkill->m_aExtraEffect[i].m_wEffect == 0xffff)	break;

		if (lpSkill->m_aExtraEffect[i].m_wEffect	>=	eSKILL_EE_FIRE_DAMAGE_SHIELD	&&
			lpSkill->m_aExtraEffect[i].m_wEffect	<=	eSKILL_EE_DARK_DAMAGE_SHIELD)
			return	TRUE;
	}

	return	FALSE;
}	//	cAbility::isAttackMagicShield()

//
//	공격형 마법 실드 얻어오기
CSkillExtraEffect*
cAbility::getAttackMagicShieldInfo(int _iIndex)
{
	CSkillExtraEffect	*lpEffect;
	CSkill				*lpSkill	=	getSkill();

	if (lpSkill == NULL) return	FALSE;

	int	iCount	=	0;

	for (int i=0;i<dEXTRA_EFFECT_COUNT;i++)
	{
		lpEffect	=	&lpSkill->m_aExtraEffect[i];

		if (lpEffect->m_wEffect == 0xffff)	break;

		if (lpEffect->m_wEffect >= eSKILL_EE_FIRE_DAMAGE_SHIELD && lpEffect->m_wEffect <= eSKILL_EE_DARK_DAMAGE_SHIELD)
		{
			if (_iIndex	==	iCount)	return	lpEffect;

			iCount++;
		}
	}

	return	NULL;
}	//	cAbility::getAttackMagicShieldInfo(int _iIndex)

//
//	미러 타워 수치?
int
cAbility::getMirrorTowerValue()
{
	CSkill	*lpSkill	=	getSkill();

	int	iValue	=	lpSkill->m_aValue[0]+lpSkill->m_aValue[1]*m_wLevel;

	iValue		=	min(iValue,lpSkill->m_aValue[2]);

	return	iValue/100;
}

//
//	미러 타워 유효 사정거리
int
cAbility::getMirrorTowerAvailRange()
{
	CSkill	*lpSkill	=	getSkill();

	int	iValue	=	lpSkill->m_aValue[4]+lpSkill->m_aValue[5]*m_wLevel;

	return	iValue;
}

int
cAbility::getMirrorTowerTargetDamageToCasterCPPercentage()
{
	CSkill	*lpSkill	=	getSkill();

	int	iValue	=	(lpSkill->m_aValue[6]+lpSkill->m_aValue[7]*m_wLevel)/100;

	return	iValue;
}

int
cAbility::getUnlockDoorLevel()
{
	CSkill	*lpSkill	=	getSkill();

	if (!lpSkill || lpSkill->m_wSkillUniqueSpecialFeature	!=	eSUSF_IS_UNLOCK_DOOR)
		return	0;

	CSkillExtraEffect	*lpEE	=	lpSkill->getExtraEffectByEffect(eSKILL_EE_UNLOCK_DOOR);

	if (!lpEE)
		return	0;

	return	(lpEE->m_aiValue[0]+lpEE->m_aiValue[1]*m_wLevel)/100;
}

int
cAbility::getUnlockArcaLevel()
{
	CSkill	*lpSkill	=	getSkill();

	if (!lpSkill || lpSkill->m_wSkillUniqueSpecialFeature	!=	eSUSF_IS_UNLOCK_ARCA)
		return	0;

	CSkillExtraEffect	*lpEE	=	lpSkill->getExtraEffectByEffect(eSKILL_EE_UNLOCK_ARCA);

	if (!lpEE)
		return	0;

	return	(lpEE->m_aiValue[0]+lpEE->m_aiValue[1]*m_wLevel)/100;
}

BOOL
cAbility::isActivatePassiveSkill(int _iCurrentCP)	//	활성화된 패시브 스킬이냐?
{
	CSkill	*lpSkill	=	getSkill();

	if	(!lpSkill)
		return	FALSE;

	if	(g_bIsDuelServer	&&	lpSkill->m_bf1IsBlockInDuelField)
		return	FALSE;

	if	(_iCurrentCP	>=	getSpentCP())
		return	TRUE;
	
	return	FALSE;
}

BOOL
cAbility::isActivateReactionSkill(int _iCPLevel)	//	활성화된 리액션 스킬이냐?
{
	CSkill	*lpSkill	=	getSkill();
	
	if	(!lpSkill)
		return	FALSE;
	
	if	(g_bIsDuelServer	&&	lpSkill->m_bf1IsBlockInDuelField)
		return	FALSE;
	
	if	(_iCPLevel	>=	getSkill()->m_wDifficultyLevel)
		return	TRUE;
	
	return	FALSE;
}