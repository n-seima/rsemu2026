#include "CSkill.H"
#include "cMESSAGE.H"
#include "CHero.H"

DWORD	l_aSkilExpTable[5]=	{0,1,5,20,45};

//
//	레벨 업까지 남은 경험치
int
cAbility::getRemainExpForLevelUp()
{
	CSkill		*lpSkill	=	getSkill();

	if	(m_wLevel	==	0)
		return	l_aSkilExpTable[lpSkill->m_wDifficultyLevel-1]+lpSkill->m_wDifficultyLevel;

	return	lpSkill->m_wDifficultyLevel*(m_wLevel+1);
}

cSTRING	cAbility::s_strTooltip;


BOOL
cAbility::isAidAttackSkill()
{
	for (int i=0;;i++)
	{
		CSkillExtraEffect	*lpEffect	=	getAdditionalDamageInfo(i);

		if	(!lpEffect)
			break;

		return	TRUE;
	}

	return	FALSE;
}

//
//	사용 가능한 스킬이냐?
int
cAbility::isUseAble(int _iCP,BOOL _isEquipWeapon,BOOL _isEquipWeapon2,BOOL _isEquipShield,int _iBulletCount,int _iPetCount,int _iSummonBeastCount)
{
	if	(m_wLevel		==	0)
		return	eUSR_FAULT_SKILL;

	CSkill	*lpSkill	=	getSkill();

	if	(!lpSkill)
		return	eUSR_FAULT_SKILL;

	if	(g_bIsTestServer	==	FALSE	&&	lpSkill->m_bf1IsTestSkill)
		return	eUSR_VALID_SKILL;

	if	(lpSkill->isRequireWeapon())
		if	(!_isEquipWeapon)
			return	eUSR_WEAPON_NOT_FOUND;
	if	(lpSkill->isRequireOtherJobWeapon())
		if	(!_isEquipWeapon2)
			return	eUSR_WEAPON_NOT_FOUND;
	if	(lpSkill->isRequireShield())
		if	(!_isEquipShield)
			return	eUSR_SHIELD_NOT_FOUND;
	if	(lpSkill->isRequireBullet())	
	{
		int	iShotCount	=	getContinuousShotCount();
		iShotCount		=	max(iShotCount,1);

		if	(_iBulletCount	<	iShotCount)
			return	eUSR_BULLET_NOT_FOUND;
	}

	if	(lpSkill->isRequirePet())
		if	(_iPetCount==0)
			return	eUSR_PET_NOT_FOUND;
	if	(lpSkill->isRequireSummonBeast())
		if	(_iSummonBeastCount==0)
			return	eUSR_SUMMON_BEAST_NOT_FOUND;
	if	(lpSkill->isRequirePetOrBeast())
		if	(_iPetCount+_iSummonBeastCount==0)
			return	eUSR_SUMMON_BEAST_NOT_FOUND;

	if	(getSpentCP()==	0)
		return	eUSR_SUCCESS;

	if	(lpSkill->m_bf1IsToggleSkill)
		return	eUSR_SUCCESS;

	if	(_iCP	>=	c_aMinimumCP[lpSkill->m_wDifficultyLevel-1])
		return	eUSR_SUCCESS;

	return	eUSR_LOWER_CP;
}

//
//	이 스킬이 적중했을때 얻는 CP
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
cAbility::getAttackRange(cItem *_lpWeapon,int _iDefaultRange)
{
	int	iRange	=	0;

	CSkill	*lpSkill	=	getSkill();

	if (!lpSkill)
		return	iRange;

	if (lpSkill->m_wType	==	eSKILL_TYPE_WHIRLWIND)
		return	getHitRange(_lpWeapon);

	if (lpSkill->m_wShootRange	==	0	&&	lpSkill->m_wShootRangePerLevel	==	0)
	{
		if (_lpWeapon)
		{
			int	iCorrectRange	=	(lpSkill->m_wWeaponShootRangeCorrect+lpSkill->m_wWeaponShootRangeCorrectPerLevel*m_wLevel)/100;

			iRange	=	_lpWeapon->getBasicItem(TRUE)->m_wRange;
			iRange	=	iRange*iCorrectRange/100;
		}
	}
	else
		iRange	=	lpSkill->m_wShootRange + lpSkill->m_wShootRangePerLevel*m_wLevel;

	iRange		+=	_iDefaultRange;

	iRange		=	iRange*lpSkill->m_wShootRangeCorrect/100;

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

	if (!lpSkill)	return	iRange;

	if (lpSkill->m_wHitRange	==	0	&&	lpSkill->m_wHitRangePerLevel	==	0)
	{
		if (_lpWeapon)
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

//
//	명중률 보정 수치
int
cAbility::getCorrectHitChance()
{
	CSkill	*lpSkill	=	getSkill();

	return	lpSkill->m_sHitChance+lpSkill->m_sHitChancePerLevel*m_wLevel;
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
cAbility::getAttackPower(BOOL _isMin)
{
	CSkill	*lpSkill	=	getSkill();

	int	iPower	=	lpSkill->m_sAttackPoint+lpSkill->m_sAttackPointPerLevel*m_wLevel;

	if (lpSkill->m_sAttackPointRangePerLevel)
	{
		int	iRange	=	lpSkill->m_sAttackPointRangePerLevel*m_wLevel;

		if (_isMin)
			iPower	-=	iRange;
		else
			iPower	+=	iRange;
	}

	return	iPower;
}

//
//	공격력 퍼센티지
int
cAbility::getAttackPowerPercentage()
{
	CSkill	*lpSkill	=	getSkill();

	int	iPercentage		=	lpSkill->m_sCorrectAttackPoint+lpSkill->m_sAttackPercentage+lpSkill->m_sAttackPercentagePerLevel*m_wLevel;

	if (lpSkill->m_sAttackPercentage+lpSkill->m_sAttackPercentagePerLevel	==	0)
	{
		if(lpSkill->m_wWideAreaAttackDamagePercent	!=	0xffff)
			iPercentage		=	lpSkill->m_sCorrectAttackPoint+lpSkill->m_wWideAreaAttackDamagePercent+lpSkill->m_wWideAreaAttackDamagePercentPerLevel*m_wLevel;
	}

	return	iPercentage/100;
}

//
//	퀵 캐스트가 가능한 스킬이냐?
BOOL
cAbility::isQuickCastSkill()
{
	if (getSkill()->m_dwTargetMethod&eSKILL_CAST_QUICK)	return	TRUE;

	return	FALSE;
}

//
//	적한테 쓰는 스킬이냐?
BOOL
cAbility::isCastEnemySkill()
{
	if (getSkill()->m_dwTargetMethod&eSKILL_CAST_AT_ENEMY)	return	TRUE;

	return	FALSE;
}

//
//	플레이어 한테 쓰는 스킬이냐?
BOOL
cAbility::isCastPlayerSkill()
{
	if (getSkill()->m_dwTargetMethod&eSKILL_CAST_AT_PLAYER)	return	TRUE;

	return	FALSE;
}

//
//	필드에 사용하는 스킬이냐?
BOOL
cAbility::isCastGroundSkill()
{
	if (getSkill()->m_dwTargetMethod&eSKILL_CAST_AT_GROUND)	return	TRUE;

	return	FALSE;
}

//
//	탄수
int
cAbility::getShotCount()
{
	CSkill	*lpSkill	=	getSkill();

	if (lpSkill->m_wType	==	eSKILL_TYPE_SPREAD_ARROW)
	{
		int	iHitAngleRange	=	lpSkill->m_wHitAngleRange+lpSkill->m_wHitAngleRangePerLevel*m_wLevel;

		iHitAngleRange		/=	100;
		iHitAngleRange		/=	5;

		return	iHitAngleRange;
	}

	int	iCount	=	lpSkill->m_sShotCount+lpSkill->m_sShotCountPerLevel*m_wLevel;

	if (lpSkill->m_wShotCountDecimalMethod	==	eDECIMAL_METHOD_ROUND_OFF)	if (iCount%100>=50)	iCount+=	50;
	if (lpSkill->m_wShotCountDecimalMethod	==	eDECIMAL_METHOD_UP)			if (iCount%100 > 0)	iCount+=	100;

	return	iCount/100;
}

//
//	탄수
int
cAbility::getBulletCount()
{
	CSkill	*lpSkill	=	getSkill();

	int	iCount	=	lpSkill->m_sBulletCount+lpSkill->m_sBulletCountPerLevel*m_wLevel;

	if (lpSkill->m_wBulletDecimalMethod	==	eDECIMAL_METHOD_ROUND_OFF)	if (iCount%100>=50)	iCount+=	50;
	if (lpSkill->m_wBulletDecimalMethod	==	eDECIMAL_METHOD_UP)			if (iCount%100 > 0)	iCount+=	100;

	return	min(iCount/100,lpSkill->m_sBulletLimitCount);
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
	if	(lpSkill->m_wContiniousShotDecimalMethod==	eDECIMAL_METHOD_UP)
		if (iCount%100 > 0)	
			iCount+=	100;

	if	(_bIsCheckDefaultLimit	&&	lpSkill->m_sContiniousShotMaxCount	==	1000)
		iCount	=	min(iCount,dMAX_CONTINUOUS_HIT_INFO_COUNT*100);

	return	min(iCount/100,lpSkill->m_sContiniousShotMaxCount);
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
//	리액션 스킬이냐?
BOOL
cAbility::isReactionSkill()
{
	CSkill	*lpSkill	=	getSkill();

	if (!lpSkill)	return	FALSE;

	if (lpSkill->m_wCastMethod	==	eST_PASSIVE	||	lpSkill->m_wCastMethod	==	eST_REACTION)	return	TRUE;

	return	FALSE;
}

//
//	스킬 포인트?
int
cAbility::getSkillPoint()
{
	CSkill	*lpSkill=	getSkill();

	int	iPoint	=	l_aSkilExpTable[lpSkill->m_wDifficultyLevel-1]+lpSkill->m_wDifficultyLevel;

	for (int i=1;i<m_wLevel;i++)
		iPoint	+=	lpSkill->m_wDifficultyLevel*(i+1);

	return	iPoint;
}

//
//	언덕 위에서 쓸 수 없는 스킬
BOOL
cAbility::isCanNotActionSkillOnHill()
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