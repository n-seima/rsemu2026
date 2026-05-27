#include <math.h>
#include "CActiveSkill.H"
#include "cACTOR.H"
#include "CField.H"
#include "CGame.H"

//	¹Æ® ×
int
CActiveSkill::fireBiteHanging()
{
	if	(	m_lpCaster->m_wBiteTarget	!=	0xffff	||	
			m_lpTarget->m_wBiter		!=	0xffff	||
			m_lpTarget->m_wBiteTarget	!=	0xffff	||	
			m_lpCaster->m_wBiter		!=	0xffff)
		return	eSKILL_FIRE_RESULT_NOT_APT_SKILL_TO_THIS_STATUS;

	m_lpCaster->action(m_lpSkill->m_wAction);

	resetBiteHangingCasting();

	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}

//	¹Æ® ×
BOOL
CActiveSkill::castBiteHanging()
{
	mBeginTLog();

	g_eh.addLog("cB 1");

	if	(	!m_lpCaster	||	!m_lpTarget	||	m_lpCaster->m_wSerialInField	==	0xffff	||	m_lpTarget->m_wSerialInField	==	0xffff	||
			m_lpCaster->m_wBiteTarget	!=	0xffff	||	
			m_lpTarget->m_wBiter		!=	0xffff	||
			m_lpTarget->m_wBiteTarget	!=	0xffff	||	
			m_lpCaster->m_wBiter		!=	0xffff)
		return	FALSE;

	g_eh.addLog("cB 2");
	int	iUpkeepTime				=	m_lpSkill->m_wUpkeepTime+m_lpSkill->m_wUpkeepTimePerLevel*m_ability.m_wLevel;

	g_eh.addLog("cB 3");
	m_lpTarget->stop();

	g_eh.addLog("cB 4");

	if	(!m_lpCaster	||	!m_lpTarget	||	m_lpCaster->m_wSerialInField	==	0xffff	||	m_lpTarget->m_wSerialInField	==	0xffff)
		return	FALSE;

	m_lpCaster->m_wBiteTarget	=	m_lpTarget->m_wSerialInField;
	g_eh.addLog("cB 5");
	m_lpTarget->m_wBiter		=	m_lpCaster->m_wSerialInField;

	g_eh.addLog("cB 6");
	setBiteHangingActionTime(0);
	g_eh.addLog("cB 7");
	setBiteHangingUpkeepTime(g_dwCurrentTime+iUpkeepTime*10);
	g_eh.addLog("cB 8");
	setBiteHangingCasting();

	g_eh.addLog("cB 9");
	int	iCheckHPRate			=	max(m_lpCaster->getRemainHPRate()-m_lpSkill->m_aValue[0]/100,1);

	g_eh.addLog("cB 10");
	if	(m_lpSkill->m_aValue[0]	==	0)
		iCheckHPRate			=	0;

	g_eh.addLog("cB 11");
	setReleaseBiteHangingCasterHPRate(iCheckHPRate);

	g_eh.addLog("cB 12");
	return	TRUE;
}

//	¹Æ® ×
BOOL
CActiveSkill::updateBiteHanging()
{
	if	(!isBiteHangingCasting())
	{
		if (isActionTime())
		{
			if	(!castBiteHanging())
				return	TRUE;
		}

		return	FALSE;
	}

	if	(isBiteHangingFinishTime(g_dwCurrentTime))
		return	TRUE;
	if	(isReleaseBiteHangingCasterHPRate(m_lpCaster->getRemainHPRate()))
		return	TRUE;

	if	(isBiteHangingActionTime(g_dwCurrentTime))
	{
		if	(!m_lpTarget	||	!m_lpCaster	||	m_lpTarget->m_wBiter==	0xffff	||	m_lpCaster->m_wBiteTarget==	0xffff)
			return	TRUE;

		setBiteHangingActionTime(g_dwCurrentTime+m_ability.getStrikePeriod()*10);

		m_lpTarget->getDamage(m_lpCaster,&m_ability,0,0,&m_hitInfo);
		m_lpCaster->strike(m_lpTarget,&m_hitInfo,&m_ability);
		
		if	(m_lpTarget == NULL || m_lpTarget->isDeath() || m_lpTarget->m_wSerialInField == 0xffff)
			return	TRUE;

		m_lpTarget->addSendHitDamage(m_lpCaster,m_hitInfo.m_dwPhysicalDamage,m_hitInfo.m_dwMagicDamage,m_hitInfo.m_dwResultField,m_ability.m_wSkill);

		if (m_hitInfo.isHit() && m_wIsGetdCP == FALSE)
		{
			m_wIsGetdCP	=	TRUE;
			m_lpCaster->increaseCP(m_ability.getGetCP());
			m_lpCaster->sendPlayerBasicInformation();
		}
	}

	return	FALSE;
}