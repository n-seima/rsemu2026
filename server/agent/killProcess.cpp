#include "stdafx.h"
#include "killProcess.h"
#include <tlhelp32.h>

BOOL
IsActiveProcess(char *_lpstrProcessName)
{
    HANDLE         hProcessSnap = NULL;
    BOOL			Return       = FALSE;
    PROCESSENTRY32 pe32         = {0};

    hProcessSnap	=	CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if	(hProcessSnap == INVALID_HANDLE_VALUE)
		return	FALSE;

    pe32.dwSize	=	sizeof(PROCESSENTRY32);

    if	(Process32First(hProcessSnap, &pe32))
    {
        do
		{
            HANDLE hProcess	=	OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);

            GetPriorityClass(hProcess);

            if	(stricmp(_lpstrProcessName,pe32.szExeFile)==NULL)
				Return = TRUE;
			else
            if	(strstr(_lpstrProcessName,pe32.szExeFile)==NULL)
				Return = TRUE;

			CloseHandle(hProcess);
        }
        while(Process32Next(hProcessSnap, &pe32));
    }

    CloseHandle(hProcessSnap);

    return Return;
}

BOOL
SafeTerminateProcess(HANDLE _hProcess,UINT _uExitCode)
{
	DWORD		dwTID,dwCode,dwErr= 0;
	HANDLE		hProcessDup = INVALID_HANDLE_VALUE;
	HANDLE		hRT = NULL;
	HINSTANCE	hKernel = GetModuleHandle("Kernel32");

	BOOL	bIsSuccess	=	FALSE;

	BOOL	bDup	=	DuplicateHandle(GetCurrentProcess(),_hProcess,GetCurrentProcess(),&hProcessDup,PROCESS_ALL_ACCESS,FALSE,0);

	if	(GetExitCodeProcess((bDup) ? hProcessDup : _hProcess ,& dwCode) && (dwCode == STILL_ACTIVE))
	{
		FARPROC	pfnExitProc	=	GetProcAddress(hKernel,"ExitProcess");

		hRT	=	CreateRemoteThread((bDup) ? hProcessDup : _hProcess , NULL , 0 ,(LPTHREAD_START_ROUTINE)pfnExitProc,(PVOID)_uExitCode,0,&dwTID);

		if	(hRT == NULL)
			dwErr = GetLastError();
	}
	else
	{
		dwErr	=	ERROR_PROCESS_ABORTED;
	}

	if	(hRT)
	{
		WaitForSingleObject((bDup) ? hProcessDup : _hProcess , INFINITE);

		CloseHandle(hRT);

		bIsSuccess	=	TRUE;
	}

	if	(bDup)
		CloseHandle(hProcessDup);

	if	(!bIsSuccess)
		SetLastError(dwErr);

	return	bIsSuccess;
}

DWORD
KillProcess(char *_lpstrProcessName)
{
    HANDLE         hProcessSnap = NULL;
    DWORD          Return       = FALSE;
    PROCESSENTRY32 pe32         = {0};

    hProcessSnap	=	CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if	(hProcessSnap == INVALID_HANDLE_VALUE)
		return (DWORD)INVALID_HANDLE_VALUE;

    pe32.dwSize	=	sizeof(PROCESSENTRY32);

    if	(Process32First(hProcessSnap, &pe32))
    {
        DWORD Code = 0;
        DWORD dwPriorityClass;

        do
		{
            HANDLE hProcess;

            hProcess		=	OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
            dwPriorityClass	=	GetPriorityClass(hProcess);

            if	(stricmp(pe32.szExeFile,_lpstrProcessName)==NULL)
            {
//               if	(TerminateProcess(hProcess, 0))
                if	(SafeTerminateProcess(hProcess, 0))
					GetExitCodeProcess(hProcess, &Code);
				else
					return GetLastError();
            }

			CloseHandle(hProcess);
        }

        while(Process32Next(hProcessSnap, &pe32));
			Return = TRUE;
    }
    else
		Return = FALSE;

    CloseHandle(hProcessSnap);

    return Return;
}