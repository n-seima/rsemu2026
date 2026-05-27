#include "CPopupMenu.H"
#include "cDRAW.H"

//
//	팝업 초기화
BOOL
CPopupMenu::init(CPopupMenuInfo *_lpInfo)
{
	memcpy(this,_lpInfo,sizeof(CPopupMenuInfo));

	if (!m_lpttFrame)	return	FALSE;

	m_lpttFrame	=	_lpInfo->m_lpttFrame;
	m_pMenu		=	new cMENU_ITEM [m_iCountOfMaxMenu];

	for (int i=0;i<m_iCountOfMaxMenu;i++)
	{
		m_pMenu[i].m_pString	=	new char [m_iTextLength];
		m_pMenu[i].m_pString[0]	=	NULL;
		m_pMenu[i].m_iMenuId	=	-1;
	}

	return	TRUE;
}	//	CPopupMenu::init(int _iMaxMenuCount,int _iTextLength)

//
//	팝업 
BOOL
CPopupMenu::popup(int _iX,int _iY)
{
	reset();

	m_rectFrame.x1	=	_iX;
	m_rectFrame.y1	=	_iY;

	m_isPopuped		=	TRUE;
	m_iFocusMenu	=	-1;

	return	TRUE;
}	//	CPopupMenu::popup(int _iX,int _iY)

//
//	메뉴 추가
BOOL
CPopupMenu::addMenu(int _iMenu,char *_lpstrText)
{
	if (m_iCount	>=	m_iCountOfMaxMenu)	return	FALSE;

	m_pMenu[m_iCount].m_iMenuId	=	_iMenu;
	ustrncpy(m_pMenu[m_iCount].m_pString,_lpstrText,m_iTextLength);

	m_iTextFrameWidth		=	0;

	m_iCount++;

	return	TRUE;
}	//	CPopupMenu::addMenu(DWORD _dwMenu,char *_lpstrText)

//
//	업데이트
int
CPopupMenu::update(int _iX,int _iY,BOOL _bIsClickedLeftButton,BOOL _bIsLeftButtonPressed,BOOL _bIsClickedRightButton)
{
	if (m_iTextFrameWidth	==	0)
		return	-1;

	CPos	pos;

	pos.x			=	m_rectFrame.x1;
	pos.y			=	m_rectFrame.y1;

	m_iFocusMenu	=	-1;

	for (int i=0;i<m_iCount;i++)	
	{
		cRECT	rect;

		rect.set(m_rectFrame.x1,pos.y-m_wLineSpace/2,m_rectFrame.x2,pos.y+m_lpText->m_iFontHeight+m_wLineSpace/2);

		if (rect.isIn(_iX,_iY))
		{
			m_iFocusMenu	=	i;

			if (_bIsClickedLeftButton)
			{
				reset();

				return	m_pMenu[i].m_iMenuId;
			}

			break;
		}

		pos.y	+=	m_lpText->m_iFontHeight+m_wLineSpace;
	}

	if (_bIsClickedLeftButton || _bIsClickedRightButton)
		reset();

	if (_bIsLeftButtonPressed	&&	m_iFocusMenu	==	-1)
		reset();

	return	-1;
}


//
//	판넬을 그려준다.
void
CPopupMenu::drawPannel()
{
	if (m_wIsHalfblendingPannel)	m_lpttFrame->drawBox(&m_rectFrame,m_wPannelColor,16,TRUE,TRUE);
	else							m_lpttFrame->drawBox(&m_rectFrame,m_wPannelColor,32,TRUE,TRUE);

	CPos	pos;

	pos.x	=	m_rectFrame.x1;//+m_lpttFrame->m_wVertSpace;
	pos.y	=	m_rectFrame.y1;//+m_lpttFrame->m_wHorzSpace;

	for (int i=0;i<m_iCount;i++)	
	{
		if (m_iFocusMenu	==	i)
		{
			if (m_wIsHalfblendingBar)	
					cDRAW::FillHB(m_wBarColor,pos.x-m_lpttFrame->m_wVertSpace,pos.y-m_wLineSpace/2,m_rectFrame.x2+m_lpttFrame->m_wVertSpace-1,pos.y+m_lpText->m_iFontHeight+m_wLineSpace/2-1);
			else	cDRAW::Fill(m_wBarColor,pos.x-m_lpttFrame->m_wVertSpace,pos.y-m_wLineSpace/2,m_rectFrame.x2+m_lpttFrame->m_wVertSpace-1,pos.y+m_lpText->m_iFontHeight+m_wLineSpace/2-1);

			m_lpText->pushShadowStatus(m_wIsActiveTextPutShadow);
			m_lpText->put(pos.x,pos.y,m_dwActiveTextColor,m_pMenu[i].m_pString);
			m_lpText->popShadowStatus();
		}
		else
		{
			m_lpText->pushShadowStatus(m_wIsTextPutShadow);
			m_lpText->put(pos.x,pos.y,m_dwTextColor,m_pMenu[i].m_pString);
			m_lpText->popShadowStatus();
		}

		pos.y	+=	m_lpText->m_iFontHeight+m_wLineSpace;
	}

}	//	CPopupMenu::drawPannel()

//
//	그려줘!!
void
CPopupMenu::draw()
{
	if (m_iCount	<=	0)	return;

	if (m_iTextFrameWidth==	0)
	{
		for (int i=0;i<m_iCount;i++)	m_iTextFrameWidth	=	max(m_lpText->getPixelSize(m_pMenu[i].m_pString),m_iTextFrameWidth);

		m_iTextFrameHeight	=	(m_lpText->m_iFontHeight+m_wLineSpace)*m_iCount-m_wLineSpace;

		m_rectFrame.x2		=	m_rectFrame.x1+m_iTextFrameWidth;
		m_rectFrame.y2		=	m_rectFrame.y1+m_iTextFrameHeight;

		if (m_rectFrame.x2	>=	cDRAW::WIDTH-m_lpttFrame->getBordersWidth()-2	)
		{
			m_rectFrame.x2	=	cDRAW::WIDTH-m_lpttFrame->getBordersWidth()-2;
			m_rectFrame.x1	=	m_rectFrame.x2-m_iTextFrameWidth;
		}

		if (m_rectFrame.y2	>=	cDRAW::HEIGHT-m_lpttFrame->getBordersHeight()-2	)
		{
			m_rectFrame.y2	=	cDRAW::HEIGHT-m_lpttFrame->getBordersHeight()-2;
			m_rectFrame.y1	=	m_rectFrame.y2-m_iTextFrameHeight;
		}
	}

	drawPannel();
}	//	CPopupMenu::draw()