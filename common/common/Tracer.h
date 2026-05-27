#ifndef  __Tracer_H
#define  __Tracer_H

#ifndef _WINDOWS_
#include <windows.h>
#endif
#include <stdio.h>

#define DTType_Console		0x00
#define DTType_File			0x01
#define DTType_Trace		0x02
#define DTType_ALL			0x03

#define dTRACERNAME_LENGTH	512
#define dTRACERPATH_LENGTH	1024
#define dTRACER_FILE_LENGTH	4096000

class ITracer
{
public:
	CRITICAL_SECTION	cs;

			ITracer(){}
	virtual	~ITracer(){}

	virtual	bool		open( LPCTSTR path , LPCTSTR name ) = 0;
	virtual	void		output( LPCTSTR str ,DWORD type=0) = 0;
	virtual	void		close() = 0;
};

class TracerConsole : public ITracer
{
public:
			TracerConsole();
	virtual	~TracerConsole();

	virtual	bool		open( LPCTSTR path , LPCTSTR name );
	virtual	void		output( LPCTSTR str ,DWORD type=0);
	virtual	void		close();

private:
			HANDLE		handleConsole;
};


class TracerFile : public ITracer
{
public:
			TracerFile();
	virtual	~TracerFile();

	virtual	bool		open( LPCTSTR path , LPCTSTR name );
	virtual	void		output( LPCTSTR str ,DWORD type=0);
	virtual	void		close();

			BOOL		SetFileHandle();

protected:
			FILE*		fp;
			char	strPath[2048] , strName[512] , fn[2048],dir[2048];
			int		iNameCount, iWriteCount,ver,day;
};

class TracerTrace : public ITracer
{
public:
			TracerTrace();
	virtual	~TracerTrace();

	virtual	bool		open( LPCTSTR path , LPCTSTR name );
	virtual	void		output( LPCTSTR str ,DWORD type=0);
	virtual	void		close();
};

class TracerAll	:	public TracerFile
{
public:
			TracerAll();
	virtual ~TracerAll();

	virtual	bool		open( LPCTSTR path , LPCTSTR name );
	virtual	void		output( LPCTSTR str ,DWORD type=0);
	virtual	void		close();

private:
			HANDLE		handleConsole;
};

class DebugTracer
{
public:
			DebugTracer();
			DebugTracer( WORD type, LPCTSTR fileName = 0 );
	virtual	~DebugTracer();

			bool		initialize( WORD type, LPCTSTR fileName = 0 );
			void		close();

			void	__cdecl	output( WORD type, LPCTSTR format, ... );
			void	__cdecl	output( LPCTSTR format, ... );
			void	__cdecl	outputf( LPCTSTR format, ... );
			void	__cdecl	outputc( LPCTSTR format, ... );
/*
			DebugTracer&	operator << ( bool arg );
			DebugTracer&	operator << ( int arg );
			DebugTracer&	operator << ( long arg );
			DebugTracer&	operator << ( DWORD arg );
			DebugTracer&	operator << ( __int64 arg );
			DebugTracer&	operator << ( LPTSTR arg );
*/
private:
			ITracer*		tracer;
};

#endif
