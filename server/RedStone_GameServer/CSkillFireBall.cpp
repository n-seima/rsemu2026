#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cGAME.H"
#include <math.h>

//
//	파이어볼 발사
int
CActiveSkill::fireFireBallTypeSkill()
{
	int	iShotCount		=	m_ability.getContinuousShotCount();
	int	iPhysicalDamage	=	0,iMagicDamage	=	0;

	CHitInfo	hitInfo;

	int	iTotalActionTime	=	0;
	int	iCorrectAttackSpeed	=	m_ability.getCorrectAttackSpeed()-100;
	int	iActionTime			=	m_ability.getAttackSpeed();

	if (iActionTime	==	0)
		iActionTime			=	dSYNC_FPS*iActionTime/100;

	for (int i=0;i<iShotCount;i++)
	{
		m_lpTarget->getDamage(m_lpCaster,&m_ability,0,0,&hitInfo);

		iPhysicalDamage		+=	hitInfo.m_dwPhysicalDamage;
		iMagicDamage		+=	hitInfo.m_dwMagicDamage;

		iTotalActionTime	+=	iActionTime;
		iActionTime			=	iActionTime*iCorrectAttackSpeed/100;
	}

	iTotalActionTime				=	iTotalActionTime*dCORRECT_ACTION_SPEED/100;

	m_hitInfo.m_dwPhysicalDamage	=	iPhysicalDamage;
	m_hitInfo.m_dwMagicDamage		=	iMagicDamage;
	m_hitInfo.m_dwResultField		=	iTotalActionTime*100/120;

	m_lpCaster->m_attackInfo.m_aInfo[0].copy(&m_hitInfo);
	m_lpCaster->m_wRemainActionTime	=	(iTotalActionTime+dSYNC_FPS/4)/dREVISE_ACTION_SPEED_FOR_TEST;

	setFireBallPhysicalDamage(iPhysicalDamage);
	setFireBallMagicDamage(iMagicDamage);

//	액션
	{
		m_lpCaster->action(m_lpSkill->m_wAction);

		int	iTriggerFrame			=	m_lpCaster->getTriggerFrame();
		m_dwBookedActionTime		=	g_dwCurrentTime + (iTotalActionTime*1000/dSYNC_FPS+iTriggerFrame*1000/m_lpCaster->m_iFPS)/dREVISE_ACTION_SPEED_FOR_TEST;

		m_lpCaster->setActionFinishTime(m_dwBookedActionTime + (m_lpCaster->m_iFrameCount-iTriggerFrame)*1000/dSYNC_FPS);
	}

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	파이어볼 타입 스킬 캐스팅
BOOL
CActiveSkill::castFireBallTypeSkill()
{
	int	iShotCount	=	m_ability.getContinuousShotCount();
	int	iTotalDamage=	getFireBallPhysicalDamage()+getFireBallMagicDamage();

	CHitInfo	hitInfo;
	hitInfo.reset();
	hitInfo.setHit();

	hitInfo.m_dwMagicDamage	=	iTotalDamage;
	hitInfo.m_dwResultField	=	0;

	m_lpCaster->strike(m_lpTarget,&hitInfo,&m_ability);

	if (m_lpCaster)	m_lpCaster->increaseCP(m_ability.getGetCP());

	return	TRUE;
}	//	CActiveSkill::castFireBallTypeSkill()

//
//	파이어볼 타입 스킬 업데이트
BOOL
CActiveSkill::updateFireBallTypeSkill()
{
	if	(!m_lpTarget || m_lpTarget->isDeath()		)
		return	TRUE;	//	음냐.. -_-a
	if	(m_lpTarget->m_wSerialInField	==	0xffff	)	
		return	TRUE;	//	음냐.. -_-a

	if	(g_dwCurrentTime	>= m_dwBookedActionTime )
	{
	
		castFireBallTypeSkill();

		return	TRUE;
	}

	return	FALSE;
/*
	if (m_wFrameCounter	==	0xffff	)
	{
		if (m_lpCaster->m_isTriggerFrame)
		{
			m_wFrameCounter	=	0;

			m_lpCaster->m_wRemainActionTime	=	getFireBallCastTime();
		}

		return	FALSE;	//	아직 쏘기 직전이다.
	}

	if (decreaseFireBallCastTime())
	{
		castFireBallTypeSkill();
		return	TRUE;
	}

	return	FALSE;
*/
}	//	CActiveSkill::updateFireBallTypeSkill()