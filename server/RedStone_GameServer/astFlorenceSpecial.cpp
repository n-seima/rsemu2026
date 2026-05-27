#include "CActiveSkill.H"
#include "cACTOR.H"
#include "CField.H"
#include "CGame.H"
#include	"astFlorenceSpecial.h"

//
//	회오리형 스킬 사용
int
cAS_FlorenceSpecial::fire()
{
	m_lpCaster->action(m_lpSkill->m_wAction);

	m_wShotCount	=	m_ability.getContinuousShotCount();
	m_wCurrentShot	=	0;

	setCastingTimeByCasterDamageFrame(0);
	
	return	eSKILL_FIRE_RESULT_OK;
}

//
//	
BOOL
cAS_FlorenceSpecial::strike()
{
	if	(m_lpCaster	==	NULL)
		return	TRUE;

	BOOL	bIsGetedCP	=	m_wIsGetdCP;

	m_lpCaster->m_lpField->addSendOccurEffectBySkill(m_pos.x,m_pos.y,m_lpSkill->m_wSerial,eSE_EXPLOSION);
	m_lpCaster->m_lpField->strikeExplosionSkill(m_lpCaster,NULL,m_pos.x,m_pos.y,&m_ability,&bIsGetedCP,dMAX_TARGET_FOR_WIDE_AREA_ATTACK,NULL,m_wCurrentShot);

	m_wIsGetdCP			=	bIsGetedCP;

	return	TRUE;
}

//
//	회오리형 업데이트
BOOL
cAS_FlorenceSpecial::update()
{
	if	(m_lpCaster	==	NULL)
		return	TRUE;

	if	(!isActionTime())
		return	FALSE;	//	아직 발동 직전이다.

	if	(m_wCurrentShot	<	m_wShotCount)
		strike();

	if	(m_wCurrentShot	>=	m_wShotCount)
	{
		int	iTR			=	m_lpCaster->isAbleToTransformation(TRUE);

		if	(iTR	==	eTRANSFORMATION_SUCCESS	&&	m_ability.isJobChange())
			m_lpCaster->sendTransformation(eTR_BY_SKILL,m_lpSkill->m_bf1PutTrasAfterAnm);

		return	TRUE;
	}

	m_wCurrentShot++;
	setCastingTimeByCasterDamageFrame(m_wCurrentShot);

	return	FALSE;
}