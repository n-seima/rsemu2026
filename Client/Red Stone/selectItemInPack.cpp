#include "CWindowInterface.H"
#include "CGamePlayInterface.H"
#include "CActor.H"
#include "CMessage.H"

cSelectItemInPackWnd	g_gwSelectItemInPack;

#define	dSHOP_ITEM_HEIGHT				40
#define	dSHOP_INTERFACE_TITLE_HEIGHT	6
#define	dSHOP_INTERFACE_BOTTOM			26
#define	dSHOP_INTERFACE_HEIGHT_GAP		(dSHOP_INTERFACE_TITLE_HEIGHT+dSHOP_INTERFACE_BOTTOM+6)

enum
{
	eSIWI_TITLE		=	66,
};


void
cSelectItemInPackWnd::open()
{
	m_iSelectItem		=	-1;	//	구입을 위해 선택한 아이템
	m_iFocusItem		=	-1;	//	포커스가 간 아이템
	m_iScrollBarPos		=	0;
	m_wIsWaitResult		=	FALSE;

	g_hero.lockOff();
	cINPDEV::ReleaseExclusive();
	ResetAllFocus();
	ClosePopupWindow();

	s_lpInventoryWindowEquipmentList=	g_hero.m_aEquip;
	s_lpInventoryWindowItemList		=	g_hero.m_aItems;
	s_lpGold						=	&g_hero.m_iGold;

	int	iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iTitleHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_SHOP);

	s_iPopupInterface		=	ePIW_SELECT_ITEM_IN_PACK;
	m_rect.x1				=	g_iScreenWidth-iWindowWidth*2;
	m_rect.y1				=	iTitleHeight+g_iCorrectBI_Y/2;
	m_rect.x2				=	g_iScreenWidth-iWindowWidth;
	m_rect.y2				=	iTitleHeight+iWindowHeight+g_iCorrectBI_Y/2;

	s_bmMiddleWindowButton.reset();

	s_bmMiddleWindowButton.addImageBar(dMSG_SELECT,IDOK		,m_rect.x1+197,471+g_iCorrectBI_Y/2,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
	s_bmMiddleWindowButton.setClickMargin(IDOK,1,1);
	s_bmMiddleWindowButton.setAllBarTextColor(0,FALSE);

	s_sbMiddleWindow.set(m_rect.x1+iWindowWidth-16,m_rect.y1+dSHOP_INTERFACE_TITLE_HEIGHT,iWindowHeight-dSHOP_INTERFACE_HEIGHT_GAP);
	s_sbMiddleWindow.setPos(m_iScrollBarPos);

	SetInventoryWindowPos();
	InitPopupWindowCloseButton();
}


void
cSelectItemInPackWnd::close()
{
	cINPDEV::ReleaseExclusive();

	CloseInventoryWindow();

	s_iPopupInterface	=	ePIW_NONE;
}

BOOL
cSelectItemInPackWnd::isOpened()
{
	if	(s_iPopupInterface		==	ePIW_SELECT_ITEM_IN_PACK)
		return	TRUE;

	return	FALSE;
}

int
cSelectItemInPackWnd::getFocusItem()
{
	int		iX,iY,iCount	=	0;
	int		iLineCount		=	0;
	int		iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);

	iX		=	m_rect.x1;
	iY		=	m_rect.y1;

	iY		+=	dSHOP_INTERFACE_TITLE_HEIGHT;

	int	i,iPutCount	=	0;

//	스크롤바 정보 재설정
	{
		int	iHeight	=	iWindowHeight-dSHOP_INTERFACE_HEIGHT_GAP;

		iLineCount	=	iHeight/dSHOP_ITEM_HEIGHT;
	}

	for (i=s_sbMiddleWindow.m_iCurrentPos;i<m_itemPackInfo.m_wCount && iPutCount<iLineCount;i++)
	{
		cItem	item;

		memcpy(&item,&m_itemPackInfo.m_aItems[i],sizeof(cItem));

		int	iPutX	=	iX+10;
		int	iPutY	=	iY+iPutCount*dSHOP_ITEM_HEIGHT;

		cRECT	rectSelect;

		rectSelect.set(iPutX-2,iPutY-2,iPutX+245-6,iPutY+42-2);

		if	(rectSelect.isIn(s_posMouse.x,s_posMouse.y))
		{
			s_rectTooltip.set(rectSelect.x1,rectSelect.y1,rectSelect.x1+dITEM_SHAPE_WIDTH+2,rectSelect.y1+dITEM_SHAPE_HEIGHT+2);

			return	i;
		}

		iPutCount++;
	}

	return	0xffff;
}

void
cSelectItemInPackWnd::update()
{
	if	(!isOpened())
		return;

	if	(!cINPDEV::IsExclusive(eMOUSE_STATE_SHOP_SCROLL_BAR))
	{
		if (s_sbMiddleWindow.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed))
			cINPDEV::SetExclusive(eMOUSE_STATE_SHOP_SCROLL_BAR);
		else
			cINPDEV::ReleaseExclusive(TRUE,eMOUSE_STATE_SHOP_SCROLL_BAR);
	}

	OperateInventoryWindow();

	if	(cINPDEV::IsExclusive(eMOUSE_STATE_POPUP_MENU))
		return;

	m_iFocusItem	=	getFocusItem();

	if	(m_iFocusItem	!=	0xffff)
	{
		if (s_rectTooltip.isIn(s_posMouse.x,s_posMouse.y))
		{
			s_rectTooltip.add(dITEM_SHAPE_HEIGHT+8,8);
			
			cItem	item;

			memcpy(&item,&m_itemPackInfo.m_aItems[m_iFocusItem],sizeof(cItem));

			if	(m_itemPackInfo.m_aItems[m_iFocusItem].m_bf1IsBlockToTrade)
			{
				item.m_bf1IsSpecialItem	=	TRUE;
				cSpecialItem	*lpSpecialItem		=	item.getSpecialItem();

				lpSpecialItem->m_isBroken						=	FALSE;
				lpSpecialItem->m_bf1IsSpecialItem				=	TRUE;

				lpSpecialItem->m_bf1IsReversionItem				=	FALSE;
				lpSpecialItem->m_bf1IsReversionItemWhenEquip	=	FALSE;
				lpSpecialItem->m_bf1IsSetItem					=	FALSE;	//	세트 아이템이다.
				lpSpecialItem->m_bf1IsUnknown					=	FALSE;	//	무슨 아이템인지 모르겠다.

				lpSpecialItem->m_bf11SValue					=	0;	//	세트 아이템이나 조각 아이템 인덱스
				lpSpecialItem->m_bf10EquipLevel				=	0;	//	착용 제한 레벨

				lpSpecialItem->m_bf1IsReversionItem	=	TRUE;
			}

			char	*strComment	=	item.getItemTooltip(-1,-1,TRUE);

			strComment			=	g_hero.getAfterEquipInfo(strComment,&item);

			s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,strComment,eTFM_NONE);
		}
	}

	if	(s_isTouchLeftButton && !s_bmMiddleWindowButton.isPressed() && m_iFocusItem != 0xffff)
		m_iSelectItem	=	m_iFocusItem;

	if	(m_iSelectItem	!=	-1)	//	구입 버튼
	{
		DWORD	dwMenu	=	s_bmMiddleWindowButton.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);

		switch(dwMenu)
		{
			case	IDOK	:
			{
				if	(m_wIsWaitResult)
				{
					AddSystemMessage(LTYELLOW,dMSG_WAIT_FOR_RESULT_OF_BUY_ITEM);
					break;
				}

				s_agent.sendEtcWork(eCEW_SELECT_ITEM_IN_PACK,m_wPackItem,m_iSelectItem,0xffff);
				
				break;
			}
		}
	}
}


void
cSelectItemInPackWnd::draw()
{
	if (!isOpened())
		return;

	DrawInventoryWindow();

	int		iX,iY,iCount		=	0;
	int		iLineCount			=	0;
	int		iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);

	iX		=	m_rect.x1;
	iY		=	m_rect.y1;

	if	(g_config.m_bIsInfoWindowHalfblending)
		cDRAW::FillHB(0,iX+8,iY+6,iX+266-4,iY+482-8);
	else
		cDRAW::Fill  (0,iX+8,iY+6,iX+266-4,iY+482-8);

	g_sprInterface2.Put(iX	,g_iCorrectBI_Y/2		,eSIWI_TITLE);
	g_sprInterface.Put(iX	,iY		,eWINDOW);

	iY		+=	dSHOP_INTERFACE_TITLE_HEIGHT;

//	스크롤바 정보 재설정
	{
		int	iHeight	=	iWindowHeight-dSHOP_INTERFACE_HEIGHT_GAP;

		iLineCount	=	iHeight/dSHOP_ITEM_HEIGHT;
		s_sbMiddleWindow.setSize(m_itemPackInfo.m_wCount,iLineCount);
	}

	int	i,iPutCount	=	0;

	for (i=s_sbMiddleWindow.m_iCurrentPos;i<m_itemPackInfo.m_wCount&& iPutCount<iLineCount;i++)
	{
		cItem	item;

		memcpy(&item,&m_itemPackInfo.m_aItems[i],sizeof(cItem));

		int	iPutX	=	iX+10;
		int	iPutY	=	iY+iPutCount*dSHOP_ITEM_HEIGHT;

		if (i	==	m_iSelectItem)
		{
			iPutY--;
			cDRAW::Fill(RGB24To16(0,100,200),iPutX-2,iPutY+2,iPutX+245-6,iPutY+42-2);	//	선택한 아이템이다.
		}

		g_sprInterface.Put(iPutX+3,iPutY+4,eIWI_ITEM_SLOT_FRAME);
		PutItem(&item,iPutX+3,iPutY+4,TRUE);

		char	strItemName[128];

		strcpy(strItemName,item.getName());

		int		iTextWidth	=	(iPutX+245-6)-(iPutX-2)-4-40;

		if	(s_text.getLineCount(iTextWidth,strItemName)	>=	2)
		{
			s_text.setLineGap(0);
			s_text.cPut(iPutX+40,iPutY+2,iTextWidth,40,0,WHITE,dFITTEXT_HEIGHT_CENTER,strItemName);
			s_text.setLineGap(2);
		}
		else
		{
			s_text.put(iPutX+42,iPutY+14			,WHITE,strItemName);
		}

		iPutCount++;

		if	(i	==	m_iSelectItem)
			iPutY++;
	}

	s_bmMiddleWindowButton.draw();
	s_sbMiddleWindow.draw();

	g_sprInterface.Put(m_rect.x1+7,463+g_iCorrectBI_Y/2		,eWINDOW_SEPERATE_BAR);	//	경계
}

void
cSelectItemInPackWnd::setData(int _iItem,void *_lpData)
{
	m_wPackItem	=	_iItem;

	cItemPackInfo	*lpPack	=	(cItemPackInfo	*)_lpData;

	memcpy(&m_itemPackInfo,_lpData,sizeof(cItemPackInfo));
}