#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	보조 마법 업데이트
int
CActiveSkill::updateAidAttackSkill()
{
	if	(m_lpCaster->m_isTriggerFrame)
	{
		g_esm.playActionSound(m_lpSkill);

		return	TRUE;
	}

	return	FALSE;
}