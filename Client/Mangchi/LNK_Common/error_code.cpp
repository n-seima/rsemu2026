#include	"error_code.h"
#include	<iostream>
#include	<stdarg.h>

#ifdef	WIN32
#include	<windows.h>
#endif

s4		cErrorCodeManager::s_s4IsExist	=	0;

cErrorCodeManager	g_ecm;	//	error code manager

cErrorCodeManager::cErrorCodeManager()
{
	if	(s_s4IsExist)
	{
#ifdef	WIN32
		MessageBox(GetActiveWindow(),"cErrorCodeManager::s_s4IsExist","error!!",MB_OK);
#endif
	}

	s_s4IsExist	=	1;
}

cErrorCodeManager::~cErrorCodeManager()
{
}

//	실제 에러가 저장된 슬롯을 리턴
s4
cErrorCodeManager::_getErrorSlot(s4 _s4Index)
{
	if	(_s4Index	==	-1)
		_s4Index		=	min(m_s4ErrorCount-1,eMaxErrorCount-1);

	if	(_s4Index	>=	min(m_s4ErrorCount,eMaxErrorCount)	||	_s4Index	<	0)
		return	-1;

	return	(m_s4BeginPos+_s4Index)%eMaxErrorCount;
}

//
//	초기화를 한다.
//	에러 코드 파일을 인자로 받는다.
bool
cErrorCodeManager::init(char *_lpstrErrorCodeFileName)
{
	reset();

	return	true;
}

void
cErrorCodeManager::reset()
{
	m_s4ErrorCount	=	0;
	m_s4BeginPos		=	0;
}

//
//	에러 추가
//	에러의 수가 최대 100개를 넘게 되면 가장 먼저 들어온 에러가 제거된다.
void
cErrorCodeManager::addError(s4 _s4ErrorCode,char *_lpstrErrorMessage,...)
{
	if	(_lpstrErrorMessage)
	{
		va_list		argptr;

		va_start(argptr, _lpstrErrorMessage);
		vsprintf(m_strTextBuffer,_lpstrErrorMessage,argptr);
		va_end(argptr);
	}

	s4	iSlot			=	m_s4ErrorCount;
	
	if	(m_s4ErrorCount	>=	eMaxErrorCount)
	{
		m_s4BeginPos		=	(m_s4ErrorCount+1-eMaxErrorCount)%eMaxErrorCount;
		iSlot			=	m_s4ErrorCount%eMaxErrorCount;
	}

	m_s4ErrorCount++;

	cErrorCode	*lpSlot	=	&m_aErrorList[iSlot];

	lpSlot->m_s4Code		=	_s4ErrorCode;

#ifdef	WIN32
	SetLastError(_s4ErrorCode);
#endif

	if	(_lpstrErrorMessage)
		strncpy(lpSlot->m_strMessage,m_strTextBuffer,eMaxErrorMessageLength-1);
	else
		strcpy(lpSlot->m_strMessage," ");
}

//
//	에러 추가
//	에러의 수가 최대 100개를 넘게 되면 가장 먼저 들어온 에러가 제거된다.
void
cErrorCodeManager::addError(char *_lpstrErrorMessage,...)
{
	va_list		argptr;

	va_start(argptr, _lpstrErrorMessage);
	vsprintf(m_strTextBuffer,_lpstrErrorMessage,argptr);
	va_end(argptr);

	s4	iSlot			=	m_s4ErrorCount;
	
	if	(m_s4ErrorCount	>=	eMaxErrorCount)
	{
		m_s4BeginPos		=	(m_s4ErrorCount+1-eMaxErrorCount)%eMaxErrorCount;
		iSlot			=	m_s4ErrorCount%eMaxErrorCount;
	}

	m_s4ErrorCount++;

	cErrorCode	*lpSlot	=	&m_aErrorList[iSlot];

	lpSlot->m_s4Code		=	eEC_Valid;

#ifdef	WIN32
	SetLastError(eEC_Valid);
#endif

	strncpy(lpSlot->m_strMessage,_lpstrErrorMessage,eMaxErrorMessageLength-1);
}

//	현재까지 에러 수를 얻어 온다.
s4
cErrorCodeManager::getErrorCount(s4 *_lps4TotalErrorCount)
{
	if	(_lps4TotalErrorCount)
		*_lps4TotalErrorCount	=	m_s4ErrorCount;

	return	min(m_s4ErrorCount,eMaxErrorCount);
}

//	에러를 얻는다. 인자가 없을 경우 마지막 에러 리턴
s4
cErrorCodeManager::getLastError(s4 _s4Index)
{
	s4	iSlot	=	_getErrorSlot(_s4Index);

	if	(iSlot	==	-1)
		return	-1;

	return	m_aErrorList[iSlot].m_s4Code;
}

//	에러 메시지를 얻는다. 인자가 없을 경우 마지막 에러 리턴
char*
cErrorCodeManager::getLastErrorMessage(s4 _s4Index)
{
	s4	iSlot	=	_getErrorSlot(_s4Index);

	if	(iSlot	==	-1)
		return	" ";

	return	m_aErrorList[iSlot].m_strMessage;
}