#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

int
CActiveSkill::fireBoomerang()
{
	if (m_lpSkill->m_wShootImage == 0xffff)	return	eSKILL_FIRE_RESULT_FAILED;

	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);
	m_hitInfo.copy(&m_lpCaster->m_attackInfo.m_aInfo[0]);

	m_sHeight		=	m_lpCaster->getBodyHeight()*2/3;
	m_sDestHeight	=	m_lpTarget->getBodyHeight()*2/3;			//	목표 높이
	m_wFrameCounter	=	0xffff;
	setBoomerangImage(m_lpSkill->m_wShootImage);

	if (m_lpSkill->m_bf1IsSyncShieldShape)		//	방패 모양 싱크
	{
		cBasicItem	*lpShield	=	m_lpCaster->m_itemShield.getBasicItem();

		if (lpShield)
		{
			if (lpShield->m_wEquippedShape	!=	0xffff)
				setBoomerangImage(m_lpSkill->m_wShootImage+lpShield->m_wEquippedShape);
		}
	}

	return	eSKILL_FIRE_RESULT_OK;
}

void
CActiveSkill::castBoomerang()
{
	CRY(!m_lpTarget || !m_lpCaster,"cry in CActiveSkill::castMissile()");

	m_wFrameCounter	=	0;						//	바로시작

	if (m_lpSkill->m_wShootImage	==	0xffff)	return;

	int	iBodySize	=	m_lpCaster->getBodySize()+30;
	int	iDirectCount=	g_apEffect[m_lpSkill->m_wShootImage]->GetDirectCount(0);

	m_posTarget.x	=	m_lpTarget->m_pos.x;
	m_posTarget.y	=	m_lpTarget->m_pos.y-m_lpTarget->getHeight();
	m_posBegin.x	=	m_lpCaster->m_pos.x;
	m_posBegin.y	=	m_lpCaster->m_pos.y-m_lpCaster->getHeight();

	GetTargetPos(&m_pos,m_posTarget.x,m_posTarget.y,iBodySize);

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wShootImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wShootImage]->GetFPS(0);

	m_wAngle		=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y,1);
	m_wDirect		=	GetDirect(m_wAngle,iDirectCount);

	setBoomerangSpeed(m_lpSkill->m_wSpeed);
	setBoomerangStatus(eBS_FLY_TO_TARGET);
	setBoomerangAngle(m_lpSkill->m_wAngle);

	m_iLastDistance	=	0x7ffffff;

	m_sCurAngle		=	cANGLE::GetAngleToTarget1024(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y);	//	최초에 타겟과의 각도

	m_posAcc.x		=	INTTOFIXED ( m_pos.x );
	m_posAcc.y		=	INTTOFIXED ( m_pos.y );

	g_esm.playActionSound(m_lpSkill);

	if (m_lpSkill->isHideShield())
		m_lpCaster->m_wIsHideShield	=	TRUE;
}

BOOL
CActiveSkill::updateFlyBoomerang()
{
	increaseBoomerangStep();

	if (getBoomerangStatus() == eBS_FLY_TO_TARGET)
	{
		if (m_lpTarget)
		{
			m_posTarget.x	=	m_lpTarget->m_pos.x;
			m_posTarget.y	=	m_lpTarget->m_pos.y-m_lpTarget->getHeight();
		}

		GetTargetPos(&m_pos,m_posTarget.x,m_posTarget.y,getBoomerangSpeed());

		int	iCurrentRange	=	GetOvalRange(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y);

		if (iCurrentRange	>=	m_iLastDistance)
		{
			if (m_lpCaster	&&	m_lpTarget)
				m_lpCaster->strike(m_lpTarget,&m_ability,&m_hitInfo,m_wDirect,TRUE);

			setBoomerangStatus(eBS_DECREASE_SPEED);

			m_posTarget.x	=	m_lpCaster->m_pos.x;
			m_posTarget.y	=	m_lpCaster->m_pos.y-m_lpCaster->getHeight();
				
			m_posAcc.x		=	INTTOFIXED ( m_pos.x );
			m_posAcc.y		=	INTTOFIXED ( m_pos.y );

			return	FALSE;
		}

		m_iLastDistance	=	iCurrentRange;

		return	FALSE;
	}

	if (getBoomerangStatus() == eBS_DECREASE_SPEED)
	{
		int	iAngle	=	m_lpSkill->m_wAngle;
		int	iSpeed	=	getBoomerangSpeed();

		iSpeed		=	iSpeed*4/5;

		if (iSpeed <= 1)	iSpeed	=	2;

		setBoomerangSpeed(iSpeed);

		m_posTarget.x		=	m_lpCaster->m_pos.x;
		m_posTarget.y		=	m_lpCaster->m_pos.y-m_lpCaster->getHeight();

		int iExactAngle		=	cANGLE::GetAngleToTarget1024(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y);
		int	iAngleGab		=	abs ( m_sCurAngle - iExactAngle );

		if ( iAngleGab < iAngle )	m_sCurAngle	=	iExactAngle;
		else						m_sCurAngle	+=	cANGLE::GetAngleDir	(m_sCurAngle, iExactAngle)*iAngle;

		m_sCurAngle			=	cANGLE::ClipAngle(m_sCurAngle);
		m_posAcc.x			+=	cANGLE::GetCos(m_sCurAngle )*getBoomerangSpeed();
		m_posAcc.y			-=	cANGLE::GetSin(m_sCurAngle )*getBoomerangSpeed();

		m_pos.x				=	FIXEDTOINT ( m_posAcc.x);	// X,Y 좌표 계산...(실 좌표에 해당함..)
		m_pos.y				=	FIXEDTOINT ( m_posAcc.y);

		iAngleGab			=	abs ( m_sCurAngle - iExactAngle );

		if (iAngleGab		<	128)
		{
			iSpeed			=	2;

			setBoomerangStatus(eBS_RETURN_TO_OWNER);

			return	FALSE;
		}

		return	FALSE;
	}

	if (getBoomerangStatus() == eBS_RETURN_TO_OWNER)
	{
		int	iSpeed		=	getBoomerangSpeed();
		int	iAngle		=	min(getBoomerangAngle(),1024/10);

		increaseBoomerangAngle(1);

		int	iDeltaSpeed	=	max(iSpeed/6,1);

		iSpeed			+=	iDeltaSpeed;
		iSpeed			=	min(m_lpSkill->m_wSpeed,iSpeed);

		setBoomerangSpeed(iSpeed);

		m_posTarget.x		=	m_lpCaster->m_pos.x;
		m_posTarget.y		=	m_lpCaster->m_pos.y-m_lpCaster->getHeight();;

		int iExactAngle		=	cANGLE::GetAngleToTarget1024(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y);
		int	iAngleGab		=	abs ( m_sCurAngle - iExactAngle );

		if ( iAngleGab < iAngle )	m_sCurAngle	=	iExactAngle;
		else						m_sCurAngle	+=	cANGLE::GetAngleDir	(m_sCurAngle, iExactAngle)*iAngle;

		m_sCurAngle			=	cANGLE::ClipAngle	(m_sCurAngle);

		m_posAcc.x			+=	cANGLE::GetCos ( m_sCurAngle )*getBoomerangSpeed();
		m_posAcc.y			-=	cANGLE::GetSin ( m_sCurAngle )*getBoomerangSpeed();

		m_pos.x				=	FIXEDTOINT ( m_posAcc.x);	// X,Y 좌표 계산...(실 좌표에 해당함..)
		m_pos.y				=	FIXEDTOINT ( m_posAcc.y);

		int	iCurrentRange	=	GetOvalRange(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y);

		if (iCurrentRange < iSpeed*iSpeed || getBoomerangStep() >= dSYNC_FPS*3)
			return	TRUE;

		return	FALSE;
	}

	return FALSE;
}

BOOL
CActiveSkill::updateBoomerang()
{
	if (m_wFrameCounter	==	0xffff	)
	{
		if (m_lpCaster->m_isTriggerFrame)	castBoomerang();
		
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

	if (updateFlyBoomerang())	return	TRUE;
	
	return	FALSE;
}

//
//	부메랑 찍어!!
void
CActiveSkill::putBoomerang()
{
	if (m_wFrameCounter			==	0xffff)	
		return;	//	아직 쏘기 직전이다.

	int	iX	=	GetScaledXPos(m_pos.x);
	int	iY	=	GetScaledYPos(m_pos.y);

	g_apEffect[getBoomerangImage()]->PutReg(iX,iY-m_sHeight,m_wAnm,m_wDirect,m_wFrame,g_iGameScale,g_iGameScale);
}