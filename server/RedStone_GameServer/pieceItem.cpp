#include "itemSet.h"
#include "pieceItem.h"
#include <math.h>

cPieceItemManager	g_pieceItemManager;

void
cPieceItem::reset()
{
	m_wSerial	=	0xffff;
}

BOOL
cPieceItem::isUniqueItem()
{
	if (m_bf1IsSetItem)
		return	FALSE;

	return	g_aBasicItem[m_item.m_wBaseItem].isUnique();
}

int
cPieceItem::getDropAblePieceCount(BOOL _bIsUnique,BOOL _bIsSetItem,BOOL _bIsInSecretDungeon,BOOL _bIsInBox,int _iDropLevel,int _iItemType)
{
	int		iDropAblePieceCount	=	0;
	int		iKind,i;

	if (_bIsUnique)
	{
		if	(isUniqueItem() == FALSE)
			return	0;

		iKind		=	g_aBasicItem[m_item.m_wBaseItem].m_wKind;
	}
	else
	if (_bIsSetItem)
	{
		if	(isSetItem() == FALSE)
			return	0;

		cItem	item;

		if	(!generateItem(&item))
			return	0;

		iKind		=	g_aBasicItem[item.m_wBaseItem].m_wKind;
	}
	else
	{
		if	(isUniqueItem() || isSetItem())
			return	0;

		iKind		=	g_aBasicItem[m_item.m_wBaseItem].m_wKind;
	}

	if	(_iItemType	==	eIK_ALL_WEAPON)
	{
		BOOL	bIsCorrect	=	FALSE;

		for	(i=0;i<dWEAPON_TYPE_ITEM_COUNT;i++)
		{
			if	(c_aWeaponTypeItem[i]	==	iKind)
			{
				bIsCorrect	=	TRUE;
				break;
			}
		}

		if	(!bIsCorrect)
			return	FALSE;
	}
	else
	if	(_iItemType	!=	iKind)
		return	FALSE;

	for	(i=0;i<m_wPieceCount;i++)
	{
		cItemPieceDefine	*lpPiece	=	&m_aPiece[i];

		if	(lpPiece->m_wDropLevel	>	_iDropLevel)
			continue;

		BOOL	bIsDropAble	=	FALSE;

		if	(lpPiece->m_bf1IsDropInNormalFieldArca)
		{
			if	(_bIsInSecretDungeon	==	FALSE	&&	_bIsInBox	==	TRUE)
				bIsDropAble		=	TRUE;
		}
		if	(lpPiece->m_bf1IsDropInTempDungeonMonster)
		{
			if	(_bIsInSecretDungeon	==	TRUE	&&	_bIsInBox	==	FALSE)
				bIsDropAble		=	TRUE;
		}
		if	(lpPiece->m_bf1IsDropInTempDungeonArca)
		{
			if	(_bIsInSecretDungeon	==	TRUE	&&	_bIsInBox	==	TRUE)
				bIsDropAble		=	TRUE;
		}
		if	(lpPiece->m_bf1IsDropInNormalFieldMonster)
		{
			if	(_bIsInSecretDungeon	==	FALSE	&&	_bIsInBox	==	FALSE)
				bIsDropAble		=	TRUE;
		}

		if	(bIsDropAble)
			iDropAblePieceCount++;
	}

	return	iDropAblePieceCount;
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
cPieceItem::isSetItem()
{
	return	m_bf1IsSetItem;
}

cItemPiece*
cPieceItem::getPiece(int _iIndex)
{
	if (_iIndex	< 0 || _iIndex >= m_wPieceCount)
		return	NULL;

	return	(cItemPiece*)&m_aPiece[_iIndex];
}

BOOL
cPieceItem::generateItem(cItem *_lpItem)
{
	if (m_bf1IsSetItem	==	FALSE)
	{
		if	(m_item.m_wBaseItem	==	0xffff)
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

		if	(!lpGroup)
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

BOOL
cPieceItemManager::generateItemPiece(BOOL _bIsUnique,BOOL _bIsSetItem,int _iDropLevel,int _iItemType,cItem *_lpItem,BOOL _bIsInSecretDungeon,BOOL _bIsInBox,int _iFieldIndex)
{
	int		iPieceItem;
	WORD	awPieceItemList[1024],wPieceItemCount=0;

	for (iPieceItem=1;iPieceItem<m_den.getMaxCount();iPieceItem++)
	{
		cPieceItem	*lpPieceItem	=	m_den.get(iPieceItem);

		if	(!lpPieceItem)
			continue;

		if	(lpPieceItem->getDropAblePieceCount(_bIsUnique,_bIsSetItem,_bIsInSecretDungeon,_bIsInBox,_iDropLevel,_iItemType)	==	0)
			continue;

		awPieceItemList[wPieceItemCount++]	=	iPieceItem;
	}

	if	(wPieceItemCount	==0)
		return	FALSE;

	int			iSelectPieceItem=	awPieceItemList[random(wPieceItemCount)];
	cPieceItem	*lpDropPieceItem=	m_den.get(iSelectPieceItem);

	if	(!lpDropPieceItem)
		return	FALSE;

	int			iDropItemPiece,iPiece;
	cItemPiece*	lpDropItemPiece;

	for(iPiece=0;iPiece<=100;iPiece++)
	{
		iDropItemPiece	=	random(lpDropPieceItem->m_wPieceCount);
		lpDropItemPiece	=	lpDropPieceItem->getPiece(iDropItemPiece);

		if	(!lpDropItemPiece)
			return	FALSE;

		if	(_iDropLevel	>=	lpDropItemPiece->m_wDropLevel	)
			break;

		lpDropItemPiece	=	NULL;
	}

	if	(lpDropItemPiece	==	NULL)
	{
		for(iPiece=0;iPiece<lpDropPieceItem->m_wPieceCount;iPiece++)
		{
			lpDropItemPiece	=	lpDropPieceItem->getPiece(iPiece);

			if	(!lpDropItemPiece)
				return	FALSE;

			if	(_iDropLevel	>=	lpDropItemPiece->m_wDropLevel	)
				break;

			lpDropItemPiece	=	NULL;
		}
	}

	if	(lpDropItemPiece	==	NULL)
		return	FALSE;

	if	(random(10000)	>=	lpDropItemPiece->m_wDropChance)
		return	FALSE;

	return	g_im.generateItemPiece(_lpItem,iSelectPieceItem,iDropItemPiece,_bIsInSecretDungeon,_bIsInBox,_iFieldIndex);
}