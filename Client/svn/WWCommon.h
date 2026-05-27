#pragma once

#ifdef WIN32
	#include <windows.h>
#endif
///////////////////////////////////////////////////////////////////
//	Common Libary Of 지현이(WhiteWitch)
//	- 가능한 가볍고, 꼭 필요한 것들만 챙겨보자.
//	- x86-32,64bit를 지원하도록 하자.
//	- 리눅스와 공용으로 사용도록 ..
//	- Unicode를 기본으로 작성한다.
//	- SFC에서 가져온 내용도 있다. 
//	  (SFC의 원작자(sonaki@hitel.net)
//
//	Start Date : 16 Nov 2005 (Wednesday)
//
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//  Explanation
//
///////	Light Stuff
//
//	class CCritical : CriticalSection 사용시 정해진 범위를 벋어날때 자동으로 해제하도록 하는 것. 부분적으로 해제할때는 조심할 것.
//	class CHashCode : 검색 향상을 위해.
//	class CUniCode	: 유니코드 와 ASCii의 변환 기능
//
///////	Non-Light Stuff
//
//	class CZLib : zip lib를 사용한 압축/풀기 기능
//	class CWWTime : time관련된 내용을 얻어오는 기능을 정의해뒀다. 
//	class CCPUClockCheck : CPU의 클럭값을 가져올수 있는 asm 코드. timeGetTime 보다 좀 더 자세한 퍼포먼스 확인을 위해 사용한다. 무게감있는 클래스는 아니나, 2개의 전역변수를 사용해야 하여, 이쪽으로 분리했다.
//	class CEmrErrorLog : MSJ에 발표된 기사를 토대로 작성된 클래스로, 해당 APP에 문제가 발생했을때 RPT파일에 현재의 스택정보와 에러난 위치를 기록하는 클래스다.
//
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//	사용하지 않는 기능은 삭제하도록 하자.
//#define __USE_CWWTime
//#define __USE_CZLib
//#define __USE_CCPUCLOCKCHECK
//#define __USE_CEmrErrorLog
#define __UNICODE	//	UNICODE가 기반이 될 경우
//#define __64BIT		//	64BIT 환경에서 컴파일 할 경우 
#define __USE_FILE_UTIL

#ifdef WIN32
#ifndef _WINDOWS
#define _WINDOWS
#endif
#endif
//	
///////////////////////////////////////////////////////////////////

//	기타 64bit 환경에서의 변화되는 것들을 정의해두자. 추후..
//#ifdef __64BIT
//	#define W2LONG		__int64
//	#define	W2ULONG		unsigned __int64
//#else
	#define W2LONG		__int64
	#define	W2ULONG		unsigned __int64
//#endif

#ifdef __UNICODE	//	관련 코드들을 잘 정리할 것!!

	////////////////////////////////////////////////////
	//	variable type
	#define W2BYTE		unsigned char

	#define W2CHAR		wchar_t
	#define W2UCHAR		wchar_t
	//	variable type
	////////////////////////////////////////////////////

	////////////////////////////////////////////////////
	//	about string
	#define W2PRINTF	wprintf
	#define W2SPRINTF	swprintf

	#define W2STRCPY	wcscpy
	#define W2STRNCPY	wcsncpy
	#define W2STRCMP	wcscmp
	#define W2STRICMP	_wcsicmp

	#define W2STRLEN	wcslen
	#define W2STRCAT	wcscat
	//	about string
	////////////////////////////////////////////////////

	////////////////////////////////////////////////////
	//	about file
	#define W2FOPEN		_wfopen
	//	about file
	////////////////////////////////////////////////////

#else
	////////////////////////////////////////////////////
	//	variable type
	#define W2BYTE		unsigned char

	#define W2CHAR		char
	#define W2UCHAR		unsigned char
	//	variable type
	////////////////////////////////////////////////////

	////////////////////////////////////////////////////
	//	about string
	#define W2PRINTF	printf
	#define W2SPRINTF	sprintf

	#define W2STRCPY	strcpy
	#define W2STRNCPY	strncpy
	#define W2STRCMP	strcmp
	#define W2STRICMP	stricmp

	#define W2STRLEN	strlen
	#define W2STRCAT	strcat
	//	about string
	////////////////////////////////////////////////////

	////////////////////////////////////////////////////
	//	about file
	#define W2FOPEN		fopen
	//	about file
	////////////////////////////////////////////////////
#endif

namespace W2Lib
{
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃class	CCritical
class	CCritical
{
public:
	CRITICAL_SECTION	* cs;

	CCritical(CRITICAL_SECTION * _cs)
	{
		cs = _cs;
		EnterCriticalSection(cs);
	}

	~CCritical()
	{
		LeaveCriticalSection(cs);
	}
};
//┃class	CCritical
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃class	CHashCode
class CHashCode
{
public:
	static W2ULONG GetHashCode(BYTE* _lpString);
};
//┃class	CHashCode
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃class	cUniCode
class cUniCode
{
public:
	static char m_pStrChar[1024];		//	간단한 텍스트 변역용
	static wchar_t m_pStrWChar[1024];	//	간단한 텍스트 변역용
	//	return value 
	//	0 : 잘못된 데이터가 입력되거나, 주어진 포인터가 잘못되었다.
	//	>0 : 변환된 사이즈가 전달된다.
	static char * UnicodeToAscii(wchar_t * _lpStr1,int opt = CP_ACP);
	static wchar_t * AsciiToUnicode(char * _lpStr1,int opt = CP_ACP);
	static int UnicodeToAscii(wchar_t * _lpStr1,char * _lpStr2);
	static int AsciiToUnicode(char * _lpStr1,wchar_t * _lpStr2);
};
//┃class	cUniCode
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃class	cFILEUTIL
#ifdef __USE_FILE_UTIL
class cFileUtil
{
public:
	static W2CHAR	m_strTmpStr[1024];
	static W2CHAR* exportExt(W2CHAR *str);		//	확장자만 뽑아 낸다. - SFC
	static W2CHAR* exportFolderName(W2CHAR *str,W2CHAR *target=NULL);//	폴더 이름만 뽑아서, target에 담아준다. - SFC
	static W2CHAR* exportFileName(W2CHAR *str,W2CHAR *target=NULL);	//	화일 이름만 뽑아 낸다. - SFC
	static W2CHAR* exportName(W2CHAR *str,W2CHAR *target=NULL);		//	확장자를 제외한 화일 이름만 뽑아 낸다. - SFC
};
#endif
//┃class	cFILEUTIL
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

};