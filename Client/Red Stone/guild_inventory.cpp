#include	"guild_inventory.h"
#include	"cHero.h"
#include	"cMESSAGE.h"
#include	"CGamePlayInterface.H"

cGuildInventory	g_gwGuildInventory;

const	int	c_iItemHorizonCountOfGuildInventory		=	6;
const	int	c_iItemVertialCountOfGuildInventory		=	11;
const	int	c_iGuildInventoryWindowDisplayItemCount	=	c_iItemHorizonCountOfGuildInventory*c_iItemVertialCountOfGuildInventory;
const	int	c_iItemSlotWidthOfGuildInventory		=	40;
const	int	c_iItemSlotHeightOfGuildInventory		=	38;

const	int	c_iGuildInventoryWindowTitleHeight		=	16;
const	int	c_iGuildInventoryWindowBottom			=	46;
const	int	c_iGuildInventoryWindowHeightGap		=	c_iGuildInventoryWindowTitleHeight+c_iGuildInventoryWindowBottom;

enum
{
	eCSWI_CARROT	=	6,
	eCSWI_TITLE		=	67,
	eCSWI_FIRST_BAR	=	72,
};

cGuildInventory::cGuildInventory()
{
}
							
BOOL
cGuildInventory::isOpened()	//	열렸냐?
{
	if	(s_iPopupInterface	==	ePIW_GUILD_INVENTORY)
		return	TRUE;

	return	FALSE;
}

void
cGuildInventory::open(cGuildInventoryItemForPacket	*_lpItemList,int _iCount)	//	열기
{
	m_wIsWaitWorkResult	=	FALSE;

	m_iFocusStorage		=	-1;
	m_iSelectStorage	=	0;

	g_guild.receiveInventoryInfo(_lpItemList,_iCount);

	g_hero.lockOff();
	cINPDEV::ReleaseExclusive();
	ResetAllFocus();
	ClosePopupWindow();

	m_iScrollBarPos		=	0;

	int	iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iTitleHeight	=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_TRADE);

	s_iPopupInterface	=	ePIW_GUILD_INVENTORY;

	m_rect.x1			=	g_iScreenWidth-iWindowWidth*2;
	m_rect.x2			=	g_iScreenWidth-iWindowWidth;
	m_iDragItem			=	0xffff;

	m_rect.y1			= g_iCorrectBI_Y/2;
	m_rect.y2			= m_rect.y1+iWindowHeight+iTitleHeight;

	int	iScrollSize		=	c_iGuildInventorySize/c_iItemHorizonCountOfGuildInventory;
	s_sbMiddleWindow.set(m_rect.x1+iWindowWidth-16,m_rect.y1+c_iGuildInventoryWindowTitleHeight+8,iWindowHeight-c_iGuildInventoryWindowHeightGap);
	s_sbMiddleWindow.setPos(0);
	s_sbMiddleWindow.setSize(iScrollSize,c_iItemVertialCountOfGuildInventory);

	SetInventoryWindowPos();
	InitPopupWindowCloseButton();
}

void
cGuildInventory::close()
{
}

int
cGuildInventory::getFocusStorage()
{
	int		iX,iY,iCount	=	0,i;
	int		iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iWidth			=	iWindowWidth-46;

	iX	=	m_rect.x1+13;
	iY	=	m_rect.y2-27;

	for	(i=0;i<4;i++)
	{
		int		iImage		=	eCSWI_FIRST_BAR;
		int		iBarHeight	=	g_sprInterface2.getSpriteHeight(iImage);

		cRECT	rect;

		rect.set(iX,iY,iX+iWidth/4,iY+iBarHeight);

		if	(rect.isIn(s_posMouse.x,s_posMouse.y))
			return	i;

		iX	+=	(iWidth/4+8);
	}

	return	0xffff;
}

int
cGuildInventory::getFocusItemSlot()
{
	int		iX,iY,i;
	CPos	pos;

	int	iTitleHeight	=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_TRADE);
	
	iX		=	m_rect.x1+12;
	iY		=	m_rect.y1+9+iTitleHeight;

	for (i = 0;i<c_iGuildInventoryWindowDisplayItemCount;i++)
	{
		int		iItemIndex	=	i+s_sbMiddleWindow.m_iCurrentPos*c_iItemHorizonCountOfGuildInventory;

		pos.x	=	iX+(i%c_iItemHorizonCountOfGuildInventory)*c_iItemSlotWidthOfGuildInventory;
		pos.y	=	iY+(i/c_iItemHorizonCountOfGuildInventory)*c_iItemSlotHeightOfGuildInventory;

		g_sprInterface.Put(pos.x,pos.y,eIWI_ITEM_SLOT_FRAME);

		if	(iItemIndex	>=	g_guild.m_abInventorySize[m_iSelectStorage])
			break;

		cRECT	rect;

		rect.set(pos.x,pos.y,pos.x+c_iItemSlotWidthOfGuildInventory,pos.y+c_iItemSlotHeightOfGuildInventory);

		if	(rect.isIn(s_posMouse.x,s_posMouse.y))
		{
			s_rectTooltip.set(&rect);
			s_rectTooltip.add(0,40);

			return	g_guild.getInventorySlotIndex(m_iSelectStorage,iItemIndex);
		}
	}

	int		iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);

	cRECT	rect;

	rect.set(m_rect.x1+14,m_rect.y1+iWindowHeight-56+iTitleHeight,0,0);
	rect.x2	=	rect.x1+27;
	rect.y2	=	rect.y1+27;

	if	(rect.isIn(s_posMouse.x,s_posMouse.y))
		return	dITEM_DRAG_GOLD;

	return	0xffff;
}

void
cGuildInventory::operateDragItem()
{
	m_iFocusSlot	=	getFocusItemSlot();

	if	(m_iDragItem	==	0xffff && m_iFocusSlot	==	0xffff)
		return;

	if	(cINPDEV::IsExclusive(eMOUSE_STATE_GUILD_INVENTORY_ITEM_DRAG))
		return;

	if (s_isTouchLeftButton && m_iDragItem == 0xffff)
	{
		if	(m_iFocusSlot	==	dITEM_DRAG_GOLD)
		{
			m_iDragItem		=	m_iFocusSlot;
			cINPDEV::SetExclusive(eMOUSE_STATE_GUILD_INVENTORY_ITEM_DRAG);
		}

		cItem	*lpItem		=	g_guild.getItem(m_iFocusSlot);

		if	(lpItem && lpItem->m_wBaseItem	!=	0xffff)
		{
			m_iDragItem		=	m_iFocusSlot;
			cINPDEV::SetExclusive(eMOUSE_STATE_GUILD_INVENTORY_ITEM_DRAG);
		}
	}

	if	(s_isLeftButtonPressed || m_iDragItem == 0xffff)
		return;

	int	iItemSlot	=	GetFocusItemSlot();
	int	iDroppingArea=	GetItemDroppingArea();

	if	(iDroppingArea==	eDROPPING_ITEM_AREA_INVENTORY_WHEN_OPEN_GUILD_INVENTORY)
		withdrawItem();

	if	(m_iFocusSlot	!=	0xffff && m_iFocusSlot != m_iDragItem		)
		changeItemPlace();

	cINPDEV::ReleaseExclusive();

	m_iDragItem	=	0xffff;
}

void
cGuildInventory::update()	//	스테이터스 윈도우 업데이트
{
	if	(s_iPopupInterface	!=	ePIW_GUILD_INVENTORY)
		return;
	if	(m_wIsWaitWorkResult						)
		return;

	if	(!cINPDEV::IsExclusive(eMOUSE_STATE_SHOP_SCROLL_BAR))
	{
		if	(s_sbMiddleWindow.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed))
			cINPDEV::SetExclusive(eMOUSE_STATE_SHOP_SCROLL_BAR);
		else
			cINPDEV::ReleaseExclusive(TRUE,eMOUSE_STATE_SHOP_SCROLL_BAR);
	}

	m_iFocusStorage	=	getFocusStorage();

	OperateInventoryWindow();
	OperateDragItem();	//	인벤토리 창에 드래그 아이템 처리

	operateDragItem();

	if	(s_isTouchLeftButton	&&	m_iFocusStorage	!=	0xffff)
		m_iSelectStorage	=	m_iFocusStorage;
	else
	if	(g_hero.isGuildMaster()	&&	s_isClickedRightButton	&&	m_iFocusStorage	!=	0xffff)
	{
		s_pmCommon.popup(s_posMouse.x,s_posMouse.y-160);
		s_ttCommon.hide();

		int	i;

		s_pmCommon.addMenu(ePM_CHANGE_GUILD_INVENTORY_STORAGE_SIZE,dMSG_KW_CHANGE_GUILD_INVENTORY_STORAGE_SIZE);

		for	(i=0;i<5;i++)
			s_pmCommon.addMenu(i+ePM_CHANGE_ENABLE_WIDTHDRAW_GOLD_LIMIT_FOR_GUILD_INVENTORY_BEGIN,
								_ms("%s [%s]",dMSG_CHANGE_ENABLE_WIDTHDRAW_GOLD_LIMIT_FOR_GUILD_INVENTORY,g_pstrGuildRank[i+1]));
		for	(i=0;i<5;i++)
			s_pmCommon.addMenu(i+ePM_CHANGE_ENABLE_WIDTHDRAW_ITEM_LIMIT_FOR_GUILD_INVENTORY_BEGIN,
								_ms("%s [%s]",dMSG_CHANGE_ENABLE_WIDTHDRAW_ITEM_LIMIT_FOR_GUILD_INVENTORY,g_pstrGuildRank[i+1]));

		if	(s_pmCommon.getCount())
		{
			m_iPopupedStorage	=	m_iFocusStorage;
			m_iFocusStorage		=	0xffff;
			cINPDEV::SetExclusive(eMOUSE_STATE_POPUP_MENU);
		}
	}

	if	(m_iFocusSlot	!=	0xffff && m_iDragItem == 0xffff)
	{
		if	(m_iFocusSlot	!=	dITEM_DRAG_GOLD)
		{
			cItem	*lpItem		=	g_guild.getItem(m_iFocusSlot);

			if	(lpItem	&&	lpItem->m_wBaseItem	!=	0xffff)
			{
				char	*strComment	=	lpItem->getItemTooltip(-1,-1,TRUE);

				strComment			=	g_hero.getAfterEquipInfo(strComment,lpItem);

				s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,strComment);
			}
		}
	}

	if	(cINPDEV::IsExclusive(eMOUSE_STATE_GUILD_INVENTORY_WINDOW_BUTTONS))
		return;

	DWORD	dwMenu	=	s_bmMiddleWindowButton.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);

	if	(s_bmMiddleWindowButton.isPressed())
		cINPDEV::SetExclusive(eMOUSE_STATE_GUILD_INVENTORY_WINDOW_BUTTONS);
	else
		cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_GUILD_INVENTORY_WINDOW_BUTTONS);
}

void
cGuildInventory::draw()		//	스테이터스 윈도우 그려주기
{
	DrawInventoryWindow();

	int		iX,iY,iCount	=	0,i;
	int		iLineCount		=	0;
	int		iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iTradeFrameWidth=	g_sprInterface.getSpriteWidth(eTWI_FRAME);
	int		iTitleHeight	=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_PET);
	CPos	pos;

	iX		=	m_rect.x1;
	iY		=	m_rect.y1;

	if	(g_config.m_bIsInfoWindowHalfblending)
		cDRAW::FillHB(0,iX+8,iY+6,iX+266-4,iY+482-8+iTitleHeight);
	else
		cDRAW::Fill  (0,iX+8,iY+6,iX+266-4,iY+482-8+iTitleHeight);
	
	g_sprInterface.Put(iX	,iY, eWINDOW_TITLE_BANK); // insu modify .. 0 -> iY
	g_sprInterface.Put(iX	,iY+iTitleHeight, eWINDOW);

	iX		=	m_rect.x1+12;
	iY		=	m_rect.y1+9+iTitleHeight;

	int	iFocusSlot	=	g_guild.getSlotIndexInStorage(m_iFocusSlot);

	for (i = 0;i<c_iGuildInventoryWindowDisplayItemCount;i++)
	{
		int	iItemIndex	=	i+s_sbMiddleWindow.m_iCurrentPos*c_iItemHorizonCountOfGuildInventory;

		pos.x	=	iX+(i%c_iItemHorizonCountOfGuildInventory)*c_iItemSlotWidthOfGuildInventory;
		pos.y	=	iY+(i/c_iItemHorizonCountOfGuildInventory)*c_iItemSlotHeightOfGuildInventory;

		g_sprInterface.Put(pos.x,pos.y,eIWI_ITEM_SLOT_FRAME);

		if	(iItemIndex >= g_guild.m_abInventorySize[m_iSelectStorage])
		{
			g_sprInterface.Put(pos.x,pos.y,eIWI_BLOCKED_ITEM_SLOT);
			continue;
		}

		iItemIndex			=	g_guild.getInventorySlotIndex(m_iSelectStorage,iItemIndex);

		cItem	*lpItem		=	g_guild.getItem(iItemIndex);

		if	(!lpItem	||	lpItem->m_wBaseItem	==	0xffff)
			continue;

		PutItem(lpItem,pos.x,pos.y,TRUE);
	}

	if	(m_iFocusSlot != 0xffff || m_iDragItem != 0xffff)
	{
		if	(m_iFocusSlot !=	0xffff && s_iDragItem != dITEM_DRAG_GOLD && m_iDragItem != dITEM_DRAG_GOLD)
		{
			int	iIndex	=	iFocusSlot-s_sbMiddleWindow.m_iCurrentPos*c_iItemHorizonCountOfGuildInventory;

			pos.x	=	iX+(iIndex%c_iItemHorizonCountOfGuildInventory)*c_iItemSlotWidthOfGuildInventory;
			pos.y	=	iY+(iIndex/c_iItemHorizonCountOfGuildInventory)*c_iItemSlotHeightOfGuildInventory;

			g_sprInterface.Put(pos.x,pos.y,eIWI_ITEM_SLOT_ANM_1+((s_iFrameCounter/8)%5));
		}
	}

	int	iWidth	=	iWindowWidth-46;

	iX	=	m_rect.x1+13;
	iY	=	m_rect.y2-27;

	g_sprInterface.Put(m_rect.x1+14, m_rect.y1+iWindowHeight-56+17, eBW_GOLD_SLOT);
	s_text.put(m_rect.x1+44,m_rect.y1+iWindowHeight-46+17,WHITE,"%s Gold",GetMoneyString(g_guild.m_llGuildGold));

	for	(i=0;i<4;i++)
	{
		int	iImage	=	eCSWI_FIRST_BAR;

		if	(i	==	m_iSelectStorage)
			iImage	+=	3;
		else
		if	(i	==	m_iFocusStorage)
			iImage	+=	6;

		g_sprInterface2.put3PieceBar(iX,iY,iImage,iWidth/4,100);
		s_text.pushShadowStatus(FALSE);
		s_text.cPut(iX+iWidth/4/2,iY+4,0,"%s %d",dMSG_STORAGE,i+1);
		s_text.popShadowStatus();
		iX	+=	(iWidth/4+8);
	}

	if	(s_pmCommon.isPopuped()	==	FALSE	&&	m_iFocusStorage	!=	0xffff)
	{
		if	(g_hero.isGuildMaster())
		{
			iX	=	m_rect.x1+20-100+m_iFocusStorage*(iWidth/4+8);
			iY -=	107; 

			cRECT	rect;
			rect.set(iX-100,iY,iX+300,iY+100);

			s_ttCommon.drawBoxAddBorderSize(&rect,0,24);

			iY	+=	2;

			s_text.putRF(iX,iY,LTGRAY,dMSG_RANK_IN_GUILD2);
			s_text.cPut(iX+80,iY,LTGRAY,dMSG_ENABLE_WIDTHDRAW_GOLD_LIMIT_FOR_GUILD_INVENTORY);
			s_text.cPut(iX+230,iY,LTGRAY,dMSG_ENABLE_WIDTHDRAW_ITEM_LIMIT_FOR_GUILD_INVENTORY);

			iY	+=	18;

			for	(int i=0;i<5;i++)
			{
				s_text.putRF(iX,iY,WHITE,g_pstrGuildRank[i+1]);
				s_text.cPut(iX+80,iY,WHITE,"%s %s",GetMoneyString(g_guild.m_aiWithdrawGoldLimitPerDay[i]),"Gold");
				s_text.cPut(iX+230,iY,WHITE,dMSG_COUNT_FORM,g_guild.m_aWithdrawItemCountPerDay[m_iFocusStorage][i]);
				iY	+=	16;
			}
		}
		else
		{
			iX	=	m_rect.x1+20-100+m_iFocusStorage*(iWidth/4+8);
			iY	=	426;

			cRECT	rect;
			rect.set(iX,iY,iX+300,iY+34);

			s_ttCommon.drawBoxAddBorderSize(&rect,0,24);

			iY	+=	2;

			s_text.cPut(iX+80,iY,LTGRAY,dMSG_ENABLE_WIDTHDRAW_GOLD_LIMIT_FOR_GUILD_INVENTORY);
			s_text.cPut(iX+230,iY,LTGRAY,dMSG_ENABLE_WIDTHDRAW_ITEM_LIMIT_FOR_GUILD_INVENTORY);

			iY	+=	18;
			int	iDrawItemCount	=	g_hero.m_abTodayWithdrawItemCount[m_iFocusStorage];
			int	iDrawGold		=	g_hero.m_iTodayWithdrawGoldInGuild;

			s_text.cPut(iX+80,iY,WHITE,"%s %s",GetMoneyString(g_guild.m_aiWithdrawGoldLimitPerDay[g_hero.m_wGuildRank-1]-iDrawGold),"Gold");
			s_text.cPut(iX+230,iY,WHITE,dMSG_COUNT_FORM,g_guild.m_aWithdrawItemCountPerDay[m_iFocusStorage][g_hero.m_wGuildRank-1]-iDrawItemCount);
		}
	}

	s_sbMiddleWindow.draw();

//	인출 가능 아이템 수 변경
//	인출 가능 금액 변경
}

void
cGuildInventory::cancel()
{
}

void
cGuildInventory::storeItem(int _iItemSlot)
{
	if	(_iItemSlot == dITEM_DRAG_GOLD)
	{
		cItem	item;

		item.m_wBaseItem	=	dITEM_MONEY;
		item.m_dwSerial		=	g_hero.m_iGold;

		g_itemCountWindow.popup(s_posMouse.x,s_posMouse.y,0,&item,eICWR_RETURN_STORE_GOLD_TO_GUILD_INVENTORY);

		return;
	}

	int	iSlotIndex	=	m_iFocusSlot;

	if	(!g_guild.isValidInventorySlot(iSlotIndex))	//	빈 슬롯이 아니다.
	{
		AddSystemMessage(WHITE,dMSG_ADD_ITEM_ONLY_GUILD_INVENTORY_EMPTY_SLOT);
		return;
	}

	s_agent.sendEtcWork(eCEW_STORE_GUILD_INVENTORY_ITEM,_iItemSlot,iSlotIndex);
}

void
cGuildInventory::withdrawItem()
{
	int	iItemSlot	=	GetFocusItemSlot();

	if	(m_iDragItem==	dITEM_DRAG_GOLD)
	{
		cItem	item;

		item.m_wBaseItem	=	dITEM_MONEY;
		item.m_dwSerial		=	min(g_guild.m_llGuildGold,2000000000);

		g_itemCountWindow.popup(s_posMouse.x,s_posMouse.y,0,&item,eICWR_RETURN_WITHDRAW_GOLD_FROM_GUILD_INVENTORY);

		return;
	}

	if	(iItemSlot	==	0xffff)
		return;

	s_agent.sendEtcWork(eCEW_WITHDRAW_GUILD_INVENTORY_ITEM,iItemSlot,m_iDragItem);
}

void
cGuildInventory::storeGold(int _iGold)
{
	WORD	wGold1	=	_iGold>>16;
	WORD	wGold2	=	_iGold&0xffff;

	s_agent.sendEtcWork(eCEW_STORE_GUILD_INVENTORY_GOLD,wGold1,wGold2);
}

void
cGuildInventory::withdrawGold(int _iGold)
{
	WORD	wGold1	=	_iGold>>16;
	WORD	wGold2	=	_iGold&0xffff;

	s_agent.sendEtcWork(eCEW_WITHDRAW_GUILD_INVENTORY_GOLD,wGold1,wGold2);
}

void
cGuildInventory::changeItemPlace()
{
	s_agent.sendEtcWork(eCEW_CHANGE_GUILD_INVENTORY_ITEM_PLACE,m_iDragItem,m_iFocusSlot);
}