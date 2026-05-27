#include "cSKILL.H"
#include "CActor.H"
#include "CGame.H"
#include "CSound.H"
#include <math.h>

CPos lpPos;
//
//	연속 타격 스킬
int
CActiveSkill::fireScimitarCutting()
{
	m_lpCaster->action(m_lpTarget->m_pos.x,m_lpTarget->m_pos.y,m_lpSkill->m_wAction);
	m_lpCaster->m_wUseSpecialActionSkill	=	eSAS_SCIMATAR_CUTTING;
//	m_lpCaster->m_iFPS	=	48;

	int	iCurrentAngle	=	GetOvalAngleToTarget(m_lpTarget->m_pos.x,m_lpTarget->m_pos.y,m_lpCaster->m_pos.x,m_lpCaster->m_pos.y);
	int	iAngleStep		=	360*1000/dSYNC_FPS/m_lpSkill->m_wSpeed;
	int	iRange			=	GetOvalRange(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,m_lpTarget->m_pos.x,m_lpTarget->m_pos.y);
	iRange				=	(int)sqrt((double)iRange);

	setScimitarCuttingCurrentRange(iRange);

	resetScimitarCuttingGetCPStatus();
	resetScimitarCuttingIsWhenStartBiteHanging();
	resetScimitarCuttingRoundingCount();
	setScimitarCuttingAngleStep(iAngleStep);
	setScimitarCuttingAngle(0);
	setScimitarCuttingBeginAngle(iCurrentAngle);
	setScimitarCuttingHitCount(0);
	g_esm.playActionSound(m_lpSkill);

	if (m_lpCaster->isHero())	g_hero.setNotFocusStatus();

	lpPos.Set(m_lpCaster->m_pos.x , m_lpCaster->m_pos.y);

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	캐스팅
void
CActiveSkill::castScimitarCutting()
{
}

//
//	연속 타격 스킬
BOOL
CActiveSkill::updateScimitarCutting()
{
	if (!m_lpCaster				)	return TRUE;
	if (m_lpCaster->isDeath()	)	return TRUE;

	if (m_lpTarget)	m_posTarget.Set(m_lpTarget->m_pos.x,m_lpTarget->m_pos.y);

	int	iAngle	=	getScimitarCuttingAngle();

	if (increaseScimitarCuttingAngle())
	{
		if (iAngle	<	90 || (iAngle < 270 && getScimitarCuttingAngle() >= 270))
		{
			int			iShotIndex	=	getScimitarCuttingRoundingCount();
			CHitInfo	*lpHitInfo	=	&m_lpCaster->m_attackInfo.m_aInfo[iShotIndex];
	
			if(getScimitarCuttingHitCount() <getScimitarCuttingRoundCount())
			{
				if (m_lpTarget && !m_lpTarget->isDeath())
				{
					m_lpCaster->strike(m_lpTarget,&m_ability,lpHitInfo,0,FALSE);
				}
				
				if (m_lpCaster->isHero() && !m_lpSkill->m_wIsNotSyncCP)
				{
					if (lpHitInfo->getDamage() && getScimitarCuttingGetCPStatus()==FALSE)
					{
						setScimitarCuttingGetCPStatus();
						g_hero.increaseCP(m_ability.getGetCP());
					}
				}
				increaseScimitarCuttingHitCount();
			}
		}
	}

	iAngle		=	getScimitarCuttingAngle()+getScimitarCuttingBeginAngle();
	iAngle		%=	360;

	if (getScimitarCuttingRoundingCount()	>=	getScimitarCuttingRoundCount())	iAngle		=	getScimitarCuttingBeginAngle();

	CPos	pos;

	pos.Set(m_posTarget.x,m_posTarget.y);
	
	GetOvalAnglePos(&pos,iAngle,getScimitarCuttingCurrentRange());

	if(m_lpCaster->m_wBiter==0xffff)
		m_lpCaster->setPos(pos.x,pos.y);

	if (getScimitarCuttingCurrentRange()	<	getScimitarCuttingRange())
	{
		increaseScimitarCuttingCurrentRange(10);

		if (getScimitarCuttingCurrentRange()>	getScimitarCuttingRange())	setScimitarCuttingCurrentRange(getScimitarCuttingRange());
	}

	if (getScimitarCuttingCurrentRange()	>	getScimitarCuttingRange())
	{
		increaseScimitarCuttingCurrentRange(-10);

		if (getScimitarCuttingCurrentRange()<	getScimitarCuttingRange())	setScimitarCuttingCurrentRange(getScimitarCuttingRange());
	}

	if (getScimitarCuttingRoundingCount()	>=	getScimitarCuttingRoundCount())
	{
		if(m_lpCaster->m_wBiter != 0xffff)
			m_lpCaster->setPos(lpPos.x,lpPos.y);
		else
			m_lpCaster->setPos(m_pos.x,m_pos.y);
			

		return	TRUE;
	}

	return	FALSE;
}
//
//	찍기
void
CActiveSkill::putScimitarCutting()
{
}