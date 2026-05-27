#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	미사일형 스킬 사용
int
CActiveSkill::fireSpecialMissile()
{
	if	(m_lpSkill->m_wShootImage	==	0xffff	)
		return	eSKILL_FIRE_RESULT_FAILED;

	if	(m_lpCaster	&& m_lpSkill->m_bf1IsGroundMissile == FALSE)
	{
		m_sHeight	=	m_lpCaster->getHeight()+m_lpCaster->getBodyHeight(FALSE)*2/3;
		m_pos.x		=	m_lpCaster->m_pos.x;
		m_pos.y		=	m_lpCaster->m_pos.y;
		m_pos.y		-=	m_sHeight;
	}
	else
		m_sHeight	=	0;

	if	(m_lpTarget && m_lpSkill->m_bf1IsGroundMissile == FALSE)
		m_sDestHeight=	m_lpTarget->getBodyHeight(FALSE)*2/3;			//	목표 높이
	else
		m_sDestHeight=	0;

	m_posBegin.x	=	m_pos.x;
	m_posBegin.y	=	m_pos.y;

	m_wFrameCounter	=	0;						//	바로시작
	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wShootImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wShootImage]->GetFPS(0);

	int	iDirectCount=	g_apEffect[m_lpSkill->m_wShootImage]->GetDirectCount(0);

	m_wAngle		=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y,2);
	m_wDirect		=	GetDirect(m_wAngle,iDirectCount);
	
	setSpecialMissileUpkeepTime(m_lpSkill->m_aValue[0]);
	setIsMagicMissile(m_lpSkill->m_aValue[3]);
	int	iMinSinHeight = m_lpSkill->m_aValue[4];
	int	iMaxSinHeight = max(m_lpSkill->m_aValue[5], iMinSinHeight);

	int iRand		  =	random(iMaxSinHeight - iMinSinHeight) + iMinSinHeight;
	setMagicMissileMaxSinHeight(iRand);
	setMagicMissileAcceleration(m_lpSkill->m_aValue[6]);
	resetSpecialMissileCurrentUpkeepTime();
	
	castSpecialMissile();

	return	eSKILL_FIRE_RESULT_OK;
}


//
//	머쉰 미사일 발사
void
CActiveSkill::castSpecialMissile()
{
	m_wFrameCounter	=	0;						//	바로시작

	if	(m_lpSkill->m_wShootImage	==	0xffff)
		return;

	int	iDirectCount=	g_apEffect[m_lpSkill->m_wShootImage]->GetDirectCount(0);

	if	(m_lpTarget)
	{
		m_posTarget.x	=	m_lpTarget->m_pos.x;

		if	(m_lpSkill->m_bf1IsGroundMissile)
			m_posTarget.y	=	m_lpTarget->m_pos.y;
		else
			m_posTarget.y	=	m_lpTarget->m_pos.y-m_sDestHeight-m_lpTarget->getHeight(FALSE);
	}

	int	iAngle		=	cANGLE::GetAngleToTarget(m_posTarget.x,m_posTarget.y,m_pos.x,m_pos.y);

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wShootImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wShootImage]->GetFPS(0);
	m_wFrame		=	0;

	m_wAngle		=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y,1);
	m_wDirect		=	GetDirect(m_wAngle,iDirectCount);
	if	(m_lpSkill->m_wImageRadius)
		GetTargetPos(&m_pos,m_posTarget.x,m_posTarget.y,m_lpSkill->m_wImageRadius);

	if(getIsMagicMissile())
	{
		WORD wRange = sqrt(GetRange(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y));
		BOOL bIsLeft = random(2);
		setMagicMissileBeginRange(wRange);
		setMagicMissileSpeedFrame(0);
		if(bIsLeft)
			setMagicMissileAngle(90);
		else
			setMagicMissileAngle(270);

		
	}

	m_iLastDistance	=	0x7fffffff;
}

//
//	머신 미사일형 업데이트
BOOL
CActiveSkill::updateSpecialMissile()
{
		
	if(getIsMagicMissile())
	{
		return updateMagicMissile();
	}

	if	(m_wFrameCounter	>=	dSYNC_FPS)		//	프레임 갱신
	{
		m_wFrameCounter	-=	dSYNC_FPS;
		m_wFrame++;
		if	(m_wFrame	>=	m_wMaxFrame)
			m_wFrame	=	0;
	}

	m_wFrameCounter	+=	m_wFPS;

	CPos	pos		=	m_pos;

	if	(m_lpTarget)
	{
		m_posTarget.x	=	m_lpTarget->m_pos.x;

		if	(m_lpSkill->m_bf1IsGroundMissile)
			m_posTarget.y	=	m_lpTarget->m_pos.y;
		else
			m_posTarget.y	=	m_lpTarget->m_pos.y-m_sDestHeight-m_lpTarget->getHeight();
	}

	GetTargetPos(&pos,m_posTarget.x,m_posTarget.y,	m_lpSkill->m_wSpeed );

	int	iCurrentRange	=	GetOvalRange(pos.x,pos.y,m_posTarget.x,m_posTarget.y);

	if	(iCurrentRange	>=	m_iLastDistance)
	{
		if	(getSpecialMissileCurrentUpkeepTime()	==	0)
		{
			if	(m_lpCaster	&&	m_lpTarget)
				m_lpCaster->strike(m_lpTarget,&m_ability,&m_hitInfo,m_wDirect,FALSE);

			if	(!m_lpCaster&&	m_lpTarget)
				m_lpTarget->strike(m_lpTarget,&m_ability,&m_hitInfo,m_wDirect,FALSE);
		}

		increaseSpecialMissileCurrentUpkeepTime();

		if	(getSpecialMissileCurrentUpkeepTime()	>=	getSpecialMissileUpkeepTime())
			return	TRUE;

		return	FALSE;
	}

	if	(m_lpSkill->m_wMissileFollowImage	!=	0xffff)
	{
		CPos	posFollow	=	m_pos;

		if	(m_lpSkill->m_wMissileDustRange)
			GetTargetPos(&posFollow,m_posBegin.x,m_posBegin.y,m_lpSkill->m_wMissileDustRange);

		g_floatEffect.add(posFollow.x,posFollow.y,m_lpSkill->m_wMissileFollowImage,m_lpSkill->m_wDustImageRange,m_lpSkill->m_bf1IsRandomDustImageFrame,m_wDirect);
	}

	m_iLastDistance	=	iCurrentRange;
	m_pos			=	pos;

	return FALSE;
}

BOOL
CActiveSkill::updateMagicMissile()
{
	if	(m_wFrameCounter	>=	dSYNC_FPS)		//	프레임 갱신
	{
		m_wFrameCounter	-=	dSYNC_FPS;
		m_wFrame++;

		if(getMagicMissileSpeedFrame() * getMagicMissileAcceleration()/100 + m_lpSkill->m_wSpeed < 10)
			increaseMagicMissileSpeedFrame();

		if	(m_wFrame	>=	m_wMaxFrame)
			m_wFrame	=	0;
	}
	
	m_wFrameCounter	+=	m_wFPS;
	
	CPos	pos		=	m_pos;
	
	if	(m_lpTarget)
	{
		m_posTarget.x	=	m_lpTarget->m_pos.x;
		
		if	(m_lpSkill->m_bf1IsGroundMissile)
			m_posTarget.y	=	m_lpTarget->m_pos.y;
		else
			m_posTarget.y	=	m_lpTarget->m_pos.y-m_sDestHeight-m_lpTarget->getHeight();
	}
	int iSpeed = min(10,(m_lpSkill->m_wSpeed + getMagicMissileSpeedFrame() *  getMagicMissileAcceleration()/100));
	GetTargetPos(&pos,m_posTarget.x,m_posTarget.y,iSpeed);
	int	iCurrentRange	=	GetOvalRange(pos.x,pos.y,m_posTarget.x,m_posTarget.y);

	int iRange = 	sqrt(GetRange(pos.x,pos.y,m_posTarget.x,m_posTarget.y));
	if(iRange > getMagicMissileBeginRange())
		setMagicMissileBeginRange(iRange);
	int iAngle  = (float)iRange / getMagicMissileBeginRange() * 180 + 90;
	iAngle = iAngle%360;
	
	int	iSinHeight	= abs((float)dSIN_TABLE[iAngle] * getMagicMissileMaxSinHeight() / 256);	
	
	setMagicMissileSinHeight(iSinHeight);

	if(iCurrentRange <= g_apEffect[m_lpSkill->m_wShootImage]->m_iCrashSize * g_apEffect[m_lpSkill->m_wShootImage]->m_iCrashSize)
	{
		if	(getSpecialMissileCurrentUpkeepTime()	==	0)
		{
			if	(m_lpCaster	&&	m_lpTarget)
				m_lpCaster->strike(m_lpTarget,&m_ability,&m_hitInfo,m_wDirect,FALSE);
			
			if	(!m_lpCaster&&	m_lpTarget)
				m_lpTarget->strike(m_lpTarget,&m_ability,&m_hitInfo,m_wDirect,FALSE);
		}
		
		increaseSpecialMissileCurrentUpkeepTime();
		
		if	(getSpecialMissileCurrentUpkeepTime()	>=	getSpecialMissileUpkeepTime())
			return	TRUE;
		
		return	FALSE;
	}
	
	if	(m_lpSkill->m_wMissileFollowImage	!=	0xffff)
	{
		CPos	posFollow	=	m_pos;
		
		if	(m_lpSkill->m_wMissileDustRange)
			GetTargetPos(&posFollow,m_posBegin.x,m_posBegin.y,m_lpSkill->m_wMissileDustRange);
		
		g_floatEffect.add(posFollow.x,posFollow.y,m_lpSkill->m_wMissileFollowImage,m_lpSkill->m_wDustImageRange,m_lpSkill->m_bf1IsRandomDustImageFrame,m_wDirect);
	}
	
	m_iLastDistance	=	iCurrentRange;
	m_pos			=	pos;
	
	return FALSE;
}

//
//	스킬 이미지 그려주기
void
CActiveSkill::putSpecialMissile()
{
	if	(m_lpSkill->m_wShootImage	==	0xffff)
		return;

	cANM	*lpMissile	=	(cANM *)g_apEffect[m_lpSkill->m_wShootImage];


	
	if(getIsMagicMissile())
	{
		CPos	pos = m_pos;
		
		GetOvalAnglePos(&pos,(m_wAngle+getMagicMissileAngle())%360,getMagicMissileSinHeight());
		int iAngle		=	cANGLE::GetAngleToTarget(pos.x,pos.y,m_posTarget.x,m_posTarget.y);

		
		int		iX		=	GetScaledXPos(pos.x);
		int		iY		=	GetScaledYPos(pos.y);		
		lpMissile->PutReg(iX ,iY,m_wAnm,m_wDirect,m_wFrame,g_iGameScale,g_iGameScale);


		float fHeight = abs((dSIN_TABLE[(CGameInfo::s_iFrameCounter*5)%360]*15.0/256));
	
		for(int i=0;i<2;++i)
		{
			pos.x = iX;
			pos.y = iY;
			GetOvalAnglePos(&pos,(iAngle+180)%360,fHeight * (1 + i));

			cANM_BASE* lpBase	=	(cANM_BASE*)lpMissile;
			float fSize  = 1 - 0.25 * (1 + i);

			// 아래 부분은 엔진이 수정되면서 스케일 조정할때 약간의 문제가 있어 추가 했다. 
			pos.x = (pos.x - lpMissile->m_posRefit.x * g_iGameScale * fSize/100) + lpMissile->m_posRefit.x;
			pos.y = (pos.y - lpMissile->m_posRefit.y * g_iGameScale * fSize/100) + lpMissile->m_posRefit.y;

			lpMissile->PutReg(pos.x,pos.y,m_wAnm,m_wDirect,m_wFrame,g_iGameScale * fSize,g_iGameScale * fSize);
		}
	}
	else
	{
		int		iX		=	GetScaledXPos(m_pos.x);
		int		iY		=	GetScaledYPos(m_pos.y);
		lpMissile->PutReg(iX,iY,m_wAnm,m_wDirect,m_wFrame,g_iGameScale,g_iGameScale);
	}
	

	switch(m_lpSkill->m_wAfterImageType)
	{
		case	eSKILL_AFTER_IMAGE_SELF_COPY	:
		{
			int	iRange	=	GetRange(m_pos.x,m_pos.y,m_posBegin.x,m_posBegin.y);
			iRange		=	(int)sqrt((double)iRange);

			CPos	pos;

			pos.x	=	m_pos.x;
			pos.y	=	m_pos.y;

			GetAnglePos(&pos,m_wAngle,m_lpSkill->m_wAfterImageFirstImageDistance);

			iRange		-=	m_lpSkill->m_wAfterImageFirstImageDistance;

			int	iX		=	GetScaledXPos(pos.x);
			int	iY		=	GetScaledYPos(pos.y);

			lpMissile->linerPut(iX,iY,
								m_wAnm,m_wDirect,m_wFrame,(m_wAngle+180)%360,iRange,
								m_lpSkill->m_wAfterImageGap,m_lpSkill->m_wAfterImageCount,
								m_lpSkill->m_wAfterImageFirstImageAlphaDepth,m_lpSkill->m_sAfterImageDecreaseAlphaDepthValue,g_iGameScale);
			break;
		}

		case	eSKILL_AFTER_IMAGE_NEXT_ANIMATION	:
			break;

		case	eSKILL_AFTER_IMAGE_INCREASE_FRAME	:
			break;
	}
}