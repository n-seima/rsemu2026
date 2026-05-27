#include	"ast_gas.h"
#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cFIELD.H"
#include "CGame.H"

//
//	폭발형 스킬 사용
int
cAS_Gas::fire()
{
	m_lpCaster->action(m_lpSkill->m_wAction);

	m_wFrameCounter	=	0xffff;

	getActionTime();

	m_wStep			=	eGTS_READY;

	m_iInhaleEnemyRange		=	m_lpSkill->m_aValue[1];
	m_wInhalePeroid			=	m_lpSkill->m_aValue[2];
	m_wInhalePixel			=	m_lpSkill->m_aValue[3];
	m_wMinInhaleRange		=	m_lpSkill->m_aValue[4];
	m_iUpdateCounter		=	0;
	
	return	eSKILL_FIRE_RESULT_OK;
}

//
//	폭발형 스킬 사용
BOOL
cAS_Gas::cast()
{
	int	iUpkeepTime	=	m_lpSkill->m_wUpkeepTime+m_lpSkill->m_wUpkeepTimePerLevel*m_ability.m_wLevel;
	
	m_uiOccurActionTime	=	g_dwCurrentTime+m_lpSkill->m_sOccurActionPeriod/2*10;
	
	m_uiUpkeepTime	=	g_dwCurrentTime+iUpkeepTime*10;
	m_wStep			=	eGTS_FIRE;
	m_wFrameCounter	=	0;
	if	(m_lpSkill->m_bf1IsGetCPGasSkillByCasting)
	{
		m_wIsGetdCP	=	TRUE;
		m_lpCaster->increaseCP(m_ability.getGetCP());
	}
	
	return	TRUE;
}

//
//	폭발형 스킬 업데이트
BOOL
cAS_Gas::update()
{
	mDS_AS.m_iActiveSkillInnerStep	=	1;

	m_iUpdateCounter++;

	if	(m_wStep	==	eGTS_READY)
	{
		mDS_AS.m_iActiveSkillInnerStep	=	2;

		if (!isActionTime())
			return	FALSE;	//	아직 쏘기 직전이다.
		
		mDS_AS.m_iActiveSkillInnerStep	=	3;
		cast();
		
		mDS_AS.m_iActiveSkillInnerStep	=	4;
		return	FALSE;
	}
	
	mDS_AS.m_iActiveSkillInnerStep	=	5;

	if	(isFinishSkillTime(g_dwCurrentTime))
		return	TRUE;
	
	mDS_AS.m_iActiveSkillInnerStep	=	6;
	if	(m_lpCaster->m_lpField	==	NULL)
		return	TRUE;

	mDS_AS.m_iActiveSkillInnerStep	=	7;
	if	(m_iInhaleEnemyRange &&  m_wInhalePeroid)
	{
		m_lpCaster->m_wInhaleTime	=	4;
		
		if	(m_iUpdateCounter	%	m_wInhalePeroid	==	0)
		{
			mDS_AS.m_iActiveSkillInnerStep	=	8;
			m_lpCaster->m_lpField->thrust(m_lpCaster,m_iInhaleEnemyRange,-m_wInhalePixel,m_wMinInhaleRange);
			mDS_AS.m_iActiveSkillInnerStep	=	9;
		}
	}
	
	mDS_AS.m_iActiveSkillInnerStep	=	10;

	if	(!isOccurActionTime(g_dwCurrentTime))
		return	FALSE;
	
	mDS_AS.m_iActiveSkillInnerStep	=	11;
	BOOL	bIsGetedCP	=	m_wIsGetdCP;
	
	if	(m_lpCaster)
	{
		mDS_AS.m_iActiveSkillInnerStep	=	12;

		m_lpCaster->m_lpField->strikeExplosionSkill(m_lpCaster,m_lpTarget,m_posTarget.x,m_posTarget.y,&m_ability,&bIsGetedCP);
		
		m_wIsGetdCP	=	bIsGetedCP;
	}
	
	mDS_AS.m_iActiveSkillInnerStep	=	13;

	m_uiOccurActionTime	=	g_dwCurrentTime+m_lpSkill->m_sOccurActionPeriod*10;
	
	return	FALSE;
}