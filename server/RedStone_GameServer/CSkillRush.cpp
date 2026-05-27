#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cFIELD.H"
#include <MATH.H>

//
//	머신타입 스킬 사용
int
CActiveSkill::fireRushTypeSkill()
{
	if	(m_lpCaster->isSealdMove())
		return	eSKILL_FIRE_RESULT_FAILED;

	memset(m_aValue,0,sizeof(m_aValue));

	m_lpCaster->action(m_lpSkill->m_wAction);

	m_lpCaster->m_wRemainActionTime	=	dSYNC_FPS/dREVISE_ACTION_SPEED_FOR_TEST;

	if	(m_lpSkill->m_bf1IsWhirlRunningStyleRush)
		setIsWhirlRunningStyleRush();

	m_wFrame		=	0;
	m_wFrameCounter	=	0xffff;

	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}

//
//
void
CActiveSkill::castRushTypeSkill(BOOL _bIsRunning)
{
	if	(m_lpTarget)
	{
		m_posTarget.x	=	m_lpTarget->m_pos.x;
		m_posTarget.y	=	m_lpTarget->m_pos.y;
	}

	cPOINT							posArrive;
	CActiveSkillDamagedActorInfo	info;

	if	(_bIsRunning)
		m_lpCaster->m_lpField->getRushPoint(m_lpCaster,m_lpTarget,m_posTarget.x,m_posTarget.y,&m_ability,&posArrive);
	else
		m_lpCaster->m_lpField->operateRushSkill(m_lpCaster,m_lpTarget,m_posTarget.x,m_posTarget.y,&m_ability,&posArrive,&info);

	m_posTarget.x	=	posArrive.x;
	m_posTarget.y	=	posArrive.y;

	int	iRange		=	GetOvalRange(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y);

	iRange			=	(int)sqrt(iRange);

	int	iRequireFrame=	iRange*dSYNC_FPS/m_lpSkill->m_wSpeed;

	iRequireFrame	/=	2;

	m_wFrame		=	iRequireFrame;
	m_wFrameCounter	=	0;
}

void
CActiveSkill::crushRushSkill()
{
	CActiveSkillDamagedActorInfo	info;
	cPOINT	posArrive;

	info.reset();

	m_lpCaster->m_lpField->operateRushSkill(m_lpCaster,m_lpTarget,m_posTarget.x,m_posTarget.y,&m_ability,&posArrive,&info,TRUE);

	int	iShotCount	=	m_ability.getShotCount(),iIndex	=	0;
	
	if	(iShotCount	==	0 || !m_lpCaster || !m_lpCaster->m_lpField)
		return;

	while(iShotCount && m_lpCaster && m_lpCaster->m_lpField)
	{
		int	iAngle;

		if	(iIndex % 2)
			iAngle	=	(360 + m_lpCaster->m_wAngle - m_lpSkill->m_bf9IsParallellRushAngle)%360;
		else
			iAngle	=	(360 + m_lpCaster->m_wAngle + m_lpSkill->m_bf9IsParallellRushAngle)%360;

		cPOINT	pos,posTarget,posDummy;

		pos.x		=	m_pos.x;
		pos.y		=	m_pos.y;
		posTarget.x	=	posArrive.x;
		posTarget.y	=	posArrive.y;

		int	iRange	=	m_lpSkill->m_bf8ParallellRushGab;
		iRange		*=	(iIndex/2+1);

		g_protractor.getAnglePos(&pos,iAngle,iRange);
		g_protractor.getAnglePos(&posTarget,iAngle,iRange);

		m_lpCaster->m_pos.x	=	pos.x;
		m_lpCaster->m_pos.y	=	pos.y;

		if	(!m_lpCaster->m_lpField->isMissileBlockedLine(m_pos.x,m_pos.y,pos.x,pos.y))	//	막혀 있지 않다면..
			m_lpCaster->m_lpField->operateRushSkill(m_lpCaster,m_lpTarget,posTarget.x,posTarget.y,&m_ability,&posDummy,&info,TRUE);

		iShotCount--;
		iIndex++;

		m_lpCaster->setActorPos(m_pos.x,m_pos.y);
	}

	m_lpCaster->setActorPos(posArrive.x,posArrive.y,TRUE,TRUE);
}

BOOL
CActiveSkill::updateRushTypeSkill()
{
	if(!m_lpCaster)
		return FALSE;

	m_lpCaster->setRemainActionTime(4);	

	if	(m_wFrameCounter	!=	0xffff)
	{
		if	(decreaseRushSkillRequireFrame())
		{
			crushRushSkill();

			if	(m_lpCaster)
				m_lpCaster->setRemainActionTime(1);	
			
			return	TRUE;
		}

		return	FALSE;
	}

	if	(isWhirlRunningStyleRush())
	{
		castRushTypeSkill(TRUE);

		return	FALSE;
	}

	if	(isActionTime())
	{
		castRushTypeSkill();

		if	(m_lpCaster)
			m_lpCaster->setRemainActionTime(1);	

		return	TRUE;
	}

	return	FALSE;
}