#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//	바이트 행잉
int
CActiveSkill::fireBiteHanging()
{
	m_lpCaster->action(m_lpTarget->m_pos.x,m_lpTarget->m_pos.y,m_lpSkill->m_wAction);

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	연속 타격 스킬
BOOL
CActiveSkill::updateBiteHanging()
{
	if (m_lpCaster->m_isTriggerFrame)
	{
		CActiveSkill *lpAciveSkill = g_sm.get(m_lpTarget->m_wUseSkill);

		//if(!lpAciveSkill)
		//	return FALSE;

		//if(!(lpAciveSkill->m_wType == eSKILL_TYPE_SCIMITAR_CUTTING))
		GetTargetPos(&m_lpCaster->m_pos,m_lpTarget->m_pos.x,m_lpTarget->m_pos.y,20);

		m_lpCaster->m_wBiteTarget						=	m_lpTarget->m_wSerial;
		m_lpTarget->m_wBiter							=	m_lpCaster->m_wSerial;
		m_lpTarget->m_enchantedImage.m_bf1IsExistBiter	=	TRUE;

		return	TRUE;
	}

	return	FALSE;	//	아직 쏘기 직전이다.
}