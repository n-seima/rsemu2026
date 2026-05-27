/**************************************************************************
문자열 분석 클래스와 배경 업로드 루틴 이벤트 업로드 루틴

  개요 :  SRPG 게임을 생각하고 만든 간단한 스크립터

작성일 : 98.06.13
***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cDECODER.h"	//	클래스 헤더화일..

const	char	*l_strSystemCode	="isSystemCode";
const	char	*l_strBeginCode		="isBeginCode";
const	char	*l_strCloseCode		="isCloseCode";
const	char	*l_strEndCode		="isEndCode";
const	char	*l_strValidCode		="isValidCode";
const	char	*l_strFinishCode	="isFinishCode";

static	int		l_isChar[256]		=	{100,};

cDECODER	tempDecoder;		//	종료시 버퍼를 비운다.

cDECODER::cDECODER()
{	
	m_pSTREAM		=	NULL;
	m_iBP			=	0;
	m_iFileLength	=	0;
	m_lpKeyWord		=	NULL;
	m_bIsUseComment	=	TRUE;

	if (l_isChar[0]	==	100)
	{
		for (int i=0;i<128;i++)
		{	l_isChar[i]		=	0;
			l_isChar[i+128]	=	1;

			if (i >= 'a' && i <= 'z'	) l_isChar[i]	=	1;
			if (i >= 'A' && i <= 'Z'	) l_isChar[i]	=	1;
			if (i == '_'				) l_isChar[i]	=	1;
			if (i >= '0' && i <= '9'	) l_isChar[i]	=	1;
		}
	}
}

cDECODER::~cDECODER()
{	Reset();
}


//	버퍼를 비운다.
void
cDECODER::Reset()
{	
	pKILL(m_pSTREAM);
	m_lpKeyWord	=	NULL;
	m_iBP		=	0;
	m_isDone	=	FALSE;
}

//	화일을 버퍼에 올린다.

BOOL
cDECODER::Upload(char *fn,cKEYWORD_ITEM *keyword)
{	cFILE	file;

	if (!file.Open( fn,"rb"))
		return MSGOUT("Error in cDECODER::Upload - can not find '%s' file",fn);

	Reset();

	m_lpKeyWord					=	keyword;
	m_iFileLength				=	file.Length+1;
	m_pSTREAM					=	new char [m_iFileLength];
	file.Read(m_pSTREAM,m_iFileLength-1);
	m_iFileLength--;
	m_pSTREAM[m_iFileLength]	=	0;
	m_iBP						=	0;
	file.Close();

	return TRUE;
}

BOOL
cDECODER::UploadBuffer(char *stream,cKEYWORD_ITEM *keyword)
{
	Reset();

	m_lpKeyWord		=	keyword;
	m_iFileLength	=	strlen(stream)+1;
	m_pSTREAM		=	new char [m_iFileLength];
	strcpy(m_pSTREAM,stream);

	return	TRUE;
}

//	글자냐?
inline BOOL
cDECODER::isChar(char ch)
{	return l_isChar[(BYTE)ch];
}

//	종료 코드인가?
inline BOOL
cDECODER::isCloseCode(char ch)
{	if (ch == '}')	return TRUE;
 
	return FALSE;
}

inline BOOL
cDECODER::isFinishCode(char ch)
{
 	if (ch == ';')	return TRUE;

	return	FALSE;
}

//	시작 코드 인가?
inline BOOL
cDECODER::isBeginCode(char ch)
{	if (ch == '{')	return TRUE;
 
	return FALSE;
}

//	시스템 코드인가?
inline BOOL
cDECODER::isSystemCode(char ch)
{	if (ch == '{')	return TRUE;
	if (ch == '}')	return TRUE;
	if (ch == ';')	return TRUE;

	return FALSE;
}

//
//	스킵해야되는 문자냐?
inline BOOL
cDECODER::isSkipChar(char ch)
{	if (ch == 8)	return TRUE;	//	탭
	if (ch == ' ')	return TRUE;	//	스페이스바

	return FALSE;
}	//	cDECODER::isSkipChar(char ch)

//	숫자인가?
inline BOOL
cDECODER::isNumber(char ch)
{	if (ch>='0' && ch<='9') return TRUE;

	return FALSE;
}

//	숫자를 찾는다.
inline BOOL
cDECODER::FindNumber()
{	while(1)
	{	if (isNumber(m_pSTREAM[m_iBP++]))
		{	m_iBP--;
			return TRUE;
		}

		TEST(m_iBP>=m_iFileLength,return cMSG::Error("cDECODER::FindNumber() - can not find number"));
	}

	return FALSE;
}

//	시작 코드를 찾는다.
BOOL
cDECODER::FindOpenCode()
{	while(1)
	{	if (isBeginCode(m_pSTREAM[m_iBP++])) return TRUE;

		TEST(m_iBP>=m_iFileLength,return cMSG::Error("cDECODER::FindBeginCode() - can not find begin code"));
	}

	return FALSE;
}

//	클로즈 코드를 찾는다.
BOOL
cDECODER::FindCloseCode()
{	while(1)
	{	if (isCloseCode(m_pSTREAM[m_iBP++]))	return TRUE;

		TEST(m_iBP>=m_iFileLength,return cMSG::Error("cDECODER::FindEndCode() - can not find end code"));
	}

	return FALSE;
}

//	종료 코드를 찾는다.
BOOL
cDECODER::FindFinishCode()
{	while(1)
	{	if (isFinishCode(m_pSTREAM[m_iBP++]))	return TRUE;

		TEST(m_iBP>=m_iFileLength,return cMSG::Error("cDECODER::FindEndCode() - can not find end code"));
	}

	return FALSE;
}

//
//	바로 다음 문자를 얻어 온다.
int
cDECODER::getNextChar()
{
	int	ch;

	while(1)
	{
		if (m_iBP>=m_iFileLength)	return	0;

		ch		=	m_pSTREAM[m_iBP++];

		if (ch  ==  '/' && m_bIsUseComment	)	//	주석이다. 원래는 //를 체크해야 하지만.. 구찮어.. --;;
		{	while(ch != 0x0a	)
			{	ch	=	m_pSTREAM[m_iBP++];
				if (m_iBP>=m_iFileLength) return FALSE;
			}
		}
		else
		if (isSkipChar(ch))	continue;
		else				break;
	}

	m_iBP--;

	return ch;
}	//	cDECODER::getNextChar()

/************************************************************************
char cDECODER::FindCharExact(char ch)
지정된 문자를 찾는다. 버퍼 포인터는 찾아낸 문자를 가르킨다.

char ch:
	찾아야 할 문자..

return VALUE	:
	없음
************************************************************************/

inline char
cDECODER::FindAnyCharExact(BOOL _bIsExceptStream)
{	int	ch;

	while(1)
	{
		if (m_iBP>=m_iFileLength)	return	0;

		ch		=	m_pSTREAM[m_iBP++];

		if (_bIsExceptStream)
		{
			if (ch == '"')
			{
				while(m_pSTREAM[m_iBP] != '"')
				{
					m_iBP++;

					if (m_iBP>=m_iFileLength) return 0;
				}
				m_iBP++;
			}
		}

		if (ch  ==  '/'&& m_bIsUseComment			)	//	주석이다. 원래는 //를 체크해야 하지만.. 구찮어.. --;;
		{	while(ch != 0x0a	)
			{	ch	=	m_pSTREAM[m_iBP++];
				if (m_iBP>=m_iFileLength) return 0;
			}
		}
		else
		if (isChar(ch))			break;
		else
		if (isSystemCode(ch))	break;
	}

	m_iBP--;

	return ch;
}

inline char
cDECODER::FindCharExact(char ch,BOOL _bIsExceptStream)
{	int getCh;

	while(1)
	{	getCh	=	FindAnyCharExact(_bIsExceptStream);
		if (getCh	== 0	) return 0;	//	끝났거나 없다.
		if (getCh	== ch	) break;	

		m_iBP	++;
	}

	return m_pSTREAM[m_iBP];
}

/************************************************************************
char cDECODER::FindCharOver(char ch)
지정된 문자를 찾는다. 버퍼 포인터는 바로 다음 문자를 가르킨다.

char ch:
	찾아야 할 문자..

return VALUE	:
	없음
************************************************************************/

BOOL
cDECODER::FindCharOver()
{	if (FindAnyCharExact()	== 0	) return FALSE;	//	끝났거나 없다.

	m_iBP	++;

	return TRUE;
}

BOOL
cDECODER::FindCharOver(char ch)
{	int getCh;

	while(1)
	{	getCh	=	m_pSTREAM[m_iBP++];

		if (ch	==	getCh		)	return TRUE;
		if (ch  ==  '/'&& m_bIsUseComment			)	//	주석이다. 원래는 //를 체크해야 하지만.. 구찮어.. --;;
		{	while(getCh != 0x0a	)
			{	getCh	=	m_pSTREAM[m_iBP++];
				if (m_iBP>=m_iFileLength) return FALSE;
			}
		}
		if (m_iBP>=m_iFileLength) return FALSE;
	}

	return FALSE;
}

/************************************************************************
BOOL cDECODER::GetStream(char *stream)
따옴표로 둘러 쌓인 문장을 읽어 들인다.

char  *stream:
	문자열을 저장할 버퍼

return VALUE	:
	없음
************************************************************************/

char *
cDECODER::GetStream()
{	int		count	=	0;
	int		ch		=	0;

	if	(!FindCharOver('"')) 
		return NULL;	//	문장 시작"

	while(1)
	{	ch	=	m_pSTREAM[m_iBP++];

		if (m_iBP	>=	m_iFileLength)	
		{	
			m_isDone	=	TRUE;
			break;
		}

		if (ch	==	'\\'		)	//	문장 넘어 가유~~
		{	
			ch	=	FindAnyCharExact();

			if	(ch == 0		)
				return NULL;

			continue;
		}

		if	(ch	==	'"'		)
			break;		//	스트림의 끝..

		if	(ch	==	9		)				//	탭 처리
		{	
			m_strStream[count++]	=	32;
			m_strStream[count++]	=	32;
			m_strStream[count++]	=	32;
			m_strStream[count++]	=	32;
		}
		else
			m_strStream[count++]	=	ch;

		if	(count >= dDECODER_MAX_STREAM-1)
			break;
	}

	if	(count == 0)
		return NULL;

	m_strStream[count]	=	NULL;

	return m_strStream;
}

char *
cDECODER::GetStream(char cEndSymbol,int _iEndSymbol2,BOOL _bIsAcceptZeroSize)
{
	int		count	=	0;
	char	ch		=	0;

	m_iMeetSymbol	=	-1;

	if (m_iBP	>=	m_iFileLength)	
		return	NULL;

	while(1)
	{
		ch	=	m_pSTREAM[m_iBP++];

		if (m_iBP	>=	m_iFileLength)	
		{
			m_isDone	=	TRUE;
			break;
		}

		if	(ch	==	cEndSymbol	)
		{
			m_iMeetSymbol	=	ch;
			break;		//	스트림의 끝..
		}

		if	(_iEndSymbol2	!=	-1	&&	ch	==	_iEndSymbol2	)
		{
			m_iMeetSymbol	=	ch;
			break;		//	스트림의 끝..
		}

		m_strStream[count++]	=	ch;

		if	(count >= dDECODER_MAX_STREAM-1)
			break;
	}

	if	(count == 0 )
	{
		if	(m_isDone || _bIsAcceptZeroSize == FALSE)
			return NULL;
	}

	m_strStream[count]	=	NULL;

	return m_strStream;
}

/************************************************************************
char *cDECODER::GetWord()
단어를 찾아서 버퍼에 올린다. 공백이나 엔딩 문자열을 받을때까지 읽어 들임..

return VALUE	:
	단어의 길이..
************************************************************************/

//
//	_cEndSymbol까지 얻어오기
char*
cDECODER::getLongWord(char _cEndSymbol)
{
	char	ch;
	int		count	=	0;

	while(1) // 공백을 만날때 까지..
	{	ch	=	m_pSTREAM[m_iBP];

		if (ch < 0)	//	한글이나 기타 2바이트 문자다.
		{	m_iBP++;
			m_strStream[count++]=	ch;
			m_strStream[count++]=	m_pSTREAM[m_iBP];
		}
		else
		{
			if (ch == _cEndSymbol)	break;

			m_strStream[count++]=	ch;
		}

		if (m_iBP	>=	m_iFileLength)	return (char*)l_strEndCode;

		m_iBP++;
	}

	if (count==0) return (char*)l_strValidCode;

	m_strStream[count]	=	NULL;
	
	m_iBP++;

	return m_strStream;
}	//	cDECODER::getLongWord(char _cEndSymbol)

char *
cDECODER::GetWord(BOOL _bIsExceptStream,char _cEndSymbol)
{
	int		count=0;
	int		ch;

	ch	=	FindAnyCharExact(_bIsExceptStream);

	if (ch==0)	return (char*)l_strEndCode;		//	화일이 끝났다.

	if(isBeginCode(ch)	)
	{	m_iBP++;
		return (char*)l_strBeginCode;
	}

	if (isCloseCode(ch)	)
	{	m_iBP++;
		return (char*)l_strCloseCode;
	}

	if (isFinishCode(ch))
	{	m_iBP++;
		return (char*)l_strFinishCode;
	}

	while(1) // 공백을 만날때 까지..
	{	
		ch	=	m_pSTREAM[m_iBP];
		

		if (ch < 0)	//	한글이나 기타 2바이트 문자다.
		{	m_iBP++;
			m_strWORD[count++]=	ch;
			m_strWORD[count++]=	m_pSTREAM[m_iBP];
		}
		else
		{
			if (_bIsExceptStream)
				if (ch == '"')
					break;

			if (_cEndSymbol	==	-1)
			{
				if (!isChar(ch))	break;
			}
			else
			{
				if (ch == _cEndSymbol || ch == 0)	break;
			}


			m_strWORD[count++]=	ch;
		}

		if (m_iBP	>=	m_iFileLength)	return (char*)l_strEndCode;

		m_iBP++;
	}

	if (count==0) return (char*)l_strValidCode;

	m_strWORD[count]	=	NULL;

	return m_strWORD;
}

char *
cDECODER::GetWordVerBC(BOOL _bIsExceptStream,char _cEndSymbol)
{
	int		count=0;
	int		ch;
	
	ch	=	FindAnyCharExact(_bIsExceptStream);
	
	if (ch==0)	return (char*)l_strEndCode;		//	화일이 끝났다.
	
	if(isBeginCode(ch)	)
	{	m_iBP++;
	return (char*)l_strBeginCode;
	}
	
	if (isCloseCode(ch)	)
	{	m_iBP++;
	return (char*)l_strCloseCode;
	}
	
	if (isFinishCode(ch))
	{	m_iBP++;
	return (char*)l_strFinishCode;
	}
	
	while(1) // 공백을 만날때 까지..
	{	
		ch	=	m_pSTREAM[m_iBP];
		
		if (ch == 32)		//스페이스는 넘긴다.ㅠ.ㅠ;
		{
			m_strWORD[count++]=	ch;
			m_strWORD[count++]=	m_pSTREAM[m_iBP];
			m_iBP++;
			continue;
		}
		
		if (ch < 0)	//	한글이나 기타 2바이트 문자다.
		{	m_iBP++;
			m_strWORD[count++]=	ch;
			m_strWORD[count++]=	m_pSTREAM[m_iBP];
		}
		else
		{
			if (_bIsExceptStream)
				if (ch == '"')
					break;
				
				if (_cEndSymbol	==	-1)
				{
					if (!isChar(ch))	break;
				}
				else
				{
					if (ch == _cEndSymbol || ch == 0)	break;
				}
				
				
				m_strWORD[count++]=	ch;
		}
		
		if (m_iBP	>=	m_iFileLength)	return (char*)l_strEndCode;
		
		m_iBP++;
	}
	
	if (count==0) return (char*)l_strValidCode;
	
	m_strWORD[count]	=	NULL;
	
	return m_strWORD;
}

/************************************************************************
BOOL cDECODER::FindWord(char *tmpword)
단어를 찾는다. 엔딩 코드 발견시 중지...

char *tmpword
	검색할 단어..	

return VALUE	:
	단어를 발견했을시  TRUE
************************************************************************/

BOOL
cDECODER::FindWord(char *cmpWord)
{	while(1)
	{	if (strcmp(GetWord(),cmpWord)==0)	return TRUE;

		if (m_iBP	>=	m_iFileLength	)	
			return 	ERRMSG("Error in FindWord(char *cmpWord) - can not find '%s' word",cmpWord);
	}

	return FALSE;
}


/************************************************************************
int cDECODER::GetNumber()
숫자를 찾아서 그 값을 리턴한다..

return VALUE	:
	찾아낸 숫자의 정수 값
************************************************************************/

int
cDECODER::GetNumber(BOOL *_bIsSuccess)
{	int		count=0;
	int		ch=0,minus=1;
	char	strNumber[16];

	while(1)
	{	ch	=	m_pSTREAM[m_iBP++];

		if (m_iBP>m_iFileLength)
		{
			*_bIsSuccess	=	FALSE;
//			ERRMSG("Error in cDECODER::GetNumber() - can not find number");
			return dCODE_VALID_NUMBER;
		}

		if (isNumber(ch)	)	
		{	m_iBP--;
			break;
		}

		if (isChar(ch)		)
		{	
			*_bIsSuccess	=	FALSE;
			return dCODE_VALID_NUMBER;
		}
	}

	if (m_iBP	>	0)	if (m_pSTREAM[m_iBP-1]=='-')	minus	=	-1;

	while(1) // 숫자가 아닌 문자를 만날때 까지..
	{	ch	=	m_pSTREAM[m_iBP++];

		if (m_iBP >= m_iFileLength)	break;

		if (isNumber(ch))	strNumber[count++]=ch;
		else
		{
			m_iBP--;
			break;
		}

		if (count > 12)
		{
			*_bIsSuccess	=	FALSE;
//			ERRMSG("Error in cDECODER::GetNumber() - so big number..");
			return dCODE_VALID_NUMBER;
		}
	}

	if (count==0)
	{
		*_bIsSuccess	=	FALSE;

		return dCODE_VALID_NUMBER;
	}

	*_bIsSuccess	=	TRUE;

	strNumber[count]=NULL;

	return (atoi(strNumber)*minus);
}

//	등록된 코드 분석..

DWORD
cDECODER::AnalCode(char *cmpKeyword)
{	int		count=0;
	DWORD	code	=	0;

	while(code	!=	dCODE_END)
	{	code	=	m_lpKeyWord[count].m_dwCode;

		if (strcmp(cmpKeyword,m_lpKeyWord[count].m_strKeyword)==0) return code;

		count++;
	}

	if (strcmp(cmpKeyword,l_strFinishCode	)==0) return dCODE_FINISH;
	if (strcmp(cmpKeyword,l_strBeginCode	)==0) return dCODE_OPEN;
	if (strcmp(cmpKeyword,l_strCloseCode	)==0) return dCODE_CLOSE;
	if (strcmp(cmpKeyword,l_strEndCode		)==0) return dCODE_END;
	if (strcmp(cmpKeyword,l_strValidCode	)==0) return dCODE_ERROR;

	return	dCODE_UNDEFINE;
}

DWORD
cDECODER::AnalCode(cKEYWORD_ITEM *keyword,char *cmpKeyword)
{	int		count=0;
	DWORD	code	=	0;

	while(code	!=	dCODE_END)
	{	code	=	keyword[count].m_dwCode;

		if (strcmp(cmpKeyword,keyword[count].m_strKeyword)==0) return code;

		count++;
	}

	if (strcmp(cmpKeyword,l_strFinishCode	)==0) return dCODE_FINISH;
	if (strcmp(cmpKeyword,l_strBeginCode	)==0) return dCODE_OPEN;
	if (strcmp(cmpKeyword,l_strCloseCode	)==0) return dCODE_CLOSE;
	if (strcmp(cmpKeyword,l_strEndCode		)==0) return dCODE_END;
	if (strcmp(cmpKeyword,l_strValidCode	)==0) return dCODE_ERROR;

	return	dCODE_UNDEFINE;
}

char*
cDECODER::getCSVWord(BOOL *_lpbIsEnd)
{
	return doCSVParsing(_lpbIsEnd);
}

int
cDECODER::getCSVNumber(BOOL *_lpbIsEnd)
{
	char	*lpstrText	=	doCSVParsing(_lpbIsEnd);

	if (!lpstrText)
		return	NULL;

	return	atoi(lpstrText);
}

int
cDECODER::getCSVNumberList(int *_lpiList,BOOL *_lpbIsEnd)
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
cDECODER::doCSVParsing(BOOL *_lpbIsEnd)
{
	char	cChar;
	int		iCount				=	0;
	BOOL	bIsQuotationCase	=	FALSE;
	BOOL	bIsEvenTimeQuotation=	FALSE;
	char	cEndSymbol			=	',';

	if (m_pSTREAM[m_iBP]	==	'"')
	{
		m_iBP++;
		bIsQuotationCase	=	TRUE;
	}

	if	(_lpbIsEnd)
		*_lpbIsEnd	=	FALSE;

	while(1) // 공백을 만날때 까지..
	{	
		if	(m_iBP	>=	m_iFileLength)
		{
			if	(_lpbIsEnd)
				*_lpbIsEnd	=	TRUE;

			break;
		}

		cChar	=	m_pSTREAM[m_iBP];

//		if (Is2ByteChar(cChar))	//	한글이나 기타 2바이트 문자다.
		if (cChar < 0)	//	한글이나 기타 2바이트 문자다.
		{
			m_iBP++;
			m_strStream[iCount++]	=	cChar;
			m_strStream[iCount++]	=	m_pSTREAM[m_iBP];

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

					if (m_pSTREAM[m_iBP] == '\n')
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

				if (m_pSTREAM[m_iBP] == '\n')
					m_iBP++;

				break;
			}
		}

		if (bIsEvenTimeQuotation	==	FALSE)
			m_strStream[iCount++]	=	cChar;

		m_iBP++;
	}

	if	(iCount == 0)
		return	NULL;

	m_strStream[iCount]	=	NULL;

	return m_strStream;
}