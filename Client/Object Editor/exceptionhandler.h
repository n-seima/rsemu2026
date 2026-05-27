#ifdef	WIN32
#ifndef __ClassExceptionHandler_H__
#define __ClassExceptionHandler_H__

#include <imagehlp.h>
#include "lnk_define.h"

const	s4	c_s4EH_HeaderLength			=	512;
const	s4	c_s4EH_DefaultLogSize		=	1024*100;	//	100kb
const	s4	c_s4EH_MaxTemplateLogCount	=	1024;
const	s4	c_s4EH_StaticLogBufferSize	=	c_s4EH_DefaultLogSize;

//	사용 예
//
//	g_eh.setHeader("붉은 보석 버젼 %d.%.4d",g_s4Version/10000,g_s4Version%10000);
//	g_eh.setLogFileName(파일 전체 경로);	//	로그를 남길 파일 이름(가급적이면 전체 경로를 지정하는게 좋음)
//	g_eh.addStaticLog("OS ......");	
//	g_eh.addStaticLog("CPU ......");
//	g_eh.addStaticLog("Memory ......");
//
//	g_eh.setLauncher(SendEH_Log);	//	프로그램이 죽으면 리포트 파일 생성후 이 함수를 호출하게 된다.
//
//	void
//	cActor::put(s4 _s4X,s4 _s4Y)
//	{
//		mBeginTLog();	//	임시 로그 시작
//
//		g_eh.addLog("%s : %d,%d",m_strName,_s4X,_s4Y);	//	이렇게 추가한 로그는 이 함수에서 문제 없이 끝나면 사라진다.
//		put....
//		g_eh.addLog("put shadow");
//		putShadow....
//	}
//	g_eh.outputStaticLog("%s/result.txt",g_strLogFolder);	//	setHeader,addStaticLog 등으로 추가된 내용을 파일에 출력한다.

class cExceptionHandler
{
public:
							cExceptionHandler();
							~cExceptionHandler();

	void					setLogFileName(PTSTR pszLogFileName);	//	로그 파일명 설정
	void					setHeader(char *_lpstrHeader);	//	헤더 설정

	void					initLog(s4 _s4Size=c_s4EH_DefaultLogSize);//	로그 초기화

	BOOL					addStaticLog(char *_lpstrFormat, ...);	//	로그 추가(계속 누적됨)
	void					outputStaticLog(char *_lpstrFileName);	//	로그 출력

	void					resetLog();			//	로기 초기화
	BOOL					addLog(char *_lpstrFormat, ...);		//	로그 추가(아래 임시 로그로 사용)
	void					beginTemplateLog();	//	임시 로그 시작
	void					endTemplateLog();	//	임시 로그 종료
	void					setLauncher(void (*_lpLauncher)(void *lpInfo))	//	예외 상황이 발생했을때 호츌할 함수
	{
		s_fpLauncher	=	_lpLauncher;
	}

private:
	static	char			*s_pLog;
	static	s4				s_s4LogPointer,s_s4LogCount,s_s4LogBufferSize,s_aiTemplateLogPoint[c_s4EH_MaxTemplateLogCount][2],s_s4TemplateLogPointCount;
	static	s4				s_s4StaticLogPointer,s_s4StaticLogCount;

	static	u2				s_wGenerateYear,s_wGenerateMonth,s_wGenerateDay,s_wGenerateHour,s_wGenerateMinute,s_wGenerateSecond;
	static	BOOL			s_bIsExist;	//	싱글드다.
	static	void			(*s_fpLauncher)(void *lpInfo);

	static	TCHAR			s_strHeader[c_s4EH_HeaderLength];
	static	TCHAR			s_strStaticLog[c_s4EH_StaticLogBufferSize];
	static	TCHAR			s_strLogFileName[MAX_PATH];
	static	HANDLE			s_hReportFile;
	static	LPTOP_LEVEL_EXCEPTION_FILTER	s_previousFilter;

	static	char*			GetLog(s4 _s4Index);
	static	char*			GetStaticLog(s4 _s4Index);


	// entry point where control comes on an unhandled exception
	static LONG WINAPI		UnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo );

	// where report info is extracted and generated    
	static void				GenerateExceptionReport( PEXCEPTION_POINTERS pExceptionInfo );

	// Helper functions
	static	LPTSTR			GetExceptionString( DWORD dwCode );
	static	BOOL			GetLogicalAddress(PVOID addr, PTSTR szModule, DWORD len,DWORD& section, DWORD& offset );
	static	void			IntelStackWalk( PCONTEXT pContext );
	static	void			ImagehlpStackWalk( PCONTEXT pContext );
	static	s4 __cdecl		WriteLogToFile(const TCHAR * format, ...);

	static	BOOL			InitImagehlpFunctions( void );

	// Make typedefs for some IMAGEHLP.DLL functions so that we can use them
	// with GetProcAddress
	typedef	BOOL (__stdcall * SYMINITIALIZEPROC)( HANDLE, LPSTR, BOOL );
	typedef	BOOL (__stdcall *SYMCLEANUPPROC)( HANDLE );

	typedef	BOOL (__stdcall * STACKWALKPROC)( DWORD, HANDLE, HANDLE, LPSTACKFRAME, LPVOID,
				PREAD_PROCESS_MEMORY_ROUTINE,PFUNCTION_TABLE_ACCESS_ROUTINE,
				PGET_MODULE_BASE_ROUTINE, PTRANSLATE_ADDRESS_ROUTINE );

	typedef LPVOID (__stdcall *SYMFUNCTIONTABLEACCESSPROC)( HANDLE, DWORD );
	typedef DWORD (__stdcall *SYMGETMODULEBASEPROC)( HANDLE, DWORD );
	typedef BOOL (__stdcall *SYMGETSYMFROMADDRPROC)( HANDLE, DWORD, PDWORD, PIMAGEHLP_SYMBOL );

	static	SYMINITIALIZEPROC			s_symInitialize;
	static	SYMCLEANUPPROC				s_symCleanup;
	static	STACKWALKPROC				s_stackWalk;
	static	SYMFUNCTIONTABLEACCESSPROC	s_symFunctionTableAccess;
	static	SYMGETMODULEBASEPROC		s_symGetModuleBase;
	static	SYMGETSYMFROMADDRPROC		s_symGetSymFromAddr;
};

extern cExceptionHandler g_eh;  // global instance of class

class	cExceptionHandlerDoumi
{
public:
	cExceptionHandler	*m_lpHandler;

	cExceptionHandlerDoumi(cExceptionHandler *_lpHandler)
	{
		m_lpHandler	=	_lpHandler;

		m_lpHandler->beginTemplateLog();
	}

	~cExceptionHandlerDoumi()
	{
		m_lpHandler->endTemplateLog();
	}
};

#define	mBeginTLog()	cExceptionHandlerDoumi __templateExceptionHandler__(&g_eh);	//	임시 익셉션 핸들러 로그 시작
#define	mTL()			cExceptionHandlerDoumi __templateExceptionHandler__(&g_eh);	//	임시 익셉션 핸들러 로그 시작

#endif
#endif