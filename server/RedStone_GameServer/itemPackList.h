#ifndef	_classItemPackList_h
#define	_classItemPackList_h

#include	"cItem.h"

class	cItemPackList
{
public:
	cItemPackInfo	m_aPacks[dMAX_ITEM_PACK_COUNT];
	BOOL			m_bIsReceiveAllInfo;

	void			init();
	void			reset();

	BOOL			generatePackItem(cItem *_lpItem,int _iPack,int _iCount);
	void			receivePack(cItemPackInfo *_lpPack);
	void			resetCurrentItemDropCount(int _iPack);
	void			updateItemDropCount(int _iPack,int _iItem,int _iCount);

	WORD			getLinkItem(int _iPackSerial);
	BOOL			linkItem(cItem *_lpItem);
	int				getKwangItem(int _iPackSerial);
	cItem*			getRandomItem(int _iPackSerial,BOOL _bIsTest=FALSE);
	BOOL			getItem(cItem *_lpResultItem,int _iPackSerial,int _iItem,int _iPrefix,BOOL _bIsTest=FALSE);
	BOOL			checkReceiveAllInfo();
};

extern	cItemPackList	g_itemPackList;

#endif
