#include "mergeItemWnd.h"
#include "CHero.h"
#include "CMessage.h"
#include "cItem.h"
#include "pieceItem.h"
#include "dialog_box.H"

cMergeItemWnd	g_gwMergeItem;

void
cMergeItemWnd::build()
{
	int		i,j;
	BOOL	abIsUseItem[dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE];

	m_iItemCount	=	0;

	memset(abIsUseItem,0,sizeof(abIsUseItem));

	for	(i=0;i<dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE;i++)	//	먼저 빈 아이템이나 조각 아이템이 아닌 아이템은 골라 낸다.
	{
		cItem		*lpItem		=	g_hero.getInventoryItem(i);

		if	(!lpItem || lpItem->isUnknownItem())
		{
			abIsUseItem[i]			=	TRUE;
			continue;
		}

		if	(lpItem->getPieceItem()	==	NULL)
		{
			abIsUseItem[i]			=	TRUE;
			continue;
		}
	}

	for	(i=0;i<dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE;i++)
	{
		if	(m_iItemCount	>=	11)
			break;
		
		if	(abIsUseItem[i])
			continue;

		cItem		*lpItem		=	g_hero.getInventoryItem(i);
		cPieceItem	*lpFirstPiece=	lpItem->getPieceItem();		//	임의의 조각을 선택한후에

		abIsUseItem[i]			=	TRUE;

		cItemInfo	*lpItemInfo	=	&m_aItemList[m_iItemCount];

		m_iItemCount++;

		memcpy(&lpItemInfo->m_item,lpItem,sizeof(cItem));
		memset(lpItemInfo->m_awLackPieceList,0xff,sizeof(lpItemInfo->m_awLackPieceList));
		lpItemInfo->m_wPieceSerial		=	lpFirstPiece->m_wSerial;
		lpItemInfo->m_wExistPieceCount	=	1;
		lpItemInfo->m_wRequirePieceCount=	lpFirstPiece->m_wPieceCount;
		lpItemInfo->m_wItemSlot			=	i;
		lpItemInfo->m_wIsMerged			=	FALSE;

		BYTE	abExistPiece[dMAX_PIECE_ITEM_PIECE_COUNT];

		memset(abExistPiece,0,sizeof(abExistPiece));

		abExistPiece[lpItem->m_aOption[0]]	=	1;

		for	(j=0;j<dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE;j++)	//	동일한 아이템 조각 검색
		{
			if	(abIsUseItem[j])
				continue;

			lpItem						=	g_hero.getInventoryItem(j);

			cPieceItem	*lpPieceItem	=	lpItem->getPieceItem();

			if	(lpPieceItem->m_wSerial	==	lpFirstPiece->m_wSerial)
			{
				if	(abExistPiece[lpItem->m_aOption[0]])	//	이미 검색되서 포함된 조각이다.
					continue;

				abIsUseItem[j]						=	TRUE;	//	얘는 더 이상 검색 대상에 포함되지 않는다.
				abExistPiece[lpItem->m_aOption[0]]	=	1;
				lpItemInfo->m_wExistPieceCount++;
			}
		}

		int	iLackPieceCount	=	0;

		for	(j=0;j<lpItemInfo->m_wRequirePieceCount;j++)
		{
			if	(abExistPiece[j])
				continue;

			lpItemInfo->m_awLackPieceList[iLackPieceCount]	=	j;
		}
	}
}

void
cMergeItemWnd::open(int _iClientSerial)
{
	g_dialogBox.closeComplexDialog();

	m_iClientSerial	=	_iClientSerial;
	m_iSelectItem	=	0;

	build();

	if	(m_iItemCount	==	0)
	{
		AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_FIND_PIECE_ITEM_IN_INVENTORY);
		return;
	}

	cINPDEV::ReleaseExclusive();
	ResetAllFocus();
	ClosePopupWindow();

	s_lpInventoryWindowEquipmentList=	g_hero.m_aEquip;
	s_lpInventoryWindowItemList		=	g_hero.m_aItems;
	s_lpGold						=	&g_hero.m_iGold;

	int	iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iTitleHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_SHOP);
	int	iBarWidth			=	g_sprInterface.getSpriteWidth(eBUTTON_NORMAL);
	int	iBarHeight			=	g_sprInterface.getSpriteHeight(eBUTTON_NORMAL);

	s_iPopupInterface		=	ePIW_MERGE_ITEM;
	
	m_pos.x					=	g_iScreenWidth-iWindowWidth;
	m_pos.y					=	iTitleHeight+g_iCorrectBI_Y/2;
	
	int	iX					=	m_pos.x+iWindowWidth-6-iBarWidth;
	int	iY					=	m_pos.y+iWindowHeight-9-iBarHeight;

	m_bmMenu.init(&g_sprInterface,2);

	m_bmMenu.addImageBar(dMSG_CANCEL		,IDCANCEL	,iX,iY,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
	iX						-=	(iBarWidth+2);
	m_bmMenu.addImageBar(dMSG_MAKE			,IDOK		,iX,iY,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);

	m_bmMenu.setClicktAllBarMargin(1,1);
	m_bmMenu.setAllBarTextColor(0,FALSE);
}

void
cMergeItemWnd::close()
{
	s_rectInterfaceSize.set(0, 0, 0, 0);
	s_iPopupInterface		=	ePIW_NONE;

	cINPDEV::ReleaseExclusive();
}

void
cMergeItemWnd::updateBarmenu()
{
	cItemInfo	*lpItemInfo	=	&m_aItemList[m_iSelectItem];

	if	(lpItemInfo->m_wIsMerged)
		m_bmMenu.hide(IDOK);
	else
		m_bmMenu.show(IDOK);

	DWORD		dwMenu		=	m_bmMenu.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

	switch(dwMenu)
	{
		case	IDOK		:
		{
			char		strText[256];

			cPieceItem	*lpPieceItem	=	g_pieceItemManager.get(lpItemInfo->m_wPieceSerial);

			if	(!lpPieceItem	||	lpItemInfo->m_wIsMerged	||	lpItemInfo->m_wExistPieceCount	!=	lpItemInfo->m_wRequirePieceCount)
				return;

			cItem		item;

			lpPieceItem->generateItem(&item);

			int			iMergePrice		=	lpPieceItem->getMergePrice();

			sprintf(strText,dMSG_MERGE_ITEM_FORM,item.getName(),GetMoneyString(iMergePrice));

			g_msgBox.cPopup("",strText,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_MERGE_SELECT_ITEM);
			
			break;
		}

		case	IDCANCEL	:
			close();
			break;
	}

	if (m_bmMenu.isPressed())
		cINPDEV::SetExclusive(eMOUSE_STATE_INTERFACE);
	else
		cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_INTERFACE);
}

void
cMergeItemWnd::update()
{
	if	(s_iPopupInterface	!=	ePIW_MERGE_ITEM)
		return;
	if	(cINPDEV::IsExclusive(eMOUSE_STATE_INTERFACE))
		return;

	s_rectInterfaceSize.x1 = g_iScreenWidth-g_sprInterface.getSpriteWidth(eWINDOW);
	s_rectInterfaceSize.x2 = g_iScreenWidth;

	updateBarmenu();

	if	(cINPDEV::IsExclusive())
		return;

	m_iFocusItem			=	getFocusItem();

	if	(s_isTouchLeftButton && !s_bmMenu.isPressed()	&&	m_iFocusItem	!=	0xffff)
		m_iSelectItem		=	m_iFocusItem;
}


void
cMergeItemWnd::draw()
{
	int		iX,iY,i,iCount=0;

	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iContentsWidth	=	iWindowWidth-8;

	iX		=	m_pos.x;
	iY		=	m_pos.y;

	if	(g_config.m_bIsInfoWindowHalfblending)
		cDRAW::FillHB(0,iX+8,iY+6,iX+4+iContentsWidth,iY+iWindowHeight-8);
	else
		cDRAW::Fill	 (0,iX+8,iY+6,iX+4+iContentsWidth,iY+iWindowHeight-8);

	g_sprInterface.Put(iX,iY,eWINDOW);
	g_sprInterface2.Put(iX,g_iCorrectBI_Y/2,dETC_WND_TITLE_BAR_SPRITE_INDEX);

	iX		=	m_pos.x+12;
	iY		=	m_pos.y+10;

	//	아이템과 아이템 이름 출력
	cItem	item;

	for	(i=0;i<m_iItemCount;i++)
	{
		cItemInfo	*lpItemInfo	=	&m_aItemList[i];
		cItem		*lpItem		=	&lpItemInfo->m_item;
		cPieceItem	*lpPieceItem=	lpItem->getPieceItem();

		if	(lpPieceItem)
			if	(lpPieceItem->generateItem(&item))
				lpItem			=	&item;

		int			iDY			=	0;

		if	(i	==	m_iSelectItem)
		{
			cDRAW::Fill(RGB24To16(0,100,200),iX-2,iY-2-1,iX+iContentsWidth-14,iY+36-1);	//	선택한 아이템이다.
			iDY	=	-1;
		}

		g_sprInterface.Put(iX+2,iY+iDY,eIWI_ITEM_SLOT_FRAME);
		PutItem(lpItem,iX+2,iY+iDY,FALSE,FALSE,FALSE);

		if	(lpItemInfo->m_wIsMerged)
			s_text.cPut(iX+40+2,iY-2+iDY,iWindowWidth-60,40,0,WHITE,dFITTEXT_HEIGHT_CENTER,"%s<c:LTGREEN>[C]<n>",lpItem->getName());
		else
			s_text.cPut(iX+40+2,iY-2+iDY,iWindowWidth-60,40,0,WHITE,dFITTEXT_HEIGHT_CENTER,"%s[%d/%d]",lpItem->getName(),lpItemInfo->m_wExistPieceCount,lpItemInfo->m_wRequirePieceCount);

		iY		+=	40;
	}
	
	g_sprInterface.Put(m_pos.x+7,463+g_iCorrectBI_Y/2		,eWINDOW_SEPERATE_BAR);	//	경계

	m_bmMenu.draw();
}

int
cMergeItemWnd::getFocusItem()
{
	int		iX,iY,i,iCount=0;

	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iContentsWidth	=	iWindowWidth-8;

	iX		=	m_pos.x+12;
	iY		=	m_pos.y+10;

	//	아이템과 아이템 이름 출력
	for	(i=0;i<m_iItemCount;i++)
	{
		cRECT		rect;

		rect.set(iX,iY,iX+iContentsWidth,iY+40);

		if	(rect.isIn(s_posMouse.x,s_posMouse.y))
			return	i;

		iY		+=	40;
	}

	return	0xffff;
}

BOOL
cMergeItemWnd::isOpened()
{
	if	(s_iPopupInterface		==	ePIW_MERGE_ITEM)
		return	TRUE;

	return	FALSE;
}

int
cMergeItemWnd::getSelectItem()
{
	return	m_aItemList[m_iSelectItem].m_wItemSlot;
}

void
cMergeItemWnd::mergeItem(int _iItem)
{
	for	(int i=0;i<m_iItemCount;i++)
	{
		if	(m_aItemList[i].m_wItemSlot	==	_iItem)
		{
			m_aItemList[i].m_wIsMerged	=	TRUE;

			return;
		}
	}
}