#include "main.h"
#include "cWindow.h"
#include "message.h"
#include "_resource.sd.h"
#include "cWindow.h"
#include "cDraw.h"

#include "complete.h"
#include "installData.h"
#include "license.h"
#include "welcome.h"
#include "cdecoder.h"
#include "cTEXT.h"

int				g_iTestWindowButtonHeight	=	0;
int				g_iWindowHeight				=	0;

LPCTSTR		lpszAppName		=	"RedStoneInstall";
char		*g_strLogFile	=	"uninstall.dat";

cMain		g_main;
BOOL		g_bActive		=	TRUE;


char		g_strInstallFolder[1024];

cSPRITE		g_sprPannel;

cBarMenu	cCommonFrame::s_menuButton;

#ifdef	_FOR_JAPAN_HANGAME
LPCTSTR			lpszRegistry				=	"software\\L&K Logic Korea\\Red Stone for japan hangame";
LPCTSTR			lpszTestRegistry			=	"software\\L&K Logic Korea\\Red Stone for japan hangame(test)";
LPCTSTR			lpszBalanceRegistry			=	"software\\L&K Logic Korea\\Red Stone for japan hangame(balance)";
LPCTSTR			lpszUninstallRegistry		=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone for japan hangame";
LPCTSTR			lpszUninstallTestRegistry	=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone for japan hangame(test)";
LPCTSTR			lpszUninstallBalanceRegistry=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone for japan hangame(balance)";
#elif	_FOR_KOREA
LPCTSTR			lpszRegistry				=	"software\\L&K Logic Korea\\Red Stone";
LPCTSTR			lpszTestRegistry			=	"software\\L&K Logic Korea\\Red Stone(test)";
LPCTSTR			lpszBalanceRegistry			=	"software\\L&K Logic Korea\\Red Stone(balance)";
LPCTSTR			lpszUninstallRegistry		=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone";
LPCTSTR			lpszUninstallTestRegistry	=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(test)";
LPCTSTR			lpszUninstallBalanceRegistry=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(balance)";
#elif	_FOR_JAPAN
LPCTSTR			lpszRegistry				=	"software\\L&K Logic Korea\\Red Stone for Japan";
LPCTSTR			lpszTestRegistry			=	"software\\L&K Logic Korea\\Red Stone(test) for Japan";
LPCTSTR			lpszBalanceRegistry			=	"software\\L&K Logic Korea\\Red Stone(balance) for Japan";
LPCTSTR			lpszUninstallRegistry		=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone";
LPCTSTR			lpszUninstallTestRegistry	=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(test)";
LPCTSTR			lpszUninstallBalanceRegistry=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(balance)";
#elif	_FOR_INTER
LPCTSTR			lpszRegistry				=	"software\\L&K Logic Korea\\Red Stone for Inter";
LPCTSTR			lpszTestRegistry			=	"software\\L&K Logic Korea\\Red Stone(test) for Inter";
LPCTSTR			lpszBalanceRegistry			=	"software\\L&K Logic Korea\\Red Stone(balance) for Inter";
LPCTSTR			lpszUninstallRegistry		=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone for Inter";
LPCTSTR			lpszUninstallTestRegistry	=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(test) for Inter";
LPCTSTR			lpszUninstallBalanceRegistry=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(balance) for Inter";
#elif	_FOR_USA
LPCTSTR			lpszRegistry				=	"software\\L&K Logic Korea\\Red Stone for USA";
LPCTSTR			lpszTestRegistry			=	"software\\L&K Logic Korea\\Red Stone(test) for USA";
LPCTSTR			lpszBalanceRegistry			=	"software\\L&K Logic Korea\\Red Stone(balance) for USA";
LPCTSTR			lpszUninstallRegistry		=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone for USA";
LPCTSTR			lpszUninstallTestRegistry	=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(test) for USA";
LPCTSTR			lpszUninstallBalanceRegistry=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(balance) for USA";
#elif	_FOR_WCG
LPCTSTR			lpszRegistry				=	"software\\L&K Logic Korea\\Red Stone for WCG";
LPCTSTR			lpszTestRegistry			=	"software\\L&K Logic Korea\\Red Stone(test) for WCG";
LPCTSTR			lpszBalanceRegistry			=	"software\\L&K Logic Korea\\Red Stone(balance) for WCG";
LPCTSTR			lpszUninstallRegistry		=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone for WCG";
LPCTSTR			lpszUninstallTestRegistry	=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(test) for WCG";
LPCTSTR			lpszUninstallBalanceRegistry=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(balance) for WCG";
#elif	_FOR_THAI
LPCTSTR			lpszRegistry				=	"software\\L&K Logic Korea\\Red Stone for Thai";
LPCTSTR			lpszTestRegistry			=	"software\\L&K Logic Korea\\Red Stone(test) for Thai";
LPCTSTR			lpszBalanceRegistry			=	"software\\L&K Logic Korea\\Red Stone(balance) for Thai";
LPCTSTR			lpszUninstallRegistry		=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone for Thai";
LPCTSTR			lpszUninstallTestRegistry	=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(test) for Thai";
LPCTSTR			lpszUninstallBalanceRegistry=	"software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Red Stone(balance) for Thai";
#endif 

#ifdef	_FOR_KOREA
int				g_iVersion					=	535;
#elif	_FOR_JAPAN
int				g_iVersion					=	540;
#elif	_FOR_USA
int				g_iVersion					=	443;
#elif	_FOR_WCG
int				g_iVersion					=	470;
#elif	_FOR_INTER
int				g_iVersion					=	388;
#elif	_FOR_THAI
int				g_iVersion					=	434;
#endif

int				g_iPortalType	=	ePortal_RedStone;

BOOL			g_bIsChannelingService		=	FALSE;
BOOL			g_bIsTestClient				=	FALSE;
char			g_strExcuteFileName[1024];
char			g_strPortalRedstoneAddress[256];

LPCTSTR			lpstrPlayNetAdress			=	"http://playnet.naver.com/bridge.nhn?pno=P_REDGEM";

LPCTSTR			lpstrYahooAdress			=	"http://redgem.yahoo.co.kr";
LPCTSTR			lpstrParanAdress			=	"http://redgem.paran.com";

//	±­
BOOL
cMain::init(HINSTANCE _hInst)
{
	m_bIsCancel	=	FALSE;

	char	strCommandLine[256];
	char	strCheckStr[256];

	strcpy(strCommandLine,GetCommandLine());
	strcpy(strCheckStr,strCommandLine);
	strupr(strCheckStr);

	char	*lpstrYahooInstall		=	strstr(strCheckStr,"YAHOOINSTALL");
	char	*lpstrParanInstall		=	strstr(strCheckStr,"PARANINSTALL");
	char	*lpstrAnycallInstall	=	strstr(strCheckStr,"ANYCALLINSTALL");
	char	*lpstrPlayNetInstall	=	strstr(strCheckStr,"PLAYNETINSTALL");
	char	*lpstrIsYahoo			=	strstr(strCheckStr,"SETUPRSFORYAHOO");
	char	*lpstrIsParan			=	strstr(strCheckStr,"SETUPRAFORPARAN");

	if	(lpstrPlayNetInstall)
	{
		g_iPortalType			=	ePortal_PlayNet;
		g_bIsChannelingService	=	TRUE;
 		strcpy(g_strPortalRedstoneAddress,lpstrPlayNetAdress);
	}
	if	(lpstrYahooInstall || lpstrIsYahoo)
	{
		g_bIsChannelingService	=	TRUE;
		strcpy(g_strPortalRedstoneAddress,lpstrYahooAdress);
	}
	if	(lpstrParanInstall || lpstrIsParan)
	{
		g_bIsChannelingService	=	TRUE;
		strcpy(g_strPortalRedstoneAddress,lpstrParanAdress);
	}

#ifdef	_FOR_JAPAN_HANGAME
	g_bIsChannelingService	=	TRUE;
	strcpy(g_strPortalRedstoneAddress,"http://redstone.hangame.co.jp");
#endif

	hINST		=	_hInst;

	strcpy(g_strExcuteFileName,GetExcuteFileName());

	char	strCompare[512];

	strcpy(strCompare,_exportFileName(g_strExcuteFileName));
	strupr(strCompare);

	strcpy(g_strGameName,dMSG_GAME_NAME);

	if (strstr(strCompare,"TEST"))
	{
		lpszRegistry			=	lpszTestRegistry;
		lpszUninstallRegistry	=	lpszUninstallTestRegistry;

		sprintf(g_strGameName,"%s(test)",dMSG_GAME_NAME);

		g_bIsTestClient			=	TRUE;
	}
	else
	if (strstr(strCompare,"±¹Æ"))
	{
		lpszRegistry			=	lpszBalanceRegistry;
		lpszUninstallRegistry	=	lpszUninstallBalanceRegistry;

		sprintf(g_strGameName,"%s(±¹Æ)",dMSG_GAME_NAME);

		g_bIsTestClient			=	TRUE;
	}

	cDRAW::Init();

	__UploadIRSData();

	g_sprPannel.uploadImage(__g_iIRS_Count,__g_pIRS_Data,__g_aIRS_Offset);

	__DestroyIRS();

	int	width			=	GetSystemMetrics(SM_CXSCREEN);
	int	height			=	GetSystemMetrics(SM_CYSCREEN);

	WORD	*lpSprite	=	g_sprPannel.Get16(0);

	if	(!cDIBWND::Init(0,lpszAppName,dMSG_TITLE,WS_POPUPWINDOW,
					(width-lpSprite[0])/2,(height-lpSprite[1])/2,lpSprite[0],lpSprite[1]+g_iTestWindowButtonHeight,
					NULL,NULL,_hInst,NULL,
					SW_SHOW,
					(WNDPROC)WndProc
					)
		)
		return FALSE;

	g_iWindowHeight		=	lpSprite[1]+g_iTestWindowButtonHeight;

	m_lpActiveFrame		=	&g_welcome;
	m_lpActiveFrame->s_menuButton.init(&g_sprPannel,20);
	m_lpActiveFrame->init();

	cINPDEV::Init(hWND);

	draw();

	SetTimer(hWND,1000,1000/10,NULL);

	return	TRUE;
}

BOOL
cMain::run()
{
	cINPDEV::Update();

	BOOL	bIsRunning	=	m_lpActiveFrame->run();

	operateChangeFrameWork();

	draw();

	if	(m_bIsCancel)
		bIsRunning	=	FALSE;
	
	return	bIsRunning;
}

void
cMain::changeFrame(cCommonFrame *_lpNextFrame)
{
	m_lpActiveFrame->readyToClose();
	m_lpNextFrame				=	_lpNextFrame;
}

void
cMain::operateChangeFrameWork()
{
	if	(!m_lpNextFrame)
		return;

	if	(m_lpActiveFrame->isDeath())
	{
		m_lpActiveFrame	=	m_lpNextFrame;
		m_lpActiveFrame->readyToInit();
		m_lpNextFrame	=	NULL;
	}
}


void
cMain::draw()
{
	if	(!Active())
		return;

	g_sprPannel.Put(0,0,0);
	g_sprPannel.Put(176,60,1);

	m_lpActiveFrame->draw();
	
	setMinusArea(g_iTestWindowButtonHeight);

	Flip();
}


LRESULT CALLBACK
cMain::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	switch( uMsg )
	{
		case	WM_CREATE		:
			break;

		case	WM_TIMER		:
			break;

		case	WM_ACTIVATE		:
		{
			BOOL	fActive		=	LOWORD(wParam);           // activation flag 
			BOOL	fMinimized	=	(BOOL) HIWORD(wParam); // minimized flag 
			HWND	hwndPrevious=	(HWND) lParam;       // window handle 

			if	(fActive	==	WA_INACTIVE)
				g_bActive	=	FALSE;
			else
			{
				g_bActive	=	TRUE;
				g_main.draw();
			}
			break;
		}

		case	WM_CLOSE		:
			PostQuitMessage(0);
			break;

		case	WM_PAINT		:
		{
			g_main.draw();
			break;
		}

		case	WM_COMMAND		:
			switch( LOWORD( wParam ) )
			{
				case	eWelcome_Menu_Next		:
					g_main.changeFrame(&g_license);
					break;
				case	eWelcome_Menu_Cancel	:
					if	(cMSG::YESNO(dMSG_END_SETUP,dMSG_CANCELED_PROCESS)	==	IDYES)
						g_main.cancel();
					break;
					
				case	eLicence_Menu_Pre		:
					g_main.changeFrame(&g_welcome);
					break;
				case	eLicence_Menu_Permit	:
					if	(g_license.selectInstallFolder())
						g_main.changeFrame(&g_installData);
					break;
				case	eLicence_Menu_Cancel	:
					if	(cMSG::YESNO(dMSG_END_SETUP,dMSG_CANCELED_PROCESS)	==	IDYES)
						g_main.cancel();
					break;

				case	eInstall_Menu_HomePage	:
					break;

				case	eInstall_Menu_Cancel	:
				{
					if	(cNUX::IsPaused())
						break;
					
					cNUX::Pause();

					if	(cMSG::YESNO(dMSG_END_SETUP,dMSG_CANCELED_PROCESS)	==	IDYES)
					{
						cNUX::Cancel();
						g_main.cancel();
						break;

					}

					cNUX::Resume();
					break;
				}

				case	eComplete_Menu_Finish	:
					g_main.cancel();

					break;

				case	eComplete_Menu_LaunchGame:
				{
					if	(g_bIsChannelingService	==	FALSE)
					{
						SetCurrentDirectory(g_strInstallFolder);
						
						if	(g_bIsTestClient)
							ShellExecute(NULL,"open","Launcher(forTest).exe",NULL,NULL,SW_SHOWNORMAL);
						else
							ShellExecute(NULL,"open","Launcher.exe",NULL,NULL,SW_SHOWNORMAL);
					}
					else
					{
						ShellExecute(NULL,"open",g_strPortalRedstoneAddress,NULL,NULL,SW_SHOWNORMAL);
					}
					
					Sleep(1000);
					g_main.cancel();
					
					break;
				}
				
			}

		default :
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return( 0L );
}