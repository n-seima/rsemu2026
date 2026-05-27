#include "ast_chain_lightning.h"
#include "CActor.h"
#include "CSound.h"


BOOL
cAS_ChainLightning::fire()
{
	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);

	m_wFrameCounter	=	0xffff;
	m_wGrade		=	eWait;

	return	eSKILL_FIRE_RESULT_OK;
}

BOOL
cAS_ChainLightning::casting(WORD *_lpwTargetList,int _iTargetCount)
{
	m_wTargetCount	=	_iTargetCount;
	memcpy(m_awTargetList,_lpwTargetList,sizeof(WORD)*_iTargetCount);

	g_esm.playActionSound(m_lpSkill);

	int	iImage	=	m_lpSkill->m_wShootImage;

	m_wGrade	=	eFire;

	if	(iImage	==	0xffff	)
		return FALSE;
	
	int	iDirectCount;

	m_wMaxFrame		=	g_apEffect[iImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[iImage]->GetFPS(0);
	m_wFrameCounter	=	0;		//	바로시작
	m_wAnm			=	0;

	m_wDirect		=	m_lpCaster->m_iDirect;
	iDirectCount	=	g_apEffect[iImage]->GetDirectCount(0);

	m_wDirect		=	min(iDirectCount-1,m_wDirect);

	return	TRUE;
}

BOOL
cAS_ChainLightning::update()
{
	if	(m_wGrade	==	eWait)
		return	FALSE;

	if	(m_wFrameCounter	==	0xffff)
		return	FALSE;

	while (m_wFrameCounter	>=	dSYNC_FPS)
	{
		m_wFrameCounter	-=	dSYNC_FPS;
		m_wFrame++;

		if	(m_wFrame	>=	m_wMaxFrame)
			return	TRUE;
	}

	m_wFrameCounter	+=	m_wFPS;

	return	FALSE;
}

void
cAS_ChainLightning::put()
{
	if	(m_wGrade	==	eWait)
		return;

	if	(m_lpSkill->m_wShootImage	==	0xffff)
		return;

	cANM	*lpMissile	=	(cANM *)g_apEffect[m_lpSkill->m_wShootImage];

	CPos	pos,posDest,posHit;
	CPos	aRoad[1024];

	int		iScale	=	m_lpSkill->m_wImageScale*g_iGameScale/100;
	int		iTerm	=	m_lpSkill->m_wAfterImageGap*iScale/100;
	int		iDirectCount=	lpMissile->GetDirectCount(0);

	pos.Set(0xffff,0xffff);

	m_lpCaster->getHitPoint(&posHit);

	int		i;

	for	(i=0;i<m_wTargetCount;i++)
	{
		CActor	*lpTarget	=	g_am.getTestedActor(m_awTargetList[i],FALSE);

		if	(lpTarget)
		{
			lpTarget->getHitPoint(&posHit);

			pos.x	=	posHit.x;
			pos.y	=	posHit.y;
			break;
		}
	}

	if	(pos.x	==	0xffff)
		return;

	for	(;i<m_wTargetCount;i++)
	{
		CActor	*lpTarget	=	g_am.getTestedActor(m_awTargetList[i],FALSE);

		if	(!lpTarget)
			continue;

		pos.x		=	GetScaledXPos(pos.x);
		pos.y		=	GetScaledYPos(pos.y);

		lpTarget->getHitPoint(&posHit);
		posDest.x	=	GetScaledXPos(posHit.x);
		posDest.y	=	GetScaledYPos(posHit.y);

		int	iAngle		=	cANGLE::GetAngleToTarget1024(pos.x,pos.y,posDest.x,posDest.y);

		{
			float	fDirect	=	(float)1024/iDirectCount;
			int		iDirect	=	(int)((float)(iAngle+fDirect/2)/fDirect);

			m_wDirect	=	(iDirectCount/4-iDirect+iDirectCount)%iDirectCount;
		}

		int	iCount	=	GetLinerPath(pos.x,pos.y,posDest.x,posDest.y,aRoad,iTerm,1024);

		for	(int iPath=0;iPath<iCount;iPath++)
		{
//			int	iDx	=	random(4)-2;
//			int	iDy	=	random(4)-2;
			lpMissile->PutReg(aRoad[iPath].x,aRoad[iPath].y,m_wAnm,m_wDirect,random(m_wMaxFrame),iScale,iScale);
		}

		lpTarget->getHitPoint(&posHit);

		pos.x		=	posHit.x;
		pos.y		=	posHit.y;
	}
}