#include <math.h>
#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cFIELD.H"

//
//	미사일형 스킬 사용
int
CActiveSkill::fireExplosionMissile()
{
	if	(m_lpTarget	==	NULL)
		return	eSKILL_FIRE_RESULT_FAILED;

	m_lpCaster->action(m_lpSkill->m_wAction);

	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	캐스팅
void
CActiveSkill::castExplosionMissile()
{
	BOOL	bIsGetedCP	=	FALSE;

	m_lpCaster->m_lpField->strikeExplosionSkill(m_lpCaster,m_lpTarget,m_lpTarget->m_pos.x,m_lpTarget->m_pos.y,&m_ability,&bIsGetedCP);
	CSkill* lpSkill = m_ability.getSkill();

	if(lpSkill)
	{
		if(lpSkill->m_aValue[10] != 0 || lpSkill->m_aValue[11] !=0)
		{	// 모으기 범위가 있으면
			int iThrustCenterRange = lpSkill->m_aValue[10] + lpSkill->m_aValue[11] * m_ability.m_wLevel;
			if(lpSkill->m_aValue[12] != 0)
				iThrustCenterRange = min(iThrustCenterRange, lpSkill->m_aValue[12]);
			m_lpCaster->m_lpField->thrustCenter(m_lpCaster, m_lpTarget->m_pos.x, m_lpTarget->m_pos.y,iThrustCenterRange);
		}

	}
	m_wFrameCounter	=	0;						//	바로시작
}

//
//	미사일형 업데이트
BOOL
CActiveSkill::updateExplosionMissile()
{
	if	(!m_lpTarget)
		return	TRUE;	//	음냐.. -_-a
	if	(m_lpTarget->isDeath())
		return	TRUE;	//	음냐.. -_-a
	if	(m_lpTarget->m_wSerialInField	==	0xffff)
		return	TRUE;	//	음냐.. -_-a

	if	(m_wFrameCounter	==	0xffff	)
	{
		if	(isActionTime())
			castExplosionMissile();

		return	FALSE;	//	아직 쏘기 직전이다.
	}
	
	return	TRUE;
}