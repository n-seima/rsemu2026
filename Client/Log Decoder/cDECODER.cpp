/**************************************************************************
문자열 분석 클래스와 배경 업로드 루틴 이벤트 업로드 루틴

  개요 :  SRPG 게임을 생각하고 만든 간단한 스크립터

작성일 : 98.06.13
***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cDECODER.h"	//	클래스 헤더화일..
#include "cMESSAGE.h"	//	클래스 헤더화일..

const	char	*l_strSystemCode	="isSystemCode";
const	char	*l_strBeginCode		="isBeginCode";
const	char	*l_strCloseCode		="isCloseCode";
const	char	*l_strEndCode		="isEndCode";
const	char	*l_strValidCode		="isValidCode";
const	char	*l_strFinishCode	="isFinishCode";

const	BOOL	c_isChar[256]		=	
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
	1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,0,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1
};


cDECODER::cDECODER()
{	m_pStream		=	NULL;
	m_lpKeyWord		=	NULL;
	m_iBP			=	0;
	m_iFileLength	=	0;
	m_isActive		=	FALSE;
}

cDECODER::~cDECODER()
{	reset();
}


//	버퍼를 비운다.
void
cDECODER::reset()
{	pKILL(m_pStream);
	m_lpKeyWord	=	NULL;
	m_iBP		=	0;
	m_isActive	=	FALSE;
}

//	화일을 버퍼에 올린다.

BOOL
cDECODER::upload(char *fn,cKEYWORD_ITEM *keyword)
{	cFILE	file;

	if (!file.Open( fn,"rb"))
		return FALSE;

	reset();

	m_lpKeyWord					=	keyword;
	m_iFileLength				=	cFILE::Size(fn);
	m_pStream					=	new char [m_iFileLength+10];
	file.Read(m_pStream,m_iFileLength);
	m_iFileLength--;
	m_pStream[m_iFileLength]	=	0;
	file.Close();
	m_isActive					=	TRUE;

	return TRUE;
}

//	글자냐?
inline BOOL
cDECODER::isChar(char ch)
{	return c_isChar[(BYTE)ch];
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

//	숫자인가?
inline BOOL
cDECODER::isNumber(char ch)
{	if (ch>='0' && ch<='9') return TRUE;

	return FALSE;
}

//	숫자를 찾는다.
inline BOOL
cDECODER::findNumber()
{	while(1)
	{	if (isNumber(m_pStream[m_iBP++]))
		{	m_iBP--;
			return TRUE;
		}

	}

	return FALSE;
}

//	시작 코드를 찾는다.
BOOL
cDECODER::findOpenCode()
{	while(1)
	{	if (isBeginCode(m_pStream[m_iBP++])) return TRUE;

	}

	return FALSE;
}

//	클로즈 코드를 찾는다.
BOOL
cDECODER::findCloseCode()
{	while(1)
	{	if (isCloseCode(m_pStream[m_iBP++]))	return TRUE;

	}

	return FALSE;
}

//	종료 코드를 찾는다.
BOOL
cDECODER::findFinishCode()
{	while(1)
	{	if (isFinishCode(m_pStream[m_iBP++]))	return TRUE;

	}

	return FALSE;
}

/************************************************************************
char cDECODER::findCharExact(char ch)
지정된 문자를 찾는다. 버퍼 포인터는 찾아낸 문자를 가르킨다.

char ch:
	찾아야 할 문자..

return VALUE	:
	없음
************************************************************************/

inline char
cDECODER::findCharExact()
{	int	ch;

	while(1)
	{
		if (m_iBP>=m_iFileLength)	return	0;

		ch		=	m_pStream[m_iBP++];

		if (ch  ==  '/'			)	//	주석이다. 원래는 //를 체크해야 하지만.. 구찮어.. --;;
		{	while(ch != 0x0a	)
			{	ch	=	m_pStream[m_iBP++];
				if (m_iBP>=m_iFileLength) return FALSE;
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
cDECODER::findCharExact(char ch)
{	int getCh;

	while(1)
	{	getCh	=	findCharExact();
		if (getCh	== 0	) return 0;	//	끝났거나 없다.
		if (getCh	== ch	) break;	

		m_iBP	++;
	}

	return m_pStream[m_iBP];
}

/************************************************************************
char cDECODER::findCharOver(char ch)
지정된 문자를 찾는다. 버퍼 포인터는 바로 다음 문자를 가르킨다.

char ch:
	찾아야 할 문자..

return VALUE	:
	없음
************************************************************************/

inline BOOL
cDECODER::findCharOver()
{	if (findCharExact()	== 0	) return FALSE;	//	끝났거나 없다.

	m_iBP	++;

	return TRUE;
}

inline BOOL
cDECODER::findCharOver(char ch)
{	int getCh;

	while(1)
	{	getCh	=	m_pStream[m_iBP++];

		if (ch	==	getCh		)	return TRUE;
		if (ch  ==  '/'			)	//	주석이다. 원래는 //를 체크해야 하지만.. 구찮어.. --;;
		{	while(getCh != 0x0a	)
			{	getCh	=	m_pStream[m_iBP++];
				if (m_iBP>=m_iFileLength) return FALSE;
			}
		}
		if (m_iBP>=m_iFileLength) return FALSE;
	}

	return FALSE;
}

/************************************************************************
BOOL cDECODER::getStream(char *stream)
따옴표로 둘러 쌓인 문장을 읽어 들인다.

char  *stream:
	문자열을 저장할 버퍼

return VALUE	:
	없음
************************************************************************/

char *
cDECODER::getStream()
{	int		count	=	0;
	int		ch		=	0;

	if (!findCharOver('"')) return NULL;	//	문장 시작"

	while(1)
	{	ch	=	m_pStream[m_iBP++];

		if (m_iBP	>=	m_iFileLength)	
		{	
			return	NULL;
		}

		if (ch	==	'\\'		)	//	문장 넘어 가유~~
		{
			if (m_pStream[m_iBP]	==	0x0d	||	m_pStream[m_iBP]	==	0x0a)
			{
				ch	=	findCharExact();
				if (ch == 0		) return (char *)l_strCloseCode;
				continue;
			}
		}

		if (ch	==	'"'		) break;		//	스트림의 끝..

		if (ch	==	9		)				//	탭 처리
		{	m_strStream[count++]	=	32;
			m_strStream[count++]	=	32;
			m_strStream[count++]	=	32;
			m_strStream[count++]	=	32;
		}
		else m_strStream[count++]	=	ch;

		if (count >= dDECODER_MAX_STREAM-1)	break;
	}

	if (count == 0)	return NULL;

	m_strStream[count]	=	NULL;

	return m_strStream;
}

/************************************************************************
char *cDECODER::getWord()
단어를 찾아서 버퍼에 올린다. 공백이나 엔딩 문자열을 받을때까지 읽어 들임..

return VALUE	:
	단어의 길이..
************************************************************************/

char *
cDECODER::getWord()
{	int		count=0;
	int		ch;

	ch	=	findCharExact();

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
	{	ch	=	m_pStream[m_iBP];

		if (ch < 0)	//	한글이나 기타 2바이트 문자다.
		{	m_iBP++;
			m_strWord[count++]=	ch;
			m_strWord[count++]=	m_pStream[m_iBP];
		}
		else
		{	
			if (!isChar(ch))
				break;

			m_strWord[count++]=	ch;
		}

		if (m_iBP	>=	m_iFileLength)	return (char*)l_strEndCode;

		m_iBP++;
	}

	if (count==0) return (char*)l_strValidCode;

	m_strWord[count]	=	NULL;

	return m_strWord;
}

/************************************************************************
BOOL cDECODER::findWord(char *tmpword)
단어를 찾는다. 엔딩 코드 발견시 중지...

char *tmpword
	검색할 단어..	

return VALUE	:
	단어를 발견했을시  TRUE
************************************************************************/

BOOL
cDECODER::findWord(char *cmpWord)
{	while(1)
	{	if (strcmp(getWord(),cmpWord)==0)	return TRUE;

		if (m_iBP	>=	m_iFileLength	)	
			return 	cMSG::Error("Error in findWord(char *cmpWord) - can not find '%s' word",cmpWord);
	}

	return FALSE;
}


/************************************************************************
int cDECODER::getNumber()
숫자를 찾아서 그 값을 리턴한다..

return VALUE	:
	찾아낸 숫자의 정수 값
************************************************************************/

int
cDECODER::getNumber()
{	int		count=0;
	int		ch=0,minus=1;
	char	strNumber[16];

	while(1)
	{	ch	=	m_pStream[m_iBP++];

		if (m_iBP>m_iFileLength)
		{	cMSG::Error("Error in cDECODER::getNumber() - can not find number");
			return dCODE_VALID_NUMBER;
		}

		if (isNumber(ch)	)	
		{	m_iBP--;
			break;
		}

		if (isChar(ch)		)
		{	return dCODE_VALID_NUMBER;
		}
	}

	if (m_iBP	>	0)	if (m_pStream[m_iBP-1]=='-')	minus	=	-1;

	while(1) // 숫자가 아닌 문자를 만날때 까지..
	{	ch	=	m_pStream[m_iBP++];

		if (m_iBP >= m_iFileLength)	break;

		if (isNumber(ch))	strNumber[count++]=ch;
		else				break;

		if (count > 12)
		{	cMSG::Error("Error in cDECODER::getNumber() - so big number..");
			return dCODE_VALID_NUMBER;
		}
	}

	if (count==0) return dCODE_VALID_NUMBER;

	strNumber[count]=NULL;

	return (atoi(strNumber)*minus);
}

//	등록된 코드 분석..

DWORD
cDECODER::analCode(char *cmpKeyword)
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