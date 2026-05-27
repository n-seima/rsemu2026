#include "CWindowInterface.H"
#include "CGamePlayInterface.H"
#include "CActor.H"
#include "CShoppingCart.H"
#include "CMessage.H"

CCartWindow	g_gwCart;

#define		dITEM_HORIZON_COUNT_OF_THE_CART		6
#define		dITEM_VERTICAL_COUNT_OF_THE_CART	8
#define		dCART_WINDOW_DISPLAY_ITEM_COUNT		(dITEM_HORIZON_COUNT_OF_THE_CART*dITEM_VERTICAL_COUNT_OF_THE_CART)
#define		dITEM_SLOT_WIDTH_OF_THE_CART		40
#define		dITEM_SLOT_HEIGHT_OF_THE_CART		38

#define		dCART_WINDOW_TITLE_HEIGHT			16
#define		dCART_WINDOW_BOTTOM					46
#define		dCART_WINDOW_HEIGHT_GAP				(dCART_WINDOW_TITLE_HEIGHT+dCART_WINDOW_BOTTOM)

enum
{
	eCWI_CARROT_COUNT	=	34,
};

//
//	ø≠æ!!
void
CCartWindow::open(void *_lpItemInfo,int _iCartSerial,int _iItemCount)
{
	g_shoppingCart.open((tsBuyLst *)_lpItemInfo,_iItemCount,_iCartSerial);

	for (int i = 0;i<_iItemCount;i++)
	{
		cItem	item;

		if (!g_shoppingCart.generateItem(&item,i))
			continue;

		if (item.isItemPack())
		{
			int	iPackIndex	=	item.m_aOption[0];

			if	(g_astrItemPackListName[iPackIndex][0]	==	NULL)
				AddAskItemPackName(iPackIndex);
		}
	}

	g_hero.lockOff();
	cINPDEV::ReleaseExclusive();
	ResetAllFocus();
	ClosePopupWindow();

	int	iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iTitleHeight	=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_TRADE);

	s_iPopupInterface	=	ePIW_SHOPPING_CART;
	m_rect.x1			=	g_iScreenWidth-iWindowWidth*2;
	m_rect.y1			=	iTitleHeight+g_iCorrectBI_Y/2;
	m_rect.x2			=	g_iScreenWidth-iWindowWidth;
	m_rect.y2			=	iTitleHeight+iWindowHeight+g_iCorrectBI_Y/2;
	m_iDragCartItem		=	0xffff;

	s_bmMiddleWindowButton.reset();

	s_bmMiddleWindowButton.addImageBar(dMSG_CLOSE_SHOPPING_CART,IDOK		,m_rect.x1+197,461+g_iCorrectBI_Y/2,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
	s_bmMiddleWindowButton.setClickMargin(IDOK,1,1);
	s_bmMiddleWindowButton.setAllBarTextColor(0,FALSE);

	SetInventoryWindowPos();
	InitPopupWindowCloseButton();
}

//
//	±
void
CCartWindow::close()
{
	if	(isOpened())
	{
		g_shoppingCart.close();

		if	(g_hero.m_bf1IsWaitShoppingCommandResult	==	FALSE)
			s_agent.sendCloseShoppingCart();
	}

	cINPDEV::ReleaseExclusive();

	CloseInventoryWindow();

	s_iPopupInterface	=	ePIW_NONE;
	m_iDragCartItem		=	0xffff;
	s_bmMiddleWindowButton.reset();
}

//
//	´Æø° øΩ∫∞° ∞ æ¿€
int
CCartWindow::getFocusCartItemSlot()
{
	int	i;
//	int	iX	=	m_rect.x1+11;
//	int	iY	=	m_rect.y1+9;
	int	iX	=	m_rect.x1+18;
	int	iY	=	m_rect.y1+dCART_WINDOW_TITLE_HEIGHT+60;

	for (i = 0;i<dCART_WINDOW_DISPLAY_ITEM_COUNT;i++)
	{
		int	iIndex	=	i;

		if (iIndex	>=	g_shoppingCart.m_wItemCount)
			break;

		cRECT	rect;
		CPos	pos;


		pos.x	=	iX+(i%dITEM_HORIZON_COUNT_OF_THE_CART)*dITEM_SLOT_WIDTH_OF_THE_CART;
		pos.y	=	iY+(i/dITEM_HORIZON_COUNT_OF_THE_CART)*dITEM_SLOT_HEIGHT_OF_THE_CART;

		rect.set(pos.x,pos.y,pos.x+dITEM_SHAPE_WIDTH,pos.y+dITEM_SHAPE_HEIGHT);

		if (!rect.isIn(s_posMouse.x,s_posMouse.y))
			continue;

		s_rectTooltip.set(&rect);
		s_rectTooltip.add(0,40);

		return	iIndex;
	}

	return	0xffff;
}

//
//	æ¿€ ¿øø° æ ∏∫ √≥∏Æ
inline	void 
CCartWindow::operateCartWindowPopupMenu()
{
	if (cINPDEV::IsExclusive(eMOUSE_STATE_POPUP_MENU))	return;

	if (!s_isClickedRightButton		)
		return;
	if (m_iFocusCartSlot==	0xffff	)
		return;

	m_iSelectCartSlot	=	m_iFocusCartSlot;

	if (s_pmCommon.isPopuped())
	{
		s_pmCommon.reset();

		return;
	}

	tsBuyLst	*lpItem	=	g_shoppingCart.getCartItem(m_iFocusCartSlot);

	if	(!lpItem						)
		return;
	if	(lpItem->dwItemNo==	0xffffffff	)
		return;

	s_pmCommon.popup(s_posMouse.x,s_posMouse.y);
	s_ttCommon.hide();

//	s_pmCommon.addMenu(ePM_DESTROY_CART_ITEM	,dMSG_DESTROY_CART_ITEM);

	if (s_pmCommon.getCount())
		cINPDEV::SetExclusive(eMOUSE_STATE_POPUP_MENU);
	else
		s_pmCommon.reset();
}

//
//	¿∫‡√ æ¿€ ∑°± √≥∏Æ
void 
CCartWindow::operateDragItem()
{
	m_iFocusCartSlot	=	getFocusCartItemSlot();

	if (m_iDragCartItem	==	0xffff && m_iFocusCartSlot	==	0xffff)
		return;

	if (cINPDEV::IsExclusive(eMOUSE_STATE_CART_ITEM_DRAG))
		return;

	if (s_isTouchLeftButton && m_iDragCartItem == 0xffff)
	{
		tsBuyLst	*lpItem	=	g_shoppingCart.getCartItem(m_iFocusCartSlot);

		if (lpItem && lpItem->dwItemNo	!=	0xffffffff)
		{
			m_iDragCartItem = m_iFocusCartSlot;
			cINPDEV::SetExclusive(eMOUSE_STATE_CART_ITEM_DRAG);
		}
	}

	if (s_isLeftButtonPressed || m_iDragCartItem == 0xffff)
		return;

	int	iItemSlot	=	GetFocusItemSlot();

	if (GetItemDroppingArea()	==	eDROPPING_ITEM_AREA_INVENTORY_WHEN_CART_TRANSACTION	)
	{
		cItem	item;

		if (g_shoppingCart.generateItem(&item,m_iDragCartItem))
		{
			m_iWithdrawItemCartSlot	=	m_iDragCartItem;
			
			if (item.getExpandInventorySize())
			{
				g_msgBox.cPopup("",dMSG_WARNING_WITHDRAW_MAGIC_BAG,dMSG_YES,dMSG_NO);
				g_msgBox.setButtonId(eMBI_WITHDRAW_MAGIC_BAG);
			}
			else
				askWithdrawItemFromTheCart();
		}
	}

	cINPDEV::ReleaseExclusive();

	m_iDragCartItem	=	0xffff;
}	//	CCartWindow::operateDragItem()

//
//	æ¿Æ
void
CCartWindow::update()
{
	if (s_iPopupInterface	!=	ePIW_SHOPPING_CART	)
		return;
	if (g_hero.m_bf1IsWaitShoppingCommandResult		)
		return;

	if (!g_shoppingCart.isOpened())
	{
		close();
		return;
	}

	OperateInventoryWindow();
	OperateDragItem();	//	¿Œ∫‰∏Æ √ø° ∑°± æ¿€ √≥∏Æ
	operateDragItem();

	if (cINPDEV::IsExclusive(eMOUSE_STATE_CART_WINDOW_BUTTONS))
		return;

	if (m_iFocusCartSlot	!=	0xffff && m_iDragCartItem == 0xffff)
	{
		cItem	item;

		if (g_shoppingCart.generateItem(&item,m_iFocusCartSlot))
		{
			char	*strComment	=	item.getItemTooltip(-1,-1,TRUE);

			strComment			=	g_hero.getAfterEquipInfo(strComment,&item);

			s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,strComment);
		}
	}

	DWORD	dwMenu	=	s_bmMiddleWindowButton.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);

	if (s_bmMiddleWindowButton.isPressed())
		cINPDEV::SetExclusive(eMOUSE_STATE_CART_WINDOW_BUTTONS);
	else
		cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_CART_WINDOW_BUTTONS);

	if (dwMenu	==	IDOK)	close();
}	//	CCartWindow::update()

//
//	Ω∫¿Ω∫ ¿ø ±∑¡¡÷±
void
CCartWindow::draw()
{
	if (s_iPopupInterface	!=	ePIW_SHOPPING_CART)
		return;

	DrawInventoryWindow();

	int		iX,iY,iCount	=	0,i;
	int		iLineCount		=	0;
	int		iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iTradeFrameWidth=	g_sprInterface.getSpriteWidth(eTWI_FRAME);
	CPos	pos;

	iX		=	m_rect.x1;
	iY		=	m_rect.y1;

	if (g_config.m_bIsInfoWindowHalfblending)
		cDRAW::FillHB(0,iX+8,iY+6,iX+266-4,iY+482-8);
	else
		cDRAW::Fill  (0,iX+8,iY+6,iX+266-4,iY+482-8);

	g_sprInterface.Put(iX	,g_iCorrectBI_Y/2		,eWINDOW_TITLE_SHOPPING_CART);
	g_sprInterface.Put(iX	,iY		,eWINDOW);

	iX		=	m_rect.x1+18;
	iY		=	m_rect.y1+dCART_WINDOW_TITLE_HEIGHT+60;

	for (i = 0;i<dCART_WINDOW_DISPLAY_ITEM_COUNT;i++)
	{
		int		iItemIndex	=	i;

		pos.x	=	iX+(i%dITEM_HORIZON_COUNT_OF_THE_CART)*dITEM_SLOT_WIDTH_OF_THE_CART;
		pos.y	=	iY+(i/dITEM_HORIZON_COUNT_OF_THE_CART)*dITEM_SLOT_HEIGHT_OF_THE_CART;

		g_sprInterface.Put(pos.x,pos.y,eIWI_ITEM_SLOT_FRAME);

		cItem	item;

		if (!g_shoppingCart.generateItem(&item,iItemIndex))
			continue;

		PutItem(&item,pos.x,pos.y,TRUE);
	}

	if (s_iDragItem != 0xffff || m_iDragCartItem != 0xffff)
	{
		if (m_iFocusCartSlot !=	0xffff)
		{
			int	iIndex	=	m_iFocusCartSlot;

			pos.x	=	iX+(iIndex%dITEM_HORIZON_COUNT_OF_THE_CART)*dITEM_SLOT_WIDTH_OF_THE_CART;
			pos.y	=	iY+(iIndex/dITEM_HORIZON_COUNT_OF_THE_CART)*dITEM_SLOT_HEIGHT_OF_THE_CART;

			g_sprInterface.Put(pos.x,pos.y,eIWI_ITEM_SLOT_ANM_1+((s_iFrameCounter/8)%5));
		}
	}

	s_bmMiddleWindowButton.draw();
}

//
//	¿∫‡∞∑° √º“ - π∫∞° πÆ¡¶∞° ¿÷.
void
CCartWindow::cancelCartTransaction()
{
	g_hero.m_bf1IsWaitShoppingCommandResult	=	FALSE;

	CGamePlay::AddSystemMessage(WHITE,dMSG_CLOSED_SHOPPING_CART_BY_NOT_MATCHED_DATA_WITH_SERVER);

	s_agent.sendCloseShoppingCart();
	close();
}

//
//	æ¿€¿ª Øø°º≠ √æø¿±
void
CCartWindow::askWithdrawItemFromTheCart(BOOL _bIsWithdrawMagicBag)
{
	tsBuyLst	*lpItem		=	g_shoppingCart.getCartItem(m_iWithdrawItemCartSlot);

	if	(!lpItem || lpItem->dwItemNo	==	0xffffffff)
		return;

	if	(m_iWithdrawItemCartSlot>=	g_shoppingCart.m_wItemCount	||	m_iWithdrawItemCartSlot	<	0	)
		return;

	int		iItemSlot	=	GetFocusItemSlot();

	if (_bIsWithdrawMagicBag)
		iItemSlot	=	0;
	else
	{
		if	(iItemSlot	>=	g_hero.m_wItemSlotCount	|| iItemSlot	<	0	)
			return;

		if (g_hero.m_aItems[iItemSlot].m_wBaseItem	!=	0xffff)
			return;
	}

	s_agent.sendWithdrawItemFromTheCart(g_shoppingCart.m_wSerial,m_iWithdrawItemCartSlot,iItemSlot,g_shoppingCart.getCheckSum());
}

//
//	æ¿€¿ª Øø°º≠ √æø¿±
void
CCartWindow::withdrawItemFromTheCart(int _iCartslot,int _iInventorySlot,cItem *_lpItem)
{
	tsBuyLst	*lpCartItem	=	g_shoppingCart.getCartItem(_iCartslot);

	if	(lpCartItem->dwItemNo	==	0xffffffff)
	{
		s_agent.sendCloseShoppingCart();

		return;
	}

	if	(_lpItem->getExpandInventorySize()==0	&&	g_hero.m_aItems[_iInventorySlot].m_wBaseItem	!=	0xffff)
	{
		s_agent.sendCloseShoppingCart();
		return;
	}

	cBasicItem	*lpBasicItem	=	_lpItem->getBasicItem();

	if	(lpBasicItem->m_wIsIDPublicItem	==	FALSE)
		lpCartItem->dwItemNo	=	0xffffffff;	

	if (_lpItem->getExpandInventorySize())
	{
		CGamePlay::AddSystemMessage(LTGREEN,dMSG_EXAPND_INVENTORY);
		g_hero.m_bf6ExtraInventorySize	+=	_lpItem->getExpandInventorySize();
	}
	else
		memcpy(&g_hero.m_aItems[_iInventorySlot],_lpItem,sizeof(cItem));

	g_hero.buildPower();
}

//
//
void
CCartWindow::removeCartItem(int _iCartslot)
{
	tsBuyLst	*lpCartItem	=	g_shoppingCart.getCartItem(_iCartslot);

	if (lpCartItem)
		lpCartItem->dwItemNo	=	0xffffffff;
}

//
//	∞Æ∞° ø≠∑¡ ¿÷
BOOL
CCartWindow::isOpened()
{
	if (g_shoppingCart.m_wSerial!=	0xffff)
		return	TRUE;

	return	FALSE;
}

void
CCartWindow::askDestroyCartItem()
{
	tsBuyLst	*lpCartItem	=	g_shoppingCart.getCartItem(m_iSelectCartSlot);

	if (!lpCartItem)	return;

	if (lpCartItem->timeEndDate.m_dwValue	==	0)
	{
		g_msgBox.cPopup("",dMSG_CAN_NOT_DESTROY_NOT_USED_CART_ITEM,dMSG_OK);
		return;
	}

	cItem	item;

	if (g_shoppingCart.generateItem(&item,m_iSelectCartSlot))
	{
		char	strText[256];

		sprintf(strText,dMSG_ARE_YOU_REALLY_DESTROY_CART_ITEM_FORM,item.getName());

		g_msgBox.cPopup("",strText,dMSG_YES,dMSG_NO);
		g_msgBox.setButtonId(eMBI_DESTROY_CART_ITEM);
	}
}