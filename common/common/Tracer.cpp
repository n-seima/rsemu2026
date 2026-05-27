#include "Tracer.h"

#include <TChar.h>
#include <stdio.h> 
#include <io.h>
#include "cSRVUTIL.h"

//	┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	┃	Console로만 출력 
TracerConsole::TracerConsole()
	: handleConsole( INVALID_HANDLE_VALUE )
{
}

TracerConsole::~TracerConsole()
{
}

bool TracerConsole::open( LPCTSTR path , LPCTSTR name )
{
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

	return true;
}

void TracerConsole::output( LPCTSTR str ,DWORD type)
{
	DWORD size = 0;

//	::WriteFile( handleConsole, str, _tcsclen( str ), &size, 0 );
	::WriteFile( handleConsole, str, strlen( str ), &size, 0 );
}

void TracerConsole::close()
{
	if( handleConsole != INVALID_HANDLE_VALUE )
	{
		::CloseHandle( handleConsole );

		handleConsole = INVALID_HANDLE_VALUE;
	}
}

//	┃	Console로만 출력 
//	┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━




//	┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	┃	File 로만 출력 
TracerFile::TracerFile()
	: fp( 0 )
{
	InitializeCriticalSection(&cs);
	iWriteCount = iNameCount = ver = 0;
	day = cSRVUTIL::GetTime();
	strcpy(strPath,"");
	strcpy(strName,"");
	strcpy(dir,"");
	strcpy(fn,"");
}

TracerFile::~TracerFile()
{
	DeleteCriticalSection(&cs);
}

BOOL TracerFile::SetFileHandle()
{
	try
	{
		DWORD size = 0;
		if(fp)
		{
			size = filelength(fileno(fp));
			if(size>dTRACER_FILE_LENGTH)
			{
				++ver;
				fclose(fp);	fp = NULL;
			}
			else if(day!=cSRVUTIL::GetTime())
			{
				fclose(fp);	fp = NULL;
				day = cSRVUTIL::GetTime();
				//	오늘의 디렉토리를 만들어보자.
				printf("current dir = [%s]\n",strPath);
				sprintf(dir,"%s\\%d",strPath,cSRVUTIL::GetTime());
				CreateDirectory(dir,NULL);
				SetCurrentDirectory(dir);
				ver=0;
			}else
				return FALSE;
		}
		sprintf(fn,"%s\\%s_%02d_%d.txt",dir,strName,day,ver);
		if((fp = fopen(fn,"a+"))==NULL)
		{
			return FALSE;
		}
		
		if(size>dTRACER_FILE_LENGTH)
			SetFileHandle();
		return TRUE;
	}catch(...){
		return FALSE;
	}
}

bool TracerFile::open( LPCTSTR path , LPCTSTR name )
{
	strcpy(strPath,path);
	strcpy(strName,name);
	day = cSRVUTIL::GetTime();
	SetCurrentDirectory(strPath);
	CreateDirectory("Log",NULL);
	sprintf(dir,"%s\\Log",strPath);
	SetCurrentDirectory(dir);
	strcpy(strPath,dir);
	//	오늘의 디렉토리를 만들어보자.
	sprintf(dir,"%s\\%d",strPath,cSRVUTIL::GetTime());
	CreateDirectory(dir,NULL);
	SetCurrentDirectory(dir);
	if(!SetFileHandle())	return FALSE;

	return true;
}

void TracerFile::output( LPCTSTR str ,DWORD type)
{
	if( fp == 0 )	return;
	EnterCriticalSection(&cs);
	iWriteCount++;
	_ftprintf( fp, str );

	if(iWriteCount>10000)	
	{
		if(!SetFileHandle())	++ver;
		else					iWriteCount = 0;
	}
	fflush(fp);
	LeaveCriticalSection(&cs);
}

void TracerFile::close()
{
	if( fp != 0 )
	{
		fclose( fp );

		fp = 0;
	}
}
//	┃	File로만 출력 
//	┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//	┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	┃	VC의 Trace 로만 출력 
TracerTrace::TracerTrace()
{
}

TracerTrace::~TracerTrace()
{
}

bool TracerTrace::open( LPCTSTR path , LPCTSTR name )
{
	return true;
}

void TracerTrace::output( LPCTSTR str ,DWORD type)
{
	::OutputDebugString( str );
}

void TracerTrace::close()
{
}
//	┃	VC의 Trace로만 출력 
//	┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//	┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	┃	Console , File ,Trace 출력 
TracerAll::TracerAll()
	: handleConsole( INVALID_HANDLE_VALUE )
{
}

TracerAll::~TracerAll()
{

}

bool TracerAll::open( LPCTSTR path , LPCTSTR name ) 
{
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

	strcpy(strPath,path);
	strcpy(strName,name);
	day = cSRVUTIL::GetTime();
	SetCurrentDirectory(strPath);
	CreateDirectory("Log",NULL);
	sprintf(dir,"%s\\Log",strPath);
	SetCurrentDirectory(dir);
	strcpy(strPath,dir);
	//	오늘의 디렉토리를 만들어보자.
	sprintf(dir,"%s\\%d",strPath,cSRVUTIL::GetTime());
	CreateDirectory(dir,NULL);
	SetCurrentDirectory(dir);
	if(!SetFileHandle())	return FALSE;

	return true;
}

void TracerAll::output( LPCTSTR str ,DWORD type)
{
//#ifndef _WINDOWS_
	if(type == DTType_Console || type== DTType_ALL)
	{
		DWORD size = 0;
		//	::WriteFile( handleConsole, str, _tcsclen( str ), &size, 0 );
			::WriteFile( handleConsole, str, strlen( str ), &size, 0 );
			::WriteFile( handleConsole, _T("\n"), strlen( _T("\n") ), &size, 0 );
	}
//#endif
	if(type == DTType_File || type== DTType_ALL)
	{
		if( fp )
		{
			EnterCriticalSection(&cs);
			iWriteCount++;
//			_ftprintf( fp, str );
			fprintf(fp,"[%06d] ",cSRVUTIL::GetTimes());
			fprintf( fp, str );
			fprintf(fp,"\n");
			fflush(fp);
			if(iWriteCount>10000)	
			{
				if(!SetFileHandle())	++ver;
				else					iWriteCount = 0;
			}
			LeaveCriticalSection(&cs);
		}
	}
/*	딜레이가 상당히 커서 나중에 블럭된다 -_-;
	if(type == DTType_Trace || type == DTType_ALL)
	{
		::OutputDebugString( str );
	}
*/
}

void TracerAll::close()
{
	if( handleConsole != INVALID_HANDLE_VALUE )
	{
		try
		{
			::CloseHandle( handleConsole );
			handleConsole = INVALID_HANDLE_VALUE;
		}catch(...){}
	}

	if( fp != 0 )
	{
		fclose( fp );

		fp = 0;
	}
}

//	┃	Console , File , Trace 출력 
//	┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//	┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	┃	DebugTracer의 본격적인 메소드 정의
DebugTracer::DebugTracer()
	: tracer( 0 )
{
}

DebugTracer::DebugTracer( WORD type, LPCTSTR fileName )
	: tracer( 0 )
{
	initialize( type, fileName );
}

DebugTracer::~DebugTracer()
{
	close();
}

bool DebugTracer::initialize( WORD type, LPCTSTR fileName )
{
	switch( type )
	{
	case  DTType_Console	:	tracer = ( ITracer* )new TracerConsole();		break;
	case  DTType_File		:	tracer = ( ITracer* )new TracerFile();			break;
	case  DTType_Trace		:	tracer = ( ITracer* )new TracerTrace();			break;
	case  DTType_ALL		:	tracer = ( ITracer* )new TracerAll();			break;

	default :
		return false;
	}

	char path[1024];
	GetCurrentDirectory(1024,path);
	tracer->open(path,fileName);

	return true;
}

void DebugTracer::close()
{
	if( tracer != 0 )
	{
		tracer->close();
		delete tracer;

		tracer = 0;
	}
}

void __cdecl DebugTracer::outputf( LPCTSTR format, ... )
{
	if( tracer == 0 )
		return;

	try
	{
		TCHAR buffer[2048];
		va_list args;

		va_start( args, format );
		_vstprintf( buffer, format, args );
		va_end( args );

		tracer->output( buffer , DTType_File);
	}catch(...)
	{
		tracer->output( "Error in DebugTracer::outputf" , DTType_File);
	}
}

void __cdecl DebugTracer::outputc( LPCTSTR format, ... )
{
	if( tracer == 0 )
		return;

	try
	{
		TCHAR buffer[2048];
		va_list args;

		va_start( args, format );
		_vstprintf( buffer, format, args );
		va_end( args );

		tracer->output( buffer , DTType_Console);
	}catch(...)
	{
		tracer->output( "Error in DebugTracer::outputc" , DTType_Console);
	}
}

void __cdecl DebugTracer::output( LPCTSTR format, ... )
{
	if( tracer == 0 )
		return;

	try
	{
		TCHAR buffer[2048];
		sprintf(buffer,"[%d] ",cSRVUTIL::GetTimes());

		va_list args;

		va_start( args, format );
		_vstprintf( buffer, format, args );
		va_end( args );

		strcat(buffer,"\n");
		tracer->output( buffer , DTType_ALL);
	}catch(...)
	{
		tracer->output( "Error in DebugTracer::output" , DTType_ALL);
	}
}

void __cdecl DebugTracer::output( WORD type , LPCTSTR format, ... )
{
	if( tracer == 0 )
		return;

	try
	{
		TCHAR buffer[2048];
		va_list args;

		va_start( args, format );
		_vstprintf( buffer, format, args );
		va_end( args );

		tracer->output( buffer , type);
	}catch(...)
	{
		tracer->output( "Error in DebugTracer::output" , type);
	}

}
/*
DebugTracer& DebugTracer::operator << ( bool arg )
{
	output( _T( "%d" ), arg );

	return *this;
}

DebugTracer& DebugTracer::operator << ( int arg )
{
	output( _T( "%d" ), arg );

	return *this;
}

DebugTracer& DebugTracer::operator << ( long arg )
{
	output( _T( "%d" ), arg );

	return *this;
}

DebugTracer& DebugTracer::operator << ( DWORD arg )
{
	output( _T( "%d" ), arg );

	return *this;
}

DebugTracer& DebugTracer::operator << ( __int64 arg )
{
	output( _T( "%d" ), arg );

	return *this;
}

DebugTracer& DebugTracer::operator << ( LPTSTR arg )
{
	output( arg );

	return *this;
}
*/

//	┃	DebugTracer의 본격적인 메소드 정의
//	┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
