#ifndef	_classCarrotShop_h
#define	_classCarrotShop_h

#include	"carrotShopDefine.h"

class	cCarrotShop	:	public cCarrotShopDefine
{
public:
	BOOL			m_bIsCompleteReceiveCarrotShopInfo;
	DWORD			m_dwCheckSum;

	void			buildCheckSum();
	void			reset();
	void			clearBuffer();

	void			updateInfo(int _iCategory,cCarrotShopCategoryInfo1 *_lpData,cCarrotShopCategoryInfo2* _lpCategoryInfo);
};

extern	cCarrotShop	g_carrotShopInfo;

#endif