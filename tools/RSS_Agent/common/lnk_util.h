//
//	제작 일시	2006.07.22
//
//	목적		각종 유틸리티,클래스 단위가 아닌 자잘한 함수들을 모았다.
//
//	제작		이영찬
//

#pragma once

#ifdef	_DEBUG
#define _CRTDBG_MAP_ALLOC
#endif

#include	"lnk_define.h"
#include	<stdio.h>

extern	inline int	random(int _iRange);	//	랜덤 함수 0x20000000 이상의 수치는 지원하지 않는다.

#ifdef	WIN32
#include	<windows.h>

class	cClipboard
{
	TCHAR	*m_lpBuffer;
public:

	cClipboard()
	{
		m_lpBuffer	=	NULL;
	}
	~cClipboard()
	{
		m_lpBuffer	=	NULL;
	}

//	TCHAR*	get();
//	int		copy(TCHAR *_lpstrText);


	TCHAR*	get()
	{
		pKILL(m_lpBuffer);

		BOOL	bRtn;

		// 클립보드 열고
		bRtn	=	OpenClipboard(NULL);

		if	(!bRtn)
			return	NULL;

		// 클립보드 데이터 열고

		int	iCode	=	CF_TEXT;

	#ifdef	UNICODE
		iCode	=	CF_UNICODETEXT;
	#endif

		HGLOBAL	hglb	=	GetClipboardData(iCode);

		if	(hglb == NULL)
			return FALSE; 
		// 글로벌로 memory 잡고 카피

		m_lpBuffer	=	(TCHAR *)GlobalLock(hglb);

		GlobalUnlock(hglb);

		// 클립보드 닫고
		bRtn		=	CloseClipboard();

		if	(bRtn)
			return	m_lpBuffer;

		return NULL;
	}

	int		copy(TCHAR *_lpstrText)
	{
		BOOL bRtn = FALSE;

		// 클립보드 열고
		bRtn	=	OpenClipboard(NULL);

		if	(!bRtn)
			return	false;

		bRtn	=	EmptyClipboard(); 

		if	(!bRtn)
			return	false;

		//////////////////////////////////////////////////////////////////////////
		/// 글로벌로 memory 잡고
		HGLOBAL hglb;
		LPTSTR  lptstr;

		int	cch	=	(int)STRLEN(_lpstrText);

		hglb	=	GlobalAlloc(GMEM_MOVEABLE, (cch + 1) * sizeof(TCHAR));

		if	(hglb == NULL)
			return FALSE;

		lptstr = (LPTSTR)GlobalLock(hglb);

		memcpy(lptstr, _lpstrText, cch * sizeof(TCHAR));

		lptstr[cch] = (TCHAR)0;

		GlobalUnlock(hglb);    
		//////////////////////////////////////////////////////////////////////////

	#ifdef	UNICODE
		SetClipboardData(CF_UNICODETEXT, hglb);
	#else
		SetClipboardData(CF_TEXT, hglb);
	#endif

		// 클립보드 닫고
		bRtn	=	CloseClipboard();

		return bRtn;
	}

};

//반올림 함수
inline int __inRound(float _fValue)
{
	return (int)(_fValue+(_fValue>0? .5: -.5) );
}

// float 형 '==' 검사
inline	BOOL	inAlmostSame_common( float	_f1 , float _f2 )
{
	#define dALMOST_ZERO	1.0e-4	// 소수점 오차를 잡기 

	return ( _f1 + dALMOST_ZERO >= _f2 && _f1 - dALMOST_ZERO <= _f2 );
}


inline	void	_break()
{
	assert(0);
	// 64 비트 컴파일시 에러가 난다 (error C4235: 비표준 확장 사용 : 이 아키텍처에서는 '__asm' 키워드를 사용할 수 없습니다.)
	//__asm
	//{
	//	int	3
	//}
}

//	파일 확장자 변경
extern	bool	SetFileExt(TCHAR *_lpstrFileName,int _iBufferLen,const TCHAR *_lpstrExt);
// 파일 확장자 추출
extern	TCHAR*	ExportFileExt( const TCHAR* _StrSrc, TCHAR* _strTarget = NULL );
//	폴더명 추출
extern	TCHAR*	ExportFolderName(const TCHAR *_lpstrSrc,TCHAR *_lpstrTarget=NULL);
//	파일명 추출(확장자 포함)
extern	TCHAR*	ExportFileName(const TCHAR *_lpstrSrcString,TCHAR *_lpstrTargetString=NULL);
//	파일명 추출(확장자 빼고)
extern	TCHAR*	ExportOnlyFileName(const TCHAR *_lpstrSrcString,TCHAR *_lpstrTargetString=NULL);


//	 크리티컬 섹션 자동화
class	cCriticalSectionAutomate
{
public:
	CRITICAL_SECTION	*m_lpCs;

	cCriticalSectionAutomate(CRITICAL_SECTION * _lpCs)
	{
		m_lpCs	=	_lpCs;
		EnterCriticalSection(m_lpCs);
	}

	~cCriticalSectionAutomate()
	{
		LeaveCriticalSection(m_lpCs);
	}
};

#ifndef	mCSA
#define	mCSA(cs)	cCriticalSectionAutomate ______cs(&cs)
#endif

#ifndef	mCS
#define	mCS(csname,cs)	cCriticalSectionAutomate csname(&cs)
#endif

inline int 
U2A(const TCHAR *_pszUnicode,char *_pszMulti,int _iMultiBufferSize,int _iOpt=CP_ACP)
{	
#if defined _UNICODE  ||  defined UNICODE
	int iResult = WideCharToMultiByte(_iOpt, 0, _pszUnicode, -1, _pszMulti, _iMultiBufferSize, NULL, NULL);

	if	(iResult == 0)
	{
		DWORD dwRe = GetLastError();

		if	(dwRe == ERROR_INSUFFICIENT_BUFFER)
		{
			return 0;
		}
	}

	return iResult;
#else
	if	(_iMultiBufferSize >= (int)_tcslen(_pszUnicode))
	{
		_tcscpy_s(_pszMulti, _iMultiBufferSize, _pszUnicode);
		return (int)_tcslen(_pszMulti);
	}
	return 0;
#endif
}

inline const char*
IU2A(const TCHAR *_pszUnicode,int _iOpt=CP_ACP)
{
	static	char	strInstanceUnicodeToAsciiBuffer[4096];

#if defined _UNICODE  ||  defined UNICODE

	int iResult = WideCharToMultiByte(_iOpt, 0, _pszUnicode, -1, strInstanceUnicodeToAsciiBuffer, 4096, NULL, NULL);

	if	(iResult == 0)
	{
		DWORD dwRe = GetLastError();

		if	(dwRe == ERROR_INSUFFICIENT_BUFFER)
		{
			return	NULL;
		}
	}

	return strInstanceUnicodeToAsciiBuffer;
#else
	_tcscpy_s(strInstanceUnicodeToAsciiBuffer, 4096, _pszUnicode);

	return strInstanceUnicodeToAsciiBuffer;
#endif
}

inline int 
A2U(const char *_pszMulti, TCHAR *_pszWide, int _iWideBufferSize,int _iOpt=CP_ACP)
{	
#if defined _UNICODE  ||  defined UNICODE                      
	return	MultiByteToWideChar(_iOpt, 0, _pszMulti, -1, _pszWide, _iWideBufferSize);
#else
	if	(_iWideBufferSize > (int)_tcslen(_pszMulti))
	{
		_tcscpy_s(_pszWide, _iWideBufferSize, _pszMulti);

		return (int)_tcslen(_pszWide);
	}

	return 0;		
#endif
}

inline const TCHAR*
IA2U(const char *_pszMulti,int _iOpt=CP_ACP)
{	
	static	TCHAR	strInstanceAsciiToUnicodeBuffer[4096];
#if defined _UNICODE  ||  defined UNICODE                      
	if	(MultiByteToWideChar(_iOpt, 0, _pszMulti, -1, strInstanceAsciiToUnicodeBuffer, 4096)	!=	0)
		return	strInstanceAsciiToUnicodeBuffer;

	return	NULL;
#else
	if	(4096 > (int)_tcslen(_pszMulti))
	{
		_tcscpy_s(strInstanceAsciiToUnicodeBuffer, 4096, _pszMulti);

		return (int)_tcslen(strInstanceAsciiToUnicodeBuffer);
	}

	return NULL;
#endif
}

//	폴더 복귀
//	시작 폴더를 기억했다가 돌려 준다.

class	cReturnToCurrentDirectory
{
	TCHAR	m_strCurrentFolder[512];
public:
	cReturnToCurrentDirectory()
	{
		GetCurrentDirectory(512,m_strCurrentFolder);
	}
	~cReturnToCurrentDirectory()
	{
		SetCurrentDirectory(m_strCurrentFolder);
	}
};

//	return to current directory
//	이 녀석이 호출 되었을때 디렉토리를 기억해 뒀다가 클래스 소멸시 기억한 디록 톨리로 복귀시킨다.
#define	mRTCD()	cReturnToCurrentDirectory ___templateReturnToCurrentDirectoryClass;

//	데이터 변경 체크
class	cManageUpdateData
{
	int		m_iIsVirgin;	//	이 값이 true 이면 변경된 내용이 없는 것이다.

public:
	//cManageUpdateData();

	//void	makeVrigin();	//	변경된것이 없는 상태로 돌린다.
	//int		isLostVirgin();	//	데이터가 변경되었다.
	//int		isVirgin();		//	데이터 변경된것이 없다.
	//void	lostVirgin();	//	데이터가 변경됐다.

	cManageUpdateData()
	{
		m_iIsVirgin	=	1;
	}

	void	makeVrigin()
	{
		m_iIsVirgin	=	1;
	}

	int		isLostVirgin()
	{
		return	1-m_iIsVirgin;
	}

	int		isVirgin()
	{
		return	m_iIsVirgin;
	}

	void	lostVirgin()
	{
		m_iIsVirgin	=	0;
	}
};

inline void
ODS(TCHAR *_lpstrFormat,...) // OutputDebugString
{
	TCHAR	tempStr[256];

	va_list argptr;

	va_start(argptr, _lpstrFormat);

	_vstprintf_s(tempStr,_lpstrFormat,argptr);

	va_end(argptr);

	OutputDebugString(tempStr);
}

inline void
ODS(char *_lpstrFormat,...) // OutputDebugString
{
	char	tempStr[256];

	va_list argptr;

	va_start(argptr, _lpstrFormat);

	vsprintf_s(tempStr,_lpstrFormat,argptr);

	va_end(argptr);

	OutputDebugString(IA2U(tempStr));
}

class	cStringFormat
{
	TCHAR	m_string[2048];

public:
	cStringFormat(const TCHAR *_lpstrFormat,...)
	{
		va_list argptr;

		va_start(argptr, _lpstrFormat);

		_vstprintf_s(m_string,_lpstrFormat,argptr);

		va_end(argptr);
	}

	inline	TCHAR*	getString()
	{
		return	m_string;
	}
};

//	문자열 조합이 필요할 경우. 간단하게 사용한다.
inline	TCHAR*	_cdecl 	//	make string
_ms(const TCHAR *_lpstrFormat,...)
{
	static	TCHAR	s_tempStr[1024];

	va_list argptr;

	va_start(argptr, _lpstrFormat);

	_vstprintf_s(s_tempStr,_lpstrFormat,argptr);

	va_end(argptr);

	return s_tempStr;
}

//	문자열 조합이 필요할 경우. 간단하게 사용한다.
inline	TCHAR*	_cdecl 	//	make string
_ms(const char *_lpstrFormat,...)
{
	static	TCHAR	s_tempStr[1024];
	char			strTempChar[1024];

	va_list argptr;

	va_start(argptr, _lpstrFormat);
	vsprintf_s(strTempChar,_lpstrFormat,argptr);
	va_end(argptr);

	A2U(strTempChar,s_tempStr,1024);

	return s_tempStr;
}

inline	TCHAR*
NumToKoreaRead(int _iValue)
{
	int	iUk		=	_iValue/100000000;
	int	iMan	=	(_iValue/10000)%10000;
	int	iTail	=	_iValue%10000;

	static	TCHAR	strText[64];

	TCHAR	strUk[32]	=	_T("");
	TCHAR	strMan[32]	=	_T("");
	TCHAR	strTail[32]	=	_T("");

	if	(iUk)
	{
		if	(iMan || iTail)
			SPRINTF(strUk,_T("%d억 "),iUk);
		else
			SPRINTF(strUk,_T("%d억"),iUk);
	}

	if	(iMan)
	{
		if	(iTail)
			SPRINTF(strMan,_T("%d만 "),iMan);
		else
			SPRINTF(strMan,_T("%d만"),iMan);
	}

	if	(iTail || iUk + iMan == 0)
		SPRINTF(strTail,_T("%d"),iTail);

	SPRINTF(strText,_T("%s%s%s"),strUk,strMan,strTail);

	return	strText;
}

inline	TCHAR*
NumToEngRead(int _iValue)
{
	int	i1	=	(_iValue/1000000)%1000;
	int	i2	=	_iValue/1000000;
	int	i3	=	(_iValue/1000)%1000;
	int	i4	=	_iValue%1000;

	static	TCHAR	strText[64];

	if	(i1)
		SPRINTF(strText,_T("%d,%.3d,%.3d,%.3d "),i1,i2,i3,i4);
	else
	if	(i2)
		SPRINTF(strText,_T("%d,%.3d,%.3d "),i2,i3,i4);
	else
	if	(i3)
		SPRINTF(strText,_T("%d,%.3d "),i3,i4);
	else
		SPRINTF(strText,_T("%d"),i4);

	return	strText;
}

const	UINT	c_uiLargeRandomValue			=	0x8000;
const	UINT	c_uiLargeRandomLimitValue		=	0x40000000;
const	UINT	c_uiLargeRandomIncorrectRange	=	0xffffffff;

inline TCHAR* 
ExportFileName(const TCHAR *_lpstrSrcString,TCHAR *_lpstrTargetString)
{
	static	TCHAR	s_strTempExportFileNameString[1024];
	int iDest	=	0;

	if	(!_lpstrTargetString)
	{
		STRCPY(s_strTempExportFileNameString,_lpstrSrcString);

		for (UINT i=0;i<STRLEN(_lpstrSrcString);i++)	
			if	(_lpstrSrcString[i]=='\\' || _lpstrSrcString[i]=='/') 
				iDest	=	i+1;

		return &s_strTempExportFileNameString[iDest];
	}
	else
	{
		STRCPY(_lpstrTargetString,STRLEN(_lpstrSrcString)+1,_lpstrSrcString);

		for (UINT i=0;i<STRLEN(_lpstrSrcString);i++)
			if	(_lpstrSrcString[i]=='\\' || _lpstrSrcString[i]=='/') 
				iDest	=	i+1;

		return &_lpstrTargetString[iDest];
	}
}

inline TCHAR* 
ExportOnlyFileName(const TCHAR *_lpstrSrcString,TCHAR *_lpstrTargetString)
{
	static	TCHAR	s_strTempExportFileNameString[1024];
	int iDest=0;

	if	(!_lpstrTargetString)
	{
		STRCPY(s_strTempExportFileNameString,_lpstrSrcString);

		unsigned int i =0;

		for (i=0;i<STRLEN(_lpstrSrcString)	;i++)
			if	(_lpstrSrcString[i]=='\\' || _lpstrSrcString[i]=='/') 
				iDest	=	i+1;
		for (i=iDest;i<STRLEN(_lpstrSrcString)	;i++)
			if	(_lpstrSrcString[i]	==	'.'	)
				s_strTempExportFileNameString[i]=NULL;

		return &s_strTempExportFileNameString[iDest];
	}else
	{
		UINT	iLen	=	(UINT)STRLEN(_lpstrSrcString);

		STRCPY(_lpstrTargetString,iLen+1,_lpstrSrcString);

		UINT	i =0;

		for (i=0;i<iLen	;i++)
			if (_lpstrSrcString[i]=='\\' || _lpstrSrcString[i]=='/') 
				iDest	=i+1;

		for (i=iDest;i<(int)STRLEN(_lpstrSrcString)	;i++)
			if	(_lpstrSrcString[i]	==	'.'	)
				_lpstrTargetString[i]=	NULL;

		return &_lpstrTargetString[iDest];
	}
}

inline TCHAR *
ExportFolderName(const TCHAR *_lpstrSrc,TCHAR *_lpstrTarget)
{	
	static	TCHAR	s_strTempExportFolderNameString[1024];

	if	(!_lpstrTarget)
	{
		STRCPY(s_strTempExportFolderNameString,_lpstrSrc);

		int iSize = (int)STRLEN(_lpstrSrc);

		for (int i=iSize;i;i)
		{
			--i;

			if	(_lpstrSrc[i]	==	'\\' || _lpstrSrc[i]	==	'/') 
			{
				s_strTempExportFolderNameString[i]	=	NULL;

				return s_strTempExportFolderNameString;
			}
		}
	}
	else
	{
		int iSize	=	(int)STRLEN(_lpstrSrc);

		STRCPY(_lpstrTarget,iSize+1,_lpstrSrc);

		for (int i=iSize;i;)
		{
			--i;

			if	(_lpstrSrc[i]=='\\' || _lpstrSrc[i]	==	'/') 
			{	
				_lpstrTarget[i]	=	NULL;

				return	_lpstrTarget;
			}
		}
	}
	return NULL;
}

inline bool
SetFileExt(TCHAR *_lpstrFileName,int _iBufferLen,const TCHAR *_lpstrExt)
{
	int	iLen	=	(int)STRLEN(_lpstrFileName);
	int	iExtLen	=	(int)STRLEN(_lpstrExt);

	while(iLen > 0)
	{
		if	(_lpstrFileName[iLen] == _T('.'))
		{
			if	(iLen+iExtLen	>=	_iBufferLen)
				return	false;

			STRCPY(_lpstrFileName+iLen+1,iExtLen+1,_lpstrExt);
			return	true;
		}

		iLen--;
	}

	iLen	=	(int)STRLEN(_lpstrFileName);

	if	(iLen+iExtLen+1	>=	_iBufferLen)
		return	false;

	STRCPY(_lpstrFileName+iLen,iExtLen+2,_lpstrExt);

	return	true;
}

const	int	c_iFileExtBufferSize	=	16;

inline
TCHAR* ExportFileExt( const TCHAR* _strSrc, TCHAR* _strTarget )
{
	static TCHAR strExt[ c_iFileExtBufferSize];
	int iSize = ( int )STRLEN( _strSrc );
	int iTemp = iSize;
	
	while( _strSrc[ --iTemp ] != _T('.') && iTemp);

	if	(iTemp	==	0)
		return	L"";

	int i = 0;
	while( _strSrc[ ++iTemp ] != _T('\0') )
	{
		strExt[ i ] = _strSrc[ iTemp ];
		i++;
	}

	strExt[ i ] = _T('\0');

	return strExt;
}

//	랜덤함수
inline int
largeRandom(int _irange)
{
	if	(_irange	>=	c_uiLargeRandomLimitValue)
		return	c_uiLargeRandomIncorrectRange;

	int	iValue;

	if	(_irange	<=	c_uiLargeRandomValue)	
		return	random(_irange);

	iValue		=	_irange/c_uiLargeRandomValue+1;

	if	(_irange%c_uiLargeRandomValue)
	{
		UINT		u4MaximumValue;
		u8			u8Value;

		u4MaximumValue	=	iValue*c_uiLargeRandomValue-1;
		iValue			=	random(iValue)*c_uiLargeRandomValue;
		iValue			+=	random(c_uiLargeRandomValue);

		u8Value			=	iValue;
		u8Value			*=	_irange;
		u8Value			/=	u4MaximumValue;
		iValue			=	(int)u8Value;
	}
	else
	{
		iValue--;

		iValue			=	random(iValue)*c_uiLargeRandomValue;
		iValue			+=	random(c_uiLargeRandomValue);
	}

	return	iValue;
}

inline int
random(int _irange)
{
	if	(_irange	==	0) 
		return 0;

	if	(_irange	>	c_uiLargeRandomValue)
		return	largeRandom(_irange);

	return (rand()%_irange);
}

inline int
inIntRandom(int _irange)
{
	return	random(_irange);
}

inline int
random(int _iMinValue,int _iMaxValue)
{
	if	(_iMinValue	==	_iMaxValue) 
		return	_iMaxValue;

	if	(_iMinValue	>	_iMaxValue)
		Swap(_iMinValue,_iMaxValue);

	int	iRange	=	_iMaxValue-_iMinValue+1;

	if	(iRange	>	c_uiLargeRandomValue)
		return	_iMinValue+largeRandom(iRange);

	return _iMinValue+(rand()%iRange);
}

inline int
inIntRandom(int _iMinValue,int _iMaxValue)
{
	return	random(_iMinValue,_iMaxValue);
}

inline float inRandom_common( float _fMin, float _fMax )	// Random 값 얻기
{
	return ( (_fMin == _fMax) ? _fMin : ( ((float)rand()/(float)RAND_MAX) * (_fMax - _fMin) + _fMin ) );
}


// 값의 해당 인덱스의 비트에 값을 세팅한다.
template <typename T>
BOOL	
SetBitFlag_by_T( T &_TBit, int _iIndex )
{
	// 사이즈에 어긋 난다면 
	if( _iIndex < 0 || _iIndex >= sizeof(T) * 8 )
		return FALSE;

	T wReult(1);	

	wReult <<= _iIndex;

	_TBit |= wReult;

	return TRUE;
}

// 값의 해당 인덱스의 비트에 값을 해제한다.
template <typename T>
BOOL	
RemoveBitFlag_by_T( T &_TBit, int _iIndex )
{	
	// 사이즈에 어긋 난다면 
	if( _iIndex < 0 || _iIndex >= sizeof(T) * 8 )
		return FALSE;

	T wResult(0);
	memset( &wResult, 0xff, sizeof(T));
	{
		T wTmpResult(1);		

		wTmpResult <<= _iIndex;
		wResult -= wTmpResult;
	}

	_TBit &= wResult;

	return TRUE;
}

// 값의 해당 인덱스의 비트값을 체크한다.
template <typename T>
BOOL	
CheckBitFlag_by_T( T &_TBit, int _iIndex )
{
	// 사이즈에 어긋 난다면 
	if( _iIndex < 0 || _iIndex >= sizeof(T) * 8 )
		return FALSE;

	T wResult(1);

	wResult <<= _iIndex;

	wResult &= _TBit;

	if(wResult)
		return TRUE;
	else
		return FALSE;
}

// 값의 비트 값을 출력한다. (콘솔 전용)
template <typename T>
void	
PrintBitFlag_by_T( T &_TBit)
{	
	int iCnt = sizeof(T) * 8 -1;
	for(int i=0; i<=iCnt; i++)
	{
		T wResult(1);		

		wResult <<= iCnt - i;

		//값 추출
		wResult &= _TBit;

		if(wResult) 
			wResult = 1;
		else
			wResult = 0;

		printf("%d ", wResult);
	}
}

#endif	//	#ifdef	WIN32