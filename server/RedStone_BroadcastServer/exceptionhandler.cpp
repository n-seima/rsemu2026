#ifdef	WIN32
#include "winsock2.h"
#include "exceptionHandler.h"
#include "lnk_util.h"

#ifdef __AFXWIN_H__
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#endif

#include <windows.h>
#include <tchar.h>
#include <time.h>
#include <stdio.h>

//============================== Global Variables =============================

//
// Declare the static variables of the cExceptionHandler class
//
TCHAR											cExceptionHandler::s_strHeader[c_s4EH_HeaderLength];
TCHAR											cExceptionHandler::s_strLogFileName[MAX_PATH];
TCHAR											cExceptionHandler::s_strStaticLog[c_s4EH_StaticLogBufferSize];
LPTOP_LEVEL_EXCEPTION_FILTER					cExceptionHandler::s_previousFilter;
HANDLE											cExceptionHandler::s_hReportFile;
BOOL											cExceptionHandler::s_bIsExist				=	FALSE;
char											*cExceptionHandler::s_pLog					=	NULL;


cExceptionHandler::SYMINITIALIZEPROC			cExceptionHandler::s_symInitialize			=	0;
cExceptionHandler::SYMCLEANUPPROC				cExceptionHandler::s_symCleanup				=	0;
cExceptionHandler::STACKWALKPROC				cExceptionHandler::s_stackWalk				=	0;
cExceptionHandler::SYMFUNCTIONTABLEACCESSPROC	cExceptionHandler::s_symFunctionTableAccess	=	0;
cExceptionHandler::SYMGETMODULEBASEPROC			cExceptionHandler::s_symGetModuleBase		=	0;
cExceptionHandler::SYMGETSYMFROMADDRPROC		cExceptionHandler::s_symGetSymFromAddr		=	0;

u2												cExceptionHandler::s_wGenerateYear			=	0;
u2												cExceptionHandler::s_wGenerateMonth			=	0;
u2												cExceptionHandler::s_wGenerateDay			=	0;
u2												cExceptionHandler::s_wGenerateHour			=	0;
u2												cExceptionHandler::s_wGenerateMinute		=	0;
u2												cExceptionHandler::s_wGenerateSecond		=	0;
s4												cExceptionHandler::s_s4LogBufferSize			=	0;
s4												cExceptionHandler::s_s4LogPointer			=	0;
s4												cExceptionHandler::s_s4LogCount				=	0;
s4												cExceptionHandler::s_aiTemplateLogPoint[c_s4EH_MaxTemplateLogCount][2];
s4												cExceptionHandler::s_s4TemplateLogPointCount	=	0;
s4												cExceptionHandler::s_s4StaticLogPointer		=	0;
s4												cExceptionHandler::s_s4StaticLogCount		=	0;

void											(*cExceptionHandler::s_fpLauncher)(void*)	=	NULL;

cExceptionHandler								g_eh;  // Declare global instance of class

//============================== Class Methods =============================

//
//	생성자
cExceptionHandler::cExceptionHandler( )
{
	if (s_bIsExist)
	{
		MessageBox(GetActiveWindow(),"is already exist!!","construct cExceptionHandler Error!!",MB_ICONERROR|MB_TOPMOST);
		return;
	}
	// Install the unhandled exception filter function
	s_previousFilter	=	SetUnhandledExceptionFilter(UnhandledExceptionFilter);

	// Figure out what the report file will be named, and store it away
	GetModuleFileName(0,s_strLogFileName, MAX_PATH );

	//	실행화일 이름을 얻어온후에 확장자를 RPT로 바꿔서 리포트 화일을 만든다.
	PTSTR	pszDot	=	_tcsrchr( s_strLogFileName, _T('.') );

	if ( pszDot )
	{
		pszDot++;   // Advance past the '.'
		if ( _tcslen(pszDot) >= 3 )
			_tcscpy( pszDot, _T("RPT") );   // "RPT" -> "Report"
	}

	s_bIsExist			=	TRUE;
	s_s4LogBufferSize	=	0;

	
	time_t		tTime;
	struct tm	*tmTime;

	tTime				=	time(NULL);
	tmTime				=	localtime(&tTime);

	s_wGenerateYear		=	1900+tmTime->tm_year;
	s_wGenerateMonth	=	tmTime->tm_mon+1;
	s_wGenerateDay		=	tmTime->tm_mday;
	s_wGenerateHour		=	tmTime->tm_hour;
	s_wGenerateMinute	=	tmTime->tm_min;
	s_wGenerateSecond	=	tmTime->tm_sec;

	s_pLog				=	new char [c_s4EH_DefaultLogSize];
	s_s4LogPointer		=	0;
	s_s4LogCount			=	0;
	s_s4LogBufferSize	=	c_s4EH_DefaultLogSize;
	m_iCurrentLine		=	-1;

	InitializeCriticalSection(&m_cs);
}

//
//	파괴자
cExceptionHandler::~cExceptionHandler( )
{
    SetUnhandledExceptionFilter( s_previousFilter );

	if	(s_pLog)
		delete [] s_pLog;

	DeleteCriticalSection(&m_cs);
}

//
//	헤더 
void
cExceptionHandler::setHeader(char *_lpstrHeader)
{
	_tcscpy( s_strHeader, _lpstrHeader);
}

void
cExceptionHandler::outputStaticLog(char *_lpstrFileName)
{
	FILE	*fp;

	SetFileAttributes(_lpstrFileName,FILE_ATTRIBUTE_ARCHIVE);

	fp	=	fopen(_lpstrFileName,"wb");

	if	(!fp)
		return;

	time_t	tTime;
	struct tm *tmTime;

	tTime				=	time(NULL);
	tmTime				=	localtime(&tTime);

	u2 wGenerateYear	=	1900+tmTime->tm_year;
	u2 wGenerateMonth	=	tmTime->tm_mon+1;
	u2 wGenerateDay		=	tmTime->tm_mday;
	u2 wGenerateHour	=	tmTime->tm_hour;
	u2 wGenerateMinute	=	tmTime->tm_min;
	u2 wGenerateSecond	=	tmTime->tm_sec;

    fprintf(fp,"//========================================================================================================\xd\xa");
    fprintf(fp,"Application Begin Time %4d/%2d/%2d %2d:%2d:%2d\xd\xa",s_wGenerateYear,s_wGenerateMonth,s_wGenerateDay,s_wGenerateHour,s_wGenerateMinute,s_wGenerateSecond);
    fprintf(fp,"Current Time           %4d/%2d/%2d %2d:%2d:%2d\xd\xa",wGenerateYear,wGenerateMonth,wGenerateDay,wGenerateHour,wGenerateMinute,wGenerateSecond);
    fprintf(fp,"\xd\xa");

	if	(s_strHeader[0])
		fprintf(fp,"%s\xd\xa",s_strHeader);

	if	(s_s4StaticLogCount)
	{
		for(s4 i=0;;i++)
		{
			char	*lpstrLog	=	GetStaticLog(i);

			if (!lpstrLog)	
				break;

			fprintf(fp,"%s\xd\xa",lpstrLog);
		}
	}
    fprintf(fp,"//========================================================================================================\xd\xa");
	fclose(fp);
}

//	현재 파일과 라인을 남긴다.
void
cExceptionHandler::remainCodeInfo(char *_lpstrFileName,int _iLineIndex)
{
	mCS(m_cs);
	m_iCurrentLine	=	_iLineIndex;
	strcpy(m_strCurrentFile,_lpstrFileName);
}

void
cExceptionHandler::clearRemainCodeInfo()
{
	mCS(m_cs);
	m_iCurrentLine	=	-1;
}


//
//	원한다면 로그화일 이름을 바꿔 줄수도 있지.
void
cExceptionHandler::setLogFileName( PTSTR pszLogFileName )
{
    _tcscpy( s_strLogFileName, pszLogFileName );
}

void
cExceptionHandler::initLog(s4 _s4Size)
{	
	if	(s_pLog	)
	{
		delete [] s_pLog;

		s_pLog	=	NULL;
	}

	if	(_s4Size)
		s_pLog		=	new char [_s4Size];

	s_s4LogPointer	=	0;
	s_s4LogBufferSize=	_s4Size;
}

BOOL __cdecl
cExceptionHandler::addLog(char *_lpstrFormat, ...)
{
    char	szBuff[1024*10];
    va_list	argptr;

    va_start(argptr,_lpstrFormat);
    vsprintf(szBuff,_lpstrFormat,argptr);
    va_end(argptr);

	s4		iLength	=	strlen(szBuff)+1+2;
	char	*lpData	=	s_pLog+s_s4LogPointer;

	if	(s_s4LogPointer+iLength >= s_s4LogBufferSize)
		return	FALSE;

	*(u2 *)lpData	=	(u2)(iLength);

	strcpy(lpData+2,szBuff);

	s_s4LogCount++;
	s_s4LogPointer	+=	iLength;

	return	TRUE;
}

BOOL __cdecl
cExceptionHandler::addStaticLog(char *_lpstrFormat, ...)
{
    char	szBuff[1024*10];
    va_list	argptr;

    va_start(argptr,_lpstrFormat);
    vsprintf(szBuff,_lpstrFormat,argptr);
    va_end(argptr);

	s4		iLength	=	strlen(szBuff)+1+2;
	char	*lpData	=	s_strStaticLog+s_s4StaticLogPointer;

	if	(s_s4LogPointer+iLength >= c_s4EH_StaticLogBufferSize)
		return	FALSE;

	*(u2 *)lpData	=	(u2)(iLength);

	strcpy(lpData+2,szBuff);

	s_s4StaticLogCount++;
	s_s4StaticLogPointer	+=	iLength;

	return	FALSE;
}

void
cExceptionHandler::beginTemplateLog()
{
	if	(s_s4TemplateLogPointCount	>=	c_s4EH_MaxTemplateLogCount)
		resetLog();

	s_aiTemplateLogPoint[s_s4TemplateLogPointCount][0]	=	s_s4LogPointer;
	s_aiTemplateLogPoint[s_s4TemplateLogPointCount][1]	=	s_s4LogCount;

	s_s4TemplateLogPointCount++;
}

void
cExceptionHandler::endTemplateLog()
{
	s_s4LogPointer	=	s_aiTemplateLogPoint[s_s4TemplateLogPointCount-1][0];
	s_s4LogCount		=	s_aiTemplateLogPoint[s_s4TemplateLogPointCount-1][1];

	s_s4TemplateLogPointCount--;
}

char*
cExceptionHandler::GetLog(s4 _s4Index)
{
	if	(_s4Index >= s_s4LogCount)
		return	NULL;

	s4	iPointer=	0;

	for (s4 i=0;i<_s4Index;i++)
	{
		u2	wSize	=	*(u2 *)(s_pLog+iPointer);

		iPointer		+=	wSize;
	}

	return	s_pLog+iPointer+2;
}

char*
cExceptionHandler::GetStaticLog(s4 _s4Index)
{
	if	(_s4Index >= s_s4StaticLogCount)
		return	NULL;

	s4	iPointer=	0;

	for (s4 i=0;i<_s4Index;i++)
	{
		u2	wSize	=	*(u2 *)(s_strStaticLog+iPointer);

		iPointer		+=	wSize;
	}

	return	s_strStaticLog+iPointer+2;
}

void
cExceptionHandler::resetLog()
{
	s_s4LogPointer			=	0;
	s_s4LogCount				=	0;
	s_s4TemplateLogPointCount=	0;
}

//===========================================================
// Entry point where control comes on an unhandled exception 
//===========================================================
LONG WINAPI
cExceptionHandler::UnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo )
{
#ifndef	_DEBUG
	DeleteFile(s_strLogFileName);
#endif	
	s_hReportFile	=	CreateFile( s_strLogFileName,
									GENERIC_WRITE,
									0,
									0,
									OPEN_ALWAYS,
									FILE_FLAG_WRITE_THROUGH,
									0 );

	if ( s_hReportFile )
	{
		SetFilePointer( s_hReportFile, 0, 0, FILE_END );

		GenerateExceptionReport( pExceptionInfo );

		CloseHandle( s_hReportFile );

		s_hReportFile	=	0;
	}

#ifndef	_DEBUG
	s_fpLauncher(NULL);
#endif	

	if ( s_previousFilter )
		return s_previousFilter( pExceptionInfo );

	return	EXCEPTION_EXECUTE_HANDLER;
}

//===========================================================================
// Open the report file, and write the desired information to it.  Called by 
// UnhandledExceptionFilter                                               
//===========================================================================
void
cExceptionHandler::GenerateExceptionReport(PEXCEPTION_POINTERS pExceptionInfo )
{
	time_t	tTime;
	struct tm *tmTime;

	tTime				=	time(NULL);
	tmTime				=	localtime(&tTime);

	u2 wGenerateYear	=	1900+tmTime->tm_year;
	u2 wGenerateMonth	=	tmTime->tm_mon+1;
	u2 wGenerateDay		=	tmTime->tm_mday;
	u2 wGenerateHour	=	tmTime->tm_hour;
	u2 wGenerateMinute	=	tmTime->tm_min;
	u2 wGenerateSecond	=	tmTime->tm_sec;

    // Start out with a banner
    WriteLogToFile(_T("//========================================================================================================\n") );
    WriteLogToFile(_T("Application Begin Time          %4d/%2d/%2d %2d:%2d:%2d\n"),s_wGenerateYear,s_wGenerateMonth,s_wGenerateDay,s_wGenerateHour,s_wGenerateMinute,s_wGenerateSecond);
    WriteLogToFile(_T("Exception Handler Activate Time %4d/%2d/%2d %2d:%2d:%2d\n"),wGenerateYear,wGenerateMonth,wGenerateDay,wGenerateHour,wGenerateMinute,wGenerateSecond);

    WriteLogToFile(_T("\n"));
    PEXCEPTION_RECORD pExceptionRecord	=	pExceptionInfo->ExceptionRecord;

	if (s_strHeader[0])
		WriteLogToFile(_T("%s\n"),s_strHeader);

	if (s_s4StaticLogCount)
	{
		WriteLogToFile(_T("===== normal log\n"));
		for(s4 i=0;;i++)
		{
			char	*lpstrLog	=	GetStaticLog(i);

			if (!lpstrLog)	
				break;

			WriteLogToFile("%s\n",lpstrLog);
		}

		WriteLogToFile(_T("\n"));
	}

	if (s_s4LogCount)
	{
		WriteLogToFile(_T("===== instant log\n"));
		for(s4 i=0;;i++)
		{
			char	*lpstrLog	=	GetLog(i);

			if (!lpstrLog)	break;

			WriteLogToFile(_T("%s\n"),lpstrLog);
		}

		WriteLogToFile(_T("\n"));
	}

    // First print information about the type of fault
    WriteLogToFile(_T("Exception code: %08X %s\n"),pExceptionRecord->ExceptionCode,GetExceptionString(pExceptionRecord->ExceptionCode) );

    // Now print information about where the fault occured
    TCHAR	szFaultingModule[MAX_PATH];
    DWORD	dwSection, dwOffset;

    GetLogicalAddress(  pExceptionRecord->ExceptionAddress,szFaultingModule,sizeof( szFaultingModule ),dwSection, dwOffset );

    WriteLogToFile( _T("Fault address:  %08X %02X:%08X %s\n"),pExceptionRecord->ExceptionAddress,dwSection, dwOffset, szFaultingModule );

    PCONTEXT pCtx = pExceptionInfo->ContextRecord;

    // Show the registers
#ifdef _M_IX86  // Intel Only!
    WriteLogToFile(_T("\nRegisters:\n") );
    WriteLogToFile(_T("EAX:%08X\nEBX:%08X\nECX:%08X\nEDX:%08X\nESI:%08X\nEDI:%08X\n"),pCtx->Eax, pCtx->Ebx, pCtx->Ecx, pCtx->Edx, pCtx->Esi, pCtx->Edi );
    WriteLogToFile(_T("CS:EIP:%04X:%08X\n"), pCtx->SegCs, pCtx->Eip );
    WriteLogToFile(_T("SS:ESP:%04X:%08X  EBP:%08X\n"),pCtx->SegSs, pCtx->Esp, pCtx->Ebp );
    WriteLogToFile(_T("DS:%04X  ES:%04X  FS:%04X  GS:%04X\n"),pCtx->SegDs, pCtx->SegEs, pCtx->SegFs, pCtx->SegGs );
    WriteLogToFile(_T("Flags:%08X\n"), pCtx->EFlags );
#endif

	if (!InitImagehlpFunctions() )
	{
		OutputDebugString(_T("IMAGEHLP.DLL or its exported procs not found"));

#ifdef _M_IX86  // Intel Only!
		// Walk the stack using x86 specific code
		IntelStackWalk( pCtx );
#endif
		return;
	}

	ImagehlpStackWalk( pCtx );

	s_symCleanup( GetCurrentProcess() );

	WriteLogToFile( _T("\n") );
}

//======================================================================
// Given an exception code, returns a pointer to a static string with a 
// description of the exception                                         
//======================================================================
LPTSTR
cExceptionHandler::GetExceptionString( DWORD dwCode )
{
    #define EXCEPTION( x ) case EXCEPTION_##x: return _T(#x);

    switch ( dwCode )
    {
        EXCEPTION( ACCESS_VIOLATION )
        EXCEPTION( DATATYPE_MISALIGNMENT )
        EXCEPTION( BREAKPOINT )
        EXCEPTION( SINGLE_STEP )
        EXCEPTION( ARRAY_BOUNDS_EXCEEDED )
        EXCEPTION( FLT_DENORMAL_OPERAND )
        EXCEPTION( FLT_DIVIDE_BY_ZERO )
        EXCEPTION( FLT_INEXACT_RESULT )
        EXCEPTION( FLT_INVALID_OPERATION )
        EXCEPTION( FLT_OVERFLOW )
        EXCEPTION( FLT_STACK_CHECK )
        EXCEPTION( FLT_UNDERFLOW )
        EXCEPTION( INT_DIVIDE_BY_ZERO )
        EXCEPTION( INT_OVERFLOW )
        EXCEPTION( PRIV_INSTRUCTION )
        EXCEPTION( IN_PAGE_ERROR )
        EXCEPTION( ILLEGAL_INSTRUCTION )
        EXCEPTION( NONCONTINUABLE_EXCEPTION )
        EXCEPTION( STACK_OVERFLOW )
        EXCEPTION( INVALID_DISPOSITION )
        EXCEPTION( GUARD_PAGE )
        EXCEPTION( INVALID_HANDLE )
    }

    // If not one of the "known" exceptions, try to get the string
    // from NTDLL.DLL's message table.

    static TCHAR szBuffer[512] = { 0 };

    FormatMessage(  FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_HMODULE,GetModuleHandle( _T("NTDLL.DLL") ),dwCode, 0, szBuffer, sizeof( szBuffer ), 0 );

    return szBuffer;
}

//==============================================================================
// Given a linear address, locates the module, section, and offset containing  
// that address.                                                               
//                                                                             
// Note: the szModule paramater buffer is an output buffer of length specified 
// by the len parameter (in characters!)                                       
//==============================================================================
BOOL
cExceptionHandler::GetLogicalAddress(PVOID addr, PTSTR szModule, DWORD len, DWORD& section, DWORD& offset )
{
	MEMORY_BASIC_INFORMATION mbi;

	if ( !VirtualQuery( addr, &mbi, sizeof(mbi) ) )
		return FALSE;

	DWORD hMod = (DWORD)mbi.AllocationBase;

	if ( !GetModuleFileName( (HMODULE)hMod, szModule, len ) )
		return FALSE;

	// Point to the DOS header in memory
	PIMAGE_DOS_HEADER		pDosHdr		=	(PIMAGE_DOS_HEADER)hMod;

	// From the DOS header, find the NT (PE) header
	PIMAGE_NT_HEADERS		pNtHdr		=	(PIMAGE_NT_HEADERS)(hMod + pDosHdr->e_lfanew);
	PIMAGE_SECTION_HEADER	pSection	=	IMAGE_FIRST_SECTION( pNtHdr );

	DWORD	rva	=	(DWORD)addr - hMod; // RVA is offset from module load address

	// Iterate through the section table, looking for the one that encompasses
	// the linear address.
	for	(unsigned i = 0;i < pNtHdr->FileHeader.NumberOfSections;i++, pSection++ )
	{
		DWORD sectionStart = pSection->VirtualAddress;
		DWORD sectionEnd = sectionStart + max(pSection->SizeOfRawData, pSection->Misc.VirtualSize);

		// Is the address in this section???
		if ( (rva >= sectionStart) && (rva <= sectionEnd) )
		{
			// Yes, address is in the section.  Calculate section and offset,
			// and store in the "section" & "offset" params, which were
			// passed by reference.
			section = i+1;
			offset = rva - sectionStart;
			return TRUE;
		}
	}

	return FALSE;   // Should never get here!
}

//============================================================
// Walks the stack, and writes the results to the report file 
//============================================================
void
cExceptionHandler::IntelStackWalk( PCONTEXT pContext )
{
    WriteLogToFile( _T("\nCall stack:\n") );

    WriteLogToFile( _T("Address   Frame     Logical addr  Module\n") );

    DWORD pc = pContext->Eip;
    PDWORD pFrame, pPrevFrame;
    
    pFrame = (PDWORD)pContext->Ebp;

    do
    {
        TCHAR szModule[MAX_PATH] = _T("");
        DWORD section = 0, offset = 0;

        GetLogicalAddress((PVOID)pc, szModule,sizeof(szModule),section,offset );

        WriteLogToFile( _T("%08X  %08X  %04X:%08X %s\n"),
                  pc, pFrame, section, offset, szModule );

        pc = pFrame[1];

        pPrevFrame = pFrame;

        pFrame = (PDWORD)pFrame[0]; // proceed to next higher frame on stack

        if ( (DWORD)pFrame & 3 )    // Frame pointer must be aligned on a
            break;                  // DWORD boundary.  Bail if not so.

        if ( pFrame <= pPrevFrame )
            break;

        // Can two DWORDs be read from the supposed frame address?          
        if ( IsBadWritePtr(pFrame, sizeof(PVOID)*2) )
            break;

    } while ( 1 );
}

//============================================================
// Walks the stack, and writes the results to the report file 
//============================================================
void
cExceptionHandler::ImagehlpStackWalk( PCONTEXT pContext )
{
    WriteLogToFile(_T("\nCall stack:\n") );
    WriteLogToFile(_T("Address   Frame\n") );

    // Could use SymSetOptions here to add the SYMOPT_DEFERRED_LOADS flag

    STACKFRAME sf;
    memset( &sf, 0, sizeof(sf) );

    // Initialize the STACKFRAME structure for the first call.  This is only
    // necessary for Intel CPUs, and isn't mentioned in the documentation.
    sf.AddrPC.Offset       = pContext->Eip;
    sf.AddrPC.Mode         = AddrModeFlat;
    sf.AddrStack.Offset    = pContext->Esp;
    sf.AddrStack.Mode      = AddrModeFlat;
    sf.AddrFrame.Offset    = pContext->Ebp;
    sf.AddrFrame.Mode      = AddrModeFlat;

    while ( 1 )
    {
        if ( ! s_stackWalk(  IMAGE_FILE_MACHINE_I386,
                            GetCurrentProcess(),
                            GetCurrentThread(),
                            &sf,
                            pContext,
                            0,
                            s_symFunctionTableAccess,
                            s_symGetModuleBase,
                            0 ) )
            break;

        if	( 0 == sf.AddrFrame.Offset ) // Basic sanity check to make sure
            break;                      // the frame is OK.  Bail if not.

        WriteLogToFile( _T("%08X  %08X  "), sf.AddrPC.Offset, sf.AddrFrame.Offset );

        // IMAGEHLP is wacky, and requires you to pass in a pointer to an
        // IMAGEHLP_SYMBOL structure.  The problem is that this structure is
        // variable length.  That is, you determine how big the structure is
        // at runtime.  This means that you can't use sizeof(struct).
        // So...make a buffer that's big enough, and make a pointer
        // to the buffer.  We also need to initialize not one, but TWO
        // members of the structure before it can be used.

        BYTE symbolBuffer[ sizeof(IMAGEHLP_SYMBOL) + 512 ];
        PIMAGEHLP_SYMBOL pSymbol = (PIMAGEHLP_SYMBOL)symbolBuffer;
        pSymbol->SizeOfStruct = sizeof(symbolBuffer);
        pSymbol->MaxNameLength = 512;
                        
        DWORD symDisplacement = 0;  // Displacement of the input address,
                                    // relative to the start of the symbol

        if ( s_symGetSymFromAddr(GetCurrentProcess(), sf.AddrPC.Offset,
                                &symDisplacement, pSymbol) )
        {
            WriteLogToFile( _T("%hs+%X\n"), pSymbol->Name, symDisplacement );
            
        }
        else    // No symbol found.  Print out the logical address instead.
        {
            TCHAR szModule[MAX_PATH] = _T("");
            DWORD section = 0, offset = 0;

            GetLogicalAddress(  (PVOID)sf.AddrPC.Offset,
                                szModule, sizeof(szModule), section, offset );

            WriteLogToFile( _T("%04X:%08X %s\n"),
                      section, offset, szModule );
        }
    }

}

//============================================================================
// Helper function that writes to the report file, and allows the user to use 
// printf style formating                                                     
//============================================================================
s4 __cdecl
cExceptionHandler::WriteLogToFile(const TCHAR * format, ...)
{
    TCHAR	szBuff[1024];
    s4		retValue;
    DWORD	cbWritten;
    va_list argptr;

    va_start( argptr, format );
    retValue	=	wvsprintf( szBuff, format, argptr );
    va_end( argptr );

    WriteFile( s_hReportFile, szBuff, retValue * sizeof(TCHAR), &cbWritten, 0 );

    return retValue;
}	//	cExceptionHandler::WriteLogToFile(const TCHAR * format, ...)


//=========================================================================
// Load IMAGEHLP.DLL and get the address of functions in it that we'll use 
//=========================================================================
BOOL
cExceptionHandler::InitImagehlpFunctions( void )
{
    HMODULE hModImagehlp = LoadLibrary( _T("IMAGEHLP.DLL") );

    if ( !hModImagehlp )
        return FALSE;

    s_symInitialize = (SYMINITIALIZEPROC)GetProcAddress( hModImagehlp,"SymInitialize" );
    if ( !s_symInitialize )
        return FALSE;

    s_symCleanup = (SYMCLEANUPPROC)GetProcAddress( hModImagehlp, "SymCleanup" );
    if ( !s_symCleanup )
        return FALSE;

    s_stackWalk = (STACKWALKPROC)GetProcAddress( hModImagehlp, "StackWalk" );
    if ( !s_stackWalk )
        return FALSE;

    s_symFunctionTableAccess = (SYMFUNCTIONTABLEACCESSPROC)GetProcAddress( hModImagehlp, "SymFunctionTableAccess" );

    if ( !s_symFunctionTableAccess )
        return FALSE;

    s_symGetModuleBase	=	(SYMGETMODULEBASEPROC)GetProcAddress( hModImagehlp,"SymGetModuleBase");

    if ( !s_symGetModuleBase )
        return FALSE;

    s_symGetSymFromAddr	=	(SYMGETSYMFROMADDRPROC)GetProcAddress( hModImagehlp,"SymGetSymFromAddr" );

    if ( !s_symGetSymFromAddr )
        return FALSE;

    if ( !s_symInitialize( GetCurrentProcess(), 0, TRUE ) )
        return FALSE;

    return TRUE;        
}
#endif