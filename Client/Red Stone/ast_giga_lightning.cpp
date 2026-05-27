#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

BOOL
cAS_GigaLightning::fire()
{
	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);

	m_wGrade				=	eStep_ReadyMakeCrossTarget;
	m_wSkillTime			=	0;

	return	eSKILL_FIRE_RESULT_OK;
}

void
cAS_GigaLightning::putBottom()
{
	int	iImage	=	0xffff;

	if	(m_wFrameCounter			==	0xffff		)
		return;
	if	(m_wFrame					>=	m_wMaxFrame	)
		return;

	int	iScale	=	m_lpSkill->m_wImageScale;

	iScale		=	iScale*g_iGameScale/100;

	int	iX		=	GetScaledXPos(m_posTarget.x);
	int	iY		=	GetScaledYPos(m_posTarget.y);

	if	(m_wGrade	==	eStep_LockOn)
	{
		if	(m_lpSkill->m_wSkillImage	==	0xffff		)
			return;

		g_im.put(m_lpSkill->m_wSkillImage,iX,iY,m_wAnm,m_wDirect,m_wFrame,iScale,iScale);
	}

	if	(m_wGrade	==	eStep_SerchRandomLocation)
	{
		if	(m_lpSkill->m_wSkillImage	==	0xffff		)
			return;

		g_im.put(m_lpSkill->m_wSkillImage,iX,iY,m_wAnm,m_wDirect,m_wFrame,iScale,iScale);
	}
}

void
cAS_GigaLightning::put()
{
	int	iImage	=	0xffff;

	if	(m_wFrameCounter			==	0xffff		)
		return;
	if	(m_wFrame					>=	m_wMaxFrame	)
		return;

	int	iScale	=	m_lpSkill->m_wImageScale;

	iScale		=	iScale*g_iGameScale/100;

	int	iX		=	GetScaledXPos(m_posTarget.x);
	int	iY		=	GetScaledYPos(m_posTarget.y);

	if	(m_wGrade	==	eStep_Action)
	{
		if	(m_lpSkill->m_wExplosionImage==	0xffff		)
			return;

		g_im.putWhichUsePalette(m_lpSkill->m_wExplosionImage,iX,iY,m_lpSkill->m_wPaletteIndex,m_wAnm,m_wDirect,m_wFrame,iScale,iScale,m_lpSkill->m_wOutputEffect);
	}
}

BOOL
cAS_GigaLightning::cast()
{
	g_esm.playActionSound(m_lpSkill);

	if	(m_lpSkill->m_wExplosionImage==	0xffff	)
		return FALSE;

	int	iDirectCount;

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wExplosionImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wExplosionImage]->GetFPS(0);
	m_wFrameCounter	=	0;		//	바로시작
	m_wAnm			=	0;

	m_wFPS			=	m_wFPS*m_lpSkill->m_wCorrectFPS/100;

	m_wDirect		=	m_lpCaster->m_iDirect;
	iDirectCount	=	g_apEffect[m_lpSkill->m_wExplosionImage]->GetDirectCount(0);

	m_wDirect		=	min(iDirectCount-1,m_wDirect);

	return	TRUE;
}

BOOL
cAS_GigaLightning::update()
{
	if	(m_wSkillTime	>=	dSYNC_FPS*10)
		return	TRUE;

	m_wSkillTime++;

	if	(m_wGrade	==	eStep_LockOn	||	m_wGrade	==	eStep_SerchRandomLocation	||	m_wGrade	==	eStep_Action)
	{
		while (m_wFrameCounter	>=	dSYNC_FPS)
		{
			m_wFrameCounter	-=	dSYNC_FPS;
			m_wFrame++;

			if	(m_wFrame	>=	m_wMaxFrame)
			{
				if	(m_wGrade	==	eStep_Action)
					return	TRUE;

				m_wFrame	=	0;
			}
		}

		m_wFrameCounter	+=	m_wFPS;
	}

	if	(m_wGrade	==	eStep_LockOn)
		return	FALSE;
	if	(m_wGrade	==	eStep_Action)
		return	FALSE;
	if	(m_wGrade	==	eStep_Finished)
		return	TRUE;

	if	(m_wGrade	==	eStep_SerchRandomLocation)
	{
		m_spLine.update();
		m_posTarget.x	=	m_spLine.m_pos.x;
		m_posTarget.y	=	m_spLine.m_pos.y;

		m_wUpdeteSPLineCounter++;

		if	(m_wUpdeteSPLineCounter	>=	m_wMatchStep)
			lockOn();

		return	FALSE;
	}

	if	(m_wGrade	==	eStep_Fire)
	{
		m_wGrade	=	eStep_Action;

		if	(!cast())
			return	TRUE;

		return	FALSE;
	}

	return	FALSE;
}

void
cAS_GigaLightning::serchRandomPlace(int _iPattern,int _iMatchStep,int _iRange)
{
	m_wGrade				=	eStep_SerchRandomLocation;
	m_wMatchStep			=	_iMatchStep;
	m_wUpdeteSPLineCounter	=	0;
	int	iRange				=	m_lpSkill->m_aValue[6];

	m_spLine.setRegularRandomPattern(_iPattern);
	m_spLine.init(m_lpCaster->m_pos.x-iRange,m_lpCaster->m_pos.y-iRange,m_lpCaster->m_pos.x+iRange,m_lpCaster->m_pos.y+iRange,0.02f);

	m_posTarget.x			=	m_spLine.m_pos.x;
	m_posTarget.y			=	m_spLine.m_pos.y;

	if	(m_lpSkill->m_wSkillImage	!=	0xffff)
	{
		m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFrameCount(0);
		m_wFPS			=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFPS(0);
		m_wFrameCounter	=	0;		//	바로시작
		m_wAnm			=	0;

		m_wFPS			=	m_wFPS*m_lpSkill->m_wCorrectFPS/100;

		m_wDirect		=	m_lpCaster->m_iDirect;
		int	iDirectCount=	g_apEffect[m_lpSkill->m_wSkillImage]->GetDirectCount(0);

		m_wDirect		=	min(iDirectCount-1,m_wDirect);
	}
}

void
cAS_GigaLightning::fireSkill()
{
	m_wGrade	=	eStep_Fire;

	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_aValue[4]);
}

void
cAS_GigaLightning::lockOn()
{
	m_wGrade	=	eStep_LockOn;

	if	(m_lpSkill->m_wSkillImage	!=	0xffff)
	{
		m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFrameCount(1);
		m_wFPS			=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFPS(1);
		m_wFrameCounter	=	0;		//	바로시작
		m_wAnm			=	1;

		m_wFPS			=	m_wFPS*m_lpSkill->m_wCorrectFPS/100;

		m_wDirect		=	m_lpCaster->m_iDirect;
		int	iDirectCount=	g_apEffect[m_lpSkill->m_wSkillImage]->GetDirectCount(1);

		m_wDirect		=	min(iDirectCount-1,m_wDirect);
	}
}

BOOL
cAS_GigaLightning::casterActionFinished()
{
	if	(m_wGrade		==	eStep_ReadyMakeCrossTarget	||	m_wGrade		==	eStep_SerchRandomLocation	||	m_wGrade	==	eStep_LockOn)
	{
		m_lpCaster->m_iFrame	=	0;

		m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction2);

		return	TRUE;
	}

	if	(m_wGrade	==	eStep_Action)
	{
		m_wGrade	=	eStep_Finished;

		return	FALSE;
	}

	return	FALSE;
}