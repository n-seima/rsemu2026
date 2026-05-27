#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	미사일형 스킬 사용
int
CActiveSkill::fireExplosionMissile()
{
	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);
	m_hitInfo.copy(&m_lpCaster->m_attackInfo.m_aInfo[0]);

	if	(m_lpSkill->m_aValue[1]	==	0)
		m_sHeight		=	m_lpCaster->getBodyHeight()*2/3;
	else
		m_sHeight		=	0;

	g_esm.playActionSound(m_lpSkill);

	setExplosionMissileStatus(eEMS_FLY);
	
	return	eSKILL_FIRE_RESULT_OK;
}

//
//	미사일형 스킬 사용
int
CActiveSkill::fireExplosionMissileWhichQuick()
{
	if	(m_lpSkill->m_wShootImage	==	0xffff	)
		return	eSKILL_FIRE_RESULT_FAILED;

	g_esm.playActionSound(m_lpSkill);

	setExplosionMissileStatus(eEMS_FLY);

	int	iBodySize	=	m_lpCaster->getBodySize()+30;
	int	iDirectCount=	g_apEffect[m_lpSkill->m_wShootImage]->GetDirectCount(0);

	m_pos.y			-=	m_lpCaster->getBodyHeight()*2/3;

	if	(m_lpSkill->m_aValue[1]	==	1)
	{
		CPos	pos;
		
		m_lpCaster->getReleasePos(&pos,m_lpSkill->m_wAction);				//	액션 프레임에서 마법이나 기타 이펙트 릴리즈 포인트 구함
		
		m_pos.x	=	pos.x;
		m_pos.y	=	pos.y;
	}

	m_posBegin.x	=	m_pos.x;
	m_posBegin.y	=	m_pos.y;
	m_sHeight		=	0;

	if (m_lpTarget)
	{
		m_posTarget.x	=	m_lpTarget->m_pos.x;
		m_posTarget.y	=	m_lpTarget->m_pos.y-m_lpTarget->getHeight();
	}

	GetTargetPos(&m_pos,m_posTarget.x,m_posTarget.y,iBodySize);

	m_wAngle		=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y,1);
	m_wDirect		=	GetDirect(m_wAngle,iDirectCount);

	m_iLastDistance	=	0x7ffffff;

	m_wFrameCounter	=	0;						//	바로시작
	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wShootImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wShootImage]->GetFPS(0);

	return	eSKILL_FIRE_RESULT_OK;
}


void
CActiveSkill::castExplosionMissile()
{
	m_wFrameCounter	=	0;						//	바로시작

	if	(m_lpSkill->m_wShootImage	==	0xffff)
		return;

	int	iBodySize	=	m_lpCaster->getBodySize()+30;
	int	iDirectCount=	g_apEffect[m_lpSkill->m_wShootImage]->GetDirectCount(0);

	if	(m_lpTarget)
	{
		m_posTarget.x	=	m_lpTarget->m_pos.x;
		m_posTarget.y	=	m_lpTarget->m_pos.y-m_lpTarget->getHeight();
	}

	m_posBegin.x	=	m_lpCaster->m_pos.x;
	m_posBegin.y	=	m_lpCaster->m_pos.y-m_lpCaster->getHeight();

	if	(m_lpSkill->m_aValue[1]	==	1)
	{
		CPos	pos;

		m_lpCaster->getReleasePos(&pos,m_lpSkill->m_wAction);				//	액션 프레임에서 마법이나 기타 이펙트 릴리즈 포인트 구함

		m_posBegin.x	=	pos.x;
		m_posBegin.y	=	pos.y;
		m_pos			=	m_posBegin;
	}


	GetTargetPos(&m_pos,m_posTarget.x,m_posTarget.y,iBodySize);

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wShootImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wShootImage]->GetFPS(0);

	m_wAngle		=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y,1);
	m_wDirect		=	GetDirect(m_wAngle,iDirectCount);

	m_iLastDistance	=	0x7ffffff;
}

//
//	터져!!
BOOL
CActiveSkill::explosionMissileExplosion()
{
	if	(m_lpSkill->m_wExplosionImage	==	0xffff)
		return	TRUE;

	setExplosionMissileStatus(eEMS_EXPLOSION);

	m_pos.x	=	m_posTarget.x;
	m_pos.y	=	m_posTarget.y;

	setExplosionMissileTargetPos(m_posTarget.x,m_posTarget.y-m_sHeight);

	setExplosionMissileFPS(g_apEffect[m_lpSkill->m_wExplosionImage]->GetFPS(0));
	setExplosionMissileMaxFrame(g_apEffect[m_lpSkill->m_wExplosionImage]->GetFrameCount(0));
	setExplosionMissileFrameCounter(0);
	setExplosionMissileAnm(0);
	setExplosionMissileFrame(0);
	setExplosionMissileFnishFrame(0);

	return	FALSE;
}

//
//	미사일형 업데이트
BOOL
CActiveSkill::updateExplosionMissile()
{
	if	(m_wFrameCounter	==	0xffff	)
	{
		if	(m_lpCaster->m_isTriggerFrame)	
			castExplosionMissile();

		return	FALSE;	//	아직 쏘기 직전이다.
	}

//	프레임 갱신
	if (m_lpSkill->m_wShootImage	!=	0xffff)
	{
		if (m_wFrameCounter	>=	dSYNC_FPS)
		{
			m_wFrameCounter	-=	dSYNC_FPS;
			m_wFrame++;

			if (m_wFrame	>=	m_wMaxFrame)	m_wFrame	=	0;
		}

		m_wFrameCounter	+=	m_wFPS;
	}

	if (getExplosionMissileStatus() == eEMS_EXPLOSION)
	{
		int	iFrameCounter	=	getExplosionMissileFrameCounter();
		int	iFrame			=	getExplosionMissileFrame();

		if (iFrameCounter	>=	dSYNC_FPS)
		{
			iFrameCounter	-=	dSYNC_FPS;
			iFrame++;

			if (iFrame		>=	getExplosionMissileMaxFrame())	return	TRUE;

			if (g_apEffect[m_lpSkill->m_wExplosionImage]->IsDamage(0,m_wFrame))
				g_esm.playExplosionSound(m_lpSkill);
			
			setExplosionMissileFrameCounter(iFrameCounter);
			setExplosionMissileFrame(iFrame);
		}

		iFrameCounter	+=	getExplosionMissileFPS();
		setExplosionMissileFrameCounter(iFrameCounter);
		increaseExplosionMissileFnishFrame();
	}

	if (getExplosionMissileStatus() == eEMS_FLY)
	{
		CPos	pos		=	m_pos;

		if (m_lpTarget)
		{
			m_posTarget.x	=	m_lpTarget->m_pos.x;
			m_posTarget.y	=	m_lpTarget->m_pos.y-m_lpTarget->getHeight();
		}

		GetTargetPos(&pos,m_posTarget.x,m_posTarget.y,m_lpSkill->m_wSpeed);

		int	iCurrentRange	=	GetOvalRange(pos.x,pos.y,m_posTarget.x,m_posTarget.y);

		if (iCurrentRange	>=	m_iLastDistance)
			return	explosionMissileExplosion();

		m_iLastDistance	=	iCurrentRange;
		m_pos			=	pos;
	}

	return FALSE;
}

//
//	스킬 이미지 그려주기
void
CActiveSkill::putExplosionMissile()
{
	if	(m_lpSkill->m_wShootImage==	0xffff)
		return;
	if	(m_wFrameCounter		==	0xffff)
		return;	//	아직 쏘기 직전이다.

	int		iScale			=	g_iGameScale*m_lpSkill->m_wImageScale/100;
	int		iSpeed			=	m_lpSkill->m_wSpeed;
	int		iLastDistance	=	0x7fffffff;
	int		iX;
	int		iY;
	
//	날아가는 중이다.
	if	(getExplosionMissileStatus() == eEMS_FLY)
	{
		while(m_lpSkill->isWaterCanonType())
		{
			CPos	pos	=	m_posBegin;

			GetTargetPos(&pos,m_posTarget.x,m_posTarget.y,iSpeed);

			int	iRange	=	GetOvalRange(pos.x,pos.y,m_pos.x,m_pos.y);

			if	(iRange	>=	iLastDistance)
				break;

			iLastDistance=	iRange;

			iSpeed		+=	m_lpSkill->m_wSpeed;

			int	iFrame	=	random(m_wMaxFrame);

			pos.x		+=	random(8)-4;
			pos.y		+=	random(8)-4;

			iX	=	GetScaledXPos(pos.x);
			iY	=	GetScaledYPos(pos.y);

			g_apEffect[m_lpSkill->m_wShootImage]->PutReg(iX,iY-m_sHeight,m_wAnm,m_wDirect,iFrame,iScale,iScale);
		}

		iX	=	GetScaledXPos(m_pos.x);
		iY	=	GetScaledYPos(m_pos.y);
		
		g_apEffect[m_lpSkill->m_wShootImage]->PutReg(iX,iY-m_sHeight,m_wAnm,m_wDirect,m_wFrame,iScale,iScale);

		return;
	}

//	폭발
	if (m_lpSkill->isWaterCanonType())
	{
		int	iValidCount	=	0;

		while(1)	//	뒤에 따라오던거 줄어들기
		{
			CPos	pos	=	m_posBegin;

			GetTargetPos(&pos,m_posTarget.x,m_posTarget.y,iSpeed);

			int	iRange	=	GetOvalRange(pos.x,pos.y,m_pos.x,m_pos.y);

			if (iRange	>=	iLastDistance)	break;

			iLastDistance=	iRange;

			iSpeed		+=	m_lpSkill->m_wSpeed;

			if (iValidCount	>=	getExplosionMissileFnishFrame()	)
			{
				int	iFrame	=	random(m_wMaxFrame);

				pos.x		+=	random(m_lpSkill->m_aValue[0])-m_lpSkill->m_aValue[0]/2;
				pos.y		+=	random(m_lpSkill->m_aValue[0])-m_lpSkill->m_aValue[0]/2;

				iX	=	GetScaledXPos(pos.x);
				iY	=	GetScaledYPos(pos.y);

				g_apEffect[m_lpSkill->m_wShootImage]->PutReg(iX,iY-m_sHeight,m_wAnm,m_wDirect,m_wFrame,iScale,iScale);
			}

			iValidCount++;
		}

		iX	=	GetScaledXPos(m_pos.x);
		iY	=	GetScaledYPos(m_pos.y);

		g_apEffect[m_lpSkill->m_wShootImage]->PutReg(iX,iY-m_sHeight,m_wAnm,m_wDirect,m_wFrame,iScale,iScale);
	}

	CPos	pos;

	pos.x	=	GetScaledXPos(getExplosionMissileTargetXPos());
	pos.y	=	GetScaledYPos(getExplosionMissileTargetYPos());

	g_apEffect[m_lpSkill->m_wExplosionImage]->PutReg(pos.x,pos.y,getExplosionMissileAnm(),0,getExplosionMissileFrame(),iScale,iScale);
}