#include "CWindowInterface.H"
#include "cMessage.h"
#include "cHero.h"
#include "book.h"

cBookGPW	g_gwBook;

cBookGPW::cBookGPW()
{
	m_bIsInit	=	FALSE;
}

void
cBookGPW::open(int _iItemSlot)
{
	cItem	*lpItem		=	g_hero.getInventoryItem(_iItemSlot);

	if	(!lpItem)
		return;

	cBookItemDefine	*lpBookItem	=	lpItem->getBook();

	if	(!lpBookItem)
		return;

	if	(m_bIsInit	==	FALSE)
		m_button.init(&g_sprBook,6);

	int	iUpRightButtonWidth		=	g_sprBook.getSpriteWidth(eBI_EXIT_NORMAL);
	int	iBottomRightButtonWidth	=	g_sprBook.getSpriteWidth(eBI_NEXT_PAGE_NORMAL);
	int	iBottomButtonHeight		=	g_sprBook.getSpriteHeight(eBI_NEXT_PAGE_NORMAL)+20;

	m_button.addImageBar("",eBM_INDEX			,0	,20	,eBI_INDEX_NORMAL		,eBI_INDEX_LIGHT	,eBI_INDEX_LIGHT);
	m_button.addImageBar("",eBM_EXIT			,g_iScreenWidth-iUpRightButtonWidth	,20	,eBI_EXIT_NORMAL		,eBI_EXIT_LIGHT		,eBI_EXIT_LIGHT);
	m_button.addImageBar("",eBM_PREVIOUS_PAGE	,0	,g_iScreenHeight-iBottomButtonHeight,eBI_PRE_PAGE_NORMAL	,eBI_PRE_PAGE_LIGHT	,eBI_PRE_PAGE_LIGHT);
	m_button.addImageBar("",eBM_NEXT_PAGE		,g_iScreenWidth-iBottomRightButtonWidth,g_iScreenHeight-iBottomButtonHeight,eBI_NEXT_PAGE_NORMAL	,eBI_NEXT_PAGE_LIGHT,eBI_NEXT_PAGE_LIGHT);
	m_button.setClickMargin(eBM_INDEX,1,1);
	m_button.setClickMargin(eBM_EXIT,1,1);
	m_button.setClickMargin(eBM_PREVIOUS_PAGE,1,1);
	m_button.setClickMargin(eBM_NEXT_PAGE,1,1);

	ClosePopupWindow();

	m_itemBook.copy(lpItem);

	m_iBook					=	lpBookItem->m_wBookIndex;
	s_iPopupInterface		=	ePIW_BOOK;
	s_bIsFullScreenInterface=	TRUE;
	m_iWhere				=	eBW_INDEX;
	m_iCurrentPage			=	0;
	m_iRateX = 100;
	m_iRateY = 100;
	m_iAddX  = 0;
	if (g_iScreenWidth != 800)
	{		
		m_iRateX = (int)(g_iScreenWidth*100)/800;
		m_iRateY = (int)(g_iScreenHeight*100)/600;
		m_iAddX = 52;
	}

	return;
}

void
cBookGPW::close()
{
	s_bIsFullScreenInterface=	FALSE;
	s_iPopupInterface		=	ePIW_NONE;
}

void
cBookGPW::update()
{
	if	(s_iPopupInterface	!=	ePIW_BOOK)
		return;

	m_iFocusIndex	=	getFocusIndex();

	if	(m_iFocusIndex	!=	-1	&&	s_isClickedLeftButton)
	{
		int	iSelectPage		=	g_book.getPageInBook(m_iBook,m_iFocusIndex,0);

		m_iCurrentPage		=	iSelectPage/2*2;
		m_iCurrentCategory	=	m_iFocusIndex;
		m_iWhere			=	eBW_CONTENTS;
	}

	DWORD	dwMenu	=	m_button.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);

	switch(dwMenu)
	{
		case	eBM_INDEX			:
			m_iWhere	=	eBW_INDEX;
			break;
		case	eBM_EXIT			:
			close();
			break;
		case	eBM_PREVIOUS_PAGE	:
			if	(m_iCurrentPage	>	0)
				m_iCurrentPage-=2;
			else
				m_iWhere		=	eBW_INDEX;
			break;
		case	eBM_NEXT_PAGE		:
			if	(m_iWhere	==	eBW_INDEX)
			{
				m_iCurrentPage	=	0;
				m_iWhere		=	eBW_CONTENTS;
				break;
			}
			if	(m_iCurrentPage	<	g_book.getTotalPageCount(m_iBook)-2)
				m_iCurrentPage	+=	2;
			break;
	}
}

int
cBookGPW::getFocusIndex()
{
	if	(m_iWhere	!=	eBW_INDEX)
		return	-1;

	cBookInfoDefine	*lpBook	=	g_book.getBook(m_iBook);

	int	iX	=	480+g_iCorrectBI_X/2+m_iAddX/2;
	int	iY	=	110;
	int	iDY	=	22;

	iDY	=	iDY*c_iMaxBookCategoryCount/max(lpBook->m_wCategoryCount,1);
	iY	+=	iDY/2;

	for	(int iIndex=0;iIndex<lpBook->m_wCategoryCount;iIndex++)
	{
		cRECT	rect;

		rect.set(iX,iY,iX+260,iY+22);

		if	(rect.isIn(s_posMouse.x,s_posMouse.y))
			return	iIndex;

		iY	+=	iDY;
	}

	return	-1;
}

void
cBookGPW::drawPage()
{
	s_text13M.pushShadowStatus(FALSE);
	s_text13MA90.pushShadowStatus(FALSE);
	s_textBold18M.pushShadowStatus(FALSE);
	s_text13M.setLineGap(6);

	char	*lpstrBookName		=	g_book.getBookName(m_iBook);
	int		iPageCount			=	g_book.getTotalPageCount(m_iBook);

	if	(m_iCurrentPage		<	iPageCount)
	{
		int		iCategory			=	g_book.getCategoryByPage(m_iBook,m_iCurrentPage);
		int		iPage				=	g_book.getPageByPage(m_iBook,m_iCurrentPage);
		char	*lpstrCategoryName	=	g_book.getCategoryName(m_iBook,iCategory);

		if	(iPage	!=	0)
		{
			g_sprBook.Put(210+m_iAddX/2		,565+g_iCorrectBI_Y  ,eBI_LEFT_PAGE_PANNEL);
			s_text13M.cPut(210+m_iAddX/2	,565-7+g_iCorrectBI_Y,WHITE, "%.3d", m_iCurrentPage+1);
		}

		if	(m_itemBook.isAvailPage(m_iCurrentPage))
		{
			if	(iPage	==	0)
			{
				g_sprBook.Put(122,216,eBI_CHAPTER_DECORATION);
				s_textBold18M.put(80+m_iAddX,266,296,200,0,0,lpstrCategoryName);
				s_textBold18M.put(80+m_iAddX,266+1,296,200,0,0,lpstrCategoryName);
			}
			else
			{
				s_text13MA90.put(45,295-30,RGB(160,119,67),lpstrBookName);

				char	*lpstrText	=	g_book.getBookPageContents(m_iBook,iCategory,iPage);
				s_text13M.put(81+m_iAddX,62,296,480,0,0,lpstrText);
			}
		}
		else
		{
			g_sprBook.Put(80+m_iAddX,157,eBI_VALID_PAGE,100,100,dPUT_ALPHA_BLENDING,4);
		}
	}

	if	(m_iCurrentPage+1	<	iPageCount)
	{
		int		iCategory			=	g_book.getCategoryByPage(m_iBook,m_iCurrentPage+1);
		int		iPage				=	g_book.getPageByPage(m_iBook,m_iCurrentPage+1);
		char	*lpstrCategoryName	=	g_book.getCategoryName(m_iBook,iCategory);

		if	(iPage	!=	0)
		{
			g_sprBook.Put (595+g_iCorrectBI_X/2+m_iAddX/2	,565+g_iCorrectBI_Y, eBI_RIGHT_PAGE_PANNEL);
			s_text13M.cPut(595+g_iCorrectBI_X/2+m_iAddX/2	,565+g_iCorrectBI_Y-7, WHITE,"%.3d",m_iCurrentPage+1+1);
		}

		if	(m_itemBook.isAvailPage(m_iCurrentPage+1))
		{
			if	(iPage	==	0)
			{
				g_sprBook.Put(472+g_iCorrectBI_X/2+m_iAddX/2,216,eBI_CHAPTER_DECORATION);
				s_textBold18M.put(430+g_iCorrectBI_X/2+m_iAddX/2,266,0,lpstrCategoryName);
				s_textBold18M.put(430+g_iCorrectBI_X/2+m_iAddX/2,266+1,0,lpstrCategoryName);
			}
			else
			{
				char	strCategoryName[256];

				g_book.makeVerticalCategoryName(lpstrCategoryName,strCategoryName);

				int		iPixelWidth	=	s_text13MA90.getPixelSize(strCategoryName);
				s_text13MA90.put(750+g_iCorrectBI_X,324+iPixelWidth/2,RGB(160,119,67),strCategoryName);

				char	*lpstrText	=	g_book.getBookPageContents(m_iBook,iCategory,iPage);
				s_text13M.put(431+g_iCorrectBI_X/2+m_iAddX,62,296,480,0,0,lpstrText);
			}
		}
		else
		{
			g_sprBook.Put(428+g_iCorrectBI_X/2+m_iAddX,157,eBI_VALID_PAGE,100,100,dPUT_ALPHA_BLENDING,4);
		}
	}
	s_text13MA90.popShadowStatus();
	s_text13M.popShadowStatus();
	s_textBold18M.popShadowStatus();

	{
		m_button.show(eBM_INDEX);
		m_button.show(eBM_PREVIOUS_PAGE);

		int	iUpRightButtonWidth		=	g_sprBook.getSpriteWidth(eBI_EXIT_NORMAL);
		int	iBottomRightButtonWidth	=	g_sprBook.getSpriteWidth(eBI_NEXT_PAGE_NORMAL);
		int	iBottomButtonHeight		=	g_sprBook.getSpriteHeight(eBI_NEXT_PAGE_NORMAL)+20;

		if	(m_iCurrentPage	<	g_book.getTotalPageCount(m_iBook)-2)
		{
			m_button.show(eBM_NEXT_PAGE);
			g_sprBook.Put(g_iScreenWidth-iBottomRightButtonWidth,g_iScreenHeight-iBottomButtonHeight,eBI_NEXT_PAGE_SHADOW,100,100,dPUT_ALPHA_BLENDING,12);
		}
		else
		{
			m_button.hide(eBM_NEXT_PAGE);
		}

		g_sprBook.Put(g_iScreenWidth-iUpRightButtonWidth	,20									,eBI_EXIT_SHADOW	,100,100,dPUT_ALPHA_BLENDING,12);
		g_sprBook.Put(0										,20									,eBI_INDEX_SHADOW	,100,100,dPUT_ALPHA_BLENDING,12);
		g_sprBook.Put(0										,g_iScreenHeight-iBottomButtonHeight,eBI_PRE_PAGE_SHADOW,100,100,dPUT_ALPHA_BLENDING,12);
	}
}

void
cBookGPW::drawIndex()
{
	int				iX,iY;
	cBookInfoDefine	*lpBook	=	g_book.getBook(m_iBook);

	s_textBold18.pushShadowStatus(FALSE);
	s_textBold24M.pushShadowStatus(FALSE);
	s_textBold14M.pushShadowStatus(FALSE);

	{
		iX	=	65+m_iAddX;
		iY	=	220;

		g_sprBook.Put(36,58,eBI_COVER_FRAME, m_iRateX, m_iRateY);
		g_sprBook.Put(210+m_iAddX,410+g_iCorrectBI_Y,eBI_CHAPTER_DECORATION, m_iRateX, m_iRateY);

		s_textBold24M.cPut(iX,iY+g_iCorrectBI_Y/2,294,60,0,0,dFITTEXT_CENTER,lpBook->m_strBookName);
	}
	

	{
		g_sprBook.Put(426+g_iCorrectBI_X/2,58,eBI_INDEX_FRAME, m_iRateX, m_iRateY);

		int	iMenuTextSize	=	s_textBold18.getPixelSize("INDEX");
		int	iDY				=	22;

		iY	=	85;
		g_sprBook.Put(600-iMenuTextSize/2-20+g_iCorrectBI_X/2+m_iAddX/2,iY+8,eBI_BOOK_NAME_DECORATION);
		g_sprBook.Put(600+iMenuTextSize/2+20+g_iCorrectBI_X/2+m_iAddX/2,iY+8,eBI_BOOK_NAME_DECORATION);
		s_textBold18.cPut(600+g_iCorrectBI_X/2+m_iAddX/2,iY,RGB(159,116,27),"INDEX");

		iX	=	480 + g_iCorrectBI_X/2+m_iAddX/2;
		iY	=	110;

		int	iCategoryCount	=	lpBook->m_wCategoryCount;

		iDY	=	iDY*c_iMaxBookCategoryCount/max(iCategoryCount,1);

		iY	+=	iDY/2;

		for	(int iIndex=0;iIndex<iCategoryCount;iIndex++)
		{
			DWORD	dwColor	=	0;

			if	(m_iFocusIndex	==	iIndex)
				s_textBold14M.put(iX-1,iY-1,220,60,0,RED,g_book.getCategoryName(m_iBook,iIndex%lpBook->m_wCategoryCount));
			else
				s_textBold14M.put(iX,iY,220,60,0,0,g_book.getCategoryName(m_iBook,iIndex%lpBook->m_wCategoryCount));

			iY	+=	iDY;
		}
	}

	s_textBold18.popShadowStatus();
	s_textBold24M.popShadowStatus();
	s_textBold14M.popShadowStatus();

	{
		int	iUpRightButtonWidth		=	g_sprBook.getSpriteWidth(eBI_EXIT_NORMAL);
		int	iBottomRightButtonWidth	=	g_sprBook.getSpriteWidth(eBI_NEXT_PAGE_NORMAL);
		int	iBottomButtonHeight		=	g_sprBook.getSpriteHeight(eBI_NEXT_PAGE_NORMAL)+20;

		m_button.hide(eBM_INDEX);
		m_button.hide(eBM_PREVIOUS_PAGE);
		m_button.show(eBM_NEXT_PAGE);

		g_sprBook.Put(g_iScreenWidth-iUpRightButtonWidth	,20									,eBI_EXIT_SHADOW	,100,100,dPUT_ALPHA_BLENDING,12);
		g_sprBook.Put(g_iScreenWidth-iBottomRightButtonWidth,g_iScreenHeight-iBottomButtonHeight,eBI_NEXT_PAGE_SHADOW,100,100,dPUT_ALPHA_BLENDING,12);
	}
}

void
cBookGPW::draw()
{
	g_sprBook.Put(0,0,eBI_BOOK);

	if	(m_iWhere	==	eBW_INDEX)
	{
		drawIndex();
	}
	else
	{
		drawPage();
	}


	m_button.draw();
}

BOOL
cBookGPW::isOpened()
{
	if	(s_iPopupInterface		==	ePIW_BOOK)
		return	TRUE;

	return	FALSE;
}