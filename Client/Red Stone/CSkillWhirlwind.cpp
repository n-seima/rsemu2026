#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"
#include "CGamePlay.h"

//
//	회오리형 스킬 사용
int
CActiveSkill::fireWhirlwindTypeSkill()
{
	CRY(!m_lpCaster,"cry in CActiveSkill::fireWhirlwindTypeSkill()");

	m_lpCaster->action(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,m_lpSkill->m_wAction);

	if (m_lpSkill->m_wExplosionImage	==	0xffff)	return eSKILL_FIRE_RESULT_OK;

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wExplosionImage]->GetFrameCount(0);
	m_wFrameCounter	=	0;
	m_wFPS			=	g_apEffect[m_lpSkill->m_wExplosionImage]->GetFPS(0);

	if (m_lpSkill->m_wShakeIntensity)
		g_shaker.add(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,m_lpSkill->m_wShakeIntensity,m_lpSkill->m_wShakeTime,m_ability.getHitRange(NULL));

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	회오리형 업데이트
BOOL
CActiveSkill::updateWhirlwindTypeSkill()
{
	if (m_wFrameCounter	>=	dSYNC_FPS)		//	프레임 갱신
	{
		m_wFrameCounter	-=	dSYNC_FPS;
		m_wFrame++;

		if (m_wFrame>=m_wMaxFrame)
			return	TRUE;
	}

	int iFPS = 100;
	if(m_lpCaster->getBody())
	{
		iFPS = m_lpCaster->getBody()->GetFPS(m_lpSkill->m_wAction);
		iFPS = m_lpCaster->m_iFPS/iFPS*100;
	}
	m_wFrameCounter	+=	m_wFPS*iFPS/100;	

	return	FALSE;
}

//
//	그려!!
void
CActiveSkill::putWhirlwindTypeSkill()
{
	if (m_lpSkill->m_wExplosionImage	==	0xffff)	return;

	int	iX	=	GetScaledXPos(m_pos.x);
	int	iY	=	GetScaledYPos(m_pos.y);

	g_apEffect[m_lpSkill->m_wExplosionImage]->PutReg(iX,iY-m_sHeight,m_wAnm,m_wDirect,m_wFrame,m_lpSkill->m_wImageScale*g_iGameScale/100,m_lpSkill->m_wImageScale*g_iGameScale/100);
}