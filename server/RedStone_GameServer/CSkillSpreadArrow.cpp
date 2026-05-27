#include <math.h>
#include "CActiveSkill.H"
#include "cACTOR.H"
#include "CField.H"

//
//	미사일형 스킬 사용
int
CActiveSkill::fireSpreadArrow()
{
	m_lpCaster->action(m_lpSkill->m_wAction);

	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}	//	CActiveSkill::fireSpreadArrow()

//
//	미사일형 업데이트
BOOL
CActiveSkill::updateSpreadArrow()
{
	if (!isActionTime())
		return	FALSE;

	int	iHitAngleRange	=	m_ability.getHitAngleRange();
	int	iAngle			=	g_protractor.getAngleToTarget(m_pos.x,m_pos.y,m_posTarget.x,m_posTarget.y);

	m_lpCaster->m_lpField->strikeArcShape(m_lpCaster,&m_ability,m_pos.x,m_pos.y,iAngle,iHitAngleRange);

	return	TRUE;
}	//	CActiveSkill::updateSpreadArrow()