#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cGAME.H"
#include <math.h>

//
//	연속 타격 스킬
int
CActiveSkill::fireScimitarCutting()
{
	if	(m_lpCaster->isSealdMove())
		return	eSKILL_FIRE_RESULT_FAILED;

	int	iShotCount		=	m_ability.getContinuousShotCount();
	int	iActionTime		=	m_lpSkill->m_wSpeed*dSYNC_FPS/1000*iShotCount*dCORRECT_ACTION_SPEED/100;
	int	iCurrentAngle	=	g_protractor.getAngleToTarget(m_lpTarget->m_pos.x,m_lpTarget->m_pos.y,m_lpCaster->m_pos.x,m_lpCaster->m_pos.y);
	int	iAngleStep		=	360*1000/dSYNC_FPS/m_lpSkill->m_wSpeed;
	int	iPhysicalDamage	=	0;
	int	iMagicDamage	=	0;
	int	iAttackRange	=	m_lpCaster->getAttackRange(&m_ability)+m_lpCaster->getBodySize()+m_lpTarget->getBodySize();
	int	iRange			=	GetOvalRange(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,m_lpTarget->m_pos.x,m_lpTarget->m_pos.y);
	int	iMissCount		=	0;
	int	iCriticalCount	=	0;
	int	iDoubleCriticalCount=	0;
	int	iCrushBlowCount	=	0;
	int	iMinPhysicalDamage=	0x7fffffff;
	int	iMinMagicDamage	=	0x7fffffff;
	int	iBlockingCount	=	0;
	int	iInstanceKillCount=	0;
	int	iNoActionBlockCount	=	0;
	int	iDancingBlockCount	=	0;
	int	iHardBlowCount		=	0;

	iRange				=	(int)sqrt((double)iRange);
	m_lpCaster->m_attackInfo.m_ability.copy(&m_ability);

	for (int i=0;i<iShotCount;i++)
	{
		CHitInfo	*lpInfo	=	&m_lpCaster->m_attackInfo.m_aInfo[i];

		m_lpTarget->getDamage(m_lpCaster,&m_ability,0,0,lpInfo);

		if	(lpInfo->isHit())
		{
			if	(lpInfo->isBlock())
				iBlockingCount++;
			if	(lpInfo->isNoActionBlock())
				iNoActionBlockCount++;
			if	(lpInfo->isDancingBlockerBlock())
				iDancingBlockCount++;			
 			if	(lpInfo->isCritical())
 				iCriticalCount++;
 			if	(lpInfo->isLuckyDoubleCritical())
 				iDoubleCriticalCount++;
 			if	(lpInfo->isInstanceKill())
 				iInstanceKillCount++;
			if	(lpInfo->isHardBlow())
				iHardBlowCount++;
			lpInfo->m_dwResultField	=	0;
			lpInfo->setHit();
		}
		else
		{
			lpInfo->m_dwResultField	=	0;
			iMissCount++;
		}

// 		if	(lpInfo->isCritical())
// 			iCriticalCount++;
// 		if	(lpInfo->isLuckyDoubleCritical())
// 			iDoubleCriticalCount++;
// 		if	(lpInfo->isInstanceKill())
		// 			iInstanceKillCount++;
		
		iPhysicalDamage		+=	lpInfo->m_dwPhysicalDamage;
		iMagicDamage		+=	lpInfo->m_dwMagicDamage;

		iMinPhysicalDamage	=	min((int)lpInfo->m_dwPhysicalDamage,iMinPhysicalDamage);
		iMinMagicDamage		=	min((int)lpInfo->m_dwMagicDamage,iMinMagicDamage);	
	}

	if (m_lpSkill->m_wCircleRange)
		iAttackRange	=	m_lpSkill->m_wCircleRange;

	setScimitarCuttingCurrentRange(iRange);
	setScimitarCuttingRange(iAttackRange);

	resetScimitarCuttingGetCPStatus();
	resetScimitarCuttingRoundingCount();
	
	setScimitarCuttingAngleStep(iAngleStep);
	setScimitarCuttingAngle(0);
	setScimitarCuttingBeginAngle(iCurrentAngle);
	setScimitarCuttingRoundCount(iShotCount);
	setScimitarCuttingPhysicalDamage(iPhysicalDamage/100);
	setScimitarCuttingMagicDamage(iMagicDamage/100);
	setScimitarCuttingMissCount(iMissCount);
	setScimitarCuttingCriticalCount(iCriticalCount);
	setScimitarCuttingDoubleCriticalCount(iDoubleCriticalCount);
	setScimitarCuttingMinPhysicalDamage(iMinPhysicalDamage/100);
	setScimitarCuttingMinMagicDamage(iMinMagicDamage/100);
	setScimitarCuttingBlockingCount(iBlockingCount);
	setScimitarCuttingInstanceKillCount(iInstanceKillCount);
	setScimitarCuttingNoActionBlockCount(iNoActionBlockCount);
	setScimitarCuttingDancingBlockCount(iDancingBlockCount);
	setScimitarCuttingHardBlowCount(iHardBlowCount);

	m_lpCaster->m_wRemainActionTime	=	iActionTime/dREVISE_ACTION_SPEED_FOR_TEST;

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	캐스팅
void
CActiveSkill::castScimitarCutting()
{
	if (m_lpSkill->m_wExplosionImage==	0xffff	)
		return;

	m_wMaxFrame		=	g_aEffect[m_lpSkill->m_wExplosionImage].getFrameCount(0);
	m_wFPS			=	g_aEffect[m_lpSkill->m_wExplosionImage].getFPS(0);
	m_wFrameCounter	=	0;		//	바로시작
}

//
//	연속 타격 스킬
BOOL
CActiveSkill::updateScimitarCutting()
{
	if(!m_lpCaster)
		return FALSE;

	m_lpCaster->setRemainActionTime(4);	

	if	(!m_lpTarget			||	!m_lpCaster				)
	{
		m_lpCaster->setRemainActionTime(1);	
		return TRUE;
	}
	if	(m_lpTarget->isDeath()	||	m_lpCaster->isDeath()	)
	{
		m_lpCaster->setRemainActionTime(1);	
		return TRUE;
	}

	int	iAngle	=	getScimitarCuttingAngle();

	if	(increaseScimitarCuttingAngle())
	{
		if	(iAngle	<	90 || (iAngle < 270 && getScimitarCuttingAngle() >= 270))
		{
			int			iShotIndex	=	getScimitarCuttingRoundingCount();
			CHitInfo	*lpHitInfo	=	&m_lpCaster->m_attackInfo.m_aInfo[iShotIndex];

			if	(getScimitarCuttingInstanceKillCount())
			{
				lpHitInfo->m_dwResultField	=	0;
				lpHitInfo->setHit();
				lpHitInfo->setInstanceKill();
			}

			m_lpCaster->strike(m_lpTarget,lpHitInfo,&m_ability);

			if	(lpHitInfo->getDamage() && getScimitarCuttingGetCPStatus()==FALSE)
			{
				setScimitarCuttingGetCPStatus();
				m_lpCaster->increaseCP(m_ability.getGetCP());
			}
		}
	}

	iAngle		=	getScimitarCuttingAngle()+getScimitarCuttingBeginAngle();
	iAngle		%=	360;

	if	(getScimitarCuttingRoundingCount()	>=	getScimitarCuttingRoundCount())
		iAngle		=	getScimitarCuttingBeginAngle();

	cPOINT	pos;

	pos.Set(m_posTarget.x,m_posTarget.y);
	
	g_protractor.getAnglePos(&pos,iAngle,getScimitarCuttingCurrentRange());

	if	(!m_lpCaster)
		return	TRUE;

	if	(m_lpCaster->setActorPos(pos.x,pos.y) == FALSE)
	{
		m_lpCaster->sendBlockedMessage();
		m_lpCaster->setRemainActionTime(1);	

		return	TRUE;
	}

	if	(getScimitarCuttingCurrentRange()	<	getScimitarCuttingRange())
	{
		increaseScimitarCuttingCurrentRange(10);

		if (getScimitarCuttingCurrentRange()>	getScimitarCuttingRange())	setScimitarCuttingCurrentRange(getScimitarCuttingRange());
	}

	if	(getScimitarCuttingCurrentRange()	>	getScimitarCuttingRange())
	{
		increaseScimitarCuttingCurrentRange(-10);

		if (getScimitarCuttingCurrentRange()<	getScimitarCuttingRange())	setScimitarCuttingCurrentRange(getScimitarCuttingRange());
	}

	if	(getScimitarCuttingRoundingCount()	>=	getScimitarCuttingRoundCount())
	{
		m_lpCaster->setActorPos(m_pos.x,m_pos.y);
		m_lpCaster->setRemainActionTime(1);	

		return	TRUE;
	}

	return	FALSE;
}