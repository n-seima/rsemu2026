

#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cFIELD.H"
#include "CGame.H"
#include	"astWideMissile.h"

//
//	보조마법
int
cAS_WideMissile::fire()
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
cAS_WideMissile::reset()
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
cAS_WideMissile::cast()
{

	int		iDefaultTargetCount	=	0;
	int		iShotCount			=	m_ability.getContinuousShotCount();
	int		i					=	0;

	cFIELD	*lpField			=	m_lpCaster->m_lpField;
	int		iRange				=	m_lpCaster->getHitRange(&m_ability);
	WORD	awTargetList[20];
	memset(awTargetList, 0xffff, sizeof(awTargetList));
	iShotCount					=	min(iShotCount, dMAX_CONTINUOUS_HIT_INFO_COUNT);
	int		iCount				=	lpField->getInSightEnemyActorList(awTargetList,20,m_pos.x,m_pos.y,m_lpCaster,iRange,FALSE,TRUE,&m_ability);
	
	if	(iCount	==	0)
		return	FALSE;


	for(int iIndex=0;iIndex<iCount && iShotCount;++iIndex)
	{
		int iSelectIndex			= random(iCount - iIndex) + iIndex;
		WORD	wTemp				= awTargetList[iIndex];
		awTargetList[iIndex]		= awTargetList[iSelectIndex];
		awTargetList[iSelectIndex]	= wTemp;
	}

	iCount	=	min(iCount, dMAX_CONTINUOUS_HIT_INFO_COUNT);

	if(m_lpSkill->m_bf1IsApplySameTarget)
	{
		for(int i=0;i<iShotCount;++i)
		{
			int iIndex	=	random(iCount);
			m_awTargetList[i] = awTargetList[iIndex];
		}
		
		memcpy(awTargetList, m_awTargetList, sizeof(awTargetList));
	}
	iCount	= iShotCount;	

	for(i=0;i<iCount;++i)
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


	m_dwBookedActionTime	+=	m_lpSkill->m_sContiniousShotPeriod;
	return	TRUE;
}



void
cAS_WideMissile::strike(int _iIndex)
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
cAS_WideMissile::update()
{
	if (!isActionTime())
		return	FALSE;

	strike(m_bCurrentCount);
	++m_bCurrentCount;
	
	if(m_bCurrentCount >=m_bTargetCount)
		return TRUE;

	m_dwBookedActionTime	=	g_dwCurrentTime	+ m_lpSkill->m_sContiniousShotPeriod;
	
	return	FALSE;
}