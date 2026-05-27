#include "CActiveSkill.H"
#include "cACTOR.H"

//
//	피어싱 미사일형 스킬 사용
int
CActiveSkill::firePiercingMissileTypeSkill()
{
	m_lpCaster->action(m_lpSkill->m_wAction);

	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}	//	CActiveSkill::firePiercingMissileTypeSkill()

//
//	피어싱 미사일형 스킬 사용
BOOL
CActiveSkill::updatePiercingMissileTypeSkill()
{
	if (!isActionTime())
		return	FALSE;

	m_lpCaster->strikePiercingMissile(&m_ability,m_lpTarget);

	return	TRUE;
}