#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"
#include "CGamePlay.h"

//
int
CActiveSkill::fireWhirlthrusting()
{
	CRY(!m_lpCaster,"cry in CActiveSkill::fireWhirlthrusting()");

	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);

	m_hitInfo.copy(&m_lpCaster->m_attackInfo.m_aInfo[0]);

	if (m_lpSkill->m_wExplosionImage	==	0xffff)
		return eSKILL_FIRE_RESULT_OK;

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wExplosionImage]->GetFrameCount(0);
	m_wFrameCounter	=	0;
	m_wFPS			=	g_apEffect[m_lpSkill->m_wExplosionImage]->GetFPS(0);

	if (m_lpSkill->m_wShakeIntensity)
		g_shaker.add(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,m_lpSkill->m_wShakeIntensity,m_lpSkill->m_wShakeTime,m_ability.getHitRange(NULL));

	setWhirlthrustingStep(0);

	return	eSKILL_FIRE_RESULT_OK;
}

//
BOOL
CActiveSkill::updateWhirlthrusting()
{
	if (m_wFrame	!=	0xffff)
	{
		if (m_wFrameCounter	>=	dSYNC_FPS)		//	프레임 갱신
		{
			m_wFrameCounter	-=	dSYNC_FPS;
			m_wFrame++;

			if (m_wFrame>=m_wMaxFrame)
				m_wFrame	=	0xffff;
		}
		
		m_wFrameCounter	+=	m_wFPS;
	}

	if (getWhirlthrustingStep()	==	0)
	{
		if (m_lpCaster->isDamageFrame())
			setWhirlthrustingStep(1);

		return	FALSE;
	}

	if (getWhirlthrustingStep()	==	1)
	{
		if (m_lpCaster->isDamageFrame())
		{
			if (m_lpCaster	&&	m_lpTarget)
				m_lpCaster->strike(m_lpTarget,&m_ability,&m_hitInfo);

			return	TRUE;
		}
	}


	return	FALSE;
}

//
//	그려!!
void
CActiveSkill::putWhirlthrusting()
{
	if (m_lpSkill->m_wExplosionImage	==	0xffff)	return;

	int	iX	=	GetScaledXPos(m_pos.x);
	int	iY	=	GetScaledYPos(m_pos.y);

	g_apEffect[m_lpSkill->m_wExplosionImage]->PutReg(iX,iY-m_sHeight,m_wAnm,m_wDirect,m_wFrame,g_iGameScale,g_iGameScale);
}