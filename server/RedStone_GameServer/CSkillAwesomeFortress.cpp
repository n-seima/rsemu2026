#include "CActiveSkill.H"
#include "cACTOR.H"
#include "CField.H"
#include <math.h>

//
//	분신 발사
int
CActiveSkill::fireAwesomeFortress()
{
	int	iShotCount			=	m_ability.getContinuousShotCount();

	m_lpCaster->action(m_lpSkill->m_wAction);

	setAwesomeFortressBunshineCount(iShotCount);
	setIsAwesomeFortressTriggerAction(TRUE);

	getActionTime();

	m_wFrame		=	0;

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	분신 타입 스킬 캐스팅
BOOL
CActiveSkill::castAwesomeFortress()
{
	BOOL	bIsGetedCP;
	CActiveSkillDamagedActorInfo	info;

	info.reset();
	m_lpCaster->m_lpField->strikeExplosionSkill(m_lpCaster,m_lpTarget,m_posTarget.x,m_posTarget.y,&m_ability,&bIsGetedCP,getAwesomeFortressBunshineCount(),&info);
	m_lpCaster->action(m_lpSkill->m_wAction2);

	setAwesomeFortressTargetCount(info.m_wCount);
	setIsAwesomeFortressTriggerAction(FALSE);

	for (int i=0;i<info.m_wCount;i++)
		m_aValue[10+i]	=	info.m_awActorList[i];

	return	TRUE;
}

void
CActiveSkill::strikeAwesomeFortressAttack()
{
	int	iHitCount	=	0;

	for (int i=0;i<getAwesomeFortressTargetCount();i++)
	{
		CHitInfo	*lpInfo	=	&m_lpCaster->m_attackInfo.m_aInfo[i];
		int			iTarget	=	m_aValue[10+i];
		cACTOR		*lpTarget=	m_lpCaster->m_lpField->getActor(iTarget);

		if (!lpTarget || lpTarget->isDeath())
			continue;

		if (i==0)
			m_lpCaster->strike(lpTarget,lpInfo,&m_ability);
		else
			m_lpCaster->strike(lpTarget,lpInfo,&m_ability,FALSE,FALSE);

		if (lpInfo->isHit())
			iHitCount++;
	}

	if (iHitCount)
		m_lpCaster->increaseCP(m_ability.getGetCP());
}

//
//	분신 타입 스킬 업데이트
BOOL
CActiveSkill::updateAwesomeFortress()
{
	if (isAwesomeFortressTriggerAction())
	{
		if (isActionTime())
			castAwesomeFortress();

		return	FALSE;
	}

	if (isActionTime())
	{
		strikeAwesomeFortressAttack();

		return	TRUE;
	}

	return	FALSE;
}