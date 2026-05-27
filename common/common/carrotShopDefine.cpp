#include "carrotShopDefine.h"

// 	eCarrotShop_AllItemToInventory	=	0,
// 	eCarrotShop_IDPublicItemToCart,
// 	eCarrotShop_AllItemToCart,

#ifdef	_FOR_USA
	int	g_iCarrotShopType	=	eCarrotShop_AllItemToInventory;
#elif	_FOR_JAPAN
	int	g_iCarrotShopType	=	eCarrotShop_AllItemToInventory;
#else
	int	g_iCarrotShopType	=	eCarrotShop_AllItemToInventory;
#endif
