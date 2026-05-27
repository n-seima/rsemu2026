#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cFIELD.H"
#include "cGAME.H"

//
//	미사일형 스킬 사용
int
CActiveSkill::fireHealTypeSkill()
{
	int		iHealRange	=	m_ability.getHitRange(NULL);

	if	(m_lpTarget	&&	iHealRange	==	0)
	{
		BOOL	isUseless		=	TRUE;
		int		iHealPoint		=	m_ability.getHealPoint();
		int		iHealPercentage	=	m_ability.getHealPercentageBasedRemainHP();

		if	(iHealPoint || iHealPercentage)
		{
			if	(m_lpTarget->m_iHP	>=	m_lpTarget->getMaxHP())
				isUseless	=	TRUE;
			else
				isUseless	=	FALSE;
		}

		if	(isUseless)
		{
			if	(m_lpSkill->isCureStrangeStatus()		&&	m_lpTarget->isStrangeStatus()		)	isUseless	=	FALSE;
			if	(m_lpSkill->isCureDeclinePowerStatus()	&&	m_lpTarget->isDeclinePowerStatus()	)	isUseless	=	FALSE;
			if	(m_lpSkill->isCureCurseStatus()			&&	m_lpTarget->isCurseStatus()			)	isUseless	=	FALSE;
			if	(m_lpSkill->isCureAllStrangeStatus()	&&	m_lpTarget->isWrongStatus()			)	isUseless	=	FALSE;
			if	(m_lpSkill->isResurrection()			&&	m_lpTarget->isDeath()				)	isUseless	=	FALSE;
		}

		if	(isUseless)
		{
			if	(m_lpCaster->isPlayer())
				m_lpCaster->sendUseSkillResult(eAR_NOT_APT_USED_SKILL_TO_TARGET);

			return	eSKILL_FIRE_RESULT_NOT_APT_SKILL_TO_TARGET;
		}
	}

	if (m_lpTarget	&&	m_lpSkill->isResurrection()	&&	m_lpTarget->isDeath())
	{
		int	iRestoreExpPercentage	=	(short)m_lpSkill->m_aValue[1]+(short)m_lpSkill->m_aValue[0]*m_ability.m_wLevel;
		iRestoreExpPercentage		=	max(iRestoreExpPercentage,0);
		iRestoreExpPercentage		=	min(iRestoreExpPercentage,100);

		setRestoreExpPercentageForReviveSkill(iRestoreExpPercentage);
	}

	m_lpCaster->action(m_lpSkill->m_wAction);

	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	힐타입 스킬 업데이터
BOOL
CActiveSkill::updateHealTypeSkill()
{
	if	(!isActionTime())
		return	FALSE;	//	아직 발동 직전이다.
	
	if (m_lpTarget == NULL)
		return	TRUE;

	if (m_lpTarget->m_wSerialInField	==	0xffff)
		return	TRUE;

	int	iHealRange	=	m_ability.getHitRange(NULL);
	int	iHealPoint	=	m_ability.getHealPoint();

	if	(iHealPoint	==	0)
	{
		LONGLONG	llHP	=	m_lpTarget->m_iHP;

		if(!m_lpTarget->isDeath() && llHP < 100)
			llHP = 100;

		llHP		=	llHP*m_ability.getHealPercentageBasedRemainHP()/100;
		iHealPoint	=	(int)llHP;
	}

	iHealRange		*=	iHealRange;

	int	iTR			=	m_lpCaster->isAbleToTransformation(TRUE);

	if	(m_lpTarget	&&	iHealRange	==	0)
	{
		if	(!m_lpTarget	||	!m_lpTarget->m_lpField	||	!m_lpSkill)
			return	TRUE;

		if	(m_lpSkill->m_bf1IsApplyHealPointByPercentage)
		{
			LONGLONG	llHP	=	iHealPoint/100;
			llHP				=	llHP*m_lpTarget->getMaxHP()/100;
			iHealPoint			=	(int)llHP;
		}

		m_lpTarget->cure(m_lpCaster,&m_ability,iHealPoint,getRestoreExpPercentageForReviveSkill());

		if	(!m_lpTarget	||	!m_lpTarget->m_lpField	||	!m_lpSkill)
			return	TRUE;

		{
			if	(g_bIsDuelServer)
				iHealPoint	=	iHealPoint*g_iCorrectHealEfficientUnderDuelBattle/100;
			
			m_lpTarget->correctValueEffectDuelServer(&iHealPoint , m_lpSkill);

			m_lpTarget->m_lpField->sendCureActorMessage(m_lpTarget->m_wSerialInField,m_lpSkill->m_wSerial,iHealPoint);
		}

		if	(iTR	==	eTRANSFORMATION_SUCCESS	&&	m_ability.isJobChange())
			m_lpCaster->sendTransformation(eTR_BY_SKILL,m_lpSkill->m_bf1PutTrasAfterAnm);

		return	TRUE;
	}

	if (iHealRange	==	0)
	{
		m_lpCaster->cure(m_lpCaster,&m_ability,iHealPoint,getRestoreExpPercentageForReviveSkill());

		if	(g_bIsDuelServer)
			iHealPoint	=	iHealPoint*g_iCorrectHealEfficientUnderDuelBattle/100;

		m_lpCaster->m_lpField->sendCureActorMessage(m_lpCaster->m_wSerialInField,m_lpSkill->m_wSerial,iHealPoint);

		if	(iTR	==	eTRANSFORMATION_SUCCESS	&&	m_ability.isJobChange())
			m_lpCaster->sendTransformation(eTR_BY_SKILL,m_lpSkill->m_bf1PutTrasAfterAnm);

		return	TRUE;
	}

	if (m_lpCaster->getPartySerial()	==	0xffff)
	{
		int	iRange	=	GetOvalRange(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,m_posTarget.x,m_posTarget.y);

		if	(iRange	<	iHealRange)	
			m_lpCaster->cure(m_lpCaster,&m_ability,iHealPoint,getRestoreExpPercentageForReviveSkill());

		if	(g_bIsDuelServer)
			iHealPoint	=	iHealPoint*g_iCorrectHealEfficientUnderDuelBattle/100;

		m_lpCaster->m_lpField->sendCureActorMessage(m_lpCaster->m_wSerialInField,m_lpSkill->m_wSerial,iHealPoint);

		if	(iTR	==	eTRANSFORMATION_SUCCESS	&&	m_ability.isJobChange())
			m_lpCaster->sendTransformation(eTR_BY_SKILL,m_lpSkill->m_bf1PutTrasAfterAnm);

		return	TRUE;
	}

	m_lpCaster->m_lpField->cureRangedPartyMember(m_lpCaster,&m_ability,iHealPoint,m_posTarget.x,m_posTarget.y,iHealRange,m_lpCaster->getPartySerial());

	if	(iTR	==	eTRANSFORMATION_SUCCESS	&&	m_ability.isJobChange())
		m_lpCaster->sendTransformation(eTR_BY_SKILL,m_lpSkill->m_bf1PutTrasAfterAnm);

	return	TRUE;
}