#include "itemSet.h"
#include "pieceItem.h"

cPieceItemManager	g_pieceItemManager;

void
cPieceItem::reset()
{
	m_wSerial	=	0xffff;
}

int
cPieceItem::getMergePrice()
{
	cBasicItem	*lpBasicItem	=	&g_aBasicItem[m_item.m_wBaseItem];

	if	(lpBasicItem->m_iSerial	==	0xffffffff)
		return	0;

	int	iCorrectValue	=	1;

	if	(lpBasicItem->isUnique())
		iCorrectValue	=	15;
	else
	if	(m_bf1IsSetItem)
		iCorrectValue	=	5;

	int	iPrice	=	(int)pow(lpBasicItem->m_wDropLevel,1.4f)*iCorrectValue*100;

//(드랍 레벨^1.4) * 보정치 * 100 gold1.4f)
	return	iPrice;
}

int
cPieceItem::getJudgePrice(int _iPiece)
{
//	드랍 레벨 * 100 gold

	return	m_aPiece[_iPiece].m_wDropLevel*100;
}

BOOL
cPieceItem::generateItem(cItem *_lpItem)
{
	if (m_bf1IsSetItem	==	FALSE)
	{
		if (m_item.m_wBaseItem	==	0xffff)
			return	FALSE;

		memcpy(_lpItem,&m_item,sizeof(CItemBaseInfo));

		_lpItem->m_bf1IsSpecialItem		=	TRUE;

		cSpecialItem	*lpSpecialItem	=	(cSpecialItem	*)_lpItem;

		lpSpecialItem->m_bf1IsUnknown				=	FALSE;
		lpSpecialItem->m_bf1IsSetItem				=	FALSE;
		lpSpecialItem->m_bf10EquipLevel				=	m_wEquipLevel;
		lpSpecialItem->m_bf11SValue					=	m_wSerial;
		lpSpecialItem->m_bf1IsReversionItem			=	FALSE;
		lpSpecialItem->m_bf1IsReversionItemWhenEquip=	FALSE;
		lpSpecialItem->m_isBroken					=	FALSE;
	}
	else
	{
		cItemSet	*lpGroup	=	g_itemSetManager.get(m_bf11SetGroupIndex);

		if (!lpGroup)
			return	FALSE;

		cSetItem		*lpSetItem	=	lpGroup->getItem(m_bf4SetItemIndex);

		if (!lpSetItem)
			return	FALSE;

		return	lpSetItem->generateItem(_lpItem,m_bf11SetGroupIndex);
	}

	return	TRUE;
}

void
cPieceItemManager::build()
{
	m_den.rebuild();
	m_den.remove(0);
}
