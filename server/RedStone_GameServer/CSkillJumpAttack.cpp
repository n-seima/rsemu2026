#include <math.h>
#include "CActiveSkill.H"
#include "CActor.H"
#include "CField.H"

//
//	머신타입 스킬 사용
int
CActiveSkill::fireJumpAttack()
{
	if	(m_lpTarget	==	NULL)
		return	eSKILL_FIRE_RESULT_FAILED;

	if	(m_lpCaster->isSealdMove())
		return	eSKILL_FIRE_RESULT_FAILED;

	m_lpTarget->getDamage(m_lpCaster,&m_ability,0,0,&m_hitInfo);
	m_lpCaster->m_attackInfo.reset();
	m_lpCaster->m_attackInfo.m_aInfo[0].copy(&m_hitInfo);

	m_lpCaster->action(m_lpSkill->m_wAction);

	m_wAngle			=	g_protractor.getAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y);
	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}

//
//
BOOL
CActiveSkill::castJumpAttack()
{
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

	m_pos.x		=	m_posTarget.x;
	m_pos.y		=	m_posTarget.y;

	return	TRUE;
}

BOOL
CActiveSkill::updateJumpAttack()
{
	m_lpCaster->m_wRemainActionTime	=	4;

	if (m_wFrameCounter	==	0xffff)
	{
		if (isActionTime())
			castJumpAttack();

		return	FALSE;
	}

	if (decreaseJumpFlyTime())
	{
		int	iRange	=	m_lpCaster->getWeaponAttackRange();

		if (isCorrectTarget() && m_lpTarget->isInAttackRange(m_pos.x,m_pos.y,iRange))
			m_lpCaster->strike(m_lpTarget,&m_hitInfo,&m_ability);

		m_lpCaster->increaseCP(m_ability.getGetCP());

		int	iKnockBackDistance	=	m_ability.getKnockBackDistance();

		m_lpCaster->m_wRemainActionTime	=	1;

		if (iKnockBackDistance	<=	0)	
			return TRUE;

		int	iAbilityRange	=	m_ability.getHitRange(NULL);
		int	iThrustRange	=	iAbilityRange+m_lpCaster->getBodySize();

		m_lpCaster->m_lpField->thrustActor(m_posTarget.x,m_posTarget.y,m_lpCaster,m_lpTarget,m_lpCaster->m_wActorKind,&m_ability,iThrustRange,iKnockBackDistance,m_wAngle);

		return	TRUE;
	}

	return	FALSE;
}