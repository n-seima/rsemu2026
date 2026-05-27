#include "CHero.H"
#include "CActor.H"
#include "console.H"

//
//	기본 공격의 공격 범위를 구한다.
int
CHero::getRangeOfDefaultAttack()
{
	cAbility	*lpAbility	=	getLeftAbility();

	if (!lpAbility)
	{
		m_iRangeOfDefaultAttack	=	0;

		return	m_iRangeOfDefaultAttack;
	}

	m_iRangeOfDefaultAttack	=	getAttackRange(lpAbility);

	return	m_iRangeOfDefaultAttack;
}

//
//	사정거리 구하기
int
CHero::getAttackRange(int _iAbility)
{
	cAbility *lpAbility	=	getAbility(_iAbility);

	if (!lpAbility	||	lpAbility->getSkill()	==	NULL)
		return	0;

	return	getAttackRange(lpAbility);
}

int
CHero::getAttackRange(cAbility *_lpAbility)
{
	if (!_lpAbility)
		return	0;

	return	_lpAbility->getAttackRange(getWeapon(),m_wAttackRange) + getValuePassiveSkill(eSKILL_PEE_INCREASE_RANGE)/100;
}
//
//	접근해서 공격한다.
void
CHero::positiveAttack(int _iTarget,int _iAbility)
{
	CActor		*lpTarget		=	&g_aActor[m_wLockedTarget];
	cAbility	*lpLockedAbility=	getLockedAbility();

	if (!lpTarget								)	{releaseEngagedTarget();return;}
	if (lpTarget->isDeath()						)	{releaseEngagedTarget();return;}
	if (isUseAbleSkill(_iAbility)!=	eUSR_SUCCESS)	return;
	if (g_lpHero->m_isExclusiveAction			)	return;

	int	iRange		=	getAttackRange(lpLockedAbility)+g_lpHero->getBodySize();
	int	iMinRange	=	lpLockedAbility->getMinimumAttackRange();

	dprint("positiveAttack");

	if (g_lpHero->isTouchAbleTarget(lpTarget,iRange,iMinRange))
		useSkillToTarget(_iTarget,_iAbility);	//	타겟이 공격거리 안에 있으면 공격한다.
	else
		moveTo(lpTarget->m_pos.x,lpTarget->m_pos.y);
}

//
//	교전중인 타겟을 공격 한다... 기 보다는 거리가 떨어져 있으면 접근한다. -_-a
void
CHero::attackEngagedTarget()
{
	cAbility	*lpAbility	=	getLeftAbility();

	if (!lpAbility)	return;

	CSkill	*lpSkill	=	lpAbility->getSkill();
	CActor	*lpTarget	=	&g_aActor[m_wEngageTarget];

	if (!lpTarget)				{releaseEngagedTarget();return;}
	if (lpTarget->isDeath())	{releaseEngagedTarget();return;}
}

BOOL
CHero::isEngageTarget(int _iSerial)
{
	if	(m_wEngageTarget	==	_iSerial)
		return	TRUE;

	return	isPetEngageTarget(_iSerial);
}

BOOL
CHero::isPetEngageTarget(int _iSerial)
{
	int	i;

	for (i=0;i<dOWN_PET_COUNT;i++)
	{
		if (m_aPet[i].m_bf10Level	==	0)
			continue;

		if (m_aPet[i].m_wEngageTarget	==	_iSerial)
			return	TRUE;
	}

	for (i=0;i<dOWN_SUMMON_BEAST_COUNT;i++)
	{
		if (m_aSummonBeast[i].m_bf10Level	==	0)
			continue;

		if (m_aSummonBeast[i].m_wEngageTarget	==	_iSerial)
			return	TRUE;
	}

	return	FALSE;
}