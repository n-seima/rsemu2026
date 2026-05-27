// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__28E78A69_3973_47EE_AE4C_6858403C0082__INCLUDED_)
#define AFX_STDAFX_H__28E78A69_3973_47EE_AE4C_6858403C0082__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
//#include <afxsock.h>		// MFC socket extensions

//#include <afxinet.h>	    // MFC Internet support
#include "tracer.h"
#pragma warning(disable:4786) // some STL code produce this warning

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

extern	LPCTSTR		g_lpszRegistry;
extern	LPCTSTR		g_lpszRegistryOld;
extern	HKEY		g_hRegKey;

extern DebugTracer  * cLOG;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__28E78A69_3973_47EE_AE4C_6858403C0082__INCLUDED_)
