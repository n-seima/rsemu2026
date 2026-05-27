#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	연사
int
CActiveSkill::fireContinuousShoot()
{
	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);
	m_lpCaster->m_iFPS	=	getContinuousAttackActionFPS();
	resetContinuousAttackShotIndex();
	resetContinuousAttackAnm();

	return	eSKILL_FIRE_RESULT_OK;
}	//	CActiveSkill::fireContinuousShoot()

//
//	업데이트

BOOL
CActiveSkill::updateContinuousShoot()
{
	if	(!m_lpCaster)
		return	TRUE;

	if	(m_lpCaster->isActionFinished())
	{
		if	(increaseContinuousAttackShotIndex())
		{
			m_lpCaster->stop(FALSE);
			return	TRUE;
		}

		increaseContinuousAttackAnm();

		m_lpCaster->action(m_lpTarget->m_pos.x,m_lpTarget->m_pos.y,m_lpSkill->m_wAction+getContinuousAttackAnm());
		m_lpCaster->setExclusiveAction(TRUE);
		m_lpCaster->m_iFPS	=	getContinuousAttackActionFPS();
		g_esm.playCastingSound(m_lpSkill);

		operateSkillEffect();
	}

	if	(m_lpCaster->m_isTriggerFrame)
	{
		int			iHitIndex	=	getContinuousAttackShotIndex();
		CHitInfo	*lpHitInfo	=	&m_lpCaster->m_attackInfo.m_aInfo[iHitIndex];
		CPos		pos			=	m_pos;
		int			iBodySize	=	m_lpCaster->getBodySize()+30;
		int			iHeight		=	m_lpCaster->getHeight()+m_lpCaster->getBodyHeight(FALSE)*2/3;

		pos.x					=	m_lpCaster->m_pos.x;
		pos.y					=	m_lpCaster->m_pos.y;

		GetTargetPos(&pos,m_posTarget.x,m_posTarget.y,iBodySize);

		pos.y					-=	iHeight;
		BOOL		bIsMagicMissile	=	 m_lpSkill->m_aValue[3];
		if(m_lpSkill && m_lpSkill->m_bf1IsApplyFightingSpirit ||bIsMagicMissile) 
			g_sm.castSpecialMissile(m_lpCaster,m_lpTarget,pos.x,pos.y,lpHitInfo,&m_ability);
		else
			g_sm.castSpecialMissile(NULL,m_lpTarget,pos.x,pos.y,lpHitInfo,&m_ability);

	}

	return	FALSE;
}