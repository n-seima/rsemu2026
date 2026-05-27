#include "main.h"
#include "cWindow.h"
#include "message.h"
#include "_resource.sd.h"
#include "cWindow.h"
#include "cDraw.h"

#include "update.h"
#include "launcher.h"
#include "cdecoder.h"
#include "atlbase.h"

#ifndef	_DEBUG
#ifdef	_FOR_KOREA
#include "XTrap/XTrap4Launcher.h"
#endif
#endif

int		g_iServiceID	=	0;
char	g_strUserID[20]	=	"";
char	g_strTokken[1024]	=	"";

HKEY	g_hRegKey	=	0;

LPCTSTR		lpszAppName								=	"APFRS";
LPCTSTR		lpszTitle								=	"Red stone Launcher";

#ifdef	_FOR_INTER
LPCTSTR		lpszRegistry							=	"software\\L&K Logic Korea\\Red Stone for Inter";
LPCTSTR		lpszTestRegistry						=	"software\\L&K Logic Korea\\Red Stone(test) for Inter";
char		*lpstrLoginServerAddressInfoFileAddress	=	"http://redstone.gamengame.com/Client/loginserveraddress.txt";
char		g_strAddress[40]	=	"222.111.150.93";
#elif	_FOR_KOREA
LPCTSTR		lpszRegistry							=	"software\\L&K Logic Korea\\Red Stone";
LPCTSTR		lpszTestRegistry						=	"software\\L&K Logic Korea\\Red Stone(test)";
char		*lpstrLoginServerAddressInfoFileAddress	=	"http://www.redgem.co.kr/DOWNLOAD/PATCH/loginserveraddress.txt";
char		g_strAddress[40]	=	"211.189.66.56";
#elif	_FOR_JAPAN
LPCTSTR		lpszRegistry							=	"software\\L&K Logic Korea\\Red Stone for Japan";
LPCTSTR		lpszTestRegistry						=	"software\\L&K Logic Korea\\Red Stone(test) for Japan";

LPCTSTR		lpszRegistryOld							=	"software\\L&K Logic Korea\\Red Stone";
LPCTSTR		lpszTestRegistryOld						=	"software\\L&K Logic Korea\\Red Stone(test)";

char		*lpstrLoginServerAddressInfoFileAddress	=	"";
char		g_strAddress[40]	=	"219.127.143.15";
#elif	_FOR_USA
LPCTSTR		lpszRegistry							=	"software\\L&K Logic Korea\\Red Stone for USA";
LPCTSTR		lpszTestRegistry						=	"software\\L&K Logic Korea\\Red Stone(test) for USA";
char		*lpstrLoginServerAddressInfoFileAddress	=	"http://redstoneonline.net/Client/LoginServerAddress.txt";
//char		g_strAddress[40]	=	"113.130.71.109";
char		g_strAddress[40]	=	"67.201.35.14";
#elif	_FOR_THAI
LPCTSTR		lpszRegistry							=	"software\\L&K Logic Korea\\Red Stone for Thai";
LPCTSTR		lpszTestRegistry						=	"software\\L&K Logic Korea\\Red Stone(test) for Thai";
char		*lpstrLoginServerAddressInfoFileAddress	=	"";
char		g_strAddress[40]	=	"";
#endif



cMain		g_main;
BOOL		g_bActive		=	TRUE;
BOOL		g_bIsForTest	=	FALSE;
cSPRITE		g_sprPannel;

cBarMenu	cCommonFrame::s_menuButton;

char		g_strCommandLine[1024];

#include	"mac.h"

//	초기화
BOOL
cMain::init(HINSTANCE _hInst)
{
// 	g_launcher.m_agent.init(NULL);
// 
//  	std::string strMacAddressByAdapter	=	GetMacAddressByAdapter();
//  	std::string strMacAddressFromSock	=	GetMacAddressFromSock();
//  	std::string strMacAddressFromNetBios=	GetMacAddressFromNetBIOS();
//  	std::string strMacAddress			=	GetMacAddress();
// 
// 	cMSG::Put("mac address report","1. by adapter [%s]\n2. from socket [%s]\n3. from net bios [%s]\n\nresult [%s]",
// 				strMacAddressByAdapter.c_str(),strMacAddressFromSock.c_str(),strMacAddressFromNetBios.c_str(),strMacAddress.c_str());
// 
// 	return	FALSE;

	strcpy(g_strCommandLine,GetCommandLine());

	{
		cDECODER	decoder;
		char		*lpstrKeyword;

		decoder.UploadBuffer(g_strCommandLine,NULL);

		decoder.GetStream();	//	실행 파일 정보

		lpstrKeyword	=	decoder.GetWord();		//	키워드

		if	(lpstrKeyword && stricmp(lpstrKeyword,"serviceid") == NULL)
		{
			char	*lpstrServerceID	=	decoder.GetWord();
			g_iServiceID			=	atoi(lpstrServerceID);

			decoder.GetWord();
			char*	lpstrUserID		=	decoder.GetWord();

			strcpy(g_strUserID,lpstrUserID);

			decoder.GetWord();
			char*	lpstrToken		=	decoder.GetWord();

			strcpy(g_strTokken,lpstrToken);
		}
		else
		{
#ifdef	_FOR_USA
			return ERRMSG("Red stone start Error!!","Please start the game using OGP launcher.");
#endif

		}
	}

	if	(strstr(g_strCommandLine,"Launcher(forTest).exe"))
		g_bIsForTest	=	TRUE;

	if	(IsFile("___testClient.dat"))
		g_bIsForTest	=	TRUE;

	if	(strstr(g_strCommandLine,"hangamejapan"))
	{
		g_iLaunchType		=	launch_by_japan_hangame;

		lpszRegistry		=	"software\\L&K Logic Korea\\Red Stone for japan hangame";
		lpszTestRegistry	=	"software\\L&K Logic Korea\\Red Stone(test) for japan hangame";
	}
	else
	if	(IsFile("data/japanHanGame.dat"))
		return	FALSE;


	if	(g_bIsForTest)
	{
		lpszRegistry	=	lpszTestRegistry;
#ifdef	_FOR_JAPAN
		lpszRegistryOld	=	lpszTestRegistryOld;
#endif

#ifdef	_FOR_USA
		strcpy(g_strAddress,"67.201.35.24");
#endif
	}

	{
		CRegKey		reg;

		int	iResult		=	reg.Open( HKEY_CURRENT_USER, lpszRegistry);

		if	(iResult	!=	ERROR_SUCCESS)
		{
			g_hRegKey	=	HKEY_LOCAL_MACHINE;
			iResult		=	reg.Open( HKEY_LOCAL_MACHINE, lpszRegistry);
		}
		else
			g_hRegKey	=	HKEY_CURRENT_USER;

		if	(iResult	!=	ERROR_SUCCESS)
			g_hRegKey	=	HKEY_CURRENT_USER;

#ifdef	_FOR_JAPAN
		if	(iResult	!=	ERROR_SUCCESS)
			lpszRegistry	=	lpszRegistryOld;

		iResult		=	reg.Open( HKEY_CURRENT_USER, lpszRegistry);

		if	(iResult	!=	ERROR_SUCCESS)
		{
			g_hRegKey	=	HKEY_LOCAL_MACHINE;
			iResult		=	reg.Open( HKEY_LOCAL_MACHINE, lpszRegistry);
		}
		else
			g_hRegKey	=	HKEY_CURRENT_USER;

		if	(iResult	!=	ERROR_SUCCESS)
			g_hRegKey	=	HKEY_CURRENT_USER;
#endif
	}

	cDRAW::Init();

	__UploadIRSData();

	g_sprPannel.uploadImage(__g_iIRS_Count,__g_pIRS_Data,__g_aIRS_Offset);

	__DestroyIRS();

	int	width			=	GetSystemMetrics(SM_CXSCREEN);
	int	height			=	GetSystemMetrics(SM_CYSCREEN);

	WORD	*lpSprite	=	g_sprPannel.Get16(0);

	if	(!cDIBWND::Init(0,lpszAppName,dMSG_TITLE,WS_POPUPWINDOW,
					(width-lpSprite[0])/2,(height-lpSprite[1])/2,lpSprite[0],lpSprite[1],
					NULL,NULL,_hInst,NULL,
					SW_SHOW,
					(WNDPROC)WndProc
					)
		)
		return FALSE;

	m_startButton.Init("Start",hWND,_hInst,0,IDOK,-10000,0,60,30);
	m_startButton.DISABLE();

	m_lpActiveFrame		=	&g_launcher;
	m_lpActiveFrame->s_menuButton.init(&g_sprPannel,20);
	m_lpActiveFrame->init(hWND);

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

	static	UINT	dwLastDrawTime	=	0;

	if	(dwLastDrawTime+100	<	timeGetTime())
	{
		dwLastDrawTime	=	timeGetTime();

		draw();
	}

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
		m_lpActiveFrame->readyToInit(hWND);
		m_lpNextFrame	=	NULL;
	}
}

void
cMain::receiveNotice(void *_lpData)
{
	m_lpActiveFrame->receiveNotice(_lpData);
}

void
cMain::draw()
{
	if	(!Active())
		return;

	g_sprPannel.Put(0,0,0);

	m_lpActiveFrame->draw();

#ifdef	_FOR_KOREA
	g_sprPannel.Put(36,431,36);
#endif

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

		case	WM_ASYNC		:	//	소켓 관련 이벤트가 일어났다.
		{
			g_main.m_lpActiveFrame->connectEvent(wParam,lParam);
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

		default :
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return( 0L );
}