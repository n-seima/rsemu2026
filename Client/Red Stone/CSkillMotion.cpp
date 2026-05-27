#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	머신타입 스킬 사용
int
CActiveSkill::fireMotionSkill()
{
	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);

	return	eSKILL_FIRE_RESULT_OK;
}