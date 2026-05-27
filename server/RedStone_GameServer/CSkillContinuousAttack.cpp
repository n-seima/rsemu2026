#include "CActiveSkill.H"
#include "cACTOR.H"
#include "CGame.H"
#include <math.h>

//
//	연속공격

int
CActiveSkill::fireContinuousAttack()
{
	int	iShotCount	=	m_ability.getContinuousShotCount();

	m_lpCaster->action(m_lpSkill->m_wAction);
	m_lpCaster->setActionSpeed(&m_ability);
	m_lpCaster->m_attackInfo.m_ability.copy(&m_ability);

	int	iAttackSpeed	=	m_ability.getAttackSpeed();

	if	(iAttackSpeed)
	{
		m_lpCaster->m_iFPS	=	m_lpCaster->m_iFrameCount*iShotCount*dSYNC_FPS*100/iAttackSpeed/dCORRECT_ACTION_SPEED;
		m_lpCaster->m_iFPS	=	min(m_lpCaster->m_iFPS,dSYNC_FPS*2);

		if	(iAttackSpeed	==	3)
			m_lpCaster->m_iFPS	=	16;
	}

	setContinuousAttackActionTime(dSYNC_FPS*m_lpCaster->m_iFrameCount*iShotCount/m_lpCaster->m_iFPS);
	setContinuousAttackCount(iShotCount);
	resetContinuousAttackShotCount();
	resetContinuousAttackGetCPStatus();	//	CP 아직 안 얻었다!!

	int	iPhysicalDamage	=	0,iMagicDamage	=	0,iMissCount	=	0,iCriticalCount=0,iDoubleCriticalCount=0;
	int	iMinPhysicalDamage,iMinMagicDamage,iBlockingCount,iInstanceKillCount,iNoActionBlockCount,iDancingBlockCount;
	int	iHardBlowCount = 0;
	m_lpCaster->getContinuoueAttackDamage(&m_ability,m_lpTarget,iShotCount,&iPhysicalDamage,&iMagicDamage,&iMissCount,&iCriticalCount,&iDoubleCriticalCount,
		&iMinPhysicalDamage,&iMinMagicDamage,&iBlockingCount,&iInstanceKillCount,&iNoActionBlockCount,&iDancingBlockCount, &iHardBlowCount);

	setContinuousAttackBlockingCount(iBlockingCount);
	setContinuousAttackMinPhysicalDamage(iMinPhysicalDamage);
	setContinuousAttackMinMagicDamage(iMinMagicDamage);
	setContinuousAttackDamageSumOfPhysicalDamage(iPhysicalDamage);
	setContinuousAttackDamageSumOfMagicDamage(iMagicDamage);
	setContinuousAttackMissCount(iMissCount);
	setContinuousAttackCriticalCount(iCriticalCount);
	setContinuousAttackDoubleCriticalCount(iDoubleCriticalCount);
	setContinuousAttackInstanceKillCount(iInstanceKillCount);
	setContinuousAttackNoActionBlockCount(iNoActionBlockCount);
	setContinuousAttackDancingBlockCount(iDancingBlockCount);
	setContinuousAttackHardBlowCount(iHardBlowCount);
	m_lpCaster->action(m_lpSkill->m_wAction,TRUE);

	m_lpCaster->m_wRemainActionTime	=	getContinuousAttackActionTime()/dREVISE_ACTION_SPEED_FOR_TEST;

	m_wFrame				=	0;

	{
		int	iFrame					=	m_lpCaster->getTriggerFrame();
		
		m_dwBookedActionTime		=	g_dwCurrentTime+(iFrame*1000/m_lpCaster->m_iFPS/dREVISE_ACTION_SPEED_FOR_TEST);
		m_lpCaster->setActionFinishTime(g_dwCurrentTime+m_lpCaster->m_iFrameCount*1000/m_lpCaster->m_iFPS);
		m_dwBookedActionFinishTime	=	m_lpCaster->m_dwBookedActionFinishTime;

		BOOL	bIsMagicMissile		= FALSE;
		if(m_lpSkill)
		{
			bIsMagicMissile		=	m_lpSkill->m_aValue[3];
		}
		if(bIsMagicMissile		)
		{
			int iRange	=	GetOvalRange(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,m_lpTarget->m_pos.x,m_lpTarget->m_pos.y);
			iRange	=	sqrt(iRange);
			float fAcc	=	m_lpSkill->m_aValue[6]/100.0;
			float fSpeed	=	m_lpSkill->m_wSpeed;
			
			float fTestFrame	=	0;
			float iFrame=0;

			for(iFrame=0;iFrame<dSYNC_FPS*4;++iFrame)
			{
				fTestFrame	+=	(fSpeed + fAcc*iFrame) ;
				if(	iRange<=fTestFrame)
					break;
			}
			int iAddTime	=	iFrame*1000/dSYNC_FPS * 0.64;
			int iAddFinishTime		=	m_dwBookedActionFinishTime - m_dwBookedActionTime;
			m_dwBookedActionTime		=	g_dwCurrentTime+iAddTime;
			m_lpCaster->setActionFinishTime(g_dwCurrentTime+iAddTime+iAddFinishTime);
			m_dwBookedActionFinishTime	=	m_lpCaster->m_dwBookedActionFinishTime;
		}
	}

	return	eSKILL_FIRE_RESULT_OK;
}


//
//	연속공격 스킬 업데이트
BOOL
CActiveSkill::updateContinuousAttack()
{
	if	(m_lpCaster->m_wIsActionFinished)
	{
		if	(increaseContinuousAttackShotCount())
			return	TRUE;

		m_lpCaster->action(m_lpSkill->m_wAction,TRUE);

		int	iFrame					=	m_lpCaster->getTriggerFrame();

		m_dwBookedActionTime		=	g_dwCurrentTime+(iFrame*1000/m_lpCaster->m_iFPS/dREVISE_ACTION_SPEED_FOR_TEST);
		m_lpCaster->setActionFinishTime(m_dwBookedActionFinishTime+(m_lpCaster->m_iFrameCount*1000/m_lpCaster->m_iFPS)/dREVISE_ACTION_SPEED_FOR_TEST);
		m_dwBookedActionFinishTime	=	m_lpCaster->m_dwBookedActionFinishTime;
	}

	if	(m_dwBookedActionTime && g_dwCurrentTime	>=	m_dwBookedActionTime)
	{
		CSkill* lpSkill = m_ability.getSkill();
		if(lpSkill && lpSkill->m_bf1IsRushAttack)
		{
			
			int iCurRange	=	lpSkill->m_aValue[0];
			iCurRange	*=	-1;
			m_lpCaster->m_wDirect			=	m_lpCaster->getDirect(m_lpTarget->m_pos.x, m_lpTarget->m_pos.y,TRUE);
			
			
			int			iAngle		=	g_protractor.getAngleToTarget(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,m_lpTarget->m_pos.x,m_lpTarget->m_pos.y);
			
			iAngle	=	(iAngle+180)%360;

			m_lpCaster->thrustByAngle(iAngle,iCurRange,TRUE, TRUE);
			m_lpTarget->thrustByAngle(iAngle,iCurRange,TRUE, FALSE);

		}
		int			iShotIndex	=	getContinuousAttackShotCount();
		CHitInfo	*lpHitInfo	=	&m_lpCaster->m_attackInfo.m_aInfo[iShotIndex];

		if	(getContinuousAttackInstanceKillCount())
		{
			lpHitInfo->m_dwResultField	=	0;
			lpHitInfo->setHit();
			lpHitInfo->setInstanceKill();
		}

		if	(iShotIndex	==	0)
			m_lpCaster->strike(m_lpTarget,lpHitInfo,&m_ability,TRUE);
		else
			m_lpCaster->strike(m_lpTarget,lpHitInfo,&m_ability,FALSE);

		if	((getContinuousAttackDamageSumOfPhysicalDamage()+getContinuousAttackDamageSumOfMagicDamage())	&&	
			getContinuousAttackGetCPStatus()==FALSE)
		{
			setContinuousAttackGetCPStatus();

			m_lpCaster->increaseCP(m_ability.getGetCP());
			m_lpCaster->syncPlayerBasicStatus(TRUE);
		}

		m_dwBookedActionTime	=	0;


	}

	return	FALSE;
}