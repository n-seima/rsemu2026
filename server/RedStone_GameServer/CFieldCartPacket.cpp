#include "cFIELD.H"
#include "cGAME.H"
#include "debugCode.H"
#include "carrotShop.H"
#include "packetManager.H"

//
//
void
cFIELD::receiveAskCartOpen(CG_CART_OPEN *_lpPacket,int _iSerial)
{
	if	(g_bIsDuelServer || g_iWorldServerType	==	eSERVER_TYPE_GVG)
		return;

	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	lpActor->askOpenCart();
}

//
//
void
cFIELD::receiveWithdrawItemFromTheCart(CG_WITHDRAW_ITEM_FROM_THE_CART *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	lpActor->askWithdrawItemFromTheCart(_lpPacket->wCartSerial,_lpPacket->wCartSlot,_lpPacket->wInventorySlot,_lpPacket->dwCheckSum);
}

void
cFIELD::receiveOpenCarrotShop(CG_OPEN_CARROT_SHOP *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;
	
	if	(g_bIsDuelServer || g_iWorldServerType	==	eSERVER_TYPE_GVG)
		return;

	lpActor->m_wWaitDBMessageSecond	=	0;

	BOOL	bIsWaitCarrotCount = FALSE;

	if (lpActor->m_iCarrotCount < 0)
		lpActor->m_iCarrotCount = 0;

	_log("[CARROT] open request id[%s] name[%s] cached[%d] dbRequest[%d]",
		lpActor->m_strId,lpActor->m_strName,lpActor->m_iCarrotCount,bIsWaitCarrotCount);

	if	(!bIsWaitCarrotCount	&&	lpActor->m_iCarrotCount	<	0)
		return;

	SG_CARROT_SHOP_INFO	packet;

	packet.base.set(sizeof(packet),dSG_CARROT_SHOP_INFO);
	packet.wStatus				=	0;
	int iCategoryCount = g_carrotShopInfo.m_wCategoryCount;

	if (iCategoryCount > dMAX_CARROT_SHOP_CATEGORY_COUNT)
		iCategoryCount = dMAX_CARROT_SHOP_CATEGORY_COUNT;

	packet.wCount = iCategoryCount;
	packet.iRemainCarrotCount	=	bIsWaitCarrotCount ? -1 : lpActor->m_iCarrotCount;

	DWORD	dwCheckSum	=	g_carrotShopInfo.m_dwCheckSum;

	// The shop catalog is bundled in the client as data/carrotShop.dat.
	// Sending the whole catalog on every open can stall the legacy client,
	// so the server only returns the live balance/open result here.
	packet.wIsOK				=	TRUE;
	packet.wStatus				=	2;
	packet.wIndex				=	0;
	packet.base.wSize			=	sizeof(packet)-sizeof(packet.data);

	g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);

	return;

	if	(FALSE && dwCheckSum		==	_lpPacket->dwCheckSum)
	{
		packet.wIsOK				=	TRUE;
		packet.base.wSize			=	sizeof(packet)-sizeof(packet.data);

		g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);

		return;	
	}

	packet.wIsOK	=	FALSE;

	for	(int	i=0;i<iCategoryCount;i++)
	{
		memcpy(&packet.data,&g_carrotShopInfo.m_aData[i],sizeof(packet.data));
		memcpy(&packet.categoryInfo,g_carrotShopInfo.m_aData[i].getCategoryInfo(),sizeof(cCarrotShopCategoryInfo2));

		if (packet.data.m_wItemCount > dMAX_CARROT_SHOP_ITEM_COUNT)
			packet.data.m_wItemCount = dMAX_CARROT_SHOP_ITEM_COUNT;

		packet.wIndex		=	i;
		packet.base.wSize	=	sizeof(packet)-sizeof(packet.data.m_aItemList)+sizeof(cCarrotShopItemDefine)*packet.data.m_wItemCount;

		g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);

		packet.wStatus		=	1;
	}

	packet.wStatus		=	2;
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.data);

	g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
}

void
cFIELD::receiveCloseCart(CG_CLOSE_CART *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	lpActor->closeCart();
}

void
cFIELD::receiveAskDestroyCartItem(CG_DESTROY_CART_ITEM *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	lpActor->askDestroyCartItem(_lpPacket->wCartSerial,_lpPacket->wCartSlot,_lpPacket->dwCheckSum);
}
