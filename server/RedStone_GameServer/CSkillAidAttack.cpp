#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cFIELD.H"
#include "CGame.H"

//
//	보조마법
int
CActiveSkill::fireAidAttack()
{
	if (m_lpCaster)
		m_lpCaster->action(m_lpSkill->m_wAction);

	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	캐스팅
BOOL
CActiveSkill::castAidAttack()
{
	return	m_lpCaster->m_lpField->wideAidAttack(m_lpCaster,m_lpTarget,&m_ability,m_lpTarget->m_pos.x,m_lpTarget->m_pos.y);
}

//
//	보조마법
BOOL
CActiveSkill::updateAidAttack()
{
	if	(!isActionTime())
		return	FALSE;

	if	(!m_lpTarget	||	m_lpTarget->m_wSerialInField	==	0xffff	||	m_lpTarget->isDeath())
		return	TRUE;

	m_lpTarget->operateRevenge(m_lpCaster,&m_ability);	//	스킬에 따라 팀을 바꿀 수도 있으니 여기서 처리한다.

	if	(!m_lpTarget	||	m_lpTarget->m_wSerialInField	==	0xffff	||	m_lpTarget->isDeath())
		return	TRUE;
	if	(!m_lpCaster	||	m_lpCaster->m_wSerialInField	==	0xffff	||	m_lpCaster->isDeath())
		return	TRUE;

	if	(m_lpTarget->isPlayer())
		m_lpTarget->breakdownArmorDurability();

	if	(m_lpCaster->isPlayer())
		m_lpCaster->breakdownWeaponDurability();

	if	(m_wSerial	==	0xffff)
		return	TRUE;
	
	castAidAttack();

	return	TRUE;
}