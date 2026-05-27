#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cFIELD.H"
#include "CGame.H"
#include	"astBouncingMissile.h"

//
//	보조마법
int
cAS_BouncingMissile::fire()
{
	if (m_lpCaster)
		m_lpCaster->action(m_lpSkill->m_wAction);
	
	getActionTime();
	reset();
	if	(!cast())
		return	eSKILL_FIRE_RESULT_CAN_NOT_FIND_TARGET_IN_ATTACK_RANGE;

	return	eSKILL_FIRE_RESULT_OK;
}

void
cAS_BouncingMissile::reset()
{

	memset(m_awTargetList,0xffff, sizeof(m_awTargetList));
	m_bTargetCount			=	0;
	m_bCurrentCount			=	0;
	m_wSpeed				=	0;
	m_bIsHit				=	FALSE;

}
//
//	캐스팅
BOOL
cAS_BouncingMissile::cast()
{


	cACTOR*	lpTarget	=	m_lpTarget;

	if(!lpTarget)
		return FALSE;

	int		iShotCount			=	m_ability.getShotCount();
	int		i					=	0;

	cFIELD	*lpField			=	m_lpCaster->m_lpField;
	int		iRange				=	m_lpCaster->getHitRange(&m_ability);
	WORD	awTargetList[dMAX_CONTINUOUS_HIT_INFO_COUNT];
	int		iTargetCount		=	0;

	memset(awTargetList, 0xffff, sizeof(awTargetList));
	m_wSpeed	=	m_lpSkill->m_wSpeed;
	
	if(lpTarget)
	{
		awTargetList[iTargetCount] = 	lpTarget->m_wSerialInField;
		++iTargetCount;
		--iShotCount;
	}
	for(i=0;i<iShotCount;++i)
	{
		lpTarget = lpField->findNeareastActor(lpTarget->m_iLocalPart, awTargetList,iTargetCount, lpTarget->m_pos.x, lpTarget->m_pos.y,iRange,lpTarget->m_wTeam);
		
		if(lpTarget)
		{
			awTargetList[iTargetCount] = 	lpTarget->m_wSerialInField;
			++iTargetCount;
		}
		else
		{
			break;
		}
	}
	
	if	(iTargetCount	==	0)
		return	FALSE;

	
	for(i=0;i<iTargetCount;++i)
	{
		CHitInfo	*lpInfo	=	&m_lpCaster->m_attackInfo.m_aInfo[m_bTargetCount];
		
		lpInfo->reset(TRUE);
		
		cACTOR	*lpTarget	=	lpField->getTestedActor(awTargetList[i]);
		
		if	(!lpTarget || lpTarget->isDeath())
			continue;
		
		
		lpTarget->getDamage(m_lpCaster,&m_ability,m_pos.x,m_pos.y,lpInfo,TRUE,TRUE,TRUE);
		
		
		if	(lpInfo->getDamage())
			lpInfo->setHit();
		
		m_awTargetList[m_bTargetCount]	=	awTargetList[i];
		
		++m_bTargetCount;
	}


	lpTarget	=	lpField->getTestedActor(m_awTargetList[0]);

	//if(lpTarget)
	//	m_dwBookedActionTime	+=	getNextStrikeTime(m_lpCaster->m_pos, lpTarget->m_pos);
	return	TRUE;
}


int
cAS_BouncingMissile::getNextStrikeTime(cPOINT _posBegin,cPOINT _posEnd)	
{

	float	fValue		=	0;
	int		iRange		=	GetOvalRange(_posBegin.x,_posBegin.y,_posEnd.x,_posEnd.y);
	float	fDivSec		=	(m_wSpeed*m_wSpeed* dSYNC_FPS*4);

	if(iRange && fDivSec)
	{
		fValue		=	iRange/fDivSec;
	}
	//fValue*= 10;
	return fValue;
}

void
cAS_BouncingMissile::strike(int _iIndex)
{

	cFIELD	*lpField		=	m_lpCaster->m_lpField;

	
	cACTOR	*lpTarget	=	lpField->getTestedActor(m_awTargetList[_iIndex]);
	
	if	(!lpTarget || lpTarget->isDeath() || lpTarget->isOwnForce(m_lpCaster))
		return;
	
	m_lpCaster->strike(lpTarget,&m_lpCaster->m_attackInfo.m_aInfo[_iIndex],&m_ability);

	if(!m_bIsHit && m_lpCaster->m_attackInfo.m_aInfo[_iIndex].isHit())
	{
		m_bIsHit  = TRUE;
		m_lpCaster->increaseCP(m_ability.getGetCP());
		m_lpCaster->sendPlayerBasicInformation();
	}
}

//
//	보조마법
BOOL
cAS_BouncingMissile::update()
{
	if (!isActionTime())
		return	FALSE;

	strike(m_bCurrentCount);
	++m_bCurrentCount;
	

	if(m_bCurrentCount >=m_bTargetCount)
		return TRUE;

	cACTOR*	lpTarget	=	m_lpCaster->m_lpField->getTestedActor(m_awTargetList[m_bCurrentCount]);

	if(lpTarget)	
		m_dwBookedActionTime	=	g_dwCurrentTime	+ getNextStrikeTime(m_lpCaster->m_pos, lpTarget->m_pos);
	
	return	FALSE;
}