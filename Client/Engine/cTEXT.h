#ifndef _classTEXT_H
#define _classTEXT_H

#include "SFC.H"

#define	WHITE		(RGB(255,255,255))
#define	BLACK		0

#define	LTGRAY		(RGB(200,200,200))
#define	GRAY		(RGB(128,128,128))
#define	DKGRAY		(RGB(64,64,64))

#define	LTRED		(RGB(255,0,0))
#define	RED			(RGB(188,0,0))
#define	DKRED		(RGB(128,0,0))

#define	LTORANGE	(RGB(255,180,0))
#define	ORANGE		(RGB(200,140,0))

#define	LTPINK		(RGB(255,113,255))
#define	PINK		(RGB(200,0,200))

#define	LTGREEN		(RGB(0,255,0))
#define	GREEN		(RGB(0,188,0))
#define	DKGREEN		(RGB(0,128,0))

#define	CTBLUE		(RGB(180,180,255))
#define	BTBLUE		(RGB(80,160,255))
#define	LTBLUE		(RGB(0,0,255))
#define	BLUE		(RGB(0,0,188))
#define	DKBLUE		(RGB(0,0,128))

#define	LTYELLOW	(RGB(255,255,0))
#define	YELLOW		(RGB(188,188,0))
#define	DKYELLOW	(RGB(128,128,0))

#define	LTCYAN		(RGB(0,255,255))
#define	CYAN		(RGB(0,188,188))
#define	DKCYAN		(RGB(0,128,128))

#define	BTPURPLE	(RGB(239,223,247))
#define	LTPURPLE	(RGB(208,128,255))
#define	PURPLE		(RGB(98,92,213))
#define	DKPURPLE	(RGB(184,15,255))

#define	LTBROWN		(RGB(143,98,0))
#define	BROWN		(RGB(95,69,0))
#define	DKBROWN		(RGB(54,38,0))

#define	GOLDYELLOW	(RGB(237,205,153))

#define	dFONT_COUNT					10
#define	dFONT_NAME_LENGTH			64

#define	dWINDOW_FONT_NORMAL			0
#define	dWINDOW_FONT_BOLD			1
#define	dWINDOW_FONT_SMALL			2

#define	dOPERATE_SPECIAL_CODE		1
#define	dIS_INCORRECT_SPECIAL_CODE	2
#define	dOPERATE_ENTER_CODE			3

#define	dFITTEXT_LEFT				0
#define	dFITTEXT_WIDTH_CENTER		1
#define	dFITTEXT_HEIGHT_CENTER		2
#define	dFITTEXT_CENTER				3
#define	dFITTEXT_RIGHT				4

class CTextDCFontInfo
{
public:
	char			m_strFontName[dFONT_NAME_LENGTH];
	DWORD			m_dwStyle;
	BOOL			m_bIsItalic,m_bIsUnderline;
	int				m_iAngle,m_iCharset,m_iFontSize;
};


class cFONT_INFO
{
	int				m_iWidth,m_iHeight;
	HFONT			m_hFont;
	int				*m_pHangulOffset,*m_pEnglishOffset;
	BYTE			*m_pHangulBuffer,*m_pEnglishBuffer;

public:
	CTextDCFontInfo	m_fontInfo;

					cFONT_INFO()
					{
						m_hFont			=	NULL;
						m_pHangulOffset	=	NULL;
						m_pEnglishOffset=	NULL;
						m_pHangulBuffer	=	NULL;
						m_pEnglishBuffer=	NULL;
					}

	void			close()
					{
						if (m_hFont)
						{	DeleteObject(m_hFont);
							m_hFont	=	NULL;
						}

						_Free(m_pHangulOffset);
						_Free(m_pEnglishOffset);
						_Free(m_pHangulBuffer);
						_Free(m_pEnglishBuffer);
					}

	inline	HFONT	getFont()			{	return	m_hFont;			}
	inline	int		getFontWidth()		{	return	m_iWidth;			}
	inline	int		getFontHeight()		{	return	m_iHeight;			}
	inline	BYTE*	getHangulBuffer()	{	return	m_pHangulBuffer;	}
	inline	int*	getHangulOffset()	{	return	m_pHangulOffset;	}
	inline	BYTE*	getEnglishBuffer()	{	return	m_pEnglishBuffer;	}
	inline	int*	getEnglishOffset()	{	return	m_pEnglishOffset;	}


	inline	BOOL	isLoadedWindowFont(){	if (m_hFont)	return	TRUE;return	FALSE;}
	inline	BOOL	isLoadedSpriteFont(){	if (m_pHangulOffset && m_pEnglishOffset) return	TRUE;return	FALSE;}

	BOOL			loadFont(int _iXs,int _iYs,char *_lpszFont,DWORD _dwStyle);
	BOOL			loadFont(int _iXs,int _iYs,char *_lpszFont);
};

class CText
{
public:
	class	cTEXT_COLOR
	{
	public:
		char	m_strColor[16];
		DWORD	m_dwColor;
	};

	static	cTEXT_COLOR	s_aTextColor[40];

	static	HWND		s_hWND;
	static	HDC			s_hDC;
	static	BOOL		s_bIsDeviceActive;

	static	cFONT_INFO	s_aFont[dFONT_COUNT];
	static	char		s_strTempText[1024*10];

	BOOL				m_bIsShadowStatusPushed,m_isShadow,m_isPushShadowStatus,m_isReverse,m_isPutWidthCenter,m_bIsOrgReverseStatus;
	BOOL				m_isFitRightEdge;	//	????? ?????????? ???????? ????? ????????.
	int					m_iFontWidth,m_iFontHeight,m_iLineGap,m_iStackLineGap,m_iValidLineHeight;
	int					m_iFontFixWidth,m_iFontFixHeight;
	DWORD				m_dwShadowColor,m_dwTextColor,m_dwLastSettingColor;
	int					m_iLineCount,m_iBeginLine;	//	???????????????? ????? ??????
	CPos				m_posShadowMargin,m_pos,m_basePos;	//	???????????????? ????? ????????? ??? ????,???????
	BOOL				m_bIsIgnoreSomeCode;
	BOOL				m_bIsApplyColorText,m_bIsBlinkText;
	DWORD				m_dwBlinkShowTime,m_dwBlinkHideTime;

						CText()
						{
							m_posShadowMargin.x		=	2;
							m_posShadowMargin.y		=	2;
							m_isFitRightEdge		=	TRUE;
							m_iLineGap				=	2;
							m_iStackLineGap			=	2;
							m_isPushShadowStatus	=	FALSE;
							m_bIsShadowStatusPushed	=	FALSE;
							m_bIsIgnoreSomeCode		=	FALSE;
							m_bIsApplyColorText		=	TRUE;
							m_bIsBlinkText			=	FALSE;
							m_dwBlinkShowTime		=	700;
							m_dwBlinkHideTime		=	300;
						}

	inline	void		setShadowMargin(int _iXs,int _iYs=0xffff)
	{
		if (_iYs==0xffff)	_iYs	=	_iXs;

		m_posShadowMargin.x	=	_iXs;
		m_posShadowMargin.y	=	_iYs;
	}

	inline	void		setTextColor(DWORD _dwColor)
	{
		if (m_isReverse) 
		{
			SetBkColor	(s_hDC,_dwColor);
			_dwColor	=	~_dwColor;
			_dwColor	&=	0x00ffffff;
		}

		SetTextColor(s_hDC,_dwColor);
	}
	
	inline	void		pushShadowStatus(BOOL _isPutShadow)	
	{
		if (m_bIsShadowStatusPushed)
			return;
		m_bIsShadowStatusPushed	=	TRUE;
		m_isPushShadowStatus	=	m_isShadow;
		m_isShadow				=	_isPutShadow;
	}
	inline	void		popShadowStatus()					
	{
		if (!m_bIsShadowStatusPushed)
			return;

		m_bIsShadowStatusPushed	=	FALSE;

		m_isShadow	=	m_isPushShadowStatus;
	}
	inline	void		setShadow(BOOL shadow,DWORD color=0){m_isShadow=shadow,m_dwShadowColor=color;}
	inline	void		disableFitRightEdge(){m_isFitRightEdge	=	FALSE;}
	inline	void		enableFitRightEdge(){m_isFitRightEdge	=	TRUE;}
	inline	void		setLineGap(int _iGap)
	{
		if	(m_iValidLineHeight	==	m_iFontHeight+m_iLineGap)
			setValidLineHeight(m_iFontHeight+_iGap);

		m_iLineGap		=	_iGap;
		m_iStackLineGap	=	_iGap;
	}
	inline	int			getLineGap()
	{
		return	m_iLineGap;
	}

	inline	void		setValidLineHeight(int _iHeight=0xffff)
	{
		if (_iHeight==0xffff)	m_iValidLineHeight	=	m_iFontHeight+m_iLineGap;
		else					m_iValidLineHeight	=	_iHeight;
	}

	static	void		Init(HWND _hWnd,char *_lpStrDataFolder=NULL);
	static	void		LoadWindowFont(int _iXs,int _iYs,char *_lpszFont,int _iIndex,DWORD _dwStyle);
	static	void		LoadSpriteFont(int _iXs,int _iYs,char *_strHan,char *_strEng,int _iIndex);

	static	void		Destroy();

	static	void		ReadyForOutput()
	{
		s_bIsDeviceActive	=	TRUE;
		s_hDC				=	0;
	}
	static	void		OutputEnd()
	{
		s_bIsDeviceActive	=	FALSE;
		s_hDC				=	0;
	}
	void			replaceText(char* _strSaveText, char* _strTargetText, char* _strFindText, char* _strReplaceText,int _iSaveTextLength);
};

class CTextDC : public CText
{
	HFONT			m_hFont;
	int				m_iFontCheckSum;
	BOOL			m_bIsDecodeScript,m_bIsWantPorcessReturnKeyByUser,m_bIsMixText;
	int				m_iOldFont;
	int				m_iTemplateDeviceActiveCount,m_iDeviceActiveCount;
	cRECT			m_rectViewPort;			//	???? ????	
	WORD			m_wIsActiveDevice;
	DWORD			m_dwOrgColor,m_dwTemplateOrgColor;

	CTextDCFontInfo	m_fontInfo,m_orgFontInfo,m_templateDeviceFontInfo;

	void			drawScriptText(int _iX,int _iY,int _iDx,int _iDy,int _iBeginLine,DWORD _dwColor,char *_strText,BOOL _isPutCenter=FALSE);
	void			putScriptText(int _iX,int _iY,DWORD _dwColor,char *_strText);
	int				putScriptLine(int _iX,int _iY,char *_strText,BOOL _isIncludeSpecialCode,BOOL _isOutput=TRUE);	//	????????

	int				operateSpecialCode(char *_strText,int *_iPoint);
	int				checkSpecialCodeForGetPixelSize(char *_strText,int *_iPoint);
	int				checkSpecialCodeForGetLineCount(char *_strText,int *_iPoint);
	int				checkSpecialCodeForOneLine(char *_strText,int *_iPoint);
	int				checkSpecialCode(char *_strText,int *_iPoint);

	int				getColorNumber(char *_strText,int *_iTextPoint);
	int				getColor(char *_strText,int *_iTextPoint);
	int				getNotCrackeWordPoint(BYTE *_lpString,int _iPoint);
	BOOL			isOnlySpecialCodeText(char *_strText);
	inline	BOOL	removeFrontSpace(char *_strText,int *_iPoint);
	BOOL			checkIsCorrectTextColorScript(char *_strText);	//	???? ?????? ?????? ???? ???????????

	int				getWords(char *_strText,int *_ilpPoint);
	void			getTextExtentExPoint(HDC _hDC,char *_lpstrText,int _iStrlen,int _iDx,int *_lpiOutputTextLength,SIZE *_lpSize);
	void			getTextExtentPoint32(HDC _hDC,char *_lpstrText,int *_lpiOutputTextLength,SIZE *_lpSize);
	inline	void	activeDevice();
	inline	void	closeDevice();
	void			_selectFont(int _iSize,char *_lpstrFontName,DWORD _dwStype=FW_NORMAL,BOOL _bIsItalic=FALSE,BOOL _bIsUnderline=FALSE,int _iAngle=0,int _iCharset=DEFAULT_CHARSET);//??? ????
	int				_getOneLine(int _iWidth,char *_strText,int _iTextPoint,char *_strReturnText,BOOL _isIncludeSpecialCode,int *_iTextWidth);

	void			pushCurrentFontInfo();
	void			popCurrentFontInfo();
	void			pushCurrentFontInfo(CTextDCFontInfo *_lpInfo,DWORD *_lpdwColor);
	void			popCurrentFontInfo(CTextDCFontInfo *_lpInfo,DWORD _dwColor);

public:
	int				m_iSelectFont,m_iBkMode,m_iCorrectTextWidth;	//	?????? ???,?????? ??? ?þ·?????(??? ?????? ?????? ????)

					CTextDC();
					~CTextDC();

	int				findSpecialCodePoint(char *_lpString,int _iRange=0x7ffffff);
	void			releaseClipArea();
	void			setClipArea(cRECT *_lpRect);
	void			setClipArea(int _x1,int _y1,int _x2,int _y2);

	inline	void	scriptPutOn()		{m_bIsDecodeScript	=TRUE;}		//	?????????? ?þÆ?????? ???
	inline	void	scriptPutOff()		{m_bIsDecodeScript	=FALSE;}		//	?????????? ????????
	inline	void	textMixOn()			{m_bIsMixText		=TRUE;}		//	?????????? ?þÆ?????? ???
	inline	void	textMixOff()		{m_bIsMixText		=FALSE;}		//	?????????? ????????
	inline	void	someCodeIgnoreOn()	{m_bIsIgnoreSomeCode=TRUE;}		//	?????????? ?þÆ?????? ???
	inline	void	someCodeIgnoreOff()	{m_bIsIgnoreSomeCode=FALSE;}	//	?????????? ????????
	inline	void	blinkTextOn()		{m_bIsBlinkText		=TRUE;}		//	?????????? ????????
	inline	void	blinkTextOff()		{m_bIsBlinkText		=FALSE;}	//	?????????? ????????
	inline	void	setBlinkTime(int _iShowTime,int _iHideTime)
	{
		m_dwBlinkShowTime	=	_iShowTime;
		m_dwBlinkHideTime	=	_iHideTime;
	}

	inline	void	colorChangeOn()		{m_bIsApplyColorText=TRUE;}		//	?????????? ?þÆ?????? ???
	inline	void	colorChangeOff()	{m_bIsApplyColorText=FALSE;}	//	?????????? ????????

	inline	void	userReturnOn()		{m_bIsWantPorcessReturnKeyByUser	=	TRUE;}
	inline	void	userReturnOff()		{m_bIsWantPorcessReturnKeyByUser	=	FALSE;}

	inline	HFONT	getFont()
	{
		if (m_hFont)	return	m_hFont;

		if (m_iSelectFont	!=	0xffff)	return	s_aFont[m_iSelectFont].getFont();

		CRY(1,"???~~!! ??. ????? ?????!!!");

		return	NULL;
	}
	inline	void	calcFontSize();		//	????? ???? ???? ?³???? ?????.
	inline	int		getFontHeight(){return	m_iFontHeight;}
	inline	int		getFontWidth(){return	m_iFontWidth;}


	inline	void	controlTextWidth(int _iDeltaWidth){m_iCorrectTextWidth	=	_iDeltaWidth;}	//	???? ?????? ????
	inline	void	correctTextWidth(){SetTextCharacterExtra(s_hDC,m_iCorrectTextWidth);}	//	???? ????

	inline	BOOL	isIncludeSpecialCode(char *_lpstrText)
	{
		if	(findSpecialCodePoint(_lpstrText,strlen(_lpstrText))	!=	-1)	return	TRUE;

		return	FALSE;
	}

	BOOL			activeTemplateDevice();
	BOOL			closeTemplateDevice();

	BOOL			selectFont(int _iSelectFont,BOOL _bIsCalcFontSize=TRUE);
	void			selectFont(int _iSize,char *_lpstrFontName,DWORD _dwStype=FW_NORMAL,BOOL _bIsItalic=FALSE,BOOL _bIsUnderline=FALSE,int _iAngle=0,int _iCharset=DEFAULT_CHARSET);//??? ????
	void			selectFont(CTextDCFontInfo *_lpFontInfo);

	void			putNormalText(int xPos,int yPos,DWORD color,char *_strText,...);//	???????? ???????????
	void			putNormalText(int _iX,int _iY,int _iDx,int _iDy,int _iBeginLine,DWORD _dwColor,char *_strText,...);

	void			put(int xPos,int yPos,DWORD color,char *_strText,...);		//	Device Cotext?? ??????.
	void			putRF(int xPos,int yPos,DWORD color,char *_strText,...);	//	???????? ????? ???
	void			put(int _iX,int _iY,int _iDx,int _iDy,int _iBeginLine,DWORD _dwColor,char *_strText,...);
	void			cPut(int xPos,int yPos,DWORD color,char *_strText,...);		//	xPos,yPos?? ?????? ??????.
	void			cPut(int _iX,int _iY,int _iDx,int _iDy,int _iBeginLine,DWORD _dwColor,int _iCenterType,char *_strText,...);
	void			cPut(int _iX,int _iY,int _iDx,int _iDy,int _iBeginLine,DWORD _dwColor,char *_strText,...);
	void			c2LinePut(int _iX,int _iY,int _iDx,int _iDy,int _iBeginLine,DWORD _dwColor,int _iCenterType,char *_strText,...);

	void			putReverse(int xPos,int yPos,int _iBegin,int _iEnd,DWORD _dwColor,char *_strText);
	void			putReverse(int xPos,int yPos,DWORD color,char *pStr,...);	//	???????

	int				getLineCount(int xs,char *pStr,BOOL _isActiveDevice=TRUE,BOOL _isPutWidthCenter=FALSE);
	int				getPixelSize(char *_strText);
	int				getTextHeight(int _iWidth,char *_strText);

	int				getMaxLineSize(char *_strText);
	int				getOneLine(int _iDx,char *_strText,int _iTextPoint,char *_strReturnText,BOOL _isIncludeSpecialCode,int *_iEndPoint);
	inline	int		getLineThick()				{return m_iFontHeight+m_iLineGap;}

};

class CTextMT	:	public CText
{
	int					*m_lpHangulOffset,*m_lpEnglishOffset;
	BYTE				*m_lpHangulBuffer,*m_lpEnglishBuffer;

	inline	char*		w2j(BYTE *String);		//	??????? ?????????..

	void				putNormal(int _iXPos,int _iYPos,DWORD _dwColor,char *_lpString);
	void				putHB(int _iXPos,int _iYPos,DWORD _dwColor,char *_lpString);
	void				putAB(int _iXPos,int _iYPos,DWORD _dwColor,char *_lpString,int _iAlpha);

public:
	
						CTextMT();
						~CTextMT();

	BOOL				selectFont(int _iSelectFont);
	BOOL				loadFont(char *_strName);

	void				put(int _iXPos,int _iYPos,DWORD _wColor,char *_lpString,int _iAlpha=32);
	void				cPut(int _iXPos,int _iYPos,DWORD _wColor,char *_lpString,int _iAlpha=32);

	char *				getLine(int xs,int line,char *pStr);
	int					getLineCount(int xs,char *pStr);
	void				fitPut(int _iXPos,int _iYPos,int _iWidth,int _iHeight,DWORD _dwColor,char *_lpString,int _iBase,int _iAlpha=32);

	char *				getLine(int xs,int line,char **_lpStrings);
	int					getLineCount(int xs,char **_lpStrings);
	void				fitPut(int _iXPos,int _iYPos,int _iWidth,int _iHeight,DWORD _dwColor,char **_lpStrings,int _iBase,int _iAlpha=32);

	void				setShadow(BOOL shadow,DWORD color=0)		{m_isShadow=shadow,m_dwShadowColor=color;}
};


#define	dDEFAULT_MAX_TEXT_LINE_COUNT	100
#define	dDEFAULT_TEXT_LENGTH			256

class	CTextList
{
	char			*m_pstrText;
	int				m_iMaxLineCount,m_iLineLength;
	int				m_iLineCounter;
	int				m_iLineGap;
	int				m_iHeadLine,m_iEndLine;	//	????????? ???/????? ???
	int				m_iVisibleLineCount;	//	????? ?????????? ??? ??	
	cRECT			m_rectViewPort;			//	???? ????	
	int				m_iFirstLine;			//	??????? ???????? ?????? ???
	int				m_iExpandValueCount,m_iLastestLine;
	int				m_iMaxColumnCount,*m_pColumnPos;
	int				m_iDX,m_iDY;
	int				m_iDisplayLineCount;
	int				m_iFocusLine;
	int				m_iCurrentText;

	CTextDC			*m_lpText;

public:

					CTextList();
					~CTextList();

	void			init(cRECT *_lpViewPort,CTextDC *_lpText,int _iLineCount=dDEFAULT_MAX_TEXT_LINE_COUNT,int _iTextLength=dDEFAULT_TEXT_LENGTH);	//	?????
	void			init(int _iLineCount,int _iColumnCount,int *_lpiColumnSize,int _iExpandValueCount=0);	//	?????
	void			initForMultiColumn(cRECT *_lpViewPort,CTextDC *_lpText,int _iLineCount,int _iColumnCount,int *_lpiColumnSize,int _iExpandValueCount);

	int				getFocusedLine();
	int				getFocusLine(int _iX,int _iY);
	void			setFocusLine(int _iLine);

	void			setLineGap(int _iLineGap);//	?????? ????
	void			reset();	//	?????
	void			close();	//	????	
	int				getHeight();//	??? ??????	
	inline	cRECT*	getRect(){return &m_rectViewPort;}//	???? ????	
	void			decreaseLineCount();	//	??? ??????? ??????.
	void			increaseLineCount();
	void			setRefitPos(int _iDX=0,int _iDY=0);

	int				getMaxLengthInViewText();//	?????? ???????? ??????????? ?³???
	inline	CTextDC*	getTextDC(){return	m_lpText;}
	inline	void	setTextDC(CTextDC* _lpTextDC)
	{
		m_lpText	=	_lpTextDC;
	}
	inline	int		getLineCount(){return	m_iLineCounter;}	//	???????? ??	
	inline	int		getDisplayLineCount(){return	m_iDisplayLineCount;}	//	???????? ??	
	inline	int		getVisibleLineCount(){return	m_iVisibleLineCount;}	//	??????? ????? ??	
	inline	int		getFirstLine(){return	m_iFirstLine;}	//	????? ??? ????
	inline	void	increaseFirstLine()	{setFirstLine(m_iFirstLine+1);}	//	?????? ??? ???
	inline	void	decreaseFirstLine()	{setFirstLine(m_iFirstLine-1);}	//	?????? ??? ????
	void			setFirstLine(int _iLine);	//	????? ??? ????

	inline	void	setDisplayLineCount(int _iLine,int _iLineCount)	//	????????? ?????? ????? ??? ???? ??? ??	
	{
		if (_iLine	>=	m_iMaxLineCount)
			return;

		*(DWORD *)(m_pstrText+_iLine*m_iLineLength+4)	=	(DWORD)_iLineCount;
	}
	inline	int		getDisplayLineCount(int _iLine)		//	????????? ?????? ????? ??? ???? ??? ??	
	{
		if	(_iLine	>=	m_iMaxLineCount)
			return	1;

		return	*(DWORD *)(m_pstrText+_iLine*m_iLineLength+4);
	}

	inline	char*	getTextByLine(int _iLine,int _iColumn=0)	//	???? ????~
	{
		if (_iLine	>=	m_iMaxLineCount)	return	NULL;
		if (_iColumn>=	m_iMaxColumnCount)	return	NULL;

		int	iAddress	=	_iLine*m_iLineLength+m_pColumnPos[_iColumn];

		return	&m_pstrText[iAddress];
	}

	inline	DWORD	getColor(int _iLine)	//	???? ????~
	{
		if (_iLine	>=	m_iMaxLineCount)	return	WHITE;

		return	*(DWORD *)(m_pstrText+_iLine*m_iLineLength);
	}

	inline	void	setColor(int _iLine,DWORD _dwColor)	//	????????
	{
		if (_iLine	>=	m_iMaxLineCount)
			return;

		*(DWORD *)(m_pstrText+_iLine*m_iLineLength)	=	_dwColor;
	}
	inline	void	setLastestLineColor(DWORD _dwColor)	//	???? ????~
	{
		*(DWORD *)(m_pstrText+m_iLastestLine*m_iLineLength)	=	_dwColor;
	}

	inline	int		getValue(int _iLine,int _iIndex)	//	???? ????~
	{
		if (_iLine	>=	m_iMaxLineCount		)	return	0;
		if (_iIndex	>=	m_iExpandValueCount	)	return	0;

		return	*(int *)(m_pstrText+m_iLineLength*_iLine+8+_iIndex*4);
	}
	inline	void	setValue(int _iLine,int _iIndex,int _iValue)	//	???? ????~
	{
		if (_iLine	>=	m_iMaxLineCount		)	return;
		if (_iIndex	>=	m_iExpandValueCount	)	return;

		*(int *)(m_pstrText+m_iLineLength*_iLine+8+_iIndex*4)	=	_iValue;
	}
	inline	void	setLastestLineValue(int _iIndex,int _iValue)	//	???? ????~
	{
		if (_iIndex	>=	m_iExpandValueCount	)	return;

		*(int *)(m_pstrText+m_iLineLength*m_iLastestLine+8+_iIndex*4)	=	_iValue;
	}

	int				addColorText(DWORD _dwColor,char *_lpString,BOOL _bIsIncreaseLineCounter=FALSE);	//	?????? ????
	int				addText(char *_lpString,BOOL _bIsIncreaseLineCounter=FALSE);	//	?????? ????
	int				addText(int _iColumn,char *_lpString,BOOL _bIsIncreaseLineCounter=FALSE);	//	?????? ????
	void			drawText();	//	?????? ????????
	char*			getText(int _iIndex,int _iColumn=0);	//	???? ????~
	char*			getText(char *_lpstrText,int _iColumn=0);	//	???? ????~
	char*			getLastestText(int _iColumn=0);	//	????? ????????
	int				getValue(char *_lpstrText,int _iColumn=0);	//	???
	DWORD			getColor(char *_lpstrText);	//	????

	char*			getNextText(int _iColumn=0);
	char*			getPreviousText(int _iColumn=0);

	void			setCurrentTextLineToLastestLine();

	inline	char*	getLine(int _iIndex)	//	???? ????~
	{
		if	(_iIndex >= m_iLineCounter)
			return	NULL;

		int	iLine	=	_iIndex+m_iHeadLine;

		if	(iLine	>=	m_iMaxLineCount)
			iLine	-=	m_iMaxLineCount;

		return	m_pstrText+m_iLineLength*iLine;
	}
	inline	int		getLineThick()				{return m_lpText->m_iFontHeight+m_iLineGap;}
	inline	int		getVisibleHeight()			{return getLineThick()*m_iVisibleLineCount;}
	void			resetFrame(cRECT *_lpRect,int _iCheckColumn=-1);
};	//	class	CTextList

BOOL	Is2ByteChar(char _char);

#endif