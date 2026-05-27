#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	미사일형 스킬 사용
int
CActiveSkill::fireTailChaserMissile(int _iCountDown,int _iDamage)
{
	if (m_lpSkill->m_wShootImage	==	0xffff)	return	eSKILL_FIRE_RESULT_FAILED;

	setTailChaserDamage(_iDamage);
	setTailChaserFireTime(_iCountDown*dUPDATE_PERIOD);

	m_sHeight		=	m_lpCaster->getBodyHeight()*2/3;
	m_sDestHeight	=	m_lpTarget->getBodyHeight()*2/3;			//	목표 높이

	return	eSKILL_FIRE_RESULT_OK;
}	//	CActiveSkill::fireMissileTypeSkill()

//
//	머쉰 미사일 발사
void
CActiveSkill::castTailChaserMissile()
{
	m_wFrameCounter	=	0;						//	바로시작

	if (m_lpSkill->m_wShootImage	==	0xffff)	return;

	int	iBodySize	=	m_lpCaster->getBodySize()+random(10)+20;
	int	iDirectCount=	g_apEffect[m_lpSkill->m_wShootImage]->GetDirectCount(0);

	m_posTarget.x	=	m_lpTarget->m_pos.x;
	m_posTarget.y	=	m_lpTarget->m_pos.y;
	m_posBegin.x	=	m_lpCaster->m_pos.x+random(10)-5;
	m_posBegin.y	=	m_lpCaster->m_pos.y+random(10)-5;

	m_pos.x			=	m_posBegin.x+random(30)-15;
	m_pos.y			=	m_posBegin.y+random(30)-15;

	GetTargetPos(&m_pos,m_posTarget.x,m_posTarget.y,iBodySize+random(30)-15);

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wShootImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wShootImage]->GetFPS(0);

	m_wAngle		=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y,1);
	m_wDirect		=	GetDirect(m_wAngle,iDirectCount);

	m_iLastDistance	=	0x7fffffff;

	g_esm.playActionSound(m_lpSkill);

/*
	m_sHeight		=	m_lpCaster->getBodyHeight()*2/3;
	m_sDestHeight	=	m_lpTarget->getBodyHeight()*2/3;			//	목표 높이

	m_wFrameCounter	=	0;						//	바로시작
	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wShootImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wShootImage]->GetFPS(0);

	int	iDirectCount=	g_apEffect[m_lpSkill->m_wShootImage]->GetDirectCount(0);

	m_wAngle		=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y,2);
	m_wDirect		=	GetDirect(m_wAngle,iDirectCount);
*/
}

//
//	머신 미사일형 업데이트
BOOL
CActiveSkill::updateTailChaserMissile()
{
	if (m_wFrameCounter	==	0xffff)
	{
		if (decreaseTailChaserFireTime())	castTailChaserMissile();

		return	FALSE;
	}

	if (m_wFrameCounter	>=	dSYNC_FPS)		//	프레임 갱신
	{
		m_wFrameCounter	-=	dSYNC_FPS;
		m_wFrame++;

		if (m_wFrame>=	m_wMaxFrame)	m_wFrame	=	0;
	}

	m_wFrameCounter	+=	m_wFPS;

	CPos	pos		=	m_pos;

	if (m_lpTarget)
	{
		m_posTarget.x	=	m_lpTarget->m_pos.x;
		m_posTarget.y	=	m_lpTarget->m_pos.y;
	}

	GetTargetPos(&pos,m_posTarget.x,m_posTarget.y,m_lpSkill->m_wSpeed);

	int	iCurrentRange	=	GetOvalRange(pos.x,pos.y,m_posTarget.x,m_posTarget.y);

	if (iCurrentRange	>=	m_iLastDistance)
	{
		if (m_lpCaster	&&	m_lpTarget)
			m_lpCaster->strikeDamage(m_lpTarget,&m_ability,getTailChaserDamage());

		return	TRUE;
	}

	m_iLastDistance	=	iCurrentRange;
	m_pos			=	pos;

	return FALSE;
}

//
//	스킬 이미지 그려주기
void
CActiveSkill::putTailChaserMissile()
{
	if (m_lpSkill->m_wShootImage	==	0xffff)	return;
	if (m_wFrameCounter				==	0xffff)	return;

	int	iX	=	GetScaledXPos(m_pos.x);
	int	iY	=	GetScaledYPos(m_pos.y);

	g_apEffect[m_lpSkill->m_wShootImage]->PutReg(iX,iY-m_sHeight,m_wAnm,m_wDirect,m_wFrame,g_iGameScale,g_iGameScale);
}