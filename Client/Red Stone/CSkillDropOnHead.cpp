#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	연속 타격 스킬
int
CActiveSkill::fireDropOnHeadTypeSkill()
{
	CRY(!m_lpTarget || !m_lpCaster,"cry in CActiveSkill::fireDropOnHeadTypeSkill()");
//		return	eSKILL_FIRE_RESULT_FAILED;
	m_lpCaster->action(m_lpTarget->m_pos.x,m_lpTarget->m_pos.y,m_lpSkill->m_wAction);
	m_hitInfo.copy(&m_lpCaster->m_attackInfo.m_aInfo[0]);

	return	eSKILL_FIRE_RESULT_OK;
}	//	CActiveSkill::fireDropOnHeadTypeSkill()

//
//	캐스팅
void
CActiveSkill::castDropOnHeadTypeSkill()
{
	if (m_lpSkill->m_wExplosionImage==	0xffff	)	return;

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wExplosionImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wExplosionImage]->GetFPS(0);
	m_wFrameCounter	=	0;		//	바로시작

	g_esm.playActionSound(m_lpSkill);
//	m_sHeight		=	m_lpTarget->getBodyHeight()*3/4;
}	//	CActiveSkill::castDropOnHeadTypeSkill()

//
//	연속 타격 스킬
BOOL
CActiveSkill::updateDropOnHeadTypeSkill()
{
	if (m_wFrameCounter	==	0xffff	)
	{
		if (m_lpCaster->m_isTriggerFrame)	castDropOnHeadTypeSkill();

		return	FALSE;	//	아직 쏘기 직전이다.
	}

//	프레임 갱신
	{
		while(m_wFrameCounter	>=	dSYNC_FPS)
		{
			m_wFrameCounter	-=	dSYNC_FPS;
			m_wFrame++;

			if (m_wFrame	>=	g_apEffect[m_lpSkill->m_wExplosionImage]->GetFrameCount(m_wAnm))	return	TRUE;

			if (g_apEffect[m_lpSkill->m_wExplosionImage]->IsTrigger(m_wAnm,m_wFrame))
			{
				if (m_lpCaster)
					if (m_lpTarget)
						m_lpCaster->strike(m_lpTarget,&m_ability,&m_hitInfo,0,TRUE);
			}
		}

		m_wFrameCounter	+=	m_wFPS;
	}

	return	FALSE;
}	//	CActiveSkill::updateDropOnHeadTypeSkill()


//
//머리위로 떨어지는 스킬
void
CActiveSkill::putDropOnHeadTypeSkill()
{
	if (m_wFrameCounter	==	0xffff	)
		return;

	if (m_lpTarget)
	{
		m_posTarget.x	=	GetScaledXPos(m_lpTarget->m_pos.x);
		m_posTarget.y	=	GetScaledYPos(m_lpTarget->m_pos.y)-m_lpTarget->getHeight();
	}

	g_apEffect[m_lpSkill->m_wExplosionImage]->PutReg(m_posTarget.x,m_posTarget.y,m_wAnm,m_wDirect,m_wFrame,g_iGameScale,g_iGameScale);
}