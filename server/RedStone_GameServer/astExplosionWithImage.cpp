#include	"cActiveSkill.H"
#include	"cACTOR.H"
#include	"CField.H"
#include	"CGame.H"
#include	"astExplosionWithImage.h"

//
//	폭발형 스킬 사용
int
cAS_ExplosionWithImage::fire()
{
	m_lpCaster->action(m_lpSkill->m_wAction);

	m_wFrameCounter	=	0xffff;
	
	if	(m_lpSkill->m_bf1IsZoomInEffectSkill)	
		m_wZoomEffectTime	=	dSYNC_FPS;
	else
		m_wZoomEffectTime	=	0;

	m_iInhaleEnemyRange		=	m_lpSkill->m_aValue[1];
	m_wInhalePeroid			=	m_lpSkill->m_aValue[2];
	m_wInhalePixel			=	m_lpSkill->m_aValue[3];
	m_wMinInhaleRange		=	m_lpSkill->m_aValue[4];
	m_iUpdateCounter		=	0;

	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	폭발형 스킬 사용
BOOL
cAS_ExplosionWithImage::cast()
{
	if	(m_lpSkill->m_wExplosionImage==	0xffff	)
		return	FALSE;

	int	iCastingTime	=	m_lpSkill->m_sCastingTime*dSYNC_FPS/1000;

	m_wCastingTime		=	iCastingTime;

	m_wMaxFrame		=	g_aEffect[m_lpSkill->m_wExplosionImage].getFrameCount(0);
	m_wFPS			=	g_aEffect[m_lpSkill->m_wExplosionImage].getFPS(0);
	m_wFrameCounter	=	0;		//	바로시작
	m_wAnm			=	0;

	m_wFPS			=	m_wFPS*m_lpSkill->m_wCorrectFPS/100;

	m_wIsGetdCP		=	FALSE;
	
	m_lpCaster->m_wBlockToTransformationTime	=	max(dSYNC_FPS*2,m_lpCaster->m_wBlockToTransformationTime);

	int	iTR			=	m_lpCaster->isAbleToTransformation(TRUE);
	
	if	(iTR	==	eTRANSFORMATION_SUCCESS	&&	m_ability.isJobChange())
		m_lpCaster->sendTransformation(eTR_BY_SKILL,m_lpSkill->m_bf1PutTrasAfterAnm);
	
	return	TRUE;
}

//
//	폭발형 스킬 업데이트
BOOL
cAS_ExplosionWithImage::update()
{
	m_iUpdateCounter++;
	
	mDS_AS.m_iActiveSkillInnerStep	=	1;

	if	(m_wFrameCounter	==	0xffff	)
	{
		mDS_AS.m_iActiveSkillInnerStep	=	2;

		if	(!isActionTime())
		{
			return	FALSE;	//	아직 쏘기 직전이다.
		}

		mDS_AS.m_iActiveSkillInnerStep	=	3;

		if	(!cast())
		{
			BOOL	bIsGetedCP	=	m_wIsGetdCP;
			mDS_AS.m_iActiveSkillInnerStep	=	4;
			m_lpCaster->m_lpField->strikeExplosionSkill(m_lpCaster,m_lpTarget,m_posTarget.x,m_posTarget.y,&m_ability,&bIsGetedCP);
			mDS_AS.m_iActiveSkillInnerStep	=	5;

			m_wIsGetdCP			=	bIsGetedCP;

			return	TRUE;
		}
	}

	if	(m_iInhaleEnemyRange	&&	m_wInhalePeroid)
	{
		m_lpCaster->m_wInhaleTime	=	4;

		if	(m_iUpdateCounter	%	m_wInhalePeroid	==	0)
		{
			mDS_AS.m_iActiveSkillInnerStep	=	6;
			m_lpCaster->m_lpField->thrust(m_lpCaster,m_iInhaleEnemyRange,-m_wInhalePixel,m_wMinInhaleRange);
			mDS_AS.m_iActiveSkillInnerStep	=	7;
		}
	}

	mDS_AS.m_iActiveSkillInnerStep	=	8;
	if	(decreaseCastingTime())	
	{
		mDS_AS.m_iActiveSkillInnerStep	=	9;
		return	FALSE;
	}

	mDS_AS.m_iActiveSkillInnerStep	=	10;

	if	(decreaseZoomEffectTime())
	{
		mDS_AS.m_iActiveSkillInnerStep	=	11;
		return	FALSE;
	}

//	프레임 갱신
	{
		while(m_wFrameCounter	>=	dSYNC_FPS)
		{
			m_wFrameCounter	-=	dSYNC_FPS;
			m_wFrame++;

			if	(m_wFrame	>=	m_wMaxFrame)
				return	TRUE;

			if	(!m_lpSkill	||	m_lpSkill->m_wExplosionImage	==	0xffff)
				return	TRUE;

			g_debugSign.m_iExplosionSkillStep		=	m_lpSkill->m_wExplosionImage;
			g_debugSign.m_iExplosionSkillLocalPart	=	m_wAnm;
			g_debugSign.m_iExplosionSkillTeam		=	m_wFrame;

			mDS_AS.m_iActiveSkillInnerStep	=	12;

			if	(g_aEffect[m_lpSkill->m_wExplosionImage].isDamage(m_wAnm,m_wFrame))
			{
				BOOL	bIsGetedCP	=	m_wIsGetdCP;
				mDS_AS.m_iActiveSkillInnerStep	=	13;
				m_lpCaster->m_lpField->strikeExplosionSkill(m_lpCaster,m_lpTarget,m_posTarget.x,m_posTarget.y,&m_ability,&bIsGetedCP);
				mDS_AS.m_iActiveSkillInnerStep	=	14;

				if	(m_lpCaster	&&	m_lpSkill->m_bf1IsSelfDestructionSkill)
				{
					mDS_AS.m_iActiveSkillInnerStep	=	15;
					m_lpCaster->reduceHP(m_lpCaster->m_iHP);
					mDS_AS.m_iActiveSkillInnerStep	=	16;
				}

				m_wIsGetdCP	=	bIsGetedCP;
			}
		}

		m_wFrameCounter	+=	m_wFPS;
	}

	mDS_AS.m_iActiveSkillInnerStep	=	17;

	return	FALSE;
}
