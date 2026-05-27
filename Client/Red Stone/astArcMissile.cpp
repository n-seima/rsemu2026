
#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

BOOL
cAS_ArcMissile::fire()
{

	if(!m_lpSkill || !m_lpTarget)
		return eSKILL_FIRE_RESULT_FAILED;

	if	(m_lpSkill->m_wShootImage	==	0xffff)
		return	eSKILL_FIRE_RESULT_OK;
	
	cANM	*lpMissile	=	g_im.getEffect(m_lpSkill->m_wShootImage);
	
	if	(!lpMissile)
		return	eSKILL_FIRE_RESULT_OK;
	
	m_lpTarget->m_wTargetMark	=	m_lpSkill->m_wTargetMarkImage;
	m_wMissileSize			=	0;
	m_wMissileFrameCounter	=	0;
	m_wHeight				=	0;	
	m_iDistance				=	0;
	if	(lpMissile)
	{
		m_wAnm		=	random(lpMissile->m_iAnmCount);
		
		int	iSize	=	lpMissile->m_sprite.getSpriteHeight(0);
		
		m_wMissileSize = iSize/2;
	}

	if(!cast())
		return	eSKILL_FIRE_RESULT_OK;

	return	eSKILL_FIRE_RESULT_OK;

}

BOOL
cAS_ArcMissile::update()
{
	if	(!m_lpCaster || !m_lpSkill || !m_lpTarget)
		return	TRUE;

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
	

	int iSpeed	=	m_lpSkill->m_wSpeed;
	if(g_apEffect[m_lpSkill->m_wShootImage]->m_iCrashSize<iSpeed)
	{
		iSpeed = g_apEffect[m_lpSkill->m_wShootImage]->m_iCrashSize;
	}
	

	int	iCurrentRange	= GetOvalRange(pos.x,pos.y,m_posTarget.x,m_posTarget.y);

	if(m_iDistance < iCurrentRange)
		m_iDistance	= iCurrentRange;

	int	iValue		=	((float)iCurrentRange/m_iDistance) * 180 ;
	iValue			=	max(0,iValue);
	m_wHeight = dCOS_TABLE[iValue%180]/256.0*m_lpSkill->m_bf11ArcHeight;

	GetTargetPos(&pos,m_posTarget.x,m_posTarget.y,iSpeed);


	if(iCurrentRange <= g_apEffect[m_lpSkill->m_wShootImage]->m_iCrashSize * g_apEffect[m_lpSkill->m_wShootImage]->m_iCrashSize)
	{
		if	(m_lpSkill->m_wType	==	eSKILL_TYPE_THROW_ITEM)
			return	TRUE;

		m_lpCaster->strike(m_lpTarget,&m_ability,&m_hitInfo,m_wDirect,FALSE);

		return	TRUE;
	}

	if	(m_lpSkill->m_wMissileFollowImage	!=	0xffff)
	{
		CPos	posFollow	=	m_pos;
		posFollow.y	-=m_wHeight;
		if	(m_lpSkill->m_wMissileDustRange)
			GetTargetPos(&posFollow,m_posBegin.x,m_posBegin.y,m_lpSkill->m_wMissileDustRange);
		g_floatEffect.add(posFollow.x,posFollow.y,m_lpSkill->m_wMissileFollowImage,m_lpSkill->m_wDustImageRange,m_lpSkill->m_bf1IsRandomDustImageFrame,m_wDirect);
	}

	m_iLastDistance	=	iCurrentRange;
	m_pos			=	pos;

	return FALSE;

}


BOOL
cAS_ArcMissile::cast()
{
	if	(!m_lpCaster || !m_lpSkill || !m_lpTarget)
		return	FALSE;
	
	g_esm.playActionSound(m_lpSkill);
	

	if	(m_lpTarget->m_wSerial	==	m_lpCaster->m_wSerial)
	{
		if	(m_hitInfo.isAttackToShakle())
			m_lpCaster->strike(m_lpTarget,&m_ability,&m_hitInfo,m_wDirect,TRUE);
		
		return	FALSE;
	}
	
	m_wFrameCounter	=	0;						//	바로시작
	
	if	(m_lpSkill->m_wShootImage	==	0xffff)
		return	FALSE;
	
	int	iBodySize	=	m_lpCaster->getBodySize(FALSE)+30;
	int	iDirectCount=	g_apEffect[m_lpSkill->m_wShootImage]->GetDirectCount(0);
	int	iHeight		=	(m_lpCaster->getBodyHeight(FALSE)+m_lpCaster->getHeight())*2/3;
	

	int	iDestHeight	=	(m_lpTarget->getTargetHitHeight()+m_lpTarget->getHeight())*2/3;			//	목표 높이
	
	m_posTarget.x	=	m_lpTarget->m_pos.x;
	m_posTarget.y	=	m_lpTarget->m_pos.y-iDestHeight;
	
	if	(m_lpTarget->m_wBody	==	dJOB_MONSTER_TIAMATH_FEELER	||	m_lpTarget->m_wBody==	dJOB_MONSTER_TIAMATH_BOTTOM_FEELER)
	{
		m_posTarget.x	=	m_lpTarget->m_pos.x+m_lpTarget->getBody()->m_rectCrash.x1+m_lpTarget->getBody()->m_rectCrash.getWidth();
		m_posTarget.y	=	m_lpTarget->m_pos.y+m_lpTarget->getBody()->m_rectCrash.y1-m_lpTarget->getBody()->m_rectCrash.getHeight();
	}

	iHeight*=2;
	m_posBegin.x	=	m_lpCaster->m_pos.x;
	m_posBegin.y	=	m_lpCaster->m_pos.y-iHeight;
	m_pos.x			=	m_posBegin.x;
	m_pos.y			=	m_posBegin.y;

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wShootImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wShootImage]->GetFPS(0);
	
	m_wAngle		=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y,1);
	m_wDirect		=	GetDirect(m_wAngle,iDirectCount);
	
	m_iLastDistance	=	0x7ffffff;
	
	int	iRange		=	GetRange(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,m_lpTarget->m_pos.x,m_lpTarget->m_pos.y);
	m_wRange		=	max(sqrt(iRange),1);

	
	return TRUE;
}

void
cAS_ArcMissile::put()
{
	
	
	if	(!m_lpSkill || m_lpSkill->m_wShootImage==	0xffff)
		return;
	if	(m_wFrameCounter		==	0xffff)
		return;	//	아직 쏘기 직전이다.

	cANM	*lpMissile	=	g_im.getEffect(m_lpSkill->m_wShootImage);

	if	(!lpMissile)
		return;

	
	int iX	=	GetScaledXPos(m_pos.x);
	int iY	=	GetScaledYPos(m_pos.y - m_wHeight);

	g_im.put(m_lpSkill->m_wShootImage,iX,iY,m_wAnm,m_wDirect,m_wFrame,g_iGameScale,g_iGameScale);

	
}