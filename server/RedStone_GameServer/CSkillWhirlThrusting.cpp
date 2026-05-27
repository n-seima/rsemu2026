#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cGame.H"

//	
int
CActiveSkill::fireWhirlthrusting()
{
	if	(m_lpCaster->isSealdMove())
		return	eSKILL_FIRE_RESULT_FAILED;
	
	m_lpTarget->getDamage(m_lpCaster,&m_ability,0,0,&m_hitInfo);
	m_lpCaster->m_attackInfo.reset();
	m_lpCaster->m_attackInfo.m_aInfo[0].copy(&m_hitInfo);
	m_lpCaster->action(m_lpSkill->m_wAction);

	setWhirlthrustingStep(0);
	setCastingTimeByCasterDamageFrame(0);

	int	iTime	=	m_lpCaster->getDamageFrameTime(1)-m_lpCaster->getDamageFrameTime(0);

	setWhirlthrustingStrikeTime(iTime);

	return	eSKILL_FIRE_RESULT_OK;
}

//	
BOOL
CActiveSkill::updateWhirlthrusting()
{
	if(!m_lpCaster)
		return FALSE;

	m_lpCaster->setRemainActionTime(4);	
	
	if	(getWhirlthrustingStep()	==	0)
	{
		if	(isActionTime())
		{
			m_lpCaster->strikeWhirlWind(&m_ability);

			setWhirlthrustingStep(1);

			m_dwBookedActionTime	=	g_dwCurrentTime+getWhirlthrustingStrikeTime()/dREVISE_ACTION_SPEED_FOR_TEST;
			m_dwBookedActionTime	=	max(g_dwCurrentTime,m_dwBookedActionTime);
		}

		return	FALSE;
	}

	if	(getWhirlthrustingStep()	==	1)
	{
		if	(isActionTime())
		{
			if	(m_lpTarget)
				m_lpCaster->strike(m_lpTarget,&m_hitInfo,&m_ability);

			m_lpCaster->setRemainActionTime(1);	

			return	TRUE;
		}
	}

	return	FALSE;
}	