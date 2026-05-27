#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	보조 마법 사용
int
CActiveSkill::fireAidSkill()
{
	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	보조 마법 업데이트
int
CActiveSkill::updateAidSkill()
{
	if (m_lpCaster->m_isTriggerFrame)
	{
		g_esm.playActionSound(m_lpSkill);

		if (m_lpTarget)	
		{
			if	(m_lpSkill->m_wAidSkillCastingImage	!=	0xffff)
				m_lpTarget->addEffect(m_lpSkill->m_wAidSkillCastingImageOutputPart,m_lpSkill->m_wAidSkillCastingImage);
		}

		return	TRUE;
	}

	return	FALSE;
}