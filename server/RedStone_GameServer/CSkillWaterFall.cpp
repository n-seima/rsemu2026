#include <math.h>
#include "CActiveSkill.H"
#include "cACTOR.H"
#include "CField.H"

//
//	워터폴 사용
int
CActiveSkill::fireWaterFall()
{
	m_lpCaster->action(m_lpSkill->m_wAction);

	resetWaterFallUpkeepTime();
	resetWaterFallStrikePeriod();
	resetWaterFallCasting();
	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	워터폴 사용
void
CActiveSkill::castWaterFall()
{
	int	iCastingTime	=	m_lpSkill->m_sCastingTime*dSYNC_FPS/1000;

	setWaterFallCastingTime(iCastingTime);

	setWaterFallUpkeepTime(m_ability.getUpkeepTime()*dSYNC_FPS);
	setWaterFallStrikePeriod(m_ability.getStrikePeriod()*dSYNC_FPS/100);
	setWaterFallCasting();
	resetWaterFallStrikeCounter();
}

//
//	워터폴 업데이트
BOOL
CActiveSkill::updateWaterFall()
{
	if	(!isWaterFallCasting())
	{
		if	(m_lpSkill->m_bf1InsInstanceWaterFall)
		{
			if	(isActionTime())
				castWaterFall();

			return	FALSE;
		}

		if	(!m_lpCaster->m_wIsActionFinished)
			return	FALSE;

		castWaterFall();

		return	FALSE;
	}

	if	(decreaseWaterFallCastingTime())
		return	FALSE;

	if	(decreaseWaterFallUpkeepTime())
		return	TRUE;

	if	(updateWaterFallStrikeCounter())
	{
		BOOL	bIsGetedCP	=	isWaterFallGetedCPStatus();

		m_lpCaster->m_lpField->strikeExplosionSkill(m_lpCaster,m_lpTarget,m_posTarget.x,m_posTarget.y,&m_ability,&bIsGetedCP);

		setWaterFallGetedCPStatus(bIsGetedCP);
	}

	return	FALSE;
}