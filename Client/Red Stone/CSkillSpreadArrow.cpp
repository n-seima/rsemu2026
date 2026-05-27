#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	미사일형 스킬 사용
int
CActiveSkill::fireSpreadArrow()
{
	int	iAngle		=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y,1);

	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);

	m_sHeight		=	m_lpCaster->getBodyHeight(FALSE)*2/3;

	cANM	*lpMissile	=	g_im.getEffect(m_lpSkill->m_wShootImage);

	if	(lpMissile)
	{
		m_wAnm		=	random(lpMissile->m_iAnmCount);

		int	iSize	=	lpMissile->m_sprite.getSpriteHeight(0);

		setMissileSize(iSize/2);
	}

	return	eSKILL_FIRE_RESULT_OK;
}

void
CActiveSkill::castSpreadArrow()
{
	m_wFrameCounter	=	0;						//	바로시작

	if	(m_lpSkill->m_wShootImage	==	0xffff)
		return;

	int	iBodySize	=	m_lpCaster->getBodySize()+30;
	int	iDirectCount=	g_apEffect[m_lpSkill->m_wShootImage]->GetDirectCount(0);

	CPos	posOld	=	m_pos;

	int	iOldAngle	=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y);
						
	GetTargetPos(&m_pos,m_posTarget.x,m_posTarget.y,iBodySize);

	int	iCurAngle	=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y);

	if	(((iOldAngle + 360 - iCurAngle) %360) >= 120)
		m_pos	=	posOld;
	
	m_pos.y			-=	m_lpCaster->getHeight();

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wShootImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wShootImage]->GetFPS(0);

	m_iLastDistance	=	m_lpSkill->m_wSpeed;

	g_esm.playActionSound(m_lpSkill);
}

//
//	미사일형 업데이트
BOOL
CActiveSkill::updateSpreadArrow()
{
	if (m_wFrameCounter	==	0xffff	)
	{
		if (m_lpCaster->m_isTriggerFrame)	castSpreadArrow();

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

	m_iLastDistance	+=	m_lpSkill->m_wSpeed;

	if (m_iLastDistance	>=	m_wRange)
		return	TRUE;

	return	FALSE;	//	아직 쏘기 직전이다.
}

//
//	미사일 찍기
inline	void
CActiveSkill::putSpreadArrow(int _iX,int _iY,int _iDirect,int _iAngle)
{
	g_im.put(m_lpSkill->m_wShootImage,_iX,_iY,m_wAnm,_iDirect,m_wFrame,g_iGameScale,g_iGameScale);

	if (m_lpSkill->m_wMissileHeadImage	==	0xffff)
		return;

	CPos	pos;

	pos.Set(_iX,_iY);

	GetAnglePos(&pos,_iAngle,getMissileSize());

	g_im.putEffect(m_lpSkill->m_wMissileHeadImage,pos.x,pos.y,getMissileFrameCounter(),g_iGameScale,g_iGameScale);
}

//
//	스킬 이미지 그려주기
void
CActiveSkill::putSpreadArrow()
{
	if (m_lpSkill->m_wShootImage==	0xffff)
		return;
	if (m_wFrameCounter			==	0xffff)
		return;	//	아직 쏘기 직전이다.

	int	iShotCount	=	m_ability.getShotCount();	//	이거 홀수로 만들것

	int	iAngle		=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y,1);
	int	iAngle1		=	iAngle;
	int	iAngle2		=	iAngle;
	int	iDirectCount=	g_apEffect[m_lpSkill->m_wShootImage]->GetDirectCount(0);
	int	iDirect;
	int	iRange		=	10;
	int	iX,iY;

	CPos	pos;

	if (iShotCount%2==	0)
	{
		iAngle1		+=	2;
		iAngle2		-=	3;
	}
	else
	{
		iDirect		=	GetDirect(iAngle,iDirectCount);
		GetAnglePos(&pos,iAngle,m_iLastDistance);
		putSpreadArrow(pos.x,pos.y,iDirect,iAngle);
	}

	for (int i=0;i<iShotCount/2;i++)
	{
		pos		=	m_pos;
		iAngle	=	(iAngle1+360)%360;
		iDirect	=	GetDirect(iAngle,iDirectCount);
		GetAnglePos(&pos,iAngle,m_iLastDistance);

		iX		=	GetScaledXPos(pos.x);
		iY		=	GetScaledYPos(pos.y);
		putSpreadArrow(iX,iY-m_sHeight,iDirect,iAngle);
		iAngle1	+=	5;

		pos		=	m_pos;
		iAngle	=	(iAngle2+360)%360;
		iDirect	=	GetDirect(iAngle,iDirectCount);
		GetAnglePos(&pos,iAngle,m_iLastDistance);

		iX		=	GetScaledXPos(pos.x);
		iY		=	GetScaledYPos(pos.y);
		putSpreadArrow(iX,iY-m_sHeight,iDirect,iAngle);
		iAngle2	-=	5;
	}
}