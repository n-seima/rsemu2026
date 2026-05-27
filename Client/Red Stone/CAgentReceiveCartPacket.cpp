#include "CAgent.H"
#include "CActor.H"
#include "CGamePlay.H"
#include "CSkill.H"
#include "cMESSAGE.H"
#include "CTitle.H"
#include "CWindowInterface.H"
#include "CShoppingCart.H"

//	쇼핑 카트 열기
void
cPACKET_HANDLER::ReceiveOpenCart(SERVER_PACKETS *_lpPacket)
{
	SG_OPEN_CART	*lpPacket	=	&_lpPacket->mSG_OPEN_CART;

	g_hero.m_bf1IsWaitShoppingCommandResult	=	FALSE;

	CGamePlay::AddSystemMessage(LTYELLOW,dMSG_RECEIVE_SHOPPING_CART_INFO);
	g_gwCart.open((void *)lpPacket->aItemList,lpPacket->wCartSerial,lpPacket->wItemCount);
}

//	쇼핑 카트 열기
void
cPACKET_HANDLER::ReceiveCartMessage(SERVER_PACKETS *_lpPacket)
{
	SG_CART_MESSAGE				*lpPacket	=	&_lpPacket->mSG_CART_MESSAGE;

	g_hero.m_bf1IsWaitShoppingCommandResult	=	FALSE;

	switch(lpPacket->wCartMessage)
	{
		case	eCM_CART_CLOSE_BY_INCORRECT_INFO:	//	뭔가 정보가 잘못되어서 닫았다.
		{
			g_gwCart.cancelCartTransaction();
			break;
		}
		
		case	eCM_CAN_NOT_OWN_SAME_ITEM			:
			g_msgBox.cPopup("",dMSG_CAN_NOT_OWN_SAME_ITEM,dMSG_OK);
			break;

		case	eCM_TOO_MANY_BADGE_ITEM				:
			g_msgBox.cPopup("",_ms(dMSG_TOO_MANY_BADGE_ITEM_FORM,dLIMIT_BADGE_ITEM_COUNT_IN_INVENTORY),dMSG_OK);
			break;

		case	eCM_CAN_NOT_WITHDRAW_ITEM_BY_EXTRA_BAG_SIZE_FULL	:
			g_msgBox.cPopup("",dMSG_CAN_NOT_WITHDRAW_ITEM_BY_EXTRA_BAG_SIZE_FULL,dMSG_OK);
			break;

		case	eCM_FAILED_BY_EXPIRED_ITEM		:
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_CM_FAILED_BY_EXPIRED_ITEM);
			g_gwCart.cancelCartTransaction();
			break;
		}

		case	eCM_INVENTORY_FULL				:	//	인벤토리가 비어있다.
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_WITHDRAW_ITEM_BY_FULL_INVENTORY);
			break;
		}

		case	eCM_FAILED_BY_INVENTORY_FULL	:	//	꺼낼라고 보니까 인벤토리에 빈칸이 없다!!
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_FAILED_WITHDRAW_ITEM_BY_FULL_INVENTORY);
			g_gwCart.cancelCartTransaction();
			break;
		}
		
		case	eCM_FAILED_BY_VALID_ITEM_INFO	:	//	프리미엄 아이템 정보를 기본으로 아이템을 생성하는데 실패했다.
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_FAILED_WITHDRAW_ITEM_BY_FULL_INVENTORY);
			g_gwCart.cancelCartTransaction();
			break;
		}

		case	eCM_EXIST_FAILED_PREMIUM_ITEM	:	//	꺼내기 실패한 프리미엄 아이템이 있어
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_EXIST_LOAD_FAILED_PREMIUM_ITEM);
			g_gwCart.cancelCartTransaction();
			break;
		}

		case	eCM_FAILED_BY_TOO_FAST_REASK	:	//	너무 빨리 다시 오픈 했슈.
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_PLEASE_WAIT_FOR_NEXT_OPEN_CART);
			break;
		}
		
		case	eCM_ALREADY_OPEN_CART			:	//	이미 카트를 오픈했슈
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_ALREADY_OPENED_CART);
			break;
		}
		case	eCM_TOO_MANY_OPEN_SHOPPING_CART	:	//	카트 오픈이 너무 많어.. ;;
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_TOO_MANY_OPENED_CART);
			break;
		}
		case	eCM_WAIT_CART_DATA				:	//	카트 데이터 기다려요
		{
			CGamePlay::AddSystemMessage(WHITE,dMSG_WAIT_FOR_CART_DATA);
			break;
		}

		case	eCM_NOT_OPENED_CART				:	//	카트가 열려 있지 않아요.
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_NOT_OPENED_CART);
			break;
		}
		
		case	eCM_CAN_NOT_LOAD_THAT_PLACE		:	//	그 자리에는 아이템을 꺼낼 수 없어효!!!
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_EXIST_ITEM_INVENTORY_SLOT);
			break;
		}

		case	eCM_ONLY_ONE_ITEM_IN_INVENTORY	:	//	그 자리에는 아이템을 꺼낼 수 없어효!!!
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_ONLY_ONE_ITEM_IN_INVENTORY);
			break;
		}

		case	eCM_TOO_MANY_ITEM_COUNT			:
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_TOO_MANY_CART_ITEM_COUNT);
			break;
		}

		case	eCM_ALREADY_EXIST_MORE_HIGH_LEVEL_ORB	:
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_ALREADY_EXIST_MORE_HIGH_LEVEL_ORB);
			break;
		}

		case	eCM_WAIT_FOR_PREVIOUS_WORK_RESULT		:	//	좀 전에 요청한 작업 결과 기다리는 중이잖아!!
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_WAIT_FOR_PREVIOUS_CART_WORK_RESULT);
			break;
		}

		case	eCM_LOTTERY_EVENT_COMMENT				:	//	복권 이벤트 기간이다. 자리좀 비워라
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_LOTTERY_EVENT_COMMENT);
			break;
		}

		case	eCM_MAKE_RICE_SOUP_PLACE				:
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_KOREA_NEW_YEAR_EVENT_COMMENT);
			break;
		}

		case	eCM_ALREADY_EXIST_SAME_EFFECT_IN_EMBLEM	:
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_UMCIR_ALREADY_EXIST_SHAPE);
			break;
		}

		case	eCM_CAN_NOT_WITHDRAW_CARROT_BY_OWN_LIMIT	:
		{
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_DEPOSIT_CARROT_CHECKED_BY_LIMIT_OWN_COUNT,dMAX_OWN_CARROT_COUNT);
			break;
		}
	}
}

//
//	쇼핑 카트 열기
void
cPACKET_HANDLER::ReceiveWithdrawItemFromCart(SERVER_PACKETS *_lpPacket)
{
	SG_WITHDRAW_ITEM_FROM_THE_CART		*lpPacket	=	&_lpPacket->mSG_WITHDRAW_ITEM_FROM_THE_CART;

	g_hero.m_bf1IsWaitShoppingCommandResult	=	FALSE;

	g_gwCart.withdrawItemFromTheCart(lpPacket->wCartSlot,lpPacket->wInventorySlot,(cItem *)&lpPacket->item);
}

//
//	쇼핑 카트 열기
void
cPACKET_HANDLER::ReceiveLoadFailedCartPremiumItem(SERVER_PACKETS *_lpPacket)
{
	SG_LOAD_FAILED_CART_PREMIUM_ITEM	*lpPacket	=	&_lpPacket->mSG_LOAD_FAILED_CART_PREMIUM_ITEM;

	g_hero.m_bf1IsWaitShoppingCommandResult	=	FALSE;

	CGamePlay::AddSystemMessage(LTYELLOW,dMSG_LOAD_FAILED_CART_PREMIUM_ITEM);

	memcpy(&g_hero.m_aItems[lpPacket->wInventorySlot],&lpPacket->item,sizeof(cItem));
}

//
//	카트의 아이템 제거
void
cPACKET_HANDLER::ReceiveDestroyCartItemResult(SERVER_PACKETS *_lpPacket)
{
	SG_DESTROY_CART_ITEM_RESULT		*lpPacket	=	&_lpPacket->mSG_DESTROY_CART_ITEM_RESULT;

	g_hero.m_bf1IsWaitShoppingCommandResult	=	FALSE;

	switch(lpPacket->wResult)
	{
		case	eDCIR_SUCCESS				:
			g_gwCart.removeCartItem(lpPacket->wCartSlot);
			break;

		case	eDCIR_FAILED				:
			break;

		case	eDCIR_CAN_NOT_DESTROY_NOT_USED_ITEM	:
			g_msgBox.cPopup("",dMSG_CAN_NOT_DESTROY_NOT_USED_CART_ITEM,dMSG_OK);
			break;
	}
}

void
cPACKET_HANDLER::ReceiveCarrotShopInfo(SERVER_PACKETS *_lpPacket)
{
	SG_CARROT_SHOP_INFO	*lpPacket	=	(SG_CARROT_SHOP_INFO	*)_lpPacket;

	if	(lpPacket->wIsOK	||	lpPacket->wStatus	==	2)
	{
		if	(lpPacket->wStatus	==	2)
		{
			g_carrotShop.m_wCategoryCount	=	lpPacket->wCount;
			g_carrotShop.save();
		}

		if	(lpPacket->iRemainCarrotCount	==	-1)
			return;

		g_gwCarrotShop.m_iRemainCarrotCount	=	lpPacket->iRemainCarrotCount;

		g_gwCarrotShop.open();

		return;
	}

	if	(lpPacket->wStatus	==	0)
		g_carrotShop.reset();

	g_carrotShop.updateInfo(lpPacket->wIndex,&lpPacket->data,&lpPacket->categoryInfo);
}
