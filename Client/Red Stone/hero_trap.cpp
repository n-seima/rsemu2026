#include "CHero.H"
#include "CActor.H"
#include "CMessage.h"
#include "trap.h"

BOOL
CHero::tryDisarmTrap()
{
	if	(s_iFocusTrap			==	0xffff	)
		return	FALSE;
	if	(s_isLeftButtonPressed	==	FALSE		)
		return	FALSE;

	cTrap	*lpTrap	=	g_trapManager.get(s_iFocusTrap);

	if	(!lpTrap)
		return	FALSE;

	g_trapManager.m_iTryDisarmTrap	=	s_iFocusTrap;

	int		iDisarmRange=	lpTrap->m_wRange+g_lpHero->getBodySize(FALSE)+c_iDisarmTrapAbleRange;
	int		iRange		=	GetOvalRange(lpTrap->m_pos.x,lpTrap->m_pos.y,g_lpHero->m_pos.x,g_lpHero->m_pos.y);

	if	(iRange	<=	iDisarmRange*iDisarmRange)	//	해체 가능한 거리
	{
		if	(m_iLastActionTime+dSEND_DISARM_TRAP_PERIOD	>	s_iFrameCounter	)
			return	FALSE;

		sendStop();

		s_agent.sendEtcWork(eCEW_DISARM_TRAP,g_trapManager.m_iTryDisarmTrap);

		m_iLastActionTime	=	s_iFrameCounter;
	}
	else
	{
		moveTo(lpTrap->m_pos.x,lpTrap->m_pos.y);	//	해체 가능한 거리로 이동
	}

	return	TRUE;
}