#include "CActiveSkill.H"
#include "cACTOR.H"
#include "CField.H"
#include "CGame.H"
#include	"astThrowItem.h"

//
//	아이템 던지기
int
cAS_ThrowItem::fire()
{
	m_lpCaster->action(m_lpSkill->m_wAction);

	cItem	*lpItem	=	NULL;

	if	(m_ability.isRequireDrug())
		lpItem	=	m_lpCaster->getFirstDrug(m_lpTarget);

	if	(m_ability.isRequirePotion())
		lpItem	=	m_lpCaster->getFirstPotion(&m_ability);

	if	(m_ability.isRequireFlower())
		lpItem	=	m_lpCaster->getFirstFlower();

	if	(m_ability.isRequireCandy())
		lpItem	=	m_lpCaster->getFirstCandy();

	if	(!lpItem)
		return	eSKILL_FIRE_RESULT_FAILED;

	int	iActiveChance	=	m_lpSkill->m_sActiveChance+m_lpSkill->m_sActiveChancePerLevel/max(m_ability.m_wLevel,1);
	m_iHitRange			=	m_ability.getHitRange(m_lpCaster->getWeapon());
	m_iPotionEfficient	=	(m_lpSkill->m_aValue[0]+m_lpSkill->m_aValue[1]*m_ability.m_wLevel)/100;
	m_iHealHP			=	m_ability.getHealPoint();

	memcpy(&m_item,lpItem,sizeof(cItem));

	if	(random(10000) < iActiveChance)
	{
		int		iSlot	=	m_lpCaster->getItemSlotIndex(lpItem);
		BOOL	bIsBelt	=	FALSE;

		if	(iSlot	>=	dEQUIP_BELT_SLOT1	&&	iSlot	<=	dEQUIP_BELT_SLOT4)
			bIsBelt	=	TRUE;

		lpItem->decreaseCount(1,bIsBelt);

		if	(iSlot	!=	0xffff)
			m_lpCaster->sendUpdateItemData(iSlot,TRUE);
	}
	else
		m_lpCaster->sendEtcWork(eEW_LUCKY_SAVE_THROW_ITEM);

	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	
BOOL
cAS_ThrowItem::cast()
{
	if	(m_lpCaster	==	NULL	||	m_lpTarget	==	NULL)
		return	TRUE;

	if	(m_ability.isRequireDrug())
		m_lpCaster->throwDrug(m_lpTarget,&m_ability,(cItem*)&m_item,m_iHitRange,m_iHealHP);
	else
	if	(m_ability.isRequirePotion())
		m_lpCaster->throwPotion(m_lpTarget,&m_ability,(cItem*)&m_item,m_iHitRange,m_iPotionEfficient);
	else
	if	(m_ability.isRequireCandy() || m_ability.isRequireFlower())
		m_lpCaster->throwCandy(m_lpTarget,&m_ability,m_iHitRange);

	if	(m_lpCaster)
		m_lpCaster->increaseCP(m_ability.getGetCP());

	return	TRUE;
}

//
//	회오리형 업데이트
BOOL
cAS_ThrowItem::update()
{
	if	(!isActionTime())
		return	FALSE;	//	아직 발동 직전이다.

	cast();

	return	TRUE;
}