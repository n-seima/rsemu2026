#include "CTooltip.H"
#include "cDRAW.H"

CTooltip::CTooltip()
{
	m_pString		=	NULL;

	reset();
}

CTooltip::~CTooltip()
{
	pKILL(m_pString);
}

//
//	ｸｮｼﾂ
void
CTooltip::reset()
{
	m_lpSprite		=	NULL;
	m_iWidth		=	0;
	m_isEnable		=	FALSE;

	if	(m_pString)
		m_pString[0]	=	NULL;
}

//
//	ﾃﾊｱ篳ｭ
BOOL
CTooltip::init(CTooltipInfo *_lpInfo)
{
	memcpy(this,_lpInfo,sizeof(CTooltipInfo));

	if (m_iLeftTop			==	-1)	return	ERRMSG("error!! in CTooltip::init","m_iLeftTop image not setting");
	if (m_iRightTop			==	-1)	return	ERRMSG("error!! in CTooltip::init","m_iRightTop image not setting");
	if (m_iLeftBottom		==	-1)	return	ERRMSG("error!! in CTooltip::init","m_iLeftBottom image not setting");
	if (m_iRightBottom		==	-1)	return	ERRMSG("error!! in CTooltip::init","m_iRightBottom image not setting");
	if (m_iTopBorder		==	-1)	return	ERRMSG("error!! in CTooltip::init","m_iTopBorder image not setting");
	if (m_iBottomBorder		==	-1)	return	ERRMSG("error!! in CTooltip::init","m_iBottomBorder image not setting");
	if (m_iLeftBorder		==	-1)	return	ERRMSG("error!! in CTooltip::init","m_iLeftBorder image not setting");
	if (m_iRightBorder		==	-1)	return	ERRMSG("error!! in CTooltip::init","m_iRightBorder image not setting");

	pKILL(m_pString);

	if (m_iTextLength > 0)	m_pString	=	new char [m_iTextLength];

	m_lpText				=	_lpInfo->m_lpText;
	m_lpSprite				=	_lpInfo->m_lpSprite;

	m_iLeftEdgeWidth		=	m_lpSprite->getSpriteWidth(m_iLeftTop);
	m_iTopEdgeHeight		=	m_lpSprite->getSpriteHeight(m_iLeftTop);

	m_iRightEdgeWidth		=	m_lpSprite->getSpriteWidth(m_iRightTop);
	m_iBottomEdgeHeight		=	m_lpSprite->getSpriteHeight(m_iRightBottom);

	m_iTopBorderHeight		=	m_lpSprite->getSpriteHeight(m_iTopBorder);
	m_iBottomBorderHeight	=	m_lpSprite->getSpriteHeight(m_iBottomBorder);

	m_iLeftBorderWidth		=	m_lpSprite->getSpriteWidth(m_iLeftBorder);
	m_iRightBorderWidth		=	m_lpSprite->getSpriteWidth(m_iRightBorder);

	m_iWidth				=	m_iLimitWidth;
	m_isEnable				=	TRUE;

	return	TRUE;
}

//
//	ﾀﾌｹﾌﾁ・ｼｳﾁ､
void
CTooltip::setImage(WORD *_lpImage)
{
	m_lpImage	=	_lpImage;
}

//
//	ﾆﾁ ｶ鄙・・
void
CTooltip::popupTip(cRECT *_lpRect,int _iX,int _iY,CTextDC *_lpText,DWORD _dwTextColor,char *_strString,int _iFitMethod,DWORD _dwTime)
{
	m_lpImage	=	NULL;

	if	(!m_isEnable || !m_pString)
	{
		m_isPopuped	=	FALSE;

		return;
	}

	m_dwUpkeepTime	=	0xffffffff;

	if	(m_isPopuped && _lpRect)
		if	(m_rectActiveArea.isSame(_lpRect))
			if	(strcmp(m_pString,_strString) == 0)
			{
				if (_dwTime	!=	0xffffffff	)
					m_dwPopupTime	=	timeGetTime()+_dwTime;
				else
					m_dwUpkeepTime	=	timeGetTime()+200;

				return;
			}

	hide();

	if (_dwTime	!=	0xffffffff	)
		m_dwPopupTime	=	timeGetTime()+_dwTime;
	else
		m_dwUpkeepTime	=	timeGetTime()+200;

	if (_lpRect					)
	{
		//_lpRect->add(100 , 100);
		m_rectActiveArea.set(_lpRect);
	}

	if (_iX	==	-1	&&	_iY	==	-1)
	{
		if (_lpRect)
		{
			m_rectFrame.x1	=	_lpRect->x1;
			m_rectFrame.y1	=	_lpRect->y1;
		}
		else
			ERRMSG("error!!","error in CTooltip::popupTip - position not setting");
	}
	else
	{
		m_rectFrame.x1	=	_iX;
		m_rectFrame.y1	=	_iY;
	}

	m_isPopuped		=	TRUE;
	m_dwTextColor	=	_dwTextColor;
	m_wFitMethod	=	_iFitMethod;

	strcpy(m_pString,_strString);
	
	if	(_lpText)
		m_lpText	=	_lpText;
}

//
//	ﾆﾁ ｼ﨣箜・
void
CTooltip::hide()
{
	m_pString[0]	=	NULL;	//	ｳｻｿ・	
	m_iWidth		=	0;
	m_dwPopupTime	=	0xffffffff;
	m_isPopuped		=	FALSE;
}

//
//	ｾ･ﾀﾌﾆｮ
void
CTooltip::update(int _iX,int _iY)
{
	if	(!m_isPopuped)
		return;

	if	(m_dwPopupTime	!=	0xffffffff)
	{
		if	(m_dwPopupTime	<	timeGetTime())
			hide();

		return;
	}

	if	(!m_rectActiveArea.isIn(_iX,_iY))
		hide();

	if	(m_dwUpkeepTime	!=	0xffffffff)
		if	(m_dwUpkeepTime	<	timeGetTime())
			hide();
}

//
//	ﾃ箙ﾂ
void
CTooltip::put()
{
	if (!m_isPopuped)
		return;	//	ｳｻｿ・
	m_lpText->setValidLineHeight(6);
	m_lpText->disableFitRightEdge();

	int	iImageWidth	=	0,iImageHeight	=	0;

	if (m_lpImage)
	{
		iImageWidth	=	m_lpImage[0]+8+m_iLeftEdgeWidth;
		iImageHeight=	m_lpImage[1]+8+m_iTopEdgeHeight;
	}

	if (m_iWidth	==	0)
	{
		m_iWidth			=	m_lpText->getMaxLineSize(m_pString) + (m_iLeftEdgeWidth+m_iRightEdgeWidth+m_wHorzSpace*2);
		m_iWidth			=	max(m_iWidth,iImageWidth);
		m_iWidth			=	min(m_iWidth,m_iLimitWidth);
		m_iTextFrameWidth	=	m_iWidth-(m_iLeftEdgeWidth+m_iRightEdgeWidth+m_wHorzSpace*2);
		m_iHeight			=	m_lpText->getTextHeight(m_iTextFrameWidth,m_pString)+(m_iTopEdgeHeight+m_iBottomEdgeHeight+m_wVertSpace*2)-m_lpText->m_iLineGap;
		m_iHeight			+=	iImageHeight;
		m_iTextFrameHeight	=	m_iHeight-(m_iTopEdgeHeight+m_iBottomEdgeHeight+m_wVertSpace*2);

		if (isFitBottom())
			m_rectFrame.y1	-=	m_iHeight;
		if (isFitLeft())	
			m_rectFrame.x1	-=	m_iWidth;
		if (isFitRight())	
			m_rectFrame.x1	+=	m_iWidth;

		if (m_rectFrame.x1+m_iWidth	>=cDRAW::WIDTH-5)
			m_rectFrame.x1	=	cDRAW::WIDTH-5-m_iWidth;
		if (m_rectFrame.y1+m_iHeight>=cDRAW::HEIGHT-5)
		{
			if (isFitAuto())
				m_rectFrame.y1	-=	m_iHeight;
			else
				m_rectFrame.y1	=	cDRAW::HEIGHT-5-m_iHeight;
		}
		if (m_rectFrame.x1	<	0)
			m_rectFrame.x1	=	0;
		if (m_rectFrame.y1	<	0)
			m_rectFrame.y1	=	0;

		m_rectFrame.x2	=	m_rectFrame.x1+m_iWidth;
		m_rectFrame.y2	=	m_rectFrame.y1+m_iHeight;
	}
	else
		m_iWidth	=	m_iWidth;

	if (m_wIsHalfblendingPannel)
		drawBox(&m_rectFrame,m_wPannelColor,16,FALSE);
	else
		drawBox(&m_rectFrame,m_wPannelColor,32,FALSE);

	if	(m_lpImage)
		cDRAW::PutImage(m_rectFrame.x1+m_iLeftEdgeWidth+4,m_rectFrame.y1+m_iTopEdgeHeight+4,m_lpImage);

	CPos	pos;

	pos.x	=	m_rectFrame.x1+m_iLeftEdgeWidth+m_wHorzSpace;
	pos.y	=	m_rectFrame.y1+m_iTopEdgeHeight+m_wVertSpace;

	m_lpText->put(pos.x,pos.y+iImageHeight,m_iTextFrameWidth,m_iTextFrameHeight+m_lpText->m_iLineGap,0,m_dwTextColor,m_pString);

	m_lpText->enableFitRightEdge();
	m_lpText->setValidLineHeight();
}

//
//	ﾀﾌｵ﨣ﾝ... ｹﾚｽｺｸｸ ﾇﾊｿ萇ﾑ ｰ豼・ｹﾚｽｺｸｸ ｱﾗｷﾁﾁﾘｴﾙ.
void
CTooltip::drawBox(cRECT *_lpRect,WORD _wBoxColor,int _iAlphaFactor,BOOL _isAddBorderSize,BOOL _isAddSpareSize)
{
	cRECT	rect;

	rect.set(_lpRect);

	if (_isAddBorderSize)
	{
		rect.x1	-=	m_iLeftEdgeWidth;
		rect.x2	+=	m_iRightEdgeWidth;
		rect.y1	-=	m_iTopEdgeHeight;
		rect.y2	+=	m_iBottomEdgeHeight;
	}

	if (_isAddSpareSize)
	{
		rect.x1	-=	m_wHorzSpace;
		rect.x2	+=	m_wHorzSpace;
		rect.y1	-=	m_wVertSpace;
		rect.y2	+=	m_wVertSpace;
	}

	int	iBoxWidth	=	rect.getWidth()-(m_iLeftEdgeWidth+m_iRightEdgeWidth)+1;
	int	iBoxHeight	=	rect.getHeight()-(m_iTopEdgeHeight+m_iBottomEdgeHeight)+1;

	cRECT	rectFillFrame;

	rectFillFrame.set(rect.x1+m_iLeftBorderWidth,rect.y1+m_iTopBorderHeight,rect.x2-m_iRightBorderWidth,rect.y2-m_iBottomBorderHeight);

// 	if(rectFillFrame.x1 < 0)
// 		rectFillFrame.x1 = 0;
// 
// 	if(rectFillFrame.x2 < 0)
// 		rectFillFrame.x2 = 0;
// 
// 	if(rectFillFrame.y1 < 0)
// 		rectFillFrame.y1 = 0;
// 
// 	if(rectFillFrame.y2 < 0)
// 		rectFillFrame.y2 = 0;


	if (_iAlphaFactor	==	32)
	{
		cDRAW::Fill(_wBoxColor,rectFillFrame.x1,rectFillFrame.y1,rectFillFrame.x2,rectFillFrame.y2);
	}
	else
	if (_iAlphaFactor	==	16)
	{
		cDRAW::FillHB(_wBoxColor,rectFillFrame.x1,rectFillFrame.y1,rectFillFrame.x2,rectFillFrame.y2);
	}
	else
	{
		cDRAW::FillAB(_wBoxColor,rectFillFrame.x1,rectFillFrame.y1,rectFillFrame.x2,rectFillFrame.y2,_iAlphaFactor);
	}

	m_lpSprite->Put(rect.x1							,rect.y1						,m_iLeftTop);
	m_lpSprite->Put(rect.x2-m_iRightEdgeWidth+1		,rect.y1						,m_iRightTop);
	m_lpSprite->Put(rect.x1							,rect.y2-m_iBottomEdgeHeight+1	,m_iLeftBottom);
	m_lpSprite->Put(rect.x2-m_iRightEdgeWidth+1		,rect.y2-m_iBottomEdgeHeight+1	,m_iRightBottom);

	m_lpSprite->clipPut(rect.x1+m_iLeftEdgeWidth	,rect.y1						,m_iTopBorder	,iBoxWidth,cDRAW::HEIGHT);
	m_lpSprite->clipPut(rect.x1+m_iLeftEdgeWidth	,rect.y2-m_iBottomBorderHeight+1,m_iBottomBorder,iBoxWidth,cDRAW::HEIGHT);
	m_lpSprite->clipPut(rect.x1						,rect.y1+m_iTopEdgeHeight		,m_iLeftBorder	,cDRAW::WIDTH,iBoxHeight);
	m_lpSprite->clipPut(rect.x2-m_iRightBorderWidth+1,rect.y1+m_iTopEdgeHeight		,m_iRightBorder	,cDRAW::WIDTH,iBoxHeight);
}

//
//	ﾀﾌｵ﨣ﾝ... ｹﾚｽｺｸｸ ﾇﾊｿ萇ﾑ ｰ豼・ｹﾚｽｺｸｸ ｱﾗｷﾁﾁﾘｴﾙ.
void
CTooltip::drawBoxAddBorderSize(cRECT *_lpRect,WORD _wBoxColor,int _iAlphaFactor)
{
	drawBox(_lpRect,_wBoxColor,_iAlphaFactor,TRUE);
}

//
//	ﾀﾌｵ﨣ﾝ... ｹﾚｽｺｸｸ ﾇﾊｿ萇ﾑ ｰ豼・ｹﾚｽｺｸｸ ｱﾗｷﾁﾁﾘｴﾙ.
void
CTooltip::drawFrame(cRECT *_lpRect,BOOL _isAddBorderSize)
{
	cRECT	rect;

	rect.set(_lpRect);

	if (_isAddBorderSize)
	{
		rect.x1	-=	m_iLeftEdgeWidth;
		rect.x2	+=	m_iRightEdgeWidth;
		rect.y1	-=	m_iTopEdgeHeight;
		rect.y2	+=	m_iBottomEdgeHeight;
	}

	int	iBoxWidth	=	rect.getWidth()-(m_iLeftEdgeWidth+m_iRightEdgeWidth)+1;
	int	iBoxHeight	=	rect.getHeight()-(m_iTopEdgeHeight+m_iBottomEdgeHeight)+1;

	m_lpSprite->Put(rect.x1							,rect.y1						,m_iLeftTop);
	m_lpSprite->Put(rect.x2-m_iRightEdgeWidth+1		,rect.y1						,m_iRightTop);
	m_lpSprite->Put(rect.x1							,rect.y2-m_iBottomEdgeHeight+1	,m_iLeftBottom);
	m_lpSprite->Put(rect.x2-m_iRightEdgeWidth+1		,rect.y2-m_iBottomEdgeHeight+1	,m_iRightBottom);

	m_lpSprite->clipPut(rect.x1+m_iLeftEdgeWidth	,rect.y1						,m_iTopBorder	,iBoxWidth,cDRAW::HEIGHT);
	m_lpSprite->clipPut(rect.x1+m_iLeftEdgeWidth	,rect.y2-m_iBottomBorderHeight+1,m_iBottomBorder,iBoxWidth,cDRAW::HEIGHT);
	m_lpSprite->clipPut(rect.x1						,rect.y1+m_iTopEdgeHeight		,m_iLeftBorder	,cDRAW::WIDTH,iBoxHeight);
	m_lpSprite->clipPut(rect.x2-m_iRightBorderWidth+1,rect.y1+m_iTopEdgeHeight		,m_iRightBorder	,cDRAW::WIDTH,iBoxHeight);
}

//
//	ﾀﾌｵ﨣ﾝ... ｹﾚｽｺｸｸ ﾇﾊｿ萇ﾑ ｰ豼・ｹﾚｽｺｸｸ ｱﾗｷﾁﾁﾘｴﾙ.
void
CTooltip::drawFrameAddBorderSize(cRECT *_lpRect)
{
	drawFrame(_lpRect,TRUE);
}

//
//	ﾅﾘｽｺﾆｮ ｹﾚｽｺｸｦ ﾂ・ﾂｴﾙ.
void
CTooltip::drawTextBox(cRECT *_lpRect,DWORD _dwTextColor,WORD _wBoxColor,char *_lpstrText,BOOL _bIsPutShadow,int _iAlpha)
{
	cRECT	rectTextFrame;

	cDRAW::FillAB(_wBoxColor,_lpRect->x1+m_iLeftBorderWidth,_lpRect->y1+m_iTopBorderHeight,_lpRect->x2-m_iRightBorderWidth,_lpRect->y2-m_iBottomBorderHeight,_iAlpha);

	drawFrame(_lpRect,FALSE);

	rectTextFrame.set(_lpRect);

	rectTextFrame.x1	+=	(m_iLeftEdgeWidth+m_wHorzSpace);
	rectTextFrame.x2	-=	(m_iRightEdgeWidth+m_wHorzSpace);
	rectTextFrame.y1	+=	(m_iTopEdgeHeight+m_wVertSpace);
	rectTextFrame.y2	-=	(m_iBottomEdgeHeight+m_wVertSpace);

	m_lpText->pushShadowStatus(_bIsPutShadow);

	m_lpText->disableFitRightEdge();
	m_lpText->put(rectTextFrame.x1,rectTextFrame.y1,rectTextFrame.getWidth(),rectTextFrame.getHeight(),0,_dwTextColor,_lpstrText);
	m_lpText->enableFitRightEdge();

	m_lpText->popShadowStatus();
}

//
//	ｰ｡ｴﾉ
void
CTooltip::setEnable(BOOL _isEnable)
{
	m_isEnable	=	_isEnable;
}

//
//	ｰ｡ｴﾉ
void
CTooltip::enable()
{
	m_isEnable	=	TRUE;
}

//
//	ｺﾒｰ｡
void
CTooltip::disable()
{
	m_isEnable	=	FALSE;
}