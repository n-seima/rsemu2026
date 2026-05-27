#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	차지스킬 사용
int
CActiveSkill::fireMirrorTower()
{
	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	차지 스킬 업데이트
BOOL
CActiveSkill::updateMirrorTower()
{
	if (m_lpCaster->m_isTriggerFrame)
	{
		if (m_lpTarget)	
		{
			if (m_lpCaster->isHero())	g_hero.increaseCP(m_ability.getGetCP());

			g_esm.playActionSound(m_lpSkill);

			if (m_lpSkill->m_wAidSkillCastingImage	!=	0xffff)
				m_lpTarget->addHitEffect(0,0,m_lpSkill->m_wAidSkillCastingImage);
		}

		return	TRUE;
	}

	return	FALSE;
}