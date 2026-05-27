#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cFIELD.H"
#include "CGame.H"

//
//	보조마법
int
CActiveSkill::fireAidMagic()
{
	m_lpCaster->action(m_lpSkill->m_wAction);

	if	(m_lpSkill->isCastOnCasterSkill()	||	m_lpSkill->isCastOnGroundSkill())
		m_lpTarget	=	m_lpCaster;

	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	캐스팅
BOOL
CActiveSkill::castAidMagic()
{
	if	(m_lpTarget->m_lpField	==	NULL)
		return	FALSE;

	return	m_lpTarget->applySkillExtraEffects(m_lpCaster,&m_ability,0,m_posTarget.x,m_posTarget.y);
}

//
//	보조마법
BOOL
CActiveSkill::updateAidMagic()
{
	if	(!isActionTime())
		return	FALSE;

	if	(m_lpTarget)
	{
		if	(!castAidMagic())
			m_lpTarget->addPacketMiss(m_ability.m_wSkill);
		else
		{
			if (m_lpCaster	==	NULL	||	m_lpCaster->m_wSerialInField	==	0xffff)
				return	TRUE;

			m_lpCaster->increaseCP(m_ability.getGetCP());
			m_lpCaster->sendPlayerBasicInformation();
		}

		return	TRUE;
	}

	return	TRUE;
}