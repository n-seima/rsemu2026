#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	미사일형 스킬 사용
int
CActiveSkill::fireFireBallTypeSkill()
{
	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);
	m_hitInfo.copy(&m_lpCaster->m_attackInfo.m_aInfo[0]);

	if	(m_lpSkill->m_wAction	==	m_lpSkill->m_wAction2)
	{
		m_sHeight			=	m_lpCaster->getBodyHeight(FALSE)*5/4;
		m_sDestHeight		=	m_lpTarget->getBodyHeight(FALSE)*2/3;			//	목표 높이
	}
	else
	{
		m_sHeight			=	m_lpCaster->getBodyHeight(FALSE)*4/2;
		m_sDestHeight		=	m_lpTarget->getBodyHeight(FALSE)*2/3;			//	목표 높이
	}

	int	iShotCount		=	m_ability.getContinuousShotCount();
	int	iCastingTime	=	m_hitInfo.m_dwResultField*dSYNC_FPS/dSYNC_FPS_IN_SERVER;
	int	iTriggerFrame	=	m_lpCaster->getTriggerFrame(m_lpSkill->m_wAction2);
	int	iFPS			=	m_lpCaster->getFPS(m_lpSkill->m_wAction2);

//	iCastingTime		-=	iTriggerFrame*dSYNC_FPS/iFPS;

	setFireBallCastTime(iCastingTime);
	setFireBallShotCount(iShotCount);
	setFireBallStatus(eFBTSS_READY);
	setFireBallCounter(0);
	m_lpCaster->m_wIsFreezeAtTriggerFrame	=	TRUE;
	setCurrentFireBallShotCount(0);

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	파이어볼 발사 =p=
BOOL
CActiveSkill::castFireBallTypeSkill()
{
	if (getFireBallStatus()		==	eFBTSS_READY)
		return FALSE;

	CPos	pos;
	int		iShotCount			=	getFireBallShotCount();
	int		iFireBallCastTime	=	getFireBallCastTime();

	int		iAngleGrade			=	360/iShotCount;
	int		iAngle				=	iAngleGrade/3;
	int		iDistance			=	m_lpCaster->getBodySize()+40;
	int		iFrameCounter		=	m_lpCaster->s_iFrameCounter;
	int		iFrame				=	m_wFrame;

	if (iShotCount	<=	1)
		iDistance	=	0;

	iAngle	=	iAngle+iAngleGrade*getFireBallCounter();
	iAngle	%=	360;

	pos.x	=	m_lpCaster->m_pos.x;
	pos.y	=	m_lpCaster->m_pos.y;

	if	(iDistance	>	0)
		GetOvalAnglePos(&pos,iAngle,iDistance);

	pos.y	-=	m_sHeight;
	pos.y	=	pos.y+dSIN_TABLE[(iFrameCounter*6+iAngle)%360]*5/256;

	CHitInfo	hitInfo;

	hitInfo.m_dwPhysicalDamage	=	m_hitInfo.m_dwPhysicalDamage/iShotCount;
	hitInfo.m_dwMagicDamage		=	m_hitInfo.m_dwMagicDamage/iShotCount;

	{
		int		iDamage;

		iDamage					=	hitInfo.m_dwMagicDamage/100/iShotCount;
		hitInfo.m_dwMagicDamage	=	hitInfo.m_dwMagicDamage+random(iDamage/2)*100-iDamage*100/2;

		iDamage					=	hitInfo.m_dwPhysicalDamage/100/iShotCount;
		hitInfo.m_dwPhysicalDamage=	hitInfo.m_dwPhysicalDamage+random(iDamage/2)*100-iDamage*100/2;
	}

	hitInfo.m_dwResultField		=	0;
	hitInfo.setHit();

	g_sm.castSpecialMissile(NULL,m_lpTarget,pos.x,pos.y-m_lpCaster->getHeight(),&hitInfo,&m_ability);

	g_esm.playActionSound(m_lpSkill);

	if	(getFireBallCounter()	==	0)
		if (m_lpCaster && m_lpCaster->isHero())	g_hero.increaseCP(m_ability.getGetCP());

	return	increaseFireBallCounter();
}

//
//	미사일형 업데이트
BOOL
CActiveSkill::updateFireBallTypeSkill()
{
	if (getFireBallStatus()	==	eFBTSS_READY)
	{
		if (m_wFrameCounter	==	0xffff)
		{
			if (m_lpCaster->m_isTriggerFrame)
			{
				m_wFrameCounter	=	0;
				m_wFrame		=	0;
			}

			return	FALSE;	//	아직 쏘기 직전이다.
		}

		int		iShotCount			=	getFireBallShotCount();
		int		iFireBallCastTime	=	getFireBallCastTime();
		int		iTimePerOneBall		=	iFireBallCastTime/iShotCount;
		int		iBallCount			=	min(m_wFrame/iTimePerOneBall+1,iShotCount);

		m_wFrame++;

		if (m_wFrame	>=	iFireBallCastTime)
		{
			setFireBallStatus(eFBTSS_CAST);
			m_lpCaster->m_wIsFreezeAtTriggerFrame	=	FALSE;

			if	(m_lpSkill->m_wAction	==	m_lpSkill->m_wAction2)
			{
				setFireBallStatus(eFBTSS_FIRE);

				setFireBallFireDelay(0);

				return	castFireBallTypeSkill();

			}
			m_lpCaster->action(m_lpTarget->m_pos.x,m_lpTarget->m_pos.y,m_lpSkill->m_wAction2);

			return FALSE;
		}

		if (getCurrentFireBallShotCount()	!=	iBallCount)
		{
			setCurrentFireBallShotCount(iBallCount);
			g_esm.playCreateSound(m_lpSkill);
		}

		return FALSE;
	}

	m_wFrame++;

	if (getFireBallStatus()	==	eFBTSS_CAST)
	{
		if (m_lpCaster->m_isTriggerFrame)
		{
			setFireBallStatus(eFBTSS_FIRE);

			setFireBallFireDelay(0);

			return	castFireBallTypeSkill();
		}

		return	FALSE;
	}

	increaseFireBallFireDelay();

	if (getFireBallFireDelay()	>=	dSYNC_FPS/10)
	{
		setFireBallFireDelay(0);

		BOOL	bResult	=	castFireBallTypeSkill();

		if (bResult)
			return	TRUE;
	}

	return FALSE;
}

//
//	스킬 이미지 그려주기
void
CActiveSkill::putFireBallTypeSkill()
{
	if	(m_wFrameCounter			==	0xffff	)	return;	
	if	(m_lpSkill->m_wShootImage	==	0xffff	)	return;
	cANM	*lpEffect			=	g_im.getEffect(m_lpSkill->m_wShootImage);

	if (!lpEffect)
		return;

	CPos	pos;
	int		iShotCount			=	getFireBallShotCount();
	int		iFireBallCastTime	=	getFireBallCastTime();
	int		iTimePerOneBall		=	iFireBallCastTime/iShotCount;
	int		iBallCount			=	min(m_wFrame/iTimePerOneBall+1,iShotCount);

	int		iAngleGrade			=	360/iShotCount;
	int		iAngle				=	iAngleGrade/3;
	int		iDistance			=	m_lpCaster->getBodySize()+40;
	int		iFrameCounter		=	m_lpCaster->s_iFrameCounter;
	int		iFrame				=	m_wFrame;

	int		iAppearanceAnmFrameCount=	lpEffect->GetFrameCount(eFBA_APPEARANCE);
	int		iFloatAnmFrameCount	=	lpEffect->GetFrameCount(eFBA_FLOAT);
	int		iFPS				=	lpEffect->GetFPS(eFBA_FLOAT);

	if (iShotCount	<=	1)
		iDistance	=	0;

	iAngle	=	iAngleGrade*getFireBallCounter();
	iAngle	%=	360;

	if	(getFireBallStatus()	==	eFBTSS_FIRE)
		iBallCount	=	iShotCount;

	iFrame	=	iFrame-getFireBallCounter()*iTimePerOneBall;

	for (int i=getFireBallCounter();i<iBallCount;i++)
	{
		pos.x	=	m_lpCaster->m_pos.x;
		pos.y	=	m_lpCaster->m_pos.y-m_lpCaster->getHeight();

		if	(iDistance	>	0)
			GetOvalAnglePos(&pos,iAngle,iDistance);

		pos.y	-=	m_sHeight;
		pos.y	=	pos.y+dSIN_TABLE[(iFrameCounter*6+iAngle)%360]*5/256;

		pos.x	=	GetScaledXPos(pos.x);
		pos.y	=	GetScaledYPos(pos.y);

		int	iPutFrame	=	iFrame;
		int	iAnm		=	eFBA_APPEARANCE;

		iPutFrame		=	iPutFrame*iFPS/dSYNC_FPS;

		if (iPutFrame	>=	iAppearanceAnmFrameCount)
		{
			iPutFrame	-=	iAppearanceAnmFrameCount;
			iPutFrame	%=	iFloatAnmFrameCount;
			iAnm		=	eFBA_FLOAT;
		}

		lpEffect->PutReg(pos.x,pos.y,iAnm,0,iPutFrame,g_iGameScale,g_iGameScale);

		iFrame	-=	iTimePerOneBall;
		iAngle	+=	iAngleGrade;
		iAngle	%=	360;
	}
}