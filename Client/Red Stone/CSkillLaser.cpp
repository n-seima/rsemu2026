#include "CSkill.H"
#include "CActor.H"
#include "cMAP.H"
#include "CSound.H"

//
//	미사일형 스킬 사용
int
CActiveSkill::fireLaserTypeSkill()
{
	CRY(!m_lpTarget || !m_lpCaster,"cry in CActiveSkill::fireLaserTypeSkill()");

	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);

	m_posBegin		=	m_pos;
	m_sHeight		=	m_lpCaster->getBodyHeight()*2/3;
	m_sDestHeight	=	m_lpTarget->getBodyHeight()*2/3;			//	목표 높이

	return	eSKILL_FIRE_RESULT_OK;
}	//	CActiveSkill::fireLaserTypeSkill()

//
//	캐스팅
void
CActiveSkill::castLaserTypeSkill()
{
	m_wFrameCounter	=	0;						//	바로시작

	if (m_lpSkill->m_wShootImage	==	0xffff)	return;

	int	iBodySize	=	m_lpCaster->getBodySize()+30;

	m_posTarget.x	=	m_lpTarget->m_pos.x;
	m_posTarget.y	=	m_lpTarget->m_pos.y;

	GetTargetPos(&m_pos,m_posTarget.x,m_posTarget.y,iBodySize);

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wShootImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wShootImage]->GetFPS(0);

	int	iDirectCount=	g_apEffect[m_lpSkill->m_wShootImage]->GetDirectCount(0);

	m_wAngle		=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y,1);
	m_wDirect		=	GetDirect(m_wAngle,iDirectCount);

	m_posTarget.x	=	m_lpCaster->m_pos.x;
	m_posTarget.y	=	m_lpCaster->m_pos.y;

	GetAnglePos(&m_posTarget,m_wAngle,m_wRange);

	g_esm.playActionSound(m_lpSkill);

}	//	CActiveSkill::castLaserTypeSkill()

//
//	미사일형 업데이트
BOOL
CActiveSkill::updateLaserTypeSkill()
{
	if (m_wFrameCounter	==	0xffff	)
	{
		if (m_lpCaster->m_isTriggerFrame)
			castLaserTypeSkill();

		return	FALSE;	//	아직 쏘기 직전이다.
	}

//	프레임 갱신
	{
		if (m_wFrameCounter	>=	dSYNC_FPS)
		{
			m_wFrameCounter	-=	dSYNC_FPS;
			m_wFrame++;

			if (m_wFrame>=m_wMaxFrame)	m_wFrame	=	0;
		}

		m_wFrameCounter	+=	m_wFPS;
	}

	CPos	pos		=	m_pos;

	GetTargetPos(&pos,m_posTarget.x,m_posTarget.y,m_lpSkill->m_wSpeed);

	int	iRange	=	GetOvalRange(m_posBegin.x,m_posBegin.y,pos.x,pos.y);

	if (iRange	>=	m_wRange*m_wRange)	return	TRUE;

	m_pos	=	pos;

	return FALSE;
}	//	CActiveSkill::updateLaserTypeSkill()

//
//	스킬 이미지 그려주기
void
CActiveSkill::putLaserTypeSkill()
{
	if (m_lpSkill->m_wShootImage==	0xffff)	return;
	if (m_wFrameCounter			==	0xffff)	return;	//	아직 쏘기 직전이다.

	int	x		=	GetScaledXPos(m_pos.x);
	int	y		=	GetScaledYPos(m_pos.y);
	
	g_apEffect[m_lpSkill->m_wShootImage]->PutReg(x,y-m_sHeight,m_wAnm,m_wDirect,m_wFrame,g_iGameScale,g_iGameScale);

/*
	int		iHitRange	=	m_ability.getHitRange(NULL);
	int		iAttackRange=	m_wRange;

	m_posTarget.x	=	m_lpTarget->m_pos.x;
	m_posTarget.y	=	m_lpTarget->m_pos.y;

	CPos	posX1Y1,posX2Y1,posX1Y2,posX2Y2,posDest;

	int		iAngle		=	cANGLE::GetAngleToTarget(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,m_posTarget.x,m_posTarget.y,2);
	int		iAngle1		=	iAngle+90;
	int		iAngle2		=	iAngle-90;

	if (iAngle1	>=	360	)	iAngle1	-=	360;
	if (iAngle2	<	0	)	iAngle2	+=	360;

	posDest.x	=	m_lpCaster->m_pos.x;
	posDest.y	=	m_lpCaster->m_pos.y;

	GetOvalAnglePos(&posDest,iAngle,iAttackRange);

	posX1Y1		=	posDest;
	posX2Y1		=	posDest;
	GetAnglePos(&posX1Y1,iAngle2,iHitRange/2);
	GetAnglePos(&posX2Y1,iAngle1,iHitRange/2);

	posX1Y2.x	=	m_lpCaster->m_pos.x;
	posX1Y2.y	=	m_lpCaster->m_pos.y;
	posX2Y2		=	posX1Y2;
	GetAnglePos(&posX1Y2,iAngle2,iHitRange/2);
	GetAnglePos(&posX2Y2,iAngle1,iHitRange/2);
	posX1Y1.Move(-g_map.m_pos.x,-g_map.m_pos.y);
	posX1Y2.Move(-g_map.m_pos.x,-g_map.m_pos.y);
	posX2Y1.Move(-g_map.m_pos.x,-g_map.m_pos.y);
	posX2Y2.Move(-g_map.m_pos.x,-g_map.m_pos.y);
	cDRAW::Line(_LTGREEN,posX1Y1.x,posX1Y1.y,posX2Y1.x,posX2Y1.y);
	cDRAW::Line(_LTGREEN,posX2Y1.x,posX2Y1.y,posX2Y2.x,posX2Y2.y);
	cDRAW::Line(_LTGREEN,posX2Y2.x,posX2Y2.y,posX1Y2.x,posX1Y2.y);
	cDRAW::Line(_LTGREEN,posX1Y2.x,posX1Y2.y,posX1Y1.x,posX1Y1.y);
*/
//	cDRAW::Line(_LTGREEN,m_lpCaster->m_pos.x-g_map.m_pos.x,m_lpCaster->m_pos.y-g_map.m_pos.y,m_pos.x-g_map.m_pos.x,m_pos.y-g_map.m_pos.y);
}	//	CActiveSkill::putMisslieTypeSkill()
