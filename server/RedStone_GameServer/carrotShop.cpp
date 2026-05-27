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
	cCarrotShopCategoryInfoDefine	*lpCategory	=	&m_aData[_iCategory];

	memcpy(lpCategory,_lpData,sizeof(cCarrotShopCategoryInfo1));
	memcpy(lpCategory->getCategoryInfo(),_lpCategoryInfo,sizeof(cCarrotShopCategoryInfo2));

	memset(lpCategory->m_aItemList+lpCategory->m_wItemCount,0,sizeof(cCarrotShopItemDefine)*(dMAX_CARROT_SHOP_ITEM_COUNT-lpCategory->m_wItemCount));

	printf("- update carrot shop category #%.3d [%s:%d:%d:%d]\n",
			_iCategory,lpCategory->m_strName,lpCategory->m_bf1IsTestCategory,lpCategory->m_bf1IsHotCategory,lpCategory->m_bf1IsRecommandCategory);
}

void
cCarrotShop::buildCheckSum()
{
	m_dwCheckSum	=	getCheckSum();
}