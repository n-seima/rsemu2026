#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	러쉬타입 스킬 사용
int
CActiveSkill::fireRushTypeSkill()
{


	if(m_lpSkill && m_lpSkill->m_aValue[0])
	{
		m_lpCaster->m_wRushAcceleration			=	m_lpSkill->m_aValue[6];
		m_lpCaster->m_bIsJumpRush				=	m_lpSkill->m_aValue[0];
		m_lpCaster->m_wIncreaseHeightJumpRush		=	m_lpSkill->m_aValue[1];
		m_lpCaster->m_wRushStatus	=	eRS_JUMP_RUSH_READY;
		m_lpCaster->m_wJumpRushLastActionFrame	=	m_lpSkill->m_aValue[2];
	}
	else
	{
		m_lpCaster->m_wRushStatus			=	eRS_READY;
	}

	m_lpCaster->m_wUseSpecialActionSkill=	eSAS_RUSH;

	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);

	memcpy(&m_lpCaster->m_abilityRush,&m_ability,sizeof(m_ability));

	m_wFrameCounter						=	0xffff;

	if (m_lpSkill->m_bf1IsWhirlRunningStyleRush)
	{
		castRushTypeSkill();

		m_lpCaster->m_wRushStatus		=	eRS_READY_FOR_WHIRL_RUNNING_TYPE_RUSH;
	}

	return	eSKILL_FIRE_RESULT_OK;
}

void
CActiveSkill::castRushTypeSkill()
{
	g_esm.playActionSound(m_lpSkill);
	m_wFrameCounter	=	0;

	if (m_lpSkill->m_wSkillImage==	0xffff	)	return;

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFPS(0);
	m_wAnm			=	0;
}

BOOL
CActiveSkill::updateRushTypeSkill()
{
	if	(m_wFrameCounter	==	0xffff)
	{
		if	(m_lpCaster->m_wRushStatus	==	eRS_RUSH)
			castRushTypeSkill();

		return	FALSE;	
	}

	if	(m_lpCaster->m_wRushStatus	==	eRS_FINISH || m_lpCaster->m_wRushStatus	==	eRS_NONE)
		return	TRUE;

//	프레임 갱신
	{
		while(m_wFrameCounter	>=	dSYNC_FPS)
		{
			m_wFrameCounter	-=	dSYNC_FPS;
			m_wFrame++;

			if (m_wFrame	>=	m_wMaxFrame)	m_wFrame	=	0;
		}

		m_wFrameCounter	+=	m_wFPS;
	}

	return	FALSE;
}

void
CActiveSkill::putRushTypeSkill()
{
	int	iX,iY;

	if	(m_lpCaster->m_wRushStatus	==	eRS_RUSH || m_lpCaster->m_wRushStatus	==	eRS_WHIRL_RUNNING_TYPE_RUSH)
	{
		int	iShotCount	=	m_ability.getShotCount();
		int	iIndex		=	0;

		while(iShotCount)
		{
			int	iAngle;

			if (iIndex % 2)
				iAngle	=	(360 + m_lpCaster->m_iAngle - m_lpSkill->m_bf9IsParallellRushAngle)%360;
			else
				iAngle	=	(360 + m_lpCaster->m_iAngle + m_lpSkill->m_bf9IsParallellRushAngle)%360;

			CPos	pos;

			pos.x	=	m_lpCaster->m_pos.x;
			pos.y	=	m_lpCaster->m_pos.y;

			int	iRange	=	m_lpSkill->m_bf8ParallellRushGab;

			iRange	*=	(iIndex/2+1);

			GetAnglePos(&pos,iAngle,iRange);

			pos.x	-=	g_map.m_pos.x;
			pos.y	-=	g_map.m_pos.y;

			m_lpCaster->putBody(pos.x,pos.y);

			iShotCount--;
			iIndex++;

			{
				if	(m_lpSkill->m_wSkillImage==	0xffff)
					continue;
				if	(m_wFrameCounter			==	0xffff)
					continue;

				CPos	pos;

				m_lpCaster->getReleasePos(&pos);
				GetAnglePos(&pos,iAngle,iRange);

				cANM	*lpEffect	=	g_im.getEffect(m_lpSkill->m_wSkillImage);

				iX		=	GetScaledXPos(pos.x);
				iY		=	GetScaledYPos(pos.y);

				lpEffect->PutReg(iX,iY,m_wAnm,m_lpCaster->m_iDirect,m_wFrame,g_iGameScale,g_iGameScale);
			}
		}

		if	(iShotCount	==	0)
		{
			if	(m_lpSkill->m_wSkillImage==	0xffff)
				return;
			if	(m_wFrameCounter			==	0xffff)
				return;

			CPos	pos;

			m_lpCaster->getReleasePos(&pos);

			cANM	*lpEffect	=	g_im.getEffect(m_lpSkill->m_wSkillImage);

			iX		=	GetScaledXPos(pos.x);
			iY		=	GetScaledYPos(pos.y);

			lpEffect->PutReg(iX,iY,m_wAnm,m_lpCaster->m_iDirect,m_wFrame,g_iGameScale,g_iGameScale);
		}
	}
}