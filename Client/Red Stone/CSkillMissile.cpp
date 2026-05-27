#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	미사일형 스킬 사용
int
CActiveSkill::fireMissileTypeSkill()
{
	if	(!m_lpTarget || !m_lpCaster)
		return	eSKILL_FIRE_RESULT_FAILED;

	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);
	m_hitInfo.copy(&m_lpCaster->m_attackInfo.m_aInfo[0]);

	if	(m_lpSkill->m_wShootImage	==	0xffff)
		return	eSKILL_FIRE_RESULT_OK;

	cANM	*lpMissile	=	g_im.getEffect(m_lpSkill->m_wShootImage);

	if	(!lpMissile)
		return	eSKILL_FIRE_RESULT_OK;

	m_lpTarget->m_wTargetMark	=	m_lpSkill->m_wTargetMarkImage;

	setMissileSize(0);
	resetMissileFrameCounter();

	if	(lpMissile)
	{
		m_wAnm		=	random(lpMissile->m_iAnmCount);

		int	iSize	=	lpMissile->m_sprite.getSpriteHeight(0);

		setMissileSize(iSize/2);
	}

	return	eSKILL_FIRE_RESULT_OK;
}

void
CActiveSkill::castMissile()
{
	if	(!m_lpCaster || !m_lpSkill)
		return;

	g_esm.playActionSound(m_lpSkill);

	if	(m_lpTarget)
		if	(m_lpTarget->m_wSerial	==	m_lpCaster->m_wSerial)
		{
			if	(m_hitInfo.isAttackToShakle())
				m_lpCaster->strike(m_lpTarget,&m_ability,&m_hitInfo,m_wDirect,TRUE);

			return;
		}

	m_wFrameCounter	=	0;						//	바로시작

	if	(m_lpSkill->m_wShootImage	==	0xffff)
		return;

	int	iBodySize	=	m_lpCaster->getBodySize(FALSE)+30;
	int	iDirectCount=	g_apEffect[m_lpSkill->m_wShootImage]->GetDirectCount(0);
	int	iHeight		=	(m_lpCaster->getBodyHeight(FALSE)+m_lpCaster->getHeight())*2/3;

	if	(m_lpTarget)
	{
		int	iDestHeight	=	(m_lpTarget->getTargetHitHeight()+m_lpTarget->getHeight())*2/3;			//	목표 높이

		m_posTarget.x	=	m_lpTarget->m_pos.x;
		m_posTarget.y	=	m_lpTarget->m_pos.y-iDestHeight;

		if	(m_lpTarget->m_wBody	==	dJOB_MONSTER_TIAMATH_FEELER	||	m_lpTarget->m_wBody==	dJOB_MONSTER_TIAMATH_BOTTOM_FEELER)
		{
			m_posTarget.x	=	m_lpTarget->m_pos.x+m_lpTarget->getBody()->m_rectCrash.x1+m_lpTarget->getBody()->m_rectCrash.getWidth();
			m_posTarget.y	=	m_lpTarget->m_pos.y+m_lpTarget->getBody()->m_rectCrash.y1-m_lpTarget->getBody()->m_rectCrash.getHeight();
		}
	}

	m_posBegin.x	=	m_lpCaster->m_pos.x;
	m_posBegin.y	=	m_lpCaster->m_pos.y-iHeight;
	m_pos.x			=	m_lpCaster->m_pos.x;
	m_pos.y			=	m_lpCaster->m_pos.y-iHeight;

	int	iOldAngle	=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y);
						
	GetTargetPos(&m_pos,m_posTarget.x,m_posTarget.y,iBodySize);

	int	iCurAngle	=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y);

	if	(((iOldAngle + 360 - iCurAngle) %360) >= 120)
	{
		m_posBegin.x	=	m_lpCaster->m_pos.x;
		m_posBegin.y	=	m_lpCaster->m_pos.y-iHeight;
		m_pos.x			=	m_lpCaster->m_pos.x;
		m_pos.y			=	m_lpCaster->m_pos.y-iHeight;
	}

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wShootImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wShootImage]->GetFPS(0);

	m_wAngle		=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y,1);
	m_wDirect		=	GetDirect(m_wAngle,iDirectCount);

	m_iLastDistance	=	0x7ffffff;

	int	iRange		=	GetRange(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,m_lpTarget->m_pos.x,m_lpTarget->m_pos.y);
	m_wRange		=	max(sqrt(iRange),1);

}

//
//	미사일형 업데이트
BOOL
CActiveSkill::updateMissileTypeSkill()
{
	if	(!m_lpCaster || !m_lpSkill)
		return	TRUE;

	if	(m_wFrameCounter	==	0xffff	)
	{
		if	(m_lpCaster->m_isTriggerFrame)
		{
			castMissile();

			if	(m_wFrameCounter	==	0xffff	)
				return	TRUE;
		}

		return	FALSE;	//	아직 쏘기 직전이다.
	}

//	프레임 갱신
	if	(m_lpSkill->m_wShootImage	!=	0xffff)
	{
		if	(m_wFrameCounter	>=	dSYNC_FPS)
		{
			m_wFrameCounter	-=	dSYNC_FPS;
			m_wFrame++;

			if	(m_wFrame	>=	m_wMaxFrame)
				m_wFrame	=	0;
		}

		m_wFrameCounter	+=	m_wFPS;
	}

	CPos	pos		=	m_pos;

	if	(m_lpTarget)
	{
		if	(m_lpTarget->m_wSerial	==	m_lpCaster->m_wSerial)
			return	TRUE;

		int	iDestHeight	=	(m_lpTarget->getTargetHitHeight()+m_lpTarget->getHeight())*2/3;			//	목표 높이

		m_posTarget.x	=	m_lpTarget->m_pos.x;
		m_posTarget.y	=	m_lpTarget->m_pos.y-iDestHeight;

		if	(m_lpTarget->m_wBody	==	dJOB_MONSTER_TIAMATH_FEELER	||	m_lpTarget->m_wBody==	dJOB_MONSTER_TIAMATH_BOTTOM_FEELER)
		{
			m_posTarget.x	=	m_lpTarget->m_pos.x+m_lpTarget->getBody()->m_rectCrash.x1+m_lpTarget->getBody()->m_rectCrash.getWidth();
			m_posTarget.y	=	m_lpTarget->m_pos.y+m_lpTarget->getBody()->m_rectCrash.y1-m_lpTarget->getBody()->m_rectCrash.getHeight();
		}
	}

	GetTargetPos(&pos,m_posTarget.x,m_posTarget.y,m_lpSkill->m_wSpeed);

	int	iCurrentRange	=	GetOvalRange(pos.x,pos.y,m_posTarget.x,m_posTarget.y);

	if	(iCurrentRange	>=	m_iLastDistance	||	m_wRange	<=	m_lpSkill->m_wSpeed)
	{
		if	(m_lpSkill->m_wType	==	eSKILL_TYPE_THROW_ITEM)
			return	TRUE;

		if	(m_lpCaster	&&	m_lpTarget)
			m_lpCaster->strike(m_lpTarget,&m_ability,&m_hitInfo,m_wDirect,TRUE);

		return	TRUE;
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
//	미사일 찍기
inline	void
CActiveSkill::putMissile(int _iX,int _iY)
{
	g_im.put(m_lpSkill->m_wShootImage,_iX,_iY,m_wAnm,m_wDirect,m_wFrame,g_iGameScale,g_iGameScale);

	if	(m_lpSkill->m_wMissileHeadImage	==	0xffff)
		return;

	CPos	pos;

	pos.Set(_iX,_iY);

	GetAnglePos(&pos,m_wAngle,getMissileSize());

	g_im.putEffect(m_lpSkill->m_wMissileHeadImage,pos.x,pos.y,getMissileFrameCounter(),0,0,g_iGameScale,g_iGameScale);
}

//
//	스킬 이미지 그려주기
void
CActiveSkill::putMissileTypeSkill()
{
	if	(m_lpSkill->m_wShootImage==	0xffff)
		return;
	if	(m_wFrameCounter		==	0xffff)
		return;	//	아직 쏘기 직전이다.

	int		iShotCount	=	m_ability.getShotCount();
	cANM	*lpMissile	=	g_im.getEffect(m_lpSkill->m_wShootImage);
	int		iX,iY;

	if	(iShotCount	<=	1)
	{
		if	(m_lpSkill->m_bf1IsLaser)
		{
			int	iDirectCount=	lpMissile->GetDirectCount(0);

			m_wAngle		=	cANGLE::GetAngleToTarget(m_posBegin.x,m_posBegin.y,m_posTarget.x,m_posTarget.y,1);
			m_wDirect		=	GetDirect(m_wAngle,iDirectCount);
			
			lpMissile->linerPutByDestPos(GetScaledXPos(m_posBegin.x),GetScaledYPos(m_posBegin.y),GetScaledXPos(m_posTarget.x),GetScaledYPos(m_posTarget.y),m_wAnm,m_wDirect,m_wFrame,0,m_lpSkill->m_wAfterImageGap,g_iGameScale);

			return;
		}

		iX	=	GetScaledXPos(m_pos.x);
		iY	=	GetScaledYPos(m_pos.y);

		putMissile(iX,iY);

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

				iX	=	GetScaledXPos(pos.x);
				iY	=	GetScaledYPos(pos.y);

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

		return;
	}

	int	iAngle		=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y,1);
	int	iAngle1		=	iAngle+90;
	int	iAngle2		=	iAngle-90;

	if (iAngle1	>=	360	)
		iAngle1	-=	360;
	if (iAngle2	<	0	)
		iAngle2	+=	360;

	CPos	pos;

	if (iShotCount%2==0)
	{
		GetAnglePos(&pos,iAngle2,7);

		for (int i=0;i<iShotCount/2;i++)
		{
			pos	=	m_pos;

			GetAnglePos(&pos,iAngle1,7+i*14);

			iX	=	GetScaledXPos(pos.x);
			iY	=	GetScaledYPos(pos.y);

			putMissile(iX,iY);

			pos	=	m_pos;
			GetAnglePos(&pos,iAngle2,7+i*14);

			iX	=	GetScaledXPos(pos.x);
			iY	=	GetScaledYPos(pos.y);

			putMissile(iX,iY);
		}
	}
	else
	{
		putMissile(m_pos.x-g_map.m_pos.x,m_pos.y-g_map.m_pos.y);

		for (int i=0;i<iShotCount/2;i++)
		{
			pos	=	m_pos;

			GetAnglePos(&pos,iAngle1,(i+1)*14);
			putMissile(pos.x-g_map.m_pos.x,pos.y-g_map.m_pos.y);

			pos	=	m_pos;
			GetAnglePos(&pos,iAngle2,(i+1)*14);

			iX	=	GetScaledXPos(pos.x);
			iY	=	GetScaledYPos(pos.y);

			putMissile(iX,iY);
		}
	}
}