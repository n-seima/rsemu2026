#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cFIELD.H"

int
CActiveSkill::fireTornadoTypeSkill()
{
	m_lpCaster->action(m_lpSkill->m_wAction);

	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}	//	CActiveSkill::fireTornadoTypeSkill()

BOOL
CActiveSkill::castTornadoTypeSkill()
{
	if (!m_lpCaster)	return	FALSE;

	int	iBodySize	=	m_lpCaster->getBodySize()+30;

	m_wFrameCounter	=	0;						//	바로시작
	m_wRange		=	0;
	m_wAngle		=	g_protractor.getAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y);

	g_protractor.getAnglePos(&m_pos,m_wAngle,iBodySize);

	memset(m_aValue,0xff,sizeof(m_aValue));
	mIS_GETEDCP		=	FALSE;

	return	TRUE;
}	//	CActiveSkill::castTornadoTypeSkill()

BOOL
CActiveSkill::updateTornadoTypeSkill()
{
	if (m_wFrameCounter	==	0xffff	)
	{
		if (isActionTime())
			if (!castTornadoTypeSkill())
				return	TRUE;

		return	FALSE;	//	아직 쏘기 직전이다.
	}

	g_protractor.getAnglePos(&m_pos,m_wAngle,m_lpSkill->m_wSpeed);

	BOOL	bIsGetCP				=	mIS_GETEDCP;
	BOOL	bIsTooHitTooManyTarget	=	m_lpCaster->m_lpField->strikeWideAreaDamageSkill(m_lpCaster,&m_ability,&m_pos,m_ability.getHitRange(),m_aValue,dEXTRA_VALUE_FOR_ACTIVE_SKILL-10,&bIsGetCP);
	mIS_GETEDCP	=	bIsGetCP;

	m_wRange	+=	m_lpSkill->m_wSpeed;

	if	(m_wRange>=	m_lpSkill->m_wShootRange)
		return	TRUE;

	return	FALSE;
}	//	CActiveSkill::updateTornadoTypeSkill()