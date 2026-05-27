#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cFIELD.H"
#include "CGame.H"
#include "trap.H"
#include	"astDependOnImageTrap.h"

int
cAS_DefendOnImageTrap::cast(cTrap *_lpTrap)
{
	m_wSkillType	=	eSKILL_TYPE_TRAP;
	m_wMaxFrame		=	g_aEffect[m_lpSkill->m_wExplosionImage].getFrameCount(0);
	m_wFPS			=	g_aEffect[m_lpSkill->m_wExplosionImage].getFPS(0);
	m_wFrameCounter	=	0;		//	바로시작
	m_wAnm			=	0;
	m_wFPS			=	m_wFPS*m_lpSkill->m_wCorrectFPS/100;
	m_wFrame		=	0;
	
	memcpy(&m_trap,_lpTrap,sizeof(cTrap));

	return	eSKILL_FIRE_RESULT_OK;
}

BOOL
CActiveSkill::updateDefendOnImageTrap()
{
	cAS_DefendOnImageTrap	*lpAS	=	(cAS_DefendOnImageTrap	*)this;

	return	lpAS->update();
}

BOOL
cAS_DefendOnImageTrap::update()
{
	while(m_wFrameCounter	>=	dSYNC_FPS)
	{
		m_wFrameCounter	-=	dSYNC_FPS;
		m_wFrame++;

		if	(m_wFrame	>=	m_wMaxFrame)
			return	TRUE;

		if	(g_aEffect[m_lpSkill->m_wExplosionImage].isDamage(m_wAnm,m_wFrame))
			m_lpCaster->m_lpField->explosionTrap(m_lpCaster,&m_trap,NULL,eETT_DEFEND_ON_IMAGE);
	}

	m_wFrameCounter	+=	m_wFPS;

	return	FALSE;
}