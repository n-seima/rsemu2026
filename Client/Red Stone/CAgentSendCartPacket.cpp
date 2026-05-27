#include "CAgent.H"
#include "CActor.H"
#include "CGamePlay.H"

//
//	쇼핑 카트 열기
BOOL
CAgent::sendAskOpenShoppingCart()
{
	CG_CART_OPEN	packet;

	if	(g_hero.m_bf1IsWaitShoppingCommandResult)
		return	FALSE;

	g_hero.m_bf1IsWaitShoppingCommandResult	=	TRUE;

	packet.base.set(sizeof(packet),dCG_CART_OPEN);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//
BOOL
CAgent::sendCloseShoppingCart()
{
	CG_CLOSE_CART	packet;

	if (g_hero.m_bf1IsWaitShoppingCommandResult)	return	FALSE;

	packet.base.set(sizeof(CG_CLOSE_CART),dCG_CLOSE_CART);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendWithdrawItemFromTheCart(int _iCartSerial,int _iCartSlot,int _iInventorySlot,DWORD _dwCheckSum)
{
	CG_WITHDRAW_ITEM_FROM_THE_CART	packet;

	if (g_hero.m_bf1IsWaitShoppingCommandResult)	return	FALSE;

	g_hero.m_bf1IsWaitShoppingCommandResult	=	TRUE;

	packet.base.set(sizeof(packet),dCG_WITHDRAW_ITEM_FROM_THE_CART);

	packet.wCartSerial		=	_iCartSerial;
	packet.wCartSlot		=	_iCartSlot;
	packet.wInventorySlot	=	_iInventorySlot;
	packet.dwCheckSum		=	_dwCheckSum;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendDestroyCartItem(int _iCartSerial,int _iCartSlot,DWORD _dwCheckSum)
{
	CG_DESTROY_CART_ITEM	packet;

	if (g_hero.m_bf1IsWaitShoppingCommandResult)	return	FALSE;

	g_hero.m_bf1IsWaitShoppingCommandResult	=	TRUE;

	packet.base.set(sizeof(packet),dCG_DESTROY_CART_ITEM);

	packet.wCartSerial		=	_iCartSerial;
	packet.wCartSlot		=	_iCartSlot;
	packet.dwCheckSum		=	_dwCheckSum;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendOpenCarrotShop(DWORD _dwCheckSum)
{
	CG_OPEN_CARROT_SHOP	packet;

	packet.base.set(sizeof(packet),dCG_OPEN_CARROT_SHOP);

	packet.dwCheckSum	=	_dwCheckSum;

	return	sendPacket((char *)&packet,packet.base.wSize);
}
