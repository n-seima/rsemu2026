#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

int
CActiveSkill::fireJumpAttack()
{
	m_lpCaster->m_wJumpStatus			=	eJS_READY;
	m_lpCaster->m_wUseSpecialActionSkill=	eSAS_JUMP;

	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);

	m_wFrameCounter					=	0xffff;

	int	iRange						=	GetOvalRange(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y);		//	°¸®´.. --;;
	m_wRange						=	(int)sqrt((double)iRange);
	m_wRange						=	max(m_wRange,1);

	m_lpCaster->m_wJumpTargetRange	=	m_wRange;
	m_lpCaster->m_wJumpMaxRange		=	m_ability.getAttackRange(NULL,0);

	m_hitInfo.copy(&m_lpCaster->m_attackInfo.m_aInfo[0]);

	m_aValue[0]						=	0;

	return	eSKILL_FIRE_RESULT_OK;
}

void
CActiveSkill::castJumpAttack()
{
	g_esm.playActionSound(m_lpSkill);
	m_wFrameCounter	=	0;
	g_lpHero->m_bIsJumpAttack = TRUE;

	m_lpCaster->jump(m_posTarget.x,m_posTarget.y,m_ability.m_wSkill);

	if	(m_lpSkill->m_wSkillImage==	0xffff	)
		return;

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFPS(0);
	m_wAnm			=	0;
}

BOOL
CActiveSkill::updateJumpAttack()
{
	if	(m_wFrameCounter	==	0xffff)
	{
		if	(m_lpCaster->m_wJumpStatus	==	eJS_WAIT_TRIGGER)
			castJumpAttack();

		return	FALSE;
	}

	m_aValue[0]++;

	if	(m_aValue[0]	>=	dSYNC_FPS*3)
	{
		m_lpCaster->stop();
		return	TRUE;
	}

	if	(m_lpCaster->m_wJumpStatus	==	eJS_FINISH || m_lpCaster->m_wJumpStatus	==	eJS_NONE)
	{
		if	(m_lpTarget && m_lpCaster)
			m_lpCaster->strike(m_lpTarget,&m_ability,&m_hitInfo,0,TRUE);

		if	(m_lpCaster && m_lpSkill->m_wIsNotSyncCP != TRUE && m_lpCaster->isHero())
			g_hero.increaseCP(m_ability.getGetCP());

		return	TRUE;
	}

	return	FALSE;
}

void
CActiveSkill::putJumpAttack()
{
	
}