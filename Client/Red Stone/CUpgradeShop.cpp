#include "CUpgradeShop.h"
#include "CHero.H"
#include "cMESSAGE.h"
CUpgradeShop	g_gwUpgradeShop;


enum
{
	
	ePSM_UPGRADE,
	ePSM_CANCEL,


};



CUpgradeShop::CUpgradeShop()
{
	m_iUpgradeWeaponItemSlot		= 0xffff;	
	m_iUpgradeItemSlot				= 0xffff;
	m_iUpgradeNormalItemSlot		= 0xffff;
	m_iCosmicPowerItemSerial		= 0xffff;		
	m_bUpgradePerfect				= FALSE;
}


void
CUpgradeShop::init()
{
	m_iHeightRate				= 52;
	m_iUpgradeWeaponItemSlot	= 0xffff;	
	m_iUpgradeItemSlot			= 0xffff;
	m_iUpgradeNormalItemSlot	= 0xffff;
	m_iDragItem					= 0xffff;

	m_iCosmicPowerItemSerial		= getCosmicPowerItemSerial();

	m_iSlotScale				=	100;

	int		iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW) * m_iHeightRate/100.0;
	int		iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iSlotHeight			=	g_sprInterface.getSpriteHeight(eIWI_ITEM_SLOT_FRAME)* m_iSlotScale/100.0;
	int		iSlotWidth			=	g_sprInterface.getSpriteWidth(eIWI_ITEM_SLOT_FRAME) * m_iSlotScale/100.0;
	int		iDistance			=	30;
	int		iStartPointX		=	(iWindowWidth - (iSlotWidth*3 + iDistance*2))/2;


	m_rtUpgradeWindow.x1				=	g_iScreenWidth-iWindowWidth*2;
	m_rtUpgradeWindow.y1				=	g_iCorrectBI_Y/2;
	m_rtUpgradeWindow.x2				=	g_iScreenWidth-iWindowWidth;
	m_rtUpgradeWindow.y2				=	iWindowHeight+g_iCorrectBI_Y/2;
	
	m_rtUpgradeWeaponItemSlot.x1		=	iStartPointX;
	m_rtUpgradeWeaponItemSlot.y1		=	iDistance*2 - 10;
	m_rtUpgradeWeaponItemSlot.x2		=	m_rtUpgradeWeaponItemSlot.x1 + iSlotWidth;
	m_rtUpgradeWeaponItemSlot.y2		=	m_rtUpgradeWeaponItemSlot.y1 + iSlotHeight;

	m_rtUpgradeItemSlot.x1				=	m_rtUpgradeWeaponItemSlot.x1 + iSlotWidth + iDistance;
	m_rtUpgradeItemSlot.y1				=	iDistance;
	m_rtUpgradeItemSlot.x2				=	m_rtUpgradeItemSlot.x1 + iSlotWidth;
	m_rtUpgradeItemSlot.y2				=	m_rtUpgradeItemSlot.y1 + iSlotHeight;

	m_rtUpgradeNormalItemSlot.x1		=	m_rtUpgradeWeaponItemSlot.x1 + iSlotWidth + iDistance;
	m_rtUpgradeNormalItemSlot.y1		=	m_rtUpgradeItemSlot.y2 + iDistance - 25;
	m_rtUpgradeNormalItemSlot.x2		=	m_rtUpgradeNormalItemSlot.x1 + iSlotWidth;
	m_rtUpgradeNormalItemSlot.y2		=	m_rtUpgradeNormalItemSlot.y1 + iSlotHeight;

	m_rtCosmicPowerSlot.x1				=	m_rtUpgradeItemSlot.x1 + iSlotWidth + iDistance;
	m_rtCosmicPowerSlot.y1				=	m_rtUpgradeWeaponItemSlot.y1;
	m_rtCosmicPowerSlot.x2				=	m_rtCosmicPowerSlot.x1 + iSlotWidth;
	m_rtCosmicPowerSlot.y2				=	m_rtCosmicPowerSlot.y1 + iSlotHeight;

	m_rtNextLevelItemSlot.x1			=	m_rtUpgradeItemSlot.x1;
	m_rtNextLevelItemSlot.y1			=	m_rtUpgradeNormalItemSlot.y2+ iDistance;
	m_rtNextLevelItemSlot.x2			=	m_rtNextLevelItemSlot.x1 + iSlotWidth;
	m_rtNextLevelItemSlot.y2			=	m_rtNextLevelItemSlot.y1 + iSlotHeight;




	s_bmMiddleWindowButton.reset();
	int		iButtonHeight	=	g_sprInterface.getSpriteHeight(eBUTTON_NORMAL);
	int		iButtonWidth	=	g_sprInterface.getSpriteWidth(eBUTTON_NORMAL);
	int iButtonDistance	=	25;
	int iButtonStartX	=	(iWindowWidth - (iButtonWidth*2 + iButtonDistance))/2 + m_rtUpgradeWindow.x1;
	int iButtonY	=	m_rtUpgradeWindow.y2 - 30;



	s_bmMiddleWindowButton.addImageBar(	dMSG_UPGRADE	,ePSM_UPGRADE		,iButtonStartX,iButtonY,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
	s_bmMiddleWindowButton.addImageBar(dMSG_CANCEL	,ePSM_CANCEL		,iButtonStartX +iButtonWidth+ iButtonDistance ,iButtonY,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
	s_bmMiddleWindowButton.setClickMargin(ePSM_UPGRADE,1,1);
	s_bmMiddleWindowButton.setClickMargin(ePSM_CANCEL,1,1);
	s_bmMiddleWindowButton.setAllBarTextColor(0,FALSE);

}


void 
CUpgradeShop::open()
{
	g_hero.lockOff();
	cINPDEV::ReleaseExclusive();
	ResetAllFocus();
	ClosePopupWindow();

	s_iPopupInterface		=	ePIW_UPGRADE_SHOP;
	init();


	SetInventoryWindowPos();
	InitPopupWindowCloseButton();
}	
BOOL
CUpgradeShop::isOpened()
{
	if(s_iPopupInterface	!=	ePIW_UPGRADE_SHOP)
		return FALSE;

	return TRUE;
}

void
CUpgradeShop::close()
{
	s_iPopupInterface		=	ePIW_NONE;
	cINPDEV::ReleaseExclusive();
	
	CloseInventoryWindow();
}

int
CUpgradeShop::getCosmicPowerItemSerial()			// 코스믹 파워 아이템을 찾아~~ ~_~;
{

	int iChance = 0;
	int iSerial = 0xffff;
	for(int i=0;i<dEQUIPMENT_PART_COUNT;++i)
	{
		cItem	*lpItem	=	&g_hero.m_aEquip[i];
		
		cBasicItem*	lpBasic = lpItem->getBasicItem(TRUE);
		if(lpItem->m_wBaseItem == 0xffff)
			continue;
		if(!lpBasic)
			continue;
		if(lpBasic->isCosmicPower())
		{
			if(lpBasic->getUpgradeCosmicPowerSuccessChance() >= iChance)
			{
				iChance = lpBasic->getUpgradeCosmicPowerSuccessChance();
				iSerial	= lpItem->m_wBaseItem;
			}
			

		}
			
	}
	
	for (int j=0;j<dOWN_ITEM_COUNT;++j)
	{
		cItem	*lpItem	=	&g_hero.m_aItems[j];
		cBasicItem*	lpBasic = lpItem->getBasicItem(TRUE);
		if(lpItem->m_wBaseItem == 0xffff)
			continue;
		if(!lpBasic)
			continue;
		if(lpBasic->isCosmicPower())
		{
			if(lpBasic->getUpgradeCosmicPowerSuccessChance() >= iChance)
			{
				iChance = lpBasic->getUpgradeCosmicPowerSuccessChance();
				iSerial	= lpItem->m_wBaseItem;
			}
			

		}
		
	}
	
	for(int k=0;k<g_hero.m_bf6ExtraInventorySize;++k)
	{
		cItem	*lpItem	=	&g_hero.m_aExtraInventory[k];
		cBasicItem*	lpBasic = lpItem->getBasicItem(TRUE);
		if(lpItem->m_wBaseItem == 0xffff)
			continue;
		if(!lpBasic)
			continue;
		if(lpBasic->isCosmicPower())
		{
			if(lpBasic->getUpgradeCosmicPowerSuccessChance() >= iChance)
			{
				iChance = lpBasic->getUpgradeCosmicPowerSuccessChance();
				iSerial	= lpItem->m_wBaseItem;
			}
		}
	}
	
	return iSerial;

}
BOOL
CUpgradeShop::isInSlot(cRECT* _rtSlot, int _iPosX, int _iPosY)
{
	
	cRECT rt;
	rt.x1 = m_rtUpgradeWindow.x1 + _rtSlot->x1;
	rt.x2 = rt.x1 + (_rtSlot->x2 - _rtSlot->x1);
	rt.y1 = m_rtUpgradeWindow.y1 + _rtSlot->y1;
	rt.y2 = rt.y1 + (_rtSlot->y2 - _rtSlot->y1);
	
	return rt.isIn(_iPosX, _iPosY);
}

int
CUpgradeShop::getFocusInItem()
{

	int iSerial = 0xffff;

	if(isInSlot(&m_rtUpgradeWeaponItemSlot, s_posMouse.x, s_posMouse.y))
	{
		iSerial = m_iUpgradeWeaponItemSlot;
	}
	
	if(isInSlot(&m_rtUpgradeItemSlot, s_posMouse.x, s_posMouse.y))
	{
		iSerial = m_iUpgradeItemSlot;
	}
	if(isInSlot(&m_rtUpgradeNormalItemSlot, s_posMouse.x, s_posMouse.y))
	{
		iSerial = m_iUpgradeNormalItemSlot;
	}

	return iSerial;

}

void 
CUpgradeShop::operateDragItemInUpgradeWindow()
{
	int	iFocusItem	=	getFocusInItem();



	if (cINPDEV::IsExclusive(eMOUSE_STATE_UPGRADE_SHOP_DRAG))
		return;

	if (s_isTouchLeftButton	&&	m_iDragItem	==	0xffff)
	{
		if(iFocusItem == 0xffff)
			return;
		m_iDragItem	=	iFocusItem;

		cINPDEV::SetExclusive(eMOUSE_STATE_UPGRADE_SHOP_DRAG);
	}

	if (s_isLeftButtonPressed	||	m_iDragItem	==	0xffff	)
		return;

	int		iX	=	s_posInventoryWindow.x;
	int		iY	=	s_posInventoryWindow.y;
	int		iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	cRECT	rectFrame;

//	아이템 창
	rectFrame.set(iX+18,iY,iX+iWindowWidth,iY+iWindowHeight);

	if (rectFrame.isIn(s_posMouse.x,s_posMouse.y))
	{
		if(m_iUpgradeWeaponItemSlot == m_iDragItem)
			m_iUpgradeWeaponItemSlot = 0xffff;
		if(m_iUpgradeItemSlot == m_iDragItem)
			m_iUpgradeItemSlot = 0xffff;
		if(m_iUpgradeNormalItemSlot == m_iDragItem)
			m_iUpgradeNormalItemSlot = 0xffff;
	}
		
		

	cINPDEV::ReleaseExclusive();

	m_iDragItem	=	0xffff;
}
void
CUpgradeShop::update()
{
	if(!isOpened())
		return;
	

	DWORD	dwMenu	=	s_bmMiddleWindowButton.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);
	OperateInventoryWindow();
	OperateDragItem();
	operateDragItemInUpgradeWindow();

	switch(dwMenu)
	{

	
	case ePSM_UPGRADE :
		{

			cItem* lpWeaponItem = g_hero.getInventoryItem(m_iUpgradeWeaponItemSlot);

			if(!lpWeaponItem)
				break;
			
			cBasicItem*		lpBasic		= lpWeaponItem->getBasicItem(TRUE);
			
			if(!lpBasic)
				break;

			int iCurrentItemLevel = lpBasic->getCurrentEternalItemLevel();
			
			s_agent.sendEtcWork(eCEW_REALLY_ITEM_UPGRADE,iCurrentItemLevel);

				break;
		}
	case ePSM_CANCEL :
		close();
		break;

	}


}
void
CUpgradeShop::operateDropItem()
{

	if(s_iDragItem != 0xffff)
	{
		
		cItem	*lpItem	=	g_hero.getInventoryItem(s_iDragItem);
		
		if	(!lpItem)
			return;

		if(lpItem->isExpirationData())
		{
			return;
		}	
		
		cBasicItem*	lpBasic = lpItem->getBasicItem(TRUE);
		
		if(!lpBasic)
			return;
		int iCurrentLevel = lpBasic->getCurrentEternalItemLevel();
		
		if(lpBasic->isEternalItem() && iCurrentLevel>0 && iCurrentLevel<dETERNAL_ITEM_MAX_LEVEL)
		{
			if(isInSlot(&m_rtUpgradeWeaponItemSlot, s_posMouse.x, s_posMouse.y))
			{
				m_iUpgradeWeaponItemSlot = s_iDragItem;
			}
			
		}
		else if(lpBasic->isUpgradeMaterialsItem() || lpBasic->isUpgradeEternalItemNotPenalty())
		{
			BOOL isPremium = FALSE;
			if(lpBasic->m_wKind == eIK_PREMIUM_ITEM)
				isPremium = TRUE;
			
			if(isInSlot(&m_rtUpgradeItemSlot, s_posMouse.x, s_posMouse.y) && isPremium )
			{				
				m_iUpgradeItemSlot = s_iDragItem;				
			}
			if(isInSlot(&m_rtUpgradeNormalItemSlot, s_posMouse.x, s_posMouse.y) && !isPremium )
			{				
				m_iUpgradeNormalItemSlot = s_iDragItem;				
			}

		}
	}


}
void
CUpgradeShop::draw()
{



	int		iX,iY,iCount	=	0;
	int		iLineCount		=	0;


	iX		=	m_rtUpgradeWindow.x1;
	iY		=	m_rtUpgradeWindow.y1;
	


	if (g_config.m_bIsInfoWindowHalfblending)
	{
		cDRAW::FillHB(0,iX,iY,m_rtUpgradeWindow.x2-4,  m_rtUpgradeWindow.y2);
	}else
	{
		cDRAW::Fill(0,iX,iY,m_rtUpgradeWindow.x2-4,  m_rtUpgradeWindow.y2);
	}

	
	
	g_sprInterface.Put(iX	,iY		,eWINDOW,100,m_iHeightRate);

	
	
	g_sprInterface.Put(m_rtUpgradeWeaponItemSlot.x1 + iX , m_rtUpgradeWeaponItemSlot.y1 + iY	,eIWI_ITEM_SLOT_FRAME, m_iSlotScale, m_iSlotScale);
	g_sprInterface.Put(m_rtUpgradeItemSlot.x1 + iX	 	 , m_rtUpgradeItemSlot.y1 + iY			,eIWI_ITEM_SLOT_FRAME, m_iSlotScale, m_iSlotScale);
	g_sprInterface.Put(m_rtUpgradeNormalItemSlot.x1 + iX , m_rtUpgradeNormalItemSlot.y1 + iY	,eIWI_ITEM_SLOT_FRAME, m_iSlotScale, m_iSlotScale);
	g_sprInterface.Put(m_rtNextLevelItemSlot.x1 + iX	 , m_rtNextLevelItemSlot.y1 + iY		,eIWI_ITEM_SLOT_FRAME, m_iSlotScale, m_iSlotScale);
	g_sprInterface.Put(m_rtCosmicPowerSlot.x1 + iX		 , m_rtCosmicPowerSlot.y1 + iY			,eIWI_ITEM_SLOT_FRAME, m_iSlotScale, m_iSlotScale);

	if(m_iUpgradeWeaponItemSlot !=0xffff)
	{
		cItem* lpWeaponItem				= g_hero.getInventoryItem(m_iUpgradeWeaponItemSlot);
		int iPutX = m_rtUpgradeWeaponItemSlot.x1 + iX;
		int iPutY = m_rtUpgradeWeaponItemSlot.y1 + iY;
		
		cBasicItem*	lpBasic = lpWeaponItem->getBasicItem(TRUE);
		
		if(!lpBasic)
			return;

		int iNextWeaponSerial = lpBasic->getNextEternalItemSerial();
		g_sprInterface.Put(iPutX,iPutY,eIWI_ITEM_SLOT_ANM_1+((s_iFrameCounter/8)%5),m_iSlotScale,m_iSlotScale);
		
		if (lpWeaponItem->m_wBaseItem	!=	0xffff)
		{	
			PutItem(lpWeaponItem, iPutX, iPutY, TRUE);
		}

		cItem	lpNextWeaponItem;
		cItem::GenerateItemByBaseItem(&lpNextWeaponItem,iNextWeaponSerial,1);

		iPutX = m_rtNextLevelItemSlot.x1 + iX;
		iPutY = m_rtNextLevelItemSlot.y1 + iY;
		
		g_sprInterface.Put(iPutX,iPutY,eIWI_ITEM_SLOT_ANM_1+((s_iFrameCounter/8)%5),m_iSlotScale,m_iSlotScale);
		
		if (lpNextWeaponItem.m_wBaseItem	!=	0xffff)
		{	
			PutItem(&lpNextWeaponItem, iPutX, iPutY, TRUE);
		}

	}

	if(m_iUpgradeItemSlot != 0xffff)
	{
		cItem* lpUpgradeItem				= g_hero.getInventoryItem(m_iUpgradeItemSlot);
		int iPutX = m_rtUpgradeItemSlot.x1 + iX;
		int iPutY = m_rtUpgradeItemSlot.y1 + iY;
		
		g_sprInterface.Put(iPutX,iPutY,eIWI_ITEM_SLOT_ANM_1+((s_iFrameCounter/8)%5),m_iSlotScale,m_iSlotScale);
		
		if (lpUpgradeItem->m_wBaseItem	!=	0xffff)
		{	
			PutItem(lpUpgradeItem, iPutX, iPutY, FALSE);
		}

	}

	if(m_iUpgradeNormalItemSlot != 0xffff)
	{
		cItem* lpUpgradeItem				= g_hero.getInventoryItem(m_iUpgradeNormalItemSlot);
		int iPutX = m_rtUpgradeNormalItemSlot.x1 + iX;
		int iPutY = m_rtUpgradeNormalItemSlot.y1 + iY;
		
		g_sprInterface.Put(iPutX,iPutY,eIWI_ITEM_SLOT_ANM_1+((s_iFrameCounter/8)%5),m_iSlotScale,m_iSlotScale);
		
		if (lpUpgradeItem->m_wBaseItem	!=	0xffff)
		{	
			PutItem(lpUpgradeItem, iPutX, iPutY, FALSE);
		}

	}

	if(m_iCosmicPowerItemSerial != 0xffff)
	{

		cItem	addItem;
		cItem::GenerateItemByBaseItem(&addItem,m_iCosmicPowerItemSerial,1);

		int iPutX = m_rtCosmicPowerSlot.x1 + iX;
		int iPutY = m_rtCosmicPowerSlot.y1 + iY;
		
		g_sprInterface.Put(iPutX,iPutY,eIWI_ITEM_SLOT_ANM_1+((s_iFrameCounter/8)%5),m_iSlotScale,m_iSlotScale);
		
		if (addItem.m_wBaseItem	!=	0xffff)
		{	

			cBasicItem*	lpBasic	= addItem.getBasicItem(TRUE);
			if(lpBasic)
			{
				g_smiIconItem.putScale(lpBasic->m_wIconShape,iPutX,iPutY, m_iSlotScale);	
			}
		}
	}

	
	DrawInventoryWindow();



	s_bmMiddleWindowButton.draw();
	drawText();
	drawToolTip();
	

}
void
CUpgradeShop::drawText()
{

	
	int		iX	=	m_rtUpgradeWindow.x1;
	int		iY	=	m_rtUpgradeWindow.y1;
	int		iEffectGapX = -15;
	int		iEffectGapY = 4;

	int iSlotGapX =  ((m_rtUpgradeItemSlot.x1 - m_rtUpgradeWeaponItemSlot.x2)/2);
	
	
	int iPlusPixelSize = s_text.getPixelSize(dMSG_LARGE_PLUS) + 4;
	int iPlusHeightSize = s_text.getTextHeight( s_text.getPixelSize(dMSG_LARGE_PLUS), dMSG_LARGE_PLUS) / 2;
	int iSlotCenterY = (m_rtUpgradeWeaponItemSlot.y2 - m_rtUpgradeWeaponItemSlot.y1)/2;

	s_text.put( m_rtUpgradeWeaponItemSlot.x2 + iSlotGapX + iX - iPlusPixelSize/2, m_rtUpgradeWeaponItemSlot.y1 + iSlotCenterY + iY - iPlusHeightSize ,WHITE, dMSG_LARGE_PLUS);
	s_text.put(m_rtUpgradeItemSlot.x2  + iSlotGapX + iX - iPlusPixelSize/2,m_rtUpgradeWeaponItemSlot.y1 + iSlotCenterY + iY -iPlusHeightSize,WHITE, dMSG_LARGE_PLUS);
	s_text.put(iX + (m_rtUpgradeWindow.x2-m_rtUpgradeWindow.x1)/2 - iPlusPixelSize/2, m_rtUpgradeNormalItemSlot.y2 + iSlotCenterY + iY - iPlusHeightSize,WHITE, dMSG_AN_INVERTED_TRIANGLE);

	return;

}
int
CUpgradeShop::getWidthGap(int _iFirstWidth, int _iSecondWidth)
{
	return (_iFirstWidth - _iSecondWidth)/2;
}

void
CUpgradeShop::drawToolTip()
{
	if(m_iDragItem != 0xffff)
		return;


	if(m_iUpgradeWeaponItemSlot !=0xffff)
	{
		cItem* lpWeaponItem				= g_hero.getInventoryItem(m_iUpgradeWeaponItemSlot);
		
		if(!lpWeaponItem)
			return;
		
		if(isInSlot(&m_rtUpgradeWeaponItemSlot, s_posMouse.x, s_posMouse.y))
		{
			
			char* strComment = lpWeaponItem->getItemTooltip();
			cRECT rt = m_rtUpgradeWeaponItemSlot;
			rt.x1 += m_rtUpgradeWindow.x1;
			rt.x1 = rt.x1 + m_rtUpgradeWeaponItemSlot.x2 - m_rtUpgradeWeaponItemSlot.x1;
			rt.y1	+=	g_iCorrectBI_Y/2;
			s_ttCommon.popupTip(&rt,-1,-1,&s_text,WHITE,strComment);

		}

		cBasicItem* lpBasic = lpWeaponItem->getBasicItem(TRUE);

		if(!lpBasic)
			return ;

		int iNextEternalItem = lpBasic->getNextEternalItemSerial();
		
		if(isInSlot(&m_rtNextLevelItemSlot, s_posMouse.x, s_posMouse.y))
		{
			CItemPrefixInfo		m_aWeaponPrefix[dITEM_PREFIX_COUNT];	
			memcpy(m_aWeaponPrefix, lpWeaponItem->m_aPrefix,sizeof(CItemPrefixInfo) * dITEM_PREFIX_COUNT);
			cItem	addItem;
			cItem::GenerateItemByBaseItem(&addItem,iNextEternalItem,1);
			memcpy(addItem.m_aPrefix, m_aWeaponPrefix,sizeof(CItemPrefixInfo) * dITEM_PREFIX_COUNT);
			cBasicItem* lpItem = addItem.getBasicItem(TRUE);

			if(lpItem)
				addItem.m_bDurability = lpItem->getDurability() * 100/100;

			char* strComment = addItem.getItemTooltip();
			cRECT rt = m_rtNextLevelItemSlot;
			rt.x1 += m_rtUpgradeWindow.x1;
			rt.x1 = rt.x1 + m_rtNextLevelItemSlot.x2 - m_rtNextLevelItemSlot.x1;
			rt.y1	+=	g_iCorrectBI_Y/2;			
			s_ttCommon.popupTip(&rt,-1,-1,&s_text,WHITE,strComment);
		
		}

	}
	else
	{

		if(isInSlot(&m_rtUpgradeWeaponItemSlot, s_posMouse.x, s_posMouse.y))
		{
			cRECT rt = m_rtUpgradeWeaponItemSlot;
			rt.x1 += m_rtUpgradeWindow.x1;
			rt.x1 = rt.x1 + m_rtUpgradeWeaponItemSlot.x2 - m_rtUpgradeWeaponItemSlot.x1;
			rt.y1	+=	g_iCorrectBI_Y/2;
			s_ttCommon.popupTip(&rt,-1,-1,&s_text,WHITE,dMSG_REGISTRATION_UPGRADE_ITEM);
		}
		if(isInSlot(&m_rtNextLevelItemSlot, s_posMouse.x, s_posMouse.y))
		{			
			cRECT rt = m_rtNextLevelItemSlot;
			rt.x1 += m_rtUpgradeWindow.x1;
			rt.x1 = rt.x1 + m_rtNextLevelItemSlot.x2 - m_rtNextLevelItemSlot.x1;
			rt.y1	+=	g_iCorrectBI_Y/2;			
			s_ttCommon.popupTip(&rt,-1,-1,&s_text,WHITE,dMSG_REGISTRATION_UPGRADE_NEXT_LEVEL_ITEM);
		}
	}

	if(m_iUpgradeItemSlot != 0xffff)
	{
		cItem* lpUpgradeItem				= g_hero.getInventoryItem(m_iUpgradeItemSlot);
		
		if(!lpUpgradeItem)
			return;
		
		if(isInSlot(&m_rtUpgradeItemSlot, s_posMouse.x, s_posMouse.y))
		{
			char* strComment = lpUpgradeItem->getItemTooltip();
			cRECT rt = m_rtUpgradeItemSlot;
			rt.x1 += m_rtUpgradeWindow.x1;
			rt.x1 = rt.x1 + m_rtUpgradeItemSlot.x2 - m_rtUpgradeItemSlot.x1;
			rt.y1	+=	g_iCorrectBI_Y/2;
			s_ttCommon.popupTip(&rt,-1,-1,&s_text,WHITE,strComment);

		}
	}
	else
	{
		if(isInSlot(&m_rtUpgradeItemSlot, s_posMouse.x, s_posMouse.y))
		{
			cRECT rt = m_rtUpgradeItemSlot;
			rt.x1 += m_rtUpgradeWindow.x1;
			rt.x1 = rt.x1 + m_rtUpgradeItemSlot.x2 - m_rtUpgradeItemSlot.x1;
			rt.y1	+=	g_iCorrectBI_Y/2;
			s_ttCommon.popupTip(&rt,-1,-1,&s_text,WHITE,dMSG_REGISTRATION_UPGRADE_PREMIUM_MATERIALS_ITEM);	
		}

	}

	if(m_iUpgradeNormalItemSlot != 0xffff)
	{
		cItem* lpUpgradeItem				= g_hero.getInventoryItem(m_iUpgradeNormalItemSlot);
		
		if(!lpUpgradeItem)
			return;
		
		if(isInSlot(&m_rtUpgradeNormalItemSlot, s_posMouse.x, s_posMouse.y))
		{
				

			char* strComment = lpUpgradeItem->getItemTooltip();
			
			cRECT rt = m_rtUpgradeNormalItemSlot;
			rt.x1 += m_rtUpgradeWindow.x1;
			rt.x1 = rt.x1 + m_rtUpgradeNormalItemSlot.x2 - m_rtUpgradeNormalItemSlot.x1;
			rt.y1	+=	g_iCorrectBI_Y/2;
			s_ttCommon.popupTip(&rt,-1,-1,&s_text,WHITE,strComment);

		}
	}
	else
	{
		if(isInSlot(&m_rtUpgradeNormalItemSlot, s_posMouse.x, s_posMouse.y))
		{
			cRECT rt = m_rtUpgradeNormalItemSlot;
			rt.x1 += m_rtUpgradeWindow.x1;
			rt.x1 = rt.x1 + m_rtUpgradeNormalItemSlot.x2 - m_rtUpgradeNormalItemSlot.x1;
			rt.y1	+=	g_iCorrectBI_Y/2;
			s_ttCommon.popupTip(&rt,-1,-1,&s_text,WHITE,dMSG_REGISTRATION_UPGRADE_MATERIALS_ITEM);	
		}

	}

	if(m_iCosmicPowerItemSerial != 0xffff)
	{
		if(isInSlot(&m_rtCosmicPowerSlot, s_posMouse.x, s_posMouse.y))
		{
			cItem	addItem;
			cItem::GenerateItemByBaseItem(&addItem,m_iCosmicPowerItemSerial,1);
			char* strComment = addItem.getItemTooltip();
			cRECT rt = m_rtCosmicPowerSlot;
			rt.x1 += m_rtUpgradeWindow.x1;
			rt.x1 = rt.x1 + m_rtCosmicPowerSlot.x2 - m_rtCosmicPowerSlot.x1;
			rt.y1	+=	g_iCorrectBI_Y/2;
			s_ttCommon.popupTip(&rt,-1,-1,&s_text,WHITE,strComment);

		}
		
	}
	else
	{
		if(isInSlot(&m_rtCosmicPowerSlot, s_posMouse.x, s_posMouse.y))
		{
			cRECT rt = m_rtCosmicPowerSlot;
			rt.x1 += m_rtUpgradeWindow.x1;
			rt.x1 = rt.x1 + m_rtCosmicPowerSlot.x2 - m_rtCosmicPowerSlot.x1;
			rt.y1	+=	g_iCorrectBI_Y/2;
			s_ttCommon.popupTip(&rt,-1,-1,&s_text,WHITE,dMSG_REGISTRATION_UPGRADE_COSMIC_POWER_ITEM);
		}

	}

}

CUpgradeShop::~CUpgradeShop()
{
	
}
