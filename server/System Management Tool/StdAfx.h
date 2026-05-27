// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__7779C05E_DB81_472B_AFC8_7BF574908682__INCLUDED_)
#define AFX_STDAFX_H__7779C05E_DB81_472B_AFC8_7BF574908682__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//	어떤 종류의 스케쥴 형태가 있는가..
typedef enum
{
	eST_Backup,
	eST_File,
	eST_Process,
	eST_Monitoring,

	eST_Count,
}eScheduleType;


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__7779C05E_DB81_472B_AFC8_7BF574908682__INCLUDED_)
