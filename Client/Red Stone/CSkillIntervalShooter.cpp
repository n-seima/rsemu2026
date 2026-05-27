#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	¹ ½´
int
CActiveSkill::fireIntervalShooter()
{
	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);
	m_lpCaster->m_iFPS	=	getIntervalShooterActionFPS();

	resetContinuousAttackShotIndex();
	resetContinuousAttackAnm();
	resetIsIntervalShooterStrikeTime();

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	½ºÆ®¶ ¹ ½´
BOOL
CActiveSkill::updateStrikeIntervalShooter()
{
	int	iCounter		=	getIntervalShooterStrikeCounter();

	if (iCounter%(dSYNC_FPS/8)	==	0)
	{
		int	iShotIndex	=	getIntervalShooterShotCounter();
		int	iPhysicalDamage,iMagicDamage;

		getIntervalShooterDamage(&iPhysicalDamage,&iMagicDamage);

		iPhysicalDamage	=	iPhysicalDamage/getIntervalShooterShotCount();
		iMagicDamage	=	iMagicDamage/getIntervalShooterShotCount();

		if (iPhysicalDamage)	iPhysicalDamage	=	iPhysicalDamage	+ random(iPhysicalDamage/10)-iPhysicalDamage/20;
		if (iMagicDamage)		iMagicDamage	=	iMagicDamage	+ random(iMagicDamage/10)-iMagicDamage/20;

		CHitInfo	hitInfo;

		hitInfo.m_dwResultField	=	0;

		hitInfo.setHit();

		hitInfo.m_dwPhysicalDamage	=	iPhysicalDamage;
		hitInfo.m_dwMagicDamage		=	iMagicDamage;

		g_esm.playActionSound(m_lpSkill);

		g_sm.castSpecialMissile(NULL,m_lpTarget,m_pos.x,m_pos.y,&hitInfo,&m_ability);

		if (increaseIntervalShooterShotCounter())	return	TRUE;
	}

	increaseIntervalShooterStrikeCounter();

	return	FALSE;
}


//
//	¾÷Æ®
BOOL
CActiveSkill::updateIntervalShooter()
{
	if (isIsIntervalShooterStrikeTime())
		return	updateStrikeIntervalShooter();

	if (m_lpCaster->isActionFinished())
	{
		if (increaseContinuousAttackShotIndex())
		{
			m_lpCaster->stop(FALSE);
			return	TRUE;
		}

		m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction+getContinuousAttackAnm());
		m_lpCaster->setExclusiveAction(TRUE);
		m_lpCaster->m_iFPS	=	getIntervalShooterActionFPS();
		g_esm.playCastingSound(m_lpSkill);

		operateSkillEffect();
	}

	if (m_lpCaster->m_isTriggerFrame)
	{
		CPos		pos;

		m_lpCaster->getReleasePos(&pos);

		g_sm.castCustomMissile(pos.x,pos.y,pos.x,pos.y-400,&m_ability);
	}

	return	FALSE;
}