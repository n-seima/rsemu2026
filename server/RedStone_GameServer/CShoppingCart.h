#ifndef	_classShoppingCart_h
#define	_classShoppingCart_h

#include "cItem.H"
#include "cPACKET_GAMESERVER.H"
#include "cPACKET_DBSERVER.H"

#define	dMAX_OPEN_CART		1000

//
//	Cart Item Message 
enum
{
	eCIM_FAILED,
	eCIM_SUCCESS,
	eCIM_CAN_NOT_OWN_SAME_ITEM,
	eCIM_TOO_MANY_BADGE_ITEM,
	eCIM_CAN_WITHDRAW_ITEM,
	eCIM_CAN_NOT_WITHDRAW_ITEM_BY_EXTRA_BAG_SIZE_FULL,
};

class	CShoppingCart
{
public:
	WORD					m_wSerial;
	DWORD					m_dwCheckSum;
	cACTOR					*m_lpActor;

	WORD					m_wItemCount;
	tsBuyLst				m_aItemList[dMAX_BUYLIST];

	CShoppingCart()
	{
		reset();
	}

	void					reset()
	{
		m_wSerial	=	0xffff;
		m_lpActor	=	NULL;
	}

	void					open(tsBuyLst *_lpCartInfo,int _iItemCount,cACTOR *_lpActor);
	BOOL					checkExpiredItem(int _iCartSlot);


	BOOL					askWithdrawItemFromTheCart(int _iInventorySlot,int _iCartSlot,DWORD _dwChecksum);
	BOOL					withdrawItemFromTheCart(int _iInventorySlot,int _iCartSlot);

	BOOL					askDestroyCartItem(int _iCartSlot,DWORD _dwChecksum);
	BOOL					destroyCartItem(int _iCartSlot);

	BOOL					checkIsIncorrectData(DWORD _dwChecksum);
	DWORD					getCheckSum();

	cItem*					getActorItem(cItem *_lpItem);
	BOOL					sendWorkResultLog();
};

class	CShoppingCartManager
{
public:
	CShoppingCart			*m_pCart;
	int						m_iCartCount,m_iCartSize;
	CRITICAL_SECTION		m_cs;

							CShoppingCartManager();
							~CShoppingCartManager();

	void					init(int _iCartCount = dMAX_OPEN_CART);
	int						open(tsBuyLst *_lpCartInfo,int _iItemCount,cACTOR *_lpActor);
	int						getFreeCartIndex();
	void					closeCart(int _iCartSerial,BOOL _bIsForced=FALSE);

	inline	CShoppingCart*	getCart(int _iIndex)
	{
		if (_iIndex < 0 || _iIndex >= m_iCartSize)
			return	NULL;
		if (m_pCart[_iIndex].m_wSerial	==	0xffff)
			return	NULL;

		return	&m_pCart[_iIndex];
	}
};

extern	CShoppingCartManager	g_cartManager;

#endif