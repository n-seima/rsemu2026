#include "winsock2.h"
#include "cExceptionHandler.h"
//#include "SFC.h"

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
TCHAR											cExceptionHandler::s_strHeader[dEXCEPTION_HANDLER_HEADER_LENGTH];
TCHAR											cExceptionHandler::s_strLogFileName[MAX_PATH];
TCHAR											cExceptionHandler::s_strStaticLog[dSTATIC_LOG_BUFFER_SIZE];
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

WORD											cExceptionHandler::s_wGenerateYear			=	0;
WORD											cExceptionHandler::s_wGenerateMonth			=	0;
WORD											cExceptionHandler::s_wGenerateDay			=	0;
WORD											cExceptionHandler::s_wGenerateHour			=	0;
WORD											cExceptionHandler::s_wGenerateMinute		=	0;
WORD											cExceptionHandler::s_wGenerateSecond		=	0;
int												cExceptionHandler::s_iLogBufferSize			=	0;
int												cExceptionHandler::s_iLogPointer			=	0;
int												cExceptionHandler::s_iLogCount				=	0;
int												cExceptionHandler::s_aiTemplateLogPoint[dMAX_TEMPLATE_LOG_COUNT][2];
int												cExceptionHandler::s_iTemplateLogPointCount	=	0;
int												cExceptionHandler::s_iStaticLogPointer		=	0;
int												cExceptionHandler::s_iStaticLogCount		=	0;

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
	s_iLogBufferSize	=	0;

	
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

	s_pLog				=	new char [dDEFAULT_EXCEPTION_HANDLER_LOG_SIZE];
	s_iLogPointer		=	0;
	s_iLogCount			=	0;
	s_iLogBufferSize	=	dDEFAULT_EXCEPTION_HANDLER_LOG_SIZE;
}

//
//	파괴자
cExceptionHandler::~cExceptionHandler( )
{
    SetUnhandledExceptionFilter( s_previousFilter );

	if (s_pLog)
		delete [] s_pLog;
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

	fp	=	fopen(_lpstrFileName,"wb");

//	cFILE	file;
//
//	if (!file.Open(_lpstrFileName,"wb"))
//		return;

	time_t	tTime;
	struct tm *tmTime;

	tTime				=	time(NULL);
	tmTime				=	localtime(&tTime);

	WORD wGenerateYear	=	1900+tmTime->tm_year;
	WORD wGenerateMonth	=	tmTime->tm_mon+1;
	WORD wGenerateDay	=	tmTime->tm_mday;
	WORD wGenerateHour	=	tmTime->tm_hour;
	WORD wGenerateMinute=	tmTime->tm_min;
	WORD wGenerateSecond=	tmTime->tm_sec;

    fprintf(fp,"//========================================================================================================\xd\xa");
    fprintf(fp,"Application Begin Time %4d/%2d/%2d %2d:%2d:%2d\xd\xa",s_wGenerateYear,s_wGenerateMonth,s_wGenerateDay,s_wGenerateHour,s_wGenerateMinute,s_wGenerateSecond);
    fprintf(fp,"Current Time           %4d/%2d/%2d %2d:%2d:%2d\xd\xa",wGenerateYear,wGenerateMonth,wGenerateDay,wGenerateHour,wGenerateMinute,wGenerateSecond);
    fprintf(fp,"\xd\xa");

	if (s_strHeader[0])
		fprintf(fp,"%s\xd\xa",s_strHeader);

	if (s_iStaticLogCount)
	{
		for(int i=0;;i++)
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

//
//	원한다면 로그화일 이름을 바꿔 줄수도 있지.
void
cExceptionHandler::setLogFileName( PTSTR pszLogFileName )
{
    _tcscpy( s_strLogFileName, pszLogFileName );
}

void
cExceptionHandler::initLog(int _iSize)
{	
	if (s_pLog	)
	{
		delete [] s_pLog;

		s_pLog	=	NULL;
	}

	if (_iSize)
		s_pLog		=	new char [_iSize];

	s_iLogPointer	=	0;
	s_iLogBufferSize=	_iSize;
}

BOOL
cExceptionHandler::addLog(char *_lpstrFormat, ...)
{
    char	szBuff[1024*10];
    va_list	argptr;

    va_start(argptr,_lpstrFormat);
    vsprintf(szBuff,_lpstrFormat,argptr);
    va_end(argptr);

	int		iLength	=	strlen(szBuff)+1+2;
	char	*lpData	=	s_pLog+s_iLogPointer;

	if (s_iLogPointer+iLength >= s_iLogBufferSize)
		return	FALSE;

	*(WORD *)lpData	=	(WORD)(iLength);

	strcpy(lpData+2,szBuff);

	s_iLogCount++;
	s_iLogPointer	+=	iLength;

	return	TRUE;
}

BOOL
cExceptionHandler::addStaticLog(char *_lpstrFormat, ...)
{
    char	szBuff[1024*10];
    va_list	argptr;

    va_start(argptr,_lpstrFormat);
    vsprintf(szBuff,_lpstrFormat,argptr);
    va_end(argptr);

	int		iLength	=	strlen(szBuff)+1+2;
	char	*lpData	=	s_strStaticLog+s_iStaticLogPointer;

	if (s_iLogPointer+iLength >= dSTATIC_LOG_BUFFER_SIZE)
		return	FALSE;

	*(WORD *)lpData	=	(WORD)(iLength);

	strcpy(lpData+2,szBuff);

	s_iStaticLogCount++;
	s_iStaticLogPointer	+=	iLength;

	return	FALSE;
}

void
cExceptionHandler::beginTemplateLog()
{
	if (s_iTemplateLogPointCount	>=	dMAX_TEMPLATE_LOG_COUNT)
		resetLog();

	s_aiTemplateLogPoint[s_iTemplateLogPointCount][0]	=	s_iLogPointer;
	s_aiTemplateLogPoint[s_iTemplateLogPointCount][1]	=	s_iLogCount;

	s_iTemplateLogPointCount++;
}

void
cExceptionHandler::endTemplateLog()
{
	s_iLogPointer	=	s_aiTemplateLogPoint[s_iTemplateLogPointCount-1][0];
	s_iLogCount		=	s_aiTemplateLogPoint[s_iTemplateLogPointCount-1][1];

	s_iTemplateLogPointCount--;
}

char*
cExceptionHandler::GetLog(int _iIndex)
{
	if (_iIndex >= s_iLogCount)	return	NULL;

	int	iPointer=	0;

	for (int i=0;i<_iIndex;i++)
	{
		WORD	wSize	=	*(WORD *)(s_pLog+iPointer);

		iPointer		+=	wSize;
	}

	return	s_pLog+iPointer+2;
}

char*
cExceptionHandler::GetStaticLog(int _iIndex)
{
	if (_iIndex >= s_iStaticLogCount)	return	NULL;

	int	iPointer=	0;

	for (int i=0;i<_iIndex;i++)
	{
		WORD	wSize	=	*(WORD *)(s_strStaticLog+iPointer);

		iPointer		+=	wSize;
	}

	return	s_strStaticLog+iPointer+2;
}

void
cExceptionHandler::resetLog()
{
	s_iLogPointer			=	0;
	s_iLogCount				=	0;
	s_iTemplateLogPointCount=	0;
}

//===========================================================
// Entry point where control comes on an unhandled exception 
//===========================================================
LONG WINAPI
cExceptionHandler::UnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo )
{
	if	(s_fpLauncher)
		s_fpLauncher(NULL);

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

	WORD wGenerateYear	=	1900+tmTime->tm_year;
	WORD wGenerateMonth	=	tmTime->tm_mon+1;
	WORD wGenerateDay	=	tmTime->tm_mday;
	WORD wGenerateHour	=	tmTime->tm_hour;
	WORD wGenerateMinute=	tmTime->tm_min;
	WORD wGenerateSecond=	tmTime->tm_sec;

    // Start out with a banner
    WriteLogToFile(_T("//========================================================================================================\n") );
    WriteLogToFile(_T("Application Begin Time          %4d/%2d/%2d %2d:%2d:%2d\n"),s_wGenerateYear,s_wGenerateMonth,s_wGenerateDay,s_wGenerateHour,s_wGenerateMinute,s_wGenerateSecond);
    WriteLogToFile(_T("Exception Handler Activate Time %4d/%2d/%2d %2d:%2d:%2d\n"),wGenerateYear,wGenerateMonth,wGenerateDay,wGenerateHour,wGenerateMinute,wGenerateSecond);

    WriteLogToFile(_T("\n"));
    PEXCEPTION_RECORD pExceptionRecord	=	pExceptionInfo->ExceptionRecord;

	if (s_strHeader[0])
		WriteLogToFile(_T("%s\n"),s_strHeader);

	if (s_iStaticLogCount)
	{
		WriteLogToFile(_T("===== normal log\n"));
		for(int i=0;;i++)
		{
			char	*lpstrLog	=	GetStaticLog(i);

			if (!lpstrLog)	
				break;

			WriteLogToFile("%s\n",lpstrLog);
		}

		WriteLogToFile(_T("\n"));
	}

	if (s_iLogCount)
	{
		WriteLogToFile(_T("===== instant log\n"));
		for(int i=0;;i++)
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

	if	(!InitImagehlpFunctions() )
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

        if ( 0 == sf.AddrFrame.Offset ) // Basic sanity check to make sure
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
int __cdecl
cExceptionHandler::WriteLogToFile(const TCHAR * format, ...)
{
    TCHAR	szBuff[1024];
    TCHAR	szOutput[1024];
    int		retValue;
    DWORD	cbWritten;
    va_list argptr;

    va_start( argptr, format );
    retValue	=	wvsprintf( szBuff, format, argptr );
    va_end( argptr );

	for (int i=0,iPos=0;szBuff[i];i++)
	{
		if	(szBuff[i]	==	10)
		{
			szOutput[iPos++]	=	13;
			szOutput[iPos++]	=	10;
		}
		else
			szOutput[iPos++]	=	szBuff[i];
	}

	szOutput[iPos]	=	NULL;

    WriteFile( s_hReportFile, szOutput, iPos * sizeof(TCHAR), &cbWritten, 0 );

    return iPos;
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