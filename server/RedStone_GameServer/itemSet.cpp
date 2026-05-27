#include "itemSet.h"
#include "debugCode.h"

cItemSetManager	g_itemSetManager;

BOOL
cSetItem::generateItem(cItem *_lpItem,int _iItemSet)
{
	int		i,j;

	if	(m_wBaseItem	==	0xffff)
		return FALSE;

	cItemSet	*lpItemSet			=	g_itemSetManager.get(_iItemSet);

	if	(!lpItemSet)
		return	FALSE;

	cSpecialItem	*lpSpecialItem	=	(cSpecialItem	*)_lpItem;

	if	(m_bf1IsCustomItem)
	{
		memcpy(_lpItem,this,sizeof(CItemBaseInfo));

		lpSpecialItem->reset();

		lpSpecialItem->m_bf1IsSetItem				=	TRUE;
		lpSpecialItem->m_bf10EquipLevel				=	lpItemSet->m_wEqiupLevel;
		lpSpecialItem->m_bf11SValue					=	_iItemSet;

		if	(m_bf4ReversionType		==	eIRT_REVERSION_WHEN_GET)
			lpSpecialItem->m_bf1IsReversionItem		=	TRUE;
		if	(m_bf4ReversionType		==	eIRT_REVERSION_WHEN_EQUIP)
			lpSpecialItem->m_bf1IsReversionItemWhenEquip	=	TRUE;

		return	TRUE;
	}

	cBasicItem	*lpBasicItem	=	&g_aBasicItem[m_wBaseItem];

	_lpItem->m_wBaseItem		=	m_wBaseItem;
	_lpItem->m_bCount			=	m_bCount;
	_lpItem->m_bDurability		=	(BYTE)lpBasicItem->getDurability();

	if	(_lpItem->isItemPack())
		_lpItem->m_aOption[0]	=	(BYTE)m_wGenerateQuality;
	else
	for (i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
	{
		if (lpBasicItem->m_aGenerateData[i].m_wEffect	==	0xffff)
			break;

		int	iReferenceIndex,iRange,iMinValue,iMaxValue;

//	可 1
		{
			iReferenceIndex	=	lpBasicItem->m_aGenerateData[i].m_aValue[0];
			iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
			iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue+1;

			_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+iRange*m_wGenerateQuality/100,iMaxValue);
		}

//	可 2
		{
			iReferenceIndex	=	lpBasicItem->m_aGenerateData[i].m_aValue[1];
			iMinValue		=	lpBasicItem->m_aValue[iReferenceIndex][0];
			iMaxValue		=	lpBasicItem->m_aValue[iReferenceIndex][1];
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue+1;

			_lpItem->m_aOption[iReferenceIndex]	=	min(iMinValue+iRange*m_wGenerateQuality/100,iMaxValue);
		}

		if (lpBasicItem->m_aGenerateData[i].m_wEffect	==	eIE_USE_JEWEL)
			_lpItem->m_bDurability	=	_lpItem->m_aOption[iReferenceIndex];
	}

	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		int	iPrefix	=	m_aPrefix[i].m_wPrefix;

		if (iPrefix	==	0xffff)
			break;

		_lpItem->m_aPrefix[i].m_wPrefix		=	iPrefix;

		cITEM_PREFIX	*lpPrefix			=	&g_aItemPrefix[iPrefix];

		for (j=0;j<2;j++)
		{
			int	iMinValue,iMaxValue,iRange;

			iMinValue	=	lpPrefix->m_aValue[j][0];
			iMaxValue	=	lpPrefix->m_aValue[j][1];
			iMaxValue	=	max(iMaxValue,iMinValue);
			iRange		=	iMaxValue-iMinValue+1;

			_lpItem->m_aPrefix[i].m_aValue[j]	=	min(iMinValue+iRange*m_aPrefixGenerateQuality[i]/100,iMaxValue);
		}
	}

	lpSpecialItem->reset();

	lpSpecialItem->m_bf1IsSetItem	=	TRUE;
	lpSpecialItem->m_bf10EquipLevel	=	lpItemSet->m_wEqiupLevel;
	lpSpecialItem->m_bf11SValue		=	_iItemSet;

	if	(m_bf4ReversionType		==	eIRT_REVERSION_WHEN_GET)
		lpSpecialItem->m_bf1IsReversionItem				=	TRUE;
	if	(m_bf4ReversionType		==	eIRT_REVERSION_WHEN_EQUIP)
		lpSpecialItem->m_bf1IsReversionItemWhenEquip	=	TRUE;

	return	TRUE;
}	

void
cItemSet::reset()
{
	m_wSerial	=	0xffff;
}

int
cItemSet::getItemCount()
{
	int	iCount	=	0;

	for (int i=0;i<dMAX_ITEM_COUNT_IN_SET;i++)
	{
		if (m_aItem[i].m_wBaseItem	!=	0xffff)
			iCount	++;
	}

	return	iCount;
}

cSetItem*
cItemSet::getItem(int _iSlot)
{
	if	(_iSlot	>=	dMAX_ITEM_COUNT_IN_SET)
		return	NULL;

	if	(m_aItem[_iSlot].m_wBaseItem	==	0xffff)
		return	NULL;

	return	(cSetItem*)&m_aItem[_iSlot];
}

cUniqueData*	
cItemSet::getSetEffect(int _iSetItemCount,int _iEffectIndex)
{
	if	(_iSetItemCount	<=	0	||	_iSetItemCount	>	dMAX_SET_ITEM_EFFECT_GRADE_COUNT	||
		_iEffectIndex	<	0	||	_iEffectIndex	>=	dMAX_SET_ITEM_EFFECT_COUNT)
		return	NULL;

	if (m_aSetEffect[_iSetItemCount-1][_iEffectIndex].m_wEffect	==	0xffff)
		return	NULL;

	return	&m_aSetEffect[_iSetItemCount-1][_iEffectIndex];
}

int
cItemSet::getSetEffectCount(int _iSetItemCount)
{
	if	(_iSetItemCount	<=	0	||	_iSetItemCount	>	dMAX_SET_ITEM_EFFECT_GRADE_COUNT	)
		return	0;

	int	iCount	=	0;

	for (int i=0;i<dMAX_SET_ITEM_EFFECT_COUNT;i++)
	{
		if (m_aSetEffect[_iSetItemCount-1][i].m_wEffect	!=	0xffff)
			iCount++;
	}

	return	iCount;
}

cSetItem*
cItemSet::getItemByBaseItem(int _iBaseItem)
{
	for (int i=0;i<getItemCount();i++)
		if (_iBaseItem == m_aItem[i].m_wBaseItem)
			return	(cSetItem*)&m_aItem[i];

	return	NULL;
}

void
cItemSetManager::build()
{
	for (int i=0;i<m_iCount;i++)
		for (int iSetItem=0;iSetItem<dMAX_ITEM_COUNT_IN_SET;iSetItem++)
			m_pItemSet[i].m_aItem[iSetItem].m_pItemComment	=	NULL;
}

BOOL
cItemSetManager::generateRandomSetItem(int _iItemType,int _iDropLevel,cItem *_lpItem,BOOL _bIsInSecretDungeon,BOOL _bIsInBox)
{
	int		iItemSet,iSetItemCount=0;
	DWORD	adwSetItemList[1024];

	for (iItemSet=1;iItemSet<m_iCount;iItemSet++)
	{
		cItemSet	*lpItemSet	=	get(iItemSet);

		if	(!lpItemSet)
			continue;

		for (int i=0;i<lpItemSet->getItemCount();i++)
		{
			cSetItem	*lpSetItem	=	lpItemSet->getItem(i);

			if	(!lpSetItem || lpSetItem->m_wDropLevel > _iDropLevel)
				continue;

			if	(g_aBasicItem[lpSetItem->m_wBaseItem].m_wKind	!=	_iItemType)
				continue;

			BOOL	bIsDropAble		=	FALSE;

			if	(lpSetItem->m_bf1IsDropInNormalFieldArca)
			{
				if	(_bIsInSecretDungeon	==	FALSE	&&	_bIsInBox	==	TRUE)
					bIsDropAble		=	TRUE;
			}
			if	(lpSetItem->m_bf1IsDropInTempDungeonMonster)
			{
				if	(_bIsInSecretDungeon	==	TRUE	&&	_bIsInBox	==	FALSE)
					bIsDropAble		=	TRUE;
			}
			if	(lpSetItem->m_bf1IsDropInTempDungeonArca)
			{
				if	(_bIsInSecretDungeon	==	TRUE	&&	_bIsInBox	==	TRUE)
					bIsDropAble		=	TRUE;
			}
			if	(lpSetItem->m_bf1IsDropInNormalFieldMonster)
			{
				if	(_bIsInSecretDungeon	==	FALSE	&&	_bIsInBox	==	FALSE)
					bIsDropAble		=	TRUE;
			}

			if	(bIsDropAble	==	FALSE)
				continue;

			adwSetItemList[iSetItemCount++]	=	iItemSet*0x8000 + i;
		}
	}

	if	(iSetItemCount	==0)
		return	FALSE;

	DWORD		dwSelectItemSet		=	adwSetItemList[random(iSetItemCount)];
	int			iSelectItemSet		=	dwSelectItemSet/0x8000;
	int			iSelectSetItem		=	dwSelectItemSet%0x8000;

	cItemSet	*lpSelectItemSet	=	get(iSelectItemSet);

	if	(!lpSelectItemSet)
		return	FALSE;

	cSetItem	*lpSelectSetItem	=	lpSelectItemSet->getItem(iSelectSetItem);

	if	(!lpSelectSetItem)
		return	FALSE;

	if	(random(10000) >= lpSelectSetItem->m_wDropChance)
		return	FALSE;

	return	lpSelectSetItem->generateItem(_lpItem,iSelectItemSet);
}

BOOL
cItemSetManager::generateSetItem(cItem *_lpItem,int _iSet,int _iIndex)
{
	cItemSet	*lpItemSet	=	get(_iSet);

	if	(!lpItemSet)
		return	FALSE;

	cSetItem	*lpSetItem	=	lpItemSet->getItem(_iIndex);

	if	(!lpSetItem)
		return	FALSE;

	return	lpSetItem->generateItem(_lpItem,_iSet);
}
