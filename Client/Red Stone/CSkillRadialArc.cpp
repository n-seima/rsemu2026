#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"
#include "CGamePlay.h"

//
//	회오리형 스킬 사용
int
CActiveSkill::fireRadialArc()
{
	CRY(!m_lpCaster,"cry in CActiveSkill::fireWhirlwindTypeSkill()");

	m_lpCaster->action(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,m_lpSkill->m_wAction);

	return	eSKILL_FIRE_RESULT_OK;
}	//	CActiveSkill::fireExplosionTypeSkill()

BOOL
CActiveSkill::castRadialArc()
{
	if (m_lpSkill->m_wSkillImage	==	0xffff)
		return	FALSE;

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFrameCount(0);
	m_wFrameCounter	=	0;
	m_wFPS			=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFPS(0);

	return	TRUE;
}

//
//	회오리형 업데이트
BOOL
CActiveSkill::updateRadialArc()
{
	if (m_wFrameCounter	==	0xffff)
	{
		if (m_lpCaster->isTriggerFrame())
			if (!castRadialArc())
				return	TRUE;

		return	FALSE;
	}

	if (m_wFrameCounter	>=	dSYNC_FPS)		//	프레임 갱신
	{
		m_wFrameCounter	-=	dSYNC_FPS;
		m_wFrame++;

		if (m_wFrame	>=	m_wMaxFrame)
			m_wFrame	=	0;
	}
	
	m_wFrameCounter	+=	m_wFPS;

	return	FALSE;
}

//
//	그려!!
void
CActiveSkill::putRadialArc()
{
	if (m_lpSkill->m_wExplosionImage	==	0xffff)
		return;

	int	iX	=	GetScaledXPos(m_pos.x);
	int	iY	=	GetScaledYPos(m_pos.y);

	g_apEffect[m_lpSkill->m_wExplosionImage]->PutReg(iX,iY-m_sHeight,m_wAnm,m_wDirect,m_wFrame);
}