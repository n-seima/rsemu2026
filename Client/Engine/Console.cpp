/* 

Author: 
     Gregor S.
 
E-Mail: 
     foxhunter@hlla.net
 
Date: 
     08/20/2002 
 
Description: 
     This class is a simple wrapper for some console functions.
	 The main goal is to keep it that simple so that even
	 beginners have no problems implementing it in their apps.
	 You can write debug outout or anything else you wish into
	 the console in different colors.

Usage:
	 Copy Console.cpp (this file) and Console.h into the directory
	 of your application. Insert them into the workspace if you wish.
	 Next, add the line '#include "Console.h"' into one of your source
	 files and add a member variable of tyle cConsole.
 
Copyright & disclaimer: 
     Do want you want to do with this class: Modify it, extend it to your
	 needs. Use it in both Freeware, Shareware or commercial applications,
	 but make sure to leave these comments at the top of this source file 
	 intact and don't remove my name and E-mail address.

*/


#include <stdio.h>
#include "Console.h"


cConsole	__________forIdler;

// macro for the Clear() function
#define __OPERATE_CONSOLE_ERROR(bSuccess)	{ if(!bSuccess) return; }
// macro to __CHECK_CONSOLE_HANDLE whether m_hConsole is valid
#define __CHECK_CONSOLE_HANDLE(hHandle)		{ if(m_bIsWantOutputDebug == FALSE && hHandle == NULL) return; };
#define __CHECK_IDLER_CONSOLE_HANDLE()		{ if	(!__________forIdler.isValidHandle()) 	return;};


//	debug print
void
OpenColsole(const TCHAR *_lpstrName)	//	콘솔 열기
{
	__________forIdler.create(_lpstrName,FALSE);
}

void
dprint(const TCHAR *_pszOutput, ...)
{
#ifndef	_DEBUG
	return;
#endif
	__CHECK_IDLER_CONSOLE_HANDLE();

	if	(_pszOutput	==	NULL)
	{
		__________forIdler._printRF();
		return;
	}

	TCHAR out[1024];
	va_list va;

	va_start(va, _pszOutput);
	vsprintf(out, _pszOutput, va);
	va_end(va);	

	__________forIdler._print(out);
}

void
dprint(WORD _wColor,const TCHAR *_pszOutput, ...)
{
#ifndef	_DEBUG
	return;
#endif
	__CHECK_IDLER_CONSOLE_HANDLE();

	TCHAR out[1024];
	va_list va;

	va_start(va, _pszOutput);
	vsprintf(out, _pszOutput, va);
	va_end(va);	

	__________forIdler._print(_wColor,out);
}

#ifdef	_UNICODE
void
dprint(const char *_pszOutput, ...)
{
	__CHECK_IDLER_CONSOLE_HANDLE();

	if	(_pszOutput	==	NULL)
	{
		__________forIdler._printRF();
		return;
	}

	char	out[1024];
	va_list va;

	va_start(va, _pszOutput);
	vsprintf_s(out, _pszOutput, va);
	va_end(va);

	__________forIdler._print(out);
}

void
dprint(WORD _wColor,const char *_pszOutput, ...)
{
	__CHECK_IDLER_CONSOLE_HANDLE();

	char	out[1024];
	va_list va;

	va_start(va, _pszOutput);
	vsprintf_s(out, _pszOutput, va);
	va_end(va);

	__________forIdler._print(_wColor,out);
}

#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cConsole::cConsole()
{
	m_hConsole			=	NULL; 	
	m_wCurrentTextColor	=	FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	m_bIsWantOutputDebug=	TRUE;

	m_iRFLength			=	(int)_tcslen(_T("\n"));
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cConsole::~cConsole()
{
	close();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL cConsole::create(const TCHAR *_pszTitle, BOOL _bNoClose)
{
	m_bIsWantOutputDebug	=	FALSE;

	if	(m_hConsole != NULL)
		return FALSE;
	
	// Allocate a new console for our app
	if	(!AllocConsole())
		return FALSE;
	
	// Create the actual console
	m_hConsole = CreateFile(_T("CONOUT$"), GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
	if	(m_hConsole == INVALID_HANDLE_VALUE)
		return FALSE;
	
	if	(SetConsoleMode(m_hConsole, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT) == 0)
		return FALSE;
	
	// if TRUE, disable the [x] button of the console
	if	(_bNoClose)
		disableClose();
				
	// set the console title
	if	(_pszTitle != NULL)
		SetConsoleTitle(_pszTitle);

	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cConsole::setColor(WORD _wColor,BOOL _bIsInstance)
{
	__CHECK_CONSOLE_HANDLE(m_hConsole);

	if	(_wColor != NULL)
	{	// no color specified, reset to defaults (white font on black background)
		if	(_bIsInstance	==	FALSE)
			m_wCurrentTextColor	=	_wColor;
		SetConsoleTextAttribute(m_hConsole, _wColor);
	}
	else
	{		// change font and/or background color
		if	(_bIsInstance	==	FALSE)
			m_wCurrentTextColor	=	FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

		SetConsoleTextAttribute(m_hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // white text on black bg
	}
}

void
cConsole::setCurrentColor()		// set color for output	
{
	SetConsoleTextAttribute(m_hConsole, m_wCurrentTextColor); // white text on black bg
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cConsole::print(const TCHAR *_pszOutput, ...)
{
	__CHECK_CONSOLE_HANDLE(m_hConsole);

	if	(_pszOutput	==	NULL)
	{
		_printRF();
		return;
	}

	TCHAR out[1024];
	va_list va;

	va_start(va, _pszOutput);
	vsprintf(out, _pszOutput, va);
	va_end(va);	

	_print(out);
}

void
cConsole::_print(const TCHAR *_pszOutput)
{
 	// write to the console
	if	(m_bIsWantOutputDebug)
	{
		if	(_pszOutput)
			OutputDebugString(_pszOutput);

		OutputDebugString(_T("\n"));
	}
	else
	{
		DWORD	dwWritten;

		if	(_pszOutput)
			WriteConsole(m_hConsole, _pszOutput, (int)_tcslen(_pszOutput), &dwWritten, 0);

		WriteConsole(m_hConsole, _T("\n"), m_iRFLength, &dwWritten, NULL);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void 
cConsole::print(WORD _wColor,const TCHAR *_pszOutput, ...)
{
	__CHECK_CONSOLE_HANDLE(m_hConsole);

	if	(_pszOutput	==	NULL)
	{
		_printRF();
		return;
	}

	TCHAR out[1024];
	va_list va;

	va_start(va, _pszOutput);
	vsprintf(out, _pszOutput, va);
	va_end(va);	

	_print(_wColor,out);
}

void 
cConsole::_print(WORD _wColor,const TCHAR *_pszOutput)
{
	if	(_pszOutput && m_bIsWantOutputDebug	==	FALSE)
		setColor(_wColor,TRUE);

	_print(_pszOutput);

	if	(_pszOutput && m_bIsWantOutputDebug	==	FALSE)
		setCurrentColor();
}

void
cConsole::_printRF()
{
	if	(m_bIsWantOutputDebug)
		OutputDebugString(_T("\n"));
	else
	{
		DWORD	dwWritten;

		WriteConsole(m_hConsole, _T("\n"), m_iRFLength, &dwWritten, NULL);
	}
}

#ifdef	_UNICODE

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
cConsole::print(const char *_pszOutput, ...)
{
	__CHECK_CONSOLE_HANDLE(m_hConsole);

	if	(!_pszOutput)
	{
		_printRF();
		return;
	}

	char	out[1024];
	va_list va;

	va_start(va, _pszOutput);
	vsprintf_s(out, _pszOutput, va);
	va_end(va);

	_print(out);
}

void
cConsole::_print(const char *_pszOutput)
{
	if	(_pszOutput	==	NULL)
	{
		_printRF();

		return;
	}

	TCHAR	outBuffer[1024];

	MultiByteToWideChar(CP_ACP, 0, _pszOutput, -1, outBuffer, 1024);

	if	(m_bIsWantOutputDebug)
	{
		OutputDebugString(outBuffer);
		OutputDebugString(_T("\n"));
	}
	else
	{
		DWORD	dwWritten;
		WriteConsole(m_hConsole, outBuffer, (int)_tcslen(outBuffer), &dwWritten, 0);
		WriteConsole(m_hConsole, _T("\n"), m_iRFLength, &dwWritten, NULL);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
cConsole::print(WORD _wColor,const char *_pszOutput, ...)
{
	__CHECK_CONSOLE_HANDLE(m_hConsole);

	if	(m_bIsWantOutputDebug	==	FALSE)
		setColor(_wColor,TRUE);

	char	out[1024];
	va_list va;

	va_start(va, _pszOutput);
	vsprintf_s(out, _pszOutput, va);
	va_end(va);	

	_print(out);

	if	(m_bIsWantOutputDebug	==	FALSE)
		setCurrentColor();
}

void
cConsole::_print(WORD _wColor,const char *_pszOutput)
{
	__CHECK_CONSOLE_HANDLE(m_hConsole);

	if	(m_bIsWantOutputDebug	==	FALSE)
		setColor(_wColor,TRUE);

	_print(_pszOutput);

	if	(m_bIsWantOutputDebug	==	FALSE)
		setCurrentColor();
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cConsole::setTitle(const TCHAR *_pszTitle)
{
	if	(m_bIsWantOutputDebug)
		return;
	// self-explanatory
	SetConsoleTitle(_pszTitle);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TCHAR *cConsole::getTitle()
{
	// get the title of our console and return it
	static TCHAR szWindowTitle[256] = _T("");
	GetConsoleTitle(szWindowTitle, sizeof(szWindowTitle));

	return szWindowTitle;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HWND cConsole::getHWND()
{
	if	(m_hConsole == NULL) 
		return NULL;

	// try to find our console window and return its HWND
	return FindWindow(_T("ConsoleWindowClass"), getTitle());
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cConsole::showConsole(BOOL bShow)
{
	__CHECK_CONSOLE_HANDLE(m_hConsole);

	// get out console's HWND and show/hide the console
	HWND hWnd = getHWND();
	if(hWnd != NULL)
		ShowWindow(hWnd, SW_HIDE ? SW_SHOW : bShow);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cConsole::disableClose()
{
	__CHECK_CONSOLE_HANDLE(m_hConsole);

	HWND hWnd = getHWND();
	
	// disable the [x] button if we found our console
	if(hWnd != NULL)
	{
		HMENU hMenu = GetSystemMenu(hWnd,0);
		if(hMenu != NULL)
		{
			DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
			DrawMenuBar(hWnd);
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cConsole::clear()
{
	__CHECK_CONSOLE_HANDLE(m_hConsole);

	/////////////////////////////////////////////////////////////
	// This code is from one of Microsoft's
	// knowledge base articles, you can find it at 
    // http://support.microsoft.com/default.aspx?scid=KB;EN-US;q99261&
	/////////////////////////////////////////////////////////////

	COORD coordScreen = { 0, 0 };

	BOOL bSuccess;
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi; // to get buffer info
    DWORD dwConSize; 

	// get the number of character cells in the current buffer
    bSuccess = GetConsoleScreenBufferInfo(m_hConsole, &csbi);
    __OPERATE_CONSOLE_ERROR(bSuccess);
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    // fill the entire screen with blanks
    bSuccess = FillConsoleOutputCharacter(m_hConsole, (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten);
    __OPERATE_CONSOLE_ERROR(bSuccess);

    // get the current text attribute
    bSuccess = GetConsoleScreenBufferInfo(m_hConsole, &csbi);
    __OPERATE_CONSOLE_ERROR(bSuccess);

    // now set the buffer's attributes accordingly
    bSuccess = FillConsoleOutputAttribute(m_hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
    __OPERATE_CONSOLE_ERROR(bSuccess);

    // put the cursor at (0, 0)
    bSuccess = SetConsoleCursorPosition(m_hConsole, coordScreen);
    __OPERATE_CONSOLE_ERROR(bSuccess);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HANDLE cConsole::getHandle()
{
	// simply return the handle to our console
	return m_hConsole;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cConsole::close()
{
	// free the console, now it can't be used anymore until we Create() it again
	if	(m_hConsole)
	{
		FreeConsole();
		m_hConsole = NULL;
	}
}