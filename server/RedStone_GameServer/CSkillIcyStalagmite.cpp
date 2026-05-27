#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cFIELD.H"

//
//	아이시 스탈라그마이트 발사
int
CActiveSkill::fireIcyStalagmite()
{
	m_lpCaster->action(m_lpSkill->m_wAction);

	if (m_lpCaster->m_wHillSkill != 0xffff)	m_isNotSpentCP	=	TRUE;

	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}	//	CActiveSkill::fireIcyStalagmite()

//
//	아이시 스탈라그마이트 사용
BOOL
CActiveSkill::castIcyStalagmite()
{
	if (m_isNotSpentCP)	//	이미 힐 스킬이 시전된 상태 였다.
	{
		m_lpCaster->releaseIcyStalagmite(eRUIS_RELEASE);

		return FALSE;
	}

	m_lpCaster->increaseCP(m_ability.getGetCP());

	CSkill	*lpSkill	=	m_ability.getSkill();

	int		iUpkeepTime	=	m_ability.getUpkeepTime()*dSYNC_FPS;
	int		iHP			=	m_ability.getIcyStalagmiteHP();

	m_lpCaster->m_iHillHP				=	iHP;
	m_lpCaster->m_wHillSkill			=	m_wSerial;
	m_lpCaster->m_wHillDefensivePower	=	(m_lpSkill->m_aValue[15]+m_lpSkill->m_aValue[16]*m_ability.m_wLevel)/100;
	m_lpCaster->m_wHillDodgeCorrect		=	(m_lpSkill->m_aValue[17]+m_lpSkill->m_aValue[18]*m_ability.m_wLevel)/100;

	m_wFrameCounter						=	0;

//	iUpkeepTime			=	dSYNC_FPS*4;
	setIcyStalagmiteTime(iUpkeepTime);

	return	TRUE;
}

//
//	아이시 스탈라그마이트 업뎃
BOOL
CActiveSkill::updateIcyStalagmite()
{
	if (m_wFrameCounter	== 0xffff)
	{
		if (isActionTime())
			return	1-castIcyStalagmite();

		return	FALSE;	//	아직 발동 직전이다.
	}

	if (decreaseIcyStalagmiteTime())
	{
		m_lpCaster->castIcyStalagmite(&m_ability,eRUIS_RELEASE);

		return	TRUE;	//	위에서 스킬 자체를 제거해 준다. (확인해봐!!)
	}

	return	FALSE;
}	//	CActiveSkill::updateIcyStalagmite()