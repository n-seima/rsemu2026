// AdminTool.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

#ifdef	_DEBUG
#define _CRTDBG_MAP_ALLOC
#endif

#include	<stdlib.h>
#include	<crtdbg.h>

#include "AdminTool.h"
#include "AdminToolDlg.h"
#include "LOGIN.h"
#include "AdminLogCode.h"
#include "CMessage.h"
#include "cSRVUTIL.h"

#include "AdminCommand.h"
#include "AGEGRAPH.h"
#include "assert.h"
#include "skill.h"

#include <atlbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_FOR_USA
char*	l_lpstrRegistryForKorea		=	"software\\L&K Logic Korea\\Red Stone for USA";
char*	l_lpstrRegistryForTest		=	"software\\L&K Logic Korea\\Red Stone(test) for USA";
char*	l_lpstrRegistryForDesignTeam=	"software\\L&K Logic Korea\\Red Stone(balance) for USA";
#elif	_FOR_INTER
char*	l_lpstrRegistryForKorea		=	"software\\L&K Logic Korea\\Red Stone for Inter";
char*	l_lpstrRegistryForTest		=	"software\\L&K Logic Korea\\Red Stone(test) for Inter";
char*	l_lpstrRegistryForDesignTeam=	"software\\L&K Logic Korea\\Red Stone(balance) for Inter";
#elif	_FOR_THAI
char*	l_lpstrRegistryForKorea		=	"software\\L&K Logic Korea\\Red Stone for Thai";
char*	l_lpstrRegistryForTest		=	"software\\L&K Logic Korea\\Red Stone(test) for Thai";
char*	l_lpstrRegistryForDesignTeam=	"software\\L&K Logic Korea\\Red Stone(balance) for Thai";
#elif	_FOR_JAPAN
char*	l_lpstrRegistryForKorea		=	"software\\L&K Logic Korea\\Red Stone for Japan";
char*	l_lpstrRegistryForTest		=	"software\\L&K Logic Korea\\Red Stone(test) for Japan";
char*	l_lpstrRegistryForDesignTeam=	"software\\L&K Logic Korea\\Red Stone(balance) for Japan";

char*	l_lpstrRegistryForOld		=	"software\\L&K Logic Korea\\Red Stone";
char*	l_lpstrRegistryForTestOld	=	"software\\L&K Logic Korea\\Red Stone(test)";
char*	l_lpstrRegistryForDesignTeamOld=	"software\\L&K Logic Korea\\Red Stone(balance)";

#else

char*	l_lpstrRegistryForKorea		=	"software\\L&K Logic Korea\\Red Stone";
char*	l_lpstrRegistryForTest		=	"software\\L&K Logic Korea\\Red Stone(test)";
char*	l_lpstrRegistryForDesignTeam=	"software\\L&K Logic Korea\\Red Stone(balance)";
#endif

char*	g_lpstrRegistry				=	l_lpstrRegistryForKorea;

int		g_iToolType	=	eTOOL_FOR_NORMAL;

BOOL	g_bIsLimitedEdition	=	FALSE;

//	extern	
char			g_strGameFolder[512];
char			strCurPath[1024];

char			g_strUserDBAddress[64];
char			g_strUserDBName[64];

char			g_strToolLogIP[16];
int				g_iToolLogPort;

char			g_strInnerUserDBAddress[64];
char			g_strInnerUserDBName[64];

char			g_strLogDataFtpIp[16];
int				g_iLogDataFtpPort;

long			g_lWhereisServer = eCOUNTRY_KOREA;
CAdminToolDlg	*l_pAdminToolDlg;
cScript			scp;
cADO_ADMIN		g_DBAccount,g_DBSrvInfo,g_DBAdmin;

//Nafxcwd.lib Libcmtd.lib zdll.lib ws2_32.lib winmm.lib 
/////////////////////////////////////////////////////////////////////////////
// CAdminToolApp

DebugTracer * cLOG	=	NULL;

BEGIN_MESSAGE_MAP(CAdminToolApp, CWinApp)
	//{{AFX_MSG_MAP(CAdminToolApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/*
const char szProgName[]="ADMINTOOL";
const char szProgTitle[]="RedStone Administrator Tool";
HANDLE		szMutexHandle = NULL;
*/
/////////////////////////////////////////////////////////////////////////////
// CAdminToolApp construction

CAdminToolApp::CAdminToolApp()
{
	const	int	c_iCheckLeakBlock	=	-1;

	if	(c_iCheckLeakBlock	!=	-1)
	{
		assert(0 && "메모리 릭 체크 변수가 세팅되어 있습니다!!");

		_CrtSetBreakAlloc( c_iCheckLeakBlock );
	}

  	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); 

	float avail	=	(float)(((float ) GetAvailMemory()) /1024.0 / 1024.0);

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	int size = sizeof(cServerList)*dMAX_WORLD_SERVER_COUNT;
	size = sizeof(CAGEGRAPH);
	size = sizeof(cSOCKET);
	size = sizeof(CAdminToolApp);
	size = sizeof(cADO_ADMIN);
	size = sizeof(cScript);

	size = sizeof(CPAGE_NOTICE			);
	size = sizeof(CPAGE_USERSETTING		);
	size = sizeof(CWorldScore				);
	size = sizeof(CCharacterInfo			);
	size = sizeof(CServerTool				);
	size = sizeof(cItemPackList			);
	size = sizeof(cCarrotShop				);
	size = sizeof(CSEARCHPAGE				);
	size = sizeof(CTentativeGuildManager	);
	size = sizeof(CEventManager			);
	size = sizeof(cGuildInfo				);
	size = sizeof(CGuildSkillManager);
	size = sizeof(CAdminCommand);
}


CAdminToolApp::~CAdminToolApp()
{
	if	(cLOG)
	{
		delete cLOG;

		cLOG	=	NULL;
	}

/*
	if(szMutexHandle)
		ReleaseMutex(szMutexHandle); 
*/
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAdminToolApp object

CAdminToolApp theApp;

#include "smtp.h"

/////////////////////////////////////////////////////////////////////////////
// CAdminToolApp initialization

BOOL CAdminToolApp::InitInstance()
{
	if	(IsFile("do not here!!"))
	{
		cMSG::Error("Error!!","do not here!!");
		
	
		return	FALSE;
	}

/*
	cSmtp mail;

	if(mail.ConnectServer( "211.189.66.49"))
	{
//		mail.SetSenderName("[KR] [Red Stone] Game-Result");
		mail.SetSenderEmail("[KR] [Red Stone] Game-Result","WCG@logickorea.co.kr");
		mail.SetSubject("[KR] [Red Stone] Game-Result");
//		mail.AddRecipient("rs_result@worldcybergame.com");
//		mail.AddRecipient("whitewitch@logickorea.co.kr");
		mail.AddRecipient("nemo@logickorea.co.kr");
		mail.AddCCRecipient("gawen@nate.com");

		mail.SetMessageBody("<game program=\"RedStone\" start=\"2008-04-29 18:27:20\" length=\"400\"><team name=\"TEAM_A\" result=\"Win\"/><team name=\"TEAM_B\" result=\"Loss\"/></game>");
		if(mail.GetConnectStatus())
		{
			int ret = mail.Send();
			mail.Disconnect();
		}
	}
*/
/*
	szMutexHandle = CreateMutex(NULL, TRUE, szProgName);
    if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		HWND hCurWnd = ::FindWindow(NULL,szProgTitle);
		if(hCurWnd)		::ShowWindow(hCurWnd,SW_RESTORE);
		return FALSE;
	}
*/
	AfxEnableControlContainer();

	char	strCommandLine[1024];

	strcpy(strCommandLine,GetCommandLine());

#ifdef	_FOR_JAPAN

	{
		CRegKey		reg;

		strcpy(g_strGameFolder,"");

		int iResult	=	reg.Open( HKEY_CURRENT_USER, g_lpstrRegistry);

		if	(iResult	!=	ERROR_SUCCESS)
			iResult	=	reg.Open( HKEY_LOCAL_MACHINE, g_lpstrRegistry);

		if	(iResult	!=	ERROR_SUCCESS)
		{
			l_lpstrRegistryForKorea		=	l_lpstrRegistryForOld;
			l_lpstrRegistryForTest		=	l_lpstrRegistryForTestOld;
			l_lpstrRegistryForDesignTeam=	l_lpstrRegistryForDesignTeamOld;
		}
	}

	if	(strstr(strCommandLine,"forJapanHangame"))
	{
		g_lpstrRegistry	=	"software\\L&K Logic Korea\\Red Stone for japan hangame";
	}
	else
#endif
	if	(strstr(strCommandLine,"test_AdminTool") || strstr(strCommandLine,"testAdminTool"))
	{
		g_iToolType		=	eTOOL_FOR_TEST;
		g_lpstrRegistry	=	l_lpstrRegistryForTest;
		cMSG::Put("","패치 테스트용 버젼");
	}
	else
	if	(strstr(strCommandLine,"Red Stone(balance)") || strstr(strCommandLine,"기획팀AdminTool"))
	{
		g_iToolType		=	eTOOL_FOR_BALANCE;
		g_lpstrRegistry	=	l_lpstrRegistryForDesignTeam;
		cMSG::Put("","기획팀 서버 테스트용 버젼");
	}
	else
	if	(strstr(strCommandLine,"Red Stone for International"))
	{
		g_iToolType		=	eTOOL_FOR_INTERNATIONAL;
		cMSG::Put("","인터네셔널 서버용 버젼");
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	GetModuleFileName(NULL,strCurPath,sizeof(strCurPath));
	char *pSlash = strrchr(strCurPath,'\\');
	if(pSlash)
		*pSlash = 0;

	initMessage();

	char fn[2048];
	
	sprintf(fn,"%s\\ReadMe_dev.txt",strCurPath);
	if	(!IsFile(fn))
		sprintf(fn,"%s\\ReadMe.txt",strCurPath);

	scp.Read(fn);

	strcpy(g_strUserDBAddress,scp.getData("ACCOUNTDB_INFO"));
	strcpy(g_strUserDBName,scp.getData("ACCOUNTDB_INFO",2));

	strcpy(g_strToolLogIP,scp.getData("TOOLOGINFO"));
	g_iToolLogPort = atoi(scp.getData("TOOLOGINFO",2));

	strcpy(g_strInnerUserDBAddress,scp.getData("INNER_ACCOUNTDB_INFO"));
	strcpy(g_strInnerUserDBName,scp.getData("INNER_ACCOUNTDB_INFO",2));

	g_iWorldFtpPort = atoi(scp.getData("FTPPORT",1));
//	g_iLogFtpPort = atoi(scp.getData("FTPPORT",2));

	strcpy(g_strLogDataFtpIp,scp.getData("LOGDATASERVER"));
	g_iLogDataFtpPort = atoi(scp.getData("LOGDATASERVER",2));
#if defined(_FOR_USA) || defined(_FOR_THAI)
	strcpy(strLogFtpId,scp.getData("LOGDATASERVER",3));
	strcpy(strLogFtpPw,scp.getData("LOGDATASERVER",4));
#endif

	cLOG = new DebugTracer(DTType_File,"RedStone_AdminTool");
	if(!cLOG)	return FALSE;

#ifdef _FOR_CHINA
	strcpy(strAvatarDBCId,"redstone");
	strcpy(strAvatarDBCPw,"redstone");
	strcpy(strAccountDBCId,"redstone");
	strcpy(strAccountDBCPw,"redstone");
	strcpy(strLogFtpId,"redstone");
	strcpy(strLogFtpPw,"redstone");
	strcpy(strLoginDBId,"redstone");
	strcpy(strLoginDBPw,"redstone");
	g_iDBPort = 50505;
#elif _FOR_JAPAN
	strcpy(strAvatarDBCId,"RedMaster");
	strcpy(strAvatarDBCPw,"CJFVKSRNDL");
	strcpy(strAccountDBCId,"RedMaster");
	strcpy(strAccountDBCPw,"CJFVKSRNDL");
	strcpy(strLogFtpId,"redstone");
	strcpy(strLogFtpPw,"qnfrdmsqhtjr");
	strcpy(strLoginDBId,"RedMaster");
	strcpy(strLoginDBPw,"CJFVKSRNDL");
	g_iDBPort = 50605;
#elif _FOR_USA
	strcpy(strAvatarDBCId,"redstone");
	strcpy(strAvatarDBCPw,"usatest");
	strcpy(strAccountDBCId,"redstone");
	strcpy(strAccountDBCPw,"usatest");
//	strcpy(strLogFtpId,"redstone");
//	strcpy(strLogFtpPw,"usatest");
	strcpy(strLoginDBId,"redstone");
	strcpy(strLoginDBPw,"usatest");
	g_iDBPort = 50605;
#elif _FOR_THAI
	strcpy(strAvatarDBCId,"RedMaster");
	strcpy(strAvatarDBCPw,"REDSTONEON");
	strcpy(strAccountDBCId,"RedMaster");
	strcpy(strAccountDBCPw,"REDSTONEON");
//	strcpy(strLogFtpId,"redstone");
//	strcpy(strLogFtpPw,"usatest");
	strcpy(strLoginDBId,"RedMaster");
	strcpy(strLoginDBPw,"REDSTONEON");
	g_iDBPort = 50605;
#elif _FOR_INTER
	
	strcpy(strAvatarDBCId,"RedMaster");
	strcpy(strAvatarDBCPw,"cjfvksrndl");
	strcpy(strAccountDBCId,"RedMaster");
	strcpy(strAccountDBCPw,"cjfvksrndl");
	strcpy(strLogFtpId,"redstone");
	strcpy(strLogFtpPw,"cjfvksrndl");
	strcpy(strLoginDBId,"redstone");
	strcpy(strLoginDBPw,"cjfvksrndl");

/*	strcpy(strAvatarDBCId,"redstone");
	strcpy(strAvatarDBCPw,"cjfvksrndl");
	strcpy(strAccountDBCId,"redstone");
	strcpy(strAccountDBCPw,"cjfvksrndl");
	strcpy(strLogFtpId,"redstone");
	strcpy(strLogFtpPw,"cjfvksrndl");
	strcpy(strLoginDBId,"redstone");
	strcpy(strLoginDBPw,"cjfvksrndl");
*/
	g_iDBPort = 40605;
#elif _FOR_KOREA
	strcpy(strAvatarDBCId,"RedMaster");
	strcpy(strAvatarDBCPw,"CJFVKSRNDL");
	strcpy(strAccountDBCId,"RedMaster");
	strcpy(strAccountDBCPw,"CJFVKSRNDL");
	strcpy(strLogFtpId,"ADMINTOOL");
	strcpy(strLogFtpPw,"CJFVKSRNDL");
	strcpy(strLoginDBId,"AdminToolUser");
	strcpy(strLoginDBPw,"CJFVKSRNDL");
	g_iDBPort = 5316;
#endif

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	{
		CRegKey		reg;

		strcpy(g_strGameFolder,"");

		g_hRegKey	=	HKEY_CURRENT_USER;

		int iResult	=	reg.Open( g_hRegKey, g_lpstrRegistry);

		if	(iResult	!=	ERROR_SUCCESS)
		{
			g_hRegKey	=	HKEY_LOCAL_MACHINE;
			iResult		=	reg.Open( g_hRegKey, g_lpstrRegistry);
		}

		if (iResult == ERROR_SUCCESS)
		{
			DWORD	count	=	sizeof(g_strGameFolder);

			if (reg.QueryValue((LPTSTR )g_strGameFolder,"Path",&count) == ERROR_SUCCESS)
			{
				if (count	<=	0)
					return cMSG::Error("error","can not find red stone registry data");;
			}
			else
				return cMSG::Error("error","can not find red stone registry data");;
		}
		else
			return cMSG::Error("error","can not find red stone registry data");;
	}

	LoadSkillData();

	int	nResponse;

	CLOGIN		dlgLogin;

	if	(g_bIsLimitedEdition	==	FALSE)
		nResponse	=	dlgLogin.DoModal();
	else
	{
		nResponse	=	IDOK;
		extLevel	=	eAL_SYSTEM_OPERATER;
	}

	if	(nResponse	==	IDCANCEL)
	{
		cMSG::Error("login error","incorrect ID or Password");;

		return	FALSE;
	}

	l_pAdminToolDlg	=	new CAdminToolDlg();

	if	(nResponse	==	IDOK	&&	extLevel	>=	eAL_ONLY_SEARCH)
	{
		m_pMainWnd	= l_pAdminToolDlg;

		nResponse = l_pAdminToolDlg->DoModal();

		if	(nResponse == IDCANCEL)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with Cancel
		}
	}
	else 
	if	(nResponse == IDOK && extLevel<eAL_NORMAL)
	{
		MessageBox(NULL,g_strToolMessage[0],"ERROR",MB_OK);
	}

	delete l_pAdminToolDlg;

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
