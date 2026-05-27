#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

int
CActiveSkill::fireAlteringHitter()
{
	m_lpCaster->m_wJumpStatus			=	eJS_READY;
	m_lpCaster->m_wUseSpecialActionSkill=	eSAS_JUMP;

	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);

	m_wFrameCounter						=	0xffff;

	int	iRange						=	GetOvalRange(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y);		//	°¸®´.. --;;
	m_wRange						=	(int)sqrt((double)iRange);
	m_wRange						=	max(m_wRange,1);

	m_lpCaster->m_wJumpTargetRange	=	m_wRange;
	m_lpCaster->m_wJumpMaxRange		=	m_ability.getAttackRange(NULL,0);

	setBeginAlteringHitterAttack(FALSE);
	setAlteringHitterAttackPeriod(0);
	resetAlteringHitterShotCounter();

	return	eSKILL_FIRE_RESULT_OK;
}

void
CActiveSkill::castAlteringHitter()
{
	g_esm.playActionSound(m_lpSkill);
	m_wFrameCounter	=	0;

	m_lpCaster->jump(m_posTarget.x,m_posTarget.y,m_ability.m_wSkill);

	if (m_lpSkill->m_wSkillImage==	0xffff	)	return;

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFPS(0);
	m_wAnm			=	0;
}

BOOL
CActiveSkill::updateAlteringHitter()
{
	if	(isBegunAlteringHitterAttack())
	{
		if	(decreaseAlteringHitterAttackTime())
		{
			if	(m_lpTarget)
			{
				int			iHitIndex	=	getAlteringHitterShotCounter();
				CHitInfo	*lpHitInfo	=	&m_lpCaster->m_attackInfo.m_aInfo[iHitIndex];

				m_lpCaster->strike(m_lpTarget,&m_ability,lpHitInfo,0,FALSE);
			}
			
			setAlteringHitterAttackPeriod(m_lpSkill->m_bCharacterAfterImageDelayTime);

			if	(increaseAlteringHitterShotCounter())
			{
				return	TRUE;
			}
		}
	}

	if	(m_wFrameCounter	==	0xffff)
	{
		if (m_lpCaster->m_wJumpStatus	==	eJS_WAIT_TRIGGER)
			castAlteringHitter();

		return	FALSE;
	}

	if	(m_lpCaster->m_wJumpStatus	==	eJS_FINISH || m_lpCaster->m_wJumpStatus	==	eJS_NONE)
	{
		if	(m_isGetdCP	==	FALSE	&&	m_lpSkill->m_wIsNotSyncCP != TRUE && m_lpCaster->isHero())
		{
			g_hero.increaseCP(m_ability.getGetCP());
			m_isGetdCP	=	TRUE;
		}

		setBeginAlteringHitterAttack(TRUE);

		return	FALSE;
	}

	return	FALSE;
}

void
CActiveSkill::putAlteringHitter()
{
/*	if (m_lpSkill->m_wSkillImage==	0xffff) return;
	if (m_wFrameCounter			==	0xffff) return;

	CPos	pos;

	m_lpCaster->getReleasePos(&pos);

	cANM	*lpEffect	=	g_im.getEffect(m_lpSkill->m_wSkillImage);

	lpEffect->PutReg(pos.x-g_map.m_pos.x,pos.y-g_map.m_pos.y,m_wAnm,m_lpCaster->m_iDirect,m_wFrame);
*/
}