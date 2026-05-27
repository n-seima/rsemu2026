/**************************************************************************
문자열 분석 클래스와 배경 업로드 루틴 이벤트 업로드 루틴

  개요 :  SRPG 게임을 생각하고 만든 간단한 스크립터

작성일 : 98.06.13
***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cDECODER.h"	//	클래스 헤더화일..
#include "debug.h"	//	클래스 헤더화일..

const	char	*l_strSystemCode	="isSystemCode";
const	char	*l_strBeginCode		="isBeginCode";
const	char	*l_strCloseCode		="isCloseCode";
const	char	*l_strEndCode		="isEndCode";
const	char	*l_strValidCode		="isValidCode";
const	char	*l_strFinishCode	="isFinishCode";

static	int		l_isChar[256]		=	{100,};

BOOL	cDECODER::s_bIsThai			=	FALSE;

cDECODER	tempDecoder;		//	종료시 버퍼를 비운다.

cDECODER::cDECODER()
{
	m_pSTREAM		=	NULL;
	m_iBP			=	0;
	m_iFileLength	=	0;
	m_lpKeyWord		=	NULL;
	m_bIsJapan		=	FALSE;
	m_bIsForcedJapan=	FALSE;
	m_bIsForcedThai	=	FALSE;

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
{
	Reset();
}


//	버퍼를 비운다.
void
cDECODER::Reset()
{
	_free(m_pSTREAM);
	m_lpKeyWord		=	NULL;
	m_iBP			=	0;
	m_isDone		=	FALSE;
	m_bIsJapan		=	FALSE;
	m_bIsThai		=	FALSE;
}

//	화일을 버퍼에 올린다.

BOOL
cDECODER::Upload(char *fn,cKEYWORD_ITEM *keyword)
{
	m_bIsJapan	=	FALSE;
	m_bIsThai	=	FALSE;

	cFILE	file;

	if (!file.Open( fn,"rb"))
		return MSGOUT("Error in cDECODER::Upload - can not find '%s' file",fn);

	{
		BYTE	abCode[2];
		file.Read(abCode,2);
		
		if	(abCode[0] == 255 && abCode[1] == 254 )
		{
			ERRMSG("","%s 파일은 유니코드 입니다.",fn);

			file.Close();

			return	FALSE;
		}

		file.Seek(0,SEEK_SET);
	}

	Reset();

	m_lpKeyWord		=	keyword;
	m_iFileLength	=	file.Length+1;
	m_pSTREAM		=	_new(char,m_iFileLength,"cDECODER::m_pSTREAM");
	file.Read(m_pSTREAM,m_iFileLength-1);
	file.Close();

	if	(strstr(fn,"japan"))
	{
		if	(strstr(fn,"lastest")==NULL)
			m_bIsJapan	=	TRUE;
	}
	if	(strstr(fn,"thai"))
	{
		if	(strstr(fn,"lastest")==NULL)
			m_bIsThai	=	TRUE;
	}

	if	(m_bIsForcedJapan)
		m_bIsJapan	=	TRUE;
	if	(m_bIsForcedThai)
		m_bIsThai	=	TRUE;

	return TRUE;
}

BOOL
cDECODER::UploadBuffer(char *stream,cKEYWORD_ITEM *keyword)
{
	Reset();

	m_lpKeyWord		=	keyword;
	m_iFileLength	=	strlen(stream)+1;
	m_pSTREAM		=	_new(char,m_iFileLength,"cDECODER::m_pSTREAM");
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

//	숫자인가?
inline BOOL
cDECODER::isNumber(char ch)
{	if (ch>='0' && ch<='9') return TRUE;

	return FALSE;
}

BOOL
cDECODER::isNumber(char *_lpstrText)
{
	int	iPos	=	0;

	while(_lpstrText[iPos])
	{
		if (!isNumber(_lpstrText[iPos]))
			return	FALSE;
		
		iPos++;
	}
	
	return	TRUE;
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
{	
	int	ch;

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

		if (ch  ==  '/'			)	//	주석이다. 원래는 //를 체크해야 하지만.. 구찮어.. --;;
		{
			while(ch != 0x0a	)
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
cDECODER::FindCharExact()
{	char	ch;

	while(1)
	{	ch		=	m_pSTREAM[m_iBP++];

		if (ch  ==  '/'			)	//	주석이다. 원래는 //를 체크해야 하지만.. 구찮어.. --;;
		{	while(ch != 0x0a	)
			{	ch	=	m_pSTREAM[m_iBP++];
				if (m_iBP>=m_iFileLength) return FALSE;
			}
		}
		else
		if (isChar(ch))			break;
		else
		if (isSystemCode(ch))	break;

		if (m_iBP>=m_iFileLength)	return	0;
	}

	m_iBP--;

	return ch;
}

inline char
cDECODER::FindCharExact(char ch)
{	char getCh;

	while(1)
	{	getCh	=	FindCharExact();
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

inline BOOL
cDECODER::FindCharOver()
{	
	if (FindAnyCharExact()	== 0	) return FALSE;	//	끝났거나 없다.

	m_iBP	++;

	return TRUE;
}

BOOL
cDECODER::FindCharOver(char ch)
{	
	char getCh;

	while(1)
	{
		getCh	=	m_pSTREAM[m_iBP++];

		if (getCh	==	ch		)
			return TRUE;

		if (getCh	==	'/'		)	//	주석이다. 원래는 //를 체크해야 하지만.. 구찮어.. --;;
		{	
			while(getCh != 0x0a	)
			{	
				getCh	=	m_pSTREAM[m_iBP++];

				if (m_iBP>=m_iFileLength)
					return FALSE;
			}
		}

		if (m_iBP>=m_iFileLength)
			return FALSE;
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

BOOL
cDECODER::isEmptyLine()
{
	for (int i=0;;i++)
	{
		if	(m_pSTREAM[i+m_iBP]	==	NULL)
			return	TRUE;

		if	(m_pSTREAM[i+m_iBP]	==	13	||	m_pSTREAM[i+m_iBP]	==	10	)
			return	TRUE;

		if	(m_pSTREAM[i+m_iBP]	>	32)
			return	FALSE;
	}

	return	FALSE;
}

BOOL
cDECODER::findCharWithRange(char ch,int _iRange)
{
	int	iHitCount	=	0;
	
	for (int i=0;iHitCount<_iRange;i++)
	{
		if	(m_pSTREAM[i+m_iBP]	==	NULL)
			return	FALSE;

		if	(m_pSTREAM[i+m_iBP]	==	ch)
			return	TRUE;

		if	(m_pSTREAM[i+m_iBP]	!=	32)
			iHitCount++;
	}

	return	FALSE;
}

char *
cDECODER::GetStream()
{	
	int		count				=	0;
	char	ch					=	0;
	BOOL	bIsHangul			=	FALSE;
	BOOL	bIsWantExportStream	=	FALSE;

	if (!FindCharOver('"'))
		return "stream not Found!!";	//	문장 시작"

	while(1)
	{
		if (m_iBP	>=	m_iFileLength)	
		{
			ERRMSG("Error in cDECODER::GetStream()- overfolw");
			break;
		}

		ch	=	m_pSTREAM[m_iBP++];

		if (bIsHangul	==	FALSE)
		{
			if (ch	==	'\\'		)	//	문장 넘어 가유~~
			{	
				if (m_pSTREAM[m_iBP] != 'n')
				{
					ch	=	FindAnyCharExact();

					if (ch == 0		)
						return (char *)l_strCloseCode;

					continue;
				}
				else
				{
					ch	=	10;
					m_iBP++;
				}
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
		}
		else
			m_strStream[count++]	=	ch;

		if	(m_bIsThai)
			bIsHangul	=	FALSE;
		else
		if (bIsHangul	==	FALSE)
		{
			if	(ch	<	0)
			{
				bIsHangul	=	TRUE;

				if (m_bIsJapan)
				{
					if	((BYTE)ch	>=	0xa1 && (BYTE)ch <=0xdf)
					{
						bIsHangul			=	FALSE;
						bIsWantExportStream	=	TRUE;
					}
				}
			}
		}
		else
			bIsHangul	=	FALSE;

		if (count >= dDECODER_MAX_STREAM-1)
			break;
	}

//	if (count == 0)
//		return (char*)l_strValidCode;
	m_strStream[count]	=	NULL;
	
//	if (bIsWantExportStream)
//		MSGOUT(m_strStream);

	return m_strStream;
}

char *
cDECODER::GetStream(char cEndSymbol,char cEndSymbol2)
{
	int		count	=	0;
	char	ch		=	0;

	if (m_iBP	>=	m_iFileLength)	
	{	
		m_isDone	=	TRUE;
		return	NULL;
	}

	while(1)
	{	
		ch	=	m_pSTREAM[m_iBP++];

		if (m_iBP	>=	m_iFileLength)	
		{	
			m_isDone	=	TRUE;
			break;
		}

		if	(ch	==	cEndSymbol	)
			break;		//	스트림의 끝..

		if	(cEndSymbol2)
			if	(ch	==	cEndSymbol2)
				break;

		m_strStream[count++]	=	ch;

		if (count >= dDECODER_MAX_STREAM-1)
			break;
	}

	if (count == 0)	
		return NULL;

	m_strStream[count]	=	NULL;

	return m_strStream;
}

/************************************************************************
char *cDECODER::GetWord()
단어를 찾아서 버퍼에 올린다. 공백이나 엔딩 문자열을 받을때까지 읽어 들임..

return VALUE	:
	단어의 길이..
************************************************************************/

char *
cDECODER::GetWord()
{	int		count=0;
	char	ch;

	ch	=	FindCharExact();

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

		BOOL	bIsNot2ByteChar	=	FALSE;

		if (m_bIsJapan && ch == -80)
			bIsNot2ByteChar	=	TRUE;

		if (ch < 0 && bIsNot2ByteChar	==	FALSE)	//	한글이나 기타 2바이트 문자다.
		{	
			m_iBP++;
			m_strWORD[count++]=	ch;
			m_strWORD[count++]=	m_pSTREAM[m_iBP];
		}
		else
		{	
			if (!isChar(ch))
				break;

			m_strWORD[count++]=	ch;
		}

		if (m_iBP	>=	m_iFileLength)
			return (char*)l_strEndCode;

		m_iBP++;
	}

//	if (count==0) 
//		return (char*)l_strValidCode;

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
cDECODER::GetNumber(BOOL _bIsWantMessage)
{
	int		count=0;
	char	ch=0,minus=1;
	char	strNumber[16];

	while(1)
	{	
		ch	=	m_pSTREAM[m_iBP++];

		if (m_iBP	>	m_iFileLength)
		{
			if (_bIsWantMessage)
				ERRMSG("Error in cDECODER::GetNumber() - can not find number");
			return dCODE_VALID_NUMBER;
		}

		if (isNumber(ch)	)	
		{	m_iBP--;
			break;
		}

		if (isChar(ch)		)
		{
			return dCODE_VALID_NUMBER;
		}
	}

	if (m_iBP	>	0)	if (m_pSTREAM[m_iBP-1]=='-')	minus	=	-1;

	while(1) // 숫자가 아닌 문자를 만날때 까지..
	{	ch	=	m_pSTREAM[m_iBP++];

		if (m_iBP >= m_iFileLength)	break;

		if (isNumber(ch))	
			strNumber[count++]=ch;
		else
		{
			m_iBP--;
			break;
		}

		if (count > 12)
		{	
			if (_bIsWantMessage)
				ERRMSG("Error in cDECODER::GetNumber() - so big number..");
			return dCODE_VALID_NUMBER;
		}
	}

	if (count==0) 
		return dCODE_VALID_NUMBER;

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
