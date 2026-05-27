#include "CActiveSkill.H"
#include "cACTOR.H"
#include "debugCode.H"

//
//	회오리형 스킬 사용
int
CActiveSkill::fireWhirlwindTypeSkill()
{
	m_lpCaster->action(m_lpSkill->m_wAction);

	m_aValue[0]	=	0;
	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	회오리형 업데이트
BOOL
CActiveSkill::updateWhirlwindTypeSkill()
{
	mDS_AS.m_iActiveSkillInnerStep	=	1;

	if	(isActionTime() == FALSE)
	{
		mDS_AS.m_iActiveSkillInnerStep	=	-1;
		return	FALSE;
	}

	mDS_AS.m_iActiveSkillInnerStep	=	2;

	m_lpCaster->strikeWhirlWind(&m_ability);

	mDS_AS.m_iActiveSkillInnerStep	=	-2;


/*	if	(m_lpCaster	&&	m_lpCaster->isDeath() == FALSE)
	{
		m_aValue[0]++;
		m_dwBookedActionTime	=	m_lpCaster->getDamageFrameTime(m_aValue[0]);

		if	(m_dwBookedActionTime	!=	0xffffffff)
			return	FALSE;
	}
*/
	return	TRUE;
}