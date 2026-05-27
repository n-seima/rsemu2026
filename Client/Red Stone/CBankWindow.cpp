#include "CWindowInterface.H"
#include "CGamePlayInterface.H"
#include "CActor.H"
#include "CBank.H"
#include "CMessage.H"

CBankWindow	g_gwBank;

#define		dITEM_HORIZON_COUNT_OF_THE_BANK		6
#define		dITEM_VERTICAL_COUNT_OF_THE_BANK	11
#define		dBANK_WINDOW_DISPLAY_ITEM_COUNT		(dITEM_HORIZON_COUNT_OF_THE_BANK*dITEM_VERTICAL_COUNT_OF_THE_BANK)
#define		dITEM_SLOT_WIDTH_OF_THE_BANK		40
#define		dITEM_SLOT_HEIGHT_OF_THE_BANK		38

#define		dBANK_WINDOW_TITLE_HEIGHT			16
#define		dBANK_WINDOW_BOTTOM					46
#define		dBANK_WINDOW_HEIGHT_GAP				(dBANK_WINDOW_TITLE_HEIGHT+dBANK_WINDOW_BOTTOM)

//
//	열어!!

void
CBankWindow::open(CBankInfo *_lpBankInfo,int _iBankSerial,int _iBankSize,int _iBankCharge)
{
	m_wIsWaitSaveResult	=	FALSE;

	g_bank.open(_lpBankInfo,_iBankSerial,_iBankSize,_iBankCharge);

	for (int i = 0;i<_iBankSize;i++)
	{
		cItem	*lpItem	=	g_bank.getBankItem(i);

		if	(!lpItem)
			continue;

		if	(lpItem->isItemPack())
		{
			int	iPackIndex	=	lpItem->m_aOption[0];

			if	(g_astrItemPackListName[iPackIndex][0]	==	NULL)
				AddAskItemPackName(iPackIndex);
		}
	}

	if	(!g_hero.m_bf4RebirthCount && g_hero.m_iLevel	<	3)		// 전생자는 체크하지않는당.
		AddSystemMessage(LTYELLOW,dMSG_BANK_CLIENT_LEVEL_LIMIT);

	g_hero.lockOff();
	cINPDEV::ReleaseExclusive();
	ResetAllFocus();
	ClosePopupWindow();

	s_lpInventoryWindowEquipmentList=	g_bank.m_aEquipment;
	s_lpInventoryWindowItemList		=	g_bank.m_aInventoryItem;
	s_lpGold						=	&g_bank.m_iOwnGold;
	m_iScrollBarPos		=	0;

	int	iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iTitleHeight	=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_TRADE);

	s_iPopupInterface	=	ePIW_BANK;
	m_rect.x1			=	g_iScreenWidth-iWindowWidth*2;
	m_rect.y1			=	iTitleHeight+g_iCorrectBI_Y/2;
	m_rect.x2			=	g_iScreenWidth-iWindowWidth;
	m_rect.y2			=	iTitleHeight+iWindowHeight+g_iCorrectBI_Y/2;
	m_iDragBankItem		=	0xffff;

	int	iScrollSize		=	dMAX_ITEM_IN_BANK/dITEM_HORIZON_COUNT_OF_THE_BANK;
	s_sbMiddleWindow.set(m_rect.x1+iWindowWidth-16,m_rect.y1+dBANK_WINDOW_TITLE_HEIGHT-2,iWindowHeight-dBANK_WINDOW_HEIGHT_GAP);
	s_sbMiddleWindow.setPos(0);
	s_sbMiddleWindow.setSize(iScrollSize,dITEM_VERTICAL_COUNT_OF_THE_BANK);

	s_bmMiddleWindowButton.reset();

	s_bmMiddleWindowButton.addImageBar(dMSG_FINISH_BANK_TRANSACTION,IDOK		,m_rect.x1+197,461+g_iCorrectBI_Y/2,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
	s_bmMiddleWindowButton.setClickMargin(IDOK,1,1);
	s_bmMiddleWindowButton.setAllBarTextColor(0,FALSE);

	SetInventoryWindowPos();
	InitPopupWindowCloseButton();
}



//
//	은행 닫기
void
CBankWindow::close()
{
	if (m_wIsWaitSaveResult					)
		return;

	if (isOpened())
	{
		g_bank.cancelTransaction();
		s_agent.sendCancelBankTransaction();
	}

	cINPDEV::ReleaseExclusive();

	CloseInventoryWindow();
	s_iPopupInterface	=	ePIW_NONE;
	m_iDragBankItem		=	0xffff;
	s_bmMiddleWindowButton.reset();
}

//
//	은행 거래창에 포커스가 간 아이템
int
CBankWindow::getFocusBankItemSlot()
{
	int	i;
	int	iX	=	m_rect.x1+12;
	int	iY	=	m_rect.y1+dBANK_WINDOW_TITLE_HEIGHT;

	for (i = 0;i<dBANK_WINDOW_DISPLAY_ITEM_COUNT;i++)
	{
		int	iIndex	=	i+s_sbMiddleWindow.m_iCurrentPos*dITEM_HORIZON_COUNT_OF_THE_BANK;

		if (iIndex	>=	g_bank.m_wBankSize)	break;

		cRECT	rect;
		CPos	pos;

		pos.x	=	iX+(i%dITEM_HORIZON_COUNT_OF_THE_BANK)*dITEM_SLOT_WIDTH_OF_THE_BANK;
		pos.y	=	iY+(i/dITEM_HORIZON_COUNT_OF_THE_BANK)*dITEM_SLOT_HEIGHT_OF_THE_BANK;

		rect.set(pos.x,pos.y,pos.x+dITEM_SHAPE_WIDTH,pos.y+dITEM_SHAPE_HEIGHT);

		if (!rect.isIn(s_posMouse.x,s_posMouse.y))	continue;

		s_rectTooltip.set(&rect);
		s_rectTooltip.add(0,40);

		return	iIndex;
	}

	int		iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);

	cRECT	rect;

	rect.set(m_rect.x1+14,m_rect.y1+iWindowHeight-42,0,0);
	rect.x2	=	rect.x1+27;
	rect.y2	=	rect.y1+27;

	if (rect.isIn(s_posMouse.x,s_posMouse.y))
		return	dITEM_DRAG_GOLD;


	return	0xffff;
}

//
//	은행창 아이템 드래그 처리
void 
CBankWindow::operateDragItem()
{
	m_iFocusBankSlot	=	getFocusBankItemSlot();

	if (m_iDragBankItem	==	0xffff && m_iFocusBankSlot	==	0xffff)
		return;

	if (cINPDEV::IsExclusive(eMOUSE_STATE_BANK_ITEM_DRAG))
		return;

	if (s_isTouchLeftButton && m_iDragBankItem == 0xffff)
	{
		if (m_iFocusBankSlot	==	dITEM_DRAG_GOLD)
		{
			m_iDragBankItem		=	m_iFocusBankSlot;
			cINPDEV::SetExclusive(eMOUSE_STATE_BANK_ITEM_DRAG);
		}

		cItem	*lpItem	=	g_bank.getBankItem(m_iFocusBankSlot);

		if (lpItem && lpItem->m_wBaseItem	!=	0xffff)
		{
			m_iDragBankItem = m_iFocusBankSlot;
			cINPDEV::SetExclusive(eMOUSE_STATE_BANK_ITEM_DRAG);
		}
	}

	if (s_isLeftButtonPressed || m_iDragBankItem == 0xffff)
		return;

	int	iItemSlot	=	GetFocusItemSlot();
	int	iDroppingArea=	GetItemDroppingArea();

	if (iDroppingArea==	eDROPPING_ITEM_AREA_INVENTORY_WHEN_BANK_TRANSACTION)
		withdrawItemFromTheBank();
	if (m_iFocusBankSlot		!=	0xffff && m_iFocusBankSlot != m_iDragBankItem		)
		moveBankItem();

	cINPDEV::ReleaseExclusive();

	m_iDragBankItem	=	0xffff;
}

//
//	업데이트
void
CBankWindow::update()
{
	if	(s_iPopupInterface	!=	ePIW_BANK	)
		return;
	if	(m_wIsWaitSaveResult					)
		return;

	if	(!g_bank.isOpened())
	{
		close();
		return;
	}

	if (!cINPDEV::IsExclusive(eMOUSE_STATE_SHOP_SCROLL_BAR))
	{
		if (s_sbMiddleWindow.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed))
			
			cINPDEV::SetExclusive(eMOUSE_STATE_SHOP_SCROLL_BAR);
		else
			cINPDEV::ReleaseExclusive(TRUE,eMOUSE_STATE_SHOP_SCROLL_BAR);
	}

	if (m_rect.isIn(s_posMouse.x,s_posMouse.y))
	{
		if (s_iWheelValue<0)
			s_sbMiddleWindow.setPos(s_sbMiddleWindow.m_iCurrentPos+1);
		if (s_iWheelValue>0)
			s_sbMiddleWindow.setPos(s_sbMiddleWindow.m_iCurrentPos-1);
	}

	OperateInventoryWindow();
	OperateDragItem();	//	인벤토리 창에 드래그 아이템 처리

	operateDragItem();

	if (m_iFocusBankSlot	!=	0xffff && m_iDragBankItem == 0xffff)
	{
		if (m_iFocusBankSlot	!=	dITEM_DRAG_GOLD)
		{
			cItem	*lpItem		=	g_bank.getBankItem(m_iFocusBankSlot);

			if (lpItem)
			{
				char	*strComment	=	lpItem->getItemTooltip(-1,-1,TRUE);

				strComment			=	g_hero.getAfterEquipInfo(strComment,lpItem);

				s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,strComment);
			}
		}
	}

	if (cINPDEV::IsExclusive(eMOUSE_STATE_BANK_WINDOW_BUTTONS))	return;

	DWORD	dwMenu	=	s_bmMiddleWindowButton.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);

	if (s_bmMiddleWindowButton.isPressed())
			cINPDEV::SetExclusive(eMOUSE_STATE_BANK_WINDOW_BUTTONS);
	else	cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_BANK_WINDOW_BUTTONS);

	if (dwMenu	==	IDOK)
	{
		DWORD	dwCharge=	g_bank.getBankTransactionCharge();

		if	(dwCharge	==	0)
		{
			m_wIsWaitSaveResult	=	TRUE;
			s_agent.sendFinishBankTransaction(g_bank.m_wBankSerial,g_bank.getCheckSum());
		}
		else
		{
			if ((int )dwCharge	>	g_bank.m_iOwnGold)
				g_msgBox.cPopup(NULL,_ms(dMSG_BANK_TRANSACTION_LACK_CHARGE_FORM,dwCharge),dMSG_OK);
			else
			{
				g_msgBox.cPopup(NULL,_ms(dMSG_BANK_TRANSACTION_CHARGE_FORM,dwCharge),dMSG_YES,dMSG_NO);
				g_msgBox.setButtonId(eMBI_BANK_TRANSACTION_PERMIT);
			}
		}
	}
}

//
//	스테이터스 윈도우 그려주기
void
CBankWindow::draw()
{
	if (s_iPopupInterface	!=	ePIW_BANK)
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

	g_sprInterface.Put(iX	,g_iCorrectBI_Y/2		,eWINDOW_TITLE_BANK);
	g_sprInterface.Put(iX	,iY		,eWINDOW);

	iX		=	m_rect.x1+12;
	iY		=	m_rect.y1+dBANK_WINDOW_TITLE_HEIGHT;

	for (i = 0;i<dBANK_WINDOW_DISPLAY_ITEM_COUNT;i++)
	{
		int		iItemIndex	=	i+s_sbMiddleWindow.m_iCurrentPos*dITEM_HORIZON_COUNT_OF_THE_BANK;

		pos.x	=	iX+(i%dITEM_HORIZON_COUNT_OF_THE_BANK)*dITEM_SLOT_WIDTH_OF_THE_BANK;
		pos.y	=	iY+(i/dITEM_HORIZON_COUNT_OF_THE_BANK)*dITEM_SLOT_HEIGHT_OF_THE_BANK;

		g_sprInterface.Put(pos.x,pos.y,eIWI_ITEM_SLOT_FRAME);

		if (iItemIndex >= g_bank.m_wBankSize)
		{
			g_sprInterface.Put(pos.x,pos.y,eIWI_BLOCKED_ITEM_SLOT);
			continue;
		}

		cItem	*lpItem		=	g_bank.getBankItem(iItemIndex);

		if (!lpItem)
			continue;

		PutItem(lpItem,pos.x,pos.y,TRUE);
	}

	if (s_iDragItem != 0xffff || m_iDragBankItem != 0xffff)
	{
		if (m_iFocusBankSlot !=	0xffff && s_iDragItem != dITEM_DRAG_GOLD && m_iDragBankItem != dITEM_DRAG_GOLD)
		{
			int	iIndex	=	m_iFocusBankSlot-s_sbMiddleWindow.m_iCurrentPos*dITEM_HORIZON_COUNT_OF_THE_BANK;

			pos.x	=	iX+(iIndex%dITEM_HORIZON_COUNT_OF_THE_BANK)*dITEM_SLOT_WIDTH_OF_THE_BANK;
			pos.y	=	iY+(iIndex/dITEM_HORIZON_COUNT_OF_THE_BANK)*dITEM_SLOT_HEIGHT_OF_THE_BANK;

			g_sprInterface.Put(pos.x,pos.y,eIWI_ITEM_SLOT_ANM_1+((s_iFrameCounter/8)%5));
		}
	}

	s_sbMiddleWindow.draw();
	g_sprInterface.Put(m_rect.x1+14,m_rect.y1+iWindowHeight-42,eBW_GOLD_SLOT);

	DWORD	dwGold	=	g_bank.m_bankInfo.m_dwGold;

	s_text.put(m_rect.x1+50,m_rect.y1+iWindowHeight-34,WHITE,"%s Gold",GetMoneyString(dwGold));

	s_bmMiddleWindowButton.draw();
}

//
//	은행거래 취소 - 뭔가 문제가 있다.
void
CBankWindow::cancelBankTransaction()
{
	m_wIsWaitSaveResult	=	FALSE;

	CGamePlay::AddSystemMessage(WHITE,dMSG_CANCELED_BY_NOT_MATCHED_DATA_WITH_SERVER);

	g_bank.cancelTransaction();
	s_agent.sendCancelBankTransaction();
	close();
}

//
//	아이템을 은행에 보관
void
CBankWindow::storeItemToTheBank(int _iItemSlot)
{
	if (_iItemSlot == dITEM_DRAG_GOLD)
	{
		cItem	item;

		item.m_wBaseItem	=	dITEM_MONEY;
		item.m_dwSerial		=	g_bank.m_iOwnGold;

		g_itemCountWindow.popup(s_posMouse.x,s_posMouse.y,0,&item,eICWR_RETURN_STORE_GOLD_TO_THE_BANK);

		return;
	}

	if(g_bank.storeItemToTheBank(_iItemSlot,m_iFocusBankSlot,0,TRUE) == FALSE)
		return;

	s_agent.sendStoreItemToTheBank(g_bank.m_wBankSerial,_iItemSlot,m_iFocusBankSlot,g_bank.getCheckSum());
}

//
//	아이템을 은행에서 찾아오기
void
CBankWindow::withdrawItemFromTheBank()
{
	int	iItemSlot	=	GetFocusItemSlot();

	if (m_iDragBankItem ==	dITEM_DRAG_GOLD)
	{
		cItem	item;

		item.m_wBaseItem	=	dITEM_MONEY;
		item.m_dwSerial		=	g_bank.m_bankInfo.m_dwGold;

		g_itemCountWindow.popup(s_posMouse.x,s_posMouse.y,0,&item,eICWR_RETURN_WITHDRAW_GOLD_FROM_THE_BANK);

		return;
	}

	if (iItemSlot	==	0xffff)
		return;

	if(g_bank.withdrawItemFromTheBank(iItemSlot,m_iDragBankItem,0,TRUE) == FALSE)
		return;

	s_agent.sendWithdrawItemFromTheBank(g_bank.m_wBankSerial,iItemSlot,m_iDragBankItem,g_bank.getCheckSum());
}

//
//	은행에 돈 보관
void
CBankWindow::storeGoldToTheBank(int _iGold)
{
	if(g_bank.storeGoldToTheBank(_iGold,0,TRUE) == FALSE)
		return;

	s_agent.sendStoreGoldToTheBank(g_bank.m_wBankSerial,_iGold,g_bank.getCheckSum());
}

//
//	은행에서 돈 찾기
void
CBankWindow::withdrawGoldFromTheBank(int _iGold)
{
	if(g_bank.withdrawGoldFromTheBank(_iGold,0,TRUE) == FALSE)
		return;

	s_agent.sendWithdrawGoldFromTheBank(g_bank.m_wBankSerial,_iGold,g_bank.getCheckSum());
}

//
//	아이템 이동
void
CBankWindow::moveBankItem()
{
	if (g_bank.moveItemInTheBank(m_iDragBankItem,m_iFocusBankSlot,0,TRUE) == FALSE)
		return;

	s_agent.sendMoveBankItem(g_bank.m_wBankSerial,m_iDragBankItem,m_iFocusBankSlot,g_bank.getCheckSum());
}

//
//	아이템 이동
void
CBankWindow::moveInventoryItemWhenBankTransaction()
{
	int	iItemSlot	=	GetFocusItemSlot();

	if (g_bank.moveItemInInventory(s_iDragItem,iItemSlot,0,TRUE) == FALSE)
		return;

	s_agent.sendMoveInventoryItemWhenBankTransaction(g_bank.m_wBankSerial,s_iDragItem,iItemSlot,g_bank.getCheckSum());
}

//
//	은행이 열려 있다
BOOL
CBankWindow::isOpened()
{
	if (g_bank.m_wBankSerial	!=	0xffff)
		return	TRUE;

	return	FALSE;
}