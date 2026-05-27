#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	연속 타격 스킬
int
CActiveSkill::fireImpactSkill()
{
	CRY(!m_lpCaster,"cry in CActiveSkill::fireImpactSkill()");

	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);

	m_hitInfo.copy(&m_lpCaster->m_attackInfo.m_aInfo[0]);
	m_aValue[0]	=	FALSE;

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	캐스팅
BOOL
CActiveSkill::castImpactSkill()
{
	g_esm.playActionSound(m_lpSkill);

	int	iSkillImage	=	m_lpSkill->m_wSkillImage;

	if	(iSkillImage==	0xffff	)	
		iSkillImage	=	m_lpSkill->m_wExplosionImage;

	if	(m_lpSkill->m_wShakeIntensity)
		g_shaker.add(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,m_lpSkill->m_wShakeIntensity,m_lpSkill->m_wShakeTime,m_ability.getHitRange(NULL)+m_lpCaster->getBodySize()+100);

	if	(iSkillImage==	0xffff	)	
		return FALSE;

	m_wMaxFrame		=	g_apEffect[iSkillImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[iSkillImage]->GetFPS(0);
	m_wFPS			=	m_wFPS*m_lpSkill->m_wCorrectFPS/100;
	m_wFrameCounter	=	0;		//	바로시작

	if	(m_lpTarget)
	{
		m_posTarget.x	=	m_lpTarget->m_pos.x;
		m_posTarget.y	=	m_lpTarget->m_pos.y;
	}

	return	TRUE;
}

//
//	연속 타격 스킬
BOOL
CActiveSkill::updateImpactSkill()
{
	if	(m_aValue[0])
	{
		if	(m_lpCaster->m_iAnm	!=	m_lpSkill->m_wAction)
			return	TRUE;

		return	FALSE;
	}

	if	(m_wFrameCounter	==	0xffff	)
	{
		if	(m_lpCaster->m_isTriggerFrame)	
			if	(castImpactSkill() == FALSE)
				m_aValue[0]	=	TRUE;

		return	FALSE;	//	아직 쏘기 직전이다.
	}

//	프레임 갱신
	{
		while(m_wFrameCounter	>=	dSYNC_FPS)
		{
			m_wFrameCounter	-=	dSYNC_FPS;
			m_wFrame++;

			if (m_wFrame	>=	m_wMaxFrame)
				return	TRUE;
		}

		m_wFrameCounter	+=	m_wFPS;
	}

	return	FALSE;
}

void
CActiveSkill::putImpactSkillIllusion()
{
	int		iRange			=	40;
	int		iAngle			=	m_lpCaster->m_pos.x%360;

	CPos	posOrg			=	m_lpCaster->m_pos;

	int		iHitRange		=	m_ability.getHitRange(NULL);
	CPos	pos				=	m_lpCaster->m_pos;
	int		iIllusionCount	=	iHitRange/30;

	srand(posOrg.x);

	for (int i=0;i<iIllusionCount;i++)
	{
		int	iAngle	=	posOrg.x+i*37;
		int	iRange	=	60+i*30;

		pos			=	posOrg;

		GetOvalAnglePos(&pos,iAngle%360,iRange);
		
		m_lpCaster->m_pos.x	=	pos.x;
		m_lpCaster->m_pos.y	=	pos.y;

		m_lpCaster->put();
	}

	m_lpCaster->m_pos.x	=	posOrg.x;
	m_lpCaster->m_pos.y	=	posOrg.y;
}

//
//머리위로 떨어지는 스킬
void
CActiveSkill::putImpactSkill()
{
	if	(m_lpSkill->m_bf1IsIllusionAttack)
	{
		putImpactSkillIllusion();

		return;
	}

	if	(m_wFrameCounter	==	0xffff	)
		return;

	CPos	pos;

	int	iSkillImage	=	m_lpSkill->m_wSkillImage;
	int	iDirect		=	m_lpCaster->m_iDirect;

	if	(iSkillImage==	0xffff	)	
	{
		iSkillImage	=	m_lpSkill->m_wExplosionImage;

		pos.x		=	m_posTarget.x;
		pos.y		=	m_posTarget.y;
		iDirect		=	0;
	}
	else
		m_lpCaster->getReleasePos(&pos);

	if	(iSkillImage==	0xffff	)	
		return;

	if	(m_lpSkill->m_bf1IsExplosionAtCastPos)
	{
		pos.x	=	m_lpCaster->m_pos.x;
		pos.y	=	m_lpCaster->m_pos.y;
	}

	int	iScale	=	g_iGameScale*m_lpSkill->m_wImageScale/100;

	if	(m_lpSkill->m_bf1IsRefitImageSizeByHitRange)
	{
		int	iHitRange	=	m_ability.getHitRange(NULL);
		iScale			=	iHitRange*100/m_lpSkill->m_wImageRadius;
		iScale			=	iScale*g_iGameScale/100;
	}

	cANM	*lpEffect	=	g_im.getEffect(iSkillImage);

	int	x		=	GetScaledXPos(pos.x);
	int	y		=	GetScaledYPos(pos.y);

	if	(m_lpSkill->m_wPaletteIndex	!=	0xffff)
		g_im.putWhichUsePalette(iSkillImage,x,y,m_lpSkill->m_wPaletteIndex,m_wAnm,iDirect,m_wFrame,iScale,iScale);
	else
		lpEffect->PutReg(x,y,m_wAnm,iDirect,m_wFrame,iScale,iScale);

	int	iOverlapAnm	=	lpEffect->m_pAnmData[m_wAnm].m_iLinkAnm;

	if	(iOverlapAnm!=	0xffff)
		lpEffect->PutReg(x,y,iOverlapAnm,iDirect,m_wFrame,iScale,iScale);
}