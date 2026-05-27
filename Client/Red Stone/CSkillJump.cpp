#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

int
CActiveSkill::fireJump()
{
	m_lpCaster->m_wJumpStatus			=	eJS_READY;
	m_lpCaster->m_wUseSpecialActionSkill=	eSAS_JUMP;

	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);

	m_wFrameCounter						=	0xffff;

	int	iRange						=	GetOvalRange(m_pos.x,m_pos.y,m_posTarget.x ,m_posTarget.y );		//	거리다.. --;;
	m_wRange						=	(int)sqrt((double)iRange);
	m_wRange						=	max(m_wRange,1);
	m_lpCaster->m_wJumpTargetRange	=	m_wRange;
	m_lpCaster->m_wJumpMaxRange		=	m_ability.getAttackRange(NULL,0);

	m_aValue[0]						=	0;

	return	eSKILL_FIRE_RESULT_OK;
}

void
CActiveSkill::castJump()
{
	g_esm.playActionSound(m_lpSkill);
	m_wFrameCounter	=	0;
	g_lpHero->m_bIsJumpAttack = FALSE;

	m_lpCaster->jump(m_posTarget.x,m_posTarget.y,m_ability.m_wSkill);

	if (m_lpSkill->m_wSkillImage==	0xffff	)
		return;

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFPS(0);
	m_wAnm			=	0;
}

BOOL
CActiveSkill::updateJump()
{
	m_aValue[0]++;

	if	(m_aValue[0]	>=	dSYNC_FPS*3)
	{
		m_lpCaster->stop();
		return	TRUE;
	}

	if	(m_wFrameCounter	==	0xffff)
	{
		if (m_lpCaster->m_wJumpStatus	==	eJS_WAIT_TRIGGER)
			castJump();

		return	FALSE;
	}

	if	(!m_lpCaster)
		return	TRUE;

	if	(m_lpCaster->m_wJumpStatus	==	eJS_FINISH || m_lpCaster->m_wJumpStatus	==	eJS_NONE)
	{
		if (m_lpSkill->m_wIsNotSyncCP != TRUE && m_lpCaster->isHero())
			g_hero.increaseCP(m_ability.getGetCP());

		return	TRUE;
	}

//	프레임 갱신
	{
		while(m_wFrameCounter	>=	dSYNC_FPS)
		{
			m_wFrameCounter	-=	dSYNC_FPS;
			m_wFrame++;

			if (m_wFrame	>=	m_wMaxFrame)
				m_wFrame	=	0;
		}

		m_wFrameCounter	+=	m_wFPS;
	}

	return	FALSE;
}

void
CActiveSkill::putJump()
{
/*	if (m_lpSkill->m_wSkillImage==	0xffff) return;
	if (m_wFrameCounter			==	0xffff) return;

	CPos	pos;

	m_lpCaster->getReleasePos(&pos);

	cANM	*lpEffect	=	g_im.getEffect(m_lpSkill->m_wSkillImage);

	lpEffect->PutReg(pos.x-g_map.m_pos.x,pos.y-g_map.m_pos.y,m_wAnm,m_lpCaster->m_iDirect,m_wFrame);
*/
}