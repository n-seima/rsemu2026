// TracerFromSaveLog.h: interface for the CTracerFromSaveLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRACERFROMSAVELOG_H__DA401CD7_4541_40FB_9B73_04BDB6A41F42__INCLUDED_)
#define AFX_TRACERFROMSAVELOG_H__DA401CD7_4541_40FB_9B73_04BDB6A41F42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _WINDOWS_
#include <windows.h>
#endif
#include <stdio.h>

class CTracerFromSaveLog  
{
public:
	CTracerFromSaveLog();
	virtual ~CTracerFromSaveLog();

	BOOL		initialize( char * _dir,char * _fn);
	void		close();

	void		__cdecl print(WORD	wOutputType , LPCTSTR format, ... );
	void		output( char * _strStr );
	void		outputf( char * _strStr );
	BOOL		SetFileHandle();

public:
	CRITICAL_SECTION	cs;
	FILE			*	fp;
	HANDLE				handleConsole;
	char	strPath[1024] , strName[128] , dir[2048],fn[2048];
	int		iNameCount, iWriteCount,ver,day,mon;
};

#endif // !defined(AFX_TRACERFROMSAVELOG_H__DA401CD7_4541_40FB_9B73_04BDB6A41F42__INCLUDED_)
