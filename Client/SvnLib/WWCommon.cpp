#include "WWCommon.h"
#include <stdio.h>

using namespace W2Lib;

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃class	CHashCode
W2ULONG CHashCode::GetHashCode(BYTE* _lpString)
{
	W2LONG	iResult=0,iXOR=0,iCH=0,i=0;

#ifndef _DEBUG	//	디버그 일때 NULL값이 들어오면 프로그램에 이상이 생기도록 놔두자.
	if(!_lpString)	return 0;
	if(!_lpString[0])	return 0;
#endif

	for (;_lpString[i];i++)
	{
		iCH		=	_lpString[i];

		if (iCH >= 'a' && iCH <= 'z')
			iCH	-=	('a'-'A');

		iResult	=	( ( iResult << 8 ) | iCH ) % 16777213UL; // 16777213UL
		iXOR		^=	iCH;
	}

	return iResult | (iXOR<<24);
}
//┃class	CHashCode
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃class	cUniCode
char cUniCode::m_pStrChar[1024];		//	간단한 텍스트 변역용
wchar_t cUniCode::m_pStrWChar[1024];	//	간단한 텍스트 변역용

char * cUniCode::UnicodeToAscii(wchar_t * _lpStr1,int opt)
{
	if(!_lpStr1)
		return 0;
	
	memset(m_pStrChar,0,1024);

	size_t size = wcslen(_lpStr1);
	if( !size )	
		return 0;
	
	size_t asize = WideCharToMultiByte( opt, 0, _lpStr1, size, NULL, 0, NULL, NULL );
	WideCharToMultiByte(opt,0,_lpStr1,size+1,m_pStrChar,asize,NULL,NULL);
//	size_t ret = wcstombs( m_pStrChar, _lpStr1, size ); 한글 짤린다. -_-;;
	return m_pStrChar;
}

wchar_t * cUniCode::AsciiToUnicode(char * _lpStr1,int opt)
{
	if(!_lpStr1)
		return 0;

	memset(m_pStrWChar,0,2048);

	size_t size = strlen(_lpStr1);
	if( ! size )
		return 0;

	size_t wsize = MultiByteToWideChar(opt, 0, _lpStr1, size, NULL, 0);
	wsize = MultiByteToWideChar(opt, 0, _lpStr1, size, m_pStrWChar, wsize);
//	mbstowcs( m_pStrWChar, _lpStr1, size );

	return m_pStrWChar;
}

int	cUniCode::UnicodeToAscii(wchar_t * _lpStr1,char * _lpStr2)
{
	if(!_lpStr1 || !_lpStr2)
		return 0;

	size_t size = wcslen(_lpStr1);

	if( !size )
		return 0;
	++size;	//	NULL이 들어갈 위치

	wcstombs( _lpStr2, _lpStr1, size );

	return (int)size;
}

int	cUniCode::AsciiToUnicode(char * _lpStr1,wchar_t * _lpStr2)
{
	if(!_lpStr1 || !_lpStr2)
		return 0;
	size_t size = strlen(_lpStr1);
	if( ! size )
		return 0;
	size = (size+1) * 2;

	mbstowcs( _lpStr2, _lpStr1, size );

	return (int)size;
}
//┃class	cUniCode
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃class	cFILEUTIL
#ifdef __USE_FILE_UTIL

W2CHAR	cFileUtil::m_strTmpStr[1024];

W2CHAR* 
cFileUtil::exportExt(W2CHAR *str)
{	int dest=0xffff;

	for (unsigned int i=0;i<W2STRLEN(str);i++)	if (str[i]=='.') dest=i+1;

	if (dest==0xffff) return NULL;

	return &str[dest];
}

W2CHAR* 
cFileUtil::exportFolderName(W2CHAR *str,W2CHAR *target)
{	
	if(!target)
	{
		W2STRCPY(m_strTmpStr,str);
		int size = (int)W2STRLEN(str)-1;

		for (int i=size;i>=0;i--)
		{
			if (str[i]=='\\') 
			{	m_strTmpStr[i]	=	0;
				return m_strTmpStr;
			}
		}
	}else
	{
		W2STRCPY(target,str);

		int size = (int)W2STRLEN(str)-1;

		for (int i=size;i>=0;i--)
		{
			if (str[i]=='\\') 
			{	target[i]	=	0;
				return target;
			}
		}
	}
	return NULL;
}

W2CHAR* 
cFileUtil::exportFileName(W2CHAR *str,W2CHAR *target)
{
	int dest=0;
	if(!target)
	{
		W2STRCPY(m_strTmpStr,str);

		for (UINT i=0;i<W2STRLEN(str);i++)	if (str[i]=='\\') dest=i+1;
		return &m_strTmpStr[dest];
	}else
	{
		W2STRCPY(target,str);

		for (UINT i=0;i<W2STRLEN(str);i++)	if (str[i]=='\\') dest=i+1;
		return &target[dest];
	}
}

W2CHAR* 
cFileUtil::exportName(W2CHAR *str,W2CHAR *target)
{
	int dest=0;
	if(!target)
	{
		W2STRCPY(m_strTmpStr,str);

		unsigned int i =0;
		for (i=0;i<W2STRLEN(str)	;i++)		if (str[i]=='\\') dest		=i+1;
		for (i=dest;i<W2STRLEN(str)	;i++)	if (str[i]=='.'	) m_strTmpStr[i]=NULL;

		return &m_strTmpStr[dest];
	}else
	{
		W2STRCPY(target,str);

		unsigned int i =0;
		for (i=0;i<W2STRLEN(str)	;i++)		if (str[i]=='\\') dest		=i+1;
		for (i=dest;i<W2STRLEN(str)	;i++)	if (str[i]=='.'	) target[i]=NULL;

		return &target[dest];
	}
}
#endif
//┃class	cFILEUTIL
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

