#ifndef _classPieceItem_H
#define _classPieceItem_H

#include "cItem.h"

class	cItemPiece	:	public cItemPieceDefine
{
public:
};

class	cPieceItem	:	public cPieceItemDefine
{
public:
	void		reset();
	void		copy(cPieceItem *_lpPieceItem)	{}
	BOOL		generateItem(cItem *_lpItem);
	BOOL		isUniqueItem();
	int			getDropAblePieceCount(BOOL _bIsUnique,BOOL _bIsSetItem,BOOL _bIsInSecretDungeon,BOOL _bIsInBox,int _iDropLevel,int _iItemType);
	
	int			getMergePrice();
	int			getJudgePrice(int _iPiece);

	BOOL		isSetItem();
	cItemPiece*	getPiece(int _iIndex);
};

class	cPieceItemManager
{
public:
	cDen<cPieceItem>			m_den;

	inline	cPieceItem*			get(int _iIndex)
	{
		return	m_den.get(_iIndex);
	}

	inline	void				init(int _iCount)
	{
		m_den.init(_iCount);
	}

	inline	cPieceItem*			getBay()
	{
		return	m_den.getList();
	}

	void						build();
	BOOL						generateItemPiece(BOOL _bIsUnique,BOOL _bIsSetItem,int _iDropLevel,int _iItemType,cItem *_lpItem,BOOL _bIsInSecretDungeon,BOOL _bIsInBox,int _iFieldIndex);
};

extern	cPieceItemManager	g_pieceItemManager;

#endif