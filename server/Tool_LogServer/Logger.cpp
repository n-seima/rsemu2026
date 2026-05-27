#include "SFC.h"
#include "Logger.h"

#include <TChar.h>
#include <stdio.h> 
#include <io.h>
#include "cSRVUTIL.h"

//	┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	┃	Logger의 본격적인 메소드 정의
//	┃	하루에 하나의 파일을 만든다.
Logger::Logger()
{
}

Logger::Logger( LPCTSTR mainPath)
{
	initialize( mainPath);
}

Logger::~Logger()
{
	close();
}

BOOL Logger::initialize( LPCTSTR mainPath)
{
	InitializeCriticalSection(&cs);
	day = cSRVUTIL::GetTime();
	strcpy(strPath,mainPath);
	testCount = 0;

	sprintf(fn,"%s\\%d.log",strPath,cSRVUTIL::GetTime());
	day = cSRVUTIL::GetTime();
	//	오늘의 디렉토리를 만들어보자.
	if(!SetFileHandle())	return FALSE;
	InitializeCriticalSection(&cs);

	return true;
}

BOOL Logger::SetFileHandle()
{
	try
	{
		if(day!=cSRVUTIL::GetTime())
		{
			if(fp){	fclose(fp);		}
			day = cSRVUTIL::GetTime();
			sprintf(fn,"%s\\%d.log",strPath,cSRVUTIL::GetTime());
			if((fp = fopen(fn,"wb+"))==NULL)
			{
				return FALSE;
			}
		}
		return TRUE;
	}catch(...){
		printf("ERROR SET FILE HANDLE");
		MakeFile();
		return FALSE;
	}
}

BOOL
Logger::MakeFile()
{
	printf("강제로 새로운 파일을 생성합니다.");

	fp	=	NULL;
	day	=	cSRVUTIL::GetTime();

	sprintf(fn,"%s\\%d_%d_%d_%d.log",strPath,cSRVUTIL::GetTime(),cSRVUTIL::GetHour(),cSRVUTIL::GetMin(),cSRVUTIL::GetSec());

	if	(IsFile(fn))	//	파일이 이미 존재한다.
	{
		if	((fp = fopen(fn,"b+"))==NULL)	//	wb+가 아닌 b+
		{
			printf(" ERROR FILE CREATE [%s]",fn);
			return FALSE;
		}
	}
	else
	{
		if	((fp = fopen(fn,"wb"))==NULL)
		{
			printf(" ERROR FILE CREATE [%s]",fn);
			return FALSE;
		}
	}

	return TRUE;
}

void Logger::close()
{
	if( fp != 0 )
	{
		fclose( fp );

		fp = 0;
	}
}

void
Logger::write( char * data)
{
	try
	{
		if	( fp == 0 )
		{	
			MakeFile();
			return;
		}
		EnterCriticalSection(&cs);
		WORD size = 0;
		memcpy(&size,data,sizeof(WORD));
		fwrite( data,size,1,fp);
		fflush(fp);
		++testCount;
		if(testCount>dLOGGER_DAY_CHECK_COUNT)
		{	
			testCount = 0;
			SetFileHandle();
		}
		LeaveCriticalSection(&cs);
	}catch(...)
	{
		printf("ERROR FILE WRITE");
		MakeFile();
	}
}

//	┃	Logger의 본격적인 메소드 정의
//	┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
