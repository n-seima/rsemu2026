#include "CActiveSkill.H"
#include "cACTOR.H"
#include <math.h>

//
//	미사일형 스킬 사용
int
CActiveSkill::fireBoomerang()
{
	if (m_lpTarget	==	NULL)	return	eSKILL_FIRE_RESULT_FAILED;

	m_lpTarget->getDamage(m_lpCaster,&m_ability,0,0,&m_hitInfo);
	m_lpCaster->m_attackInfo.reset();
	m_lpCaster->m_attackInfo.m_aInfo[0].copy(&m_hitInfo);
	m_lpCaster->action(m_lpSkill->m_wAction);
	
	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}	//	CActiveSkill::fireBoomerang()

//
//	캐스팅
void
CActiveSkill::castBoomerang()
{
	m_wFrameCounter	=	0;						//	바로시작

	if	(m_ability.isThrowWeapon())
		m_lpCaster->throwWeapon(m_lpTarget->m_pos.x,m_lpTarget->m_pos.y,&m_ability,FALSE);
	if	(m_ability.isJobChange())
		m_lpCaster->sendTransformation();
	if	(m_ability.isHideShield())
		m_lpCaster->m_wIsHideShield	=	TRUE;

	setBoomerangShieldTime(0);
}	//	CActiveSkill::castMissile()

//
//	미사일형 업데이트
BOOL
CActiveSkill::updateBoomerang()
{
	if (!m_lpTarget)
		return	TRUE;
	if (m_lpTarget->isDeath())
		return	TRUE;
	if (m_lpTarget->m_wSerialInField	==	0xffff)
		return	TRUE;

	if (m_wFrameCounter	==	0xffff	)
	{
		if (isActionTime())
			castBoomerang();

		return	FALSE;	//	아직 쏘기 직전이다.
	}


	if (getBoomerangShieldTime()	==	0)
	{
		if (m_lpCaster)
		{
			m_lpCaster->strike(m_lpTarget,&m_hitInfo,&m_ability);

			if (m_hitInfo.isHit())	m_lpCaster->increaseCP(m_ability.getGetCP());
		}
		else	m_lpTarget->damagedBy(NULL,NULL,m_hitInfo.m_dwPhysicalDamage,m_hitInfo.m_dwMagicDamage);
	}

	increaseBoomerangShieldTime();

	if (getBoomerangShieldTime()	>=	dSYNC_FPS)
	{
		if (m_ability.isHideShield())	m_lpCaster->m_wIsHideShield	=	FALSE;

		return	TRUE;
	}

	return	FALSE;
}	//	CActiveSkill::updateBoomerang()