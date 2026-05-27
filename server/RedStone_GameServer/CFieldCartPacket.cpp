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

	SG_CARROT_SHOP_INFO	packet;

	packet.base.set(sizeof(packet),dSG_CARROT_SHOP_INFO);
	packet.wStatus				=	0;
	packet.wCount				=	g_carrotShopInfo.m_wCategoryCount;
	packet.iRemainCarrotCount	=	lpActor->m_iCarrotCount;

	DWORD	dwCheckSum	=	g_carrotShopInfo.m_dwCheckSum;

	if	(dwCheckSum		==	_lpPacket->dwCheckSum)
	{
		packet.wIsOK				=	TRUE;
		packet.base.wSize			=	sizeof(packet)-sizeof(packet.data);

		g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);

		if	(lpActor->m_iCarrotCount	<	0)
			g_game.sendGetCarrotCount(lpActor);

		return;	
	}

	packet.wIsOK	=	FALSE;

	for	(int	i=0;i<g_carrotShopInfo.m_wCategoryCount;i++)
	{
		memcpy(&packet.data,&g_carrotShopInfo.m_aData[i],sizeof(packet.data));
		memcpy(&packet.categoryInfo,g_carrotShopInfo.m_aData[i].getCategoryInfo(),sizeof(cCarrotShopCategoryInfo2));

		packet.wIndex		=	i;
		packet.base.wSize	=	sizeof(packet)-sizeof(packet.data.m_aItemList)+sizeof(cCarrotShopItemDefine)*packet.data.m_wItemCount;

		g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);

		packet.wStatus		=	1;
	}

	packet.wStatus		=	2;
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.data);

	g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);

	if	(lpActor->m_iCarrotCount	<	0)
		g_game.sendGetCarrotCount(lpActor);
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
