#include	"carrotShop.h"

cCarrotShop	g_carrotShopInfo;

void
cCarrotShop::reset()
{
	m_bIsCompleteReceiveCarrotShopInfo	=	FALSE;
	m_wCategoryCount					=	0;

	memset(this,0,sizeof(cCarrotShopDefine));
}

void
cCarrotShop::clearBuffer()
{
	memset(this,0,sizeof(cCarrotShopDefine));
}

void
cCarrotShop::updateInfo(int _iCategory,cCarrotShopCategoryInfo1 *_lpData,cCarrotShopCategoryInfo2* _lpCategoryInfo)
{
	if	(_iCategory < 0 || _iCategory >= dMAX_CARROT_SHOP_CATEGORY_COUNT)
		return;

	cCarrotShopCategoryInfoDefine	*lpCategory	=	&m_aData[_iCategory];

	memset(lpCategory,0,sizeof(cCarrotShopCategoryInfoDefine));

	lpCategory->m_wItemCount	=	min(_lpData->m_wItemCount,dMAX_CARROT_SHOP_ITEM_COUNT);
	memcpy(lpCategory->m_strName,_lpData->m_strName,sizeof(lpCategory->m_strName));
	memcpy(lpCategory->m_aItemList,_lpData->m_aItemList,sizeof(cCarrotShopItemDefine)*lpCategory->m_wItemCount);

	if	(_lpCategoryInfo)
		memcpy(lpCategory->getCategoryInfo(),_lpCategoryInfo,sizeof(cCarrotShopCategoryInfo2));

	printf("- update carrot shop category #%.3d [%s:%d:%d:%d]\n",
			_iCategory,lpCategory->m_strName,lpCategory->m_bf1IsTestCategory,lpCategory->m_bf1IsHotCategory,lpCategory->m_bf1IsRecommandCategory);
}

void
cCarrotShop::buildCheckSum()
{
	m_dwCheckSum	=	getCheckSum();
}
