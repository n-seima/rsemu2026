#include "cCopyOrCutPrefixWnd.h"
#include "CHero.h"
#include "CMessage.h"

cCopyOrCutPrefixWnd		g_gwCopyOrCutPrefix;

//	열기
void
cCopyOrCutPrefixWnd::open(int _iSrcItem,int _iDestItem, BOOL _bIsCut)
{
	cItem	*lpSrcItem	=	g_hero.getInventoryItem(_iSrcItem);
	cItem	*lpDestItem	=	g_hero.getInventoryItem(_iDestItem);

	if	(!lpSrcItem	||	!lpDestItem)
		return;

	if	(lpDestItem->getPrefixCount()	<=	0)
		return;
	


	ClosePopupWindow();

	m_iSrcItem			=	_iSrcItem;
	m_iDestItem			=	_iDestItem;
	m_bIsCut			= _bIsCut;
	int	iWindowWidth			=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iWindowHeight			=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iWindowTitleHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_PARTY);
	int	iBarWidth				=	g_sprInterface.getSpriteWidth(eBUTTON_NORMAL);
	int	iBarHeight				=	g_sprInterface.getSpriteHeight(eBUTTON_NORMAL);

	s_iPopupInterface			=	ePIW_COPY_OR_CUT_PREFIX;
	m_iSelectPrefix				=	0;
	s_rectInterfaceSize.x1		=	g_iScreenWidth-iWindowWidth;
	s_rectInterfaceSize.x2		=	g_iScreenWidth;

	m_pos.x						=	g_iScreenWidth-iWindowWidth;
	m_pos.y						=	iWindowTitleHeight+g_iCorrectBI_Y/2;

	{
		int	iX					=	m_pos.x+iWindowWidth-6-iBarWidth;
		int	iY					=	m_pos.y+iWindowHeight-9-iBarHeight;

		m_bmMenu.init(&g_sprInterface,2);

		m_bmMenu.addImageBar(dMSG_CANCEL		,IDCANCEL	,iX,iY,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
		iX						-=	(iBarWidth+2);
		if(m_bIsCut)
			m_bmMenu.addImageBar(dMSG_CUT	,IDOK		,iX,iY,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
		else
			m_bmMenu.addImageBar(dMSG_COPY	,IDOK		,iX,iY,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);

		m_bmMenu.setClicktAllBarMargin(1,1);
		m_bmMenu.setAllBarTextColor(0,FALSE);
	}

	{
		for	(int i=0;i<dITEM_PREFIX_COUNT;i++)
		{
			cItem	item;
			
			item.copy(lpDestItem);
			
			if	(lpDestItem->m_aPrefix[i].m_wPrefix	==	0xffff)
				break;
			
			if(m_bIsCut)
			{
				for	(int j=i;j<dITEM_PREFIX_COUNT-1;j++)
					memcpy(&item.m_aPrefix[j],&item.m_aPrefix[j+1],sizeof(CItemPrefixInfo));
				
				item.m_aPrefix[dITEM_PREFIX_COUNT-1].m_wPrefix	=	0xffff;
			}
			else
			{
				for(int j=0;j<dITEM_PREFIX_COUNT;++j)
				{
					if(item.m_aPrefix[j].m_wPrefix == 0xffff)
					{
						memcpy(&item.m_aPrefix[j],&item.m_aPrefix[i],sizeof(CItemPrefixInfo));
						break;
					}
				}
			}
			m_aEquipLevel[i]	=	item.getRequireLevel();
		}
	}
}

void
cCopyOrCutPrefixWnd::close()
{
	s_rectInterfaceSize.set(0, 0, 0, 0);
	s_iPopupInterface		=	ePIW_NONE;

	cINPDEV::ReleaseExclusive();
}


int
cCopyOrCutPrefixWnd::getFocusPrefix()
{
	cItem	*lpDestItem		=	g_hero.getInventoryItem(m_iDestItem);

	int		iX,iY,i,iCount=0;

	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iContentsWidth	=	iWindowWidth-8;

	iX		=	m_pos.x+14;
	iY		=	m_pos.y+58;

	for	(i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		CItemPrefixInfo	*lpItemPrefixInfo	=	&lpDestItem->m_aPrefix[i];

		if	(lpItemPrefixInfo->m_wPrefix	==	0xffff)
			break;

		cITEM_PREFIX	*lpPrefix			=	&g_aItemPrefix[lpItemPrefixInfo->m_wPrefix];
		char			*lpstrComment		=	g_itemPrefix.getComment(lpItemPrefixInfo);
		char			strText[256];
		cRECT			rect;

		sprintf(strText,dMSG_REMOVE_PREFIX_LIST_FORM,lpPrefix->m_str1stPrefix,lpstrComment,m_aEquipLevel[i]);
		int				iTextHeight			=	60;
		int				iHeight				=	iTextHeight+6;

		rect.set(iX-2,iY-2,iX+iContentsWidth-14,iY+iTextHeight+2);

		if	(rect.isIn(s_posMouse.x,s_posMouse.y))
			return	i;

		iY									+=	iHeight;
	}

	return	0xffff;
}

void
cCopyOrCutPrefixWnd::updateBarmenu()
{
	cItem	*lpDestItem		=	g_hero.getInventoryItem(m_iDestItem);

	DWORD	dwMenu			=	m_bmMenu.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

	switch(dwMenu)
	{
		case	IDOK		:
		{
			CItemPrefixInfo	*lpItemPrefixInfo	=	&lpDestItem->m_aPrefix[m_iSelectPrefix];

			if	(lpItemPrefixInfo->m_wPrefix	==	0xffff)
				break;

			cITEM_PREFIX	*lpPrefix			=	&g_aItemPrefix[lpItemPrefixInfo->m_wPrefix];
			char			*lpstrComment		=	g_itemPrefix.getComment(lpItemPrefixInfo);
			char			strText[256];


			if(m_bIsCut)
			{
				sprintf(strText,dMSG_CONFIRM_CUT_PREFIX_FORM,lpPrefix->m_str1stPrefix,lpstrComment,m_aEquipLevel[m_iSelectPrefix]);
				
				g_msgBox.cPopup("",strText,dMSG_YES,dMSG_NO);
				g_msgBox.setButtonId(eMBI_CUT_SELECT_PREFIX);
			}
			else
			{
				sprintf(strText,dMSG_CONFIRM_COPY_PREFIX_FORM,lpPrefix->m_str1stPrefix,lpstrComment,m_aEquipLevel[m_iSelectPrefix]);
				
				g_msgBox.cPopup("",strText,dMSG_YES,dMSG_NO);
				g_msgBox.setButtonId(eMBI_COPY_SELECT_PREFIX);
			}
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
cCopyOrCutPrefixWnd::update()
{
	if	(s_iPopupInterface	!=	ePIW_COPY_OR_CUT_PREFIX)
		return;
	if	(cINPDEV::IsExclusive(eMOUSE_STATE_INTERFACE))
		return;

	s_rectInterfaceSize.x1 = g_iScreenWidth-g_sprInterface.getSpriteWidth(eWINDOW);
	s_rectInterfaceSize.x2 = g_iScreenWidth;

	updateBarmenu();

	if	(cINPDEV::IsExclusive())
		return;

	m_iFocusPrefix			=	getFocusPrefix();

	if	(s_isTouchLeftButton && !s_bmMenu.isPressed()	&&	m_iFocusPrefix	!=	0xffff)
		m_iSelectPrefix		=	m_iFocusPrefix;
}

void
cCopyOrCutPrefixWnd::draw()
{
	cItem	*lpDestItem		=	g_hero.getInventoryItem(m_iDestItem);

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

	//	아이템과 아이템 이름 출력
	{
		iX	+=	12;
		iY	+=	10;

		g_sprInterface.Put(iX,iY,eIWI_ITEM_SLOT_FRAME);
		PutItem(lpDestItem,iX,iY,FALSE,FALSE,FALSE);

		s_text.cPut(iX+40,iY-2,iWindowWidth-60,40,0,WHITE,dFITTEXT_HEIGHT_CENTER,"%s",lpDestItem->getName());

		g_sprInterface.Put(m_pos.x+7,64+g_iCorrectBI_Y/2		,eWINDOW_SEPERATE_BAR);	//	경계
	}

	iX		=	m_pos.x+14;
	iY		=	m_pos.y+58;

	for	(i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		CItemPrefixInfo	*lpItemPrefixInfo	=	&lpDestItem->m_aPrefix[i];

		if	(lpItemPrefixInfo->m_wPrefix	==	0xffff)
			break;

		cITEM_PREFIX	*lpPrefix			=	&g_aItemPrefix[lpItemPrefixInfo->m_wPrefix];
		char			*lpstrComment		=	g_itemPrefix.getComment(lpItemPrefixInfo);
		char			strText[256];
		
		if(m_bIsCut)
		{
			sprintf(strText,dMSG_CUT_PREFIX_LIST_FORM,lpPrefix->m_str1stPrefix,lpstrComment,m_aEquipLevel[i]);
		}
		else
		{
			sprintf(strText,dMSG_COPY_PREFIX_LIST_FORM,lpPrefix->m_str1stPrefix,lpstrComment,m_aEquipLevel[i]);
		}
		int				iTextHeight			=	60;

		if	(m_iSelectPrefix	==	i)
			cDRAW::FillHB(RGB24To16(0,100,200),iX-2,iY-2,iX+iContentsWidth-14,iY+iTextHeight+2);	//	선택한 아이템이다.

		s_text.cPut(iX+2,iY,iContentsWidth-14,iTextHeight,0,WHITE,dFITTEXT_HEIGHT_CENTER,strText);

		iY									+=	iTextHeight+6;
	}

	g_sprInterface.Put(m_pos.x+7,463+g_iCorrectBI_Y/2		,eWINDOW_SEPERATE_BAR);	//	경계

	m_bmMenu.draw();
}

