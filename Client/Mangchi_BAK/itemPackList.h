#ifndef	_classItemPackList_h
#define	_classItemPackList_h

#include	"cITEM.h"

class	cItemPackList
{
public:
	cItemPackInfo	m_aPacks[dMAX_ITEM_PACK_COUNT];

	void			init();
	void			reset();
};

extern	cItemPackList	g_itemPackList;

#endif
