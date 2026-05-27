#include "CActiveSkill.H"
#include "cACTOR.H"
#include "CField.H"
#include <math.h>

//
//	분신 발사
int
CActiveSkill::fireBunshineAttack()
{
	int	iShotCount	=	m_ability.getContinuousShotCount();

	m_lpCaster->action(m_lpSkill->m_wAction);

	setBunshineAttackCount(iShotCount);

	int	iPhysicalDamage	=	0,iMagicDamage	=	0,iMissCount	=	0,iCriticalCount=0,iDoubleCriticalCount=0;
	int	iMinPhysicalDamage,iMinMagicDamage,iBlockingCount,iInstanceKillCount,iNoActionBlockCount,iDancingBlockCount;
	int iHardBlowCount	=	0;
	m_lpCaster->getContinuoueAttackDamage(&m_ability,m_lpTarget,iShotCount,&iPhysicalDamage,&iMagicDamage,&iMissCount,&iCriticalCount,&iDoubleCriticalCount,
		&iMinPhysicalDamage,&iMinMagicDamage,&iBlockingCount,&iInstanceKillCount,&iNoActionBlockCount,&iDancingBlockCount, &iHardBlowCount);

	setBunshineAttackBlockingCount(iBlockingCount);
	setBunshineAttackMinPhysicalDamage(iMinPhysicalDamage);
	setBunshineAttackMinMagicDamage(iMinMagicDamage);
	setBunshineAttackDamageSumOfPhysicalDamage(iPhysicalDamage);
	setBunshineAttackDamageSumOfMagicDamage(iMagicDamage);
	setBunshineAttackMissCount(iMissCount);
	setBunshineAttackCriticalCount(iCriticalCount);
	setBunshineAttackDoubleCriticalCount(iDoubleCriticalCount);
	setBunshineAttackInstanceKillCount(iInstanceKillCount);
	setBunshineAttackNoActionBlockCount(iNoActionBlockCount);
	setBunshineAttackDancingBlockCount(iDancingBlockCount);
	setBunshineAttackHardBlowCount(iHardBlowCount);
	m_wFrame		=	0;
	
	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	분신 타입 스킬 캐스팅
BOOL
CActiveSkill::castBunshineAttack()
{
	CHitInfo	hitInfo;

	hitInfo.reset();
	hitInfo.m_dwResultField	=	0;

	hitInfo.m_dwPhysicalDamage	=	getBunshineAttackDamageSumOfPhysicalDamage();
	hitInfo.m_dwMagicDamage		=	getBunshineAttackDamageSumOfMagicDamage();

	if	(getBunshineAttackInstanceKillCount())
		hitInfo.setInstanceKill();

	if	(m_lpCaster	&&	hitInfo.getDamage())
	{
		hitInfo.setHit();
		m_lpCaster->increaseCP(m_ability.getGetCP());
		m_lpCaster->strike(m_lpTarget,&hitInfo,&m_ability);
	}

	return	TRUE;
}

//
//	분신 타입 스킬 업데이트
BOOL
CActiveSkill::updateBunshineAttack()
{
	if (isActionTime())
	{
		castBunshineAttack();
		return	TRUE;
	}

	return	FALSE;
}