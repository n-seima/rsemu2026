#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cFIELD.H"
#include "CGame.H"
#include	"astMultiMissile.h"

//
//	보조마법
int
cAS_MultiMissile::fire()
{
	m_lpCaster->action(m_lpSkill->m_wAction);

	m_dwBookedActionTime		=	m_lpCaster->getFirstTriggerTime();
	m_wIsFired					=	FALSE;
	m_wIsFinished				=	FALSE;
	m_wIsStrike					=	FALSE;
	m_bIsNotSameTarget			=	m_lpSkill->m_aValue[0];
	return	eSKILL_FIRE_RESULT_OK;
}

//
//	캐스팅
BOOL
cAS_MultiMissile::cast()
{
	m_wAttackRange				=	m_ability.getHitRange(m_lpCaster->getWeapon());

	int		iShotCount			=	m_ability.getContinuousShotCount();
	cFIELD	*lpField			=	m_lpCaster->m_lpField;
	WORD	awTargetList[100];
	int		iCount				=	lpField->getInSightEnemyActorList(awTargetList,iShotCount,m_pos.x,m_pos.y,m_lpCaster,m_wAttackRange,FALSE,TRUE,&m_ability);
	int		iIndex				=	0;

	if	(iCount	<=	0)
		return	FALSE;

	for (int i=0;i<iShotCount && i < iCount;++i)
	{
		m_awTargetList[i]	=	awTargetList[i];	
	}

	m_wShotCount	=	iCount;

	m_wIsFired		=	TRUE;

	return	TRUE;
}

BOOL
cAS_MultiMissile::strike()
{
	if(m_wIsStrike)
		return FALSE;

	for(int i=0;i<m_wShotCount;++i)
	{
		cFIELD	*lpField	=	m_lpCaster->m_lpField;
		
		cACTOR	*lpTarget	=	lpField->getTestedActor(m_awTargetList[i]);
		if	(!lpTarget || lpTarget->isDeath() || lpTarget->isOwnForce(m_lpCaster))
		{
			continue;
		}
		int	iCurRange	=	GetOvalRange(lpTarget->m_pos.x,lpTarget->m_pos.y,m_lpCaster->m_pos.x,m_lpCaster->m_pos.y);
		int	iRange		=	m_wAttackRange;
		
		iRange			*=	iRange;
		
		if	(iCurRange	>	iRange)
		{
			continue;
		}
		CHitInfo	hitInfo;
		
		lpTarget->getDamage(m_lpCaster,&m_ability,m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,&hitInfo);
		
		if	(m_lpCaster->strike(lpTarget,&hitInfo,&m_ability)	&&	m_wIsGetdCP	==	FALSE)
		{
			m_wIsGetdCP	=	TRUE;
			
			m_lpCaster->increaseCP(m_ability.getGetCP());
			m_lpCaster->sendPlayerBasicInformation();
		}
		
		lpField->addSendStrikeMultiMissile(m_lpCaster,lpTarget,&hitInfo,&m_ability);

	}
	m_wIsStrike = TRUE;
	return	TRUE;
}

//
//	보조마법
BOOL
cAS_MultiMissile::update()
{
	if	(m_wIsFinished)
	{
		if	(isActionTime())
		{		
			return	TRUE;
		}

		return	FALSE;
	}

	if	(m_wIsFired)
	{
		if	(!strike())
		{
			m_dwBookedActionTime	=	g_dwCurrentTime+1000/dREVISE_ACTION_SPEED_FOR_TEST;
			m_wIsFinished			=	TRUE;
		}

		return	FALSE;
	}

	if	(!isActionTime())
		return	FALSE;

	if	(!cast())
	{
		m_dwBookedActionTime	=	g_dwCurrentTime+1000/dREVISE_ACTION_SPEED_FOR_TEST;
		m_wIsFinished			=	TRUE;
	}

	return	FALSE;
}