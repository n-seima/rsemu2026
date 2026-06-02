// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__03322FE3_493D_444F_BAA3_231866857D8C__INCLUDED_)
#define AFX_STDAFX_H__03322FE3_493D_444F_BAA3_231866857D8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma once

#include "targetver.h"

#define _WIN32_DCOM
#define WIN32_LEAN_AND_MEAN		// ” Windows ∑÷≥˝º´…π”√¡œ

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <algorithm>
#include <iterator>

//#include <vld.h>

#include "cSOCKET.h"
#include "RSComm.h"
#include "cScript.h"
#include "cADO_ADMIN.h"

#pragma warning(disable:4786) // some STL code produce this warning
#pragma warning(disable:4284) // some STL code produce this warning


#define dINNERDBADDR	"211.235.238.104"

typedef enum
{
	eCOUNTRY_KOREA,
	eCOUNTRY_INNERTEST,
	eCOUNTRY_CHINA,
	eCOUNTRY_JAPAN,
	eCOUNTRY_USA,
	eCOUNTRY_INTER,
	eCOUNTRY_THAI,
	eCOUNTRY_COUNT,
}eCOUNTRY;

extern	cScript			scp;

extern	long			g_lWhereisServer;

extern	char			g_strUserDBAddress[64];
extern	char			g_strUserDBName[64];
extern	char			g_strToolLogIP[16];
extern	int				g_iToolLogPort;
extern	char			g_strInnerUserDBAddress[64];
extern	char			g_strInnerUserDBName[64];
extern	cADO_ADMIN		g_DBAccount,g_DBSrvInfo,g_DBAdmin;

class	cGameServerInfo
{
public:
	char			strGameIP[dIP_SIZE];
	DWORD			dwUserCount,dwMaxUserCount;
	UTime			time;
	DWORD			hThreadHandle;

	cGameServerInfo(){	reset();	}
	inline void reset()
	{
		strcpy(strGameIP,"");
		dwUserCount = 0,dwMaxUserCount = 0;
		time.m_dwValue = 0;
	}
};

#define dMAX_GS_COUNT		8

//	20060503π¡Ø ¿¿¸¿∫ 24¿.
//#define	dMAX_TIME_COUNT		24	//	1440/5
#define	dMAX_TIME_COUNT		288	//	1440/5

class	cServerList
{
public:
	char			strName[dNAME_LENGTH];
	char			strIP[dIP_SIZE];
	char			strDBIP[dIP_SIZE];
	char			strDBCIP[dIP_SIZE];

	UTime			time;
	BOOL			isConnect;
	BOOL			isTestServer;
	HTREEITEM		hTreeItem;
	TV_INSERTSTRUCT	tvStruct;
	cSOCKET			worldsock;
	cSOCKET			dbcsock;
	HANDLE			hThread;
	WORD			wGameServerCount;
	cGameServerInfo	cGS[dMAX_GS_COUNT];
	
	int				wAllUserCount;
	int				wMaxAllUserCount;
	WORD			wUserCount[dMAX_TIME_COUNT];
	int				wAvr,wTop;
#ifdef _FOR_KOREA
	int				wAllYahooUserCount;
	int				wMaxAllYahooUserCount;
	WORD			wYahooUserCount[dMAX_TIME_COUNT];
	int				wYahooAvr,wYahooTop;

	int				wAllParanUserCount;
	int				wMaxAllParanUserCount;
	WORD			wParanUserCount[dMAX_TIME_COUNT];
	int				wParanAvr,wParanTop;
#endif
	DWORD			startdate;

	cServerList(){	reset();	}
	inline void reset()
	{
		isTestServer = FALSE;
		wGameServerCount = wAllUserCount = wMaxAllUserCount = 0;
		startdate = 0;
		strcpy(strName,"");	strcpy(strIP,"");	strcpy(strDBIP,"");	strcpy(strDBCIP,"");	isConnect = FALSE;
		for(int i=0;i<dWORLD_MAX_USER_COUNT;i++)	cGS[i].reset();
//		if(wUserCount){	delete [] wUserCount;	wUserCount = NULL;	}
//		if(wYahooUserCount){	delete [] wYahooUserCount;	wYahooUserCount = NULL;}
		wAvr = wTop = 0;
	}
	inline void	CountReset()
	{
		for(int i=0;i<24;i++)
		{	
			wUserCount[i]=0;	
#ifdef _FOR_KOREA
			wYahooUserCount[i]=0;	
			wParanUserCount[i]=0;	
#endif
		}
		wAvr = wTop = 0;
#ifdef _FOR_KOREA
		wYahooAvr = wYahooTop = 0;
		wParanAvr = wParanTop = 0;
#endif
	}
};

extern BOOL				g_isViewSum;
extern WORD				g_wMaxUserCount,g_wMaxTime;
extern WORD				g_wMaxYahooUserCount,g_wMaxYahooTime;
extern WORD				g_wMaxParanUserCount,g_wMaxParanTime;
extern int				g_iSelectedServer;
extern char				strCurPath[1024];
extern cServerList		sList[dMAX_WORLD_SERVER_COUNT];
extern int				g_iWorldCount;
extern int				extLevel;
extern char				strUserName[32];
extern char				strAvatarDBCId[32];
extern char				strAvatarDBCPw[32];
extern char				strAccountDBCId[32];
extern char				strAccountDBCPw[32];
extern char				strLogFtpId[32];
extern char				strLogFtpPw[32];
extern char				strLoginDBId[32];
extern char				strLoginDBPw[32];
extern int				g_iDBPort;
extern int				g_iWorldFtpPort,g_iLogFtpPort;
extern	HKEY			g_hRegKey;

extern char				g_strLogDataFtpIp[16];
extern int				g_iLogDataFtpPort;


extern	BOOL			SendPacketToWorld(int _iServerIndex,char *_lpPacket,BOOL _bSendOne=FALSE,BOOL _bExceptTestServer=FALSE);
extern	BOOL			SendWP_SimpleWork(int _iServerIndex,int _iWork,int _iValue1=0,int _iValue2=0,int _iValue3=0,int _iValue4=0,int _iValue5=0,int _iValue6=0);
extern	BOOL			SendWP_LongSimpleWork(int _iServerIndex,int _iWork,int _iValue1=0,int _iValue2=0,int _iValue3=0,int _iValue4=0,int _iValue5=0,int _iValue6=0,
											int _iValue7=0 ,int _iValue8=0 ,int _iValue9=0 ,int _iValue10=0 ,int _iValue11=0 ,int _iValue12=0 );
extern	BOOL			SendPacketToTestServer(char *_lpPacket);
extern	BOOL			IsConnectedAllServer(BOOL _bIsWantWarningMessage=FALSE,BOOL _bIsIgnoreTestServer=FALSE);
extern	BOOL			IsConnectedTestServer(BOOL _bIsWantWarningMessage=FALSE,int _iIndex=-1);
extern	BOOL			IsConnectedServer(int _iServerIndex,BOOL _bIsWantWarningMessage=TRUE);

extern	int				GetTestServerIndex();
extern	BOOL			CheckOperatorLevel(int _iLimitLevel);


#define dSERVERStoWORLD_ASK_ITEM_PACK_LIST_TEST			0x8123
#define dWORLDtoSERVERS_ASK_ITEM_PACK_LIST_TEST			0x8823
class	cMSG_BASE_TYPE_FORTEST
{
public:
	WORD	wSize,				//	∂ ª¿¡
			wType;				//	∂ ∏¿‘
	inline	void	set(WORD size,WORD type)	{wSize	=	size,wType	=	type;}
};

struct	SERVERStoWORLD_ASK_ITEM_PACK_LIST_TEST
{
	cMSG_BASE_TYPE_FORTEST		base;

	WORD	wPackSerial;
};

struct WORLDtoSERVERS_ASK_ITEM_PACK_LIST_TEST
{
	cMSG_BASE_TYPE_FORTEST		base;

	char		data[4196];
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__03322FE3_493D_444F_BAA3_231866857D8C__INCLUDED_)
