#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"
#include "CGamePlay.h"

//
int
CActiveSkill::fireSurprisingRaid()
{
	CRY(!m_lpCaster,"cry in CActiveSkill::fireSurprisingRaid()");

	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);

	m_hitInfo.copy(&m_lpCaster->m_attackInfo.m_aInfo[0]);

	memcpy(&m_lpCaster->m_abilityRush,&m_ability,sizeof(m_ability));

	if (m_lpSkill->m_wExplosionImage	==	0xffff)
		return eSKILL_FIRE_RESULT_OK;

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wExplosionImage]->GetFrameCount(0);
	m_wFrameCounter	=	0;
	m_wFPS			=	g_apEffect[m_lpSkill->m_wExplosionImage]->GetFPS(0);

	if (m_lpSkill->m_wShakeIntensity)
		g_shaker.add(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,m_lpSkill->m_wShakeIntensity,m_lpSkill->m_wShakeTime,m_ability.getHitRange(NULL));

	setSurprisingRaidStep(0);

	return	eSKILL_FIRE_RESULT_OK;
}

void
CActiveSkill::castSurprisingRaid()
{
	m_lpCaster->m_wWaitRushTriggerTime	=	0;
	m_lpCaster->m_wRushStatus			=	eRS_WAIT_TRIGGER;
	m_lpCaster->m_wUseSpecialActionSkill=	eSAS_RUSH;
	
	g_esm.playActionSound(m_lpSkill);
	m_wFrameCounter	=	0;

	setSurprisingRaidStep(2);

	if (m_lpSkill->m_wSkillImage==	0xffff	)
		return;

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFPS(0);
	m_wAnm			=	0;
	m_wFrame		=	0;
}	//	CActiveSkill::castRushTypeSkill()

//
BOOL
CActiveSkill::updateSurprisingRaid()
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

	if (getSurprisingRaidStep()	==	0)
	{
		if (m_lpCaster->isDamageFrame())
			setSurprisingRaidStep(1);

		return	FALSE;
	}

	if (getSurprisingRaidStep()	==	1)
	{
		if (m_lpCaster->isTriggerFrame())
		{
			castSurprisingRaid();

			return	FALSE;
		}
	}

	if (getSurprisingRaidStep()	==	2)
	{
		if (m_lpCaster->m_wRushStatus	==	eRS_FINISH || m_lpCaster->m_wRushStatus	==	eRS_NONE)
		{
			if (m_lpCaster	&&	m_lpTarget)
				m_lpCaster->strike(m_lpTarget,&m_ability,&m_hitInfo,0,FALSE,TRUE);

			return	TRUE;
		}
	}

	return	FALSE;
}

//
//	그려!!
void
CActiveSkill::putSurprisingRaid()
{
	int	iX	=	GetScaledXPos(m_pos.x);
	int	iY	=	GetScaledYPos(m_pos.y);

	if (getSurprisingRaidStep()	<=	1)
	{
		if (m_lpSkill->m_wExplosionImage	==	0xffff)
			return;

		g_apEffect[m_lpSkill->m_wExplosionImage]->PutReg(iX,iY-m_sHeight,m_wAnm,m_wDirect,m_wFrame,g_iGameScale,g_iGameScale);

		return;
	}

	if (getSurprisingRaidStep()	==	2)
	{
		if (m_lpSkill->m_wSkillImage==	0xffff)
			return;
		if (m_wFrameCounter			==	0xffff)
			return;

		CPos	pos;

		m_lpCaster->getReleasePos(&pos);

		cANM	*lpEffect	=	g_im.getEffect(m_lpSkill->m_wSkillImage);

		lpEffect->PutReg(iX,iY,m_wAnm,m_lpCaster->m_iDirect,m_wFrame,g_iGameScale,g_iGameScale);
	}
}