/**************************************************************************
문자열 분석 클래스와 배경 업로드 루틴 이벤트 업로드 루틴

  개요 :  SRPG 게임을 생각하고 만든 간단한 스크립터

작성일 : 98.06.13
***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "CDecoder.h"	//	클래스 헤더화일..
#include "cExceptionHandler.H"
#include "cTEXT.H"

static	int		l_isChar[256]		=	{100,};

CDecoder	tempDecoder;		//	종료시 버퍼를 비운다.

CDecoder::CDecoder()
{
	m_pBuffer		=	NULL;
	m_iBP			=	0;
	m_iFileLength	=	0;
	m_lpKeyWord		=	NULL;

	if (l_isChar[0]	==	100)
	{
		for (int i=0;i<128;i++)
		{
			l_isChar[i]		=	0;
			l_isChar[i+128]	=	1;

			if (i >= 'a' && i <= 'z'	)
				l_isChar[i]	=	1;
			if (i >= 'A' && i <= 'Z'	)
				l_isChar[i]	=	1;
			if (i == '_'				)
				l_isChar[i]	=	1;
			if (i >= '0' && i <= '9'	)
				l_isChar[i]	=	1;
		}
	}
}

CDecoder::~CDecoder()
{
	Reset();
}


//	버퍼를 비운다.
void
CDecoder::Reset()
{
	_Free(m_pBuffer);
	m_lpKeyWord	=	NULL;
	m_iBP		=	0;
	m_isDone	=	FALSE;
}

//	화일을 버퍼에 올린다.

BOOL
CDecoder::Upload(char *fn,CDecoderKeyword *keyword)
{
	cFILE	file;

	if (!file.Open( fn,"rb"))
	{
		g_eh.addStaticLog("Error in CDecoder::Upload - can not find '%s' file",fn);
		return	FALSE;
	}

	Reset();

	m_lpKeyWord					=	keyword;
	m_iFileLength				=	file.Length+1;
	m_pBuffer					=	_New(char,m_iFileLength,NULL,"CDecoder::m_pBuffer");
	memset(m_pBuffer,0,m_iFileLength);
	file.Read(m_pBuffer,m_iFileLength-1);
	m_iFileLength--;
	m_pBuffer[m_iFileLength]	=	0;
	file.Close();

	return TRUE;
}

BOOL
CDecoder::UploadBuffer(char *stream,CDecoderKeyword *keyword)
{
	Reset();

	m_lpKeyWord		=	keyword;
	m_iFileLength	=	strlen(stream)+1;
	m_pBuffer		=	_New(char,m_iFileLength,NULL,"CDecoder::m_pBuffer");
	strcpy(m_pBuffer,stream);

	return	TRUE;
}

//	글자냐?
inline BOOL
CDecoder::isChar(char cChar)
{
	return l_isChar[(BYTE)cChar];
}

//	종료 코드인가?
inline BOOL
CDecoder::isCloseCode(char cChar)
{
	if (cChar == '}')
		return TRUE;
 
	return FALSE;
}

inline BOOL
CDecoder::isFinishCode(char cChar)
{
 	if (cChar == ';')
		return TRUE;

	return	FALSE;
}

//	시작 코드 인가?
inline BOOL
CDecoder::isBeginCode(char cChar)
{
	if (cChar == '{')
		return TRUE;
 
	return FALSE;
}

//	시스템 코드인가?
inline BOOL
CDecoder::isSystemCode(char cChar)
{
	if (cChar == '{')
		return TRUE;
	if (cChar == '}')
		return TRUE;
	if (cChar == ';')
		return TRUE;

	return FALSE;
}

//
//	스킵해야되는 문자냐?
inline BOOL
CDecoder::isSkipChar(char cChar)
{
	if (cChar == 8)
		return TRUE;	//	탭
	if (cChar == ' ')
		return TRUE;	//	스페이스바

	return FALSE;
}	//	CDecoder::isSkipChar(char cChar)

//	숫자인가?
inline BOOL
CDecoder::isNumber(char cChar)
{	
	if ((cChar>='0' && cChar<='9' )|| cChar== '-')
		return TRUE;

	return FALSE;
}

//	숫자를 찾는다.
BOOL
CDecoder::findNumber()
{	
	while(1)
	{
		if (isNumber(m_pBuffer[m_iBP++]))
		{
			m_iBP--;

			return TRUE;
		}

		TEST(m_iBP>=m_iFileLength,return cMSG::Error("CDecoder::findNumber() - can not find number"));
	}

	return FALSE;
}

//	시작 코드를 찾는다.
BOOL
CDecoder::findOpenCode()
{
	while(1)
	{
		if (isBeginCode(m_pBuffer[m_iBP++]))
			return TRUE;

		TEST(m_iBP>=m_iFileLength,return cMSG::Error("CDecoder::findBeginCode() - can not find begin code"));
	}

	return FALSE;
}

BOOL
CDecoder::increaseBP(int _iBP)
{
	m_iBP	+=	_iBP;

	return	TRUE;
}

char
CDecoder::getFirstChar()
{
	return m_pBuffer[m_iBP];
}

//	클로즈 코드를 찾는다.
BOOL
CDecoder::findCloseCode()
{
	while(1)
	{
		if (isCloseCode(m_pBuffer[m_iBP++]))
			return TRUE;

		TEST(m_iBP>=m_iFileLength,return cMSG::Error("CDecoder::findEndCode() - can not find end code"));
	}

	return FALSE;
}

//	종료 코드를 찾는다.
BOOL
CDecoder::findFinishCode()
{
	while(1)
	{	
		if (isFinishCode(m_pBuffer[m_iBP++]))
			return TRUE;

		TEST(m_iBP>=m_iFileLength,return cMSG::Error("CDecoder::findEndCode() - can not find end code"));
	}

	return FALSE;
}

//
//	바로 다음 문자를 얻어 온다.
int
CDecoder::getNextChar()
{
	int	cChar;

	while(1)
	{
		if (m_iBP>=m_iFileLength)
			return	0;

		cChar		=	m_pBuffer[m_iBP++];

		if (cChar  ==  '/'			)	//	주석이다. 원래는 //를 체크해야 하지만.. 구찮어.. --;;
		{	
			while(cChar != 0x0a	)
			{
				if (m_iBP	>=	m_iFileLength)
					return FALSE;

				cChar	=	m_pBuffer[m_iBP++];
			}
		}
		else
		if (isSkipChar(cChar))
			continue;
		else
			break;
	}

	m_iBP--;

	return cChar;
}

/************************************************************************
char CDecoder::findCharExact(char cChar)
지정된 문자를 찾는다. 버퍼 포인터는 찾아낸 문자를 가르킨다.

char cChar:
	찾아야 할 문자..

return VALUE	:
	없음
************************************************************************/

char
CDecoder::findAnyCharExact(BOOL _bIsExceptStream)
{
	int	cChar;

	while(1)
	{
		if	(m_iBP>=m_iFileLength)
			return	0;

		cChar		=	m_pBuffer[m_iBP++];

		if	(_bIsExceptStream)
		{
			if	(cChar == '"')
			{
				while(m_pBuffer[m_iBP] != '"')
				{
					m_iBP++;

					if	(m_iBP>=m_iFileLength) 
						return 0;
				}
				m_iBP++;
			}
		}

		if	(cChar  ==  '/'			)	//	주석이다. 원래는 //를 체크해야 하지만.. 구찮어.. --;;
		{
			while(cChar != 0x0a	)
			{
				if	(m_iBP>=m_iFileLength)
					return 0;

				cChar	=	m_pBuffer[m_iBP++];
			}
		}
		else
		if	(isChar(cChar))
			break;
		else
		if	(isSystemCode(cChar))
			break;
	}

	m_iBP--;

	return cChar;
}

char
CDecoder::findCharExact(char cChar,BOOL _bIsExceptStream)
{
	int getCh;

	while(1)
	{
		getCh	=	m_pBuffer[m_iBP];//findAnyCharExact(_bIsExceptStream);

		if (getCh	== 0	)
			return 0;	//	끝났거나 없다.
		if (getCh	== cChar	)
			break;	

		m_iBP	++;
	}

	return m_pBuffer[m_iBP];
}

/************************************************************************
char CDecoder::findCharOver(char cChar)
지정된 문자를 찾는다. 버퍼 포인터는 바로 다음 문자를 가르킨다.

char cChar:
	찾아야 할 문자..

return VALUE	:
	없음
************************************************************************/

BOOL
CDecoder::findCharOver()
{	
	if	(findAnyCharExact()	== 0	) 
		return FALSE;	//	끝났거나 없다.

	m_iBP	++;

	return TRUE;
}

BOOL
CDecoder::findCharOver(char cChar)
{
	int getCh;

	while(1)
	{
		getCh	=	m_pBuffer[m_iBP++];

		if (getCh	==	cChar	)
			return TRUE;
		if (getCh	==	'/'	)	//	주석이다. 원래는 //를 체크해야 하지만.. 구찮어.. --;;
		{	
			while(getCh != 0x0a	)
			{	
				if (m_iBP>=m_iFileLength)
					return FALSE;

				getCh	=	m_pBuffer[m_iBP++];
			}
		}

		if (m_iBP>=m_iFileLength)
			return FALSE;
	}

	return FALSE;
}

/************************************************************************
BOOL CDecoder::getStream(char *stream)
따옴표로 둘러 쌓인 문장을 읽어 들인다.

char  *stream:
	문자열을 저장할 버퍼

return VALUE	:
	없음
************************************************************************/

char *
CDecoder::getStream()
{
	int		iCount		=	0;
	int		cChar		=	0;
	BOOL	bIsHangul	=	FALSE;

	if (!findCharOver('"'))
		return "stream not Found!!";	//	문장 시작"

	while(1)
	{
		if (m_iBP	>=	m_iFileLength)	
		{	
			m_isDone	=	TRUE;
			break;
		}

		cChar	=	m_pBuffer[m_iBP++];

		if (bIsHangul	==	FALSE)
		{
			if (cChar	==	'\\'		)	//	문장 넘어 가유~~
			{	
				if (m_pBuffer[m_iBP] != 'n')
				{
					cChar	=	findAnyCharExact();

					if (cChar == 0		)
						return (char *)dDC_WORD_CLOSE;

					continue;
				}
				else
				{
					cChar	=	10;
					m_iBP++;
				}
			}

			if (cChar	==	'"'		)
				break;		//	스트림의 끝..

			if (cChar	==	9		)				//	탭 처리
			{
				m_strStream[iCount++]	=	32;
				m_strStream[iCount++]	=	32;
				m_strStream[iCount++]	=	32;
				m_strStream[iCount++]	=	32;
			}
			else
				m_strStream[iCount++]	=	cChar;
		}
		else
			m_strStream[iCount++]	=	cChar;

		if (bIsHangul	==	FALSE)
		{
			if (Is2ByteChar(cChar))
				bIsHangul	=	TRUE;
		}
		else
			bIsHangul	=	FALSE;

		if (iCount >= dDECODER_MAX_STREAM-1)
			break;
	}

	if (iCount == 0)	
		return (char*)dDC_WORD_VALID;

	m_strStream[iCount]	=	NULL;

	return m_strStream;
}

char *
CDecoder::getStream(char _cEndSymbol)
{
	int		iCount	=	0;
	char	cChar		=	0;

	while(1)
	{	
		if (m_iBP	>=	m_iFileLength)	
		{	
			m_isDone	=	TRUE;
			break;
		}

		cChar			=	m_pBuffer[m_iBP++];

		if (Is2ByteChar(cChar))	//	한글이나 기타 2바이트 문자다.
		{
			m_strStream[iCount++]=	cChar;
			m_strStream[iCount++]=	m_pBuffer[m_iBP];
			m_iBP++;
		}
		else
		{
			if (cChar == _cEndSymbol)
				break;

			m_strStream[iCount++]=	cChar;
		}

		if (iCount >= dDECODER_MAX_STREAM-1)
			break;
	}

	if (iCount == 0)
		return NULL;

	m_strStream[iCount]	=	NULL;

	return m_strStream;
}

char *
CDecoder::readStream(char _cCheckSymbol)
{
	int		iCount		=	0;
	int		cChar		=	0;
	BOOL	bIsHangul	=	FALSE;

	if (!findCharOver(_cCheckSymbol))
		return "stream not Found!!";	//	문장 시작"

	while(1)
	{
		if (m_iBP	>=	m_iFileLength)	
		{	
			m_isDone	=	TRUE;
			break;
		}

		cChar	=	m_pBuffer[m_iBP++];

		if (bIsHangul	==	FALSE)
		{
			if (cChar	==	'\\'		)	//	문장 넘어 가유~~
			{	
				if (m_pBuffer[m_iBP] != 'n')
				{
					cChar	=	findAnyCharExact();

					if (cChar == 0		)
						return (char *)dDC_WORD_CLOSE;

					continue;
				}
				else
				{
					cChar	=	10;
					m_iBP++;
				}
			}

			if (cChar	==	_cCheckSymbol	)
				break;		//	스트림의 끝..

			if (cChar	==	9		)				//	탭 처리
			{
				m_strStream[iCount++]	=	32;
				m_strStream[iCount++]	=	32;
				m_strStream[iCount++]	=	32;
				m_strStream[iCount++]	=	32;
			}
			else
				m_strStream[iCount++]	=	cChar;
		}
		else
			m_strStream[iCount++]	=	cChar;

		if (bIsHangul	==	FALSE)
		{
			if (Is2ByteChar(cChar))
				bIsHangul	=	TRUE;
		}
		else
			bIsHangul	=	FALSE;

		if (iCount >= dDECODER_MAX_STREAM-1)
			break;
	}

	if (iCount == 0)	
		return (char*)dDC_WORD_VALID;

	m_strStream[iCount]	=	NULL;

	return m_strStream;
}

/************************************************************************
char *CDecoder::getWord()
단어를 찾아서 버퍼에 올린다. 공백이나 엔딩 문자열을 받을때까지 읽어 들임..

return VALUE	:
	단어의 길이..
************************************************************************/

//
//	_cCheckSymbol까지 얻어오기
char*
CDecoder::getLongWord(char _cCheckSymbol)
{
	char	cChar;
	int		iCount	=	0;

	while(1) // 공백을 만날때 까지..
	{	
		if (m_iBP	>=	m_iFileLength)
		{
			if (iCount == 0)
				return (char*)dDC_WORD_END;

			break;
		}

		cChar	=	m_pBuffer[m_iBP];

		if (Is2ByteChar(cChar))	//	한글이나 기타 2바이트 문자다.
		{
			m_iBP++;
			m_strStream[iCount++]=	cChar;
			m_strStream[iCount++]=	m_pBuffer[m_iBP];
		}
		else
		{
			if (cChar == _cCheckSymbol)
				break;

			m_strStream[iCount++]=	cChar;
		}

		m_iBP++;
	}

	if (iCount	==	0)
		return (char*)dDC_WORD_VALID;

	m_strStream[iCount]	=	NULL;

	return m_strStream;
}	//	CDecoder::getLongWord(char _cCheckSymbol)

char *
CDecoder::getWord(BOOL _bIsExceptStream,char _cCheckSymbol)
{
	int		iCount=0;
	int		cChar;

	cChar	=	findAnyCharExact(_bIsExceptStream);

	if	(cChar==0)
		return (char*)dDC_WORD_END;		//	화일이 끝났다.

	if	(isBeginCode(cChar)	)
	{
		m_iBP++;
		return (char*)dDC_WORD_OPEN;
	}

	if	(isCloseCode(cChar)	)
	{
		m_iBP++;
		return (char*)dDC_WORD_CLOSE;
	}

	if	(isFinishCode(cChar))
	{
		m_iBP++;
		return (char*)dDC_WORD_FINISH;
	}

	while(1) // 공백을 만날때 까지..
	{
		if (m_iBP	>=	m_iFileLength)
		{
			if (iCount == 0)
				return (char*)dDC_WORD_END;

			break;
		}

		cChar	=	m_pBuffer[m_iBP];

		if (Is2ByteChar(cChar))	//	한글이나 기타 2바이트 문자다.
		{
			m_iBP++;
			m_strWORD[iCount++]=	cChar;
			m_strWORD[iCount++]=	m_pBuffer[m_iBP];
		}
		else
		{
			if (_bIsExceptStream)
				if (cChar == '"')
					break;

			if (_cCheckSymbol	==	-1)
			{
				if (!isChar(cChar))
					break;
			}
			else
			{
				if (cChar == _cCheckSymbol || cChar == 0)
					break;
			}

			m_strWORD[iCount++]=	cChar;
		}

		m_iBP++;
	}

	if (iCount==0)
		return (char*)dDC_WORD_VALID;

	m_strWORD[iCount]	=	NULL;

	return m_strWORD;
}

/************************************************************************
BOOL CDecoder::findWord(char *tmpword)
단어를 찾는다. 엔딩 코드 발견시 중지...

char *tmpword
	검색할 단어..	

return VALUE	:
	단어를 발견했을시  TRUE
************************************************************************/

BOOL
CDecoder::findWord(char *cmpWord)
{
	while(1)
	{
		if (strcmp(getWord(),cmpWord)==0)
			return TRUE;

		if (m_iBP	>=	m_iFileLength	)	
			return 	ERRMSG("Error in findWord(char *cmpWord) - can not find '%s' word",cmpWord);
	}

	return FALSE;
}


/************************************************************************
int CDecoder::getNumber()
숫자를 찾아서 그 값을 리턴한다..

return VALUE	:
	찾아낸 숫자의 정수 값
************************************************************************/

int
CDecoder::getNumber(int	*_lpiResult,BOOL _bIsCheckFinishCode)
{
	int		iCount=0;
	int		cChar=0,minus=1;
	char	strNumber[16];

	if	(_lpiResult)
		*_lpiResult	=	TRUE;

	cChar	=	findAnyCharExact();

	if	(cChar	==	0)
	{
		if	(_lpiResult)
			*_lpiResult	=	FALSE;

		return dCODE_VALID_NUMBER;
	}


	while(1)
	{	
		if	(m_iBP>m_iFileLength)
		{
			if	(_lpiResult)
				*_lpiResult	=	FALSE;

			return dCODE_VALID_NUMBER;
		}

		cChar	=	m_pBuffer[m_iBP++];

		if	(_bIsCheckFinishCode && isFinishCode(cChar))
		{
			m_iBP++;

			if	(_lpiResult)
				*_lpiResult	=	eGNR_IS_FINISH_CODE;

			return dCODE_VALID_NUMBER;
		}
		
		if	(isNumber(cChar)	)	
		{	
			m_iBP--;
			break;
		}

		if	(isChar(cChar)		)
		{
			if	(_lpiResult)
				*_lpiResult	=	FALSE;

			return dCODE_VALID_NUMBER;
		}
	}

	if	(m_iBP	>	0)
		if	(m_pBuffer[m_iBP-1]=='-')
			minus	=	-1;

	while(1) // 숫자가 아닌 문자를 만날때 까지..
	{
		if	(m_iBP >= m_iFileLength)
			break;

		cChar	=	m_pBuffer[m_iBP++];

		if	(isNumber(cChar))
			strNumber[iCount++]=cChar;
		else
		{
			m_iBP--;
			break;
		}

		if	(iCount >= 16)
		{
			if	(_lpiResult)
				*_lpiResult	=	FALSE;

			return dCODE_VALID_NUMBER;
		}
	}

	if	(iCount==0)
	{
		if	(_lpiResult)
			*_lpiResult	=	FALSE;

		return dCODE_VALID_NUMBER;
	}

	strNumber[iCount]=NULL;

	return (atoi(strNumber)*minus);
}

//	등록된 코드 분석..

DWORD
CDecoder::analCode(char *cmpKeyword)
{	
	int		iCount	=	0;
	DWORD	code	=	0;

	while(m_lpKeyWord && code != dCODE_END)
	{
		code	=	m_lpKeyWord[iCount].m_dwCode;

		if (strcmp(cmpKeyword,m_lpKeyWord[iCount].m_strKeyword)==0)
			return code;

		iCount++;
	}

	if (strcmp(cmpKeyword,dDC_WORD_FINISH	)==0)
		return dCODE_FINISH;
	if (strcmp(cmpKeyword,dDC_WORD_OPEN	)==0)
		return dCODE_OPEN;
	if (strcmp(cmpKeyword,dDC_WORD_CLOSE	)==0)
		return dCODE_CLOSE;
	if (strcmp(cmpKeyword,dDC_WORD_END		)==0)
		return dCODE_END;
	if (strcmp(cmpKeyword,dDC_WORD_VALID	)==0)
		return dCODE_ERROR;

	return	dCODE_UNDEFINE;
}

DWORD
CDecoder::AnalCode(CDecoderKeyword *keyword,char *cmpKeyword)
{
	int		iCount=0;
	DWORD	code	=	0;

	while(code	!=	dCODE_END)
	{	
		code	=	keyword[iCount].m_dwCode;

		if	(strcmp(cmpKeyword,keyword[iCount].m_strKeyword)==0)
			return code;

		iCount++;
	}

	if (strcmp(cmpKeyword,dDC_WORD_FINISH	)==0)
		return dCODE_FINISH;
	if (strcmp(cmpKeyword,dDC_WORD_OPEN	)==0) 
		return dCODE_OPEN;
	if (strcmp(cmpKeyword,dDC_WORD_CLOSE	)==0) 
		return dCODE_CLOSE;
	if (strcmp(cmpKeyword,dDC_WORD_END		)==0) 
		return dCODE_END;
	if (strcmp(cmpKeyword,dDC_WORD_VALID	)==0)
		return dCODE_ERROR;

	return	dCODE_UNDEFINE;
}


char*
CDecoder::getCSVWord(BOOL *_lpbIsEnd)
{
	return doCSVParsing(_lpbIsEnd);
}

int
CDecoder::getCSVNumber(BOOL *_lpbIsEnd)
{
	char	*lpstrText	=	doCSVParsing(_lpbIsEnd);

	if (!lpstrText)
		return	NULL;

	return	atoi(lpstrText);
}

int
CDecoder::getCSVNumberList(int *_lpiList,BOOL *_lpbIsEnd)
{
	char	*lpstrText	=	doCSVParsing(_lpbIsEnd);

	if (!lpstrText || *_lpbIsEnd)
		return	0;
	
	int		iLength		=	strlen(lpstrText);
	int		iBufferPoint=	0,iNumberPoint	=	0;
	char	strNumber[10];
	BOOL	bIsLoop		=	TRUE;

	for (int iCount=0;bIsLoop;iCount++)
	{
		while(1)
		{
			if (isNumber(lpstrText[iBufferPoint]))
				break;

			if (lpstrText[iBufferPoint]	==	NULL)
				return	iCount;

			iBufferPoint++;
		}

		for (iNumberPoint=0;;)
		{
			strNumber[iNumberPoint]	=	lpstrText[iBufferPoint++];

			if (strNumber[iNumberPoint]	==	NULL)
				bIsLoop	=	FALSE;

			if (!isNumber(strNumber[iNumberPoint]))
			{
				strNumber[iNumberPoint]	=	NULL;
				break;
			}

			iNumberPoint++;
		}

		_lpiList[iCount]	=	atoi(strNumber);
	}

	return	iCount;
}

char*
CDecoder::doCSVParsing(BOOL *_lpbIsEnd)
{
	char	cChar;
	int		iCount				=	0;
	BOOL	bIsQuotationCase	=	FALSE;
	BOOL	bIsEvenTimeQuotation=	FALSE;
	char	cEndSymbol			=	',';

	if (m_pBuffer[m_iBP]	==	'"')
	{
		m_iBP++;
		bIsQuotationCase	=	TRUE;
	}

	while(1) // 공백을 만날때 까지..
	{	
		if (m_iBP	>=	m_iFileLength)
			break;

		cChar	=	m_pBuffer[m_iBP];

		if (Is2ByteChar(cChar))	//	한글이나 기타 2바이트 문자다.
		{
			m_iBP++;
			m_strStream[iCount++]	=	cChar;
			m_strStream[iCount++]	=	m_pBuffer[m_iBP];

			m_iBP++;
			continue;
		}

		if (bIsQuotationCase)
		{
			if (cChar	==	'"')
				bIsEvenTimeQuotation	=	1-bIsEvenTimeQuotation;

			if (bIsEvenTimeQuotation)
			{
				if (cChar == ',' ||	cChar == '\n'	||	cChar == '\r')
				{
					m_iBP++;

					if (m_pBuffer[m_iBP] == '\n')
						m_iBP++;

					break;
				}
			}
		}
		else
		{
			if (cChar == ',' ||	cChar == '\n'	||	cChar == '\r')
			{
				m_iBP++;

				if (m_pBuffer[m_iBP] == '\n')
					m_iBP++;

				break;
			}
		}

		if (bIsEvenTimeQuotation	==	FALSE)
			m_strStream[iCount++]	=	cChar;

		m_iBP++;
	}

	if (iCount == 0)
	{
		if (_lpbIsEnd)
			*_lpbIsEnd	=	TRUE;

		return	NULL;
	}

	m_strStream[iCount]	=	NULL;

	return m_strStream;
}