#include "CGamePlay.H"
#include "CHero.H"
#include "CActor.H"
#include "CGamePlayInterface.H"
#include "CMessage.H"
#include "CWindowInterface.H"

#define	dSHOP_ITEM_HEIGHT				40
#define	dSHOP_INTERFACE_TITLE_HEIGHT	6
#define	dSHOP_INTERFACE_BOTTOM			26
#define	dSHOP_INTERFACE_HEIGHT_GAP		(dSHOP_INTERFACE_TITLE_HEIGHT+dSHOP_INTERFACE_BOTTOM+6)

CRepairItemWindow	g_gwRepairItem;

//
//	상점 열기

void
CRepairItemWindow::open(int _iNpcSerial)
{
	ClosePopupWindow();

	m_iNpcSerial	=	_iNpcSerial;
	m_iSelectItem	=	-1;
	m_iScrollBarPos	=	0;

	g_hero.lockOff();
	cINPDEV::ReleaseExclusive();
	ResetAllFocus();
	ClosePopupWindow();

	int	iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iTitleHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_SHOP);

	s_rectInterfaceSize.x1	=	g_iScreenWidth-iWindowWidth;
	s_rectInterfaceSize.x2	=	g_iScreenWidth;
	s_iPopupInterface		=	ePIW_REPAIR_ITEM;
	m_rect.x1				=	g_iScreenWidth-iWindowWidth;
	m_rect.y1				=	iTitleHeight+g_iCorrectBI_Y/2;
	m_rect.x2				=	g_iScreenWidth;
	m_rect.y2				=	iTitleHeight+iWindowHeight+g_iCorrectBI_Y/2;

	s_bmButton.reset();

	s_bmButton.addImageBar(dMSG_REPAIR	,IDOK		,m_rect.x1+130,471+g_iCorrectBI_Y/2,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
	s_bmButton.addImageBar(dMSG_CLOSE	,IDCANCEL	,m_rect.x1+196,471+g_iCorrectBI_Y/2,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
	s_bmButton.setClickMargin(IDOK,1,1);
	s_bmButton.setClickMargin(IDCANCEL,1,1);
	s_bmButton.setAllBarTextColor(0,FALSE);

	s_sbPopupWindow.set(m_rect.x1+iWindowWidth-16,m_rect.y1+dSHOP_INTERFACE_TITLE_HEIGHT,iWindowHeight-dSHOP_INTERFACE_HEIGHT_GAP);
	s_sbPopupWindow.setPos(m_iScrollBarPos);

	InitPopupWindowCloseButton();
}

//
//	딛기
void
CRepairItemWindow::close()
{
	cINPDEV::ReleaseExclusive();

	s_iPopupInterface		=	ePIW_NONE;
	s_rectInterfaceSize.set(0, 0, 0, 0);
}

//
//	그리기

void
CRepairItemWindow::draw()
{
	if (!isOpened())
		return;

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

	g_sprInterface.Put(iX	,g_iCorrectBI_Y/2		,eWINDOW_TITLE_REPAIR_ITEM);
	g_sprInterface.Put(iX	,iY		,eWINDOW);
	
	iY		+=	dSHOP_INTERFACE_TITLE_HEIGHT;

//	스크롤바 정보 재설정
	{
		int	iHeight	=	iWindowHeight-dSHOP_INTERFACE_HEIGHT_GAP;

		iLineCount	=	iHeight/dSHOP_ITEM_HEIGHT;
		s_sbPopupWindow.setSize(m_iBreakDownItemCount,iLineCount);
	}

	int	i,iPutCount	=	0;

	if (m_iBreakDownItemCount	==	0)
		s_text.put(iX+12,iY+8+6 ,256-12,100,0,WHITE,dMSG_NOT_EXIST_BREAKDOWN_ITEM_COMMENT);

	for (i=s_sbPopupWindow.m_iCurrentPos;i<m_iBreakDownItemCount && iPutCount<iLineCount;i++)
	{
		cItem	*lpItem			=	g_hero.getInventoryItem(m_aiBreakDownItemList[i]);

		if (lpItem->m_wBaseItem	==	0xffff)
			continue;

		cBasicItem	*lpBaseItem	=	lpItem->getBasicItem(TRUE);

		int	iPutX	=	iX+10;
		int	iPutY	=	iY+iPutCount*dSHOP_ITEM_HEIGHT;

		if (i==m_iSelectItem)
		{
			iPutY--;
			cDRAW::Fill(RGB24To16(0,100,200),iPutX-2,iPutY+2,iPutX+245-6,iPutY+42-2);	//	선택한 아이템이다.
		}

		g_sprInterface.Put(iPutX+3,iPutY+4,eIWI_ITEM_SLOT_FRAME);
		PutItem(lpItem,iPutX+3,iPutY+4,TRUE);

		s_text.put(iPutX+42,iPutY+5	,lpItem->getNameColor(),lpItem->getName());			// 10.02.11
		int	iRepairPrice	=	lpItem->getFullRepairPrice();
		iRepairPrice		-=	iRepairPrice*g_hero.m_wCourtesyTitleValue/10000;

		if	(g_bIsDuelServer)
			iRepairPrice	=	0;

		s_text.put(iPutX+42,iPutY+5+16,WHITE,"%s <c:LTYELLOW>%s<n> G",dMSG_REPAIR_PRICE,GetMoneyString(iRepairPrice));

		iPutCount++;

		if (i==m_iSelectItem)	
			iPutY++;
	}

	if (s_sbPopupWindow.isVisible())
		s_sbPopupWindow.draw();	//	스크롤바

	g_sprInterface.Put(iX+7,463+g_iCorrectBI_Y/2		,eWINDOW_SEPERATE_BAR);	//	경계

	s_bmButton.draw();
}

//
//	업데이트
void
CRepairItemWindow::update()
{
	if (!isOpened())
		return;

	if (!cINPDEV::IsExclusive(eMOUSE_STATE_SCROLL_BAR))
	{
		if (s_sbPopupWindow.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed))
				cINPDEV::SetExclusive(eMOUSE_STATE_SCROLL_BAR);
		else	cINPDEV::ReleaseExclusive(TRUE,eMOUSE_STATE_SCROLL_BAR);
	}

	if	(cINPDEV::IsExclusive(eMOUSE_STATE_POPUP_MENU))
		return;

	g_hero.getBreakdownItemList(m_aiBreakDownItemList,&m_iBreakDownItemCount);

	{
		DWORD	dwMenu	=	s_bmButton.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);

		switch(dwMenu)
		{
			case	IDOK	:
			{
				if	(m_iBreakDownItemCount	==	0	||	m_iSelectItem	==	-1)
					break;

				int		iSlot			=	m_aiBreakDownItemList[m_iSelectItem];
				cItem	*lpItem			=	g_hero.getInventoryItem(iSlot);

				if (!lpItem)
					break;

				DWORD	dwRepairPrice	=	lpItem->getFullRepairPrice();

				dwRepairPrice			-=	dwRepairPrice*g_hero.m_wCourtesyTitleValue/10000;

				if	(g_bIsDuelServer)
					dwRepairPrice		=	0;

				if (g_hero.m_iGold	<	(int)dwRepairPrice)
				{
					AddSystemMessage(LTYELLOW,dMSG_LACK_OF_GOLD);		//	돈이 없어요..-o-
					break;
				}

				g_msgBox.cPopup("",_ms(dMSG_REPAIR_ITEM_QUESTION_COMMENT_FORM,GetMoneyString(dwRepairPrice),lpItem->getName()),dMSG_YES,dMSG_NO);
				g_msgBox.setButtonId(eMBI_REPAIR_ITEM);

				break;
			}

			case	IDCANCEL	:
			{
				close();

				break;
			}
		}
	}

	int	iSelectItem	=	-1;

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

			s_sbPopupWindow.setSize(m_iBreakDownItemCount,iLineCount);
		}

		int	i,iPutCount	=	0;

		for (i=s_sbPopupWindow.m_iCurrentPos;i<m_iBreakDownItemCount && iPutCount<iLineCount;i++)
		{
			int		iSlot			=	m_aiBreakDownItemList[i];
			cItem	*lpItem			=	g_hero.getInventoryItem(iSlot);

			if (lpItem->m_wBaseItem	==	0xffff)	continue;

			cBasicItem	*lpBaseItem	=	lpItem->getBasicItem(TRUE);

			int		iPutX	=	iX+10;
			int		iPutY	=	iY+iPutCount*dSHOP_ITEM_HEIGHT;

			cRECT	rectSelect;

			rectSelect.set(iPutX-2,iPutY-2,iPutX+245-6,iPutY+42-2);

			if	(rectSelect.isIn(s_posMouse.x,s_posMouse.y))
			{
				s_rectTooltip.set(rectSelect.x1,rectSelect.y1,rectSelect.x1+dITEM_SHAPE_WIDTH+2,rectSelect.y1+dITEM_SHAPE_HEIGHT+2);
				iSelectItem	=	i;
				break;
			}

			iPutCount++;
		}

		if	(iSelectItem	!=	-1)
		{
			int		iSlot			=	m_aiBreakDownItemList[iSelectItem];
			cItem	*lpItem			=	g_hero.getInventoryItem(iSlot);
			
			if (lpItem	&&	s_rectTooltip.isIn(s_posMouse.x,s_posMouse.y))
			{
				s_rectTooltip.add(0,dITEM_SHAPE_HEIGHT+8);
			
				char	*strComment	=	lpItem->getItemTooltip(-1,-1,TRUE);
				
				strComment			=	g_hero.getAfterEquipInfo(strComment,lpItem);

				s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,strComment,eTFM_LEFT);
			}
		}
	}

	if	(s_isTouchLeftButton && !s_bmButton.isPressed())
		m_iSelectItem	=	iSelectItem;
}

BOOL
CRepairItemWindow::isOpened()
{
	if	(s_iPopupInterface		==	ePIW_REPAIR_ITEM)
		return	TRUE;

	return	FALSE;
}
