//	TRR34-TFXKB-JR6G3-7Y9MH-XF7KT
//	테스트

#include "cMAIN.H"
#include "CGame.H"
#include "CAgent.H"
#include "CGamePlay.H"
#include "ngpBrown.h"
#include "CMessage.h"
#include "kls.H"
#include "CLoadTextCode.h"
#include "CMap.h"
#include "tdlanguage.h"
#include "zmouse.h"
#include "tool.h"
#include "CWindowInterface.h"
#include "CAgent.h"
#include "CHero.h"
#include "version.h"
#ifdef	_USE_HS
#include "HShield.h"
#endif

int		g_iServiceID	=	0;
char	g_strUserID[20]	=	"";
char	g_strTokken[64]	=	"";


#ifdef _USE_XTRAP
//#include "XTrap4Client.h"
#pragma comment(lib,"XTrap/XTrap4Client_ClientPart_mt.lib")
#pragma comment(lib,"XTrap/XTrap4Client_mt.lib")
#pragma comment(lib,"XTrap/XTrap4Client_ServerPart_mt.lib")
#pragma comment(lib,"XTrap/XTrap_Unipher_mt.lib")

#include "XTrap/Xtrap_C_Interface.h"
#endif
#include "console.h"

BOOL		g_bIsUseConsole	=	FALSE;

#ifdef	_FOR_USA
LPCTSTR		lpszRegistry		=	"software\\L&K Logic Korea\\Red Stone for USA";
LPCTSTR		lpszTestRegistry	=	"software\\L&K Logic Korea\\Red Stone(test) for USA";
LPCTSTR		lpszBalanceRegistry	=	"software\\L&K Logic Korea\\Red Stone(balance) for USA";
#elif	_FOR_INTERNATIONAL
LPCTSTR		lpszRegistry		=	"software\\L&K Logic Korea\\Red Stone for International";
LPCTSTR		lpszTestRegistry	=	"software\\L&K Logic Korea\\Red Stone(test) for International";
LPCTSTR		lpszBalanceRegistry	=	"software\\L&K Logic Korea\\Red Stone(balance) for International";
#elif	_FOR_THAI
LPCTSTR		lpszRegistry		=	"software\\L&K Logic Korea\\Red Stone for Thai";
LPCTSTR		lpszTestRegistry	=	"software\\L&K Logic Korea\\Red Stone(test) for Thai";
LPCTSTR		lpszBalanceRegistry	=	"software\\L&K Logic Korea\\Red Stone(balance) for Thai";
#elif	_FOR_JAPAN
LPCTSTR		lpszRegistry		=	"software\\L&K Logic Korea\\Red Stone for Japan";
LPCTSTR		lpszTestRegistry	=	"software\\L&K Logic Korea\\Red Stone(test) for Japan";
LPCTSTR		lpszBalanceRegistry	=	"software\\L&K Logic Korea\\Red Stone(balance) for Japan";
LPCTSTR		lpszRegistryOld		=	"software\\L&K Logic Korea\\Red Stone";
LPCTSTR		lpszTestRegistryOld	=	"software\\L&K Logic Korea\\Red Stone(test)";
LPCTSTR		lpszBalanceRegistryOld=	"software\\L&K Logic Korea\\Red Stone(balance)";
#elif	_FOR_KOREA
LPCTSTR		lpszRegistry		=	"software\\L&K Logic Korea\\Red Stone";
LPCTSTR		lpszTestRegistry	=	"software\\L&K Logic Korea\\Red Stone(test)";
LPCTSTR		lpszBalanceRegistry	=	"software\\L&K Logic Korea\\Red Stone(balance)";
#endif

LPCTSTR		lpszRegistryDev		=	"software\\L&K Logic Korea\\Red Stone for Dev";

DWORD		g_dwLastCheckGameHackTime	=	0;
DWORD		g_dwMainThreadID;

#ifdef	_USE_NPGL
CNPGameLib	*g_npgl				=	NULL;
bool CALLBACK NPGameMonCallback(DWORD dwMsg, DWORD dwArg);
BOOL		g_bIsExistGGError	=	FALSE;
char		g_strGGError[256];
BOOL		g_bIsUseNPGL		=	TRUE;
#else
BOOL		g_bIsUseNPGL		=	FALSE;
#endif

BOOL		l_bWaitToExit		=	FALSE;

class CAutoBrush
{
public:
	//HBRUSH operator
	//{
	//}
	CAutoBrush () 
	{
		hBrush = CreateSolidBrush ( 0 );
	}
	~CAutoBrush ()
	{
		DeleteObject ( hBrush );
	}

	void SetColor ( COLORREF color )
	{
		DeleteObject ( hBrush );
		hBrush = CreateSolidBrush ( color );
	}

	HBRUSH GetBrush ( )
	{
		return hBrush;
	}

private:
	HBRUSH hBrush;
};

CAutoBrush	l_autoBrush;

static char	*lpszAppName				=	"REDSTONE"; 
static char	*lpszTitle					=	"Red Stone - The Lunatic Story";

HBRUSH		l_hBrush;
BOOL		g_bIsDevPC					=	FALSE;
BOOL		g_bIsTerminalPC				=	FALSE;
DWORD		g_dwClientVersion			=	540;	// 클라이언트 버전 110211 수정(11년 2월 11일 본섭 패치 예정)
int			g_iSeasonVariable			=	0;
BOOL		g_bIsPortalUser				=	FALSE;

cWND		*cMAIN::s_pWnd				=	NULL;
HWND		cMAIN::s_hWnd;
HINSTANCE	cMAIN::s_hInst;

HCURSOR		cMAIN::s_hNormalCursor		=	0;
HCURSOR		cMAIN::s_hAttackCursor		=	0;
HCURSOR		cMAIN::s_hWaitCursor		=	0;
HCURSOR		cMAIN::s_hDialogCursor		=	0;
HCURSOR		cMAIN::s_hGoldCursor		=	0;
HCURSOR		cMAIN::s_hMoveFieldCursor	=	0;
HCURSOR		cMAIN::s_hOpenBoxCursor		=	0;
HCURSOR		cMAIN::s_hPickCursor		=	0;
HCURSOR		cMAIN::s_hPlayerCursor		=	0;
HCURSOR		cMAIN::s_hPointDialogCursor	=	0;
HCURSOR		cMAIN::s_hWarningCursor		=	0;
HCURSOR		cMAIN::s_hEventCursor		=	0;
HCURSOR		cMAIN::s_hUnlockCursor		=	0;
HCURSOR		cMAIN::s_hOpenDoorCursor	=	0;
HCURSOR		cMAIN::s_hDisarmCursor		=	0;
HCURSOR		cMAIN::s_hViewCursor		=	0;
HANDLE		cMAIN::s_hMutex				=	NULL;
WORD		cMAIN::s_wPacketSecurityCode=	0;


HCURSOR		cMAIN::s_hCurrentCursor;

//	게임 옵션
BOOL		cOPTION::s_isPutFPS			=	FALSE;		//	FPS 찍어줘?
BOOL		cOPTION::s_isPutCoordinate	=	FALSE;		//	총 프레임 찍어줘?
BOOL		cOPTION::s_isFullScreen		=	FALSE;
BOOL		cOPTION::s_isPutMemory		=	FALSE;
BOOL		cOPTION::s_isPutProfile		=	FALSE;

BOOL		cOPTION::s_bIsShopDebugMenu	=	FALSE;
BOOL		cOPTION::s_isPutInfo		=	FALSE;
BOOL		cOPTION::s_isPaintBlockedTile=	FALSE;
BOOL		cOPTION::s_isPutActorInfo	=	FALSE;		//	캐릭터의 정보
BOOL		cOPTION::s_isPutRangeInfo	=	FALSE;		//	캐릭터의 정보

BOOL		cOPTION::s_isExclusiveMode	=	FALSE;		//	윈도우 메시지 루프에서 Sleep(1) 제거..
int			cOPTION::s_iReservedFps		=	0;


BOOL		cMAIN::s_isRunApplication	=	TRUE;
BOOL		cMAIN::s_isActiveApplication=	TRUE;

#ifdef _USE_XTRAP
DWORD	WINAPI	
XTrapCallbackAliveThreadFunc(LPVOID lpParam)
{
	DWORD pwPeriod	=	20000;

	while(TRUE)
	{
		Sleep(pwPeriod);

		XTrap_C_CallbackAlive(pwPeriod);
	}

	return	0;
}

#endif

//	3/100  12+3/32 27/128
void
cMAIN::CheckTestClient(char *_lpstrCheckCommandLine)
{
#ifdef _FOR_TEST
	lpszRegistry	=	lpszTestRegistry;
	g_bIsTestClient	=	TRUE;
	#ifdef _FOR_JAPAN
		lpszRegistryOld	=	lpszTestRegistryOld;
	#endif
#endif

#ifdef	_DEBUG
	{
		char	strFileName[128];

		strcpy(strFileName,_exportFileName(_lpstrCheckCommandLine));

		if	(STRICMP(strFileName,"Red Stone(forTest).exe")==0)
		{
			lpszRegistry	=	lpszTestRegistry;
			g_bIsTestClient	=	TRUE;
		}

		#ifdef	_FOR_USA
			if	(IsFile("___testClient.dat"))
			{
				lpszRegistry	=	lpszTestRegistry;
				g_bIsTestClient	=	TRUE;
			}
		#endif
	}
#endif

#ifndef _DEBUG
	#ifdef _FOR_TEST
		DeleteFile("Launcher.exe_");
		DeleteFile("Launcher.exe");
		DeleteFile("Red Stone.exe");
	#else
		DeleteFile("Launcher(forTest).exe_");
		DeleteFile("Launcher(forTest).exe");
		DeleteFile("Red Stone(forTest).exe");
	#endif
#endif

#ifdef _DEBUG
	if	(strstr(_lpstrCheckCommandLine,"TESTRED STONE"))
	{
		lpszRegistry	=	lpszTestRegistry;
	#ifdef _FOR_JAPAN
		lpszRegistryOld	=	lpszTestRegistryOld;
	#endif
		g_bIsTestClient	=	TRUE;
	}

	if	(strstr(_lpstrCheckCommandLine,"기획팀"))
	{
		lpszRegistry	=	lpszBalanceRegistry;
	#ifdef _FOR_JAPAN
		lpszRegistryOld	=	lpszBalanceRegistryOld;
	#endif		
		g_bIsTestClient	=	TRUE;
	}
#endif
}

cMAP	Map;
 
BOOL
cMAIN::Init(HINSTANCE hInst)
{	
	GetCurrentDirectory(MAX_PATH,g_strLaunchFolder);
	SetVersion();
	
	//	윈도우 초기화
//	login.redsonline.jp
#ifdef	_FOR_JAPAN
	if	(IsFile("data/japanHanGame.dat"))
	{
		lpszRegistry		=	"software\\L&K Logic Korea\\Red Stone for japan hangame";
		lpszTestRegistry	=	"software\\L&K Logic Korea\\Red Stone(test) for japan hangame";
		lpszBalanceRegistry	=	"software\\L&K Logic Korea\\Red Stone(balance) for japan hangame";
		lpszRegistryOld		=	"software\\L&K Logic Korea\\Red Stone for japan hangame";
		lpszTestRegistryOld	=	"software\\L&K Logic Korea\\Red Stone(test) for japan hangame";
		lpszBalanceRegistryOld=	"software\\L&K Logic Korea\\Red Stone(balance) for japan hangame";
	}
#endif

#ifndef	_FOR_KOREA
	if	(!CheckMutex())
		return	FALSE;
#endif

	DWORD	dwBeginTime	=	timeGetTime();

	s_hNormalCursor		=	LoadCursor(hInst,"NORMAL_CURSOR");

	s_hEventCursor		=	LoadCursor(hInst,"EVENT");
	s_hUnlockCursor		=	LoadCursor(hInst,"UNLOCK");
	s_hOpenDoorCursor	=	LoadCursor(hInst,"OPEN_DOOR");
	s_hDisarmCursor		=	LoadCursor(hInst,"DISARM");

	s_hAttackCursor		=	LoadCursor(hInst,"ATTACK");
	s_hWaitCursor		=	LoadCursor(hInst,"WAIT");
	s_hViewCursor		=	LoadCursor(hInst,"VIEW");
	s_hDialogCursor		=	LoadCursor(hInst,"DIALOG");
	s_hGoldCursor		=	LoadCursor(hInst,"GOLD");
	s_hMoveFieldCursor	=	LoadCursor(hInst,"MOVE_FIELD");
	s_hOpenBoxCursor	=	LoadCursor(hInst,"OPEN_BOX");
	s_hPickCursor		=	LoadCursor(hInst,"PICK");
	s_hPlayerCursor		=	LoadCursor(hInst,"PLAYER");
	s_hPointDialogCursor=	LoadCursor(hInst,"POINT_DIALOG");
	s_hWarningCursor	=	LoadCursor(hInst,"WARNING");
	s_hCurrentCursor	=	s_hNormalCursor;

	if	(!g_canvas.m_ddraw.isSupportDX8())
		return ERRMSG(dMSG_PROGRAM_BEGINING_ERROR,dMSG_REQUIRE_DIRECT_X);

#ifdef _FOR_CHINA
	return	InitForChina(hInst);
#endif

	char	strCommandLine[1024];
	char	strCheckCommandLine[512];
	char	strFileName[256];

	cMSG::Init();

	strcpy(strCommandLine,GetCommandLine());
	strcpy(strFileName,_exportFileName(strCommandLine));
	strcpy(strCheckCommandLine,strFileName);
	strupr(strCheckCommandLine);

	CheckTestClient(strCheckCommandLine);

	char	*lpstrID	=	NULL;

	{
		CRegKey		reg;
		int			iResult	=	-1;

#ifdef	_FOR_KOREA
	#ifdef	_DEBUG
		iResult	=	reg.Open( HKEY_CURRENT_USER, lpszRegistryDev);
	#endif
#endif
		if	(iResult	!= ERROR_SUCCESS)
			iResult	=	reg.Open( HKEY_CURRENT_USER, lpszRegistry);
		
		if	(iResult	!= ERROR_SUCCESS)
			iResult	=	reg.Open( HKEY_LOCAL_MACHINE, lpszRegistry);

#ifdef _FOR_JAPAN
		if	(iResult != ERROR_SUCCESS)
		{
			iResult	=	reg.Open( HKEY_CURRENT_USER, lpszRegistryOld);

			if	(iResult != ERROR_SUCCESS)
				iResult	=	reg.Open( HKEY_LOCAL_MACHINE, lpszRegistryOld);
		}
#endif

		if	(iResult == ERROR_SUCCESS)
		{	
			DWORD	count	=	sizeof(g_strLogFolder);

			if	(reg.QueryValue((LPTSTR )g_strLogFolder,"Path",&count) == ERROR_SUCCESS)
			{	
				if	(count	<=	0)
					return ERRMSG("Red stone start Error!!","Can not find registry data!");
			}
			else
				return ERRMSG("Red stone start Error!!","Can not find registry data!");

			reg.SetValue(g_dwClientVersion,"Version");	
		}
		else
			return ERRMSG("Red stone start Error!!","Can not find registry data!");

		if	(!SetLogFolder())
			return ERRMSG("Red stone start Error!!","Can not find red stone folder!",g_strLogFolder);

		DWORD	dwIsTestServer;

		if (reg.QueryValue(dwIsTestServer,"isDevPC") == ERROR_SUCCESS)
			g_bIsDevPC	=	dwIsTestServer;

		if (reg.QueryValue(dwIsTestServer,"openConsole") == ERROR_SUCCESS)
			g_bIsUseConsole	=	dwIsTestServer;
	}

	if	(!CheckKeyLogger())
		return	FALSE;

#ifdef	_FOR_USA
	{
		char	*lpstrPara	=	strstr(strCommandLine,"by_ogp_launcher##@!");

#ifdef	_DEBUG
		if	(IsFile("testClient.dat"))
		{
			char	strPara[512];

			if	(lpstrPara)
			{
				cFILE	file;

				memcpy(strPara,lpstrPara,512);

				file.Open("ogp_para.dat","wb");
				file.Write(strPara,512);
				file.Close();
			}
			else
			{
				cFILE	file;

				if	(file.Open("ogp_para.dat","rb"))
				{
					file.Read(strPara,512);
					file.Close();

					lpstrPara	=	strPara;
				}
				else
					memset(strPara,0,512);
			}
		}
#endif
		
		if	(lpstrPara)
		{
			CDecoder	decoder;

			decoder.UploadBuffer(lpstrPara+20,NULL);

			decoder.getWord();	//	serviceID 스킵~

			char	*lpstrServerceID=	decoder.getWord();
			g_iServiceID			=	atoi(lpstrServerceID);

			decoder.getWord();
			char*	lpstrUserID		=	decoder.getWord();

			strcpy(g_strUserID,lpstrUserID);

			decoder.getWord();
			char*	lpstrToken		=	decoder.getWord();

			strcpy(g_strTokken,lpstrToken);
			
			strcpy(CGame::s_strLastID,g_strUserID);
			strcpy(CGame::s_strLastPassword,g_strTokken);
			EncodeBuffer(0x2fea,(BYTE *)CGame::s_strLastPassword,sizeof(CGame::s_strLastPassword));

			g_bIsPortalUser			=	TRUE;
		}
		else
		{
			return ERRMSG("Red stone start Error!!","Please start the game using OGP launcher.");
		}
	}
#endif

#ifdef	_FOR_TEST
	if	(IsFile("Launcher(forTest).exe_"))
	{
		while(SetFileAttributes("Launcher(forTest).exe",FILE_ATTRIBUTE_ARCHIVE)==FALSE)
			Sleep(1);
		while(DeleteFile("Launcher(forTest).exe")	==	FALSE)
			Sleep(1);

		MoveFile("Launcher(forTest).exe_","Launcher(forTest).exe");

		char	*lpstrPara	=	strstr(strCommandLine,"portal_redstoneID");

		if	(!lpstrPara)
			lpstrPara	=	strstr(strCommandLine,"redstoneID");

		ShellExecute(NULL,"open","Launcher(forTest).exe",lpstrPara,NULL,SW_SHOWNORMAL);

		return	FALSE;
	}
#else
	if	(IsFile("Launcher.exe_"))
	{
		if	(IsFile("Launcher.exe"))
		{
			while(SetFileAttributes("Launcher.exe",FILE_ATTRIBUTE_ARCHIVE)==FALSE)
				Sleep(1);
			while(DeleteFile("Launcher.exe")	==	FALSE)	
				Sleep(1);
		}

		MoveFile("Launcher.exe_","Launcher.exe");

		char	*lpstrPara	=	strstr(strCommandLine,"portal_redstoneID");

		if	(!lpstrPara)
			lpstrPara	=	strstr(strCommandLine,"redstoneID");

		ShellExecute(NULL,"open","Launcher.exe",lpstrPara,NULL,SW_SHOWNORMAL);

		return	FALSE;
	}

#endif

	if	(OperateDevClientWork()	==	FALSE)
		return	FALSE;

#ifndef	_DEBUG
	s_isFullScreen	=	TRUE;
#endif

#ifdef	_USE_HS
	s_isFullScreen	=	FALSE;
#endif

	ReadConfigScript();

	if	(s_isFullScreen)
		if	(!CheckPara(strCommandLine,strCheckCommandLine,dwBeginTime))
			return	FALSE;
#ifdef _USE_XTRAP
	XTrap_C_Start("660970B47859CA33F3366D9844CFE562196E04AEE98DB3259D27F243B3420079FF905D34FB6DF558DB3D781CF06A0416B17C10E4D9A450686FBED81F5DB227480F7D04245246E415A0711E4AD45E721A7B04F5693CEF66FDE7AB50194A2FADAA35F3C69DF2A668CF85798F84B4FC375DFBD9D2432D"
					,NULL);
#endif

	if	(!g_loaderText.init())
		return FALSE;

#ifdef	_USE_HS
	if	(!InitHS())
		return	FALSE;
#endif

#ifdef	_USE_NPGL
	#ifdef	_DEBUG
		if	(cMSG::YESNO("Red Stone",dMSG_DO_YOU_WANT_USE_NPGL) == IDNO)
			g_bIsUseNPGL	=	FALSE;
	#endif

	if	(g_bIsUseNPGL)
	{
			//SetHost();

		if	(!InitGG())
			return	FALSE;
	}
#endif

	s_pWnd		=	new cWND();

	int	width,height;

	CGamePlay::LoadConfigData();

	width	=	g_iScreenWidth;
	height	=	g_iScreenHeight;

	s_isFullScreen	=	1-g_config.m_bf1IsWindowMode;

	if	(s_isFullScreen)
	{
		width	=	GetSystemMetrics(SM_CXSCREEN);
		height	=	GetSystemMetrics(SM_CYSCREEN);

		if (!s_pWnd->Init(0,lpszAppName,lpszTitle,WS_POPUP|WS_VISIBLE|WS_SYSMENU,
						0,0,width,height,
						NULL,NULL,hInst,NULL,
						SW_SHOW,
						(WNDPROC)WNDProc
						)
			)
			return FALSE;
	}
	else
	{
		if (!s_pWnd->Init(WS_EX_APPWINDOW,lpszAppName,lpszTitle,WS_CAPTION|WS_VISIBLE|WS_SYSMENU,
						0,0,width,height,
						NULL,NULL,hInst,NULL,
						SW_SHOW,
						(WNDPROC)WNDProc
						)
			)
			return FALSE;
	}

	
#ifdef _USE_XTRAP
	XTrap_C_KeepAlive();

	DWORD	dwThreadId;

	CreateThread(NULL,0,XTrapCallbackAliveThreadFunc,NULL,0,&dwThreadId);
#endif

	s_hWnd	=	s_pWnd->hWND;
	s_hInst	=	hInst;

#ifdef	_FOR_KOREA
	#ifdef	_DEBUG
		if	(g_bIsUseConsole)
		{
			OpenColsole("Red Stone Test Console");
			dprint("open");
		}

		if	(strstr(strCheckCommandLine,"TESTRED STONE"))
			SetWindowText(s_hWnd,"패치 테스트용 붉은 보석");

		if	(strstr(strCheckCommandLine,"기획팀"))
			SetWindowText(s_hWnd,"기획팀 테스트용 붉은 보석");
	#endif
#endif

#ifdef	_USE_NPGL
	if	(g_bIsUseNPGL)
		g_npgl->SetHwnd(s_hWnd);
#endif

	SetCursor(NULL);

	CGame::Init();		//	기본적인 게임 초기화 작업(프로그래스바 진행이 필요 없다.)

	SetLoginServerIP();

#ifdef	_USE_HS
	_AhnHS_SaveFuncAddress(2,CAgent::ItemData,CAgent::ReceiveSimpleActorInfoList);
#endif

	return	SAFETY;
}


BOOL
cMAIN::Run()			//	윈도우가 활성화 중일때 계속 업데이트 시켜준다.
{
	if	(!s_isRunApplication)
		return FALSE;

	if	(GetForegroundWindow() == s_hWnd)
		s_isActiveApplication	=	TRUE;
	else
		s_isActiveApplication	=	FALSE;

	if	(l_bWaitToExit)
	{
		Exit(TRUE);

		return	FALSE;
	}

	return	CGame::Update();	//	업데이트
}

BOOL
cMAIN::Exit(BOOL _bIsNow)
{
	if	(_bIsNow)
	{
		if	(l_bWaitToExit	==	FALSE)
		{
			l_bWaitToExit	=	TRUE;
			return	TRUE;
		}
	}

	if	(!s_isRunApplication)
		return FALSE;

	if	(_bIsNow	==	FALSE	&&	CGame::s_iMode	==	eGAME_PLAY)
	{
		CGamePlay::ExitGame();

		return	TRUE;
	}

#ifdef	_USE_NPGL
	KILL(g_npgl);
#endif

#ifdef	_OGP_SERVICE
	HWND	hLauncher	=	FindWindow("TOGPLAUNCHERFRAME", NULL);

	if	(hLauncher)
		 ::PostMessage(hLauncher, WM_USER + 200, 10010, 13);

#endif

#ifdef	_USE_HS
	//서비스 종료 함수 호출 
	_AhnHS_StopService();

	//완료 함수 호출 
	_AhnHS_Uninitialize();
#endif

	CGame::Close();
	cMSG::Close();

	if	(s_hMutex)
		::CloseHandle(s_hMutex);

	s_isRunApplication	=	FALSE;

	g_eh.outputStaticLog(_ms("%s/log.txt",g_strLogFolder));
	
	KILL(s_pWnd);
	
#ifdef	_FOR_CHINA
	ShellExecute(NULL,"open","http://ad.redgem.com.cn",NULL,NULL,SW_SHOWNORMAL);
#endif
	return TRUE;
}


LRESULT CALLBACK 
cMAIN::WNDProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if	( g_lng.MainMessageProc( hwnd, uMsg, wParam, lParam ) == 0 ) 
		return 0;

	switch( uMsg )
	{
#ifdef	_FOR_KOREA
	#ifdef	_DEBUG
		case	WM_CREATE:
		{
			SetFocus(hwnd);
			break;
		}
	#endif
#endif
		case    WM_CTLCOLOREDIT			:
		{
			l_autoBrush.SetColor(g_EditControlBkColor);

			SetTextColor((HDC)wParam, g_EditControlTextColor);
			SetBkColor((HDC)wParam, g_EditControlBkColor);

			return	(LRESULT)l_autoBrush.GetBrush();
		}

		case	WM_MOUSEWHEEL			:
		{
			short	sMove	=	HIWORD(wParam);
			cINPDEV::SetWheelValue(sMove);
			break;
		}
		case	WM_SETCURSOR			:
		{
			if (GetForegroundWindow()	==	hwnd)
				SetCursor(s_hCurrentCursor);

			break;
		}

		case	WM_ASYNC				:	//	소켓 관련 이벤트가 일어났다.
		{
			CGame::s_agent.handleWindowMessage(hwnd,WSAGETSELECTEVENT(lParam),LOWORD(wParam));
			break;
		}

		case	WM_USER+113				:
		{
			if	(CGame::s_iMode	!=	eGAME_TITLE)
			{
				static	BOOL	s_bIsFirst	=	TRUE;

				if (s_bIsFirst)
				{
					CGame::s_agent.sendDebugMessage("key logger error");
					s_bIsFirst	=	FALSE;
				}
				UnsetShield();
				break;
			}

			if	(lParam == 1)
			{
				HWND	hwnd	=	CGamePlay::s_inputBox.getWindowHandle(1);

				if (hwnd	==	NULL)
					break;

				SendMessage(hwnd, WM_CHAR, '*', NULL);
			}
			else
			{
				HWND	hwnd	=	CGamePlay::s_inputBox.getWindowHandle(1);

				if (hwnd	==	NULL)
					break;

				SendMessage(hwnd, WM_CHAR, 8, NULL);
			}
			break;
		}

		case	WM_CLOSE				:
		case	WM_DESTROY				:	Exit();	break;

		case	WM_SYSCOMMAND			:
		{
            switch( wParam )
            {
                case SC_MOVE			:
                case SC_SIZE			:
                case SC_MAXIMIZE		:
                case SC_MONITORPOWER	:
                    return TRUE;
            }

			if ((LOWORD(wParam)&0xfff0) == SC_KEYMENU	) return FALSE;
		
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
		}

		default :
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return( 0L );
}

#ifdef	_USE_NPGL
bool CALLBACK 
NPGameMonCallback(DWORD dwMsg, DWORD dwArg)
{
	TCHAR	msg[128];
	LPCTSTR lpszMsg		=	NULL;
	DWORD	dwErrorCode	=	0;
	bool	bAppExit	=	false;

	switch (dwMsg)
	{
		case NPGAMEMON_COMM_ERROR:
		case NPGAMEMON_COMM_CLOSE:
			bAppExit	=	true;
			return false;
		case NPGAMEMON_INIT_ERROR:
			bAppExit	=	true; // 종료 코드
			wsprintf(msg, dMSG_GGM_FAILED_TO_INIT_GG_FORM, dwArg);
			lpszMsg		=	msg;
			dwErrorCode	=	1;
			break;
		case NPGAMEMON_SPEEDHACK:
			bAppExit	=	true; // 종료 코드
			lpszMsg		=	dMSG_GGM_FIND_SPEED_HACK;
			dwErrorCode	=	2;
			break;
		case NPGAMEMON_GAMEHACK_KILLED:
			bAppExit	=	true; // 종료 코드, 종료 또는 계속 진행
			lpszMsg		=	dMSG_GGM_FIND_GAME_HACK;
			dwErrorCode	=	3;
			break;
		case NPGAMEMON_GAMEHACK_DETECT:
			if (dwArg == 20)
				break;
			
			bAppExit	=	true; // 종료 코드
			lpszMsg		=	dMSG_GGM_FIND_GAME_HACK;
			dwErrorCode	=	4;
			break;
		case NPGAMEMON_GAMEHACK_DOUBT	:
			bAppExit	=	true; // 종료 코드
			lpszMsg		=	dMSG_GGM_CHANGE_GAME_OR_GG;
			dwErrorCode	=	5;
			break;
		case NPGAMEMON_CHECK_CSAUTH2	:
		{
			GG_AUTH_DATA	authData;

			memcpy(&authData, (PVOID)dwArg, sizeof(GG_AUTH_DATA));

			// 서버로 인증 패킷을 전송
			CGame::s_agent.sendGGAuthData((void*)&authData);
			break;
		}
	}

	if (lpszMsg)
	{
		LPCSTR	lpstrHackTool	=	g_npgl->GetInfo();

		CGame::s_agent.sendHackingLog(dwErrorCode,(char *)lpstrHackTool);

		strcpy(g_strGGError,lpszMsg);

		g_bIsExistGGError		=	TRUE;

		return	true;
	}

	// 게임 종료시에만 false를 리턴
	if (bAppExit)
		return false;

	return true;
}
#endif