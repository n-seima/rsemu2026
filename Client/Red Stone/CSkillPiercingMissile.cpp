#include "CSkill.H"
#include "CActor.H"
#include "cMAP.H"
#include "CSound.H"

//
//	미사일형 스킬 사용
int
CActiveSkill::firePiercingMissileTypeSkill()
{
	CRY(!m_lpTarget || !m_lpCaster,"cry in CActiveSkill::firePiercingMissileTypeSkill()");

	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);

	m_sHeight		=	m_lpCaster->getBodyHeight(FALSE)*2/3+m_lpCaster->getHeight()*2/3;
	m_sDestHeight	=	m_sHeight;//m_lpTarget->getTargetHitHeight()*2/3+m_lpTarget->getHeight()*2/3;			//	목표 높이

	setPMCasterPos(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y);

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	캐스팅
void
CActiveSkill::castPiercingMissile()
{
	if	(m_lpSkill->m_wShootImage	==	0xffff)
		return;

	CRY(!m_lpTarget || !m_lpCaster,"cry in CActiveSkill::castPiercingMissile()");

	int	iBodySize	=	m_lpCaster->getBodySize()+30;
	int	iDirectCount=	g_apEffect[m_lpSkill->m_wShootImage]->GetDirectCount(0);

	m_posTarget.x	=	m_lpTarget->m_pos.x;
	m_posTarget.y	=	m_lpTarget->m_pos.y;
	m_pos.x			=	m_lpCaster->m_pos.x;
	m_pos.y			=	m_lpCaster->m_pos.y;

	GetTargetPos(&m_pos,m_posTarget.x,m_posTarget.y,iBodySize);

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wShootImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wShootImage]->GetFPS(0);
	m_wFrameCounter	=	0;						//	바로시작

	m_wAngle		=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y,1);
	m_wDirect		=	GetDirect(m_wAngle,iDirectCount);
	m_wAngle		=	cANGLE::GetAngleToTarget(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,m_posTarget.x,m_posTarget.y,1);

	m_posTarget.x	=	m_lpCaster->m_pos.x;
	m_posTarget.y	=	m_lpCaster->m_pos.y;

	GetAnglePos(&m_posTarget,m_wAngle,m_wRange);

	m_pos.y			-=	m_sHeight;
	m_posTarget.y	-=	m_sDestHeight;

	m_posBegin		=	m_pos;

//	GetAnglePos(&m_posBegin,m_wAngle,m_lpCaster->getBodySize()+m_lpSkill->m_wSpeed);

	g_esm.playActionSound(m_lpSkill);
}

//
//	미사일형 업데이트
BOOL
CActiveSkill::updatePiercingMissileTypeSkill()
{
	if (m_wFrameCounter	==	0xffff	)
	{
		if (m_lpCaster->m_isTriggerFrame)
			castPiercingMissile();

		return	FALSE;	//	아직 쏘기 직전이다.
	}

//	프레임 갱신
	{
		if (m_wFrameCounter	>=	dSYNC_FPS)
		{
			m_wFrameCounter	-=	dSYNC_FPS;
			m_wFrame++;

			if (m_wFrame>=m_wMaxFrame)
				m_wFrame	=	0;
		}
		
		m_wFrameCounter	+=	m_wFPS;
	}

	CPos	pos	=	m_pos,posCaster;

	getPMCasterPos(&posCaster);
	GetTargetPos(&pos,m_posTarget.x,m_posTarget.y,m_lpSkill->m_wSpeed);

	if	(m_lpSkill->m_bf1IsLaser)
	{
		int	iRange	=	GetOvalRange(m_posTarget.x,m_posTarget.y,pos.x,pos.y);

		if	(iRange	<=	m_lpSkill->m_wSpeed*m_lpSkill->m_wSpeed)
			return	TRUE;
	}
	else
	{
		int	iRange	=	GetOvalRange(posCaster.x,posCaster.y,pos.x,pos.y);

		if	(iRange	>=	m_wRange*m_wRange)
			return	TRUE;
	}

	m_pos	=	pos;

	return FALSE;
}