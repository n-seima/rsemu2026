#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	연속공격 스타일 스킬 발사!! -_-?
int
CActiveSkill::fireContinuousAttackTypeSkill()
{
	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);
	m_lpCaster->m_iFPS	=	getContinuousAttackActionFPS();
	resetContinuousAttackShotIndex();
	resetContinuousAttackAnm();

	return	eSKILL_FIRE_RESULT_OK;
}	//	CActiveSkill::fireContinuousAttackTypeSkill()

//
//	업데이트
BOOL
CActiveSkill::updateContinuousAttackTypeSkill()
{
	if (m_lpCaster->isActionFinished())
	{
		if (increaseContinuousAttackShotIndex())
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

	if (m_lpCaster->m_isTriggerFrame)
	{
		int			iHitIndex	=	getContinuousAttackShotIndex();
		CHitInfo	*lpHitInfo	=	&m_lpCaster->m_attackInfo.m_aInfo[iHitIndex];

		m_lpCaster->strike(m_lpTarget,&m_ability,lpHitInfo,0,FALSE);

		if (m_lpSkill->m_wIsNotSyncCP	==	FALSE)
			if (m_lpCaster->isHero() &&	iHitIndex == 0 && lpHitInfo->getDamage())
				g_hero.increaseCP(m_ability.getGetCP());
	}

	return	FALSE;
}	//	CActiveSkill::updateContinuousAttackTypeSkill()

//
//	출력
void
CActiveSkill::putContinuousAttackTypeSkill()
{
}	//	CActiveSkill::putContinuousAttackTypeSkill()