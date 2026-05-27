#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	머신타입 스킬 사용
int
CActiveSkill::fireChargeTypeSkill()
{
	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);
	m_lpCaster->setChargeEffect(m_lpSkill->m_wSkillImage);
	m_lpCaster->m_wIsFreezeAtTriggerFrame	=	TRUE;

	return	eSKILL_FIRE_RESULT_OK;
}