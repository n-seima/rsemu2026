// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__58AB9BCE_33C3_44D6_90AE_D64F110B0DC8__INCLUDED_)
#define AFX_STDAFX_H__58AB9BCE_33C3_44D6_90AE_D64F110B0DC8__INCLUDED_

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

#include "exceptionhandler.h"	//	예외 처리기

#define	dCONFIGFILENAME		"config-bc.txt"
#define dAVATARDBINFO		"AVATARDBINFO"
#define dAVATARDBFILE		"AvatarDbInfo.bin"
#define dAVATARDBCINFO		"AVATARDBCINFO"
#define dWORLDINFO			"WORLDINFO"
#define dCHATDBINFO			"CHATLOGDB"

//////////////////////////////////////////////////////
//	in RedStone_BoardCastServerDlg.cpp

//	유저 소켓들의 recv/send에 반응한다.
extern void WorkerThread(void * _pParam); 

//	유저들의 connect를 처리한다.
extern void AcceptThread(void * _pParam); 

//	기타의 필요한 내용은 이곳으로..
extern void AdditionThread(void * _pParam); 

//	유저의 종료를 처리한다.
extern void	DisUser(WORD _wIdx, char * _strWhy);


//	유저의 접속 기록을 남겨본다.
extern	int	g_iCheckUserConnectionInfo;
extern void	__cdecl extWriteConnectionInfo(char * _strId, char * _strArg,...);

//	in RedStone_BoardCastServerDlg.cpp
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
//	in ProcessThead.cpp
extern BOOL RecvPacket(DWORD _dwIdx);
extern BOOL SendPacket(DWORD _dwIdx);
extern BOOL SendPacket_Add(DWORD _dwIdx);
//	in ProcessThead.cpp
//////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__58AB9BCE_33C3_44D6_90AE_D64F110B0DC8__INCLUDED_)
