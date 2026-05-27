#include	<stdio.h>
#include	<stdlib.h>
#include	<conio.h>
#include	<string.h>

#include	"CText.H"
#include	"cDRAW.H"
#include	"CCanvas.H"
#include	"CExceptionHandler.H"

#ifdef	_FOR_ENG
#define _SHARP_SHARP_TO_PERCENTEMARK
#define	_USE_WORD_BREAK
#endif

#ifdef	_FOR_THAI
#define _SHARP_SHARP_TO_PERCENTEMARK
#define	_USE_WORD_BREAK
#endif

#ifdef	_SHARP_SHARP_TO_PERCENTEMARK
inline	void
OpertePercentageMark(char *_lpstrText)
{
	int		iLen	=	strlen(_lpstrText);
	char	strResult[4096];
	int		iPoint	=	0;
	
	for(int i=0;i<iLen;i++)
	{
		if	(_lpstrText[i]	==	'#'	&&	_lpstrText[i+1]	==	'#'	&&	i	<	iLen-1)
		{
			strResult[iPoint++]	=	'%';
			i++;
		}
		else
			strResult[iPoint++]	=	_lpstrText[i];
	}
	
	strResult[iPoint]	=	NULL;
	
	memcpy(_lpstrText,strResult,iPoint+1);
}
#endif

#ifdef	_FOR_THAI
inline	void
OperteCRMark(char *_lpstrText)
{
	int		iLen	=	strlen(_lpstrText);
	char	strResult[4096];
	int		iPoint	=	0;
	
	for(int i=0;i<iLen;i++)
	{
		if	(_lpstrText[i]	==	0x0a)//	&&	_lpstrText[i+1]	==	0x0a	&&	i	<	iLen-1)
		{
			strResult[iPoint++]	=	' ';
			strResult[iPoint++]	=	0x0a;
			//			strResult[iPoint++]	=	0x0d;
			i++;
		}
		else
			strResult[iPoint++]	=	_lpstrText[i];
	}
	
	strResult[iPoint]	=	NULL;
	
	memcpy(_lpstrText,strResult,iPoint+1);
}
#endif

inline	BOOL
Is2ByteChar(char *_lpstrText,int _iPoint)
{
	int	iPoint	=	0;

	while(_lpstrText[iPoint])
	{
		if (Is2ByteChar(_lpstrText[iPoint]))
		{
			if (_iPoint	==	iPoint || _iPoint	==	iPoint+1)
				return	TRUE;

			iPoint++;
		}

		iPoint++;
	}

	return	FALSE;
}

BOOL
Is2ByteChar(char _char)
{
#ifdef	_FOR_JAPAN
	if	((BYTE)_char	>=	0xa1 && (BYTE)_char <=0xdf)
		return	FALSE;
#endif

#ifdef	_FOR_THAI
	return	FALSE;
#endif

	if	(_char	<	0)
		return	TRUE;

	return	FALSE;
}

CTextDC::CTextDC()
{
	m_dwShadowColor					=	0;
	m_isShadow						=	TRUE;
	m_iBkMode						=	TRANSPARENT;
	m_isReverse						=	FALSE;
	m_iSelectFont					=	0;
	m_hFont							=	0;
	m_iFontCheckSum					=	0;
	m_bIsDecodeScript				=	TRUE;
	m_bIsMixText					=	TRUE;
	m_bIsWantPorcessReturnKeyByUser	=	FALSE;

	m_iTemplateDeviceActiveCount	=	FALSE;
	m_iDeviceActiveCount			=	FALSE;
	m_rectViewPort.set(0,0,10000,10000);
	setBlinkTime(800,200);
}

CTextDC::~CTextDC()
{
	if (m_hFont)	DeleteObject(m_hFont);
}

// insu function add
//┌---------------------------------------------------------------┐
//│ cPut을 이용해 TEXT 2줄 출력시에 좀 더 균형있게                │
//│ 출력되도록 변경된 함수                                        │
//│                                                               │
//│ 매개변수는 cPut과 동일하다.                                   │
//└---------------------------------------------------------------┘
void
CTextDC::c2LinePut(int _iX,int _iY,int _iDx,int _iDy,int _iBeginLine,DWORD _dwColor,int _iCenterType,char *_strText,...)
{
	if (!_strText)
		return;

	int istrTextLen = strlen(_strText);
	int iOutputTextLength = 0;
	SIZE	size;
	BOOL	bResult		=	GetTextExtentExPoint(s_hDC, _strText, istrTextLen, _iDx, &iOutputTextLength, NULL, &size);

	// 4글자 정도다.. 이것도 약간 문제
	if ((istrTextLen-iOutputTextLength) && istrTextLen-iOutputTextLength < 4)
	{		
		char* strRealText = new char [istrTextLen+1]; // 실제로 출력될 문자열 값
		memset(strRealText, '\0', istrTextLen+1);     // 문자열 초기화 (쓰레기값을 지운다)

		// 공백 글자를 찾는다.
		char* seek = strstr(_strText, " ");
		while (seek)
		{
			strncat(strRealText, _strText, istrTextLen-strlen(seek));
			seek = strstr(&seek[1], " ");

			if ((int)strlen(strRealText) >= istrTextLen/2)
			{
				// 출력한다.
				cPut(_iX, _iY, _iDx, _iDy, _iBeginLine, _dwColor, _iCenterType, strRealText);
				cPut(_iX, _iY+(m_iFontHeight+2), _iDx, _iDy, _iBeginLine, _dwColor, _iCenterType, &_strText[strlen(strRealText)+1]);
				delete[] strRealText;
				return;
			}
			if (!seek)
				break;
			memset(strRealText, '\0', istrTextLen+1);
		}
		cPut(_iX, _iY, _iDx, _iDy, _iBeginLine, _dwColor, _iCenterType, strRealText);
		delete[] strRealText;
		return;
	}
	cPut(_iX, _iY, _iDx, _iDy, _iBeginLine, _dwColor, _iCenterType, _strText);
}
// insu function add end


BOOL
CTextDC::selectFont(int _iSelectFont,BOOL _bIsCalcFontSize)
{
	if	(m_iSelectFont	==	0xffff		)
		return FALSE;
	if	(_iSelectFont	>=	dFONT_COUNT	)
		return FALSE;
	if	(!s_aFont[_iSelectFont].isLoadedWindowFont())
		return FALSE;

	m_iFontFixWidth				=	s_aFont[_iSelectFont].getFontWidth();
	m_iFontFixHeight			=	s_aFont[_iSelectFont].getFontHeight();
	m_iSelectFont				=	_iSelectFont;

	memcpy(&m_fontInfo,&s_aFont[_iSelectFont].m_fontInfo,sizeof(CTextDCFontInfo));

	if (_bIsCalcFontSize)
		calcFontSize();

	if (m_iFontFixWidth	==	0)
		m_iFontFixWidth	=	m_iFontWidth;
	if (m_iFontFixHeight==	0)
		m_iFontFixHeight=	m_iFontHeight;

	m_iValidLineHeight			=	m_iFontFixHeight+m_iLineGap;

	return	TRUE;
}

void
CTextDC::selectFont(CTextDCFontInfo *_lpFontInfo)
{
	selectFont(_lpFontInfo->m_iFontSize,_lpFontInfo->m_strFontName,_lpFontInfo->m_dwStyle,_lpFontInfo->m_bIsItalic,_lpFontInfo->m_bIsUnderline,_lpFontInfo->m_iAngle,_lpFontInfo->m_iCharset);
}

//폰트 생성
void
CTextDC::selectFont(int _iSize,char *_lpstrFontName,DWORD _dwStyle,BOOL _bIsItalic,BOOL _bIsUnderline,int _iAngle,int _iCharset)
{
	CTextDCFontInfo	temp;

	temp.m_dwStyle				=	_dwStyle;
	temp.m_bIsItalic			=	_bIsItalic;
	temp.m_bIsUnderline			=	_bIsUnderline;
	temp.m_iAngle				=	_iAngle;
	temp.m_iCharset				=	_iCharset;
	temp.m_iFontSize			=	_iSize;
	strcpy(temp.m_strFontName,_lpstrFontName);
	
	m_iSelectFont				=	0xffff;

/*	if (m_hFont)
	{
		int	iResult	=	memcmp(&temp,&m_fontInfo,sizeof(temp));

		if (iResult	==	0)
			return;
	}
	else
	memcpy(&m_orgFontInfo,&temp,sizeof(temp));
*/
	_selectFont(_iSize,_lpstrFontName,_dwStyle,_bIsItalic,_bIsUnderline,_iAngle,_iCharset);

	memcpy(&m_fontInfo,&temp,sizeof(temp));
}

//폰트 생성
void
CTextDC::_selectFont(int _iSize,char *_lpstrFontName,DWORD _dwStyle,BOOL _bIsItalic,BOOL _bIsUnderline,int _iAngle,int _iCharset)
{
	if	(m_hFont)
		DeleteObject(m_hFont);

	m_hFont			=	CreateFont(_iSize,0,   _iAngle,_iAngle,  _dwStyle,_bIsItalic,_bIsUnderline,0,_iCharset,OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
									DEFAULT_QUALITY, FIXED_PITCH | FF_ROMAN, _lpstrFontName);

	calcFontSize();

	if	(m_iValidLineHeight	==	0)
		m_iValidLineHeight	=	m_iFontHeight+m_iLineGap;

	if	(m_iFontFixWidth	==	0)
		m_iFontFixWidth	=	m_iFontWidth;
	if	(m_iFontFixHeight==	0)
		m_iFontFixHeight=	m_iFontHeight;

	if	(s_hDC)
		SelectObject(s_hDC,getFont());
}

inline	void
CTextDC::activeDevice()
{
	if	(m_iDeviceActiveCount++)
		return;

	g_canvas.getDCForText(&s_hDC);

	SelectObject( s_hDC, getFont());
	SetBkMode( s_hDC, m_iBkMode);
	correctTextWidth();	

	HRGN	hRgn	=	CreateRectRgn(m_rectViewPort.x1,m_rectViewPort.y1,m_rectViewPort.x2,m_rectViewPort.y2);

	SelectClipRgn(s_hDC,hRgn);

	DeleteObject(hRgn);
}

inline	void
CTextDC::closeDevice()
{
	if (m_iDeviceActiveCount	==	0	)	return;

	m_iDeviceActiveCount--;

	if (m_iDeviceActiveCount			)	return;

	if (!g_canvas.releaseDCForText(s_hDC))	return;

	s_hDC	=	0;
}

BOOL
CTextDC::activeTemplateDevice()
{
	if	(s_bIsDeviceActive)
	{
		activeDevice();
		m_iTemplateDeviceActiveCount++;

		return TRUE;
	}

	if (s_hDC)
	{
		m_iTemplateDeviceActiveCount++;
		return	TRUE;
	}

	if (m_iTemplateDeviceActiveCount++)	return TRUE;

	s_hDC		=	GetDC(s_hWND);
	m_iOldFont	=	m_iSelectFont;

	SelectObject( s_hDC, getFont());
	SetBkMode	( s_hDC, m_iBkMode);

	return	TRUE;
}

BOOL
CTextDC::closeTemplateDevice()
{
	if (m_iTemplateDeviceActiveCount==	0	)	return	FALSE;

	if (s_bIsDeviceActive)
	{
		closeDevice();
		m_iTemplateDeviceActiveCount--;

		return	TRUE;
	}

	if (s_hDC	==	0	)
	{
		m_iTemplateDeviceActiveCount--;

		return	FALSE;
	}

	m_iTemplateDeviceActiveCount--;

	if (m_iTemplateDeviceActiveCount		)	return	FALSE;

	selectFont(m_iOldFont);
	ReleaseDC(s_hWND,s_hDC);

	s_hDC	=	0;

	return	TRUE;
}

//
//	폰트의 높이를 구한다.
inline	void
CTextDC::calcFontSize()
{
	HDC		hDC	=	GetDC(s_hWND);

	SelectObject(hDC, getFont());

	TEXTMETRIC	tm;
	GetTextMetrics(hDC,&tm);

	if	(m_iValidLineHeight	==	m_iFontHeight+m_iLineGap)
		setValidLineHeight(tm.tmHeight+m_iLineGap);

	m_iFontHeight	=	tm.tmHeight;
	m_iFontWidth	=	tm.tmAveCharWidth;

	ReleaseDC(s_hWND,hDC);
}

//
//	가장 기본적인 출력
void
CTextDC::put(int xPos,int yPos,DWORD dwColor,char *_strText,...)
{
	if	(!_strText)
		return;

	if	(m_bIsBlinkText)
		if	((timeGetTime()%(m_dwBlinkShowTime+m_dwBlinkHideTime))<m_dwBlinkHideTime)
			return;

	if	(m_bIsMixText)
	{
		va_list		argptr;
		va_start(argptr, _strText);
		vsprintf(s_strTempText,_strText,argptr);
		va_end(argptr);
	}
	else
		strcpy(s_strTempText,_strText);

#ifdef	_SHARP_SHARP_TO_PERCENTEMARK
	OpertePercentageMark(s_strTempText);
#endif
#ifdef	_FOR_THAI
	OperteCRMark(s_strTempText);
#endif

	activeDevice();

	putScriptText(xPos,yPos,dwColor,s_strTempText);

	closeDevice();
}

//
//	오른쪽에 맞춰서 출력
void
CTextDC::putRF(int xPos,int yPos,DWORD _dwColor,char *_strText,...)
{
	if (!_strText)
		return;

	if (m_bIsBlinkText)
		if ((timeGetTime()%(m_dwBlinkShowTime+m_dwBlinkHideTime))<m_dwBlinkHideTime)
			return;

	if (m_bIsMixText)
	{
		va_list		argptr;
		va_start(argptr, _strText);
		vsprintf(s_strTempText,_strText,argptr);
		va_end(argptr);
	}
	else
		strcpy(s_strTempText,_strText);

#ifdef	_SHARP_SHARP_TO_PERCENTEMARK
	OpertePercentageMark(s_strTempText);
#endif
#ifdef	_FOR_THAI
	OperteCRMark(s_strTempText);
#endif

	activeDevice();

	int	iWidth	=	getPixelSize(s_strTempText);

	putScriptText(xPos-iWidth,yPos,_dwColor,s_strTempText);

	closeDevice();
}

//
//	_iWidth,iDy에 맞춰서 출력
void
CTextDC::put(int xPos,int yPos,int _iWidth,int _iHeight,int _iBeginLine,DWORD dwColor,char *_strText,...)
{
	if (!_strText)
		return;

	if (m_bIsBlinkText)
		if ((timeGetTime()%(m_dwBlinkShowTime+m_dwBlinkHideTime))<m_dwBlinkHideTime)
			return;

	if (m_bIsMixText)
	{
		va_list		argptr;
		va_start(argptr, _strText);
		vsprintf(s_strTempText,_strText,argptr);
		va_end(argptr);
	}
	else
		strcpy(s_strTempText,_strText);

#ifdef	_SHARP_SHARP_TO_PERCENTEMARK
	OpertePercentageMark(s_strTempText);
#endif
#ifdef	_FOR_THAI
	OperteCRMark(s_strTempText);
#endif

	activeDevice();

	drawScriptText(xPos,yPos,_iWidth,_iHeight,_iBeginLine,dwColor,s_strTempText);

	closeDevice();
}

//
//	가장 기본적인 출력
void
CTextDC::putNormalText(int xPos,int yPos,DWORD dwColor,char *_strText,...)
{
	if (!_strText)
		return;

	if (m_bIsBlinkText)
		if ((timeGetTime()%(m_dwBlinkShowTime+m_dwBlinkHideTime))<m_dwBlinkHideTime)
			return;

	if (m_bIsMixText)
	{
		va_list		argptr;
		va_start(argptr, _strText);
		vsprintf(s_strTempText,_strText,argptr);
		va_end(argptr);
	}
	else
		strcpy(s_strTempText,_strText);

#ifdef	_SHARP_SHARP_TO_PERCENTEMARK
	OpertePercentageMark(s_strTempText);
#endif
#ifdef	_FOR_THAI
	OperteCRMark(s_strTempText);
#endif

	activeDevice();

	scriptPutOff();
	putScriptText(xPos,yPos,dwColor,s_strTempText);
	scriptPutOn();

	closeDevice();
}

//
//	스크립트가 포함되지 않은 한줄짜리 문장을 출력
void
CTextDC::putNormalText(int xPos,int yPos,int _iWidth,int _iHeight,int _iBeginLine,DWORD dwColor,char *_strText,...)
{
	if (!_strText)
		return;

	if (m_bIsBlinkText)
		if ((timeGetTime()%(m_dwBlinkShowTime+m_dwBlinkHideTime))<m_dwBlinkHideTime)
			return;

	if (m_bIsMixText)
	{
		va_list		argptr;
		va_start(argptr, _strText);
		vsprintf(s_strTempText,_strText,argptr);
		va_end(argptr);
	}
	else
		strcpy(s_strTempText,_strText);

#ifdef	_SHARP_SHARP_TO_PERCENTEMARK
	OpertePercentageMark(s_strTempText);
#endif
#ifdef	_FOR_THAI
	OperteCRMark(s_strTempText);
#endif

	activeDevice();

	scriptPutOff();
	drawScriptText(xPos,yPos,_iWidth,_iHeight,_iBeginLine,dwColor,s_strTempText);
	scriptPutOn();

	closeDevice();
}

//
//	가운데 찍기
void
CTextDC::cPut(int xPos,int yPos,DWORD dwColor,char *_strText,...)
{
	if (!_strText)
		return;

	if (m_bIsBlinkText)
		if ((timeGetTime()%(m_dwBlinkShowTime+m_dwBlinkHideTime))<m_dwBlinkHideTime)
			return;

	if (m_bIsMixText)
	{
		va_list		argptr;
		va_start(argptr, _strText);
		vsprintf(s_strTempText,_strText,argptr);
		va_end(argptr);
	}
	else
		strcpy(s_strTempText,_strText);

#ifdef	_SHARP_SHARP_TO_PERCENTEMARK
	OpertePercentageMark(s_strTempText);
#endif
#ifdef	_FOR_THAI
	OperteCRMark(s_strTempText);
#endif

	activeDevice();

	int	iSize	=	getPixelSize(s_strTempText);

	xPos	-=	iSize/2;

	putScriptText(xPos,yPos,dwColor,s_strTempText);

	closeDevice();
}

//
//	가운데 찍기
void
CTextDC::cPut(int _iX,int _iY,int _iWidth,int _iHeight,int _iBeginLine,DWORD _dwColor,char *_strText,...)
{
	if (!_strText)
		return;

	if (m_bIsBlinkText)
		if ((timeGetTime()%(m_dwBlinkShowTime+m_dwBlinkHideTime))<m_dwBlinkHideTime)
			return;

	BOOL	isWidthCenter	=	FALSE;

	if (m_bIsMixText)
	{
		va_list		argptr;
		va_start(argptr, _strText);
		vsprintf(s_strTempText,_strText,argptr);
		va_end(argptr);
	}
	else
		strcpy(s_strTempText,_strText);

#ifdef	_SHARP_SHARP_TO_PERCENTEMARK
	OpertePercentageMark(s_strTempText);
#endif
#ifdef	_FOR_THAI
	OperteCRMark(s_strTempText);
#endif

	activeDevice();

	drawScriptText(_iX,_iY,_iWidth,_iHeight,_iBeginLine,_dwColor,s_strTempText,TRUE);

	closeDevice();
}

//
//	가운데 찍기
void
CTextDC::cPut(int _iX,int _iY,int _iWidth,int _iHeight,int _iBeginLine,DWORD _dwColor,int _iCenterType,char *_strText,...)
{
	if (!_strText)
		return;

	if (m_bIsBlinkText)
		if ((timeGetTime()%(m_dwBlinkShowTime+m_dwBlinkHideTime))<m_dwBlinkHideTime)
			return;

	BOOL	isWidthCenter	=	FALSE;

	if (m_bIsMixText)
	{
		va_list		argptr;
		va_start(argptr, _strText);
		vsprintf(s_strTempText,_strText,argptr);
		va_end(argptr);
	}
	else
		strcpy(s_strTempText,_strText);

	memcpy(&m_orgFontInfo,&m_fontInfo,sizeof(m_fontInfo));

#ifdef	_SHARP_SHARP_TO_PERCENTEMARK
	OpertePercentageMark(s_strTempText);
#endif
#ifdef	_FOR_THAI
	OperteCRMark(s_strTempText);
#endif

	activeDevice();

	if (_iCenterType & dFITTEXT_WIDTH_CENTER	)	isWidthCenter		=	TRUE;

	if (_iCenterType & dFITTEXT_HEIGHT_CENTER	)
	{
		int	iLineCount	=	getLineCount(_iWidth,s_strTempText,FALSE,isWidthCenter);
		int	iTextHeight	=	(m_iFontHeight+m_iLineGap)*iLineCount-m_iLineGap;

		if(_iHeight	>	iTextHeight) _iY	+=	(_iHeight-iTextHeight)/2;
	}

	drawScriptText(_iX,_iY,_iWidth,_iHeight,_iBeginLine,_dwColor,s_strTempText,isWidthCenter);

	closeDevice();
}

//
//	뒤집어 찍기.. begin부터 end까지 반전 시켜서 찍는다.
void
CTextDC::putReverse(int xPos,int yPos,int _iBegin,int _iEnd,DWORD _dwColor,char *_strText)
{
	if (!_strText)
		return;

	if (m_bIsBlinkText)
		if ((timeGetTime()%(m_dwBlinkShowTime+m_dwBlinkHideTime))<m_dwBlinkHideTime)
			return;

	activeDevice();

	SIZE	size;

	if (_iEnd	>=	_iBegin)
	{
		if (_iBegin	>	0)
		{
			strcpy(s_strTempText,_strText);

			s_strTempText[_iBegin]	=	NULL;

			putScriptText(xPos,yPos,_dwColor,s_strTempText);
			GetTextExtentPoint32(s_hDC,s_strTempText,strlen(s_strTempText),&size);

			xPos	+=	size.cx;
		}

		strcpy(s_strTempText,&_strText[_iBegin]);
		s_strTempText[_iEnd-_iBegin+1]	=	NULL;

		m_isReverse	=	TRUE;

		SetBkMode	(s_hDC,OPAQUE);
		SetBkColor	(s_hDC,_dwColor);

		putScriptText(xPos,yPos,_dwColor,s_strTempText);

		SetBkMode	(s_hDC,TRANSPARENT);
		m_isReverse	=	FALSE;

		GetTextExtentPoint32(s_hDC,s_strTempText,strlen(s_strTempText),&size);

		xPos	+=	size.cx;

		strcpy(s_strTempText,_strText+_iEnd+1);

		putScriptText(xPos,yPos,_dwColor,s_strTempText);
	}
	else	putScriptText(xPos,yPos,_dwColor,_strText);

	closeDevice();
}

//
//	반전찍기
void
CTextDC::putReverse(int xPos,int yPos,DWORD _dwColor,char *_strText,...)
{
	if (!_strText)
		return;

	if (m_bIsBlinkText)
		if ((timeGetTime()%(m_dwBlinkShowTime+m_dwBlinkHideTime))<m_dwBlinkHideTime)
			return;

	m_isReverse	=	TRUE;

	if (m_bIsMixText)
	{
		va_list		argptr;
		va_start(argptr, _strText);
		vsprintf(s_strTempText,_strText,argptr);
		va_end(argptr);
	}
	else
		strcpy(s_strTempText,_strText);

#ifdef	_SHARP_SHARP_TO_PERCENTEMARK
	OpertePercentageMark(s_strTempText);
#endif
#ifdef	_FOR_THAI
	OperteCRMark(s_strTempText);
#endif

	activeDevice();

	SetBkMode	(s_hDC,OPAQUE);
	SetBkColor	(s_hDC,_dwColor);

	putScriptText(xPos,yPos,_dwColor,s_strTempText);

	SetBkMode	(s_hDC,TRANSPARENT);

	closeDevice();
	
	m_isReverse	=	FALSE;
}

//
//	스크립트가 포함된 한줄짜리 문장을 출력
void
CTextDC::putScriptText(int _iX,int _iY,DWORD _dwColor,char *_strText)
{
	int		iTextPoint	=	0;
	int		iOldFont	=	m_iSelectFont;
	BOOL	isIncludeSpecialCode=FALSE,isOldReverse=m_isReverse;
	int		iTextLength	=	strlen(_strText);

	setTextColor(_dwColor);	//	컬러 설정

	if	(m_bIsDecodeScript	||	m_bIsWantPorcessReturnKeyByUser)
		if	(findSpecialCodePoint(_strText,strlen(_strText))!=-1)	
			isIncludeSpecialCode	=	TRUE;

//	각종 데이터 설정
	{
		m_pos.x			=	_iX;
		m_pos.y			=	_iY;
		m_dwTextColor	=	_dwColor;
	}	//	각종 데이터 설정

	while(_strText[iTextPoint])
	{
		int		iSpecialCodeProcess=0,iOutputTextLength,iSpecialCodePoint,iNextXPos=0;
		SIZE	size;

//	특수기호들을 처리한다.
		if (isIncludeSpecialCode)
			if ((iSpecialCodeProcess=operateSpecialCode(_strText,&iTextPoint))	==	dOPERATE_SPECIAL_CODE)
				continue;

		if (iTextPoint >= iTextLength)  break;

		iOutputTextLength	=	strlen(_strText+iTextPoint);

//	특수코드 검색
		if (isIncludeSpecialCode)
		{
			if	(iSpecialCodeProcess	==	0	||	iSpecialCodeProcess	==	dIS_INCORRECT_SPECIAL_CODE)
				iSpecialCodePoint	=	findSpecialCodePoint(_strText+iTextPoint,iOutputTextLength);

			if (iSpecialCodePoint	>	0)
			{
				iOutputTextLength	=	iSpecialCodePoint;

				GetTextExtentPoint32(s_hDC,_strText+iTextPoint,iOutputTextLength,&size);

				iNextXPos			=	m_pos.x+size.cx;
			}
		}	//	특수코드 검색

//	실제 출력
		{
			if (m_iSelectFont	==	dWINDOW_FONT_BOLD)	m_pos.y--;

			if (m_isShadow && !m_isReverse)
			{
				DWORD	dwColor	=	GetTextColor(s_hDC);

				SetTextColor(s_hDC,m_dwShadowColor);
				TextOut(s_hDC,m_pos.x+m_posShadowMargin.x,m_pos.y+m_posShadowMargin.y,_strText+iTextPoint,iOutputTextLength);
				SetTextColor(s_hDC,dwColor);
			}

			TextOut(s_hDC,m_pos.x,m_pos.y,_strText+iTextPoint,iOutputTextLength);

			if (m_iSelectFont	==	dWINDOW_FONT_BOLD)	m_pos.y++;
		}	//	실제 출력

//	상황에 따라 줄바꿈을 한다.
		m_pos.x		=	iNextXPos;
		iTextPoint	+=	iOutputTextLength;
	}

	m_isReverse	=	isOldReverse;
	m_iLineGap	=	m_iStackLineGap;

	selectFont(iOldFont);	//	원래 폰트로 돌림
}

//
//	문장안에 특수 코드 말고 일반 텍스트가 있나 찾는다.
BOOL
CTextDC::isOnlySpecialCodeText(char *_strText)
{
	int	iPoint	=	0;

	while(_strText[iPoint])
	{
		if	(Is2ByteChar(_strText[iPoint]))
			return	FALSE;

		while(_strText[iPoint]	==	' ')
			iPoint++;

		if	(!checkSpecialCode(_strText,&iPoint))
			return	FALSE;
	}

	return	TRUE;
}

//
//	스크립트가 포함된 문장을 사각형에 맞춰 그려준다.
void
CTextDC::drawScriptText(int _iX,int _iY,int _iWidth,int _iHeight,int _iBeginLine,DWORD _dwColor,char *_strText,BOOL _isPutWidthCenter)
{
	int			iTextPoint=0,iOldFont=m_iSelectFont,iFitWidth,iFitHeight,iCurrentLine;
	BOOL		isIncludeSpecialCode=FALSE,isOldReverse=m_isReverse;
	char		strOutputString[256];	//	출력할 문장
	CPos		pos;					//	출력 위치

	m_isPutWidthCenter	=	_isPutWidthCenter;

	setTextColor(_dwColor);			//	컬러 설정

//	박스 재설정(텍스트가 출력될..)
	{
		iFitWidth=	_iWidth;///m_iFontWidth*m_iFontWidth;	//	폰트 사이즈에 맞춘 실제 넓이
		_iX		+=	(_iWidth-iFitWidth)/2;	//	왼쪽도 맞춰준다.
		_iWidth	=	iFitWidth;

		iFitHeight=	_iHeight;///(m_iFontHeight+m_iLineGap)*(m_iFontHeight+m_iLineGap)-m_iLineGap;
		_iY		+=	(_iHeight-iFitHeight)/2;	//	왼쪽도 맞춰준다.
	}	//	박스 재설정(텍스트가 출력될..)

	if (m_bIsDecodeScript	||	m_bIsWantPorcessReturnKeyByUser)
		if	(findSpecialCodePoint(_strText,strlen(_strText))!=-1)
			isIncludeSpecialCode=TRUE;	//	문장 안에 특수코드-_-?

//	각종 데이터 설정
	{
		iCurrentLine	=	0;
		m_iBeginLine	=	_iBeginLine;

		pos.x			=	_iX;
		pos.y			=	_iY;
		m_dwTextColor	=	_dwColor;
	}	//	각종 데이터 설정

	BOOL	isOperatedReturnBySpecialCode	=	FALSE;

	while(_strText[iTextPoint])
	{
		int		iTextWidth		=	0;	//	문장의 길이
		DWORD	dwColor			=	GetTextColor(s_hDC);
		int		iNextPoint		=	getOneLine(_iWidth,_strText,iTextPoint,strOutputString,isIncludeSpecialCode,&iTextWidth);
		dwColor					=	GetTextColor(s_hDC);

		int		iAddCenterPoint	=	0;
		int		iTextLength		=	strlen(strOutputString);
		BOOL	isOperateReturn	=	FALSE;
		BOOL	isPut			=	FALSE;
		BOOL	bIsPuted		=	FALSE;

		if (iCurrentLine	>=	_iBeginLine)
			isPut	=	TRUE;

		if (iTextLength >= 1)
		{
			if (m_bIsWantPorcessReturnKeyByUser)
			{
				if (!Is2ByteChar(strOutputString,iTextLength-1) && !Is2ByteChar(strOutputString,iNextPoint))
					if (strOutputString[iTextLength-1] != '|' && _strText[iNextPoint]	==	'|')
						iNextPoint++;
			}

			if (strOutputString[iTextLength-1]	!=	0x0a)
			{
				if (_strText[iNextPoint]	==	0x0d)
					iNextPoint++;
				if (_strText[iNextPoint]	==	0x0a)
					iNextPoint++;
			}
		}
		else
			break;

		if	(_isPutWidthCenter)
			iAddCenterPoint	=	(_iWidth-iTextWidth)/2;

		int iOutputStringLength	=	strlen(strOutputString);

		if (iOutputStringLength	>=	1 && strOutputString[iOutputStringLength-1]	==	0x0d)
		{
			iOutputStringLength--;

			if(iOutputStringLength	>	0)	if (strOutputString[iOutputStringLength-1]	==	0x0a)	iOutputStringLength--;

			strOutputString[iOutputStringLength]	=	NULL;

			if (pos.x	==	_iX)
				if (isOnlySpecialCodeText(strOutputString))
					isOperateReturn	=	TRUE;
		}
		else	
		if (iOutputStringLength	>=	1 && strOutputString[iOutputStringLength-1]	==	0x0a)
		{
			iOutputStringLength--;

			strOutputString[iOutputStringLength]	=	NULL;

			if	(isOperatedReturnBySpecialCode)
				if	(pos.x	==	_iX)
					if	(isOnlySpecialCodeText(strOutputString))	
						isOperateReturn	=	TRUE;

			isOperatedReturnBySpecialCode			=	TRUE;
		}
		else
		if (m_bIsWantPorcessReturnKeyByUser	&& iOutputStringLength	>=	1 )
		{
			if (!Is2ByteChar(strOutputString,iOutputStringLength-1) && strOutputString[iOutputStringLength-1]	==	'|')
			{
				iOutputStringLength--;

				strOutputString[iOutputStringLength]	=	NULL;

				if (isOperatedReturnBySpecialCode)
					if (pos.x	==	_iX)
						if (isOnlySpecialCodeText(strOutputString))	isOperateReturn	=	TRUE;

				isOperatedReturnBySpecialCode			=	TRUE;
			}
		}
		else	
		{	//	캐리지 리턴이 아니고 중앙에 찍기가 아니고 마지막 단어가 아닌경우 보기좋게 맞춘다. -_-
			isOperatedReturnBySpecialCode			=	FALSE;

#ifndef	_USE_WORD_BREAK
			if (_iWidth	!=	iTextWidth	&&	_strText[iNextPoint] && !_isPutWidthCenter && m_isFitRightEdge)
			{
				int		iaSpacePoint[100],iWordCount,iDx=_iWidth-iTextWidth,iNextPoint=0;
				char	strOutputWord[256];		//	출력할 문장

				iWordCount		=	getWords(strOutputString,iaSpacePoint);

//	첫번째 단어는 보정없이 찍는다.
				{
					strcpy(strOutputWord,strOutputString);

					if (iaSpacePoint[0]>=iNextPoint)	strOutputWord[iaSpacePoint[0]-iNextPoint]	=	NULL;

					iNextPoint	=	iaSpacePoint[0];
					pos.x		=	putScriptLine(pos.x,pos.y,strOutputWord,isIncludeSpecialCode,isPut);
					iNextPoint	=	iaSpacePoint[0];
				}	//	첫번째 단어는 보정없이 찍는다.

				for (int i=1;i<iWordCount;i++)
				{
					int	iAddX	=	iDx	/ (iWordCount-1);

					if (i < (iDx % (iWordCount-1)))	iAddX++;

					strcpy(strOutputWord,strOutputString+iNextPoint);

					strOutputWord[iaSpacePoint[i]-iNextPoint]	=	NULL;
					iNextPoint	=	iaSpacePoint[i];

					pos.x		=	putScriptLine(pos.x+iAddX,pos.y,strOutputWord,isIncludeSpecialCode,isPut);
				}

				bIsPuted			=	TRUE;
//				iOutputStringLength	=	0;
			}
#endif
		}

		if	(iOutputStringLength > 0 && !bIsPuted)
		{
			int	iReturnPos	=	putScriptLine(pos.x+iAddCenterPoint,pos.y,strOutputString,isIncludeSpecialCode,isPut);

			if (iReturnPos	>	pos.x+iAddCenterPoint)
			{
				if (iCurrentLine	>=	_iBeginLine)	pos.y	+=	m_iFontHeight+m_iLineGap;		//	줄바꿈 한다.
				pos.x	=	_iX;
			}

			if	(isOperateReturn)
				pos.y	+=	m_iValidLineHeight;	//	줄바꿈 한다.
		}
		else
		{
			if (iCurrentLine>=_iBeginLine)
			{
				if	(bIsPuted)
					pos.y	+=	m_iFontHeight+m_iLineGap;
				else
					pos.y	+=	m_iValidLineHeight;	//	줄바꾸기 문자가 왔다.
			}

			pos.x	=	_iX;
		}

		if (pos.y-_iY+m_iFontHeight	>	_iHeight)	break;

		iCurrentLine++;
		iTextPoint		=	iNextPoint;
	}

	m_isReverse			=	isOldReverse;
	m_isPutWidthCenter	=	FALSE;
	m_iLineGap			=	m_iStackLineGap;

	selectFont(iOldFont);	//	원래 폰트로 돌림
}

//
//	스크립트가 포함된 문장을 사각형에 맞춰 그려준다.
int
CTextDC::getTextHeight(int _iWidth,char *_strText)
{
	int			iTextPoint=0,iOldFont=m_iSelectFont;
	BOOL		isIncludeSpecialCode=FALSE,isOldReverse=m_isReverse;
	char		strOutputString[256];	//	출력할 문장
	CPos		pos;					//	출력 위치

	activeTemplateDevice();

	if (m_bIsDecodeScript	||	m_bIsWantPorcessReturnKeyByUser)
		if	(findSpecialCodePoint(_strText,strlen(_strText))!=-1)
			isIncludeSpecialCode=TRUE;	//	문장 안에 특수코드-_-?

//	각종 데이터 설정
	{
		pos.x			=	0;
		pos.y			=	0;
	}	//	각종 데이터 설정

	BOOL	isOperatedReturnBySpecialCode	=	FALSE;

	while(_strText[iTextPoint])
	{
		int		iTextWidth		=	0;	//	문장의 길이
		int		iNextPoint		=	getOneLine(_iWidth,_strText,iTextPoint,strOutputString,isIncludeSpecialCode,&iTextWidth);
		int		iAddCenterPoint	=	0;
		int		iTextLength		=	strlen(strOutputString);
		BOOL	isOperateReturn	=	FALSE;
		BOOL	isPut			=	FALSE;
		BOOL	bIsPuted		=	FALSE;

		if (iTextLength >= 1)
		{
			if (m_bIsWantPorcessReturnKeyByUser)
				if (strOutputString[iTextLength-1]	!=	'|')
					if (_strText[iNextPoint]	==	'|')
						iNextPoint++;

			if (strOutputString[iTextLength-1]	!=	0x0a)
			{
				if (_strText[iNextPoint]	==	0x0d)
					iNextPoint++;
				if (_strText[iNextPoint]	==	0x0a)
					iNextPoint++;
			}
		}
		else
			break;

		int iOutputStringLength	=	strlen(strOutputString);

		if (iOutputStringLength >= 1 && strOutputString[iOutputStringLength-1]	==	0x0d)
		{
			iOutputStringLength--;

			if(iOutputStringLength	>	0)	if (strOutputString[iOutputStringLength-1]	==	0x0a)	iOutputStringLength--;

			strOutputString[iOutputStringLength]	=	NULL;

			if (pos.x	==	0)
				if (isOnlySpecialCodeText(strOutputString))
					isOperateReturn	=	TRUE;
		}
		else
		if (iOutputStringLength >= 1 && strOutputString[iOutputStringLength-1]	==	0x0a)
		{
			iOutputStringLength--;

			strOutputString[iOutputStringLength]	=	NULL;

			if (isOperatedReturnBySpecialCode)
				if (pos.x	==	0)
					if (isOnlySpecialCodeText(strOutputString))
						isOperateReturn	=	TRUE;

			isOperatedReturnBySpecialCode			=	TRUE;
		}
		else
		if (m_bIsWantPorcessReturnKeyByUser && iOutputStringLength >= 1 && strOutputString[iOutputStringLength-1]	==	'|')
		{
			iOutputStringLength--;

			strOutputString[iOutputStringLength]	=	NULL;

			if (isOperatedReturnBySpecialCode)
				if (pos.x	==	0)
					if (isOnlySpecialCodeText(strOutputString))
						isOperateReturn	=	TRUE;

			isOperatedReturnBySpecialCode			=	TRUE;
		}
		else
		{	//	캐리지 리턴이 아니고 중앙에 찍기가 아니고 마지막 단어가 아닌경우 보기좋게 맞춘다. -_-
			isOperatedReturnBySpecialCode			=	FALSE;

#ifndef	_USE_WORD_BREAK
			if (_iWidth	!=	iTextWidth	&&	_strText[iNextPoint] && m_isFitRightEdge)
			{
				int		iaSpacePoint[100],iWordCount,iDx=_iWidth-iTextWidth,iNextPoint=0;
				char	strOutputWord[256];		//	출력할 문장

				iWordCount		=	getWords(strOutputString,iaSpacePoint);

//	첫번째 단어는 보정없이 찍는다.
				{
					strcpy(strOutputWord,strOutputString);

					strOutputWord[iaSpacePoint[0]-iNextPoint]	=	NULL;
					iNextPoint	=	iaSpacePoint[0];
					pos.x		=	putScriptLine(pos.x,pos.y,strOutputWord,isIncludeSpecialCode,isPut);
					iNextPoint	=	iaSpacePoint[0];
				}	//	첫번째 단어는 보정없이 찍는다.

				for (int i=1;i<iWordCount;i++)
				{
					int	iAddX	=	iDx	/ (iWordCount-1);

					if (i < (iDx % (iWordCount-1)))	iAddX++;

					strcpy(strOutputWord,strOutputString+iNextPoint);

					strOutputWord[iaSpacePoint[i]-iNextPoint]	=	NULL;
					iNextPoint	=	iaSpacePoint[i];

					pos.x		=	putScriptLine(pos.x+iAddX,pos.y,strOutputWord,isIncludeSpecialCode,isPut);
				}

				iOutputStringLength	=	0;
				bIsPuted			=	TRUE;
			}
#endif
		}

		if (iOutputStringLength	>	0)
		{
			int	iReturnPos	=	putScriptLine(pos.x+iAddCenterPoint,pos.y,strOutputString,isIncludeSpecialCode,isPut);

			if (iReturnPos	>	pos.x+iAddCenterPoint)
			{
				pos.y	+=	m_iFontHeight+m_iLineGap;		//	줄바꿈 한다.
				pos.x	=	0;
			}

			if	(isOperateReturn)
				pos.y	+=	m_iValidLineHeight;		//	줄바꿈 한다.
		}
		else
		{
			if	(bIsPuted)
				pos.y	+=	m_iFontHeight+m_iLineGap;
			else
				pos.y	+=	m_iValidLineHeight;	//	줄바꾸기 문자가 왔다.

			pos.x	=	0;
		}


		iTextPoint		=	iNextPoint;
	}

	m_isReverse			=	isOldReverse;
	m_isPutWidthCenter	=	FALSE;
	m_iLineGap			=	m_iStackLineGap;

	closeTemplateDevice();

	return	pos.y-m_iLineGap;
}

//
//	스크립트가 포함된 한줄짜리 문장을 출력
int
CTextDC::putScriptLine(int _iX,int _iY,char *_strText,BOOL _isIncludeSpecialCode,BOOL _isOutput)
{
	int		iTextPoint	=	0;
	int		iTextLength	=	strlen(_strText);

//	각종 데이터 설정
	{
		m_pos.x			=	_iX;
		m_pos.y			=	_iY;
	}	//	각종 데이터 설정

	while(_strText[iTextPoint])
	{
		int		iSpecialCodeProcess=0,iOutputTextLength,iSpecialCodePoint,iNextXPos=0;
		SIZE	size;

//	특수기호들을 처리한다.
		if (_isIncludeSpecialCode)
			if ((iSpecialCodeProcess=operateSpecialCode(_strText,&iTextPoint))	==	dOPERATE_SPECIAL_CODE)
				continue;

		if (iTextPoint >= iTextLength)  break;

		iOutputTextLength	=	strlen(_strText+iTextPoint);

//	특수코드 검색
		if (_isIncludeSpecialCode)
		{
			if (iSpecialCodeProcess	==	0	||	iSpecialCodeProcess	==	dIS_INCORRECT_SPECIAL_CODE)
				iSpecialCodePoint	=	findSpecialCodePoint(_strText+iTextPoint,iOutputTextLength);

			if (iSpecialCodePoint	>	0)
			{
				iOutputTextLength	=	iSpecialCodePoint;

				GetTextExtentPoint32(s_hDC,_strText+iTextPoint,iOutputTextLength,&size);

				iNextXPos			=	m_pos.x+size.cx;
			}
		}	//	특수코드 검색

		if (m_iSelectFont	==	dWINDOW_FONT_BOLD)	m_pos.y--;

		if (m_isShadow && !m_isReverse)
		{
			if (_isOutput)
			{
				DWORD	dwColor	=	GetTextColor(s_hDC);

				SetTextColor(s_hDC,m_dwShadowColor);
				TextOut(s_hDC,m_pos.x+m_posShadowMargin.x,m_pos.y+m_posShadowMargin.y,_strText+iTextPoint,iOutputTextLength);
				SetTextColor(s_hDC,dwColor);
			}
		}

		if (_isOutput)	TextOut(s_hDC,m_pos.x,m_pos.y,_strText+iTextPoint,iOutputTextLength);

		if (m_iSelectFont	==	dWINDOW_FONT_BOLD)	m_pos.y++;

		if (iNextXPos	==	0)
		{
			GetTextExtentPoint32(s_hDC,_strText+iTextPoint,iOutputTextLength,&size);
			iNextXPos	=	m_pos.x+size.cx;
		}

		m_pos.x		=	iNextXPos;
		iTextPoint	+=	iOutputTextLength;
	}

	return	m_pos.x;
}

//
//	스크립트가 포함된 문장을 출력
int
CTextDC::getOneLine(int _iWidth,char *_strText,int _iTextPoint,char *_strReturnText,BOOL _isIncludeSpecialCode,int *_iTextWidth)
{
	int				iTextPoint		=	_iTextPoint,iBegin=_iTextPoint,iEnd=_iTextPoint;
	int				iOldFont		=	m_iSelectFont,iOutputTextLength;
	BOOL			isNewLine		=	FALSE;
	SIZE			size;
	int				iTextLength		=	strlen(_strText);
	CTextDCFontInfo	orgFondInfo;
	int				iSpaceCount		=	0,iReturnTextLength;
	char			strSpace[128];

	memcpy(&orgFondInfo,&m_fontInfo,sizeof(m_fontInfo));

	activeTemplateDevice();

	DWORD			dwColor			=	GetTextColor(s_hDC);

	m_pos.x	=	0;

	if (_iTextPoint	==	0)	isNewLine	=	TRUE;
	else
	{
		if (_iTextPoint >= 1)
		{
			if (_strText[_iTextPoint-1]	==	0x0d || _strText[_iTextPoint-1]	==	0x0a)	
				isNewLine	=	TRUE;

			if (m_bIsWantPorcessReturnKeyByUser)
				if (Is2ByteChar(_strText,_iTextPoint-1)==FALSE	&&	_strText[_iTextPoint-1]	==	'|')
					isNewLine	=	TRUE;
		}
	}

	while(_strText[iTextPoint])
	{
		int		iDx=_iWidth-m_pos.x,iSpecialCodeProcess=0,iNextXPos=0,iSpecialCodePoint;

		if	(m_pos.x	==	0	&&	!isNewLine)
			if	(removeFrontSpace(_strText,&iTextPoint))
				iBegin	=	iTextPoint;		//	라인의 처음에 나오는 공백 제거

		if	(iTextPoint >= iTextLength)
			break;

//	특수기호들을 처리한다.
		if	(_isIncludeSpecialCode)
		{
			iSpecialCodeProcess		=	checkSpecialCodeForOneLine(_strText,&iTextPoint);

			if	(_strText[iTextPoint] == NULL)
				break;

			if	(iSpecialCodeProcess	==	dOPERATE_SPECIAL_CODE)
				continue;
			if	(iSpecialCodeProcess	==	dOPERATE_ENTER_CODE)
			{
				iSpaceCount			=	0;
				iReturnTextLength	=	iTextPoint-iBegin;

				strncpy(_strReturnText,_strText+iBegin,iReturnTextLength);

				if (iReturnTextLength	>	2)
				{
					if (_strReturnText[iReturnTextLength-2]	==	0x0d)
					{
						_strReturnText[iReturnTextLength-2]	=	0x0a;
						iReturnTextLength--;
					}

					if (m_bIsWantPorcessReturnKeyByUser)
					{
						if (Is2ByteChar(_strReturnText,iReturnTextLength-2)==FALSE	&&	_strReturnText[iReturnTextLength-2]	==	'|')
						{
							_strReturnText[iReturnTextLength-2]	=	0x0a;
							iReturnTextLength--;
						}
					}

					while(_strReturnText[iReturnTextLength-2]	==	' ')
					{
						_strReturnText[iReturnTextLength-2]	=	0x0a;
						iReturnTextLength--;
						strSpace[iSpaceCount++]	=	' ';
					}
				}

				_strReturnText[iReturnTextLength]	=	NULL;

				if (iSpaceCount)
				{
					strSpace[iSpaceCount]	=	NULL;

					GetTextExtentPoint32(s_hDC,strSpace,iSpaceCount,&size);

					m_pos.x	-=	size.cx;
				}

				*_iTextWidth	=	m_pos.x;

				goto	return_label;
			}
		}

		int	iLength			=	strlen(_strText+iTextPoint);

		getTextExtentExPoint(s_hDC,_strText+iTextPoint,iLength,iDx,&iOutputTextLength,&size);

		if	(iOutputTextLength	<=	0)
		{
			break;
			getTextExtentExPoint(s_hDC,_strText+iTextPoint,iLength,_iWidth,&iOutputTextLength,&size);

			if	(iOutputTextLength	==	0)
				break;
			
			m_pos.x		=	0;
			isNewLine	=	FALSE;
			continue;
		}

//	줄바꾸면서 단어 조각 나는거 보완
		int	iPoint			=	getNotCrackeWordPoint((BYTE *)_strText,iTextPoint+iOutputTextLength);

		if	(iOutputTextLength	>	iPoint)
			iOutputTextLength	-=	iPoint;

//	특수코드 검색
		if (_isIncludeSpecialCode)
		{
			iSpecialCodePoint	=	-1;

			if	(iSpecialCodeProcess	==	0	||	iSpecialCodeProcess	==	dIS_INCORRECT_SPECIAL_CODE)
				iSpecialCodePoint		=	findSpecialCodePoint(_strText+iTextPoint,iOutputTextLength);

			if	(iSpecialCodePoint	>	-1)
			{
				iOutputTextLength	=	iSpecialCodePoint;

				GetTextExtentPoint32(s_hDC,_strText+iTextPoint,iOutputTextLength,&size);

				iNextXPos			=	m_pos.x+size.cx;
			}
			else
			{
				if	(m_bIsDecodeScript && _strText[iTextPoint+iOutputTextLength]	==	'<' && iOutputTextLength)
				{
					GetTextExtentPoint32(s_hDC,_strText+iTextPoint,iOutputTextLength,&size);

					iNextXPos			=	m_pos.x+size.cx;
				}
			}
		}	//	특수코드 검색

//	실제 출력
		iTextPoint		+=	iOutputTextLength;

//	상황에 따라 줄바꿈을 한다.
		if	(iNextXPos	==	0)
			break;

		m_pos.x		=	iNextXPos;
	}

	iSpaceCount			=	0;
	iReturnTextLength	=	iTextPoint-iBegin;

	if (iReturnTextLength	>=	1)
	{
		strncpy(_strReturnText,_strText+iBegin,iReturnTextLength);

		_strReturnText[iReturnTextLength]	=	NULL;

		while(_strReturnText[iReturnTextLength-1]	==	' ')
		{
			_strReturnText[iReturnTextLength-1]	=	NULL;
			strSpace[iSpaceCount++]				=	' ';

			iReturnTextLength--;
			iOutputTextLength--;
		}

		*_iTextWidth		=	putScriptLine(0,0,_strReturnText,m_bIsDecodeScript+m_bIsWantPorcessReturnKeyByUser,FALSE);
	}
	else
	{
		_strReturnText[0]	=	NULL;
		*_iTextWidth		=	m_pos.x;
	}

return_label:

	closeTemplateDevice();

	if	(s_bIsDeviceActive)
	{
		if (memcmp(&orgFondInfo,&m_fontInfo,sizeof(m_fontInfo))!=0)
			selectFont(&orgFondInfo);
		
		setTextColor(dwColor);
	}

	return	iTextPoint;
}

//
//	_strText를 단어 단위로 자른다.
int
CTextDC::getWords(char *_strText,int *_ilpPoint)
{
	int		iTextPoint	=	0;
	int		iSpaceCount	=	0;
	int		iTextLength	=	strlen(_strText);
	BOOL	isFront		=	TRUE;

	while(_strText[iTextPoint])
	{
		checkSpecialCode(_strText,&iTextPoint);

		if	(iTextPoint >= iTextLength) 
			break;

		if (_strText[iTextPoint]	==	' ')
		{
			while(_strText[iTextPoint]	==	' ')
				iTextPoint++;

			if (!isFront)
			{
				_ilpPoint[iSpaceCount]	=	iTextPoint;

				iSpaceCount++;
			}
		}
		else
		{
			isFront	=	FALSE;

			iTextPoint++;
		}
	}

	_ilpPoint[iSpaceCount++]	=	iTextLength;

	return	iSpaceCount;
}

//
//	어쩌구 저쩌구해서 얻기
void
CTextDC::getTextExtentExPoint(HDC _hDC,char *_lpstrText,int _iStrlen,int _iDx,int *_lpiOutputTextLength,SIZE *_lpSize)
{
	mBeginTLog();

	int		i;
	BOOL	bIsHangul	=	FALSE;
	BOOL	bResult		=	GetTextExtentExPoint(_hDC,_lpstrText,_iStrlen,_iDx,_lpiOutputTextLength,NULL,_lpSize);

	if	(bResult	==	FALSE)
	{
		*_lpiOutputTextLength	=	0;

		return;
	}

	for (i=0;i<*_lpiOutputTextLength;i++)
	{
		if	(bIsHangul			)
			bIsHangul	=	FALSE;
		else
		if	(Is2ByteChar(_lpstrText[i]))
			bIsHangul	=	TRUE;
	}

	if	(bIsHangul)
	{
		if	(*_lpiOutputTextLength	>=	2)
			(*_lpiOutputTextLength)--;
		else
			(*_lpiOutputTextLength)++;
	}
}

//
//	어쩌구 저쩌구해서 얻기
void
CTextDC::getTextExtentPoint32(HDC _hDC,char *_lpstrText,int *_lpiOutputTextLength,SIZE *_lpSize)
{
	int		i;
	BOOL	bIsHangul	=	FALSE;

	GetTextExtentPoint32(_hDC,_lpstrText,*_lpiOutputTextLength,_lpSize);

	for (i=0;i<*_lpiOutputTextLength;i++)
	{
		if (bIsHangul			)	
			bIsHangul	=	FALSE;
		else
		if (Is2ByteChar(_lpstrText[i]))
			bIsHangul	=	TRUE;
	}

	if (bIsHangul)
	{
		if (*_lpiOutputTextLength	>=	2)
			(*_lpiOutputTextLength)--;
		else
			(*_lpiOutputTextLength)++;
	}
}

//
//	이 문장이 몇 줄로 되었냐?
int
CTextDC::getLineCount(int _iWidth,char *_strText,BOOL _isActiveDevice,BOOL _isPutWidthCenter)
{
	int		iOldFont=m_iSelectFont,iTextPoint=0;
	BOOL	isIncludeSpecialCode=FALSE,isNewLine=TRUE;
	BOOL	isPutWidthCenter=m_isPutWidthCenter;
	int		iTextLength	=	strlen(_strText);

	m_isPutWidthCenter	=	_isPutWidthCenter;

	if (m_bIsDecodeScript	||	m_bIsWantPorcessReturnKeyByUser)
		if	(findSpecialCodePoint(_strText,strlen(_strText))!=-1)
			isIncludeSpecialCode	=	TRUE;

	activeTemplateDevice();

//	각종 데이터 설정
	{
		m_iLineCount	=	0;
		m_basePos.x		=	0;
		m_basePos.y		=	0;
		m_pos.x			=	m_basePos.x;
		m_pos.y			=	m_basePos.y;
	}	//	각종 데이터 설정

	BOOL	bIsCR_ByTextOver	=	FALSE;	//	텍스트가 꽉차서 줄 바꿈했다.(이렇게 줄 바꾸자 마자 \n을 만났을때 \n을 무시하기 위해)

	while(_strText[iTextPoint])
	{
		int		iSpecialCodeProcess	=	0;
		int		iDx	=	_iWidth-(m_pos.x-m_basePos.x);
		int		iOutputTextLength;
		SIZE	size;
		int		iSpecialCodePoint,iNextXPos=0;

		if	(m_pos.x	==	m_basePos.x	&&	!isNewLine)
			removeFrontSpace(_strText,&iTextPoint);	//	라인의 처음에 나오는 공백 제거

		if (iTextPoint >= iTextLength) 
			break;

//	특수기호들을 처리한다.
		if (isIncludeSpecialCode)
		{
			iSpecialCodeProcess	=	checkSpecialCodeForGetLineCount(_strText,&iTextPoint);

			if	(iTextPoint >= iTextLength)
			{
				m_iLineCount++;
				break;
			}

			if	(iSpecialCodeProcess	==	dOPERATE_SPECIAL_CODE)
			{
				if	(iTextPoint >= iTextLength	&&	isNewLine)
					m_iLineCount++;
				continue;
			}
			if	(iSpecialCodeProcess	==	dOPERATE_ENTER_CODE)
			{	
				if	(bIsCR_ByTextOver)
				{
					m_iLineCount--;
					bIsCR_ByTextOver	=	FALSE;
				}

				isNewLine	=	TRUE;
				continue;
			}
		}	//	특수기호들을 처리한다.

		bIsCR_ByTextOver	=	FALSE;

		getTextExtentExPoint(s_hDC,_strText+iTextPoint,strlen(_strText+iTextPoint),iDx,&iOutputTextLength,&size);

		if	(iOutputTextLength	<=	0)
		{
			getTextExtentExPoint(s_hDC,_strText+iTextPoint,strlen(_strText+iTextPoint),_iWidth,&iOutputTextLength,&size);

			if	(iOutputTextLength	==	0)
				break;

			m_pos.y		+=	m_iFontHeight+m_iLineGap;
			m_pos.x		=	m_basePos.x;
			m_iLineCount++;
			isNewLine	=	FALSE;
			continue;
		}

//	줄바꾸면서 단어 조각 나는거 보완
		int	iPoint			=	getNotCrackeWordPoint((BYTE *)_strText,iTextPoint+iOutputTextLength);

		if	(iOutputTextLength	>	iPoint)
			iOutputTextLength	-=	iPoint;

//	특수코드 검색
		if	(isIncludeSpecialCode)
		{
			iSpecialCodePoint	=	-1;

			if	(iSpecialCodeProcess	==	0	||	iSpecialCodeProcess	==	dIS_INCORRECT_SPECIAL_CODE)
				iSpecialCodePoint		=	findSpecialCodePoint(_strText+iTextPoint,iOutputTextLength);

			if	(iSpecialCodePoint	>	-1)
			{
				iOutputTextLength	=	iSpecialCodePoint;

				GetTextExtentPoint32(s_hDC,_strText+iTextPoint,iOutputTextLength,&size);

				iNextXPos			=	m_pos.x+size.cx;
			}
			else
			if	(m_bIsDecodeScript && _strText[iTextPoint+iOutputTextLength]	==	'<' && iOutputTextLength)
			{
				GetTextExtentPoint32(s_hDC,_strText+iTextPoint,iOutputTextLength,&size);

				iNextXPos			=	m_pos.x+size.cx;
			}
		}	//	특수코드 검색

//	상황에 따라 줄바꿈을 한다.
		if	(iNextXPos	==	0)
		{
			m_pos.y		+=	m_iFontHeight+m_iLineGap;
			m_pos.x		=	m_basePos.x;
			m_iLineCount++;
			isNewLine	=	FALSE;
			bIsCR_ByTextOver=	TRUE;
		}	//	상황에 따라 줄바꿈을 한다.
		else
			m_pos.x	=	iNextXPos;

		iTextPoint	+=	iOutputTextLength;
	}

	m_isPutWidthCenter	=	isPutWidthCenter;

//	디바이스 해제

	closeTemplateDevice();

	if (s_bIsDeviceActive)
		if (iOldFont	!=	m_iSelectFont)
			if (selectFont(iOldFont))	
				SelectObject(s_hDC, getFont());

	return	m_iLineCount;
}

//
//	이 문장의 픽셸 사이즈
int
CTextDC::getPixelSize(char *_strText)
{
	activeTemplateDevice();

	int	iSize	=	putScriptLine(0,0,_strText,m_bIsDecodeScript+m_bIsWantPorcessReturnKeyByUser,FALSE);

	closeTemplateDevice();

	return	iSize;
//	return	m_pos.x;
}

//
//	이 문장에서 가장 긴 라인의 픽셸 사이즈
int
CTextDC::getMaxLineSize(char *_strText)
{
	activeTemplateDevice();

	char	strString[1024];
	int		iTextPoint=0,iPoint=0;
	int		iMaxSize	=	0,iSize;

	while(_strText[iTextPoint])
	{
		strString[iPoint]	=	_strText[iTextPoint];

		if (Is2ByteChar(strString[iPoint]))
		{
			strString[iPoint+1]	=	_strText[iTextPoint+1];
			iPoint		+=	2;
			iTextPoint	+=	2;

			continue;
		}

		if (_strText[iTextPoint]==0x0d)
		{
			if (_strText[iTextPoint]==0x0a)	
				iTextPoint++;

			if (iPoint	>	0)
			{
				strString[iPoint]	=	NULL;
				iSize	=	getPixelSize(strString);
				iMaxSize=	max(iSize,iMaxSize);
				iPoint	=	0;
			}
		}
		else
		if (_strText[iTextPoint]==0x0a)
		{
			if (iPoint	>	0)
			{
				strString[iPoint]	=	NULL;
				iSize	=	getPixelSize(strString);
				iMaxSize=	max(iSize,iMaxSize);
				iPoint	=	0;
			}
		}
		else
		if (m_bIsWantPorcessReturnKeyByUser && _strText[iTextPoint]=='|')
		{
			if (iPoint	>	0)
			{
				strString[iPoint]	=	NULL;
				iSize	=	getPixelSize(strString);
				iMaxSize=	max(iSize,iMaxSize);
				iPoint	=	0;
			}
		}
		else	iPoint++;

		iTextPoint++;
	}

	if (iPoint	>	0)
	{
		strString[iPoint]	=	NULL;
		iSize				=	getPixelSize(strString);
		iMaxSize			=	max(iSize,iMaxSize);
	}

	closeTemplateDevice();

	return	iMaxSize;
}

//
//	_lpString의 _iRange범위 안에서 특수 코드가 있나 검색한다.
int
CTextDC::findSpecialCodePoint(char *_lpString,int _iRange)
{
	int		iPoint	=	0;

	if	(!m_bIsDecodeScript	&&	m_bIsWantPorcessReturnKeyByUser)
	{
		while(_lpString[iPoint])
		{
			if	(Is2ByteChar(_lpString[iPoint]))
				iPoint++;
			else
			{
				if	(_lpString[iPoint]	==	0x0d	||	_lpString[iPoint]	==	0x0a)
					return	iPoint;
				if	(_lpString[iPoint]	==	'|')
					return	iPoint;

			}

			iPoint++;

			if	(iPoint	>=	_iRange)
				return	-1;
		}

		return	-1;
	}

	if	(m_bIsIgnoreSomeCode)
	{
		while(_lpString[iPoint])
		{
			if (Is2ByteChar(_lpString[iPoint]))
				iPoint++;
			else
			{
				if	(_lpString[iPoint]	==	0x0d	||	_lpString[iPoint]	==	0x0a)
					return	iPoint;
				if	(m_bIsWantPorcessReturnKeyByUser	&&	_lpString[iPoint]	==	'|')
					return	iPoint;

				if (_lpString[iPoint]	==	'<')
				{
					if (_lpString[iPoint+1]	==	'c')
					{
						if (_lpString[iPoint+2]	==	':')
						{
							if	(checkIsCorrectTextColorScript(_lpString+iPoint+3)	==	TRUE)
								return	iPoint;
						}
					}
					else
					if ( _lpString[iPoint+1]	==	'n')
					{
						if (_lpString[iPoint+2]	==	'>')
						{
							m_dwLastSettingColor	=	m_dwTextColor;
							return	iPoint;
						}
					}
				}
			}

			iPoint++;

			if (iPoint	>=	_iRange)
				return	-1;
		}

		return	-1;
	}

	for(;iPoint<_iRange && _lpString[iPoint];iPoint++)
	{
		if (Is2ByteChar(_lpString[iPoint]))
		{
			iPoint++;
			continue;
		}

		if	(_lpString[iPoint]	==	0x0d	||	_lpString[iPoint]	==	0x0a)
			return	iPoint;
		if	(m_bIsWantPorcessReturnKeyByUser	&&	_lpString[iPoint]	==	'|'	)
			return	iPoint;

		if (_lpString[iPoint]	!=	'<')
			continue;

		if	(_lpString[iPoint+1]	==	's')
		{
			if	(_lpString[iPoint+2]	==	'>')
				return	iPoint;
			if	(_lpString[iPoint+2]	!=	':')
				continue;
			if	(_lpString[iPoint+3]	<	'7'	||	_lpString[iPoint+3]	>	'9'	)
				continue;
			if	(_lpString[iPoint+4]	==	'>')
				return	iPoint;
			if	(_lpString[iPoint+4]	<	'0'	||	_lpString[iPoint+4]	>	'9'	)
				continue;
			if	(_lpString[iPoint+5]	==	'>')
				return	iPoint;

			continue;
		}
		else
		if	(_lpString[iPoint+1]	==	'c')
		{
			if (_lpString[iPoint+2]	==	':')
			{
				if	(checkIsCorrectTextColorScript(_lpString+iPoint+3)	==	TRUE)
					return	iPoint;
			}
		}
		else
		if	(_lpString[iPoint+1]	==	'l')
		{
			if (_lpString[iPoint+2]	==	'>')	return	iPoint;
			if (_lpString[iPoint+2]	==	':')	return	iPoint;
		}
		else
		if	(_lpString[iPoint+1]	==	'n')
		{
			if (_lpString[iPoint+2]	==	'>')
			{
				m_dwLastSettingColor	=	m_dwTextColor;

				return	iPoint;
			}
		}
		else
		if	(_lpString[iPoint+1]	==	'r'|| _lpString[iPoint+1]	==	'b')
		{
			if (_lpString[iPoint+2]	==	'>')
				return	iPoint;
		}
	}

	return	-1;
}

//
//	요기부분이 단어가 짤리는 부분이냐?
BOOL
CTextDC::getNotCrackeWordPoint(BYTE *_lpString,int _iPoint)
{
	BYTE	ch			=	_lpString[_iPoint];
	char	cChar		=	(char)_lpString[_iPoint];
	int		iRefitPoint	=	1;


//	가로 중앙에찍을때는 한글까지 단어단위로 자른다.
	if(m_isPutWidthCenter)
	{

		if ( ch == '.' || (ch>='a' && ch<='z') || (ch >= 'A' && ch <= 'Z')|| (ch >= '0' && ch <= '9')	|| Is2ByteChar(cChar) )
		{
			ch	=	_lpString[_iPoint-iRefitPoint];

			if ( ch == '.' || (ch>='a' && ch<='z') || (ch >= 'A' && ch <= 'Z')|| (ch >= '0' && ch <= '9')	|| Is2ByteChar(cChar))
			{
				iRefitPoint++;

				while(1)
				{
					ch	=	_lpString[_iPoint-iRefitPoint];

					if ( ch == '.' || (ch>='a' && ch<='z') || (ch >= 'A' && ch <= 'Z')|| (ch >= '0' && ch <= '9')	|| Is2ByteChar(cChar))
					{
						iRefitPoint++;
						if (iRefitPoint	>=	_iPoint)	return	FALSE;

					}
					else	return	iRefitPoint-1;
				}
			}
		}

		return	0;
	}

	if (!Is2ByteChar(cChar))
	{

		if ( ch == '.' || (ch>='a' && ch<='z') || (ch >= 'A' && ch <= 'Z')|| (ch >= '0' && ch <= '9')	)
		{
			ch	=	_lpString[_iPoint-iRefitPoint];

			int	iPrePoint	=	_iPoint-iRefitPoint-1;

			if (iPrePoint	>=	0)
				if (Is2ByteChar(_lpString[iPrePoint]))
					return	0;

			if ( ch == '.' || (ch>='a' && ch<='z') || (ch >= 'A' && ch <= 'Z')|| (ch >= '0' && ch <= '9')	)
			{
				iRefitPoint++;

				while(1)
				{
					ch	=	_lpString[_iPoint-iRefitPoint];

					if ( ch == '.' || (ch>='a' && ch<='z') || (ch >= 'A' && ch <= 'Z')|| (ch >= '0' && ch <= '9')	)
					{
						iRefitPoint++;
						if (iRefitPoint	>=	_iPoint)	return	FALSE;

					}
					else	return	iRefitPoint-1;
				}
			}
		}
	}

	return	0;
}

//
//	컬러 숫자 얻어오기.
int
CTextDC::getColor(char *_strText,int *_iTextPoint)
{
	int		iPoint,iStringLength,count=0,ch=0;
	char	strNumber[32];

	iPoint			=	*_iTextPoint;
	iStringLength	=	strlen(_strText);

	while(1) // 'A'~'Z'가 아닌 문자를 만날때 까지..
	{
		ch	=	_strText[iPoint++];

		if	(ch<'A' || ch>'Z')
			break;

		strNumber[count++]=ch;

		if	(iPoint	>= iStringLength)
			return -1;
		if	(count	>= 32			)
			return -1;
	}

	if	(count < 3)
		return	-1;

	strNumber[count]=	NULL;
	count			=	0;

	while(1)
	{
		if	(s_aTextColor[count].m_dwColor	==	0xffffffff)
			return	-1;

		if	(strcmp(s_aTextColor[count].m_strColor,strNumber)==0)
			break;

		count++;
	}
		
	*_iTextPoint	=	iPoint;

	return s_aTextColor[count].m_dwColor;
}

//
//	컬러 숫자 얻어오기.
int
CTextDC::getColorNumber(char *_strText,int *_iTextPoint)
{
	int		count=0,ch=0,iPoint,iStringLength;
	char	strNumber[4];

	iPoint			=	*_iTextPoint;
	iStringLength	=	strlen(_strText);

	while(1) // 숫자가 아닌 문자를 만날때 까지..
	{
		ch	=	_strText[iPoint++];

		if	(ch<'0' || ch>'9')
			break;

		strNumber[count++]	=	ch;

		if	(iPoint	>= iStringLength)
			return -1;
		if	(count	> 3				)
			return -1;
	}

	strNumber[count]=	NULL;
	*_iTextPoint	=	iPoint;

	return (atoi(strNumber));
}

//
//	줄바꿈 후에 줄의 첫번째 문자에 오는 공백 제거
inline	BOOL
CTextDC::removeFrontSpace(char *_strText,int *_iPoint)
{
	int	iPoint	=	*_iPoint;

	if(_strText[iPoint]	==	' ')	//	공백이다
		if (iPoint	!=	0)	//	문장의 처음이 아니다.
		{
			while(_strText[iPoint]	==	' ')	iPoint++;

			*_iPoint	=	iPoint;

			return	TRUE;
		}

	return	FALSE;
}	//	줄바꿈 후에 줄의 첫번째 문자에 오는 공백 제거

BOOL
CTextDC::checkIsCorrectTextColorScript(char *_strText)
{
	int	iPoint	=	0;
	
	if	(_strText[iPoint]	==	NULL)
		return	FALSE;

	if	(_strText[iPoint]>='0' && _strText[iPoint]<='9')
	{
		int	iRed	=	getColorNumber(_strText,&iPoint);
		int	iGreen	=	getColorNumber(_strText,&iPoint);
		int	iBlue	=	getColorNumber(_strText,&iPoint);

		if	(iRed<=-1 || iGreen<=-1  || iBlue<=-1 	||	iRed > 255 || iGreen > 255 || iBlue > 255)
			return	FALSE;
	}
	else
	if	(_strText[iPoint]>='A' && _strText[iPoint]<='Z')
	{
		int	iColor	=	getColor(_strText,&iPoint);

		if	(iColor==-1)	
			return	FALSE;
	}

	if	(_strText[iPoint-1]	!=	'>')
		return	FALSE;

	return	TRUE;
}
//
//	특수 코드 처리
int
CTextDC::operateSpecialCode(char *_strText,int *_iPoint)
{
	int	iPoint			=	*_iPoint;
	int	iStringLength	=	strlen(_strText);

	if (_strText[iPoint]==0x0d)
	{
		iPoint++;

		if (_strText[iPoint]==0x0a)
			iPoint++;

		*_iPoint	=	iPoint;

		return	dOPERATE_SPECIAL_CODE;
	}	//	줄바꾸기

	if (_strText[iPoint]==0x0a)
	{
		iPoint++;

		*_iPoint	=	iPoint;

		return	dOPERATE_SPECIAL_CODE;
	}	//	줄바꾸기

	if (m_bIsWantPorcessReturnKeyByUser	&&	_strText[iPoint] ==	'|')
	{
		iPoint++;

		*_iPoint	=	iPoint;

		return	dOPERATE_SPECIAL_CODE;
	}	//	줄바꾸기

	if	(!m_bIsDecodeScript)
		return	FALSE;	//	특수코드가 아니다.

	if	(_strText[iPoint]!=	'<')
		return	FALSE;	//	특수코드가 아니다.

	iPoint++;
	if (_strText[iPoint]	==	NULL)	return	FALSE;

//	볼드
	if(_strText[iPoint]==	'b' && !m_bIsIgnoreSomeCode)
	{
		if (_strText[iPoint+1]	!=	'>')	return	FALSE;

		iPoint	+=	2;
		*_iPoint=	iPoint;

		if (m_iSelectFont	==	0xffff				)
		{
			if (m_fontInfo.m_dwStyle ==	FW_BOLD)
				m_fontInfo.m_dwStyle =	FW_NORMAL;
			else
				m_fontInfo.m_dwStyle =	FW_BOLD;

			selectFont(&m_fontInfo);
		}
		else
		if (m_iSelectFont	==	dWINDOW_FONT_BOLD)
			selectFont(dWINDOW_FONT_NORMAL);
		else
			selectFont(dWINDOW_FONT_BOLD);

		SelectObject( s_hDC, getFont());

		return	dOPERATE_SPECIAL_CODE;
	}	//	볼드

//	사이즈
	if	(_strText[iPoint]==	's' && !m_bIsIgnoreSomeCode)
	{
		if (_strText[iPoint+1]	==	'>')
		{
			iPoint					+=	2;
			*_iPoint				=	iPoint;

			if	(m_orgFontInfo.m_iFontSize	==	0)
				return	dOPERATE_SPECIAL_CODE;

			m_fontInfo.m_iFontSize	=	m_orgFontInfo.m_iFontSize;
			selectFont(&m_fontInfo);

			return	dOPERATE_SPECIAL_CODE;
		}

		if (_strText[iPoint+1]	!=	':')	return	FALSE;
		iPoint	+=	2;
		if (_strText[iPoint]	==	NULL)	return	FALSE;

		if (_strText[iPoint]>='0' && _strText[iPoint]<='9')
		{
			m_fontInfo.m_iFontSize	=	getColorNumber(_strText,&iPoint);
			selectFont(&m_fontInfo);
			*_iPoint				=	iPoint;

			return	dOPERATE_SPECIAL_CODE;
		}

		return	dIS_INCORRECT_SPECIAL_CODE;
	}	//	볼드

//	컬러 처리
	if	(_strText[iPoint]	==	'c')
	{
		int iRed,iGreen,iBlue;

		if	(_strText[iPoint+1]	!=	':')
			return	FALSE;

		iPoint+=2;

		if	(_strText[iPoint]	==	NULL)
			return	FALSE;

		if (_strText[iPoint]>='0' && _strText[iPoint]<='9')
		{
			iRed	=	getColorNumber(_strText,&iPoint);
			iGreen	=	getColorNumber(_strText,&iPoint);
			iBlue	=	getColorNumber(_strText,&iPoint);

			if	(iRed<=-1 || iGreen<=-1  || iBlue<=-1 	||	iRed > 255 || iGreen > 255 || iBlue > 255)
				return	dIS_INCORRECT_SPECIAL_CODE;

			if	(m_bIsApplyColorText)
			{
				setTextColor(RGB(iRed,iGreen,iBlue));

				m_dwLastSettingColor	=	RGB(iRed,iGreen,iBlue);
			}

			*_iPoint=	iPoint;

			return	dOPERATE_SPECIAL_CODE;
		}

		if (_strText[iPoint]>='A' && _strText[iPoint]<='Z')
		{
			int	iColor	=	getColor(_strText,&iPoint);

			if	(iColor==-1)	
				return	dIS_INCORRECT_SPECIAL_CODE;

			if	(m_bIsApplyColorText)
			{
				setTextColor(iColor);
				m_dwLastSettingColor	=	iColor;
			}

			*_iPoint	=	iPoint;

			return	dOPERATE_SPECIAL_CODE;
		}

		return	dIS_INCORRECT_SPECIAL_CODE;
	}	//	컬러 처리

//	줄간격
	if(_strText[iPoint]==	'l' && !m_bIsIgnoreSomeCode)
	{
		if (_strText[iPoint+1]	==	'>')
		{
			*_iPoint	=	iPoint+2;
			m_iLineGap	=	m_iStackLineGap;

			return	dOPERATE_SPECIAL_CODE;
		}

		BOOL	isMinus	=	FALSE;

		if (_strText[iPoint+1]	!=	':')	return	FALSE;

		iPoint	+=	2;

		if (_strText[iPoint]=='-')	isMinus	=	TRUE,iPoint++;

		if (_strText[iPoint]>='0' && _strText[iPoint]<='9')
		{
			int	iLineGap	=	getColorNumber(_strText,&iPoint);

			if	(iLineGap		==	-1)	
				return	dIS_INCORRECT_SPECIAL_CODE;

			if (isMinus)	iLineGap	=	-iLineGap;

			m_iLineGap	=	iLineGap;

			*_iPoint=	iPoint;

			return	dOPERATE_SPECIAL_CODE;
		}

		return	dIS_INCORRECT_SPECIAL_CODE;
	}

//	원래 컬러로 돌려 놓기
	if(_strText[iPoint]==	'n')
	{
		if (_strText[iPoint+1]	!=	'>')
			return	FALSE;

		iPoint	+=	2;
		*_iPoint=	iPoint;

		if (m_bIsApplyColorText)
			setTextColor(m_dwTextColor);

		m_dwLastSettingColor	=	m_dwTextColor;

		return	dOPERATE_SPECIAL_CODE;
	}	//	원래 컬러로 돌려 놓기

//	리버스
	if(_strText[iPoint]==	'r' && !m_bIsIgnoreSomeCode)
	{
		if (_strText[iPoint+1]	!=	'>')	return	FALSE;

		iPoint	+=	2;
		*_iPoint=	iPoint;

		if (!m_isReverse)
		{
			DWORD	dwColor	=	GetTextColor(s_hDC);

			m_isReverse		=	TRUE;

			SetBkMode	(s_hDC,OPAQUE);
			SetBkColor	(s_hDC,dwColor);

			setTextColor(dwColor);
		}
		else
		{
			DWORD	dwColor	=	GetBkColor(s_hDC);

			m_isReverse		=	FALSE;

			SetBkMode	(s_hDC,TRANSPARENT);
			setTextColor(dwColor);
		}

		return	dOPERATE_SPECIAL_CODE;
	}	//	리버스

	return	FALSE;
}


//
//	특수 코드 체크(색깔,반전,줄바꾸기등은 무시 getPixelSize용)
int
CTextDC::checkSpecialCodeForGetPixelSize(char *_strText,int *_iPoint)
{
	int	iPoint	=	*_iPoint;

	if (_strText[iPoint]==0x0d)
	{
		if (_strText[iPoint+1]	==	0x0a)	iPoint++;

		*_iPoint	=	iPoint+1;

		return	dOPERATE_SPECIAL_CODE;
	}	//	줄바꾸기


	if (_strText[iPoint]==0x0a)
	{
		*_iPoint	=	iPoint+1;

		return	dOPERATE_SPECIAL_CODE;
	}	//	줄바꾸기
	if (m_bIsWantPorcessReturnKeyByUser && _strText[iPoint]=='|')
	{
		*_iPoint	=	iPoint+1;

		return	dOPERATE_SPECIAL_CODE;
	}	//	줄바꾸기

	if	(!m_bIsDecodeScript)
		return	FALSE;	//	특수코드가 아니다.

	if	(_strText[iPoint]!=	'<')
		return	FALSE;	//	특수코드가 아니다.

	iPoint	++;

//	볼드
	if(_strText[iPoint]==	'b'  && !m_bIsIgnoreSomeCode)
	{
		if (_strText[iPoint+1]	!=	'>')	return	FALSE;

		iPoint	+=	2;
		*_iPoint=	iPoint;

		if (m_iSelectFont	==	0xffff				)
		{
			if (m_fontInfo.m_dwStyle ==	FW_BOLD)	m_fontInfo.m_dwStyle =	FW_NORMAL;
			else									m_fontInfo.m_dwStyle =	FW_BOLD;

			selectFont(&m_fontInfo);
		}
		else
		if (m_iSelectFont	==	dWINDOW_FONT_BOLD)	selectFont(dWINDOW_FONT_NORMAL);
		else										selectFont(dWINDOW_FONT_BOLD);

		SelectObject( s_hDC, getFont());

		return	dOPERATE_SPECIAL_CODE;
	}	//	볼드

//	사이즈
	if(_strText[iPoint]==	's' && !m_bIsIgnoreSomeCode)
	{
		if (_strText[iPoint+1]	==	'>')
		{
			iPoint	+=	2;
			*_iPoint=	iPoint;

			if	(m_orgFontInfo.m_iFontSize	==	0)
				return	dOPERATE_SPECIAL_CODE;

			m_fontInfo.m_iFontSize	=	m_orgFontInfo.m_iFontSize;
			selectFont(&m_fontInfo);

			return	dOPERATE_SPECIAL_CODE;
		}

		if (_strText[iPoint+1]	!=	':')	return	FALSE;
		iPoint	+=	2;
		if (_strText[iPoint]	==	NULL)	return	FALSE;

		if (_strText[iPoint]>='0' && _strText[iPoint]<='9')
		{
			m_fontInfo.m_iFontSize	=	getColorNumber(_strText,&iPoint);
			selectFont(&m_fontInfo);
			*_iPoint				=	iPoint;

			return	dOPERATE_SPECIAL_CODE;
		}

		return	dIS_INCORRECT_SPECIAL_CODE;
	}	//	볼드

//	컬러 처리
	if (_strText[iPoint]	==	'c')
	{
		int iRed,iGreen,iBlue;

		if	(_strText[iPoint+1]	!=	':')
			return	FALSE;

		iPoint+=2;

		if	(_strText[iPoint]	>=	'0' && _strText[iPoint]	<=	'9')
		{
			iRed	=	getColorNumber(_strText,&iPoint);
			iGreen	=	getColorNumber(_strText,&iPoint);
			iBlue	=	getColorNumber(_strText,&iPoint);

			if	(iRed<=-1 || iGreen<=-1  || iBlue<=-1	||	iRed > 255 || iGreen > 255 || iBlue > 255)
				return	dIS_INCORRECT_SPECIAL_CODE;

			*_iPoint=	iPoint;
			m_dwLastSettingColor	=	RGB(iRed,iGreen,iBlue);

			return	dOPERATE_SPECIAL_CODE;
		}

		if (_strText[iPoint]>='A' && _strText[iPoint]<='Z')
		{
			int	iColor	=	getColor(_strText,&iPoint);

			if(iColor==-1)
				return	dIS_INCORRECT_SPECIAL_CODE;

			m_dwLastSettingColor	=	iColor;
			*_iPoint				=	iPoint;

			return	dOPERATE_SPECIAL_CODE;
		}

		return	dIS_INCORRECT_SPECIAL_CODE;
	}	//	컬러 처리

	if (_strText[iPoint]	==	'l' && !m_bIsIgnoreSomeCode)
	{
		if (_strText[iPoint+1]	==	'>')
		{
			*_iPoint=	iPoint+	2;

			return	dOPERATE_SPECIAL_CODE;
		}

		if (_strText[iPoint+1]	!=	':')	return	FALSE;

		iPoint	+=	2;
		if (_strText[iPoint]	==	'-')	iPoint++;

		if (_strText[iPoint]>='0' && _strText[iPoint]<='9')
		{
			int	iGap	=	getColorNumber(_strText,&iPoint);

			if	(iGap	==	-1)	
				return	dIS_INCORRECT_SPECIAL_CODE;

			*_iPoint=	iPoint;

			return	dOPERATE_SPECIAL_CODE;
		}
	}

//	원래 컬러로 돌려 놓기
	if(_strText[iPoint]==	'n')
	{
		if (_strText[iPoint+1]	!=	'>')
			return	FALSE;

		iPoint	+=	2;
		*_iPoint=	iPoint;
		m_dwLastSettingColor	=	m_dwTextColor;

		return	dOPERATE_SPECIAL_CODE;
	}	//	원래 컬러로 돌려 놓기

//	리버스
	if(_strText[iPoint]==	'r'  && !m_bIsIgnoreSomeCode)
	{
		if (_strText[iPoint+1]	!=	'>')	return	FALSE;

		iPoint	+=	2;
		*_iPoint=	iPoint;

		return	dOPERATE_SPECIAL_CODE;
	}	//	리버스

	return	FALSE;
}

//
//	특수 코드 체크(색깔,반전등은 무시 getLineCount용)
int
CTextDC::checkSpecialCodeForGetLineCount(char *_strText,int *_iPoint)
{
	int	iPoint	=	*_iPoint;

	if (_strText[iPoint]==0x0d)
	{
		if	(_strText[iPoint+1]	==	0x0a)
			iPoint++;

		m_pos.x	=	m_basePos.x;
		m_iLineCount++;

		if	(m_iLineCount	>=	m_iBeginLine)
			m_pos.y	+=	m_iFontHeight+m_iLineGap;

		*_iPoint	=	iPoint+1;

		return	dOPERATE_ENTER_CODE;
	}	//	줄바꾸기

	if	(_strText[iPoint]==0x0a)
	{
		m_pos.x	=	m_basePos.x;
		m_iLineCount++;

		if	(m_iLineCount	>=	m_iBeginLine)
			m_pos.y	+=	m_iFontHeight+m_iLineGap;

		*_iPoint	=	iPoint+1;

		return	dOPERATE_ENTER_CODE;
	}	//	줄바꾸기

	if	(m_bIsWantPorcessReturnKeyByUser && _strText[iPoint]=='|')
	{
		m_pos.x	=	m_basePos.x;
		m_iLineCount++;

		if	(m_iLineCount	>=	m_iBeginLine)
			m_pos.y	+=	m_iFontHeight+m_iLineGap;

		*_iPoint	=	iPoint+1;

		return	dOPERATE_ENTER_CODE;
	}	//	줄바꾸기

	if	(!m_bIsDecodeScript)
		return	FALSE;

	if	(_strText[iPoint]!=	'<')
		return	FALSE;	//	특수코드가 아니다.

	iPoint	++;

//	볼드
	if(_strText[iPoint]==	'b' && !m_bIsIgnoreSomeCode)
	{
		if (_strText[iPoint+1]	!=	'>')	return	FALSE;

		iPoint	+=	2;
		*_iPoint=	iPoint;

		if (m_iSelectFont	==	0xffff				)
		{
			if (m_fontInfo.m_dwStyle ==	FW_BOLD)	m_fontInfo.m_dwStyle =	FW_NORMAL;
			else									m_fontInfo.m_dwStyle =	FW_BOLD;

			selectFont(&m_fontInfo);
		}
		else
		if (m_iSelectFont	==	dWINDOW_FONT_BOLD)	selectFont(dWINDOW_FONT_NORMAL);
		else										selectFont(dWINDOW_FONT_BOLD);

		SelectObject( s_hDC, getFont());

		return	dOPERATE_SPECIAL_CODE;
	}	//	볼드

//	사이즈
	if(_strText[iPoint]==	's' && !m_bIsIgnoreSomeCode)
	{
		if (_strText[iPoint+1]	==	'>')
		{
			iPoint	+=	2;
			*_iPoint=	iPoint;

			if	(m_orgFontInfo.m_iFontSize	==	0)
				return	dOPERATE_SPECIAL_CODE;

			m_fontInfo.m_iFontSize	=	m_orgFontInfo.m_iFontSize;
			selectFont(&m_fontInfo);

			return	dOPERATE_SPECIAL_CODE;
		}

		if (_strText[iPoint+1]	!=	':')	return	FALSE;
		iPoint	+=	2;
		if (_strText[iPoint]	==	NULL)	return	FALSE;

		if (_strText[iPoint]>='0' && _strText[iPoint]<='9')
		{
			m_fontInfo.m_iFontSize	=	getColorNumber(_strText,&iPoint);
			selectFont(&m_fontInfo);
			*_iPoint				=	iPoint;

			return	dOPERATE_SPECIAL_CODE;
		}

		return	dIS_INCORRECT_SPECIAL_CODE;
	}	//	볼드

//	컬러 처리
	if (_strText[iPoint]	==	'c')
	{
		int iRed,iGreen,iBlue;

		if	(_strText[iPoint+1]	!=	':')
			return	FALSE;

		iPoint+=2;

		if	(_strText[iPoint]	>=	'0' && _strText[iPoint]	<=	'9')
		{
			iRed	=	getColorNumber(_strText,&iPoint);
			iGreen	=	getColorNumber(_strText,&iPoint);
			iBlue	=	getColorNumber(_strText,&iPoint);

			if	(iRed<=-1 || iGreen<=-1  || iBlue<=-1 	||	iRed > 255 || iGreen > 255 || iBlue > 255)
				return	dIS_INCORRECT_SPECIAL_CODE;

			*_iPoint=	iPoint;
			m_dwLastSettingColor	=	RGB(iRed,iGreen,iBlue);

			return	dOPERATE_SPECIAL_CODE;
		}

		if (_strText[iPoint]>='A' && _strText[iPoint]<='Z')
		{
			int	iColor	=	getColor(_strText,&iPoint);

			if(iColor==-1)
				return	dIS_INCORRECT_SPECIAL_CODE;

			m_dwLastSettingColor	=	iColor;
			*_iPoint				=	iPoint;

			return	dOPERATE_SPECIAL_CODE;
		}

		return	dIS_INCORRECT_SPECIAL_CODE;
	}	//	컬러 처리

	if (_strText[iPoint]	==	'l' && !m_bIsIgnoreSomeCode)
	{
		if (_strText[iPoint+1]	==	'>')
		{
			*_iPoint=	iPoint+	2;

			return	dOPERATE_SPECIAL_CODE;
		}

		if (_strText[iPoint+1]	!=	':')	return	FALSE;

		iPoint	+=	2;
		if (_strText[iPoint]	==	'-')	iPoint++;

		if (_strText[iPoint]>='0' && _strText[iPoint]<='9')
		{
			int	iGap	=	getColorNumber(_strText,&iPoint);

			if	(iGap	==	-1)
				return	dIS_INCORRECT_SPECIAL_CODE;

			*_iPoint=	iPoint;

			return	dOPERATE_SPECIAL_CODE;
		}
	}

//	원래 컬러로 돌려 놓기
	if(_strText[iPoint]==	'n')
	{
		if (_strText[iPoint+1]	!=	'>')
			return	FALSE;

		iPoint	+=	2;
		*_iPoint=	iPoint;
		m_dwLastSettingColor	=	m_dwTextColor;

		return	dOPERATE_SPECIAL_CODE;
	}	//	원래 컬러로 돌려 놓기

//	리버스
	if(_strText[iPoint]==	'r'  && !m_bIsIgnoreSomeCode)
	{
		if (_strText[iPoint+1]	!=	'>')	return	FALSE;

		iPoint	+=	2;
		*_iPoint=	iPoint;

		return	dOPERATE_SPECIAL_CODE;
	}	//	리버스

	return	FALSE;
}

//
//	특수 코드 체크(포인트만 증가하고, 볼드 효과에만 반응한다.getOneLine용)
int
CTextDC::checkSpecialCodeForOneLine(char *_strText,int *_iPoint)
{
	int	iPoint	=	*_iPoint;

	if (_strText[iPoint]==0x0d)
	{
		if (_strText[iPoint+1]	==	0x0a)	iPoint++;

		*_iPoint	=	iPoint+1;

		return	dOPERATE_ENTER_CODE;
	}	//	줄바꾸기

	if (_strText[iPoint]==0x0a)
	{
		*_iPoint	=	iPoint+1;

		return	dOPERATE_ENTER_CODE;
	}	//	줄바꾸기

	if (m_bIsWantPorcessReturnKeyByUser && _strText[iPoint]=='|')
	{
		*_iPoint	=	iPoint+1;

		return	dOPERATE_ENTER_CODE;
	}	//	줄바꾸기

	if	(!m_bIsDecodeScript)
		return	FALSE;

	if	(_strText[iPoint]!=	'<')
		return	FALSE;	//	특수코드가 아니다.

	iPoint	++;

//	볼드
	if(_strText[iPoint]==	'b' && !m_bIsIgnoreSomeCode)
	{
		if (_strText[iPoint+1]	!=	'>')	return	FALSE;

		iPoint	+=	2;
		*_iPoint=	iPoint;

		if (m_iSelectFont	==	0xffff				)
		{
			if (m_fontInfo.m_dwStyle ==	FW_BOLD)	m_fontInfo.m_dwStyle =	FW_NORMAL;
			else									m_fontInfo.m_dwStyle =	FW_BOLD;

			selectFont(&m_fontInfo);
		}
		else
		if (m_iSelectFont	==	dWINDOW_FONT_BOLD)	selectFont(dWINDOW_FONT_NORMAL);
		else										selectFont(dWINDOW_FONT_BOLD);

		SelectObject( s_hDC, getFont());

		return	dOPERATE_SPECIAL_CODE;
	}	//	볼드

//	사이즈
	if(_strText[iPoint]==	's' && !m_bIsIgnoreSomeCode)
	{
		if (_strText[iPoint+1]	==	'>')
		{
			iPoint	+=	2;
			*_iPoint=	iPoint;

			if	(m_orgFontInfo.m_iFontSize	==	0)
				return	dOPERATE_SPECIAL_CODE;

			m_fontInfo.m_iFontSize	=	m_orgFontInfo.m_iFontSize;
			selectFont(&m_fontInfo);

			return	dOPERATE_SPECIAL_CODE;
		}

		if (_strText[iPoint+1]	!=	':')	return	FALSE;
		iPoint	+=	2;
		if (_strText[iPoint]	==	NULL)	return	FALSE;

		if (_strText[iPoint]>='0' && _strText[iPoint]<='9')
		{
			m_fontInfo.m_iFontSize	=	getColorNumber(_strText,&iPoint);
			selectFont(&m_fontInfo);
			*_iPoint				=	iPoint;

			return	dOPERATE_SPECIAL_CODE;
		}

		return	dIS_INCORRECT_SPECIAL_CODE;
	}	//	볼드

//	컬러 처리
	if (_strText[iPoint]	==	'c')
	{
		int iRed,iGreen,iBlue;

		if	(_strText[iPoint+1]	!=	':')
			return	FALSE;

		iPoint+=2;

		if	(_strText[iPoint]	>=	'0' && _strText[iPoint]	<=	'9')
		{
			iRed	=	getColorNumber(_strText,&iPoint);
			iGreen	=	getColorNumber(_strText,&iPoint);
			iBlue	=	getColorNumber(_strText,&iPoint);

			if	(iRed<=-1 || iGreen<=-1  || iBlue<=-1 	||	iRed > 255 || iGreen > 255 || iBlue > 255)
				return	dIS_INCORRECT_SPECIAL_CODE;

			*_iPoint=	iPoint;
			m_dwLastSettingColor	=	RGB(iRed,iGreen,iBlue);

			return	dOPERATE_SPECIAL_CODE;
		}

		if (_strText[iPoint]>='A' && _strText[iPoint]<='Z')
		{
			int	iColor	=	getColor(_strText,&iPoint);

			if(iColor==-1)
				return	dIS_INCORRECT_SPECIAL_CODE;

			m_dwLastSettingColor	=	iColor;
			*_iPoint				=	iPoint;

			return	dOPERATE_SPECIAL_CODE;
		}

		return	dIS_INCORRECT_SPECIAL_CODE;
	}	//	컬러 처리

	if (_strText[iPoint]	==	'l' && !m_bIsIgnoreSomeCode)
	{
		if (_strText[iPoint+1]	==	'>')
		{
			*_iPoint=	iPoint+	2;

			return	dOPERATE_SPECIAL_CODE;
		}

		if (_strText[iPoint+1]	!=	':')	return	FALSE;

		iPoint	+=	2;
		if (_strText[iPoint]	==	'-')	iPoint++;

		if (_strText[iPoint]>='0' && _strText[iPoint]<='9')
		{
			int	iGap	=	getColorNumber(_strText,&iPoint);

			if	(iGap	==	-1)	
				return	dIS_INCORRECT_SPECIAL_CODE;

			*_iPoint=	iPoint;

			return	dOPERATE_SPECIAL_CODE;
		}
	}

//	원래 컬러로 돌려 놓기
	if(_strText[iPoint]==	'n')
	{
		if (_strText[iPoint+1]	!=	'>')
			return	FALSE;

		iPoint	+=	2;
		*_iPoint=	iPoint;
		m_dwLastSettingColor	=	m_dwTextColor;

		return	dOPERATE_SPECIAL_CODE;
	}	//	원래 컬러로 돌려 놓기

//	리버스
	if(_strText[iPoint]==	'r'  && !m_bIsIgnoreSomeCode)
	{
		if (_strText[iPoint+1]	!=	'>')	return	FALSE;

		iPoint	+=	2;
		*_iPoint=	iPoint;

		return	dOPERATE_SPECIAL_CODE;
	}	//	리버스

	return	FALSE;
}

//
//	특수 코드 체크(포인트만 증가 getWords용)
int
CTextDC::checkSpecialCode(char *_strText,int *_iPoint)
{
	int	iPoint	=	*_iPoint;

	if	(_strText[iPoint]==	0x0a	||	_strText[iPoint]==	0x0d)
	{
		(*_iPoint)++;

		return	TRUE;
	}

	if	(m_bIsWantPorcessReturnKeyByUser	&&	_strText[iPoint] ==	'|')
	{
		(*_iPoint)++;

		return	TRUE;
	}

	if	(!m_bIsDecodeScript)
		return	FALSE;

	if	(_strText[iPoint]!=	'<')
		return	FALSE;	//	특수코드가 아니다.

	iPoint	++;

//	볼드
	if(_strText[iPoint]==	'b' && !m_bIsIgnoreSomeCode)
	{
		if (_strText[iPoint+1]	!=	'>')
			return	FALSE;

		iPoint	+=	2;
		*_iPoint=	iPoint;

		return	TRUE;
	}	//	볼드

//	사이즈
	if(_strText[iPoint]==	's' && !m_bIsIgnoreSomeCode)
	{
		if (_strText[iPoint+1]	==	'>')
		{
			iPoint	+=	2;
			*_iPoint=	iPoint;

			return	dOPERATE_SPECIAL_CODE;
		}

		if (_strText[iPoint+1]	!=	':')	return	FALSE;
		iPoint	+=	2;
		if (_strText[iPoint]	==	NULL)	return	FALSE;

		if (_strText[iPoint]>='0' && _strText[iPoint]<='9')
		{
			int	iSize			=	getColorNumber(_strText,&iPoint);
			*_iPoint			=	iPoint;

			return	TRUE;
		}

		return	FALSE;
	}	//	볼드

//	컬러 처리
	if (_strText[iPoint]	==	'c')
	{
		int iRed,iGreen,iBlue;

		if (_strText[iPoint+1]	!=	':')
			return	FALSE;

		iPoint+=2;

		if (_strText[iPoint]>='0' && _strText[iPoint]<='9')
		{
			iRed	=	getColorNumber(_strText,&iPoint);
			iGreen	=	getColorNumber(_strText,&iPoint);
			iBlue	=	getColorNumber(_strText,&iPoint);

			if	(iRed<=-1 || iGreen<=-1  || iBlue<=-1 	||	iRed > 255 || iGreen > 255 || iBlue > 255)
				return	FALSE;

			m_dwLastSettingColor	=	RGB(iRed,iGreen,iBlue);

			*_iPoint=	iPoint;

			return	TRUE;
		}

		if (_strText[iPoint]>='A' && _strText[iPoint]<='Z')
		{
			int	iColor	=	getColor(_strText,&iPoint);

			if(iColor==-1)	
				return	FALSE;

			m_dwLastSettingColor	=	iColor;
			*_iPoint	=	iPoint;

			return	TRUE;
		}

		return	FALSE;
	}	//	컬러 처리

	if (_strText[iPoint]	==	'l' && !m_bIsIgnoreSomeCode)
	{
		if (_strText[iPoint+1]	==	'>')
		{
			*_iPoint=	iPoint+	2;

			return	TRUE;
		}

		if (_strText[iPoint+1]	!=	':')	return	FALSE;

		iPoint	+=	2;
		if (_strText[iPoint]	==	'-')	iPoint++;

		if (_strText[iPoint]>='0' && _strText[iPoint]<='9')
		{
			int	iGap	=	getColorNumber(_strText,&iPoint);

			if	(iGap	==	-1)
				return	FALSE;

			*_iPoint=	iPoint;

			return	TRUE;
		}
	}

//	원래 컬러로 돌려 놓기
	if(_strText[iPoint]==	'n')
	{
		if (_strText[iPoint+1]	!=	'>')
			return	FALSE;

		iPoint	+=	2;
		*_iPoint=	iPoint;
		m_dwLastSettingColor	=	m_dwTextColor;

		return	TRUE;
	}	//	원래 컬러로 돌려 놓기

//	리버스
	if(_strText[iPoint]==	'r'  && !m_bIsIgnoreSomeCode)
	{
		if (_strText[iPoint+1]	!=	'>')	return	FALSE;

		iPoint	+=	2;
		*_iPoint=	iPoint;

		return	TRUE;
	}	//	리버스

	return	FALSE;
}


void
CTextDC::releaseClipArea()
{
	m_rectViewPort.set(0,0,g_canvas.m_iWidth,g_canvas.m_iHeight);
}

void
CTextDC::setClipArea(cRECT *_lpRect)
{
	m_rectViewPort.set(_lpRect);
}

void
CTextDC::setClipArea(int _x1,int _y1,int _x2,int _y2)
{
	m_rectViewPort.set(_x1,_y1,_x2,_y2);
}
