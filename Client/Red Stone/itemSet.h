#ifndef _classItemSet_H
#define _classItemSet_H

#include "cItem.h"

class	cSetItem	:	public	cSetItemDefine
{
public:
	BOOL			generateItem(cItem *_lpItem,int _iItemSet);
	char*			getComment();
};

class	cItemSet	:	public	cItemSetDefine
{
public:
	void			reset();
	void			copy(cItemSet	*_lpPieceItem)	{}

	int				getItemCount();
	cSetItem*		getItem(int _iSlot);
	cSetItem*		getItemByBaseItem(int _iBaseItem);

	cUniqueData*	getSetEffect(int _iSetItemCount,int _iEffectIndex);
	int				getSetEffectCount(int _iSetItemCount);
	char*			getComment();
};

class	cItemSetManager
{
public:
	int							m_iCount;
	cItemSet					*m_pItemSet;

	cItemSetManager()
	{
		m_pItemSet	=	NULL;
	}
	~cItemSetManager()
	{
		pKILL(m_pItemSet);
	}

	inline	cItemSet*			get(int _iIndex)
	{
		if	(_iIndex >= m_iCount || m_pItemSet[_iIndex].m_wSerial	==	0xfff)
			return	NULL;

		return	&m_pItemSet[_iIndex];
	}

	inline	void				init(int _iCount)
	{
		m_pItemSet	=	new cItemSet [_iCount];

		m_iCount	=	_iCount;
	}

	inline	cItemSet*			getBay()
	{
		return	m_pItemSet;
	}

	void						build();
};

extern	cItemSetManager	g_itemSetManager;

#endif