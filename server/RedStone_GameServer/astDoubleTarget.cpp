#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cFIELD.H"
#include "CGame.H"
#include	"astDoubleTarget.h"

//
//	보조마법
int
CActiveSkill::fireDoubleTarget()
{
	if (m_lpCaster)
		m_lpCaster->action(m_lpSkill->m_wAction);

	m_dwBookedActionTime		=	m_lpCaster->getFirstTriggerTime();

	cAS_DoubleTarget	*lpAS	=	(cAS_DoubleTarget	*)this;

	if	(!lpAS->cast())
		return	eSKILL_FIRE_RESULT_CAN_NOT_FIND_TARGET_IN_ATTACK_RANGE;

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	캐스팅
BOOL
cAS_DoubleTarget::cast()
{
	int		iDefaultTargetCount	=	0;
	int		iShotCount			=	m_ability.getShotCount(),i;

	m_bContinuousShotCount		=	m_ability.getContinuousShotCount();

	cFIELD	*lpField			=	m_lpCaster->m_lpField;
	int		iRange				=	m_lpCaster->getAttackRange(&m_ability);
	int		iCount				=	lpField->getInSightEnemyActorList(m_awTargetList,iShotCount,m_pos.x,m_pos.y,m_lpCaster,iRange,FALSE,TRUE,&m_ability);

	if	(iCount	==	0)
		return	FALSE;

	m_wTargetCount				=	iCount;
	m_bMissCount				=	0;
	m_bCriticalCount			=	0;
	m_bDoubleCriticalCount		=	0;
	m_bHardBlowCount			=	0;
	if (m_lpTarget	&&	m_lpTarget->m_wSerialInField	!=	m_lpCaster->m_wSerialInField)
	{
		BOOL	bIsIncludeTarget=	FALSE;

		for (i=0;i<iCount;i++)
		{
			if (m_awTargetList[i]	==	m_lpTarget->m_wSerialInField)
			{
				m_awTargetList[i]	=	m_awTargetList[0];
				m_awTargetList[0]	=	m_lpTarget->m_wSerialInField;
				bIsIncludeTarget	=	TRUE;
				break;
			}
		}

		if (bIsIncludeTarget	==	FALSE)
			m_awTargetList[0]	=	m_lpTarget->m_wSerialInField;
	}

	if (m_bContinuousShotCount	<=	1)
	{
		for (i=0;i<iCount;i++)
		{
			cACTOR	*lpTarget	=	lpField->getTestedActor(m_awTargetList[i]);

			if (!lpTarget || lpTarget->isDeath())
			{
				m_wTargetCount--;
				continue;
			}

			lpTarget->getDamage(m_lpCaster,&m_ability,m_pos.x,m_pos.y,&m_lpCaster->m_attackInfo.m_aInfo[i]);
		}
	}
	else
	{
		for (i=0;i<iCount;i++)
			m_lpCaster->m_attackInfo.m_aInfo[i].reset(TRUE);

		for (int j=0;j<m_bContinuousShotCount;j++)
			for (i=0;i<iCount;i++)
			{
				cACTOR	*lpTarget	=	lpField->getTestedActor(m_awTargetList[i]);

				if	(!lpTarget || lpTarget->isDeath())
					continue;

				CHitInfo	hitInfo;
				CHitInfo	*lpInfo	=	&m_lpCaster->m_attackInfo.m_aInfo[i];

				lpTarget->getDamage(m_lpCaster,&m_ability,m_pos.x,m_pos.y,&hitInfo,TRUE,TRUE,TRUE,m_bContinuousShotCount,j);

				if	(hitInfo.isMiss()	||	hitInfo.isDodge())
				{
					hitInfo.m_dwPhysicalDamage	=	0;
					hitInfo.m_dwMagicDamage		=	0;
					m_bMissCount++;
				}

				if	(hitInfo.isCritical())
					m_bCriticalCount++;
				else
				if	(hitInfo.isLuckyDoubleCritical())
					m_bDoubleCriticalCount++;
				else
				if	(hitInfo.isHardBlow())
					++m_bHardBlowCount;

				lpInfo->m_dwPhysicalDamage	+=	hitInfo.m_dwPhysicalDamage;
				lpInfo->m_dwMagicDamage		+=	hitInfo.m_dwMagicDamage;

				if	(hitInfo.isInstanceKill())
					lpInfo->setInstanceKill();
			}

		for (i=0;i<iCount;i++)
		{
			CHitInfo	*lpInfo	=	&m_lpCaster->m_attackInfo.m_aInfo[i];

			if	(lpInfo->getDamage())
				lpInfo->setHit();
		}
	}

	if	(m_wTargetCount	==	0)
		return	FALSE;

	return	TRUE;
}

void
cAS_DoubleTarget::strike()
{
	int		iRange			=	m_lpCaster->getAttackRange(&m_ability)+20;
	cFIELD	*lpField		=	m_lpCaster->m_lpField;
	int		iHitCount		=	0;

	iRange	*=	iRange;

	for (int i=0;i<m_wTargetCount;i++)
	{
		cACTOR	*lpTarget	=	lpField->getTestedActor(m_awTargetList[i]);

		if	(!lpTarget || lpTarget->isDeath() || lpTarget->isOwnForce(m_lpCaster))
			continue;

		int	iCurRange	=	GetOvalRange(lpTarget->m_pos.x,lpTarget->m_pos.y,m_lpCaster->m_pos.x,m_lpCaster->m_pos.y);

		if	(iCurRange	>	iRange)
			continue;

		m_lpCaster->strike(lpTarget,&m_lpCaster->m_attackInfo.m_aInfo[i],&m_ability);

		if	(m_lpCaster->m_attackInfo.m_aInfo[i].isHit())
			iHitCount++;
	}

	if	(iHitCount)
	{
		m_lpCaster->increaseCP(m_ability.getGetCP());
		m_lpCaster->sendPlayerBasicInformation();
	}
}

//
//	보조마법
BOOL
CActiveSkill::updateDoubleTarget()
{
	if (!isActionTime())
		return	FALSE;

	cAS_DoubleTarget	*lpAS	=	(cAS_DoubleTarget	*)this;

	lpAS->strike();

	return	TRUE;
}