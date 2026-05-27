#include <math.h>
#include "CActiveSkill.H"
#include "CActor.H"
#include "CField.H"

//
//	머신타입 스킬 사용
int
CActiveSkill::fireAlteringHitter()
{
	if	(m_lpCaster->isSealdMove())
		return	eSKILL_FIRE_RESULT_FAILED;
	
	if (m_lpTarget	==	NULL)
		return	eSKILL_FIRE_RESULT_FAILED;
	
	if (m_lpTarget->isOwnForce(m_lpCaster))
		return	eSKILL_FIRE_RESULT_FAILED;

	int	iRange		=	GetOvalRange(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y);
	int iMinRange	=	m_lpSkill->m_wMinimumShootRange;
	iMinRange		*=	iMinRange;

	if (iRange	<	iMinRange)
		return	eSKILL_FIRE_RESULT_FAILED;

	int	iShotCount	=	m_ability.getContinuousShotCount();

	m_lpCaster->action(m_lpSkill->m_wAction);
	m_lpCaster->m_attackInfo.m_ability.copy(&m_ability);

	setContinuousAttackCount(iShotCount);
	resetContinuousAttackShotCount();

	int	iPhysicalDamage	=	0,iMagicDamage	=	0,iMissCount	=	0,iCriticalCount	=	0,iDoubleCriticalCount = 0;
	int	iMinPhysicalDamage,iMinMagicDamage,iBlockingCount,iInstanceKillCount,iNoActionBlockCount,iDancingBlockCount;
	int iHardBlowCount = 0; 
	m_lpCaster->getContinuoueAttackDamage(&m_ability,m_lpTarget,iShotCount,&iPhysicalDamage,&iMagicDamage,&iMissCount,&iCriticalCount,
		&iDoubleCriticalCount,&iMinPhysicalDamage,&iMinMagicDamage,&iBlockingCount,&iInstanceKillCount,&iNoActionBlockCount,&iDancingBlockCount, &iHardBlowCount);

	setContinuousAttackBlockingCount(iBlockingCount);
	setContinuousAttackMinPhysicalDamage(iMinPhysicalDamage);
	setContinuousAttackMinMagicDamage(iMinMagicDamage);
	setContinuousAttackMissCount(iMissCount);
	setContinuousAttackDamageSumOfPhysicalDamage(iPhysicalDamage);
	setContinuousAttackDamageSumOfMagicDamage(iMagicDamage);
	setContinuousAttackCriticalCount(iCriticalCount);
	setContinuousAttackDoubleCriticalCount(iDoubleCriticalCount);
	setContinuousAttackInstanceKillCount(iInstanceKillCount);
	setAlteringHitterAttackPeriodCounter(0);
	setBeginAlteringHitterAttack(FALSE);
	setContinuousAttackNoActionBlockCount(iNoActionBlockCount);
	setContinuousAttackDancingBlockCount(iDancingBlockCount);
	setContinuousAttackHardBlowCount(iHardBlowCount);
	m_wFrame			=	0;
	m_wAngle			=	g_protractor.getAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y);

//	g_protractor.getAnglePos(&m_posTarget,(m_wAngle+180)%360,m_lpTarget->getBodySize());
	
	getActionTime();
	
	return	eSKILL_FIRE_RESULT_OK;
}

//
//
BOOL
CActiveSkill::castAlteringHitter()
{
	m_wFrameCounter	=	0;

	if	(!m_lpCaster->setActorPos(m_posTarget.x,m_posTarget.y,TRUE,TRUE))
		return	FALSE;

	int	iRange	=	GetOvalRange(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y);		//	거리다.. --;;
	iRange		=	(int)sqrt((double)iRange);

	int	iTime	=	iRange*100/m_lpSkill->m_wSpeed;
	iTime		=	iTime*dSYNC_FPS/100;

	setJumpFlyTime(iTime);

	m_lpCaster->releaseIcyStalagmite(eRUIS_RELEASE);
	m_lpCaster->setJumpFlyTime(iTime);	//	점프해서 나는중이다. 건들지 말어!!
	m_lpCaster->sendTeleport(m_ability.m_wSkill,eTM_JUMP);

	m_pos.x		=	m_posTarget.x;
	m_pos.y		=	m_posTarget.y;

	return	TRUE;
}

BOOL
CActiveSkill::updateAlteringHitter()
{
	if(!m_lpCaster)
		return FALSE;

	m_lpCaster->setRemainActionTime(2);

	if (isBegunAlteringHitterAttack())
	{
		if (decreaseAlteringHitterAttackPeriodCounter())
		{
			if (!isCorrectTarget())
			{
				m_lpCaster->setRemainActionTime(1);
				return	TRUE;
			}

			int			iShotIndex	=	getContinuousAttackShotCount();
			CHitInfo	*lpHitInfo	=	&m_lpCaster->m_attackInfo.m_aInfo[iShotIndex];

			m_lpCaster->strike(m_lpTarget,lpHitInfo,&m_ability,TRUE);

			setAlteringHitterAttackPeriodCounter(m_lpSkill->m_sContiniousShotPeriod);

			if (increaseContinuousAttackShotCount())
			{
				m_lpCaster->setRemainActionTime(1);

				return	TRUE;
			}
		}
	}

	if (m_wFrameCounter	==	0xffff)
	{
		if (isActionTime())
			castJumpAttack();

		return	FALSE;
	}

	if	(decreaseJumpFlyTime())
	{
		int	iRange	=	m_lpCaster->getWeaponAttackRange();

		if (isCorrectTarget() && m_lpTarget->isInAttackRange(m_pos.x,m_pos.y,iRange))
			m_lpCaster->strike(m_lpTarget,&m_hitInfo,&m_ability);

		m_lpCaster->increaseCP(m_ability.getGetCP());

		int	iKnockBackDistance	=	m_ability.getKnockBackDistance();

		if (iKnockBackDistance)
		{
			int	iAbilityRange	=	m_ability.getHitRange(NULL);
			int	iThrustRange	=	iAbilityRange+m_lpCaster->getBodySize();

			m_lpCaster->m_lpField->thrustActor(m_posTarget.x,m_posTarget.y,m_lpCaster,m_lpTarget,m_lpCaster->m_wActorKind,&m_ability,iThrustRange,iKnockBackDistance,m_wAngle);
		}

		if (!isCorrectTarget())
		{
			m_lpCaster->setRemainActionTime(1);
			return	TRUE;
		}

		setBeginAlteringHitterAttack(TRUE);

		CHitInfo	*lpHitInfo	=	&m_lpCaster->m_attackInfo.m_aInfo[0];

		if	(getContinuousAttackInstanceKillCount())
		{
			lpHitInfo->m_dwResultField	=	0;
			lpHitInfo->setHit();
			lpHitInfo->setInstanceKill();
		}

		m_lpCaster->strike(m_lpTarget,lpHitInfo,&m_ability,TRUE);

		setAlteringHitterAttackPeriodCounter(m_lpSkill->m_sContiniousShotPeriod);

		if	(getContinuousAttackInstanceKillCount()	||	increaseContinuousAttackShotCount())
		{
			m_lpCaster->setRemainActionTime(1);
			return	TRUE;
		}

		return	FALSE;
	}

	return	FALSE;
}