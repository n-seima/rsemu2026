#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	폭발형
int
CActiveSkill::fireExplosionTypeSkillDependOnImage()
{
	CRY(!m_lpCaster,"cry in CActiveSkill::fireExplosionTypeSkillDependOnImage()");

	if	(m_lpSkill->m_wType	==	eSKILL_TYPE_EXPLOSION_DEPEND_ON_IMAGE)
		m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);
	else	
		castExplosionTypeSkillDependOnImage();

	setExplosionWhichDefendOnImageSkillScale(20);
	setExplosionWhichDefendOnImageSkillZoommingStart();
	setExplosionWhichDefendOnImageSkillFrameStart();

	if	(m_lpSkill->m_bf1IsRefitImageSizeByHitRange)
	{
		int	iHitRange	=	m_ability.getHitRange(NULL);
		int	iScale		=	iHitRange*100/m_lpSkill->m_wImageRadius;

		setExplosionWhichDefendOnImageSkillScale(iScale);
	}

	if	(m_lpSkill->m_aValue[0])
		m_sHeight	=	m_lpCaster->getBodyHeight();
	else
		m_sHeight	=	0;

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	캐스팅
BOOL
CActiveSkill::castExplosionTypeSkillDependOnImage()
{
	if	(m_lpSkill->m_wType	!=	eSKILL_TYPE_WATER_FALL)
		g_esm.playActionSound(m_lpSkill);

	if	(m_lpSkill->m_wExplosionImage==	0xffff	)
		return FALSE;

	int	iCastingTime	=	m_lpSkill->m_sCastingTime*dSYNC_FPS/1000;

	setDependOnImageExplosionSkillCastingTime(iCastingTime);
	setDependOnImageExplosionSkillHitCount();

	int	iDirectCount;

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wExplosionImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wExplosionImage]->GetFPS(0);
	m_wFrameCounter	=	0;		//	바로시작
	m_wAnm			=	0;

	if	(m_lpSkill->m_wType	==	eSKILL_TYPE_WATER_FALL)
		m_wAnm		=	random(g_apEffect[m_lpSkill->m_wExplosionImage]->m_iAnmCount);

	m_wFPS			=	m_wFPS*m_lpSkill->m_wCorrectFPS/100;

	m_wDirect		=	m_lpCaster->m_iDirect;
	iDirectCount	=	g_apEffect[m_lpSkill->m_wExplosionImage]->GetDirectCount(0);

	m_wDirect		=	min(iDirectCount-1,m_wDirect);

	if (m_lpSkill->m_wShakeTiming	==	eST_CAST)
		if (m_lpSkill->m_wShakeIntensity)
			g_shaker.add(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wShakeIntensity,m_lpSkill->m_wShakeTime,m_ability.getHitRange(NULL));

	return	TRUE;
}

//
//	폭발 스킬
BOOL
CActiveSkill::updateExplosionTypeSkillDependOnImage()
{
	if	(m_lpCaster == NULL)
		return	TRUE;

	if	(m_lpSkill == NULL)
		return	TRUE;
		
	if (m_wFrameCounter	==	0xffff	)
	{
		if (m_lpCaster->m_isTriggerFrame)
			if (!castExplosionTypeSkillDependOnImage())
			{
				return	TRUE;
			}

		return	FALSE;	//	아직 쏘기 직전이다.
	}

	if (decreaseDependOnImageExplosionSkillCastingTime())
	{
		return	FALSE;
	}

	if (m_lpSkill->m_bf1IsZoomInEffectSkill && isExplosionWhichDefendOnImageSkillZoomming())
	{
		int	iScale	=	getExplosionWhichDefendOnImageSkillScale();

		iScale		+=	iScale/3;
		iScale		=	min(iScale,m_lpSkill->m_wImageScale);

		setExplosionWhichDefendOnImageSkillScale(iScale);

		if (iScale	<	m_lpSkill->m_wImageScale)	return	FALSE;

		setExplosionWhichDefendOnImageSkillZoommingComplete();
	}

//	프레임 갱신
	{
		while (m_wFrameCounter	>=	dSYNC_FPS)
		{
			m_wFrameCounter	-=	dSYNC_FPS;
			m_wFrame++;

			if (m_wFrame	>=	m_wMaxFrame)
			{
				if	(m_lpSkill->m_bf1IsZoomInEffectSkill)
					setExplosionWhichDefendOnImageSkillDeclineScale();
				else
				{
					return	TRUE;
				}
			}

			if	(!m_lpSkill	||	g_apEffect[m_lpSkill->m_wExplosionImage]	==	NULL)
				return	TRUE;

			if	(g_apEffect[m_lpSkill->m_wExplosionImage]->IsDamage(0,m_wFrame))
			{
				if (m_lpSkill->m_wShakeTiming	==	eST_EXPLOSION)
					if (m_lpSkill->m_wShakeIntensity)
						g_shaker.add(m_pos.x,m_pos.y,m_lpSkill->m_wShakeIntensity,m_lpSkill->m_wShakeTime,m_ability.getHitRange(NULL));

				if (getDependOnImageExplosionSkillHitCount()	==	0)
					if (m_lpSkill->m_wType	==	eSKILL_TYPE_EXPLOSION_DEPEND_ON_IMAGE)
						g_esm.playExplosionSound(m_lpSkill);
					
				increaseDependOnImageExplosionSkillHitCount();
			}
		}

		m_wFrameCounter	+=	m_wFPS;
	}

	if (m_lpSkill->m_bf1IsZoomInEffectSkill && isExplosionWhichDefendOnImageSkillDeclineScale())
	{
		int	iScale	=	getExplosionWhichDefendOnImageSkillScale();

		iScale		-=	iScale/4;
		iScale		=	max(iScale,10);

		setExplosionWhichDefendOnImageSkillScale(iScale);

		if (iScale	<=	10)
		{
			return	TRUE;
		}
	}

	return	FALSE;
}

//
//	폭발 스킬
void
CActiveSkill::putExplosionTypeSkillDependOnImage()
{
	if (m_wFrameCounter				==	0xffff		)
		return;
	if (m_lpSkill->m_wExplosionImage==	0xffff		)
		return;
	if	(m_lpSkill->m_bf1IsRapeExplosionImage		)
		return;
	if	(m_wFrame					>=	m_wMaxFrame	)
		return;
	if (m_lpSkill->m_bf1IsZoomInEffectSkill && isExplosionWhichDefendOnImageSkillZoomming())
		return;

	int	iScale	=	m_lpSkill->m_wImageScale;

	if	(m_lpSkill->m_bf1IsZoomInEffectSkill	||	m_lpSkill->m_bf1IsRefitImageSizeByHitRange)
		iScale	=	getExplosionWhichDefendOnImageSkillScale();

	iScale		=	iScale*g_iGameScale/100;

	int	iX		=	GetScaledXPos(m_posTarget.x);
	int	iY		=	GetScaledYPos(m_posTarget.y)-m_sHeight;

	if(m_lpSkill->m_bf1IsRefitImageSizeByHitRange && iScale>= 100)
	{
		int iFrameCount = g_apEffect[m_lpSkill->m_wExplosionImage]->GetFrameCount(0);

		int iWidth	= 0;
		int iHeight = 0;

		for(int i=0;i<iFrameCount;++i)
		{
			iWidth	= max(iWidth ,g_apEffect[m_lpSkill->m_wExplosionImage]->getSpriteWidth(0,0,i));
			iHeight = max(iHeight, g_apEffect[m_lpSkill->m_wExplosionImage]->getSpriteHeight(0,0,i));
		}
		int iScaleWidth = iWidth*iScale/100;
		int iScaleHeight = iHeight*iScale/100;
		iX			-=	(iScaleWidth-iWidth)/2;
		iY			-=	(iScaleHeight-iHeight)/2;
	}

	g_im.putWhichUsePalette(m_lpSkill->m_wExplosionImage,iX,iY,m_lpSkill->m_wPaletteIndex,m_wAnm,m_wDirect,m_wFrame,iScale,iScale,m_lpSkill->m_wOutputEffect);
}

//
//	폭발 스킬
void
CActiveSkill::putBottomExplosionTypeSkillDependOnImage()
{
	if	(m_wFrameCounter				==	0xffff	)
		return;

	int	iImage	=	m_lpSkill->m_wBottomImage;

	if	(m_lpSkill->m_bf1IsRapeExplosionImage)
		iImage	=	m_lpSkill->m_wExplosionImage;

	if	(iImage	==	0xffff	)
		return;

	int	iScale	=	m_lpSkill->m_wImageScale;

	if	(m_lpSkill->m_bf1IsZoomInEffectSkill	||	m_lpSkill->m_bf1IsRefitImageSizeByHitRange)
		iScale	=	getExplosionWhichDefendOnImageSkillScale();

	iScale	=	iScale*g_iGameScale/100;

	int	iX	=	GetScaledXPos(m_posTarget.x);
	int	iY	=	GetScaledYPos(m_posTarget.y);

	g_im.putWhichUsePalette(iImage,iX,iY,m_lpSkill->m_wPaletteIndex,m_wAnm,m_wDirect,m_wFrame,iScale,iScale,m_lpSkill->m_wOutputEffect);
}