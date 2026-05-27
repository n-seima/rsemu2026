#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cFIELD.H"

//
//	피어싱 미사일형 스킬 사용
int
CActiveSkill::fireLaserTypeSkill()
{
	m_lpCaster->action(m_lpSkill->m_wAction);
	m_wRange			=	m_lpCaster->getAttackRange(&m_ability);

	setCastingTimeByCasterDamageFrame(0);
	m_dwBookedActionFinishTime	=	m_lpCaster->getFrameTime();

	m_aValue[0]	=	FALSE;

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	피어싱 미사일형 스킬 사용
BOOL
CActiveSkill::updateLaserTypeSkill()
{
	if	(m_lpTarget)
	{
		m_posTarget.x	=	m_lpTarget->m_pos.x;
		m_posTarget.y	=	m_lpTarget->m_pos.y;
	}

	if	(!isActionTime())
		return	FALSE;

	if	(m_aValue[0])
	{
		if	(m_ability.isJobChange())
		{
			int	iTR			=	m_lpCaster->isAbleToTransformation(TRUE);

			if	(iTR	==	eTRANSFORMATION_SUCCESS	&&	m_ability.isJobChange())
				m_lpCaster->sendTransformation(eTR_BY_SKILL,m_lpSkill->m_bf1PutTrasAfterAnm);
		}

		return	TRUE;
	}

	cPOINT	posX1Y1,posX2Y1,posX1Y2,posX2Y2,posDest;

	int		iHitRange	=	m_ability.getHitRange(NULL);
	int		iAngle		=	g_protractor.getAngleToTarget(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,m_posTarget.x,m_posTarget.y);
	int		iAngle1		=	iAngle+90;
	int		iAngle2		=	iAngle-90;

	if	(iAngle1	>=	360	)
		iAngle1	-=	360;
	if	(iAngle2	<	0	)
		iAngle2	+=	360;

	posDest.x	=	m_lpCaster->m_pos.x;
	posDest.y	=	m_lpCaster->m_pos.y;

	if (m_lpTarget)
		g_protractor.getAnglePos(&posDest,iAngle,m_wRange+m_lpCaster->getBodySize()+m_lpTarget->getBodySize());
	else
		g_protractor.getAnglePos(&posDest,iAngle,m_wRange+m_lpCaster->getBodySize());

	posX1Y1		=	posDest;
	posX2Y1		=	posDest;
	g_protractor.getAnglePos(&posX1Y1,iAngle2,iHitRange/2);
	g_protractor.getAnglePos(&posX2Y1,iAngle1,iHitRange/2);

	posX1Y2.x	=	m_lpCaster->m_pos.x;
	posX1Y2.y	=	m_lpCaster->m_pos.y;
	posX2Y2		=	posX1Y2;
	g_protractor.getAnglePos(&posX1Y2,iAngle2,iHitRange/2);
	g_protractor.getAnglePos(&posX2Y2,iAngle1,iHitRange/2);

	m_lpCaster->strikeLaser(&m_ability,&posX1Y1,&posX2Y1,&posX2Y2,&posX1Y2);

	m_aValue[0]	=	TRUE;

	m_dwBookedActionTime	=	m_dwBookedActionFinishTime;

	return	FALSE;
}