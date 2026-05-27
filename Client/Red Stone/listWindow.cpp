#include "CGamePlay.H"
#include "CActor.H"
#include "CMessage.H"
#include "CWindowInterface.H"

const	int	c_iContensCountInListWindow1			=	26;
const	int	c_iContensCountInListWindow2			=	25;
const	int	c_iCommonListWindowContentsLineHeight	=	16;
	
cListWindow	g_gwCommonList;

enum
{
	eCLW_TITLE_FIRST_BAR	=	72,
};

enum
{
	eMENU_OK,
	eMENU_CLOSE,
};

cListWindow::cListWindow()
{
}

void
cListWindow::reset()
{
	m_iFocus		=	-1;
	m_bExistBottomText=	FALSE;
	m_iCount		=	0;
	m_iSelectCount	=	0;
	m_iIncreaseCount=	0;
	m_iPoint		=	0;
	m_strBottomText[0]=	NULL;
	m_strTitle[0]	=	NULL;

	strcpy(m_strOkButtonText,"ok");

	m_bmPlus.reset();
	m_bmMinus.reset();
}

void
cListWindow::close()
{
	cINPDEV::ReleaseExclusive();
	reset();
	m_bmPlus.reset();
	m_bmMinus.reset();
}

void
cListWindow::open(int _iMode,BOOL _bIsWantPlusButton,BOOL _bIsSelectAble)
{
	if (s_iPopupInterface	==	ePIW_COMMON_LIST)
	{
		ClosePopupWindow();

		return;
	}

	ClosePopupWindow();

	m_bIsExistPlus			=	_bIsWantPlusButton;
	s_iPopupInterface		=	ePIW_COMMON_LIST;
	m_iMode					=	_iMode;
	m_bIsSelectAble			=	_bIsSelectAble;

	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iTabWidth			=	g_sprInterface.getSpriteWidth(eACTIVE_SKILL_TAB);
	int	iWindowTitleHeight	=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_PET);
	int	iTitleHeight		=	g_sprInterface2.getSpriteHeight(eCLW_TITLE_FIRST_BAR);

	s_rectInterfaceSize.x1 = g_iScreenWidth-iWindowWidth;
	s_rectInterfaceSize.x2 = g_iScreenWidth;

	m_pos.x		=	g_iScreenWidth-iWindowWidth;
	m_pos.y		=	iWindowTitleHeight+g_iCorrectBI_Y/2;

	s_rectInterfaceSize.y1 = m_pos.y-iWindowTitleHeight;
	s_rectInterfaceSize.y2 = m_pos.y+iWindowHeight;

	m_bExistBottomText	=	FALSE;

	int	iSBHeight		=	426;

	switch(m_iMode)
	{
		case	eCLW_SELECT_CH5_TITLE_BONUS	:
			m_bExistBottomText	=	TRUE;
			break;
	}

	int	iContentsCount	=	c_iContensCountInListWindow1;

	if	(m_bExistBottomText)
	{
		iSBHeight		-=	16;
		iContentsCount	=	c_iContensCountInListWindow2;
	}

	s_sbMiddleWindow.set(m_pos.x+iWindowWidth-16,m_pos.y+iTitleHeight+4,iSBHeight);
	s_sbMiddleWindow.setPos(0);
	s_sbMiddleWindow.setSize(m_iCount,iContentsCount);

	s_bmMiddleWindowButton.reset();

	s_bmMiddleWindowButton.addImageBar(m_strOkButtonText,eMENU_OK	,m_pos.x+140,470+g_iCorrectBI_Y/2,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
	s_bmMiddleWindowButton.addImageBar(dMSG_CLOSE		,eMENU_CLOSE,m_pos.x+200,470+g_iCorrectBI_Y/2,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
	s_bmMiddleWindowButton.setClickMargin(0,1,1);
	s_bmMiddleWindowButton.setClickMargin(1,1,1);
	s_bmMiddleWindowButton.setAllBarTextColor(0,FALSE);

	resetButtons();
}


void
cListWindow::init()
{
	m_bmPlus.init(&g_sprInterface,100);
	m_bmMinus.init(&g_sprInterface,100);
}

void
cListWindow::resetButtons()
{
	if	(m_bIsExistPlus	== FALSE)
		return;

	int	i;

	m_bmPlus.reset();
	m_bmMinus.reset();

	switch(m_iMode)
	{
		case	eCLW_SELECT_CH5_TITLE_BONUS	:
		{
			int	iRemainPoint	=	m_aiValue[0]-m_iIncreaseCount;

			if	(m_bIsExistPlus	&& iRemainPoint)
			{
				for (i=0;i<m_iCount;i++)
				{
					m_bmPlus.addImageBar("",m_aContentsList[i].m_iID,0,0,eSTATE_UP_NORMAL,eSTATE_UP_ACTIVE,eSTATE_UP_PRESSED,0xffff,eSTATE_UP_DISABLE);
					m_bmMinus.addImageBar("",m_aContentsList[i].m_iID,0,0,eSTATE_DOWN_NORMAL,eSTATE_DOWN_ACTIVE,eSTATE_DOWN_PRESSED,0xffff,eSTATE_DOWN_DISABLE);
				}
			}
		}
	}

}

//  
//	스킬 윈도우의 버튼들 위치 설정
void
cListWindow::resetButtonsPos()
{
	int	iRemainPoint	=	m_iPoint-m_iIncreaseCount;

	if	(m_bIsExistPlus	== FALSE)
		return;

	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iTitleHeight	=	g_sprInterface2.getSpriteHeight(eCLW_TITLE_FIRST_BAR);

	switch(m_iMode)
	{
		case	eCLW_SELECT_CH5_TITLE_BONUS	:
			iRemainPoint	=	m_aiValue[0]-m_iIncreaseCount;
			break;
	}

	int	iX	=	m_pos.x+9;
	int	iY	=	m_pos.y+iTitleHeight;

	int	i,iCount=0;

	for (i=0;i<m_iCount;i++)
	{
		m_bmPlus.hide(m_aContentsList[i].m_iID);
		m_bmMinus.hide(m_aContentsList[i].m_iID);
	}

	int	iContentsCount	=	c_iContensCountInListWindow1;

	if	(m_bExistBottomText)
		iContentsCount	=	c_iContensCountInListWindow2;

	for (i=0;i < m_iCount && iCount < iContentsCount;i++)
	{
		if	(i < s_sbMiddleWindow.m_iCurrentPos)
			continue;

		int		iPutX	=	iX;
		int		iPutY	=	iY+iCount*c_iCommonListWindowContentsLineHeight+8;
		int		iValue	=	m_aContentsList[i].m_iID;

		m_bmPlus.resetImageBar(iValue,"",iValue,iPutX		,iPutY,eSTATE_UP_NORMAL,eSTATE_UP_ACTIVE,eSTATE_UP_PRESSED,0xffff,eSTATE_UP_DISABLE);
		m_bmMinus.resetImageBar(iValue,"",iValue,iPutX+12	,iPutY,eSTATE_DOWN_NORMAL,eSTATE_DOWN_ACTIVE,eSTATE_DOWN_PRESSED,0xffff,eSTATE_DOWN_DISABLE);

		if	(m_aContentsList[i].m_iPlusValue)
			m_bmMinus.enable(iValue);
		else
			m_bmMinus.disable(iValue);

		if	(iRemainPoint)
			m_bmPlus.enable(iValue);
		else
			m_bmPlus.disable(iValue);

		iCount++;
	}
}

bool
cListWindow::addContents(char *_lpstrText,int _iID,int _iValue1,int _iValue2,int _iValue3,int _iValue4,int _iValue5,int _iValue6)
{
	if	(m_iCount	>=	c_iMaxListWindowContentsCount)
		return	false;

	strcpy(m_aContentsList[m_iCount].m_strText,_lpstrText);
	m_aContentsList[m_iCount].m_iID		=	_iID;

	m_aContentsList[m_iCount].m_aiValue[0]	=	_iValue1;
	m_aContentsList[m_iCount].m_aiValue[1]	=	_iValue2;
	m_aContentsList[m_iCount].m_aiValue[2]	=	_iValue3;
	m_aContentsList[m_iCount].m_aiValue[3]	=	_iValue4;
	m_aContentsList[m_iCount].m_aiValue[4]	=	_iValue5;
	m_aContentsList[m_iCount].m_aiValue[5]	=	_iValue6;

	m_aContentsList[m_iCount].m_bIsSelected	=	FALSE;
	m_aContentsList[m_iCount].m_iPlusValue	=	0;

	m_iCount	++;

	return	true;
}

int
cListWindow::getFocus()
{
	int	iX,iY,i,iCount=0;

	int	iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iTitleHeight	=	g_sprInterface2.getSpriteHeight(eCLW_TITLE_FIRST_BAR);

	iX		=	m_pos.x;
	iY		=	m_pos.y;

	s_text.pushShadowStatus(FALSE);
	s_text.put(iX+14,iY+8,0,m_strTitle);
	s_text.popShadowStatus();

	iX			=	m_pos.x+8;
	iY			=	m_pos.y+iTitleHeight+6;

	int	iContentsCount	=	c_iContensCountInListWindow1;

	if	(m_bExistBottomText)
		iContentsCount	=	c_iContensCountInListWindow2;

	for (i=0;i<m_iCount && iCount < iContentsCount;i++)
	{
		if	(i < s_sbMiddleWindow.m_iCurrentPos)
			continue;

		int	iPutX	=	iX+4;
		int	iPutY	=	iY+iCount*c_iCommonListWindowContentsLineHeight;

		iCount++;

		cRECT	rect;
		rect.set(iPutX,iPutY-2,iPutX+iWindowWidth-28,iPutY+c_iCommonListWindowContentsLineHeight-2);

		if	(rect.isIn(s_posMouse.x,s_posMouse.y))
		{
			m_rectFocus.set(&rect);

			m_iRealFocus	=	i;

			if	(m_aContentsList[i].m_iID	!=	-1)
				return	i;
		}
	}

	return	-1;
}

void
cListWindow::select(int _iIndex)
{
	for (int i=m_iCount;i;)
	{
		--i;

		if	(_iIndex	==	i)
		{
			m_aContentsList[i].m_bIsSelected	=	TRUE;
			continue;
		}
		else
		if	(m_bIsMultiSelect && _iIndex	!=	-1)
			continue;

		m_aContentsList[i].m_bIsSelected	=	FALSE;
	}
}

int
cListWindow::getSelectedCount()
{
	int	iCount	=	0;

	for (int i=m_iCount;i;)
	{
		--i;

		if	(m_aContentsList[i].m_bIsSelected)
			iCount++;
	}

	return	iCount;
}

int
cListWindow::getSelected()
{// 마지막 수정일 : 09.09.23

	for (int i=0;i<c_iMaxListWindowContentsCount;++i)
	{
		if	(m_aContentsList[i].m_bIsSelected)
			return	i;
	}

	return	-1;
}


//
//	아이템 인터페이스 업데이트
void
cListWindow::update()
{
	if	(s_iPopupInterface	!=	ePIW_COMMON_LIST)
		return;

	m_iRealFocus	=	-1;
	m_iFocus		=	-1;

	switch(m_iMode)
	{
		case	eCLW_SELECT_CH5_TITLE_BONUS			:
			s_bmMiddleWindowButton.hide(eMENU_OK);

			if	(m_aiValue[0]-m_iIncreaseCount	<=	0)
				s_bmMiddleWindowButton.show(eMENU_OK);
			break;

		case	eCLW_SELECT_CH5_NORMAL_ITEM_PREFIX_LEVEL_UP	:
		case	eCLW_SELECT_CH5_UNIQUE_ITEM_PREFIX_LEVEL_UP	:
		case	eCLW_SELECT_CH5_RELEASE_REVERSION	:
		case	eCLW_SELECT_CH5_REVERSE_STATE_POINT	:
		case	eCLW_SELECT_CH5_REVERSE_SKILL_POINT	:
		case	eCLW_SELECT_CH5_NORMAL_ITEM_PREFIX_UPGRADE	:
		case	eCLW_SELECT_CH5_UNIQUE_ITEM_PREFIX_UPGRADE	:
		case	eCLW_SELECT_DISJOINTING_ITEM				:
			s_bmMiddleWindowButton.hide(eMENU_OK);

			if	(getSelectedCount()	==	1)
				s_bmMiddleWindowButton.show(eMENU_OK);
			break;
	}

	resetButtonsPos();

	if	(!cINPDEV::IsExclusive(eMOUSE_STATE_SHOP_SCROLL_BAR))
	{
		if	(s_sbMiddleWindow.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed))
			
			cINPDEV::SetExclusive(eMOUSE_STATE_SHOP_SCROLL_BAR);
		else
			cINPDEV::ReleaseExclusive(TRUE,eMOUSE_STATE_SHOP_SCROLL_BAR);
	}

	if	(cINPDEV::IsExclusive(eMOUSE_STATE_BANK_WINDOW_BUTTONS))
		return;

	DWORD	dwMenu	=	s_bmMiddleWindowButton.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

	switch(dwMenu)
	{
		case	eMENU_OK	:
		{
			switch(m_iMode)
			{
				case	eCLW_SELECT_ALLIGNMENT	:
				{
					int	iSelected	=	getSelected();

					if	(iSelected	==	-1)
						break;

					int	iItem	=	m_aiValue[0];
					int	iValue	=	m_aContentsList[iSelected].m_iID;

					s_agent.sendEtcWork(eCEW_CH5_SELECT_ALLIGNMENT,iItem,iValue);

					ClosePopupWindow();
					break;
				}

				case	eCLW_SELECT_CH5_NORMAL_ITEM_PREFIX_LEVEL_UP	:
				case	eCLW_SELECT_CH5_UNIQUE_ITEM_PREFIX_LEVEL_UP	:
				case	eCLW_SELECT_CH5_NORMAL_ITEM_PREFIX_UPGRADE	:
				case	eCLW_SELECT_CH5_UNIQUE_ITEM_PREFIX_UPGRADE	:
				case	eCLW_SELECT_DISJOINTING_ITEM				:
				{
					int	iSelected	=	getSelected();

					if	(iSelected	==	-1)
						break;

					int	iSrcItem	=	m_aiValue[0];
					int	iDestItem	=	m_aContentsList[iSelected].m_iID;
					int	iSlot		=	m_aContentsList[iSelected].m_aiValue[0];

					if	(m_iMode	==	eCLW_SELECT_CH5_NORMAL_ITEM_PREFIX_LEVEL_UP)
						s_agent.sendEtcWork(eCEW_CH5_NORMAL_ITEM_PREFIX_LEVEL_UP,iSrcItem,iDestItem,iSlot);
					else
					if	(m_iMode	==	eCLW_SELECT_CH5_UNIQUE_ITEM_PREFIX_LEVEL_UP)
						s_agent.sendEtcWork(eCEW_CH5_UNIQUE_ITEM_PREFIX_LEVEL_UP,iSrcItem,iDestItem,iSlot);
					else
					if	(m_iMode	==	eCLW_SELECT_CH5_NORMAL_ITEM_PREFIX_UPGRADE)
						s_agent.sendEtcWork(eCEW_CH5_NORMAL_ITEM_UPGRADE,iSrcItem,iDestItem,iSlot);
					else
					if	(m_iMode	==	eCLW_SELECT_DISJOINTING_ITEM)
					{

						cItem	*lpEmptySlot	=	g_hero.getEmptyItemSlot();
						cItem	*lpDestItem		=	g_hero.getInventoryItem(iDestItem);		// 09.09.07 수정

						if(!lpDestItem)
						{
							AddSystemMessage(LTYELLOW , dMSG_DISJOINTING_ITEM_FAILED);
							break;
						}

						if(!lpDestItem->isUniqueItem())
						{
							AddSystemMessage(LTYELLOW , dMSG_DISJOINTING_ITEM_FAILED_IS_NOT_UNIQUE);
							break;
						}
						char strMessage[256] = "";
						sprintf(strMessage, "%s\n%s", lpDestItem->getName(), dMSG_REALLY_DISJOINTING_ITEM);
						g_msgBox.cPopup("",strMessage,dMSG_YES,dMSG_NO);		// 아이템 분해창 메세지박스 09.09.07	 
						g_msgBox.setButtonId(eMBI_DISJOINTING_ITEM);			// 아이템 분해창 버튼 아디~~ 09.09.07
						
					}
					else
						s_agent.sendEtcWork(eCEW_CH5_UNIQUE_ITEM_UPGRADE,iSrcItem,iDestItem,iSlot);

					ClosePopupWindow();
					break;
				}


				case	eCLW_SELECT_CH5_REVERSE_STATE_POINT	:
				{
					int	iSelected	=	getSelected();

					if	(iSelected	==	-1)
						break;

					s_agent.sendEtcWork(eCEW_CH5_RESET_ONE_STATE,m_aiValue[0],m_aContentsList[iSelected].m_iID);
					ClosePopupWindow();
					break;
				}

				case	eCLW_SELECT_CH5_REVERSE_SKILL_POINT	:
				{
					int	iSelected	=	getSelected();

					if	(iSelected	==	-1)
						break;

					s_agent.sendEtcWork(eCEW_CH5_RESET_ONE_SKILL,m_aiValue[0],m_aContentsList[iSelected].m_iID);
					ClosePopupWindow();
					break;
				}

				case	eCLW_SELECT_CH5_RELEASE_REVERSION	:
				{
					int	iSelected	=	getSelected();

					if	(iSelected	==	-1)
						break;

					s_agent.sendEtcWork(eCEW_CH5_RELEASE_ITEM_REVERSION,m_aiValue[0],m_aContentsList[iSelected].m_iID);
					ClosePopupWindow();
					break;
				}

				case	eCLW_SELECT_CH5_TITLE_BONUS	:
				{
					WORD	awValue[12];
					int		iCount	=	0;

					awValue[0]	=	m_aiValue[1];
					awValue[1]	=	m_iIncreaseCount;

					for (int i=0;i<m_iCount;i++)
					{
						for (int j=0;j<m_aContentsList[i].m_iPlusValue;j++)
							awValue[2+iCount++]	=	m_aContentsList[i].m_iID;
					}

					if	(iCount)
					{
						s_agent.sendEtcWork(eCEW_INCREASE_TITLE_LEVEL_BY_CH5,12,awValue);
						ClosePopupWindow();
					}

					break;
				}

// 				case	eCLW_SELECT_DISJOINTING_ITEM:
// 				{
// 					int	iSelected	=	getSelected();
// 					
// 					if	(iSelected	==	-1)
// 						break;
// 					
// 					int	iItem	=	m_aiValue[0];
// 					int	iValue	=	m_aContentsList[iSelected].m_iID;
// 					
// 					s_agent.sendEtcWork(eCEW_CH5_SELECT_ALLIGNMENT,iItem,iValue);
// 
// 					break;
// 				}
			}
			break;
		}
		case	eMENU_CLOSE	:
			ClosePopupWindow();
			break;
	}

//	스킬 레벨
	if	(m_bIsExistPlus)
	{
		dwMenu	=	m_bmPlus.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

		if	(dwMenu	!=	0xffffffff)
		{
			int	iIndex	=	m_bmPlus.getIndex(dwMenu);
			m_aContentsList[iIndex].m_iPlusValue++;
			m_iIncreaseCount++;
		}

		dwMenu	=	m_bmMinus.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

		if	(dwMenu	!=	0xffffffff)
		{
			int	iIndex	=	m_bmMinus.getIndex(dwMenu);
			m_aContentsList[iIndex].m_iPlusValue--;
			m_iIncreaseCount--;
		}

	}

	if	(m_bmMinus.isPressed() || m_bmPlus.isPressed() || s_bmMiddleWindowButton.isPressed())
		cINPDEV::SetExclusive(eMOUSE_STATE_BANK_WINDOW_BUTTONS);
	else
		cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_BANK_WINDOW_BUTTONS);

	if	(cINPDEV::IsExclusive())
		return;

	if	(m_bIsSelectAble)
	{
		m_iFocus	=	getFocus();

		if	(s_isTouchLeftButton)
		{
			BOOL	bIsSelect	=	TRUE;

			switch(m_iMode)
			{
				case	eCLW_SELECT_CH5_NORMAL_ITEM_PREFIX_LEVEL_UP	:
				case	eCLW_SELECT_CH5_UNIQUE_ITEM_PREFIX_LEVEL_UP	:
				case	eCLW_SELECT_CH5_NORMAL_ITEM_PREFIX_UPGRADE	:
				case	eCLW_SELECT_CH5_UNIQUE_ITEM_PREFIX_UPGRADE	:
				{
					if	(m_iRealFocus	==	-1)
						break;

					cListWindowContents	*lpContents	=	&m_aContentsList[m_iRealFocus];

					if	(lpContents->m_aiValue[0]	==	-1)
						bIsSelect	=	FALSE;

					break;
				}
				case	eCLW_SELECT_DISJOINTING_ITEM	:
					{
						if	(m_iRealFocus	==	-1)
							break;
						
						cListWindowContents	*lpContents	=	&m_aContentsList[m_iRealFocus];
						
						break;
				}
			}

			if	(bIsSelect)
				select(m_iFocus);
		}

		if	(m_iRealFocus	!=	-1)
		{
			char	strTT[1024]	=	"";
			char	*lpstrTT	=	NULL;

			m_rectFocus.add(-4,-2);

			cListWindowContents	*lpContents	=	&m_aContentsList[m_iRealFocus];

			cItem	*lpItem		=	NULL;

			switch(m_iMode)
			{
				case	eCLW_SELECT_CH5_REVERSE_STATE_POINT	:
				{
					sprintf(strTT,dMSG_WHEN_RESET_STATE_BONUS_POINT_TT_FORM,lpContents->m_aiValue[0]);
					lpstrTT	=	strTT;
					break;
				}
				case	eCLW_SELECT_CH5_REVERSE_SKILL_POINT	:
				{
					sprintf(strTT,dMSG_WHEN_RESET_SKILL_BONUS_POINT_TT_FORM,lpContents->m_aiValue[0]);
					lpstrTT	=	strTT;
					break;
				}

				case	eCLW_SELECT_CH5_NORMAL_ITEM_PREFIX_LEVEL_UP	:
				case	eCLW_SELECT_CH5_UNIQUE_ITEM_PREFIX_LEVEL_UP	:
				{
					lpItem	=	g_hero.getInventoryItem(lpContents->m_iID);

					if	(lpItem	&&	lpContents->m_aiValue[0]	!=	-1)
					{
						int				iNextPrefix		=	lpContents->m_aiValue[2];
						int				iPrefixIndex	=	lpContents->m_aiValue[0];
						int				iPrefix			=	lpItem->m_aPrefix[iPrefixIndex].m_wPrefix;
						cITEM_PREFIX	*lpPrefix		=	&g_aItemPrefix[iPrefix];
						cITEM_PREFIX	*lpNextPrefix	=	&g_aItemPrefix[iNextPrefix];

						char	*lpstrPrefixComment		=	g_itemPrefix.getComment(&lpItem->m_aPrefix[iPrefixIndex],TRUE);
						sprintf(strTT,"<c:LTGREEN>%s<n>\n- %s",lpPrefix->m_str1stPrefix,lpstrPrefixComment);

						char	*lpstrNextPrefixComment	=	g_itemPrefix.getComment(iNextPrefix);

						strcat(strTT,_ms("\n\n<c:LTGREEN>%s<n>\n- %s",lpNextPrefix->m_str1stPrefix,lpstrNextPrefixComment));

						lpstrTT	=	strTT;
						lpItem	=	NULL;
					}

					break;
				}

				case	eCLW_SELECT_CH5_NORMAL_ITEM_PREFIX_UPGRADE	:
				case	eCLW_SELECT_CH5_UNIQUE_ITEM_PREFIX_UPGRADE	:
				case	eCLW_SELECT_DISJOINTING_ITEM				:
				{
					lpItem	=	g_hero.getInventoryItem(lpContents->m_iID);

					if	(lpItem	&&	lpContents->m_aiValue[0]	!=	-1)
					{
						int				iPrefixIndex=	lpContents->m_aiValue[0];
						int				iPrefix		=	lpItem->m_aPrefix[iPrefixIndex].m_wPrefix;
						cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[iPrefix];
						char	*lpstrPrefixComment	=	g_itemPrefix.getComment(&lpItem->m_aPrefix[iPrefixIndex]);

						sprintf(strTT,"<c:LTGREEN>%s<n>\n\n- %s",lpPrefix->m_str1stPrefix,lpstrPrefixComment);

						strcat(strTT,_ms(dMSG_PREFIX_VALUE_REFORM_RANGE_FORM,
										lpItem->m_aPrefix[iPrefixIndex].m_aValue[0],lpPrefix->m_aValue[0][0],lpPrefix->m_aValue[0][1]));

						if	(lpPrefix->m_aValue[1][0],lpPrefix->m_aValue[1][1])
							strcat(strTT,_ms(dMSG_PREFIX_VALUE_REFORM_RANGE_FORM,
											lpItem->m_aPrefix[iPrefixIndex].m_aValue[1],lpPrefix->m_aValue[1][0],lpPrefix->m_aValue[1][1]));

						lpstrTT	=	strTT;
						lpItem	=	NULL;
					}

					break;
				}

				case	eCLW_SELECT_CH5_RELEASE_REVERSION	:
				{
					lpItem	=	g_hero.getInventoryItem(lpContents->m_iID);
					break;
				}
			}

			if	(lpItem)
			{
				lpstrTT			=	lpItem->getItemTooltip();
				WORD	*lpImage=	g_smiIconItem.get16Image(lpItem->getBasicItem()->m_wIconShape);

				s_ttCommon.popupTip(&m_rectFocus,-1,-1,&s_text,WHITE,lpstrTT,eTFM_LEFT);
				s_ttCommon.setImage(lpImage);
			}
			else
			if	(lpstrTT)
				s_ttCommon.popupTip(&m_rectFocus,-1,-1,&s_text,WHITE,lpstrTT,eTFM_LEFT);
		}
	}
}

//
//	스킬 정보
void
cListWindow::draw()
{
	int	iX,iY,i,iCount=0;

	int	iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iTitleHeight	=	g_sprInterface2.getSpriteHeight(eCLW_TITLE_FIRST_BAR);
	int	iTextDX	=	0;

	iX		=	m_pos.x;
	iY		=	m_pos.y;

	if	(g_config.m_bIsInfoWindowHalfblending)
		cDRAW::FillAB(0,iX+8,iY+6,iX+266-4,iY+482-8,24);
	else
		cDRAW::Fill  (0,iX+8,iY+6,iX+266-4,iY+482-8);

	g_sprInterface2.Put(iX	,g_iCorrectBI_Y/2	,66);
	g_sprInterface.Put(iX	,iY		,eWINDOW);

	g_sprInterface2.put3PieceBar(iX+12,iY+4,eCLW_TITLE_FIRST_BAR,iWindowWidth-20,100,100);

	s_text.pushShadowStatus(FALSE);
	s_text.put(iX+14,iY+8,0,m_strTitle);
	s_text.popShadowStatus();

	iX			=	m_pos.x+8;
	iY			=	m_pos.y+iTitleHeight+6;

	if	(m_bIsExistPlus)
		iTextDX	=	12;

	switch(m_iMode)
	{
		case	eCLW_SELECT_CH5_REVERSE_SKILL_POINT			:
		case	eCLW_SELECT_CH5_RELEASE_REVERSION			:
		case	eCLW_SELECT_CH5_NORMAL_ITEM_PREFIX_LEVEL_UP	:
		case	eCLW_SELECT_CH5_UNIQUE_ITEM_PREFIX_LEVEL_UP	:
		case	eCLW_SELECT_CH5_NORMAL_ITEM_PREFIX_UPGRADE	:
		case	eCLW_SELECT_CH5_UNIQUE_ITEM_PREFIX_UPGRADE	:
		case	eCLW_SELECT_DISJOINTING_ITEM				:
			iTextDX	=	16;
			break;
	}

	int	iContentsCount	=	c_iContensCountInListWindow1;

	if	(m_bExistBottomText)
		iContentsCount	=	c_iContensCountInListWindow2;

	for (i=0;i<m_iCount && iCount < iContentsCount;i++)
	{
		if	(i < s_sbMiddleWindow.m_iCurrentPos)
			continue;

		cListWindowContents	*lpContents	=	&m_aContentsList[i];

		int		iPutX	=	iX+4;
		int		iPutY	=	iY;
		DWORD	dwColor	=	WHITE;

		if	(m_bIsExistPlus)
			iPutX	+=	10;

		if	(lpContents->m_bIsSelected)
			cDRAW::Fill(_LTBLUE,iX+4-2,iY,iX+iWindowWidth - 28,iY+c_iCommonListWindowContentsLineHeight-2);
		else
		if	(i	==	m_iFocus)	
			cDRAW::Fill(_BLUE,iX+4-2,iY,iX+iWindowWidth - 28,iY+c_iCommonListWindowContentsLineHeight-2);

		switch(m_iMode)
		{
			case	eCLW_SELECT_CH5_REVERSE_SKILL_POINT		:
			{
				CSkill		*lpSkill	=	g_hero.m_aAbility[lpContents->m_iID].getSkill();
				g_smiIconSkill.putScale(lpSkill->m_wIconIndex,iPutX-2,iPutY,45);
				break;
			}

			case	eCLW_SELECT_CH5_NORMAL_ITEM_PREFIX_LEVEL_UP	:
			case	eCLW_SELECT_CH5_UNIQUE_ITEM_PREFIX_LEVEL_UP	:
			case	eCLW_SELECT_CH5_NORMAL_ITEM_PREFIX_UPGRADE	:
			case	eCLW_SELECT_CH5_UNIQUE_ITEM_PREFIX_UPGRADE	:
			case	eCLW_SELECT_DISJOINTING_ITEM				:
			{
				if	(lpContents->m_aiValue[1])
				{
					cItem	*lpItem	=	g_hero.getInventoryItem(lpContents->m_iID);
					g_smiIconItem.putScale(lpItem->getBasicItem()->m_wIconShape,iPutX-2,iPutY,50);
				}

				break;
			}
			case	eCLW_SELECT_CH5_RELEASE_REVERSION	:
			{
				cItem	*lpItem	=	g_hero.getInventoryItem(lpContents->m_iID);
				g_smiIconItem.putScale(lpItem->getBasicItem()->m_wIconShape,iPutX-2,iPutY,50);
				break;
			}
		}

		if	(lpContents->m_iPlusValue)
			s_sText.put(iPutX+iTextDX,iPutY+2,dwColor,"%s<c:LTGREEN>+%d<n>",lpContents->m_strText,lpContents->m_iPlusValue);
		else
			s_sText.put(iPutX+iTextDX,iPutY+2,dwColor,lpContents->m_strText);

		iY		+=	c_iCommonListWindowContentsLineHeight;
		iCount++;
	}

	iY		=	m_pos.y+iWindowHeight-36;

	switch(m_iMode)
	{
		case	eCLW_SELECT_CH5_TITLE_BONUS	:
			s_text.putRF(iX+iWindowWidth-16,iY-7,WHITE,m_strBottomText,m_iIncreaseCount,m_aiValue[0]);
			iY	-=	16;
			break;
	}


	g_sprInterface.Put(iX-1,iY,eWINDOW_SEPERATE_BAR);	//	경계

	m_bmPlus.draw();
	m_bmMinus.draw();
	s_bmMiddleWindowButton.draw();
	s_sbMiddleWindow.draw();
}