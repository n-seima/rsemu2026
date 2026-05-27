#include "cACTOR.H"
#include "cFIELD.H"
#include "debugCode.H"
#include "CGame.H"
#include "CDataSave.H"
#include "CShoppingCart.h"
#include "packetManager.h"


//
//	카트 오픈 요청
BOOL
cACTOR::askOpenCart()
{
	if (m_wCartSerial	!=	0xffff)
	{
		closeCart();
		sendCartMessage(eCM_ALREADY_OPEN_CART);

		return	TRUE;
	}
	
	if	(g_bIsDuelServer || g_iWorldServerType == eSERVER_TYPE_GVG)
		return	FALSE;

	if (!g_bIsTestServer && g_dwCurrentTime < m_dwLastestAskPremiumItemCartInfoTime+dASK_CART_INFO_PERIOD && g_dwCurrentTime > m_dwLastestAskPremiumItemCartInfoTime)
	{
		sendCartMessage(eCM_FAILED_BY_TOO_FAST_REASK);
		return FALSE;
	}

	if (m_itemFailedPremiumItem.m_wBaseItem	!=	0xffff)
	{
		cItem	*lpInventorySlot	=	getValidInventorySlot();

		if (!lpInventorySlot)
		{
			sendCartMessage(eCM_EXIST_FAILED_PREMIUM_ITEM);

			return FALSE;
		}

		int		iSlotIndex			=	getValidInventorySlotIndex();

		memcpy(lpInventorySlot,&m_itemFailedPremiumItem,sizeof(cItem));

		m_itemFailedPremiumItem.reset();

		sendLoadFailedCartPremiumItem(lpInventorySlot,iSlotIndex);
	}

	m_dwLastestAskPremiumItemCartInfoTime	=	g_dwCurrentTime;

	if (g_cartManager.getFreeCartIndex() == 0xffff)
	{
		sendCartMessage(eCM_TOO_MANY_OPEN_SHOPPING_CART);

		return	FALSE;
	}

	sendCartMessage(eCM_WAIT_CART_DATA);

	{
		GDMSG_BUYLIST	packet;

		packet.base.set(sizeof(packet),dGDMSG_BUYLIST);
		packet.dwSerialInServer	=	m_iZoneSerial;
		strcpy(packet.strId,m_strId);

		g_socketDBC.SendPacket((char *)&packet,((cMSG_BASE_TYPE *)&packet)->wSize);
	}

	return	TRUE;
}

BOOL
cACTOR::askWithdrawItemFromTheCart(int _iCartSerial,int _iCartSlot,int _iInventorySlot,DWORD _dwCheckSum)
{
	if	(m_wCartSerial	==	0xffff)
	{
		sendCartMessage(eCM_NOT_OPENED_CART);

		return	FALSE;
	}

	CShoppingCart	*lpCart		=	g_cartManager.getCart(_iCartSerial);

	if	(lpCart			==	NULL)
	{
		sendCartMessage(eCM_NOT_OPENED_CART);

		return	FALSE;
	}

	if	(_iCartSerial	!=	m_wCartSerial)
	{
		g_cartManager.closeCart(_iCartSerial);
		sendCartMessage(eCM_NOT_OPENED_CART);

		return	FALSE;
	}

	int	iWithdrawResult	=	lpCart->askWithdrawItemFromTheCart(_iInventorySlot,_iCartSlot,_dwCheckSum);

	if	(iWithdrawResult	==	eCIM_CAN_NOT_OWN_SAME_ITEM)
	{
		sendCartMessage(eCM_CAN_NOT_OWN_SAME_ITEM);

		return	TRUE;
	}

	if	(iWithdrawResult	==	eCIM_TOO_MANY_BADGE_ITEM)
	{
		sendCartMessage(eCM_TOO_MANY_BADGE_ITEM);

		return	TRUE;
	}

	if	(iWithdrawResult	==	eCIM_CAN_NOT_WITHDRAW_ITEM_BY_EXTRA_BAG_SIZE_FULL)
	{
		sendCartMessage(eCM_CAN_NOT_WITHDRAW_ITEM_BY_EXTRA_BAG_SIZE_FULL);

		return	TRUE;
	}

	if	(iWithdrawResult	==	eCIM_FAILED)
	{
		printf("askWithdrawItemFromTheCart - iWithdrawResult == eCIM_FAILED\n");
		CLOG("cart","askWithdrawItemFromTheCart - iWithdrawResult == eCIM_FAILED");

		g_cartManager.closeCart(_iCartSerial);
		sendCartMessage(eCM_CART_CLOSE_BY_INCORRECT_INFO);

		return	FALSE;
	}
	
	return	TRUE;
}

BOOL
cACTOR::askDestroyCartItem(int _iCartSerial,int _iCartSlot,DWORD _dwCheckSum)
{
	if (m_wCartSerial	==	0xffff)
	{
		sendCartMessage(eCM_NOT_OPENED_CART);

		return	FALSE;
	}

	CShoppingCart	*lpCart		=	g_cartManager.getCart(_iCartSerial);

	if	(lpCart			==	NULL)
	{
		sendCartMessage(eCM_NOT_OPENED_CART);

		return	FALSE;
	}

	if	(_iCartSerial	!=	m_wCartSerial)
	{
		g_cartManager.closeCart(_iCartSerial);
		sendCartMessage(eCM_NOT_OPENED_CART);

		return	FALSE;
	}

	if (!lpCart->askDestroyCartItem(_iCartSlot,_dwCheckSum))
	{
		printf("askDestroyCartItem - !lpCart->askDestroyCartItem(_iCartSlot,_dwCheckSum)\n");
		CLOG("cart","askDestroyCartItem - !lpCart->askDestroyCartItem(_iCartSlot,_dwCheckSum)");
		
		g_cartManager.closeCart(_iCartSerial);
		sendCartMessage(eCM_CART_CLOSE_BY_INCORRECT_INFO);

		return	FALSE;
	}
	
	return	TRUE;
}

BOOL
cACTOR::openCart(tsBuyLst *_lpCartInfo,int _iItemCount)
{
	if (m_wCartSerial	!=	0xffff)
		return	FALSE;

	int	iCartSerial	=	g_cartManager.open(_lpCartInfo,_iItemCount,this);
	
	if (iCartSerial	==	0xffff)
	{
		m_dwLastestAskPremiumItemCartInfoTime	=	0;
		sendCartMessage(eCM_TOO_MANY_OPEN_SHOPPING_CART);

		return	FALSE;
	}

	m_wCartSerial		=	iCartSerial;

	SG_OPEN_CART	packet;

	packet.base.set(sizeof(packet),dSG_OPEN_CART);
	packet.wCartSerial	=	iCartSerial;
	packet.wItemCount	=	_iItemCount;
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aItemList)+_iItemCount*sizeof(tsBuyLst);

	memcpy(&packet.aItemList,_lpCartInfo,_iItemCount*sizeof(tsBuyLst));
	
	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);

	return	TRUE;
}

BOOL
cACTOR::closeCart()
{
	if (m_wCartSerial	==	0xffff)	return	FALSE;

	g_cartManager.closeCart(m_wCartSerial);

	m_wCartSerial		=	0xffff;

	return	TRUE;
}	//	cACTOR::closeCart()


BOOL
cACTOR::withdrawCartItem(int _iCartSlot,int _iInventorySlot)
{
	if	(m_wCartSerial		==	0xffff)
		return	FALSE;

	CShoppingCart	*lpCart	=	g_cartManager.getCart(m_wCartSerial);

	if	(!lpCart)
	{
		printf("withdrawCartItem - !g_cartManager.getCart(%d)\n",m_wCartSerial);
		CLOG("cart","withdrawCartItem - !g_cartManager.getCart(%d)",m_wCartSerial);
		cancelCart();
		return	FALSE;
	}

	if	(!lpCart->withdrawItemFromTheCart(_iInventorySlot,_iCartSlot))
	{
		g_cartManager.closeCart(m_wCartSerial);
		m_wCartSerial		=	0xffff;
		
		return FALSE;
	}

	return	TRUE;
}

BOOL
cACTOR::destroyCartItem(int _iCartSlot)
{
	if (m_wCartSerial		==	0xffff)	return	FALSE;

	CShoppingCart	*lpCart	=	g_cartManager.getCart(m_wCartSerial);

	if	(!lpCart)
	{
		printf("destroyCartItem - !g_cartManager.getCart(%d)\n",m_wCartSerial);
		CLOG("cart","destroyCartItem - !g_cartManager.getCart(%d)",m_wCartSerial);

		cancelCart();
		return	FALSE;
	}

	if (!lpCart->destroyCartItem(_iCartSlot))
	{
		g_cartManager.closeCart(m_wCartSerial);
		m_wCartSerial		=	0xffff;
		
		return FALSE;
	}

	return	TRUE;
}

BOOL
cACTOR::cancelCart()
{
	if (m_wCartSerial	==	0xffff)	return	FALSE;

	g_cartManager.closeCart(m_wCartSerial);

	m_wCartSerial		=	0xffff;

	sendCartMessage(eCM_CART_CLOSE_BY_INCORRECT_INFO);

	return	TRUE;
}

BOOL
cACTOR::failedCart()
{
	m_itemFailedPremiumItem.reset();

	return	cancelCart();
}