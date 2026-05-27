#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"
#include "CGamePlay.h"

//
//	폭
int
CActiveSkill::fireContinuousWhirlwindSkill()
{
	CRY(!m_lpCaster,"cry in CActiveSkill::fireExplosionTypeSkillDependOnImage()");

	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);
	
	setExplosionWhichDefendOnImageSkillScale(20);
	setExplosionWhichDefendOnImageSkillZoommingStart();
	setExplosionWhichDefendOnImageSkillFrameStart();

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	
BOOL
CActiveSkill::castContinuousWhirlwindSkill()
{
	g_esm.playActionSound(m_lpSkill);

	if (m_lpSkill->m_wExplosionImage==	0xffff	)
		return FALSE;

	if	(m_lpSkill->m_bf1IsRefitImageSizeByHitRange)
	{
		int	iHitRange	=	m_ability.getHitRange(NULL);
		int	iScale		=	iHitRange*100/m_lpSkill->m_wImageRadius;

		setExplosionWhichDefendOnImageSkillScale(iScale);
	}

	int	iCastingTime	=	m_lpSkill->m_sCastingTime*dSYNC_FPS/1000;

	setDependOnImageExplosionSkillCastingTime(iCastingTime);
	setDependOnImageExplosionSkillHitCount();

	int	iDirectCount;

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wExplosionImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wExplosionImage]->GetFPS(0);
	m_wFrameCounter	=	0;		//	바로시작
	m_wAnm			=	0;

	m_wFPS			=	m_wFPS*m_lpSkill->m_wCorrectFPS/100;

	m_wDirect		=	m_lpCaster->m_iDirect;
	iDirectCount	=	g_apEffect[m_lpSkill->m_wExplosionImage]->GetDirectCount(0);

	m_wDirect		=	min(iDirectCount-1,m_wDirect);

	int	iUpkeepTime	=	m_lpSkill->m_wUpkeepTime+m_lpSkill->m_wUpkeepTimePerLevel*m_ability.m_wLevel;

	setContinuousWhirlwindSkillUpkeepTime(timeGetTime()+iUpkeepTime*10);

	if (m_lpSkill->m_wShakeTiming	==	eST_CAST)
		if (m_lpSkill->m_wShakeIntensity)
			g_shaker.add(m_pos.x,m_pos.y,m_lpSkill->m_wShakeIntensity,m_lpSkill->m_wShakeTime,m_ability.getHitRange(NULL));

	if	(m_lpSkill->m_bf1IsGetCPGasSkillByCasting	&&	m_lpCaster->isHero())
		g_hero.increaseCP(m_ability.getGetCP());

	return	TRUE;
}

//
//	업데이트
BOOL
CActiveSkill::updateContinuousWhirlwindSkill()
{
	if(!m_lpCaster)
		return	TRUE;
	if (m_wFrameCounter	==	0xffff	)
	{
		if (m_lpCaster->m_isTriggerFrame)
			if (!castGasTypeSkill())
			{
				return	TRUE;

			}

		return	FALSE;	//	아직 쏘기 직전이다.
	}

	if	(decreaseDependOnImageExplosionSkillCastingTime())
		return	FALSE;

//	프레임 갱신
	{

	m_lpCaster->setExclusiveAction(TRUE);
		while (m_wFrameCounter	>=	dSYNC_FPS)
		{
			m_wFrameCounter	-=	dSYNC_FPS;
			m_wFrame++;
			
			if (m_wFrame	>=	m_wMaxFrame)
			{
				if	(m_lpSkill->m_bf1IsLoopLastFrame)
					m_wFrame	=	m_wMaxFrame-1;
				else
					m_wFrame	=	0;

				if	(m_wAnm	==	0)
				{
					cANM	*lpImage	=	g_im.getEffect(m_lpSkill->m_wExplosionImage);

					if	(lpImage	&&	lpImage->m_iAnmCount	>	1)
					{
						m_wAnm			=	1;

						m_wMaxFrame		=	lpImage->GetFrameCount(m_wAnm);
						m_wFPS			=	lpImage->GetFPS(m_wAnm);
						m_wFrameCounter	=	0;		//	바로시작
						m_wFPS			=	m_wFPS*m_lpSkill->m_wCorrectFPS/100;
					}
				}
				
			}
		}

		m_wFrameCounter	+=	m_wFPS;
	}

	BOOL bFinish = isContinuousWhirlwindSkillFinishSkillTime(timeGetTime());
	if(bFinish)
		m_lpCaster->stop();
	return	bFinish;
}
