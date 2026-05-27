#include "SFC.h"
#include "Logger.h"

#include <TChar.h>
#include <stdio.h> 
#include <io.h>
#include "cSRVUTIL.h"
#include "rscomm.h"

//	┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	┃	Logger의 본격적인 메소드 정의
//		하루에 하나의 파일을 만든다.
Logger::Logger()
{
	m_pFP	=	NULL;
}

Logger::Logger( LPCTSTR mainPath)
{
	m_pFP	=	NULL;
	initialize( mainPath);
}

Logger::~Logger()
{
	close();
}

BOOL
Logger::initialize( LPCTSTR mainPath)
{
	InitializeCriticalSection(&cs);

	day	=	cSRVUTIL::GetTime();

	strcpy(m_strLogPath,mainPath);

	testCount = 0;

	sprintf(m_strLogFileName,"%s\\%d.log",m_strLogPath,cSRVUTIL::GetTime());

	day	=	cSRVUTIL::GetTime();
	//	오늘의 디렉토리를 만들어보자.

	if	(!CreateFileHandle())
		return FALSE;

	return true;
}

BOOL
Logger::makeFile(char *_lpstrFileName)
{
	strcpy(m_strLogFileName,_lpstrFileName);

	if	(IsFile(_lpstrFileName))
	{
		if	((m_pFP = fopen(m_strLogFileName,"rb+"))==NULL)
		{
			_log("- found old file");
			_log(_ms("- but failed to open file [%s]",m_strLogFileName));
			
			return FALSE;
		}

		fseek(m_pFP,0,SEEK_END);

		_log("- found old file");
		_log(_ms("- attatch log with exist file [%s]",m_strLogFileName));
	}
	else
	{
		if	((m_pFP = fopen(m_strLogFileName,"wb"))==NULL)
			return FALSE;

		_log("- can not find old file");
		_log(_ms("- make new file [%s]",m_strLogFileName));
	}

	return	TRUE;
}

BOOL
Logger::CreateFileHandle()
{
	CCritical	CS(&cs);

	try
	{
		if	(day	!=	cSRVUTIL::GetTime())
		{
			if	(m_pFP)
				return TRUE;

			day	=	cSRVUTIL::GetTime();

			sprintf(m_strLogFileName,"%s\\%d.log",m_strLogPath,cSRVUTIL::GetTime());
			_log(" create file : %s",m_strLogFileName);

			return	makeFile(m_strLogFileName);
		}

		return TRUE;
	}

	catch(...)
	{
		_log("ERROR Logger::CreateFileHandle");
		MakeFile();
		return FALSE;
	}
}

BOOL
Logger::SetFileHandle()
{
//	CCritical	CS(&cs);
	try
	{
		if	(day	!=	cSRVUTIL::GetTime())	//	날짜가 바뀌었으니 새로 파일 생성하자.
		{
			if	(m_pFP)
				fclose(m_pFP);

			day = cSRVUTIL::GetTime();

			sprintf(m_strLogFileName,"%s\\%d.log",m_strLogPath,cSRVUTIL::GetTime());

			return	makeFile(m_strLogFileName);
		}

		return TRUE;
	}

	catch(...)
	{
		_log("ERROR Logger::SetFileHandle()");
		MakeFile();
		return FALSE;
	}
}

BOOL
Logger::MakeFile()
{
	_log("START - Make new log");

	m_pFP	=	NULL;
	day	=	cSRVUTIL::GetTime();

	_log("path : %s",m_strLogPath);

//	sprintf(m_strLogFileName,"%s\\%d_%.2d_%.2d_%.2d.log",m_strLogPath,cSRVUTIL::GetTime(),cSRVUTIL::GetHour(),cSRVUTIL::GetMin(),cSRVUTIL::GetSec());
	sprintf(m_strLogFileName,"%s\\%d.log",m_strLogPath,cSRVUTIL::GetTime());

	_log("m_strLogFileName = %s",m_strLogFileName);

	if	(!makeFile(m_strLogFileName))
	{
		_log(" ERROR FILE CREATE [%s]",m_strLogFileName);

		return FALSE;
	}

	_log("END - Make New Log");

	return TRUE;
}

void Logger::close()
{
	if	(m_pFP != 0 )
	{
		fclose( m_pFP );

		m_pFP = 0;
	}
}

void
Logger::write(void *_lpData)
{
	CCritical	CS(&cs);

	try
	{
		if	(m_pFP == 0 )	//	파일이 오픈 안되어 있으면 파일 생성
			MakeFile();

		WORD	wSize	=	*(WORD*)_lpData;

		fwrite( _lpData,wSize,1,m_pFP);
		fflush(m_pFP);

		SetFileHandle();	//	날짜 변경되면 파일 변경
	}

	catch(...)
	{
		_log("Logger::write");
		MakeFile();
	}
}

//	┃	Logger의 본격적인 메소드 정의
//	┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━