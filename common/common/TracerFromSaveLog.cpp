// TracerFromSaveLog.cpp: implementation of the CTracerFromSaveLog class.
//
//////////////////////////////////////////////////////////////////////

#include "TracerFromSaveLog.h"
#include "cSRVUTIL.h"
#include <TChar.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTracerFromSaveLog::CTracerFromSaveLog()
{
	fp = NULL;
	strcpy(strPath,"");
	strcpy(strName,"");
	strcpy(fn,"");
	strcpy(dir,"");
	iNameCount = ver = iWriteCount = 0;
	day = cSRVUTIL::GetTime();
	mon = cSRVUTIL::GetMonth()+1;
	InitializeCriticalSection(&cs);
}

CTracerFromSaveLog::~CTracerFromSaveLog()
{
	if( handleConsole != INVALID_HANDLE_VALUE )
	{
		::CloseHandle( handleConsole );

		handleConsole = INVALID_HANDLE_VALUE;
	}
	EnterCriticalSection(&cs);
	if(fp)	fclose(fp);
	LeaveCriticalSection(&cs);
	DeleteCriticalSection(&cs);
}

BOOL	CTracerFromSaveLog::initialize( char * _dir,char * _fn)
{
	InitializeCriticalSection(&cs);

	strcpy(strPath,"");
	strcpy(dir,"");
	strcpy(strName,_fn);
	//	console 출력
	handleConsole = ::GetStdHandle( STD_OUTPUT_HANDLE );
	if( handleConsole == INVALID_HANDLE_VALUE )
	{
		if( ::AllocConsole() == FALSE )
		{
			::FreeConsole();
			return false;
		}

		handleConsole = ::GetStdHandle( STD_OUTPUT_HANDLE );
		if( handleConsole == INVALID_HANDLE_VALUE )
			return false;
	}

	//	file 출력
	day = cSRVUTIL::GetTime();
	mon = cSRVUTIL::GetMonth()+1;
	sprintf(strPath,"%s\\SaveLog",_dir);
	CreateDirectory(strPath,NULL);
	//	오늘의 디렉토리를 만들어보자.
	int dirname = 1900+cSRVUTIL::GetYear();
	sprintf(dir,"%s\\%d_%d",strPath,dirname,mon);
	CreateDirectory(dir,NULL);
	if(!SetFileHandle())	return FALSE;

	return TRUE;
}

void	CTracerFromSaveLog::close()
{
	if( handleConsole != INVALID_HANDLE_VALUE )
	{
		::CloseHandle( handleConsole );

		handleConsole = INVALID_HANDLE_VALUE;
	}
	EnterCriticalSection(&cs);
	if(fp)	fclose(fp);
	LeaveCriticalSection(&cs);
}

void	__cdecl CTracerFromSaveLog::print(WORD	wOutputType , LPCTSTR format, ... )
{
	TCHAR buffer[2048];	va_list args;

	va_start( args, format );
	_vstprintf( buffer, format, args );
	va_end( args );

	switch(wOutputType)
	{
	case 0:
		output(buffer);
		break;
	case 1:
		outputf(buffer);
		break;
	case 2:
		output(buffer);
		outputf(buffer);
		break;
	}
}

void	CTracerFromSaveLog::output( char * _strStr )
{
	DWORD size = 0;
	char	tmpStr[2048];
	sprintf(tmpStr,"[%d] %s\n",cSRVUTIL::GetTimes(),_strStr);
	tmpStr[2047] = 0;
	::WriteFile( handleConsole, tmpStr, strlen(tmpStr), &size, 0 );
}

void	CTracerFromSaveLog::outputf( char * _strStr )
{
	if( fp == 0 )
	{
		EnterCriticalSection(&cs);
		if(!SetFileHandle()){	++ver;	return;	}
		LeaveCriticalSection(&cs);
	}
	EnterCriticalSection(&cs);
	char	tmpStr[2048];
	sprintf(tmpStr,"[%d] %s\n",cSRVUTIL::GetTimes(),_strStr);
	tmpStr[2047] = 0;
	fprintf( fp, tmpStr );
	fflush(fp);
	++iWriteCount;
	if(iWriteCount>10000)
	{
		if(day!=cSRVUTIL::GetTime())
		{
			if(!SetFileHandle())	++ver;
			else					iWriteCount = 0;
		}else						iWriteCount = 0;

	}
	LeaveCriticalSection(&cs);
}

BOOL	CTracerFromSaveLog::SetFileHandle()
{
	try
	{
		if(fp)
		{
			fclose(fp);	fp = NULL;
		}
		if(mon!=cSRVUTIL::GetMonth()+1)
		{
			//	오늘의 디렉토리를 만들어보자.
			mon = cSRVUTIL::GetMonth()+1;
			printf("current dir = [%s]\n",strPath);
			int dirname = 1900+cSRVUTIL::GetYear();
			sprintf(dir,"%s\\%d_%d",strPath,dirname,mon);
			CreateDirectory(dir,NULL);
			ver=0;
		}
		day = cSRVUTIL::GetTime();
		sprintf(fn,"%s\\%s_%02d_%d.txt",dir,strName,ver,day);
		if((fp = fopen(fn,"a+"))==NULL)
			return FALSE;
		//	그 전 파일은 압축하고, 압축한 txt파일은 지운다.
		return TRUE;
	}catch(...){
		return FALSE;
	}
}