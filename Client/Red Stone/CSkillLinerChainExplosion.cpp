#include "CSkill.H"
#include "CActor.H"
#include "cMAP.H"
#include "CSound.H"

//
//직선형연쇄폭발형 스킬 사용
int
CActiveSkill::fireLinerChainExplosionTypeSkill()
{
	CRY(!m_lpCaster,"cry in CActiveSkill::fireLinerChainExplosionTypeSkill()");

	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);

	int	iHitRange	=	m_lpSkill->m_wImageRadius;
	int	iBodySize	=	m_lpCaster->getBodySize()*2+iHitRange/2;

	if	(m_lpSkill->m_aValue[0]	==	FALSE)
		m_sHeight	=	m_lpCaster->getBodyHeight()*2/3;
	else
	{
		m_sHeight	=	0;
		iBodySize	=	0;
	}

	setLCECasterBodySize(iBodySize);
	setLCEHitRange(iHitRange);

	setLCEAnm(m_lpSkill->m_wAction);	//	몸통 사이즈 리턴
	setLCEDirect(m_lpCaster->m_iDirect);	//	타격 범위 리턴
	
	return	eSKILL_FIRE_RESULT_OK;
}

//
//	캐스팅
void
CActiveSkill::castLinerChainExplosionTypeSkill()
{
	if	(m_lpSkill->m_wExplosionImage==	0xffff	)
		return;
	CRY(!m_lpCaster,"cry in CActiveSkill::castLinerChainExplosionTypeSkill()");

	if	(m_lpTarget								)
		m_posTarget	=	m_lpTarget->m_pos;

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wExplosionImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wExplosionImage]->GetFPS(0);
	m_wFrameCounter	=	0;						//	바로시작

	int	iDirectCount=	g_apEffect[m_lpSkill->m_wExplosionImage]->GetDirectCount(0);

	m_wAngle		=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y,2);
	m_wDirect		=	GetDirect(m_wAngle,iDirectCount);

	m_posTarget.x	=	m_lpCaster->m_pos.x;
	m_posTarget.y	=	m_lpCaster->m_pos.y;

	GetOvalAnglePos(&m_posTarget,m_wAngle,m_wRange);

	g_esm.playActionSound(m_lpSkill);
}

//
//직선형연쇄폭발형 업데이트
BOOL
CActiveSkill::updateLinerChainExplosionTypeSkill()
{
	if (m_wFrameCounter	==	0xffff	)
	{
		if (m_lpCaster->m_isTriggerFrame)
			castLinerChainExplosionTypeSkill();

		if	(m_lpSkill->m_wExplosionImage==	0xffff	)
			return	TRUE;

		return	FALSE;	//	아직 쏘기 직전이다.
	}

	if	(m_lpSkill->m_aValue[1])
	{
		if	(!m_lpCaster || m_lpCaster->isActionFinished())
			return	TRUE;

		return	FALSE;
	}

//	프레임 갱신
	{
		if (m_wFrameCounter	>=	dSYNC_FPS)
		{
			m_wFrameCounter	-=	dSYNC_FPS;
			m_wFrame++;
		}

		m_wFrameCounter	+=	m_wFPS;
	}

	int		iHitRange	=	getLCEHitRange();
	int		iShotCount	=	m_wFrame;

	if (iShotCount*iHitRange	>	m_wRange)
	{
		BOOL	isResult	=	TRUE;

		iShotCount	=	m_wRange/iHitRange;

		for (int i=0;i<iShotCount;i++)
		{
			int	iFrame	=	m_wFrame-i;

			if (iFrame>=m_wMaxFrame	||	iFrame<0)
				continue;

			isResult	=	FALSE;
		}

		return	isResult;
	}

	return FALSE;
}

//
//직선형연쇄폭발형 그려주기
void
CActiveSkill::putLinerChainExplosionTypeSkill()
{
	if	(m_lpCaster	==	NULL)
	{
		if	(m_lpSkill->m_aValue[0])
			return;

		if	(m_lpSkill->m_aValue[1])
			return;
	}

	if	(m_lpSkill->m_aValue[1])
	{
		m_lpCaster->putImageSyncWithActor(m_lpSkill->m_wExplosionImage);

		return;
	}

	if	(m_wFrameCounter	==	0xffff)
		return;

	int	iScale		=	m_lpSkill->m_wImageScale;
	iScale			=	iScale*g_iGameScale/100;

	int	iShotCount	=	m_wFrame+1,i,iFrame;
	int	iHitRange	=	getLCEHitRange();
	int	iBodySize	=	getLCECasterBodySize();

	iShotCount	=	min(iShotCount,m_wRange/iHitRange);

	CPos	pos;

	for (i=0;i<iShotCount;i++)
	{
		pos		=	m_pos;

		if	(m_lpSkill->m_aValue[0])
			m_lpCaster->getReleasePos(&pos,getLCEAnm(),getLCEDirect());

		GetOvalAnglePos(&pos,m_wAngle,iHitRange*i+iBodySize);

		iFrame	=	m_wFrame-i;

		if	(iFrame	>=	m_wMaxFrame	||	iFrame	<	0)
			continue;

		if	(GetOvalRange(m_pos.x,m_pos.y,pos.x,pos.y)	>	(m_wRange+m_lpSkill->m_wImageRadius)*(m_wRange+m_lpSkill->m_wImageRadius))
			continue;

		if	(m_lpSkill->m_aValue[0])
			pos.y		-=	m_sHeight;
		
		int	x		=	GetScaledXPos(pos.x);
		int	y		=	GetScaledYPos(pos.y);

		g_apEffect[m_lpSkill->m_wExplosionImage]->PutReg(x,y,m_wAnm,m_wDirect,iFrame,iScale,iScale);
	}
}