#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

int
CActiveSkill::fireBunshine()
{
	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);

	int	iBunshineCount		=	getBunshineAttackShotCount();

	m_lpCaster->m_wParallelBunshineCount	=	iBunshineCount;

	if (m_lpSkill->m_bf1IsRoundRappedBunshinAttack)
		m_lpCaster->m_wParallelBunshineAngleGap	=	360/(iBunshineCount+1);
	else
		m_lpCaster->m_wParallelBunshineAngleGap	=	m_lpSkill->m_wAngle;

	m_lpCaster->m_posTarget.x				=	m_lpTarget->m_pos.x;
	m_lpCaster->m_posTarget.y				=	m_lpTarget->m_pos.y;

	return	eSKILL_FIRE_RESULT_OK;
}

void
CActiveSkill::castBunshine()
{
	int	iDamage	=	0;

	for (int i=0;i<getBunshineAttackShotCount();i++)
	{
		CHitInfo	*lpHitInfo	=	&m_lpCaster->m_attackInfo.m_aInfo[i];

		iDamage	+=	lpHitInfo->getDamage();

		if (i== 0)
			m_lpCaster->strike(m_lpTarget,&m_ability,lpHitInfo,0,FALSE);
		else
			m_lpCaster->strike(m_lpTarget,&m_ability,lpHitInfo,0,FALSE,FALSE);
	}

	if (iDamage && m_lpCaster->isHero())
		g_hero.increaseCP(m_ability.getGetCP());

	g_esm.playActionSound(m_lpSkill);
}

BOOL
CActiveSkill::updateBunshine()
{
	if (m_lpCaster->m_isTriggerFrame)
	{
		castBunshine();

		return	TRUE;
	}

	return	FALSE;
}

void
CActiveSkill::putBunshine()
{

}
