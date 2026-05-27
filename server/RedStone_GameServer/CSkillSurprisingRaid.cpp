#include "CActiveSkill.H"
#include "cACTOR.H"
#include "CField.H"
#include "debugCode.H"
#include <math.h>

//	
int
CActiveSkill::fireSurprisingRaid()
{
	if	(m_lpCaster->isSealdMove())
		return	eSKILL_FIRE_RESULT_FAILED;

	m_lpTarget->getDamage(m_lpCaster,&m_ability,0,0,&m_hitInfo);
	m_lpCaster->m_attackInfo.reset();
	m_lpCaster->m_attackInfo.m_aInfo[0].copy(&m_hitInfo);
	m_lpCaster->action(m_lpSkill->m_wAction);

	setSurprisingRaidStep(0);

	return	eSKILL_FIRE_RESULT_OK;
}	

//
//
BOOL
CActiveSkill::castSurprisingRaid()
{
	if	(!m_lpCaster || !m_lpTarget || !m_lpTarget->getBody() || !m_lpCaster->getBody() )
		return	FALSE;
	
	m_posTarget.x	=	m_lpTarget->m_pos.x;
	m_posTarget.y	=	m_lpTarget->m_pos.y;

	int	iAngle		=	g_protractor.getAngleToTarget(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,m_posTarget.x,m_posTarget.y);
	int	iRange		=	GetOvalRange(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,m_posTarget.x,m_posTarget.y);
	int	iGab		=	m_lpTarget->getBodySize()+m_lpCaster->getBodySize()+40;
	iRange			=	(int)sqrt(iRange);

	iRange			-=	iGab;

	if (iRange	>	0)
	{
		iAngle		+=	180;
		iAngle		%=	360;

		g_protractor.getAnglePos(&m_posTarget,iAngle,iGab);
	}
	else
	{
		m_posTarget.x=	m_lpCaster->m_pos.x;
		m_posTarget.y=	m_lpCaster->m_pos.y;
	}

	if	(!m_lpCaster)
		return	FALSE;

	cPOINT	posArrive;

	CActiveSkillDamagedActorInfo	info;

	info.reset();
	
	m_lpCaster->m_lpField->operateRushSkill(m_lpCaster,m_lpTarget,m_posTarget.x,m_posTarget.y,&m_ability,&posArrive,&info);

	return	TRUE;
}


//	
BOOL
CActiveSkill::updateSurprisingRaid()
{
	if	(!m_lpCaster || !m_lpCaster->getBody())
		return FALSE;

	m_lpCaster->setRemainActionTime(4);	

	if	(m_lpCaster->m_isAction	==	FALSE)
	{
		m_lpCaster->setRemainActionTime(1);	

		return	TRUE;
	}

	if	(getSurprisingRaidStep()	==	0)
	{
		if	(m_lpCaster->m_bIsDamageFrame)
		{
			m_wIsGetdCP	=	m_lpCaster->strikeWhirlWind(&m_ability);

			setSurprisingRaidStep(1);
		}

		return	FALSE;
	}

	if (getSurprisingRaidStep()	==	1)
	{
		if (m_lpCaster->m_bIsDamageFrame)
		{
			if	(!castSurprisingRaid())
				return	FALSE;

			if (m_lpTarget)
			{
				if	(m_lpCaster)
					m_lpCaster->strike(m_lpTarget,&m_hitInfo,&m_ability);

				if	(m_lpCaster && m_hitInfo.isHit() && m_wIsGetdCP == FALSE)
				{
					m_lpCaster->increaseCP(m_ability.getGetCP());
					m_lpCaster->sendPlayerBasicInformation();
				}
			}

			if	(m_lpCaster)
				m_lpCaster->setRemainActionTime(1);	

			return	TRUE;
		}
	}

	return	FALSE;
}