#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	러쉬타입 스킬 사용
int
CActiveSkill::fireBouncingLinear()
{
	m_lpCaster->m_wUseSpecialActionSkill=	eSAS_BOUNCING_LINEAR;

	setBouncingLinearTargetIndex(0);

	m_lpTarget			=	getBouncingLinearCurrentTarget();

	m_lpCaster->action(m_lpTarget->m_pos.x,m_lpTarget->m_pos.y,m_lpSkill->m_wAction);
	m_wFrameCounter		=	0xffff;

	if	(m_lpSkill->m_wSkillImage	!=	0xffff)
	{
		m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFrameCount(0);
		m_wFPS			=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFPS(0);
		m_wFrame		=	0;
	}

	return	eSKILL_FIRE_RESULT_OK;
}

BOOL
CActiveSkill::castBouncingLinear()
{
	if (aimingNextBouncingLinearTarget())
		return	TRUE;

	int	iPPS	=	m_lpCaster->m_iPPS;

	m_wFrameCounter		=	0;
	m_lpCaster->m_iPPS	=	m_lpSkill->m_wSpeed;
	m_lpCaster->moveTo(m_lpTarget->m_pos.x,m_lpTarget->m_pos.y,TRUE,FALSE,TRUE);
	g_esm.playActionSound(m_lpSkill);
	m_lpCaster->m_iPPS	=	iPPS;
	g_hero.m_isWalking	=	FALSE;


	return	FALSE;
}

//
BOOL
CActiveSkill::aimingNextBouncingLinearTarget()
{
	if (getBouncingLinearTargetCount() == getBouncingLinearTargetIndex())
		return	TRUE;

	CWideAreaDamageInfo	*lpInfo	=	(CWideAreaDamageInfo *)m_aValue;

	while(1)
	{
		m_lpTarget	=	getBouncingLinearCurrentTarget();

		if (m_lpTarget)
			break;

		increaseBouncingLinearTargetIndex();

		if (getBouncingLinearTargetCount() == getBouncingLinearTargetIndex())
			return	TRUE;
	}

	int	iRange	=	GetOvalRange(m_pos.x,m_pos.y,m_lpTarget->m_pos.x,m_lpTarget->m_pos.y);

	iRange		=	(int)sqrt(iRange);

	if (iRange	>=	m_ability.getAttackRange(NULL,0)+100)
		return	TRUE;

	return	FALSE;
}

CActor*
CActiveSkill::getBouncingLinearCurrentTarget()
{
	CWideAreaDamageInfo	*lpInfo	=	(CWideAreaDamageInfo *)m_aValue;

	int		iIndex		=	getBouncingLinearTargetIndex();

	return	g_am.getTestedActor(lpInfo->aData[iIndex].wActor);
}

void
CActiveSkill::strikeBouncingLinearTarget()
{
	CWideAreaDamageInfo	*lpInfo	=	(CWideAreaDamageInfo *)m_aValue;

	CActor	*lpTarget	=	getBouncingLinearCurrentTarget();
	int		iIndex		=	getBouncingLinearTargetIndex();

	if (!lpTarget)
		return;

	CHitInfo	hitInfo;

	hitInfo.m_dwPhysicalDamage	=	lpInfo->aData[iIndex].wPhysicalDamage*100;
	hitInfo.m_dwMagicDamage		=	lpInfo->aData[iIndex].wMagicDamage*100;
	hitInfo.m_dwResultField		=	lpInfo->aData[iIndex].dwResultField;

	m_lpCaster->strike(lpTarget,&m_ability,&hitInfo,0,FALSE);
}

BOOL
CActiveSkill::updateBouncingLinear()
{
	if (m_wFrameCounter	==	0xffff)
		if (castBouncingLinear())
			return	TRUE;

	if (m_lpCaster->m_isMove	==	FALSE)
	{
		strikeBouncingLinearTarget();

		increaseBouncingLinearTargetIndex();

		if (castBouncingLinear())
			return	TRUE;
	}

//	프레임 갱신
	{
		while(m_wFrameCounter	>=	dSYNC_FPS)
		{
			m_wFrameCounter	-=	dSYNC_FPS;
			m_wFrame++;

			if (m_wFrame	>=	m_wMaxFrame)
				m_wFrame	=	0;
		}

		m_wFrameCounter	+=	m_wFPS;
	}

	return	FALSE;
}

void
CActiveSkill::putBouncingLinear()
{
	if (m_lpSkill->m_wSkillImage	==	0xffff)
		return;

	cANM	*lpAnm	=	(cANM *)g_apEffect[m_lpSkill->m_wSkillImage];

	int	iX	=	GetScaledXPos(m_lpCaster->m_pos.x);
	int	iY	=	GetScaledYPos(m_lpCaster->m_pos.y);

	lpAnm->PutReg(iX,iY,0,m_lpCaster->m_iDirect,m_wFrame,g_iGameScale,g_iGameScale);
}