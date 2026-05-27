#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cFIELD.H"
#include "CGame.H"
#include	"astContinuousHitOfDoppelganger.h"
//
//	분신 발사
int
CActiveSkill::fireContinuousHitOfDoppelganger()
{
	m_lpCaster->action(m_lpSkill->m_wAction);

	setIsContinuousHitOfDoppelgangerTriggerAction(TRUE);

	getActionTime();

	m_wFrame		=	0;

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	분신 타입 스킬 캐스팅
BOOL
cAS_ContinuousHitOfDoppelganger::cast()
{


	int		iDefaultTargetCount	=	0;
	int		iShotCount			=	m_ability.getShotCount(),i;

	m_wContinuousShotCount		=	m_ability.getContinuousShotCount();

	m_lpCaster->sendPlayerBasicInformation();

	CSkill* lpSkill = m_ability.getSkill();
	if(!lpSkill)
		return FALSE;

	cFIELD	*lpField			=	m_lpCaster->m_lpField;
	int		iRange				=	m_lpCaster->getAttackRange(&m_ability);
	int		iCount				=	lpField->getInSightEnemyActorList(m_awTargetList,iShotCount,m_pos.x,m_pos.y,m_lpCaster,iRange,FALSE,TRUE,&m_ability);

	if	(iCount	==	0)
		return	FALSE;

	m_wTargetCount				=	iCount;
	m_bMissCount				=	0;
	m_bCriticalCount			=	0;
	m_bDoubleCriticalCount		=	0;
	m_wCheckShot				=	0;
	m_bHardBlowCount			=	0;

	int iDamageFrame = m_lpCaster->getDamageFrameInAnm(lpSkill->m_wAction2);
	int iFrameCount = m_lpCaster->getBody()->getFrameCount(lpSkill->m_wAction2);

	m_wDoppelgangerActionSpeed = lpSkill->m_aValue[0]*10;

	if(iDamageFrame == 0xffff)
		iDamageFrame = iFrameCount;
	m_dwBookedActionTime		=	g_dwCurrentTime + m_wDoppelgangerActionSpeed * float(iDamageFrame)/iFrameCount;

	if (m_wContinuousShotCount	<=	1)
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

		for (int j=0;j<m_wContinuousShotCount;j++)
			for (i=0;i<iCount;i++)
			{
				
				cACTOR	*lpTarget	=	lpField->getTestedActor(m_awTargetList[i]);
				if	(!lpTarget || lpTarget->isDeath())
					continue;

				CHitInfo	hitInfo;
				CHitInfo	*lpInfo	=	&m_lpCaster->m_attackInfo.m_aInfo[i];

				lpTarget->getDamage(m_lpCaster,&m_ability,m_pos.x,m_pos.y,&hitInfo,TRUE,TRUE,TRUE,m_wContinuousShotCount,j);

				if	(hitInfo.isMiss()	||	hitInfo.isDodge())
				{
					hitInfo.m_dwPhysicalDamage	=	0;
					hitInfo.m_dwMagicDamage		=	0;
					m_bMissCount++;
				}

				if	(hitInfo.isCritical())
					++m_bCriticalCount;
				else
				if	(hitInfo.isLuckyDoubleCritical())
					++m_bDoubleCriticalCount;
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
			{
				lpInfo->setHit();
			}
		}
	}



	if	(m_wTargetCount	==	0)
		return	FALSE;	

	m_lpCaster->m_attackInfoCHOD.reset();
	memcpy(&m_lpCaster->m_attackInfoCHOD,  &m_lpCaster->m_attackInfo , sizeof(CAttackInfo));

	lpField->sendContinuousActionToGroup(m_lpCaster,&m_ability);

	return	TRUE;
}




void
cAS_ContinuousHitOfDoppelganger::strike()
{

	cFIELD	*lpField		=	m_lpCaster->m_lpField;
	int		iHitCount		=	0;

	for (int i=0;i<m_wTargetCount && i< dMAX_CONTINUOUS_HIT_INFO_COUNT;i++)
	{
		cACTOR	*lpTarget	=	lpField->getTestedActor(m_awTargetList[i]);
		
		if	(!lpTarget || lpTarget->isDeath())
			continue;
		CHitInfo			hitInfo;
		memcpy(&hitInfo, &m_lpCaster->m_attackInfoCHOD.m_aInfo[i], sizeof(CHitInfo));

		if(m_lpCaster->m_attackInfoCHOD.m_aInfo[i].m_dwPhysicalDamage)
		{
			hitInfo.m_dwPhysicalDamage = m_lpCaster->m_attackInfoCHOD.m_aInfo[i].m_dwPhysicalDamage/m_wContinuousShotCount;
			m_lpCaster->m_attackInfoCHOD.m_aInfo[i].m_dwPhysicalDamage = max(m_lpCaster->m_attackInfoCHOD.m_aInfo[i].m_dwPhysicalDamage, 0);
		}
		if(m_lpCaster->m_attackInfoCHOD.m_aInfo[i].m_dwMagicDamage)
		{
			hitInfo.m_dwMagicDamage = m_lpCaster->m_attackInfoCHOD.m_aInfo[i].m_dwMagicDamage/m_wContinuousShotCount;
			m_lpCaster->m_attackInfoCHOD.m_aInfo[i].m_dwMagicDamage = max(m_lpCaster->m_attackInfoCHOD.m_aInfo[i].m_dwMagicDamage, 0);
		}
		if(m_lpCaster->m_attackInfoCHOD.m_aInfo[i].m_dwAbsorbDamage)
		{
			hitInfo.m_dwAbsorbDamage = m_lpCaster->m_attackInfoCHOD.m_aInfo[i].m_dwAbsorbDamage/m_wContinuousShotCount;
			m_lpCaster->m_attackInfoCHOD.m_aInfo[i].m_dwAbsorbDamage = max(m_lpCaster->m_attackInfoCHOD.m_aInfo[i].m_dwAbsorbDamage, 0);
		}
		if(m_wCheckShot == 0)
			m_lpCaster->strike(lpTarget,&hitInfo,&m_ability);
		else
			m_lpCaster->strike(lpTarget,&hitInfo,&m_ability,FALSE);

	}

	++m_wCheckShot;
	CSkill* lpSkill = m_ability.getSkill();
	if(!lpSkill)
		return;

	m_dwBookedActionTime		=	g_dwCurrentTime + lpSkill->m_aValue[0]*10;

}



BOOL
cAS_ContinuousHitOfDoppelganger::update()
{
	if(isActionTime())
	{
		strike();

		if(m_wCheckShot >= m_wContinuousShotCount)
		{
			if(m_wContinuousShotCount>m_bMissCount)
			{
				m_lpCaster->increaseCP(m_ability.getGetCP());
				m_lpCaster->sendPlayerBasicInformation();
			}
			return TRUE;
		}
	}
	
	return FALSE;
}
//
//	분신 타입 스킬 업데이트
BOOL
CActiveSkill::updateContinuousHitOfDoppelganger()
{
	cAS_ContinuousHitOfDoppelganger	*lpAS	=	(cAS_ContinuousHitOfDoppelganger	*)this;
	if(!lpAS)
		return TRUE;
	
	if (isContinuousHitOfDoppelgangerTriggerAction())
	{
		if (isActionTime())
		{
			lpAS->cast();
			setIsContinuousHitOfDoppelgangerTriggerAction(FALSE);
		}
		return	FALSE;
	}
	
	return lpAS->update();
	
}
