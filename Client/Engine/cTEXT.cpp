/*
-------------------=X=|)___)
-------------------)__) )___) )
------------------)___) )____))_)
---------------- )____)_____))__)＼
--------------- )__|____/|___|___- ＼---
-------^^^^^^~~~＼oo oo oo oo /~~^^^^^^
------------~^^^^ ~~~~^^~~~~^^~~^^~~~~~
-----------------~~^^ ~^^~ ~^~ ~^
------------~^~~ ~~~^^~  ~~~~^^~~~~^^~~^^~~~~~
*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#include "CText.H"


HWND		CText::s_hWND				=	NULL;
HDC			CText::s_hDC				=	0;
cFONT_INFO	CText::s_aFont[dFONT_COUNT];
char		CText::s_strTempText[10*1024];
BOOL		CText::s_bIsDeviceActive	=	FALSE;

CText::cTEXT_COLOR	CText::s_aTextColor[40]	=
{
	{"WHITE",WHITE},
	{"BLACK",BLACK},
	{"LTGRAY",LTGRAY},
	{"GRAY",GRAY},
	{"DKGRAY",DKGRAY},
	{"LTRED",LTRED},
	{"RED",RED},
	{"DKRED",DKRED},
	{"LTGREEN",LTGREEN},
	{"GREEN",GREEN},
	{"DKGREEN",DKGREEN},
	{"LTBLUE",LTBLUE},
	{"BLUE",BLUE},
	{"DKBLUE",DKBLUE},
	{"LTYELLOW",LTYELLOW},
	{"YELLOW",YELLOW},
	{"DKYELLOW",DKYELLOW},
	{"LTCYAN",LTCYAN},
	{"CYAN",CYAN},
	{"DKCYAN",DKCYAN},
	{"BTPURPLE",BTPURPLE},
	{"LTPURPLE",LTPURPLE},
	{"CTPURPLE",LTPURPLE},
	{"PURPLE",PURPLE},
	{"DKPURPLE",DKPURPLE},
	{"GOLDYELLOW",GOLDYELLOW},
	{"LTBROWN",LTBROWN},
	{"BROWN",BROWN},
	{"DKBROWN",DKBROWN},
	{"BTBLUE",BTBLUE},
	{"CTBLUE",CTBLUE},
	{"LTORANGE",LTORANGE},
	{"ORANGE",ORANGE},
	{"LTPINK",LTPINK},	
	{"PINK",PINK},	
	{"",0xffffffff}
};

BOOL	
cFONT_INFO::loadFont(int _iXs,int _iYs,char *_lpszFont,DWORD _dwStyle)
{
	close();

	m_iWidth	=	_iXs,m_iHeight	=	_iYs;

	m_hFont		=	CreateFont( _iYs,_iXs, 0, 0, _dwStyle, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
								DEFAULT_QUALITY, DEFAULT_PITCH, _lpszFont);

	m_fontInfo.m_iFontSize		=	_iYs;
	strcpy(m_fontInfo.m_strFontName,_lpszFont);
	m_fontInfo.m_dwStyle		=	_dwStyle;
	m_fontInfo.m_iFontSize		=	_iYs;
	m_fontInfo.m_bIsItalic		=	FALSE;
	m_fontInfo.m_bIsUnderline	=	FALSE;
	m_fontInfo.m_iAngle			=	0;
	m_fontInfo.m_iCharset		=	DEFAULT_CHARSET;

	return	TRUE;
}

BOOL
cFONT_INFO::loadFont(int _iXs,int _iYs,char *_strName)
{
	m_iWidth	=	_iXs,m_iHeight	=	_iYs;

	LPCTSTR	lpszWFONTReg = "Sonaki's Pressed WANSUNG font";
	LPCTSTR	lpszJFONTReg = "Sonaki's Pressed JOHAP font";
	LPCTSTR	lpszEFONTReg = "Sonaki's Pressed ENG font";

	cFILE	File;
	char	strReg[50];
	int		iCount;

	if (!File.Open(_strName,"rb"))	return	ERRMSG("Error in CTextMT::loadHangulFont(..","can not find follow file\n\n%s",_strName);

	File.Seek(4);
	File.Read(strReg,40);

	if (strcmp(strReg,lpszJFONTReg)==0)
	{
		_Free(m_pHangulOffset);
		_Free(m_pHangulBuffer);

		File.Read(&iCount,4);

		m_pHangulOffset	=	_New(int,iCount+1,NULL,_ms("%s Hangul Offset",_strName));
		File.Read(m_pHangulOffset,4*(iCount+1));

		m_pHangulBuffer	=	_New(BYTE,m_pHangulOffset[iCount],NULL,_ms("%s Hangul Buffer",_strName));

		File.Read(m_pHangulBuffer,m_pHangulOffset[iCount]);
	}
	else
	if (strcmp(strReg,lpszEFONTReg)==0)
	{
		_Free(m_pEnglishOffset);
		_Free(m_pEnglishBuffer);

		File.Read(&iCount,4);

		m_pEnglishOffset	=	_New(int,iCount+1,NULL,_ms("%s English Offset",_strName));
		File.Read(m_pEnglishOffset,4*(iCount+1));

		m_pEnglishBuffer	=	_New(BYTE,m_pEnglishOffset[iCount],NULL,_ms("%s English Buffer",_strName));

		File.Read(m_pEnglishBuffer,m_pEnglishOffset[iCount]);
	}
	else
	{
		File.Close();

		return	ERRMSG("Error in CTextMT::loadHangulFont(..","i don't know follow file type\n\n%s",_strName);
	}

	File.Close();

	return TRUE;
}

void
CText::Init(HWND _hWnd,char *_lpStrFontName)
{
	s_hWND			=	_hWnd;

	if	(!_lpStrFontName)
		return;

	int	iMinSize	=	11;

#ifdef	_FOR_THAI
	iMinSize	=	13;
#endif

#ifdef	_FOR_ENG
	LoadWindowFont(0,max(iMinSize,13),_lpStrFontName,dWINDOW_FONT_NORMAL	,FW_NORMAL);
	LoadWindowFont(0,max(iMinSize,13),_lpStrFontName,dWINDOW_FONT_BOLD		,FW_BOLD);
	LoadWindowFont(0,max(iMinSize,12),_lpStrFontName,dWINDOW_FONT_SMALL		,FW_NORMAL);
#elif	_FOR_JAPAN
	LoadWindowFont(0,max(iMinSize,12),_lpStrFontName,dWINDOW_FONT_NORMAL	,FW_NORMAL);
	LoadWindowFont(0,max(iMinSize,13),_lpStrFontName,dWINDOW_FONT_BOLD		,FW_BOLD);
	LoadWindowFont(0,max(iMinSize,11),_lpStrFontName,dWINDOW_FONT_SMALL		,FW_NORMAL);
#else
	LoadWindowFont(0,max(iMinSize,12),_lpStrFontName,dWINDOW_FONT_NORMAL	,FW_NORMAL);
	LoadWindowFont(0,max(iMinSize,13),_lpStrFontName,dWINDOW_FONT_BOLD		,FW_BOLD);
	LoadWindowFont(0,max(iMinSize,12),_lpStrFontName,dWINDOW_FONT_SMALL		,FW_NORMAL);
#endif
}

void
CText::LoadWindowFont(int _iXs,int _iYs,char *_lpszFont,int _iIndex,DWORD _dwStyle)
{
	s_aFont[_iIndex].loadFont(_iXs,_iYs,_lpszFont,_dwStyle);
}

void
CText::LoadSpriteFont(int _iXs,int _iYs,char *_strHan,char *_strEng,int _iIndex)
{
	s_aFont[_iIndex].loadFont(_iXs,_iYs,_strHan);
	s_aFont[_iIndex].loadFont(_iXs,_iYs,_strEng);
}

void
CText::Destroy()
{
	for (int i=0;i<dFONT_COUNT;i++)	s_aFont[i].close();
}



void
CText::replaceText(char* _strSaveText, char* _strTargetText, char* _strFindText, char* _strReplaceText,int _iSaveTextLength)
{		// 텍스트바꾸기..

	char* lpText			= new char[_iSaveTextLength];

	if(!lpText)
		return;

	strcpy(lpText,"");

	int iTargetTextLen		= strlen(_strTargetText);
	int iFindTextLen		= strlen(_strFindText);
	int iReplaceTextLen		= strlen(_strReplaceText);
	char strTempText[2]		= "";
	for(int i=0;i< iTargetTextLen;++i)
	{
		if(iTargetTextLen - i < iFindTextLen)
		{
			strcat(lpText, (_strTargetText+i));
			break;
		}
		
		if(strnicmp(&_strTargetText[i],_strFindText, iFindTextLen) == 0)
		{
			if(int(strlen(lpText)) + iReplaceTextLen >= _iSaveTextLength)
				break;
			strcat(lpText, _strReplaceText);
			i += iFindTextLen - 1;
		}
		else
		{
			strcpy(strTempText,"");
			strTempText[0]	=	_strTargetText[i];
			strcat(lpText,strTempText);
		}
	}

	strcpy(_strSaveText,lpText);

	delete	lpText;
}
