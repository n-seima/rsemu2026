#include "CSkill.H"
#include "CActor.H"
#include "cMAP.H"
#include "CSound.H"

//
//	토네이도 스킬 발사!!
int
CActiveSkill::fireTornadoTypeSkill()
{
	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);

	setTornadoCoreImage(m_lpSkill->m_wMachineImage);

	if (m_lpSkill->m_wMachineImage	!=	0xffff)
	{
		if (m_lpSkill->m_bf1IsSyncShieldShape)		//	방패 모양 싱크
		{
			cBasicItem	*lpShield	=	m_lpCaster->m_itemShield.getBasicItem();

			if (lpShield)
			{
				if (lpShield->m_wEquippedShape	!=	0xffff)
					setTornadoCoreImage(m_lpSkill->m_wMachineImage+lpShield->m_wEquippedShape);
			}
		}

		cANM	*lpCoreImage	=	(cANM*)g_apEffect[getTornadoCoreImage()];

		setTornadoCoreFrameCount(lpCoreImage->m_sprite.m_iCount);
		setTornadoCoreFPS(lpCoreImage->GetFPS(0));
	}

	return	eSKILL_FIRE_RESULT_OK;
}	//	CActiveSkill::fireTornadoTypeSkill()

//
//	캐스팅
void
CActiveSkill::castTornadoTypeSkill()
{
	m_wFrameCounter	=	0;						//	바로시작
	m_wRange		=	0;
	m_iLastDistance	=	0;					

	if (m_lpSkill->m_wSkillImage	==	0xffff)	return;

	int	iBodySize	=	m_lpCaster->getBodySize()+30;

	if (m_lpTarget)
	{
		m_posTarget.x	=	m_lpTarget->m_pos.x;
		m_posTarget.y	=	m_lpTarget->m_pos.y;
	}

	m_pos.x			=	m_lpCaster->m_pos.x;
	m_pos.y			=	m_lpCaster->m_pos.y;

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFPS(0);

	int	iDirectCount=	g_apEffect[m_lpSkill->m_wSkillImage]->GetDirectCount(0);

	m_wAngle		=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y,1);
	m_wDirect		=	GetDirect(m_wAngle,iDirectCount);
//	m_posTarget		=	m_pos;

	GetTargetPos(&m_pos,m_posTarget.x,m_posTarget.y,iBodySize);
//	GetAnglePos(&m_posTarget,m_wAngle,m_wRange);

	g_esm.playActionSound(m_lpSkill);
}

//
//	미사일형 업데이트
BOOL
CActiveSkill::updateTornadoTypeSkill()
{
	if (m_wFrameCounter	==	0xffff	)
	{
		if (m_lpCaster->m_isTriggerFrame)
			castTornadoTypeSkill();

		return	FALSE;	//	아직 쏘기 직전이다.
	}

//	프레임 갱신
	{
		m_iLastDistance++;

		if (m_wFrameCounter	>=	dSYNC_FPS)
		{
			m_wFrameCounter	-=	dSYNC_FPS;
			m_wFrame++;

			if (m_wFrame>=m_wMaxFrame)	m_wFrame	=	0;
		}

		m_wFrameCounter	+=	m_wFPS;
	}

	if	(m_iLastDistance%dUPDATE_PERIOD	==	0)
	{
		GetAnglePos(&m_pos,m_wAngle,m_lpSkill->m_wSpeed);

		m_wRange	+=	m_lpSkill->m_wSpeed;
	}

	if (m_wRange>=	m_lpSkill->m_wShootRange)
		return	TRUE;

	return FALSE;
}	//	CActiveSkill::updateTornadoTypeSkill()

//
//	스킬 이미지 그려주기
void
CActiveSkill::putTornadoTypeSkill()
{
	if	(m_lpSkill->m_wSkillImage	==	0xffff)	
		return;
	if	(m_wFrameCounter				==	0xffff)	
		return;	//	아직 쏘기 직전이다.

	int		iCoreImage	=	getTornadoCoreImage();
	CPos	pos;

	pos.x		=	GetScaledXPos(m_pos.x);
	pos.y		=	GetScaledYPos(m_pos.y)-m_sHeight;

	int	iScale	=	m_lpSkill->m_wImageScale*g_iGameScale/100;

	if	(iCoreImage	!=	0xffff)
	{
		CPos	posCore;
		cANM	*lpCoreImage	=	(cANM*)g_apEffect[iCoreImage];

		posCore.x		=	pos.x;
		posCore.y		=	pos.y;

		int	iFrameCount	=	getTornadoCoreFrameCount();
		int	iFPS		=	96;//getTornadoCoreFPS();

		int	iFrame		=	(m_iLastDistance*iFPS/dSYNC_FPS)%iFrameCount;

		GetAnglePos(&pos,m_iLastDistance*20%360,20);

		lpCoreImage->PutSprite(pos.x,posCore.y,iFrame,iScale,iScale,dPUT_HALF_BLENDING);
	}
	else
	{
		if	(m_lpSkill->m_bf1IsNotWhirlTornado == FALSE)
			GetAnglePos(&pos,m_iLastDistance*20%360,20);
	}

	g_apEffect[m_lpSkill->m_wSkillImage]->PutReg(pos.x,pos.y,m_wAnm,m_wDirect,m_wFrame,iScale,iScale);
}