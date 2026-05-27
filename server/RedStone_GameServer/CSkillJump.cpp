#include <math.h>
#include "CActiveSkill.H"
#include "CActor.H"
#include "CField.H"

//
//	머신타입 스킬 사용
int
CActiveSkill::fireJump(BOOL _bIsWildStamp)
{
	setIsWildStamp(_bIsWildStamp);
	m_lpCaster->action(m_lpSkill->m_wAction);

	m_wAngle		=	g_protractor.getAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y);

	getActionTime();

	if	(m_lpCaster->isSealdMove())
		return	eSKILL_FIRE_RESULT_FAILED;

	return	eSKILL_FIRE_RESULT_OK;
}

//
//
BOOL
CActiveSkill::castJump()
{
	m_lpCaster->increaseCP(m_ability.getGetCP());
	m_wFrameCounter	=	0;

	if (!m_lpCaster->setActorPos(m_posTarget.x,m_posTarget.y,TRUE,TRUE))
		return	FALSE;

	int	iRange	=	GetOvalRange(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y);		//	거리다.. --;;
	iRange		=	(int)sqrt((double)iRange);

	int	iTime	=	iRange*100/m_lpSkill->m_wSpeed;
	iTime		=	iTime*dSYNC_FPS/100;

	setJumpFlyTime(iTime);
	
	m_lpCaster->releaseIcyStalagmite(eRUIS_RELEASE);
	m_lpCaster->setJumpFlyTime(iTime);	//	점프해서 나는중이다. 건들지 말어!!
	m_lpCaster->sendTeleport(m_ability.m_wSkill,eTM_JUMP);

	return	TRUE;
}

BOOL
CActiveSkill::updateJump()
{
	if(!m_lpCaster)
		return FALSE;
	
	m_lpCaster->setRemainActionTime(4);

	if (m_wFrameCounter	==	0xffff)
	{
		if (isActionTime())
			castJump();

		return	FALSE;
	}

	if (decreaseJumpFlyTime())
	{
		if (isWildStamp())
		{
			BOOL	bIsGetedCP	=	TRUE;

			m_lpCaster->m_lpField->strikeExplosionSkill(m_lpCaster,m_lpTarget,m_posTarget.x,m_posTarget.y,&m_ability,&bIsGetedCP);
		}
		
		m_lpCaster->setRemainActionTime(1);

		int	iKnockBackDistance	=	m_ability.getKnockBackDistance();

		if (iKnockBackDistance	<=	0)	
			return TRUE;

		int	iAbilityRange	=	m_ability.getHitRange(NULL);
		int	iThrustRange	=	iAbilityRange+m_lpCaster->getBodySize();

		m_lpCaster->m_lpField->thrustActor(m_posTarget.x,m_posTarget.y,m_lpCaster,m_lpTarget,m_lpCaster->m_wActorKind,&m_ability,iThrustRange,iKnockBackDistance,m_wAngle);

		return	TRUE;
	}

	return	FALSE;
}