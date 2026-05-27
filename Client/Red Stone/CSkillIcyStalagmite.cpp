#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

#define	dICY_STALAGMITE_PEAK_FRAME	10

int
CActiveSkill::fireIcyStalagmite()
{
	m_lpCaster->action(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,m_lpSkill->m_wAction);
	
	setIcyStalagmiteStatus(eISS_RISE);
	m_sHeight	=	0;
	resetReleaseIcyStalagmite();

	return	eSKILL_FIRE_RESULT_OK;
}

BOOL
CActiveSkill::castIcyStalagmite()
{
	if (isReleaseIcyStalagmite())
	{
		m_lpCaster->releaseStalagmite();

		return	FALSE;
	}

	m_wAnm			=	0;
	m_wFrameCounter	=	0;
	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFrameCount(0);
	m_wFPS			=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFPS(0);
	m_sHeight		=	0;

	if (m_lpCaster->isHero())
		g_hero.increaseCP(m_ability.getGetCP());
	setIcyStalagmiteHeight(m_lpSkill->m_aValue[dICY_STALAGMITE_PEAK_FRAME]);

	m_lpCaster->m_wHillSkill	=	m_wSerial;

	g_esm.playActionSound(m_lpSkill);

	return	TRUE;
}

BOOL
CActiveSkill::updateIcyStalagmiteRise()
{
	while(m_wFrameCounter>=	dSYNC_FPS)
	{
		m_wFrameCounter	-=	dSYNC_FPS;
		m_wFrame++;

		if (m_wFrame	>=	m_wMaxFrame)
		{
			m_wAnm			=	1;
			m_wFrameCounter	=	0;
			m_wFrame		=	0;
			m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFrameCount(1);
			m_wFPS			=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFPS(1);

			setIcyStalagmiteStatus(eISS_STAND);

			return	FALSE;
		}
	}

	m_sHeight		=	m_lpSkill->m_aValue[2+m_wFrame];
	m_wFrameCounter	+=	m_wFPS;
	
	return	FALSE;
}

BOOL
CActiveSkill::updateIcyStalagmiteStand()
{
	while(m_wFrameCounter>=	dSYNC_FPS)
	{
		m_wFrameCounter	-=	dSYNC_FPS;
		m_wFrame++;

		if (m_wFrame	>=	m_wMaxFrame)	m_wFrame	=	0;
	}

	m_sHeight		=	m_lpSkill->m_aValue[dICY_STALAGMITE_PEAK_FRAME];
	m_wFrameCounter	+=	m_wFPS;
	
	return	FALSE;
}

BOOL
CActiveSkill::updateIcyStalagmiteSink()
{
	while(m_wFrameCounter>=	dSYNC_FPS)
	{
		m_wFrameCounter	-=	dSYNC_FPS;
		m_wFrame++;

		if (m_wFrame	>=	m_wMaxFrame)
		{
			m_lpCaster->m_wHillSkill	=	0xffff;

			return	TRUE;
		}
	}

	m_sHeight		=	m_lpSkill->m_aValue[dICY_STALAGMITE_PEAK_FRAME+m_wFrame];
	m_wFrameCounter	+=	m_wFPS;

	return	FALSE;
}

BOOL
CActiveSkill::updateIcyStalagmiteDestroy()
{
	while(m_wFrameCounter>=	dSYNC_FPS)
	{
		m_wFrameCounter	-=	dSYNC_FPS;
		m_wFrame++;

		if (m_wFrame	>=	m_wMaxFrame)
		{
			m_lpCaster->m_wHillSkill	=	0xffff;

			return	TRUE;
		}
	}

	if (m_wFrame >= 2)	m_sHeight	=	0;

	m_wFrameCounter	+=	m_wFPS;

	return	FALSE;
}

void
CActiveSkill::sinkIcyStalagmite()
{
	mBeginTLog();
	
	g_eh.addLog("sinkIcyStalagmite 1");

	if	(getIcyStalagmiteStatus()	==	eISS_SINK)
		return;

	g_eh.addLog("si 2");

	if	(!m_lpSkill	||	m_lpSkill->m_wSkillImage==	0xffff)
		return;

	g_eh.addLog("si 3");

	m_wAnm			=	2;
	m_wFrameCounter	=	0;
	m_wFrame		=	0;

	g_eh.addLog("si 4");
	g_eh.addLog("si 5 %d",m_lpSkill->m_wSkillImage);

	m_wMaxFrame		=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFrameCount(2);
	g_eh.addLog("si 6");
	m_wFPS			=	g_apEffect[m_lpSkill->m_wSkillImage]->GetFPS(2);

	g_eh.addLog("si 7");
	setIcyStalagmiteStatus(eISS_SINK);
	g_eh.addLog("si 8");
}

void
CActiveSkill::destroyIcyStalagmite()
{
	if	(!m_lpSkill	||	m_lpSkill->m_wType	!=	eSKILL_TYPE_ICY_STALAGMITE)
		return;

	cANM	*lpEffect	=	g_im.getEffect(m_lpSkill->m_wSkillImage);

	if	(!lpEffect)
		return;

	if	(getIcyStalagmiteStatus()	==	eISS_SINK)
		return;

	m_wAnm			=	2;
	m_wFrameCounter	=	0;
	m_wFrame		=	0;
	m_wMaxFrame		=	lpEffect->GetFrameCount(2);
	m_wFPS			=	lpEffect->GetFPS(2);

	setIcyStalagmiteStatus(eISS_DESTROY);
}

BOOL
CActiveSkill::updateIcyStalagmite()
{
	if (m_wFrameCounter	==	0xffff	)
	{
		if (m_lpCaster->m_isTriggerFrame			)	return	1-castIcyStalagmite();

		return	FALSE;	//	아직 쏘기 직전이다.
	}

	switch(getIcyStalagmiteStatus())
	{
		case	eISS_RISE	:
			return	updateIcyStalagmiteRise();

		case	eISS_STAND	:
			return	updateIcyStalagmiteStand();

		case	eISS_SINK	:
			return	updateIcyStalagmiteSink();

		case	eISS_DESTROY:
			return	updateIcyStalagmiteDestroy();
	}

	return	FALSE;
}

//
//
void
CActiveSkill::putIcyStalagmite(int _iX,int _iY)
{
	mBeginTLog();
	
	g_eh.addLog("putIcyStalagmite 1");
	if	(m_wFrameCounter			==	0xffff)
		return;
	g_eh.addLog("pi 2");
	if	(!m_lpSkill	||	m_lpSkill->m_wSkillImage==	0xffff)
		return;
	g_eh.addLog("pi 3");

	int	x		=	GetScaledXPos(m_pos.x);
	g_eh.addLog("pi 4");
	int	y		=	GetScaledYPos(m_pos.y);

	g_eh.addLog("pi 5");
	g_eh.addLog("pi 5 si %d,an %d,fr %d,scale %d",m_lpSkill->m_wSkillImage,m_wAnm,m_wFrame,g_iGameScale);
	g_apEffect[m_lpSkill->m_wSkillImage]->PutShadow(x,y,m_wAnm,0,m_wFrame,g_iGameScale,g_iGameScale);
	g_eh.addLog("pi 6");
	g_apEffect[m_lpSkill->m_wSkillImage]->PutReg(x,y,m_wAnm,0,m_wFrame,g_iGameScale,g_iGameScale);
	g_eh.addLog("pi 7");
}