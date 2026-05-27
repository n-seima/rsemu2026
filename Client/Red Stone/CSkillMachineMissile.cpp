#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	미사일형 스킬 사용
int
CActiveSkill::fireMachineMissile()
{
	if	(m_lpSkill->m_wShootImage	==	0xffff)
		return	eSKILL_FIRE_RESULT_FAILED;

	if	(m_lpSkill->isMiniPetSkill()	==	FALSE	&&	m_lpSkill->m_wMachineImage	==	0xffff)
		return	eSKILL_FIRE_RESULT_FAILED;

	m_sHeight		=	m_lpCaster->getBodyHeight()*4/3;
	m_sDestHeight	=	m_lpTarget->getBodyHeight()*2/3;			//	목표 높이

	m_wFrameCounter	=	0;						//	바로시작

	if	(m_lpSkill->isMiniPetSkill())
	{
		m_sHeight		=	m_lpCaster->getBodyHeight()-10;
		castMachineMissile();
	}
	else
	{
		m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wMachineImage]->GetFrameCount(0);
		m_wFPS			=	g_apEffect[m_lpSkill->m_wMachineImage]->GetFPS(0);

		int	iDirectCount=	g_apEffect[m_lpSkill->m_wMachineImage]->GetDirectCount(0);

		m_wAngle		=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y,2);
		m_wDirect		=	GetDirect(m_wAngle,iDirectCount);

		m_lpCaster->m_wMachineAngle	=	m_wAngle;
		setMachineSkillUpdateStatus(0);
	}

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	머쉰 미사일 발사

void
CActiveSkill::castMachineMissile()
{
	m_wFrameCounter	=	0;						//	바로시작

	if	(m_lpSkill->m_wShootImage	==	0xffff)
		return;

	int	iBodySize	=	m_lpCaster->getBodySize()+random(10)+20;
	int	iDirectCount=	g_apEffect[m_lpSkill->m_wShootImage]->GetDirectCount(0);

	m_posTarget.x	=	m_lpTarget->m_pos.x;
	m_posTarget.y	=	m_lpTarget->m_pos.y;
	m_posBegin.x	=	m_lpCaster->m_pos.x;
	m_posBegin.y	=	m_lpCaster->m_pos.y;
	m_posTarget.y	+=	(m_sHeight-m_sDestHeight);

	if	(m_lpSkill->isMiniPetSkill())
	{
		if	(m_aValue[1]	==	0)
		{
			m_pos.x = m_lpCaster->m_posPet.x;
			m_posBegin.x = m_lpCaster->m_posPet.x;
			m_pos.y	=	m_lpCaster->m_posPet.y;
			m_posBegin.y	=	m_lpCaster->m_posPet.y;
		}
		else
		{
			m_pos.x = m_lpCaster->m_posPet2.x;
			m_posBegin.x = m_lpCaster->m_posPet2.x;
			m_pos.y	=	m_lpCaster->m_posPet2.y;
			m_posBegin.y	=	m_lpCaster->m_posPet2.y;
		}

		int	iAngle		=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y);

		GetAnglePos(&m_pos,iAngle,20);
		GetAnglePos(&m_posBegin,iAngle,20);
	}
	else
	{
		int	iAngle		=	cANGLE::GetAngleToTarget(m_posTarget.x,m_posTarget.y,m_pos.x,m_pos.y);

		GetAnglePos(&m_pos,iAngle,60);
	}

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wShootImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wShootImage]->GetFPS(0);
	m_wFrame		=	0;

	m_wAngle		=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y,1);
	m_wDirect		=	GetDirect(m_wAngle,iDirectCount);

	m_iLastDistance	=	0x7fffffff;

	setMachineSkillUpdateStatus(1);

	g_esm.playActionSound(m_lpSkill);
}

//
//	머신 미사일형 업데이트
BOOL
CActiveSkill::updateMachineMissile()
{
	if	(m_wFrameCounter	>=	dSYNC_FPS)		//	프레임 갱신
	{
		m_wFrameCounter	-=	dSYNC_FPS;
		m_wFrame++;

		if	(m_wFrame>=m_wMaxFrame)
			m_wFrame	=	0;

		if	(m_lpCaster && getMachineSkillUpdateStatus()	==	0)
		{
			m_lpCaster->m_wMachineFrame	=	m_wFrame;

			if	(g_apEffect[m_lpSkill->m_wMachineImage]->IsTrigger(0,m_wFrame))
				castMachineMissile();
		}
	}

	m_wFrameCounter	+=	m_wFPS;

	if	(getMachineSkillUpdateStatus()	==	0)
		return	FALSE;

	CPos	pos		=	m_pos;

	if	(m_lpTarget)
	{
		m_posTarget.x	=	m_lpTarget->m_pos.x;
		m_posTarget.y	=	m_lpTarget->m_pos.y;
		m_posTarget.y	+=	(m_sHeight-m_sDestHeight);
	}

	GetTargetPos(&pos,m_posTarget.x,m_posTarget.y,m_lpSkill->m_wSpeed);

	int	iCurrentRange	=	GetOvalRange(pos.x,pos.y,m_posTarget.x,m_posTarget.y);

	if	(iCurrentRange	>=	m_iLastDistance)
	{
		m_lpCaster->m_wMachineFrame	=	0;
		m_lpCaster->m_wMachineAngle	=	0xffff;

		if	(m_lpCaster	&&	m_lpTarget)
			m_lpCaster->strike(m_lpTarget,&m_ability,&m_hitInfo,m_wDirect,FALSE);

		return	TRUE;
	}

	m_iLastDistance	=	iCurrentRange;
	m_pos			=	pos;

	return FALSE;
}

//
//	스킬 이미지 그려주기
void
CActiveSkill::putMachineMissile()
{
	if	(m_lpSkill->m_wShootImage		==	0xffff)
		return;
	if	(getMachineSkillUpdateStatus()	!=	1)
		return;

	int		iScale		=	g_iGameScale*m_lpSkill->m_wImageScale/100;

	cANM	*lpMissile	=	(cANM *)g_apEffect[m_lpSkill->m_wShootImage];

	lpMissile->PutReg(GetScaledXPos(m_pos.x),GetScaledYPos(m_pos.y)-m_sHeight,m_wAnm,m_wDirect,m_wFrame,iScale,iScale);

	switch	(m_lpSkill->m_wAfterImageType)
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

			int	x		=	GetScaledXPos(pos.x);
			int	y		=	GetScaledYPos(pos.y-m_sHeight);

			lpMissile->linerPut(x,y,
								m_wAnm,m_wDirect,m_wFrame,(m_wAngle+180)%360,iRange,
								m_lpSkill->m_wAfterImageGap,m_lpSkill->m_wAfterImageCount,
								m_lpSkill->m_wAfterImageFirstImageAlphaDepth,m_lpSkill->m_sAfterImageDecreaseAlphaDepthValue,iScale);
			break;
		}

		case	eSKILL_AFTER_IMAGE_NEXT_ANIMATION	:
			break;

		case	eSKILL_AFTER_IMAGE_INCREASE_FRAME	:
			break;
	}
}