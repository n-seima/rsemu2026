#include "CActiveSkill.H"
#include "CActor.H"
#include "CField.H"

//
//	머신타입 스킬 사용
int
CActiveSkill::fireTeleport()
{
	if	(m_lpCaster->isSealdMove())
		return	eSKILL_FIRE_RESULT_FAILED;
	
	m_lpCaster->action(m_lpSkill->m_wAction);

	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}

//
//
BOOL
CActiveSkill::castTeleport()
{
	m_lpCaster->increaseCP(m_ability.getGetCP());

	if	(!m_lpCaster->setActorPos(m_posTarget.x,m_posTarget.y,TRUE,TRUE))
		return	FALSE;

	m_lpCaster->releaseIcyStalagmite(eRUIS_RELEASE);

	if	(!m_lpCaster)
		return	TRUE;
	
	m_lpCaster->sendTeleport(m_ability.m_wSkill,eTM_TELEPORT);

	return	TRUE;
}

BOOL
CActiveSkill::updateTeleport()
{
	if(!m_lpCaster)
		return FALSE;

	m_lpCaster->setRemainActionTime(4);	
	
	if	(isActionTime())
	{
		castTeleport();
		m_lpCaster->setRemainActionTime(1);	

		return	TRUE;
	}

	return	FALSE;
}