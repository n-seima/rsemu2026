#include "CWindowInterface.H"
#include "cHero.h"
#include "cMessage.h"

const	int	s_iBBW_VerticalGap		=	30;
const	int	s_iBBW_Height			=	84;

enum
{
	eBOTTOMLESS_BOX_WINDOW_IMAGE	=	92,
};

cBottomlessBoxWindow	g_gwBottomlessBoxWindow;

void
cBottomlessBoxWindow::open(int _iItemSlot)
{
	m_iBottomlessBox	=	_iItemSlot;
	m_dwWaitToResultTime=	0;
	
	g_hero.lockOff();
	cINPDEV::ReleaseExclusive();
	ResetAllFocus();
	ClosePopupWindow();

	s_lpInventoryWindowEquipmentList=	g_hero.m_aEquip;
	s_lpInventoryWindowItemList		=	g_hero.m_aItems;
	s_lpGold						=	&g_hero.m_iGold;

	int	iWindowHeight		=	g_sprInterface.getSpriteHeight(eBOTTOMLESS_BOX_WINDOW_IMAGE);
	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW)+g_sprInterface2.getSpriteWidth(eBOTTOMLESS_BOX_WINDOW_IMAGE);
	int	iTitleHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_SHOP);

	s_iPopupInterface		=	ePIW_BOTTOMLESS_BOX;
	m_rect.x1				=	g_iScreenWidth-iWindowWidth+10;
	m_rect.y1				=	iTitleHeight+s_iBBW_VerticalGap+g_iCorrectBI_Y/2;
	m_rect.x2				=	g_iScreenWidth-g_sprInterface.getSpriteWidth(eWINDOW)-5;
	m_rect.y2				=	m_rect.y1+s_iBBW_Height-5;

	m_aMixItem[0].reset();
	m_aMixItem[1].reset();

	SetInventoryWindowPos();

	InitPopupWindowCloseButton();

	s_bmMiddleWindowButton.reset();

	s_bmMiddleWindowButton.addImageBar(dMSG_OK,IDOK		,m_rect.x1+16,100+g_iCorrectBI_Y/2,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
	s_bmMiddleWindowButton.setClickMargin(IDOK,1,1);
	s_bmMiddleWindowButton.setAllBarTextColor(0,FALSE);
}


void
cBottomlessBoxWindow::close()
{
	cINPDEV::ReleaseExclusive();

	CloseInventoryWindow();

	s_iPopupInterface	=	ePIW_NONE;
}

BOOL
cBottomlessBoxWindow::isOpened()
{
	if	(s_iPopupInterface		==	ePIW_BOTTOMLESS_BOX)
		return	TRUE;

	return	FALSE;
}

BOOL
cBottomlessBoxWindow::addItem(int _iItem)
{
	m_iFocusSlot	=	getFocusSlot();

	if	(m_iFocusSlot	==	0xffff)
		return	FALSE;

	m_aiMixItem[m_iFocusSlot]	=	_iItem;

	memcpy(&m_aMixItem[m_iFocusSlot],g_hero.getInventoryItem(_iItem),sizeof(cItem));

	return	TRUE;
}

void
cBottomlessBoxWindow::mix()
{
	if	(timeGetTime()	<	m_dwWaitToResultTime)
		return;

	m_dwWaitToResultTime	=	timeGetTime()+1000;

	s_agent.sendEtcWork(eCEW_USE_BOTTOMLESS_BOX,m_iBottomlessBox,m_aiMixItem[0],m_aiMixItem[1]);
}

void
cBottomlessBoxWindow::update()
{
	if	(!isOpened())
		return;

	OperateInventoryWindow();

	if	(cINPDEV::IsExclusive(eMOUSE_STATE_POPUP_MENU))
		return;

	m_iFocusSlot	=	getFocusSlot();

	if	(m_iFocusSlot	!=	0xffff)
	{
		if	(s_rectTooltip.isIn(s_posMouse.x,s_posMouse.y) && m_aMixItem[m_iFocusSlot].m_wBaseItem	!=	0xffff)
		{
			s_rectTooltip.add(8,dITEM_SHAPE_HEIGHT+8);

			char	*strComment	=	m_aMixItem[m_iFocusSlot].getItemTooltip(-1,-1,TRUE);

			strComment			=	g_hero.getAfterEquipInfo(strComment,&m_aMixItem[m_iFocusSlot]);

			s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,strComment,eTFM_NONE);
		}
	}

	DWORD	dwMenu	=	s_bmMiddleWindowButton.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);

	switch(dwMenu) 
	{
		case	IDOK	:
		{
			if(!g_hero.getValidInventorySlot())
			{
				AddSystemMessage(LTYELLOW,dMSG_PICK_ITEM_INVENTORY_FULL);
				break;
			}

			if	(m_aMixItem[0].m_wBaseItem	==	0xffff	||	m_aMixItem[1].m_wBaseItem	==	0xffff)
			{
				AddSystemMessage(LTYELLOW,dMSG_CNUB_BY_VALID_SLOT);
				break;
			}

			if	(m_aMixItem[0].getItemType()	!=	m_aMixItem[1].getItemType())
			{
				AddSystemMessage(LTYELLOW,dMSG_CNUB_BY_MISMATCH_ITEM_TYPE);
				break;
			}

			if	(c_aItemEquipPlace[m_aMixItem[0].getItemType()] == dITEM_KIND_WASTING  || c_aItemEquipPlace[m_aMixItem[0].getItemType()] == 0xffff)	
			{		// 변경.
				AddSystemMessage(LTYELLOW,dMSG_CNUB_BY_IS_ABLE_ONLY_EQUIPPMENT);
				break;
			}
			if	(m_aMixItem[0].getBasicItem()->m_bf1IsRingOfInfinity || m_aMixItem[1].getBasicItem()->m_bf1IsRingOfInfinity ||
				m_aMixItem[0].getCarvingLevel() ||m_aMixItem[1].getCarvingLevel() )
			{
				g_msgBox.cPopup("",dMSG_CNUB_BY_CAN_NOT_MIX_ITEM_COMMENT,dMSG_OK);
				break;
			}

			if	(m_aMixItem[0].getPrefixCount() + m_aMixItem[1].getPrefixCount() == 0)
			{
				AddSystemMessage(LTYELLOW,dMSG_CNUB_BY_PREFIX_ZERO);
				break;
			}

			if	(m_aMixItem[0].getSetItem() || m_aMixItem[1].getSetItem() || m_aMixItem[0].isExceptionItem() || m_aMixItem[1].isExceptionItem())
			{
				g_msgBox.cPopup("",dMSG_CNUB_BY_CAN_NOT_MIX_ITEM_COMMENT,dMSG_OK);
				break;
			}
			if	(m_aMixItem[0].isCanNotTradeItem() || m_aMixItem[1].isCanNotTradeItem())
			{
				g_msgBox.cPopup("",dMSG_CNUB_BY_CAN_NOT_MIX_ITEM_COMMENT,dMSG_OK);
				break;
			}
			if	(m_aMixItem[0].isExtraItem() || m_aMixItem[1].isExtraItem())
			{
				g_msgBox.cPopup("",dMSG_CNUB_BY_CAN_NOT_MIX_ITEM_COMMENT,dMSG_OK);
				break;
			}

			if	(m_aMixItem[0].getBasicItem()->isEternalItem() || m_aMixItem[1].getBasicItem()->isEternalItem())
			{
				g_msgBox.cPopup("",dMSG_CNUB_BY_CAN_NOT_MIX_ITEM_COMMENT,dMSG_OK);
				break;
			}

			if	(m_aMixItem[0].isLimitTermItem() || m_aMixItem[1].isLimitTermItem())
			{
				g_msgBox.cPopup("",dMSG_CNUB_BY_CAN_NOT_MIX_ITEM_COMMENT,dMSG_OK);
				break;
			}

			char	strItemName1[128];
			char	strItemName2[128];
			char	strText[512];

			strcpy(strItemName1,m_aMixItem[0].getName());
			strcpy(strItemName2,m_aMixItem[1].getName());

			sprintf(strText,dMSG_DO_YOU_REALLY_MIX_TWO_ITEM_FORM,strItemName1,strItemName2);

			g_msgBox.cPopup("",strText,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_USE_BOTTOMLESS_BOX);	//	길드전이 끝나서 마을로 돌아가기
			
			break;
		}
	}
}

int
cBottomlessBoxWindow::getFocusSlot()
{
	s_rectTooltip.set(m_rect.x1+7	,m_rect.y1+5,m_rect.x1+42,m_rect.y1+40);

	if	(s_rectTooltip.isIn(s_posMouse.x,s_posMouse.y))
		return	0;

	s_rectTooltip.set(m_rect.x1+7+40,m_rect.y1+5,m_rect.x1+40+42,m_rect.y1+40);

	if	(s_rectTooltip.isIn(s_posMouse.x,s_posMouse.y))
		return	1;

	return	0xffff;
}

void
cBottomlessBoxWindow::draw()
{
	if	(!isOpened())
		return;

	DrawInventoryWindow();

	int		iX,iY,iCount		=	0;
	int		iLineCount			=	0;
	int		iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);

	iX		=	m_rect.x1;
	iY		=	m_rect.y1;

	if	(g_config.m_bIsInfoWindowHalfblending)
		cDRAW::FillHB(0,m_rect.x1,m_rect.y1,m_rect.x2,m_rect.y2);
	else
		cDRAW::Fill  (0,m_rect.x1,m_rect.y1,m_rect.x2,m_rect.y2);

	if	(m_aMixItem[0].m_wBaseItem	!=	0xffff)
		PutItem(&m_aMixItem[0],m_rect.x1+9	 ,m_rect.y1+6,TRUE);
	if	(m_aMixItem[1].m_wBaseItem	!=	0xffff)
		PutItem(&m_aMixItem[1],m_rect.x1+9+40,m_rect.y1+6,TRUE);

	g_sprInterface2.Put(g_iScreenWidth-iWindowWidth,iY-s_iBBW_VerticalGap,eBOTTOMLESS_BOX_WINDOW_IMAGE);

	s_bmMiddleWindowButton.draw();
}