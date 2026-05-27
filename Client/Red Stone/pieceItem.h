#ifndef _classPieceItem_H
#define _classPieceItem_H

#include "cItem.h"

class	cPieceItem	:	public cPieceItemDefine
{
public:
	void	reset();
	void	copy(cPieceItem *_lpPieceItem)	{}
	BOOL	generateItem(cItem *_lpItem);
	int		getMergePrice();
	int		getJudgePrice(int _iPiece);
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
};

extern	cPieceItemManager	g_pieceItemManager;

#endif