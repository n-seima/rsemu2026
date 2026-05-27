#ifndef	_classCarrotShopDefine_h
#define	_classCarrotShopDefine_h

#include	"CItemDefine.h"

#define	dCARROT_SHOP_CATEGORY_NAME_LENGTH	20
#define	dMAX_OWN_CARROT_COUNT				500

class	cCarrotShopItemDefine
{
public:
	WORD							m_wPremiunItemIndex;

	WORD							m_bf8ItemCount : 8;
	WORD							m_bf1IsTestItem	: 1;		//	test 아이템
	WORD							m_bf1IsHotItem : 1;			//	신규 아이템
	WORD							m_bf1IsRecommandItem : 1;	//	추천 아이템
	WORD							:	5;

	WORD							m_wCarrotCount;
};

class	cCarrotShopCategoryInfoDefineOld
{
public:
	WORD							m_wItemCount;
	char							m_strName[20];

	cCarrotShopItemDefine			m_aItemList[100];
};

class	cCarrotShopCategoryInfo1
{
public:
	WORD							m_wItemCount;
	char							m_strName[dCARROT_SHOP_CATEGORY_NAME_LENGTH];

	cCarrotShopItemDefine			m_aItemList[dMAX_CARROT_SHOP_ITEM_COUNT];
};

class	cCarrotShopCategoryInfo2
{
public:
	WORD							m_bf1IsTestCategory	:	1;	//	테스트 카테과賭(이건 테섭에서만 보인다.)
	WORD							m_bf1IsHotCategory	:	1;	//	신규 아이템 카테과賭
	WORD							m_bf1IsRecommandCategory	:	1;	//	추천 아이템 카테과賭
	WORD							:	13;
	WORD							m_awSpareBuffer[2];
};

class	cCarrotShopCategoryInfoDefine : public cCarrotShopCategoryInfo1,public cCarrotShopCategoryInfo2
{
public:
	cCarrotShopCategoryInfo2*	getCategoryInfo()
	{
		char	*lpBuffer	=	(char*)this;

		return	(cCarrotShopCategoryInfo2*)(lpBuffer+sizeof(cCarrotShopCategoryInfo1));
	}
};

class	cCarrotShopDefine
{
public:
	WORD							m_wCategoryCount;

	cCarrotShopCategoryInfoDefine	m_aData[dMAX_CARROT_SHOP_CATEGORY_COUNT];

	DWORD	getCheckSum()
	{
		DWORD	dwCrc32	=	0;

		cCRC32::BufferToCrc32(this,sizeof(cCarrotShopDefine),dwCrc32);

		return	dwCrc32;
	}
};

enum
{
	eCarrotShop_AllItemToInventory	=	0,
	eCarrotShop_IDPublicItemToCart,
	eCarrotShop_AllItemToCart,
};

extern	int	g_iCarrotShopType;

#endif