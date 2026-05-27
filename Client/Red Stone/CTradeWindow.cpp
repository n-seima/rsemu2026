#include "CGamePlay.H"
#include "CHero.H"
#include "CActor.H"
#include "CGamePlayInterface.H"
#include "CMessage.H"
#include "CLog.H"

CTradeManager		CGamePlay::s_tradeManager;
char				CGamePlay::s_strRequestTradeAskerName[dNAME_LENGTH];
cRECT				CGamePlay::s_rectTradeBox;
int					CGamePlay::s_iTradeItemSlot			=	0xffff;
int					CGamePlay::s_iDragTradeItem			=	0xffff;
int					CGamePlay::s_iChangeTradeItemSlot	=	0xffff;
int					CGamePlay::s_iSetGoldValueCount		=	0;
int					CGamePlay::s_iMateLevel				=	0;

#define	dBORDER_OF_TRADEBOX	100	//	상대방과 나를 구분짓는 경계
//
//	Trade Window Button
enum
{
	eTWB_TRADE,
	eTWB_CANCEL,
	eTWB_READY,
	eTWB_MATE_READY,
};

//
//	트레이드창 열기
void
CGamePlay::OpenTradeBox(CActor *_lpMate,int _iTradeBoxSerial,int _iTargetLevel)
{
	s_iSetGoldValueCount			=	0;
	s_iMateLevel					=	_iTargetLevel;
	s_lpInventoryWindowEquipmentList=	g_hero.m_aEquip;
	s_lpInventoryWindowItemList		=	g_hero.m_aItems;
	s_lpGold						=	&g_hero.m_iGold;

	g_hero.lockOff();
	cINPDEV::ReleaseExclusive();
	ResetAllFocus();
	ClosePopupWindow();

	int	iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iTitleHeight	=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_TRADE);

	s_iPopupInterface	=	ePIW_TRADE_BOX;
	s_rectTradeBox.x1	=	g_iScreenWidth-iWindowWidth*2;
	s_rectTradeBox.y1	=	iTitleHeight+g_iCorrectBI_Y/2;
	s_rectTradeBox.x2	=	g_iScreenWidth-iWindowWidth;
	s_rectTradeBox.y2	=	iTitleHeight+iWindowHeight+g_iCorrectBI_Y/2;
	s_iDragTradeItem	=	0xffff;

	s_tradeManager.beginTrade(_lpMate,_iTradeBoxSerial);

	SetInventoryWindowPos();

	int	iX		=	s_rectTradeBox.x1;
	int	iY		=	s_rectTradeBox.y1;

	s_bmMiddleWindowButton.reset();

	s_bmMiddleWindowButton.addImageBar(dMSG_TM_TRADE_PERMIT	,eTWB_TRADE		,iX+45	,444+g_iCorrectBI_Y/2,eTWI_PERMIT_NORMAL,eTWI_PERMIT_ACTIVE,eTWI_PERMIT_PRESSED,0xffff,eTWI_PERMIT_DISABLE);
	s_bmMiddleWindowButton.addImageBar(dMSG_TM_TRADE_CANCEL	,eTWB_CANCEL	,iX+171	,444+g_iCorrectBI_Y/2,eTWI_CANCEL_NORMAL,eTWI_CANCEL_ACTIVE,eTWI_CANCEL_PRESSED);

	s_bmMiddleWindowButton.addImageBar(dMSG_READY_TRADE	,eTWB_MATE_READY	,iX+15	,401+g_iCorrectBI_Y/2,eTWI_READY_NORMAL,eTWI_READY_ACTIVE,eTWI_READY_PRESSED,0xffff,eTWI_READY_NORMAL);
	s_bmMiddleWindowButton.addImageBar(dMSG_READY_TRADE	,eTWB_READY			,iX+142	,401+g_iCorrectBI_Y/2,eTWI_READY_NORMAL,eTWI_READY_ACTIVE,eTWI_READY_PRESSED,0xffff,eTWI_READY_NORMAL);

	int	iReadyButtonWidth	=	g_sprInterface.getSpriteWidth(eTWI_READY_NORMAL);
	int	iReadyButtonHeight	=	g_sprInterface.getSpriteHeight(eTWI_READY_NORMAL);

	s_bmMiddleWindowButton.setTextPos(eTWB_TRADE		,1,1);
	s_bmMiddleWindowButton.setTextPos(eTWB_CANCEL		,1,1);
	s_bmMiddleWindowButton.setTextPos(eTWB_READY		,iReadyButtonWidth+2,iReadyButtonHeight/2-6);
	s_bmMiddleWindowButton.setTextPos(eTWB_MATE_READY	,iReadyButtonWidth+2,iReadyButtonHeight/2-6);

	s_bmMiddleWindowButton.disable(eTWB_MATE_READY);
	InitPopupWindowCloseButton();
}

//
//	상점 닫기
void
CGamePlay::CloseTradeBox()
{
	if	(IsOpenTradeBox())
		s_agent.sendCancelTrade();

	cINPDEV::ReleaseExclusive();

	s_tradeManager.reset();
	CloseInventoryWindow();
	s_iPopupInterface	=	ePIW_NONE;
}

//
//	그리기

void
CGamePlay::DrawTradeBox()
{
	if	(s_iPopupInterface	!=	ePIW_TRADE_BOX)
		return;

	DrawInventoryWindow();

	int		iX,iY,iCount	=	0,i;
	int		iLineCount		=	0;
	int		iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iTradeFrameWidth=	g_sprInterface.getSpriteWidth(eTWI_FRAME);

	iX		=	s_rectTradeBox.x1;
	iY		=	s_rectTradeBox.y1;

	if (g_config.m_bIsInfoWindowHalfblending)	cDRAW::FillHB(0,iX+8,iY+6,iX+266-4,iY+482-8);
	else										cDRAW::Fill  (0,iX+8,iY+6,iX+266-4,iY+482-8);

	g_sprInterface.Put(iX+7					,iY+50		,eTWI_FRAME);
	g_sprInterface.Put(iX+6+iTradeFrameWidth,iY+50		,eTWI_FRAME);
	g_sprInterface.Put(iX	,g_iCorrectBI_Y/2		,eWINDOW_TITLE_TRADE);

	s_text2.put(iX+44 ,140+g_iCorrectBI_Y/2,LTYELLOW,"%s Gold",GetMoneyString(s_tradeManager.m_mateTradeBox.m_iGold));
	s_text2.put(iX+172,140+g_iCorrectBI_Y/2,LTYELLOW,"%s Gold",GetMoneyString(s_tradeManager.m_myTradeBox.m_iGold));

	{
		int	iPermitWidth		=	s_text2.getPixelSize(dMSG_TM_TRADE_PERMIT);
		int	iCancelWidth		=	s_text2.getPixelSize(dMSG_TM_TRADE_CANCEL);
		int	iTradeButtonWidth	=	g_sprInterface.getSpriteWidth(eTWI_PERMIT_NORMAL);
		int	iTradeButtonHeight	=	g_sprInterface.getSpriteHeight(eTWI_PERMIT_NORMAL);

		s_bmMiddleWindowButton.setTextPos(eTWB_TRADE		,iTradeButtonWidth/2-iPermitWidth/2,iTradeButtonHeight+3+g_iCorrectBI_Y/2);
		s_bmMiddleWindowButton.setTextPos(eTWB_CANCEL	,iTradeButtonWidth/2-iCancelWidth/2,iTradeButtonHeight+3+g_iCorrectBI_Y/2);
	}

	s_bmMiddleWindowButton.draw();

	int	iSlotWidth	=	39;
	int	iSlotHeight	=	38;

	for (i=0;i<dTRADE_ITEM_COUNT;i++)
	{
		CPos	pos;

		pos.x	=	iX+15+i%3*iSlotWidth;
		pos.y	=	167+i/3*iSlotHeight +g_iCorrectBI_Y/2;

		cItem	*lpItem	=	&s_tradeManager.m_mateTradeBox.m_aItems[i];

		if (lpItem->m_wBaseItem	!=	0xffff)	
				PutItem(lpItem,pos.x,pos.y,TRUE);
		else	g_sprInterface.Put(pos.x,pos.y,eIWI_BLOCKED_ITEM_SLOT);
	}

	for (i=0;i<dTRADE_ITEM_COUNT;i++)
	{
		CPos	pos;

		pos.x	=	iX+142+i%3*iSlotWidth;
		pos.y	=	167+i/3*iSlotHeight+g_iCorrectBI_Y/2;

		int	iItemSlot	=	s_tradeManager.m_myTradeBox.m_aItems[i].m_wBaseItem;

		if (iItemSlot	==	0xffff)	continue;

		cItem	*lpItem	=	g_hero.getInventoryItem(iItemSlot,TRUE);

		if (lpItem	==	NULL)	continue;

		cItem	item;

		item.copy(lpItem);
		if	(lpItem->isExceptionItem()	==	FALSE)
			item.m_bCount	=	s_tradeManager.m_myTradeBox.m_aItems[i].m_bCount;

		PutItem(&item,pos.x,pos.y,TRUE);
	}

	g_sprInterface.Put(iX	,iY		,eWINDOW);

//	거래창 상태 설명
	if (s_tradeManager.isReady())
	{
		if (s_tradeManager.m_myTradeBox.m_wStatus	==	eTS_PERMIT)	//	나는 승낙했고 상대방이 승락하기를 기다리고 있다.
		{
			s_text2.cPut(iX+10,26+g_iCorrectBI_Y/2,250,40,0,WHITE,dFITTEXT_HEIGHT_CENTER,dMSG_TM_WAIT_PERMIT_FORM,s_tradeManager.m_mateTradeBox.m_strName);
			s_bmMiddleWindowButton.disable(eTWB_TRADE);
		}
		else
		if (s_tradeManager.m_mateTradeBox.m_wStatus	==	eTS_PERMIT)	//	상대방은 승인했고 내가 승락하기를 기다리고 있다.
		{
			s_text2.cPut(iX+10,26+g_iCorrectBI_Y/2,250,40,0,WHITE,dFITTEXT_HEIGHT_CENTER,dMSG_TM_PLEASE_PERMIT_FORM,s_tradeManager.m_myTradeBox.m_strName);
			s_bmMiddleWindowButton.enable(eTWB_TRADE);
		}
		else
		{
			s_text2.cPut(iX+10,26+g_iCorrectBI_Y/2,250,40,0,WHITE,dFITTEXT_HEIGHT_CENTER,dMSG_TRADE_BOX_CONFIRM_MESSAGE,s_tradeManager.m_mateTradeBox.m_strName);
			s_bmMiddleWindowButton.enable(eTWB_TRADE);
		}
	}
	else
	{
		s_text2.cPut(iX+10,26+g_iCorrectBI_Y/2,250,40,0,WHITE,dFITTEXT_HEIGHT_CENTER,dMSG_TRADE_BOX_COMMENT_FORM,s_tradeManager.m_mateTradeBox.m_strName);
		s_bmMiddleWindowButton.disable(eTWB_TRADE);
	}

//	거래창 주인 표시
	{
		if (g_hero.isFriend(s_tradeManager.m_mateTradeBox.m_strName))
				s_text2.cPut(iX+14,75+g_iCorrectBI_Y/2,114,44,0	,WHITE,dFITTEXT_HEIGHT_CENTER,dMSG_TRADE_WINDOW_FORM,dMSG_FRIEND_ICON,s_tradeManager.m_mateTradeBox.m_strName);
		else	s_text2.cPut(iX+14,75+g_iCorrectBI_Y/2,114,44,0	,WHITE,dFITTEXT_HEIGHT_CENTER,dMSG_TRADE_WINDOW_FORM,"",s_tradeManager.m_mateTradeBox.m_strName);

		s_text2.cPut(iX+140,75+g_iCorrectBI_Y/2,114,44,0	,WHITE,dFITTEXT_HEIGHT_CENTER,dMSG_TRADE_WINDOW_FORM,"",s_tradeManager.m_myTradeBox.m_strName);
	}

//	거래 준비 버튼 에니메이션
	{
		if (s_tradeManager.m_myTradeBox.m_wStatus	>=	eTS_READY)
		{
			int	iFrameCount	=	eTWI_LOCKED_FRAME7-eTWI_LOCKED_FRAME1+1;
			int	iFrame		=	s_iFrameCounter*8/dSYNC_FPS%iFrameCount+eTWI_LOCKED_FRAME1;

			g_sprInterface.Put(iX+142	,401+g_iCorrectBI_Y/2,iFrame);
			s_bmMiddleWindowButton.disable(eTWB_READY);
		}
		else	s_bmMiddleWindowButton.enable(eTWB_READY);

		if (s_tradeManager.m_mateTradeBox.m_wStatus	>=	eTS_READY)
		{
			int	iFrameCount	=	eTWI_LOCKED_FRAME7-eTWI_LOCKED_FRAME1+1;
			int	iFrame		=	(s_iFrameCounter+16)*8/dSYNC_FPS%iFrameCount+eTWI_LOCKED_FRAME1;

			g_sprInterface.Put(iX+15	,401+g_iCorrectBI_Y/2,iFrame);
		}
	}
}

//
//	거래창에 포커스가 간 아이템

int
CGamePlay::GetFocusItemInTradeWindow()
{
	int	i;
	int	iSlotWidth	=	39;
	int	iSlotHeight	=	38;
	
	int	iX			=	s_rectTradeBox.x1;
	int	iY			=	s_rectTradeBox.y1;

	for (i=0;i<dTRADE_ITEM_COUNT;i++)
	{
		CPos	pos;

		pos.x	=	iX+15+i%3*iSlotWidth;
		pos.y	=	167+i/3*iSlotHeight+g_iCorrectBI_Y/2;

		cItem	*lpItem	=	&s_tradeManager.m_mateTradeBox.m_aItems[i];

		if (lpItem->m_wBaseItem	==	0xffff)	continue;
		cRECT	rect;

		rect.set(pos.x,pos.y,pos.x+dITEM_SHAPE_WIDTH,pos.y+dITEM_SHAPE_HEIGHT);

		if (rect.isIn(s_posMouse.x,s_posMouse.y))
		{
			s_rectTooltip.set(&rect);
			return	i+dBORDER_OF_TRADEBOX;
		}
	}

	for (i=0;i<dTRADE_ITEM_COUNT;i++)
	{
		CPos	pos;

		pos.x	=	iX+142+i%3*iSlotWidth;
		pos.y	=	167+i/3*iSlotHeight+g_iCorrectBI_Y/2;

		int	iItemSlot	=	s_tradeManager.m_myTradeBox.m_aItems[i].m_wBaseItem;

		if (iItemSlot	==	0xffff)	continue;

		cItem	*lpItem	=	g_hero.getInventoryItem(iItemSlot,TRUE);

		if (lpItem	==	NULL)	continue;

		cRECT	rect;

		rect.set(pos.x,pos.y,pos.x+dITEM_SHAPE_WIDTH,pos.y+dITEM_SHAPE_HEIGHT);

		if (rect.isIn(s_posMouse.x,s_posMouse.y))
		{
			s_rectTooltip.set(&rect);

			return	i;
		}
	}

	return	0xffff;
}


//
//	아이템 드래그 처리
void 
CGamePlay::OperateDragItemInTradeWindow()
{
	int	iFocusItem	=	GetFocusItemInTradeWindow();

	if (s_iDragTradeItem	==	0xffff)
		if (iFocusItem		>=	dBORDER_OF_TRADEBOX)
			return;

	if (cINPDEV::IsExclusive(eMOUSE_STATE_TRADE_ITEM_DRAG))
		return;

	if (s_isTouchLeftButton	&&	s_iDragTradeItem==	0xffff)
	{
		s_iDragTradeItem	=	iFocusItem;

		cINPDEV::SetExclusive(eMOUSE_STATE_TRADE_ITEM_DRAG);
	}

	if (s_isLeftButtonPressed	||	s_iDragTradeItem	==	0xffff	)
		return;

	int		iX	=	s_posInventoryWindow.x;
	int		iY	=	s_posInventoryWindow.y;

	cRECT	rectFrame;

//	아이템 창
	rectFrame.set(iX+18,iY,iX+252,iY+468);

	if (rectFrame.isIn(s_posMouse.x,s_posMouse.y))
		s_agent.sendRemoveTradeItem(s_iDragTradeItem);

	cINPDEV::ReleaseExclusive();

	s_iDragTradeItem	=	0xffff;
}

//
//	업데이트
void
CGamePlay::UpdateTradeBox()
{
	if (s_iPopupInterface	!=	ePIW_TRADE_BOX)	return;
	if (!IsOpenTradeBox())	return;

	DWORD	dwMenu	=	s_bmMiddleWindowButton.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);

	if (dwMenu	!=	0xffffffff)
	{
		switch(dwMenu)
		{
			case	eTWB_TRADE		:
			{
				char	strText[256];
				sprintf(strText,dMSG_TRADE_WARNING_FORM,s_tradeManager.m_mateTradeBox.m_strName);//,s_iMateLevel);

				g_msgBox.cPopup("",strText,dMSG_YES,dMSG_NO);
				g_msgBox.setButtonId(eMBI_PERMIT_TRADE);
				g_msgBox.setTextDC(&s_textForMsgbox);
				break;
			}

			case	eTWB_CANCEL		:
				s_agent.sendCancelTrade();
				break;

			case	eTWB_READY		:
				s_agent.sendReadyTrade();
				break;
		}
	}

	if (s_bmMiddleWindowButton.isPressed())
	{
		cINPDEV::SetExclusive(eMOUSE_STATE_EXCLUSIVE_BY_TRADE_BOX);
		return;
	}
	else	cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_EXCLUSIVE_BY_TRADE_BOX);

	OperateInventoryWindow();
	OperateDragItem();	//	드래그 아이템 처리
	OperateDragItemInTradeWindow();

	if (cINPDEV::IsExclusive())	return;

	int	iFocusItem	=	GetFocusItemInTradeWindow();

	if (iFocusItem	==	0xffff)	return;

	if (iFocusItem	>=	dBORDER_OF_TRADEBOX)
	{
		iFocusItem	-=	dBORDER_OF_TRADEBOX;

		cItem	*lpItem		=	&s_tradeManager.m_mateTradeBox.m_aItems[iFocusItem];

		char	*strComment	=	lpItem->getItemTooltip(-1,-1,TRUE);

		strComment			=	g_hero.getAfterEquipInfo(strComment,lpItem);

		s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text2,WHITE,strComment);
		return;
	}

	int	iInventorySlot	=	s_tradeManager.m_myTradeBox.m_aItems[iFocusItem].m_wBaseItem;

	cItem	*lpItem		=	g_hero.getInventoryItem(iInventorySlot,TRUE);

	if (lpItem)
	{
		char	*strComment	=	lpItem->getItemTooltip(-1,-1,TRUE);

		strComment			=	g_hero.getAfterEquipInfo(strComment,lpItem);

		s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text2,WHITE,strComment);
	}
}
//
//	트레이드 아이템 추가할 것을 요청한다.
void
CGamePlay::AskAddTradeItem(int _iInventorySlot)
{
	int	iWork	=	eICWR_RETURN_TRADE_ITEM_COUNT;

	if (_iInventorySlot	==	dITEM_DRAG_GOLD)
	{
		if	(s_iSetGoldValueCount	>=	1)
			return;

		if	(g_hero.m_iGold			<=	0)
			return;

		cItem	item;

		item.m_wBaseItem	=	dITEM_MONEY;
		item.m_dwSerial		=	g_hero.m_iGold;

		g_itemCountWindow.popup(s_posMouse.x,s_posMouse.y,_iInventorySlot,&item,eICWR_RETURN_TRADE_GOLD);

		cINPDEV::SetExclusive(eMOUSE_STATE_GET_ITEM_COUNT);
		return;
	}

	cItem	*lpItem	=	g_hero.getInventoryItem(_iInventorySlot);

	if	(!lpItem)
	{
		lpItem					=	g_hero.getInventoryItem(_iInventorySlot,TRUE);

		if	(lpItem	==	NULL)
			return;

		if	(lpItem->isExceptionItem()	==	FALSE)
			if	(lpItem->m_bCount	<	1)
				return;

		iWork					=	eICWR_RETURN_CHANGE_TRADE_ITEM_COUNT;
		s_iChangeTradeItemSlot	=	s_tradeManager.getTradeItemSlot(_iInventorySlot);
	}
	
	if	(lpItem->isCanNotTradeItem())
	{
		g_lpHero->warning(dMSG_CAN_NOT_TRADE_ITEM);
		return;
	}

	if	(lpItem->m_bCount	>	1	&&	lpItem->isExtraItem()	==	FALSE	&&	lpItem->isExceptionItem()	==	FALSE)
	{
		g_itemCountWindow.popup(s_posMouse.x,s_posMouse.y,_iInventorySlot,lpItem,iWork);

		s_iTradeItemSlot	=	_iInventorySlot;

		cINPDEV::SetExclusive(eMOUSE_STATE_GET_ITEM_COUNT);
	}
	else
		s_agent.sendAddTradeItem(_iInventorySlot,1);
}

//
//	트레이드 요청을 받았다.
void
CGamePlay::RequestTrade(SG_REQUEST_TRADE *_lpPacket)
{
	if (IsOpenedEventWindow())
		return;

	char	strMessage[256];

	strcpy(s_strRequestTradeAskerName,_lpPacket->strAsker);
	sprintf(strMessage,dMSG_REQUEST_TRADE_FORM,s_strRequestTradeAskerName);

	g_msgBox.cPopup(dMSG_RECEIVE_TRADE_REQUEST,strMessage,dMSG_YES,dMSG_NO);
	g_msgBox.setButtonId(eMBI_ACCEPT_TRADE_REQUEST,eMBI_DECLINE_TRADE_REQUEST);
}

//
//	트레이드 아이템 추가
void
CGamePlay::AddTradeItemByMe(int _iSlot,int _iCount)
{
	if (!IsOpenTradeBox())
	{
		s_agent.sendCancelTrade();
		return;
	}

	cItem	item;
	cItem	*lpItem	=	g_hero.getInventoryItem(_iSlot);

	if (!lpItem)
	{
		s_agent.sendCancelTrade();
		return;
	}

	item.copy(lpItem);
	item.m_wBaseItem=	_iSlot;
	if	(lpItem->isExceptionItem()	==	FALSE)
		item.m_bCount	=	_iCount;

	if (!s_tradeManager.addMyItem(&item))
	{
		s_agent.sendCancelTrade();
		return;
	}
}

//
//	거래 아이템 추가
void
CGamePlay::AddTradeItemByTradeMate(cItem *_lpItem)
{
	if	(!IsOpenTradeBox())
	{
		s_agent.sendCancelTrade();
		return;
	}

	if	(!s_tradeManager.addMateItem(_lpItem))
		s_agent.sendCancelTrade();
}

void
CGamePlay::ChangeTradeItemDataByMe(int _iTradeSlot,int _iSlot,int _iCount)
{
	if (!IsOpenTradeBox())
	{
		s_agent.sendCancelTrade();
		return;
	}

	cItem	item;
	cItem	*lpItem	=	g_hero.getInventoryItem(_iSlot,TRUE);

	if (!lpItem)
	{
		s_agent.sendCancelTrade();
		return;
	}

	item.copy(lpItem);
	item.m_wBaseItem=	_iSlot;

	if	(lpItem->isExceptionItem()	==	FALSE)
		item.m_bCount	=	_iCount;

	if (!s_tradeManager.changeTradeItemDataByMe(_iTradeSlot,&item))
	{
		s_agent.sendCancelTrade();
		return;
	}
}

void
CGamePlay::ChangeTradeItemDataByTradeMate(int _iTradeSlot,cItem *_lpItem)
{
	if (!IsOpenTradeBox())
	{
		s_agent.sendCancelTrade();
		return;
	}

	if (!s_tradeManager.changeTradeItemDataByMate(_iTradeSlot,_lpItem))
		s_agent.sendCancelTrade();

	s_iSetGoldValueCount++;
}

//
//	거래금 설정
void
CGamePlay::SetTradeGold(int _iTrader,int _iGold)
{
	if	(!IsOpenTradeBox())
	{
		s_agent.sendCancelTrade();
		return;
	}

	if	(!s_tradeManager.setGold(_iTrader,_iGold))
		return;


}

//
//	 거래 아이템 제거
void
CGamePlay::RemoveTradeItem(int _iTrader,int _iTradeBoxSlot)
{
	if (!IsOpenTradeBox())
	{
		s_agent.sendCancelTrade();
		return;
	}

	if (!s_tradeManager.removeItem(_iTrader,_iTradeBoxSlot))
		s_agent.sendCancelTrade();
}

//
//	트레이드 준비
void
CGamePlay::ReadyTrade(int _iTrader)
{
	if (!IsOpenTradeBox())
	{
		s_agent.sendCancelTrade();
		return;
	}

	if (!s_tradeManager.readyTrade(_iTrader))
		s_agent.sendCancelTrade();
}

//
//	트레이드 수락
void
CGamePlay::PermitTrade(int _iTrader)
{
	if	(!IsOpenTradeBox())
	{
		s_agent.sendCancelTrade();
		return;
	}

	if	(!s_tradeManager.permitTrade(_iTrader))
		s_agent.sendCancelTrade();
}

//
//	거래아이템을 비운다.
BOOL
CGamePlay::EmptyTradeItem(SG_TRADE *_lpPacket)
{
	for (int i=0;i<dTRADE_ITEM_COUNT;i++)
	{
		int	iSlot	=	_lpPacket->aGiveItems[i].m_wItemSlot;

		if (iSlot	==	0xffff)	break;

		cItem	*lpItem	=	g_hero.getInventoryItem(iSlot,TRUE);

		if (!lpItem)
		{
			s_agent.sendAskItemData();
			return	FALSE;
		}

		lpItem->decreaseCount(_lpPacket->aGiveItems[i].m_wTradeCount);
	}

	return	TRUE;
}

//
//	거래 아이템을 받는다.
BOOL
CGamePlay::ReceiveTradeItem(SG_TRADE *_lpPacket)
{
	for (int i=0;i<_lpPacket->wReceiveItemCount;i++)
	{
		cItem	*lpReceiveItem	=	(cItem *)&_lpPacket->aReceiveItems[i];

		if	(lpReceiveItem->m_wBaseItem	==	0xffff)
			break;

		cItem	*lpItem			=	g_hero.getValidInventorySlot();

		if	(!lpItem)
		{
			s_agent.sendAskItemData();
			return	FALSE;
		}

		lpItem->copy(lpReceiveItem);

		if	(lpItem->isPresentCoupleRing())
			lpItem->changeToCoupleRing();
	}

	return	TRUE;
}

//
//	거래 하기
void
CGamePlay::Trade(SG_TRADE *_lpPacket)
{
	g_logChat.accuseTradeLog(g_lpHero->m_strName,s_tradeManager.m_mateTradeBox.m_strName,FALSE);

	g_hero.m_iGold			+=	_lpPacket->dwReceiveGold;
	g_hero.m_iGold			-=	_lpPacket->dwGiveGold;

	if	(_lpPacket->wFirstWorkTrader	==	g_lpHero->m_wSerial)	//	일단 준 다음에 받자.
	{
		if	(!EmptyTradeItem(_lpPacket))
			return;
		if	(!ReceiveTradeItem(_lpPacket))
			return;
	}
	else
	{
		if	(!ReceiveTradeItem(_lpPacket))
			return;
		if	(!EmptyTradeItem(_lpPacket))
			return;
	}

	g_hero.buildPower();

	s_tradeManager.reset();

	CloseTradeBox();

	OpenInventoryWindow();
}

//
//	거래 아이템
BOOL
CGamePlay::GetTradeItem(int _iSlot,cItem *_lpItem)
{
	return	s_tradeManager.getTradeItem(_iSlot,_lpItem);
}