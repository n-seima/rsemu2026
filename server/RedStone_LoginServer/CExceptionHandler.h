#ifndef __ClassExceptionHandler_H__
#define __ClassExceptionHandler_H__

#include <imagehlp.h>

#define	dEXCEPTION_HANDLER_HEADER_LENGTH	512
const	int	dDEFAULT_EXCEPTION_HANDLER_LOG_SIZE	=	1024*100;	//	100kb
#define	dMAX_TEMPLATE_LOG_COUNT				1024
#define	dSTATIC_LOG_BUFFER_SIZE				dDEFAULT_EXCEPTION_HANDLER_LOG_SIZE

class cExceptionHandler
{
public:
							cExceptionHandler();
							~cExceptionHandler();
     
	void					setLogFileName(PTSTR pszLogFileName);
	void					setHeader(char *_lpstrHeader);

	void					initLog(int _iSize=dDEFAULT_EXCEPTION_HANDLER_LOG_SIZE);
	BOOL					addLog(char *_lpstrFormat, ...);

	BOOL					addStaticLog(char *_lpstrFormat, ...);
	void					outputStaticLog(char *_lpstrFileName);

	void					resetLog();
	void					beginTemplateLog();
	void					endTemplateLog();
	void					setLauncher(void (*_lpLauncher)(void *lpInfo))
	{
		s_fpLauncher	=	_lpLauncher;
	}
	static	int __cdecl		WriteLogToFile(const TCHAR * format, ...);

private:
	static	char			*s_pLog;
	static	int				s_iLogPointer,s_iLogCount,s_iLogBufferSize,s_aiTemplateLogPoint[dMAX_TEMPLATE_LOG_COUNT][2],s_iTemplateLogPointCount;
	static	int				s_iStaticLogPointer,s_iStaticLogCount;

	static	WORD			s_wGenerateYear,s_wGenerateMonth,s_wGenerateDay,s_wGenerateHour,s_wGenerateMinute,s_wGenerateSecond;
	static	BOOL			s_bIsExist;	//	싱글드다.
	static	void			(*s_fpLauncher)(void *lpInfo);

	static	TCHAR			s_strHeader[dEXCEPTION_HANDLER_HEADER_LENGTH];
	static	TCHAR			s_strStaticLog[dSTATIC_LOG_BUFFER_SIZE];
	static	TCHAR			s_strLogFileName[MAX_PATH];
	static	HANDLE			s_hReportFile;
	static	LPTOP_LEVEL_EXCEPTION_FILTER	s_previousFilter;

	static	char*			GetLog(int _iIndex);
	static	char*			GetStaticLog(int _iIndex);


	// entry point where control comes on an unhandled exception
	static LONG WINAPI		UnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo );

	// where report info is extracted and generated    
	static void				GenerateExceptionReport( PEXCEPTION_POINTERS pExceptionInfo );

	// Helper functions
	static	LPTSTR			GetExceptionString( DWORD dwCode );
	static	BOOL			GetLogicalAddress(PVOID addr, PTSTR szModule, DWORD len,DWORD& section, DWORD& offset );
	static	void			IntelStackWalk( PCONTEXT pContext );
	static	void			ImagehlpStackWalk( PCONTEXT pContext );

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

#endif