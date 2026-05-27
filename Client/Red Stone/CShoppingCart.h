#ifndef	_classShoppingCart_h
#define	_classShoppingCart_h

#include "SFC.H"
#include "definePacketData.h"
#include "cPACKET_DBSERVER.h"
#include "cItem.h"

class	CShoppingCart
{
public:
	WORD					m_wSerial;
	DWORD					m_dwCheckSum;
	WORD					m_wItemCount;
	tsBuyLst				m_aItemList[dMAX_BUYLIST];
	int						m_iBookedCartItem;
	int						m_iBookedInventorySlot;

	CShoppingCart()
	{
		reset();
	}

	void					reset()
	{
		m_wSerial	=	0xffff;
	}

	BOOL					isOpened(){if (m_wSerial == 0xffff) return FALSE;return TRUE;}
	void					open(tsBuyLst *_lpCartInfo,int _iItemCount,int _iSerial);
	void					close();

	tsBuyLst*				getCartItem(int _iSlot);
	DWORD					getCheckSum();
	BOOL					generateItem(cItem *_lpItem,int _iIndex);
};

extern	CShoppingCart	g_shoppingCart;

#endif