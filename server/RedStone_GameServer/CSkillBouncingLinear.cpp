#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cFIELD.H"
#include <MATH.H>

//
//	머신타입 스킬 사용
int
CActiveSkill::fireBouncingLinear()
{
	if	(m_lpCaster->isSealdMove())
		return	eSKILL_FIRE_RESULT_FAILED;
	
	memset(m_aValue,0,sizeof(m_aValue));

	m_lpCaster->action(m_lpSkill->m_wAction);

	CActiveSkillDamagedActorInfo	info;

	int	iMaxCount	=	m_ability.getContinuousShotCount(),iHitCount=0;

	m_lpCaster->m_lpField->getBouncingLinearTarget(m_lpCaster,m_lpTarget,&m_ability,&info,iMaxCount);

	for (int i=0;i<info.m_wCount;i++)
	{
		CHitInfo	*lpInfo		=	&m_lpCaster->m_attackInfo.m_aInfo[i];
		cACTOR		*lpTarget	=	m_lpCaster->m_lpField->getActor(info.m_awActorList[i]);

		lpInfo->reset();
		m_aValue[i]	=	info.m_awActorList[i];

		if	(lpTarget)
		{
			lpTarget->getDamage(m_lpCaster,&m_ability,0,0,lpInfo,TRUE,TRUE,TRUE,info.m_wCount,i);
			iHitCount	+=	lpInfo->isHit();
		}
	}

	setBouncingLinearAttackRange(m_lpCaster->getAttackRange(&m_ability)+20);
	setBouncingLinearTargetIndex(0);
	setBouncingLinearTargetCount(info.m_wCount);

	getActionTime();

	m_wFrameCounter	=	0xffff;

	return	eSKILL_FIRE_RESULT_OK;
}

//
BOOL
CActiveSkill::castBouncingLinear()
{
	return	aimingNextBouncingLinearTarget();
}

//
BOOL
CActiveSkill::aimingNextBouncingLinearTarget()
{
	if(!m_lpCaster)
		return FALSE;
	
	if (getBouncingLinearTargetCount() == getBouncingLinearTargetIndex())
	{
		m_lpCaster->setRemainActionTime(1);
		return	TRUE;
	}

	cACTOR		*lpTarget;

	while(1)
	{
		lpTarget	=	m_lpCaster->m_lpField->getActor(getBouncingLinearTargetIndex());

		if (lpTarget)
			break;

		increaseBouncingLinearTargetIndex();

		if (getBouncingLinearTargetCount() == getBouncingLinearTargetIndex())
		{
			m_lpCaster->setRemainActionTime(1);
			return	TRUE;
		}
	}

	int	iRange		=	GetOvalRange(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y);

	iRange			=	(int)sqrt(iRange);

	if (iRange		>=	getBouncingLinearAttackRange())
	{
		m_lpCaster->setRemainActionTime(1);
		return	TRUE;
	}

	int	iRequireFrame=	iRange*dSYNC_FPS/m_lpSkill->m_wSpeed;

	iRequireFrame	/=	2;

	m_wFrame		=	max(iRequireFrame,1);
	m_wFrameCounter	=	0;

	return	FALSE;
}

BOOL
CActiveSkill::updateBouncingLinear()
{
	if(!m_lpCaster)
		return FALSE;

	m_lpCaster->setRemainActionTime(4);
	
	if (m_wFrameCounter	==	0xffff)
		return	castBouncingLinear();

	m_wFrame--;

	if (m_wFrame	==	0)
	{
		int	iIndex	=	getBouncingLinearTargetIndex();

		CHitInfo	*lpInfo		=	&m_lpCaster->m_attackInfo.m_aInfo[iIndex];
		cACTOR		*lpTarget	=	m_lpCaster->m_lpField->getActor(m_aValue[iIndex]);

		if (lpTarget)
		{
			m_lpCaster->strike(lpTarget,lpInfo,&m_ability);

			if (m_wIsGetdCP == FALSE && lpInfo->isHit())
			{
				m_lpCaster->increaseCP(m_ability.getGetCP());
				m_lpCaster->sendPlayerBasicInformation();

				m_wIsGetdCP	=	TRUE;
			}
		}

		increaseBouncingLinearTargetIndex();

		if (aimingNextBouncingLinearTarget())
		{
			m_lpCaster->setRemainActionTime(1);
			return	TRUE;
		}
	}

	return	FALSE;
}