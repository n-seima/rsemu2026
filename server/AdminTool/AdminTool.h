// AdminTool.h : main header file for the ADMINTOOL application
//

#if !defined(AFX_ADMINTOOL_H__EAF2F3ED_0631_4CB1_8CF1_65AB5F129FE1__INCLUDED_)
#define AFX_ADMINTOOL_H__EAF2F3ED_0631_4CB1_8CF1_65AB5F129FE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAdminToolApp:
// See AdminTool.cpp for the implementation of this class
//

//
//	Administrator Level
enum
{
	eAL_ONLY_SEARCH		=	0x00,	//	캐릭터 검색과 파일 저장만 할 수 있음.
	eAL_NORMAL			=	0x01,	//	관리자 레벨 1
	eAL_TESTER			=	0x02,	//	관리자 레벨 2
	eAL_OPERATOR		=	0x04,	//	관리자 레벨 3
	eAL_BOSS			=	0x08,	//	reserve - 현재 사용하고 있지 않다. (2006.12.18)
	eAL_MASTER_OPERATER	=	0x10,	//	운영팀장
	eAL_SYSTEM_OPERATER	=	0x20,	//	툴 제작자
};

extern	char*	g_lpstrRegistry	;
extern	int		g_iToolType;
enum
{
	eTOOL_FOR_NORMAL,
	eTOOL_FOR_TEST,
	eTOOL_FOR_BALANCE,
	eTOOL_FOR_INTERNATIONAL,
};

class CAdminToolApp : public CWinApp
{
public:
	CAdminToolApp();
	~CAdminToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdminToolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAdminToolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

extern	BOOL	g_bIsLimitedEdition;

#endif // !defined(AFX_ADMINTOOL_H__EAF2F3ED_0631_4CB1_8CF1_65AB5F129FE1__INCLUDED_)
