#include "CActor.H"

void
CActor::revive()
{
	if (!isDeath())
		return;

	m_wWhiteFadeStep=	1;
	m_wCorpseTime	=	0;
	m_iHP			=	1;
}

//
//	
void
CActor::cure(int _iSkill,int _iHealPoint)
{
	CSkill	*lpSkill	=	&g_aSkill[_iSkill];

	if	(lpSkill->isCureStrangeStatus())
		cureStrangeStatus();
	if	(lpSkill->isCureDeclinePowerStatus())
		cureDeclinePowerStatus();
	if	(lpSkill->isCureCurseStatus())
		cureCurseStatus();
	if	(lpSkill->isCureAllStrangeStatus())
		cureWrongStatus();
	if	(lpSkill->isResurrection())
		revive();

	if	(!isHero() && !isPet() && !isSummonBeast())
	{
		m_iHP	+=	_iHealPoint;

		if (m_iHP	>	m_iMaxHP)
			m_iHP	=	m_iMaxHP;
	}

	makeHealEffect(_iSkill,_iHealPoint);
}

//
//	
void
CActor::absorbDamage(int _iHealPoint)
{
	m_iHP	+=	_iHealPoint;

	makeAbsorbDamageEffect(_iHealPoint);

	if	(m_iHP	>	m_iMaxHP)
		m_iHP	=	m_iMaxHP;
}

void
CActor::cureStrangeStatus()
{
}

void
CActor::cureDeclinePowerStatus()
{
}

void
CActor::cureCurseStatus()
{
}

void
CActor::cureWrongStatus()
{
}