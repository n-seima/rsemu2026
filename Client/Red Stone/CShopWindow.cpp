#include "CGamePlay.H"
#include "CHero.H"
#include "CActor.H"
#include "CGamePlayInterface.H"
#include "CMessage.H"
#include "CWindowInterface.H"
#include "dialog_box.H"

#define	dSHOP_ITEM_HEIGHT				40
#define	dSHOP_INTERFACE_TITLE_HEIGHT	6
#define	dSHOP_INTERFACE_BOTTOM			26
#define	dSHOP_INTERFACE_HEIGHT_GAP		(dSHOP_INTERFACE_TITLE_HEIGHT+dSHOP_INTERFACE_BOTTOM+6)

CShopWindow			g_gwShop;

enum
{
	eSBM_INCREASE_COUNT	=	10,
	eSBM_DECREASE_COUNT,
};

void
CShopWindow::initCommon()
{
	int	iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iTitleHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_SHOP);

	if	(m_iShopKind	==	eSK_PIGEON_POST)
	{
		m_rect.x1				=	g_iScreenWidth-iWindowWidth;
		m_rect.x2				=	g_iScreenWidth;
	}
	else
	{
		m_rect.x1		=	g_iScreenWidth-iWindowWidth*2;
		m_rect.x2		=	g_iScreenWidth-iWindowWidth;
	}

	m_rect.y1		=	iTitleHeight+g_iCorrectBI_Y/2;
	m_rect.y2		=	iTitleHeight+iWindowHeight+g_iCorrectBI_Y/2;

	m_iSelectShopItem			=	-1;
	m_iShopWindowScrollBarPos	=	0;
	m_iBuyItemCount				=	1;

	s_bmMiddleWindowButton.reset();

	s_bmMiddleWindowButton.addImageBar(dMSG_BUY	,IDOK				,m_rect.x1+197	,471+g_iCorrectBI_Y/2,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);

	if	(m_iShopKind	!=	eSK_PIGEON_POST)
	{
		s_bmMiddleWindowButton.addImageBar(""		,eSBM_INCREASE_COUNT,m_rect.x1+10	,474+g_iCorrectBI_Y/2,eSTATE_UP_NORMAL,eSTATE_UP_ACTIVE,eSTATE_UP_PRESSED,0xffff,eSTATE_UP_DISABLE);
		s_bmMiddleWindowButton.addImageBar(""		,eSBM_DECREASE_COUNT,m_rect.x1+22	,474+g_iCorrectBI_Y/2,eSTATE_DOWN_NORMAL,eSTATE_DOWN_ACTIVE,eSTATE_DOWN_PRESSED,0xffff,eSTATE_DOWN_DISABLE);
	}

	s_bmMiddleWindowButton.setClickMargin(IDOK,1,1);
	s_bmMiddleWindowButton.setAllBarTextColor(0,FALSE);

	s_sbMiddleWindow.set(m_rect.x1+iWindowWidth-16,m_rect.y1+dSHOP_INTERFACE_TITLE_HEIGHT,iWindowHeight-dSHOP_INTERFACE_HEIGHT_GAP);
	s_sbMiddleWindow.setPos(m_iShopWindowScrollBarPos);

	SetInventoryWindowPos();
	InitPopupWindowCloseButton();
}


//
//	상점 열기
void
CShopWindow::open(SG_OPEN_SHOP *_lpPacket)
{
	g_dialogBox.closeComplexDialog();

	if	(_lpPacket->bf15SellItemCount	==	0)
		return;

	m_bIsDuelPointShop			=	_lpPacket->bf1IsDuelPointShop;
	m_bIsLevelPerMoneyShop		=	_lpPacket->bf1IsLevelPerGoldShop;
	m_wLevelPerGold				=	_lpPacket->bf15LevelPerGoldValue;
	m_iSelectShopItem			=	-1;
	m_iSellItemSlot				=	-1;
	m_iShopWindowScrollBarPos	=	0;
	m_iShopKind					=	eSK_NORMAL;
	m_wTokkenItem				=	_lpPacket->wTokenItem;
	m_bIsNotBuyItem				=	_lpPacket->bf1IsNotBuyItem;		// 상점 구입 불가.
	m_bIsBadgeOfTraderShop		=	_lpPacket->bf1IsBadgeOfTraderShop;	// 거래상인의 배지 상점..
	if	(m_bIsDuelPointShop)
	{
		if	(g_hero.m_bf1IsWaitToReceiveDuelRecord	==	FALSE	&&	g_hero.m_bf1IsReceivedDuelRecord	==	FALSE)
		{
			s_agent.sendEtcWork(eCEW_ASK_DUEL_RECORD_INFO);
			g_hero.m_bf1IsWaitToReceiveDuelRecord	=	TRUE;
		}
	}
	
	g_hero.lockOff();
	cINPDEV::ReleaseExclusive();
	ResetAllFocus();
	ClosePopupWindow();

	s_lpInventoryWindowEquipmentList=	g_hero.m_aEquip;
	s_lpInventoryWindowItemList		=	g_hero.m_aItems;
	s_lpGold						=	&g_hero.m_iGold;

	s_iPopupInterface		=	ePIW_SHOP;

	s_shop.m_wPriceFactor	=	_lpPacket->wPriceFactor;
	s_shop.m_wItemCount		=	_lpPacket->bf15SellItemCount;
	s_shop.m_wOwner			=	_lpPacket->wOwner;
	s_shop.m_wSerial		=	_lpPacket->wShopSerial;
	s_shop.m_dwLimitPrice	=	_lpPacket->dwLimitPrice;
//	s_shop.m_TokkenShop		=	_lpPacket->wTokkenItem;

	memcpy(s_shop.m_strName,_lpPacket->strName,dSHOP_NAME_LENGTH);

	for (int i=0;i<_lpPacket->bf15SellItemCount;i++)
	{
		s_shop.m_aSellItem[i].reset();

		memcpy(&s_shop.m_aSellItem[i],&_lpPacket->aItem[i],sizeof(CItemDefine));
	}

	initCommon();
}


void
CShopWindow::openForGuildShop(SG_OPEN_GUILD_SHOP *_lpPacket)
{
	s_shop.m_wItemCount		=	_lpPacket->wSellItemCount;
	memcpy(m_awGuildShopItemList,_lpPacket->awSellItemList,sizeof(m_awGuildShopItemList));

	g_dialogBox.closeComplexDialog();

	if	(_lpPacket->wSellItemCount	==	0)
		return;

	m_iSelectShopItem			=	-1;
	m_iSellItemSlot				=	-1;
	m_iShopWindowScrollBarPos	=	0;
	m_iShopKind					=	eSK_GUILD_SHOP;
	m_wTokkenItem				=	0xffff;		// 추가.
	g_hero.lockOff();
	cINPDEV::ReleaseExclusive();
	ResetAllFocus();
	ClosePopupWindow();

	s_lpInventoryWindowEquipmentList=	g_hero.m_aEquip;
	s_lpInventoryWindowItemList		=	g_hero.m_aItems;
	s_lpGold						=	&g_hero.m_iGold;

	s_iPopupInterface		=	ePIW_SHOP;

	for (int i=0;i<_lpPacket->wSellItemCount;i++)
	{
		s_shop.m_aSellItem[i].reset();

		cItem::GenerateItemByBaseItem(&s_shop.m_aSellItem[i],_lpPacket->awSellItemList[i],1);

		s_shop.m_aSellItem[i].m_dwSerial	=	s_shop.m_aSellItem[i].getPrice(1);
	}

	initCommon();
}

//
//	상점 열기
void
CShopWindow::openForPigeonPost(int _iCharge,int _iCount)
{
	g_dialogBox.closeComplexDialog();

	if	(_iCount	==	0)
		return;

	if	(s_iPopupInterface		==	ePIW_SHOP	&&	m_iShopKind		==	eSK_PIGEON_POST)
		return;

	m_iShopKind					=	eSK_PIGEON_POST;
	m_wTokkenItem				=	0xffff;

	m_iSelectShopItem			=	-1;
	m_iSellItemSlot				=	-1;
	m_iShopWindowScrollBarPos	=	0;

	g_hero.lockOff();
	cINPDEV::ReleaseExclusive();
	ResetAllFocus();
	ClosePopupWindow();

	s_lpGold				=	&g_hero.m_iGold;

	s_iPopupInterface		=	ePIW_SHOP;

	s_shop.m_wPriceFactor	=	_iCharge;
	s_shop.m_wItemCount		=	min(_iCount,g_pigeonPostShop.m_iItemCount);

	for (int i=0;i<_iCount;i++)
		g_pigeonPostShop.getItemForShop(&s_shop.m_aSellItem[i],i,_iCharge);

	initCommon();
}

//
//	상점 열기
void
CShopWindow::close()
{
	cINPDEV::ReleaseExclusive();

	s_shop.m_wItemCount	=	0;

	CloseInventoryWindow();
	s_iPopupInterface	=	ePIW_NONE;
}

//
//	그리기
void
CShopWindow::draw()
{	
	if	(!isOpened())
		return;

	if	(m_iShopKind	!=	eSK_PIGEON_POST)
		DrawInventoryWindow();

	int		iX,iY,iCount	=	0;
	int		iLineCount		=	0;
	int		iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);

	iX		=	m_rect.x1;
	iY		=	m_rect.y1;

	if	(g_config.m_bIsInfoWindowHalfblending)
		cDRAW::FillHB(0,iX+8,iY+6,iX+266-4,iY+482-8);
	else
		cDRAW::Fill  (0,iX+8,iY+6,iX+266-4,iY+482-8);

	g_sprInterface.Put(iX	,g_iCorrectBI_Y/2		,eWINDOW_TITLE_SHOP);
	g_sprInterface.Put(iX	,iY		,eWINDOW);
//	g_sprInterface.Put(iX+7	,iY+20	,eWINDOW_SEPERATE_BAR);	//	경계

//	s_text.cPut(iX+iWindowWidth/2,iY+8,LTYELLOW,s_shop.m_strName);

	iY		+=	dSHOP_INTERFACE_TITLE_HEIGHT;

//	스크롤바 정보 재설정
	{
		int	iHeight	=	iWindowHeight-dSHOP_INTERFACE_HEIGHT_GAP;

		iLineCount	=	iHeight/dSHOP_ITEM_HEIGHT;
		s_sbMiddleWindow.setSize(s_shop.m_wItemCount,iLineCount);
	}

	int	i,iPutCount	=	0;

	for (i=s_sbMiddleWindow.m_iCurrentPos;i<s_shop.m_wItemCount && iPutCount<iLineCount;i++)
	{
		cItem	*lpItem			=	&s_shop.m_aSellItem[i];

		if	(lpItem->m_wBaseItem	==	0xffff)
			continue;

		cBasicItem	*lpBaseItem	=	lpItem->getBasicItem(TRUE);

		int	iPutX	=	iX+10;
		int	iPutY	=	iY+iPutCount*dSHOP_ITEM_HEIGHT;

		if (i==m_iSelectShopItem)
		{
			iPutY--;
			cDRAW::Fill(RGB24To16(0,100,200),iPutX-2,iPutY+2,iPutX+245-6,iPutY+42-2);	//	선택한 아이템이다.
		}

		g_sprInterface.Put(iPutX+3,iPutY+4,eIWI_ITEM_SLOT_FRAME);
		PutItem(lpItem,iPutX+3,iPutY+4,TRUE);
		char	strName[256] = "";

		if(lpBaseItem->m_bf1IsRingOfInfinity && lpItem->getCarvingLevel())
			sprintf(strName,"%s<c:LTGREEN>Lv %d<n>]",lpBaseItem->m_strName,lpItem->getCarvingLevel());
		else
			sprintf(strName,"%s",lpBaseItem->m_strName);		
		s_text.put(iPutX+42,iPutY+5	,lpItem->getNameColor(),strName);
		if(!m_bIsNotBuyItem)		// 구입불가상점이 아니면..
		{	
			if(m_wTokkenItem != 0xffff)			//토큰 상점.
			{
				cBasicItem* lpRequireItem = &g_aBasicItem[m_wTokkenItem];
				
				g_smiIconItem.putScale(lpRequireItem->m_wIconShape,iPutX + 41,iPutY + 5 + 16 ,35);
				if(lpRequireItem->m_bf1IsRingOfInfinity && lpItem->getCarvingLevel())
					s_text.put(iPutX+55,iPutY+5+16,WHITE,"%s<c:LTGREEN>Lv %d<n>] <c:LTYELLOW>%s<n>",lpRequireItem->m_strName, lpItem->getCarvingLevel(),_ms(dMSG_COUNT_FORM ,lpItem->m_dwSerial));	// 수정.
				else
					s_text.put(iPutX+55,iPutY+5+16,WHITE,"%s <c:LTYELLOW>%s<n>",lpRequireItem->m_strName, _ms(dMSG_COUNT_FORM ,lpItem->m_dwSerial));	// 수정.
			}
			else if	(m_bIsDuelPointShop)
				s_text.put(iPutX+42,iPutY+5+16,WHITE,"%s <c:LTYELLOW>%s<n>",dMSG_DUEL_POINT,GetMoneyString(lpItem->m_dwSerial));
			else if (m_bIsLevelPerMoneyShop)
				s_text.put(iPutX+42,iPutY+5+16,WHITE,"%s <c:LTYELLOW>%s<n> G",dMSG_PRICE,GetMoneyString(lpBaseItem->m_dwPrice * m_wLevelPerGold * g_hero.m_iLevel * 100));
			else
				s_text.put(iPutX+42,iPutY+5+16,WHITE,"%s <c:LTYELLOW>%s<n> G",dMSG_PRICE,GetMoneyString(lpItem->m_dwSerial));
		}
		iPutCount++;

		if	(i==m_iSelectShopItem)
			iPutY++;
	}

	if	(s_sbMiddleWindow.isVisible())
		s_sbMiddleWindow.draw();	//	스크롤바

//	선택한 상점 아이템 정보
	{
		int	iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
		int	iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
		
		g_sprInterface.Put(iX+7,463+g_iCorrectBI_Y/2		,eWINDOW_SEPERATE_BAR);	//	경계
		if(!m_bIsNotBuyItem)
		{		// 상점구입불가가 아니면...+_+;
			
			CPos	pos;
			
			pos.x	=	m_rect.x1+12;
			pos.y	=	m_rect.y1+iWindowHeight-24;
			
			if	(m_iSelectShopItem	==	-1)
				s_text.put(pos.x,pos.y,WHITE,dMSG_SELECT_ITEM_NOT_FOUND);
			else
			{
				m_iBuyItemCount	=	min(m_iBuyItemCount,g_hero.getEmptyInventorySlotCount());
				m_iBuyItemCount	=	max(m_iBuyItemCount,1);
				
				if	(m_iShopKind	!=	eSK_PIGEON_POST)
					s_text.put(pos.x+24,pos.y,WHITE,"%s <c:LTGREEN>%d<n>/%d",dMSG_BUY_COUNT,m_iBuyItemCount,g_hero.getEmptyInventorySlotCount());
				
				s_bmMiddleWindowButton.draw();
			}
		}
	}

	DrawPopupWindowCloseButton();
}


//
//	업데이트
void
CShopWindow::update()
{// 마지막 수정일 : 09.10.26
	if	(!isOpened())
		return;

	if	(!cINPDEV::IsExclusive(eMOUSE_STATE_SHOP_SCROLL_BAR))
	{
		if	(s_sbMiddleWindow.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed))
			cINPDEV::SetExclusive(eMOUSE_STATE_SHOP_SCROLL_BAR);
		else
			cINPDEV::ReleaseExclusive(TRUE,eMOUSE_STATE_SHOP_SCROLL_BAR);
	}

	if	(m_rect.isIn(s_posMouse.x,s_posMouse.y))
	{
		if	(s_iWheelValue<0)
			s_sbMiddleWindow.setPos(s_sbMiddleWindow.m_iCurrentPos+1);
		if	(s_iWheelValue>0)
			s_sbMiddleWindow.setPos(s_sbMiddleWindow.m_iCurrentPos-1);
	}

	if	(m_iShopKind	!=	eSK_PIGEON_POST)
		OperateInventoryWindow();
	if	(m_iShopKind	==	eSK_NORMAL)
		OperateDragItem();	//	드래그 아이템 처리

	if	(cINPDEV::IsExclusive(eMOUSE_STATE_POPUP_MENU))
		return;

	if	(m_iSelectShopItem	!=	-1 && !m_bIsNotBuyItem)	//	구입 버튼 (구입불가체크된상태일때는 구입버튼이 활성화 되지 않는다.)
	{
		DWORD	dwMenu	=	s_bmMiddleWindowButton.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);
		
		switch(dwMenu)
		{
		case	eSBM_INCREASE_COUNT	:
			{
				// 10.02.11 추가
				if(s_aKey[KEY_LSHIFT])
					m_iBuyItemCount	+= 10;
				else if(s_aKey[KEY_LCONTROL])
					m_iBuyItemCount	+= 50;
				else
					m_iBuyItemCount	++;
				m_iBuyItemCount	=	min(m_iBuyItemCount,g_hero.getEmptyInventorySlotCount());
				m_iBuyItemCount	=	max(m_iBuyItemCount,1);
				break;
			}
		case	eSBM_DECREASE_COUNT	:
			{
				// 10.02.11 추가
				if(s_aKey[KEY_LSHIFT])
					m_iBuyItemCount	-= 10;
				else if(s_aKey[KEY_LCONTROL])
					m_iBuyItemCount	-= 50;
				else
					m_iBuyItemCount	--;
				m_iBuyItemCount	=	max(m_iBuyItemCount,1);
				break;
			}
		case	IDOK	:
			{
				cItem	*lpItem	=	&s_shop.m_aSellItem[m_iSelectShopItem];
				
				if(m_wTokkenItem != 0xffff)
				{
					int iItemCount = g_hero.getItemCountInInventory(m_wTokkenItem);
					
					if (iItemCount < lpItem->m_dwSerial)
					{
						AddSystemMessage(WHITE,dMSG_LACK_OF_TOKKEN);		//	토큰이 부족해요.
						break;
					}
				}
				else if	(m_bIsDuelPointShop	==	FALSE)
				{
					if	(g_hero.m_iGold	<	(int)lpItem->m_dwSerial	)
					{
						AddSystemMessage(WHITE,dMSG_LACK_OF_GOLD);		//	돈이 없어요..-o-
						break;
					}
				}
				else if	(g_hero.m_duelRecord.m_iDuelPoint	<	(int)lpItem->m_dwSerial	)
				{
					AddSystemMessage(WHITE,dMSG_LACK_OF_DUEL_POINT);	//	결투 포인트가 없어요..-o-
					break;
				}
				
				if	(g_hero.getValidInventorySlot()	==	NULL)
				{
					AddSystemMessage(WHITE,dMSG_FAILED_BUY_ITEM_BY_FULL_INVENTORY);		//	돈이 없어요..-o-
					break;
				}
				
				if	(m_iShopKind	==	eSK_PIGEON_POST)
				{
					char	strQuestion[256];
					
					sprintf(strQuestion,dMSG_BUY_PIGEON_SHOP_ITEM_QUESTION_FORM,lpItem->m_dwSerial,lpItem->getName(),lpItem->m_bCount);
					
					g_msgBox.cPopup("",strQuestion,dMSG_YES,dMSG_NO);
					g_msgBox.setButtonId(eMBI_BUY_PIGEON_SHOP_ITEM);
				}
				else if	(m_iShopKind	==	eSK_GUILD_SHOP)
					s_agent.sendBuyItem(0,0,m_awGuildShopItemList[m_iSelectShopItem],TRUE,m_iBuyItemCount, FALSE, m_bIsBadgeOfTraderShop);
				else if (m_wTokkenItem != 0xffff)
				{
					s_agent.sendBuyItem(s_shop.m_wSerial,s_shop.m_wOwner,m_iSelectShopItem,FALSE,m_iBuyItemCount ,TRUE,m_bIsBadgeOfTraderShop);
				}
				else
					s_agent.sendBuyItem(s_shop.m_wSerial,s_shop.m_wOwner,m_iSelectShopItem,FALSE,m_iBuyItemCount, FALSE, m_bIsBadgeOfTraderShop);
				break;
			}
		}
	}

	int	iSelectShopItem	=	-1;

	{
		int	iX,iY,iCount=0;
		int	iLineCount	=	0;

		iX	=	m_rect.x1;
		iY	=	m_rect.y1;
		iY	+=	dSHOP_INTERFACE_TITLE_HEIGHT;

	//	스크롤바 정보 재설정
		{
			int	iHeight	=	g_sprInterface.getSpriteHeight(eWINDOW)-dSHOP_INTERFACE_HEIGHT_GAP;

			iLineCount	=	iHeight/dSHOP_ITEM_HEIGHT;

			s_sbMiddleWindow.setSize(s_shop.m_wItemCount,iLineCount);
		}

		int	i,iPutCount	=	0;

		for (i=s_sbMiddleWindow.m_iCurrentPos;i<s_shop.m_wItemCount && iPutCount<iLineCount;i++)
		{
			cItem	*lpItem			=	&s_shop.m_aSellItem[i];

			if (lpItem->m_wBaseItem	==	0xffff)	continue;

			cBasicItem	*lpBaseItem	=	lpItem->getBasicItem(TRUE);

			int		iPutX	=	iX+10;
			int		iPutY	=	iY+iPutCount*dSHOP_ITEM_HEIGHT;

			cRECT	rectSelect;

			rectSelect.set(iPutX-2,iPutY-2,iPutX+245-6,iPutY+42-2);

			if (rectSelect.isIn(s_posMouse.x,s_posMouse.y))
			{
				s_rectTooltip.set(rectSelect.x1,rectSelect.y1,rectSelect.x1+dITEM_SHAPE_WIDTH+2,rectSelect.y1+dITEM_SHAPE_HEIGHT+2);
				iSelectShopItem	=	i;
				break;
			}

			iPutCount++;
		}

		if	(iSelectShopItem	!=	-1)
		{
			if (s_rectTooltip.isIn(s_posMouse.x,s_posMouse.y))
			{
				s_rectTooltip.add(0,dITEM_SHAPE_HEIGHT+8);

				cItem	*lpItem		=	&s_shop.m_aSellItem[iSelectShopItem];
				char	*strComment	=	lpItem->getItemTooltip(-1,-1,TRUE);

				strComment			=	g_hero.getAfterEquipInfo(strComment,lpItem);

				s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,strComment,eTFM_LEFT);
			}
		}
	}

	if	(s_isTouchLeftButton && !s_bmMiddleWindowButton.isPressed())
	{
		if	(m_iSelectShopItem	!=	iSelectShopItem)
			m_iBuyItemCount	=	1;

		m_iSelectShopItem	=	iSelectShopItem;
	}
}

//
//	아이템 판매
void
CShopWindow::sellItem(int _iItemSlot)
{
	if	(m_iShopKind	!=	eSK_NORMAL)
	{
		WarningMessage(dMSG_CAN_NOT_SELL_ITEM_SHOP);
		return;
	}
	
	if	(_iItemSlot	==	dITEM_DRAG_GOLD)
		return;

	if	(_iItemSlot	>=	dBORDER_OF_ITEM_AND_EQUIPMENT)
	{
		AddSystemMessage(LTGREEN,dMSG_CAN_NOT_SELL_EQUIPPED_ITEM);
		return;
	}

	cBasicItem	*lpItem	=	g_hero.m_aItems[_iItemSlot].getBasicItem();

	if (lpItem && lpItem->m_attr.isCanNotSelllItem)
	{
		AddSystemMessage(LTRED,dMSG_CAN_NOT_SELL_ITEM);
		return;
	}

#ifndef	_FOR_KOREA
	if (lpItem && g_hero.m_aItems[_iItemSlot].isExtraItem())
	{
		AddSystemMessage(LTRED,dMSG_CAN_NOT_SELL_ITEM);
		return;
	}
#endif
		
	g_itemCountWindow.popup(s_posMouse.x,s_posMouse.y,_iItemSlot,&g_hero.m_aItems[_iItemSlot],eICWR_RETURN_SELL_ITEM_COUNT,s_shop.m_wPriceFactor);

	m_iSellItemSlot	=	_iItemSlot;

	cINPDEV::SetExclusive(eMOUSE_STATE_GET_ITEM_COUNT);
}