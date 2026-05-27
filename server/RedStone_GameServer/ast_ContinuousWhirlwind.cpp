#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cFIELD.H"
#include "CGame.H"
#include "ast_ContinuousWhirlwind.h"


int
cAS_ContinuousWhirlwind::fire()
{
	m_lpCaster->action(m_lpSkill->m_wAction);

	m_wFrameCounter	=	0xffff;

	getActionTime();

	m_wStep			=	0;

	m_iInhaleEnemyRange		=	m_lpSkill->m_aValue[1];
	m_wInhalePeroid			=	m_lpSkill->m_aValue[2];
	m_wInhalePixel			=	m_lpSkill->m_aValue[3];
	m_wMinInhaleRange		=	m_lpSkill->m_aValue[4];
	m_wInhaleRangePerLevel	=	m_lpSkill->m_aValue[5];
	m_iUpdateCounter		=	0;
	
	return	eSKILL_FIRE_RESULT_OK;
}
BOOL
cAS_ContinuousWhirlwind::cast()
{

	int	iUpkeepTime	=	m_lpSkill->m_wUpkeepTime+m_lpSkill->m_wUpkeepTimePerLevel*m_ability.m_wLevel;
	
	m_uiOccurActionTime	=	g_dwCurrentTime+m_lpSkill->m_sOccurActionPeriod/2*10;
	
	m_uiUpkeepTime	=	g_dwCurrentTime+iUpkeepTime*10;
	m_wStep			=	1;
	
	if	(m_lpSkill->m_bf1IsGetCPGasSkillByCasting)
	{
		m_wIsGetdCP	=	TRUE;
		m_lpCaster->increaseCP(m_ability.getGetCP());
	}
	

	return	TRUE;
}

BOOL
cAS_ContinuousWhirlwind::update()
{
	m_iUpdateCounter++;

	if	(m_wStep	==	0)
	{
		if (!isActionTime())
			return	FALSE;	//
		
		cast();
		
		return	FALSE;
	}
	
	if	(isFinishSkillTime(g_dwCurrentTime))
		return	TRUE;
	
	if	(m_lpCaster->m_lpField	==	NULL)
		return	TRUE;
	
	if	(m_iInhaleEnemyRange &&  m_wInhalePeroid)
	{
		
		if	(m_iUpdateCounter	%	m_wInhalePeroid	==	0)
		{
			int iRange = m_iInhaleEnemyRange + (m_wInhaleRangePerLevel * m_ability.m_wLevel)/100;
			m_lpCaster->m_lpField->thrust(m_lpCaster,iRange,-m_wInhalePixel,m_wMinInhaleRange, TRUE);
			
		}
		
	}
	
	if	(!isOccurActionTime(g_dwCurrentTime))
		return	FALSE;
	

	if	(m_lpCaster)
	{
		BOOL bIsGetCP = m_wIsGetdCP;
		m_lpCaster->m_lpField->strikeWhirlWind(m_lpCaster,&m_ability,!m_wIsGetdCP, &bIsGetCP);
		m_wIsGetdCP	= bIsGetCP;
	}
	
	
	m_uiOccurActionTime	=	g_dwCurrentTime+m_lpSkill->m_sOccurActionPeriod*10;
	
	return	FALSE;
}

void
cAS_ContinuousWhirlwind::release()
{

}

