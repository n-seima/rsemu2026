#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	텔레포트 사용
int
CActiveSkill::fireTeleport()
{
	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);

	return	eSKILL_FIRE_RESULT_OK;
}

void
CActiveSkill::castTeleport()
{
	if (m_lpCaster->isHero())
		g_hero.increaseCP(m_ability.getGetCP());

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFPS(0);

	m_wFrameCounter	=	0;						//	바로시작
	m_wAnm			=	0;
	m_wDirect		=	0;
	m_wFrame		=	0;

	g_esm.playActionSound(m_lpSkill);
}

//
//	텔레포트 업데이트
BOOL
CActiveSkill::updateTeleport()
{
	if (m_wFrameCounter	==	0xffff	)
	{
		if (m_lpCaster->m_isTriggerFrame)	castTeleport();

		return	FALSE;	//	아직 쏘기 직전이다.
	}

	if (m_wFrameCounter	>=	dSYNC_FPS)
	{
		m_wFrameCounter	-=	dSYNC_FPS;
		m_wFrame++;

		if (m_wFrame	>=	m_wMaxFrame)	return	TRUE;
	}

	m_wFrameCounter	+=	m_wFPS;

	return	FALSE;
}

//
//	텔레포트 업데이트
void
CActiveSkill::putTeleport()
{
	if (m_lpSkill->m_wSkillImage==	0xffff)	return;
	if (m_wFrameCounter			==	0xffff)	return;	//	아직 쏘기 직전이다.

	int	iX	=	GetScaledXPos(m_pos.x);
	int	iY	=	GetScaledYPos(m_pos.y);

	g_apEffect[m_lpSkill->m_wSkillImage]->PutReg(iX,iY,m_wAnm,m_wDirect,m_wFrame,g_iGameScale,g_iGameScale);
}