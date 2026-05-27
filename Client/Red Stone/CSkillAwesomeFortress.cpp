#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

int
CActiveSkill::fireAwesomeFortress()
{
	if	(isTriggerAwesomeFortress())
		m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);
	else
	{
		m_lpCaster->m_wAwesomeFortressAction	=	m_lpSkill->m_wAction2;
		m_lpCaster->m_awesomeFortressActionInfo.init(m_lpCaster,m_lpSkill->m_wAction2,m_lpSkill->m_bf5BunshineAlphaDepth);
	}

	return	eSKILL_FIRE_RESULT_OK;
}

void
CActiveSkill::strikeAwesomeFortressAttack()
{
	CWideAreaDamageInfo	*lpInfo	=	(CWideAreaDamageInfo *)m_aValue;
	int	iHitCount	=	0;

	for (int i=0;i<getAwesomeFortressBunshinCount();i++)
	{
		CHitInfo	hitInfo;
		CActor		*lpTarget	=	g_am.getTestedActor(lpInfo->aData[i].wActor);

		if	(!lpTarget)
			continue;

		hitInfo.m_dwPhysicalDamage	=	lpInfo->aData[i].wPhysicalDamage*100;
		hitInfo.m_dwMagicDamage		=	lpInfo->aData[i].wMagicDamage*100;
		hitInfo.m_dwResultField		=	lpInfo->aData[i].dwResultField;

		iHitCount	+=	hitInfo.isHit();

		if	(i== 0)
			m_lpCaster->strike(lpTarget,&m_ability,&hitInfo,0,FALSE);
		else
			m_lpCaster->strike(lpTarget,&m_ability,&hitInfo,0,FALSE,FALSE);
	}

	if	(iHitCount && m_lpCaster->isHero())	
		g_hero.increaseCP(m_ability.getGetCP());

	g_esm.playActionSound(m_lpSkill);
}

BOOL
CActiveSkill::updateAwesomeFortress()
{
	if	(isTriggerAwesomeFortress())
		return	TRUE;

	if	(m_lpCaster->m_awesomeFortressActionInfo.m_bf1IsTriggerFrame)
	{
		strikeAwesomeFortressAttack();

		return	TRUE;
	}

	return	FALSE;
}

void
CActiveSkill::putAwesomeFortress()
{

}
