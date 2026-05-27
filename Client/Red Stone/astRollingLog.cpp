#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	러쉬타입 스킬 사용
int
cAS_RollingLog::fire()
{
	m_lpCaster->m_wRushStatus			=	eRS_READY;
	m_lpCaster->m_wUseSpecialActionSkill=	eSAS_RUSH;

	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);

	return	eSKILL_FIRE_RESULT_OK;
}

void
cAS_RollingLog::cast()
{
	g_esm.playActionSound(m_lpSkill);

	m_wFrameCounter					=	0;
	m_lpCaster->m_wRollingLogImage	=	m_lpSkill->m_wSkillImage;
}

BOOL
cAS_RollingLog::update()
{
	if	(m_wFrameCounter	==	0xffff)
	{
		if	(m_lpCaster->m_isTriggerFrame)
			cast();

		return	FALSE;
	}

	if	(m_lpCaster->m_wRushStatus	==	eRS_FINISH || m_lpCaster->m_wRushStatus	==	eRS_NONE)
		return	TRUE;

	return	FALSE;
}

void
cAS_RollingLog::put()
{
}