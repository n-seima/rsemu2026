#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cFIELD.H"
#include "CGame.H"
#include	"astContinuousBit.h"

//
//	보조마법
int
CActiveSkill::fireContinuousBit()
{
	if (m_lpCaster)
		m_lpCaster->action(m_lpSkill->m_wAction);

	m_dwBookedActionTime		=	m_lpCaster->getFirstTriggerTime();

	cAS_ContinuousBit	*lpAS	=	(cAS_ContinuousBit	*)this;

	if	(!lpAS->cast())
		return	eSKILL_FIRE_RESULT_CAN_NOT_FIND_TARGET_IN_ATTACK_RANGE;

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	캐스팅
BOOL
cAS_ContinuousBit::cast()
{
	int		iDefaultTargetCount	=	0;
	int		iShotCount			=	m_ability.getShotCount();
	int		i					=	0;

	cFIELD	*lpField			=	m_lpCaster->m_lpField;
	int		iRange				=	m_lpCaster->getHitRange(&m_ability);
	WORD	awTargetList[dMAX_CONTINUOUS_HIT_INFO_COUNT];
	memset(awTargetList, 0xffff, sizeof(awTargetList));
	int		iCount				=	lpField->getInSightEnemyActorList(awTargetList,iShotCount,m_pos.x,m_pos.y,m_lpCaster,iRange,FALSE,TRUE,&m_ability);
	m_bTargetCount				=	0;
	if	(iCount	==	0)
		return	FALSE;


	m_bContinuousShotCount		=	m_ability.getContinuousShotCount();

	for(i=0;i<iCount;++i)
	{
		CHitInfo	*lpInfo	=	&m_lpCaster->m_attackInfo.m_aInfo[m_bTargetCount];

		lpInfo->reset(TRUE);

		cACTOR	*lpTarget	=	lpField->getTestedActor(awTargetList[i]);

		if	(!lpTarget || lpTarget->isDeath())
			continue;
		int iBitCount		= lpTarget->getBitCount(m_lpCaster->m_wSerialInField , m_lpCaster->m_dwNameHashCode);
		int iAttackCount	= min(dMAX_CONTINUOUS_HIT_INFO_COUNT, iBitCount + m_bContinuousShotCount);
		if(!iAttackCount)
			continue;
		lpInfo->continuousHitInfo.m_bf6ContinuousShotCount	=	iAttackCount;
		for(int j=0;j<iAttackCount;++j)
		{
			CHitInfo	hitInfo;
			
			lpTarget->getDamage(m_lpCaster,&m_ability,m_pos.x,m_pos.y,&hitInfo,TRUE,TRUE,TRUE,iAttackCount,j);
			
			if	(hitInfo.isMiss()	||	hitInfo.isDodge())
			{
				hitInfo.m_dwPhysicalDamage	=	0;
				hitInfo.m_dwMagicDamage		=	0;
				++lpInfo->continuousHitInfo.m_bf6MissCount;
			}
			
			if	(hitInfo.isCritical())
			{
				++lpInfo->continuousHitInfo.m_bf6CriticalCount;
			}
			else if	(hitInfo.isLuckyDoubleCritical())
			{
				++lpInfo->continuousHitInfo.m_bf6DoubleCriticalCount;
			}
			else if	(hitInfo.isHardBlow())
			{
				++lpInfo->continuousHitInfo.m_bf6HardBlowCount;
			}
			lpInfo->m_dwPhysicalDamage	+=	hitInfo.m_dwPhysicalDamage;
			lpInfo->m_dwMagicDamage		+=	hitInfo.m_dwMagicDamage;
			
			if	(hitInfo.isInstanceKill())
			{
				lpInfo->setInstanceKill();		
			}
		}

		if	(lpInfo->getDamage())
			lpInfo->setHit();

		if(m_lpSkill->m_bf4SpendBit)
		{
			lpTarget->removeBit(m_lpCaster->m_wSerialInField, m_lpCaster->m_dwNameHashCode, iBitCount);
		}

		m_awTargetList[m_bTargetCount]	=	awTargetList[i];

		++m_bTargetCount;

	}

	if	(m_bTargetCount	==	0)
		return	FALSE;

	return	TRUE;
}

void
cAS_ContinuousBit::strike()
{

	cFIELD	*lpField		=	m_lpCaster->m_lpField;
	int		iHitCount		=	0;

	for (int i=0;i<m_bTargetCount;++i)
	{
		cACTOR	*lpTarget	=	lpField->getTestedActor(m_awTargetList[i]);

		if	(!lpTarget || lpTarget->isDeath() || lpTarget->isOwnForce(m_lpCaster))
			continue;
		
		m_lpCaster->strike(lpTarget,&m_lpCaster->m_attackInfo.m_aInfo[i],&m_ability);

		if	(m_lpCaster->m_attackInfo.m_aInfo[i].isHit())
			++iHitCount;
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
CActiveSkill::updateContinuousBit()
{
	if (!isActionTime())
		return	FALSE;

	cAS_ContinuousBit	*lpAS	=	(cAS_ContinuousBit	*)this;

	lpAS->strike();

	return	TRUE;
}