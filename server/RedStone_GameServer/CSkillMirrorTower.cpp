#include "CActiveSkill.H"
#include "cACTOR.H"
#include "cFIELD.H"

//
//	차지스킬 사용
int
CActiveSkill::fireMirrorTower()
{
	int	iReceiverCount	=	m_lpCaster->m_lpField->getMirrorTowerReceiverCount(m_lpCaster);

	if (iReceiverCount	>=	m_lpSkill->m_aValue[3])
		return	eSKILL_FIRE_RESULT_TOO_MANY_USE_THAT_SKILL;

	m_lpCaster->action(m_lpSkill->m_wAction);

	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	차지 스킬 업데이트
BOOL
CActiveSkill::updateMirrorTower()
{
	if (!isActionTime())
		return	FALSE;	//	아직 발동 직전이다.

	m_lpCaster->increaseCP(m_ability.getGetCP());

	m_lpTarget->m_wMirrorTowerCaster	=	m_lpCaster->m_wSerialInField;
	m_lpTarget->m_wMirrorTowerFactor	=	m_ability.getMirrorTowerValue();
	m_lpTarget->m_wMirrorTowerRange		=	m_ability.getMirrorTowerAvailRange();
	m_lpTarget->m_wMirrorTowerUpkeepTime=	m_ability.getUpkeepTime();
	m_lpTarget->m_wMirrorTowerSkill		=	m_ability.m_wSkill;
	m_lpTarget->m_wMirrorTowerCPValue	=	m_ability.getMirrorTowerTargetDamageToCasterCPPercentage();
	m_lpCaster->applySkillExtraEffects(m_lpCaster,&m_ability);

	return	TRUE;
}