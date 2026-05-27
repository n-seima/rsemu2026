#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

#include "console.h"



int
cAS_WideMissile::fire()
{

	if	(!m_lpTarget || !m_lpCaster)
		return	eSKILL_FIRE_RESULT_FAILED;

	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);
	m_lpCaster->m_wIsFreezeAtTriggerFrame	=	TRUE;
	m_bIsTrigger	=	FALSE;
	m_dwCastTime	=	0;
	m_wCurrentCount	=	0;
	
	return	eSKILL_FIRE_RESULT_OK;

}



BOOL
cAS_WideMissile::update()
{

	if(!m_lpCaster || !m_lpSkill)
	{
		m_lpCaster->m_wIsFreezeAtTriggerFrame	=	FALSE;
		return TRUE;
	}


	if(!m_bIsTrigger)
	{
		if (m_lpCaster->m_isTriggerFrame)
		{
			m_bIsTrigger	= TRUE;
			m_dwCastTime	= timeGetTime();
		}
		else
			return FALSE;
	}

	DWORD	dwTime	=	timeGetTime();	

	if(m_dwCastTime <= dwTime)
	{
		m_dwCastTime			=	dwTime + m_lpSkill->m_sContiniousShotPeriod - (dwTime - m_dwCastTime);

		CHitInfo*	lpHitInfo	=	&m_lpCaster->m_attackInfo.m_aInfo[m_wCurrentCount];
		m_lpTarget	=	g_am.getTestedActor(m_awTargetList[m_wCurrentCount]);
		if(m_lpTarget)
			g_sm.castArcMissile(m_lpCaster, m_lpTarget, m_lpCaster->m_pos.x, m_lpCaster->m_pos.y, lpHitInfo, &m_ability);
		++m_wCurrentCount;
	}

	if(m_wCurrentCount >= m_wTargetCount)
	{
		m_lpCaster->m_wIsFreezeAtTriggerFrame	=	FALSE;
		return TRUE;
	}



	return FALSE;
}

