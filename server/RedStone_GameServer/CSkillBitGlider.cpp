#include <math.h>
#include "CActiveSkill.H"
#include "cACTOR.H"
#include "debugCode.H"

//
//	연사 스킬
int
CActiveSkill::fireBitGlider()
{
	int	iShotCount	=	m_ability.getContinuousShotCount();

	m_lpCaster->action(m_lpSkill->m_wAction);
	m_lpCaster->setActionSpeed(&m_ability);
	m_lpCaster->m_attackInfo.m_ability.copy(&m_ability);

	setContinuousAttackActionTime(m_lpCaster->m_wRemainActionTime);
	setContinuousAttackCount(iShotCount);
	resetContinuousAttackShotCount();
	resetContinuousAttackGetCPStatus();	//	CP 아직 안 얻었다!!

	int	iPhysicalDamage	=	0,iMagicDamage	=	0,iMissCount	=	0,iCriticalCount	=	0,iDoubleCriticalCount = 0;
	int	iMinPhysicalDamage,iMinMagicDamage,iBlockingCount,iInstanceKillCount,iNoActionBlockCount,iDancingBlockCount;
	int iHardBlowCount = 0;
	m_lpCaster->getContinuoueAttackDamage(&m_ability,m_lpTarget,iShotCount,&iPhysicalDamage,&iMagicDamage,&iMissCount,&iCriticalCount,&iDoubleCriticalCount,
		&iMinPhysicalDamage,&iMinMagicDamage,&iBlockingCount,&iInstanceKillCount,&iNoActionBlockCount,&iDancingBlockCount,&iHardBlowCount);

	setContinuousAttackBlockingCount(iBlockingCount);
	setContinuousAttackMinPhysicalDamage(iMinPhysicalDamage);
	setContinuousAttackMinMagicDamage(iMinMagicDamage);
	setContinuousAttackMissCount(iMissCount);
	setContinuousAttackDamageSumOfPhysicalDamage(iPhysicalDamage);
	setContinuousAttackDamageSumOfMagicDamage(iMagicDamage);
	setContinuousAttackCriticalCount(iCriticalCount);
	setContinuousAttackDoubleCriticalCount(iDoubleCriticalCount);
	setContinuousAttackInstanceKillCount(iInstanceKillCount);
	setContinuousAttackNoActionBlockCount(iNoActionBlockCount);
	setContinuousAttackDancingBlockCount(iDancingBlockCount);
	setContinuousAttackHardBlowCount(iHardBlowCount);
	m_lpCaster->action(m_lpSkill->m_wAction,TRUE);

	m_lpCaster->m_wRemainActionTime	=	getContinuousAttackActionTime()/dREVISE_ACTION_SPEED_FOR_TEST;

	m_wFrame		=	0;

	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}
//
//	연사 스킬 업데이트
BOOL
CActiveSkill::updateBitGlider()
{
	mDS_AS.m_iActiveSkillInnerStep	=	1;

	if (isActionTime())
	{
		mDS_AS.m_iActiveSkillInnerStep	=	2;

		CHitInfo	hitInfo;

		hitInfo.reset();

		mDS_AS.m_iActiveSkillInnerStep	=	3;

		hitInfo.m_dwPhysicalDamage	=	getContinuousAttackDamageSumOfPhysicalDamage();
		mDS_AS.m_iActiveSkillInnerStep	=	4;
		hitInfo.m_dwMagicDamage		=	getContinuousAttackDamageSumOfMagicDamage();
		mDS_AS.m_iActiveSkillInnerStep	=	5;
		hitInfo.m_dwResultField		=	0;

		if	(hitInfo.m_dwPhysicalDamage + hitInfo.m_dwMagicDamage)
		{
			mDS_AS.m_iActiveSkillInnerStep	=	6;

			hitInfo.setHit();

			mDS_AS.m_iActiveSkillInnerStep	=	7;

			if	(getContinuousAttackInstanceKillCount())
			{
				mDS_AS.m_iActiveSkillInnerStep	=	8;
				hitInfo.setInstanceKill();
			}

			mDS_AS.m_iActiveSkillInnerStep	=	9;
			m_lpCaster->strike(m_lpTarget,&hitInfo,&m_ability);
			
			if	(m_lpCaster	&&	m_lpCaster->m_wSerialInField	!=	0xffff)
			{
				mDS_AS.m_iActiveSkillInnerStep	=	10;
				m_lpCaster->increaseCP(m_ability.getGetCP());
			}
		}

		mDS_AS.m_iActiveSkillInnerStep	=	-1;

		return	TRUE;
	}
	
	mDS_AS.m_iActiveSkillInnerStep	=	-2;

	return	FALSE;
}