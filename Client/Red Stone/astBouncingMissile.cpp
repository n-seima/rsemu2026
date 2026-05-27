#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"


int
cAS_BouncingMissile::fire()
{
	reset();

	if	(!m_lpTarget || !m_lpCaster)
		return	eSKILL_FIRE_RESULT_FAILED;

	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);

	return	eSKILL_FIRE_RESULT_OK;

}



BOOL
cAS_BouncingMissile::update()
{

	if(!m_lpCaster || !m_lpSkill)
		return TRUE;
	if (m_lpCaster->m_isTriggerFrame)
	{
		g_sm.castFlyBouncingMissile(m_lpCaster, m_lpTarget, m_lpCaster->m_pos.x, m_lpCaster->m_pos.y, &m_ability, m_awTargetList, m_wTargetCount);
		return TRUE;
	}

	return FALSE;
}

void
cAS_FlyBouncingMissile::reset()
{

	((cAS_GroupBase*)this)->reset();
	m_wCurrentCount			=	0;
	m_wMissileSize			=	0;
	m_wMissileFrameCounter	=	0;
	m_posStart.x			=	0;
	m_posStart.y			=	0;

}

BOOL
cAS_FlyBouncingMissile::fire()
{
	reset();

	if	(m_lpSkill->m_wShootImage	==	0xffff)
		return	eSKILL_FIRE_RESULT_OK;
	
	cANM	*lpMissile	=	g_im.getEffect(m_lpSkill->m_wShootImage);
	
	if	(!lpMissile)
		return	eSKILL_FIRE_RESULT_OK;
	
	m_lpTarget->m_wTargetMark	=	m_lpSkill->m_wTargetMarkImage;
	
	if	(lpMissile)
	{
		m_wAnm		=	random(lpMissile->m_iAnmCount);
		
		int	iSize	=	lpMissile->m_sprite.getSpriteHeight(0);
		
		m_wMissileSize = iSize/2;
	}

	m_posStart	=	m_lpCaster->m_pos;
	
	g_esm.playActionSound(m_lpSkill);

	return	eSKILL_FIRE_RESULT_OK;

}
BOOL
cAS_FlyBouncingMissile::update()
{

	if(updateBouncingMissile())
	{		
		++m_wCurrentCount;
		
		if(m_wCurrentCount>=m_wTargetCount)
			return	TRUE;
		if(m_lpTarget)
			m_posStart	=	m_lpTarget->m_pos;
		if(!cast())
			return TRUE;
	}

	return FALSE;

}

BOOL
cAS_FlyBouncingMissile::updateBouncingMissile()
{

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
	++m_wMissileFrameCounter;
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
		{
			m_lpCaster->strike(m_lpTarget,&m_ability,&m_vecContinuousHit[m_wCurrentCount],m_wDirect,FALSE);
		}

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
BOOL
cAS_FlyBouncingMissile::cast()
{
	if	(!m_lpCaster || !m_lpSkill || !m_lpTarget)
		return	FALSE;

	
	m_lpTarget	=	g_am.getTestedActor(m_awTargetList[m_wCurrentCount]);
	 if(!m_lpTarget)
		 return FALSE;
	
	g_esm.playActionSound(m_lpSkill);
	
	if	(m_lpTarget)
	{
		if	(m_lpTarget->m_wSerial	==	m_lpCaster->m_wSerial)
		{
			if	(m_hitInfo.isAttackToShakle())
				m_lpCaster->strike(m_lpTarget,&m_ability,&m_vecContinuousHit[m_wCurrentCount],m_wDirect,TRUE);
			
			return	FALSE;
		}
	}	
	m_wFrameCounter	=	0;						//	바로시작
	
	if	(m_lpSkill->m_wShootImage	==	0xffff)
		return	FALSE;
	
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
	
	m_posBegin.x	=	m_posStart.x;
	m_posBegin.y	=	m_posStart.y-iHeight;
	m_pos.x			=	m_posStart.x;
	m_pos.y			=	m_posStart.y-iHeight;
	
	int	iOldAngle	=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y);
	
	GetTargetPos(&m_pos,m_posTarget.x,m_posTarget.y,iBodySize);
	
	int	iCurAngle	=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y);
	
	if	(((iOldAngle + 360 - iCurAngle) %360) >= 120)
	{
		m_posBegin.x	=	m_posStart.x;
		m_posBegin.y	=	m_posStart.y-iHeight;
		m_pos.x			=	m_posStart.x;
		m_pos.y			=	m_posStart.y-iHeight;
	}
	
	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wShootImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wShootImage]->GetFPS(0);
	
	m_wAngle		=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y,1);
	m_wDirect		=	GetDirect(m_wAngle,iDirectCount);
	
	m_iLastDistance	=	0x7ffffff;
	
	int	iRange		=	GetRange(m_posStart.x,m_posStart.y,m_lpTarget->m_pos.x,m_lpTarget->m_pos.y);
	m_wRange		=	max(sqrt(iRange),1);
	
	return TRUE;
}

void
cAS_FlyBouncingMissile::put()
{
	
	
	if	(m_lpSkill->m_wShootImage==	0xffff)
		return;
	if	(m_wFrameCounter		==	0xffff)
		return;	//	아직 쏘기 직전이다.

	cANM	*lpMissile	=	g_im.getEffect(m_lpSkill->m_wShootImage);

	if	(!lpMissile)
		return;

	
	int iX	=	GetScaledXPos(m_pos.x);
	int iY	=	GetScaledYPos(m_pos.y);

	g_im.put(m_lpSkill->m_wShootImage,iX,iY,m_wAnm,m_wDirect,m_wFrame,g_iGameScale,g_iGameScale);

	
}
