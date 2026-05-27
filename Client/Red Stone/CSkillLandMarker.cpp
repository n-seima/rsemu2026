#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	연사
int
CActiveSkill::fireLandMarker()
{
	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);
	m_lpCaster->m_iFPS	=	getContinuousAttackActionFPS();
	resetContinuousAttackShotIndex();
	resetContinuousAttackAnm();

	return	eSKILL_FIRE_RESULT_OK;
}	//	CActiveSkill::fireLandMarker()

//
//	업데이트
BOOL
CActiveSkill::updateLandMarker()
{
	if (m_lpCaster->isActionFinished())
	{
		if (increaseContinuousAttackShotIndex())
		{
			m_lpCaster->stop(FALSE);
			return	TRUE;
		}

		increaseContinuousAttackAnm();

		m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction+getContinuousAttackAnm());
		m_lpCaster->setExclusiveAction(TRUE);
		m_lpCaster->m_iFPS	=	getContinuousAttackActionFPS();
		g_esm.playCastingSound(m_lpSkill);

		operateSkillEffect();
	}

	if (m_lpCaster->m_isTriggerFrame)
	{
		int			iHitIndex	=	getContinuousAttackShotIndex();
		CPos		pos			=	m_pos;

		GetTargetPos(&pos,m_posTarget.x,m_posTarget.y,(iHitIndex+1)*m_lpSkill->m_aValue[0]);
		
		g_sm.castExplosionMissile(m_lpCaster,NULL,pos.x,pos.y,&m_ability);
	}

	return	FALSE;
}