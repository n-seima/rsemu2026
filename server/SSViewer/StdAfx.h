// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__BC414351_5E8A_48DD_A86B_8FF35435CD2C__INCLUDED_)
#define AFX_STDAFX_H__BC414351_5E8A_48DD_A86B_8FF35435CD2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.





// UserCode
////////////////////////////////////////////////////////////////////////////////////////////////
// Define
#define dUM_TRAYICON_NOTIFY		WM_USER+10	// 트레이 아이콘에서 사용
#define dMAXMESSAGESIZE			512
#define dSOUNDFILE				"Sound\\Notice.WAV"
#define dDATAFILE				"Data\\Notice.dat"
// Define
////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////
// Enum

// Timer
typedef enum _enumTIMER
{
	eTIMER_DLEINITDLG = 0,			//	처음 다이얼로그 생성시 다이얼로그 보이지 않게 설정
	eTIMER_CHECKSS,					//	주기적으로 DB를 확인한다.
}enumTIMER;
// Enum
////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////
// Struct
typedef struct _stMESSAGEINFO
{
	int iIdx;
	char szMessage[dMAXMESSAGESIZE];
}stMESSAGEINFO;
// Struct
////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////
// Extern
extern UINT g_iShellRestart;					// 트레이 아이콘이 사라지는 버그 때문에
class CSSViewerDlg;
extern CSSViewerDlg *g_pcMainFramePtr;			// 메인 프레임의 포인터
// Extern
////////////////////////////////////////////////////////////////////////////////////////////////


#endif // !defined(AFX_STDAFX_H__BC414351_5E8A_48DD_A86B_8FF35435CD2C__INCLUDED_)
