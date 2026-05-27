#include	"banner.h"
#include	"SFC.h"
#include	"cMap.h"

cBannerManager	g_bannerManager;

void
cBanner::copy(cBanner *_lpBanner)
{
	memcpy(this,_lpBanner,sizeof(cBannerDefine));
	m_dwPopupTextTime	=	0;
}

void
cBanner::reset()
{
	m_wSerial		=	0xffff;
	m_strText[0]	=	NULL;
	m_dwPopupTextTime=	0;
}

void
cBanner::showText()
{
	m_dwPopupTextTime	=	timeGetTime();
}

BOOL
cBanner::isFocusVertical(int _iX,int _iY)
{
	CTextDC	*lpText			=	&CGamePlay::s_text;
	int		iIndex			=	m_bf15Shape*5;
	int		iDefaultHeight	=	g_sprBanner.getSpriteHeight(iIndex+1);
	int		iPoint			=	0;
	int		iLineCount		=	0;
	int		iSpaceCount		=	0;

	while(m_strTitle[iPoint])
	{
		if	(Is2ByteChar(m_strTitle[iPoint]))
			iPoint++;

		if	(m_strTitle[iPoint]	==	' ')
			iSpaceCount++;
		else
			iLineCount++;

		iPoint++;
	}

	int		iTextHeight		=	lpText->getFontHeight()*iLineCount+iSpaceCount*6;
	int		iHeight			=	max(iDefaultHeight,iTextHeight);
	cRECT	rect;

	rect.set(m_wX-10,m_wY-iHeight,m_wX+10,m_wY);

	if	(rect.isIn(_iX,_iY))
		return	TRUE;

	return	FALSE;
}

BOOL
cBanner::isFocusHorizon(int _iX,int _iY)
{
	CTextDC	*lpText			=	&CGamePlay::s_text;
	int		iIndex			=	m_bf15Shape*5+5;
	int		iDefaultWidth	=	g_sprBanner.getSpriteWidth(iIndex+1);
	int		iTextWidth		=	lpText->getPixelSize(m_strTitle);
	int		iWidth			=	max(iDefaultWidth,iTextWidth);

	cRECT	rect;

	rect.set(m_wX-iWidth/2,m_wY-20,m_wX+iWidth/2,m_wY);

	if	(rect.isIn(_iX,_iY))
		return	TRUE;

	return	FALSE;
}

BOOL
cBanner::isFocus(int _iX,int _iY)
{
	if	(m_bf1IsVertical)
		return	isFocusVertical(_iX,_iY);

	return	isFocusHorizon(_iX,_iY);
}

void
cBanner::putVertical(int _iX,int _iY,BOOL _bIsFocused)
{
	CTextDC	*lpText			=	&CGamePlay::s_text;
	int		iIndex			=	m_bf15Shape*5;
	int		iDefaultHeight	=	g_sprBanner.getSpriteHeight(iIndex+1);
	int		iPoint			=	0;
	int		iLineCount		=	0;
	int		iSpaceCount		=	0;

	while(m_strTitle[iPoint])
	{
		if	(Is2ByteChar(m_strTitle[iPoint]))
			iPoint++;

		if	(m_strTitle[iPoint]	==	' ')
			iSpaceCount++;
		else
			iLineCount++;

		iPoint++;
	}

	int		iTextHeight		=	lpText->getFontHeight()*iLineCount+iSpaceCount*6;
	int		iHeight			=	max(iDefaultHeight,iTextHeight);
	CPos	pos;

	pos.x	=	_iX;
	pos.y	=	_iY-iHeight;

	if	(iHeight	>	iDefaultHeight)
		g_sprBanner.clipPut(pos.x,pos.y+iDefaultHeight,iIndex+2,100,iHeight-iDefaultHeight);

	g_sprBanner.Put(pos.x,_iY,iIndex+4,100,100,dPUT_HALF_BLENDING);
	g_sprBanner.Put(pos.x,pos.y,iIndex+1);
	g_sprBanner.Put(pos.x,pos.y,iIndex);
	g_sprBanner.Put(pos.x,_iY,iIndex+3);

	iPoint			=	0;

	char	strChar[3];

	lpText->pushShadowStatus(FALSE);

	while(m_strTitle[iPoint])
	{
		strChar[0]	=	m_strTitle[iPoint];
		strChar[1]	=	NULL;

		if	(Is2ByteChar(m_strTitle[iPoint]))
		{
			iPoint++;
			strChar[1]	=	m_strTitle[iPoint];
			strChar[2]	=	NULL;
		}

		if	(m_strTitle[iPoint]	==	' ')
			pos.y		+=	6;
		else
		{
			if	(_bIsFocused)
				lpText->cPut(pos.x,pos.y,LTBLUE,strChar);
			else
				lpText->cPut(pos.x,pos.y,BLACK,strChar);
			pos.y		+=	lpText->getFontHeight();
		}

		iPoint++;
	}

	if	(m_dwPopupTextTime	+	3000	>	timeGetTime()	&&	m_strText[0])
	{
		pos.x	=	_iX;
		pos.y	=	_iY-iHeight;

		cRECT	rect;

		int		iWidth		=	100;
		int		iHeight		=	lpText->getTextHeight(iWidth,m_strText);

		iWidth	=	min(lpText->getPixelSize(m_strText),iWidth);

		rect.set(pos.x+20,pos.y,pos.x+20+iWidth,pos.y+iHeight);

		CGamePlay::s_ttDialogFrame.drawBoxAddBorderSize(&rect,_WHITE,32);

		lpText->put(rect.x1,rect.y1,iWidth,iHeight,0,0,m_strText);
	}

	lpText->popShadowStatus();
}

void
cBanner::putHorizon(int _iX,int _iY,BOOL _bIsFocused)
{
	CTextDC	*lpText			=	&CGamePlay::s_text;
	int		iIndex			=	m_bf15Shape*5+5;
	int		iDefaultWidth	=	g_sprBanner.getSpriteWidth(iIndex+1);
	int		iTextWidth		=	lpText->getPixelSize(m_strTitle);
	int		iWidth			=	max(iDefaultWidth,iTextWidth);
	CPos	pos;

	lpText->pushShadowStatus(FALSE);

	if	(m_dwPopupTextTime	+	3000	>	timeGetTime()	&&	m_strText[0])
	{
		pos.x	=	_iX;
		pos.y	=	_iY-30;

		cRECT	rect;

		iWidth				+=	g_sprBanner.getSpriteWidth(iIndex)+g_sprBanner.getSpriteWidth(iIndex+3);

		int		iHeight		=	lpText->getTextHeight(iWidth,m_strText);
		iWidth				=	min(lpText->getPixelSize(m_strText),iWidth);

		rect.set(pos.x-iWidth/2,pos.y-iHeight,pos.x+iWidth/2,pos.y);

		CGamePlay::s_ttDialogFrame.drawBoxAddBorderSize(&rect,_WHITE,32);

		lpText->put(rect.x1,rect.y1,iWidth,iHeight,0,0,m_strText);
	}

	iWidth					=	max(iDefaultWidth,iTextWidth);
	pos.x					=	_iX-iWidth/2;
	pos.y					=	_iY;

	if	(iWidth	>	iDefaultWidth)
		g_sprBanner.clipPut(pos.x+iDefaultWidth,pos.y,iIndex+2,iWidth-iDefaultWidth,100);

	g_sprBanner.Put(pos.x,pos.y,iIndex+4,100,100,dPUT_HALF_BLENDING);
	g_sprBanner.Put(pos.x,pos.y,iIndex+1);
	g_sprBanner.Put(pos.x,pos.y,iIndex);
	g_sprBanner.Put(pos.x+iWidth,pos.y,iIndex+3);

	if	(_bIsFocused)
		lpText->cPut(_iX,_iY-18,LTBLUE,m_strTitle);
	else
		lpText->cPut(_iX,_iY-18,BLACK,m_strTitle);

	lpText->popShadowStatus();
}

void
cBanner::put(int _iX,int _iY,BOOL _bIsFocused)
{
	if	(m_bf1IsVertical)
		putVertical(_iX,_iY,_bIsFocused);
	else
		putHorizon(_iX,_iY,_bIsFocused);
}

void
cBannerManager::reset()
{
	for	(int	i=0;i<dMAX_BANNER_COUNT_IN_FIELD;i++)
		m_aBannerList[i].reset();
}

cBanner*
cBannerManager::get(int _iIndex)
{
	for	(int	i=0;i<dMAX_BANNER_COUNT_IN_FIELD;i++)
	{
		if	(m_aBannerList[i].m_wSerial	==	_iIndex)
			return	&m_aBannerList[i];
	}

	return	NULL;
}

BOOL
cBannerManager::add(cBanner *_lpBanner)
{
	cBanner	*lpExistBanner	=	get(_lpBanner->m_wSerial);

	if	(lpExistBanner)
	{
		lpExistBanner->copy(_lpBanner);

		return	TRUE;
	}

	for	(int	i=0;i<dMAX_BANNER_COUNT_IN_FIELD;i++)
	{
		if	(m_aBannerList[i].m_wSerial	==	0xffff)
		{
			m_aBannerList[i].copy(_lpBanner);

			return	TRUE;
		}
	}

	return	FALSE;
}

BOOL
cBannerManager::remove(int _iIndex)
{
	cBanner	*lpBanner	=	get(_iIndex);

	if	(!lpBanner)
		return	FALSE;

	lpBanner->reset();

	return	TRUE;
}

BOOL
cBannerManager::generate(int _iSerial,int _iX,int _iY,int _iShape,BOOL _bIsVertical,char *_lpstrTitle)
{
	cBanner	banner;

	banner.m_wX				=	_iX;
	banner.m_wY				=	_iY;
	banner.m_bf15Shape		=	_iShape;
	banner.m_bf1IsVertical	=	_bIsVertical;
	banner.m_wSerial		=	_iSerial;
	
	strcpy(banner.m_strTitle,_lpstrTitle);
	strcpy(banner.m_strText,"");

	return	add(&banner);
}

void
cBannerManager::setText(int _iSerial,char *_lpstrText)
{
	m_wIsWaitBannerText	=	FALSE;

	cBanner	*lpBanner	=	get(_iSerial);

	if	(!lpBanner)
		return;

	strcpy(lpBanner->m_strText,_lpstrText);
}

int
cBannerManager::getViewBanners(cINDEX *_lpIndexes,cRECT *_lpRectViewport)
{
	int	iCount	=	0,iVisibleBannerCount	=	0;

	for (int i=0;i<dMAX_BANNER_COUNT_IN_FIELD;i++)
	{
		cBanner	*lpBanner		=	&m_aBannerList[i];

		if (lpBanner->m_wSerial	==	0xffff)
			continue;

		if (_lpRectViewport->isIn(lpBanner->m_wX,lpBanner->m_wY))
		{
			_lpIndexes[iVisibleBannerCount].Index	=	dSORT_OBJECT_BORDER_BANNER+i;
			_lpIndexes[iVisibleBannerCount].Value	=	lpBanner->m_wY;
			iVisibleBannerCount++;
		}
	}

	return	iVisibleBannerCount;
}

void
cBannerManager::drawBanner(int _iIndex,int _iDX,int _iDY)
{
	cBanner	*lpBanner	=	&m_aBannerList[_iIndex];

	if (lpBanner->m_wSerial	==	0xffff)
		return;

	int		iX		=	lpBanner->m_wX-_iDX;
	int		iY		=	lpBanner->m_wY-_iDY;
	BOOL	bIsFocused=	FALSE;

	if	(lpBanner->m_wSerial	==	m_iFocusBanner)
		bIsFocused	=	TRUE;

	lpBanner->put(iX,iY,bIsFocused);
}

void
cBannerManager::strcpyVerticalBannerTitle(char *_lpstrText,char *_lpstrMessage)
{
	int		iPoint			=	0;
	int		iLineCount		=	0;
	int		iSpaceCount		=	0;

	while(_lpstrMessage[iPoint])
	{
		_lpstrText[iPoint]	=	_lpstrMessage[iPoint];

		if	(Is2ByteChar(_lpstrMessage[iPoint]))
		{
			if	(iLineCount*2+iSpaceCount+2	>	(dBANNER_TITLE_LENGTH-2)*2)
			{
				_lpstrText[iPoint]	=	NULL;
				return;
			}
			if	(iPoint		+1				>	dBANNER_TITLE_LENGTH-2)
			{
				_lpstrText[iPoint]	=	NULL;
				return;
			}


			iPoint++;
			_lpstrText[iPoint]	=	_lpstrMessage[iPoint];
		}

		if	(_lpstrMessage[iPoint]	==	' ')
			iSpaceCount++;
		else
			iLineCount++;

		iPoint++;

		if	(iLineCount*2+iSpaceCount	>=	(dBANNER_TITLE_LENGTH-2)*2)
		{
			_lpstrText[iPoint]	=	NULL;
			return;
		}

		if	(iPoint						>=	dBANNER_TITLE_LENGTH-2)
		{
			_lpstrText[iPoint]	=	NULL;
			return;
		}
	}
}

void
cBannerManager::update()
{
	m_iFocusBanner	=	0xffff;

	if	(IsOpenedEventWindow())
		return;

	for	(int i=0;i<dMAX_BANNER_COUNT_IN_FIELD;i++)
	{
		cBanner	*lpBanner	=	&m_aBannerList[i];

		if	(lpBanner->m_wSerial	==	0xffff)
			continue;

		if	(lpBanner->isFocus(s_posCursor.x,s_posCursor.y))
		{
			m_iFocusBanner	=	lpBanner->m_wSerial;

			break;
		}
	}

	if	(s_isClickedLeftButton	&&	m_iFocusBanner	!=	0xffff)
	{
		cBanner	*lpBanner			=	get(m_iFocusBanner);

		lpBanner->showText();

		if	(isAbleToAskBannerText())
		{
			setAskBannerTextTime();

			s_agent.sendAskBannerText(m_iFocusBanner);
		}
	}
}

void
cBannerManager::setAskBannerTextTime()
{
	m_wIsWaitBannerText		=	TRUE;
	m_dwAskBannerTextTime	=	timeGetTime();
}

BOOL
cBannerManager::isAbleToAskBannerText()
{
	if	(m_wIsWaitBannerText==	FALSE)
		return	TRUE;

	if	(timeGetTime()	>=	m_dwAskBannerTextTime+1000)
		m_wIsWaitBannerText	=	FALSE;

	return	FALSE;
}