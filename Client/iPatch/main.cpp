#include "main.h"
#include "cWindow.h"
#include "message.h"
#include "_resource.sd.h"
#include "cWindow.h"
#include "cDraw.h"
#include "stdafx.h"

#include "complete.h"
#include "installData.h"
#include "welcome.h"

BOOL		g_bIsTemplatePatch	=	FALSE;

LPCTSTR		lpszAppName			=	"RedStonePatch";
char		*g_strLogFile		=	"uninstall.dat";

cMain		g_main;
BOOL		g_bActive		=	TRUE;
char		g_strInstallFolder[1024];

cSPRITE		g_sprPannel;

cBarMenu	cCommonFrame::s_menuButton;

#ifdef	_FOR_JAPAN_HANGAME
LPCTSTR			lpszRegistry			=	"software\\L&K Logic Korea\\Red Stone for japan hangame";
LPCTSTR			lpszTestRegistry		=	"software\\L&K Logic Korea\\Red Stone(test) for japan hangame";
LPCTSTR			lpszBalanceTestRegistry	=	"software\\L&K Logic Korea\\Red Stone(balance) for japan hangame";

LPCTSTR			lpszRegistryOld			=	"software\\L&K Logic Korea\\Red Stone for japan hangame";
LPCTSTR			lpszTestRegistryOld		=	"software\\L&K Logic Korea\\Red Stone(test) for japan hangame";
LPCTSTR			lpszBalanceTestRegistryOld=	"software\\L&K Logic Korea\\Red Stone(balance) for japan hangame";

#elif	_FOR_INTER
LPCTSTR			lpszRegistry			=	"software\\L&K Logic Korea\\Red Stone for Inter";
LPCTSTR			lpszTestRegistry		=	"software\\L&K Logic Korea\\Red Stone(test) for Inter";
LPCTSTR			lpszBalanceTestRegistry	=	"software\\L&K Logic Korea\\Red Stone(balance) for Inter";
#elif	_FOR_USA
LPCTSTR			lpszRegistry			=	"software\\L&K Logic Korea\\Red Stone for USA";
LPCTSTR			lpszTestRegistry		=	"software\\L&K Logic Korea\\Red Stone(test) for USA";
LPCTSTR			lpszBalanceTestRegistry	=	"software\\L&K Logic Korea\\Red Stone(balance) for USA";
#elif	_FOR_THAI
LPCTSTR			lpszRegistry			=	"software\\L&K Logic Korea\\Red Stone for Thai";
LPCTSTR			lpszTestRegistry		=	"software\\L&K Logic Korea\\Red Stone(test) for Thai";
LPCTSTR			lpszBalanceTestRegistry	=	"software\\L&K Logic Korea\\Red Stone(balance) for Thai";
#elif	_FOR_KOREA
LPCTSTR			lpszRegistry			=	"software\\L&K Logic Korea\\Red Stone";
LPCTSTR			lpszTestRegistry		=	"software\\L&K Logic Korea\\Red Stone(test)";
LPCTSTR			lpszBalanceTestRegistry	=	"software\\L&K Logic Korea\\Red Stone(balance)";
#elif	_FOR_JAPAN
LPCTSTR			lpszRegistry			=	"software\\L&K Logic Korea\\Red Stone for Japan";
LPCTSTR			lpszTestRegistry		=	"software\\L&K Logic Korea\\Red Stone(test) for Japan";
LPCTSTR			lpszBalanceTestRegistry	=	"software\\L&K Logic Korea\\Red Stone(balance) for Japan";

LPCTSTR			lpszRegistryOld			=	"software\\L&K Logic Korea\\Red Stone";
LPCTSTR			lpszTestRegistryOld		=	"software\\L&K Logic Korea\\Red Stone(test)";
LPCTSTR			lpszBalanceTestRegistryOld	=	"software\\L&K Logic Korea\\Red Stone(balance)";
#endif

int				l_iRequireMinVersion	=	0;

char			g_strExcuteFileName[1024];
char			g_strPortalRedstoneAddress[256];
BOOL			g_bIsYahoo	=	FALSE;
BOOL			g_bIsParan	=	FALSE;
int				g_iVersion	=	363;

//	√±≠
BOOL
cMain::init(HINSTANCE _hInst)
{
	strcpy(g_strExcuteFileName,GetExcuteFileName());

	{
		char	strCompare[512];
		char	strFileName[512];
		strcpy(strFileName,_exportFileName(g_strExcuteFileName));

		strcpy(strCompare,strFileName);
		strupr(strCompare);

		int	iPos	=	0;

		if	(strstr(strCompare,"TEST"))
		{
			lpszRegistry	=	lpszTestRegistry;
#ifdef	_FOR_JAPAN
			lpszRegistryOld	=	lpszTestRegistryOld;
#endif
			iPos			=	4;
		}
		if	(strstr(strCompare,"±π¿"))
		{
			lpszRegistry	=	lpszBalanceTestRegistry;
#ifdef	_FOR_JAPAN
			lpszRegistryOld	=	lpszBalanceTestRegistryOld;
#endif
			iPos			=	6;
		}

		if	(g_bIsTemplatePatch	==	FALSE)
		{
			char	strCurrentVersion[64],strMinVersion[64];

			strncpy(strCurrentVersion,strFileName+10+iPos,4);
			strncpy(strMinVersion,strFileName+5+iPos,4);

			strCurrentVersion[4]=	NULL;
			strMinVersion[4]	=	NULL;

			l_iRequireMinVersion	=	atoi(strMinVersion);
			g_iVersion				=	atoi(strCurrentVersion);

			if	(g_iVersion	==	0)
				g_iVersion	=	l_iRequireMinVersion;

			if	(l_iRequireMinVersion	==	0	||	g_iVersion	==	0)
				return	ERRMSG(dMSG_PATCH_ERROR,"incorrect file name");

			l_iRequireMinVersion--;
		}
	}

	{
		CRegKey		reg;
		DWORD		dwVersion	=	0;
		DWORD		dwAuto		=	0;

		int			iResult		=	reg.Open( HKEY_CURRENT_USER, lpszRegistry);

		if	(iResult	!=	ERROR_SUCCESS)
			iResult		=	reg.Open( HKEY_LOCAL_MACHINE, lpszRegistry);

#ifdef	_FOR_JAPAN
		if	(iResult	!=	ERROR_SUCCESS)
			iResult		=	reg.Open( HKEY_CURRENT_USER, lpszRegistryOld);
		if	(iResult	!=	ERROR_SUCCESS)
			iResult		=	reg.Open( HKEY_LOCAL_MACHINE, lpszRegistryOld);
#endif

		if	(iResult	== ERROR_SUCCESS)
		{
			DWORD	count	=	sizeof(_logFolder);
			BOOL	bIsError=	FALSE;

			if	(reg.QueryValue((LPTSTR )_logFolder,"Path",&count) == ERROR_SUCCESS)
			{	
				if	(count	<=	0)
				{
					ERRMSG(dMSG_PATCH_ERROR,dMSG_CAN_NOT_FIND_RED_STONE);

					return FALSE;
				}
			}
			else
			{
				ERRMSG(dMSG_PATCH_ERROR,dMSG_CAN_NOT_FIND_RED_STONE);

				return FALSE;
			}

			if	(!g_bIsTemplatePatch && g_iVersion != -1)
			{
				if	(reg.QueryValue(dwVersion,"Version") != ERROR_SUCCESS)
					dwVersion	=	0;

				if	(dwVersion	>	(DWORD)g_iVersion)
				{
					ERRMSG(dMSG_PATCH_ERROR,dMSG_ALREADY_EXIST_HIGH_VERSION_CLIENT);
					return FALSE;
				}

				if	(dwVersion	<	(DWORD)l_iRequireMinVersion)
				{
					ERRMSG(dMSG_PATCH_ERROR,dMSG_THIS_PATCH_MINI_VERSION_FORM,l_iRequireMinVersion/10000,l_iRequireMinVersion%10000);
					return FALSE;
				}
			}

			SetLogFolder();
		}
		else
		{
			ERRMSG(dMSG_PATCH_ERROR,dMSG_CAN_NOT_FIND_RED_STONE);

			return FALSE;
		}
	}

	strcpy((char *)g_strInstallFolder,(char *)_logFolder);

	cDRAW::Init();

	__UploadIRSData();

	g_sprPannel.uploadImage(__g_iIRS_Count,__g_pIRS_Data,__g_aIRS_Offset);

	__DestroyIRS();

	m_lpActiveFrame		=	&g_welcome;
	m_lpActiveFrame->s_menuButton.init(&g_sprPannel,20);
	m_lpActiveFrame->init();

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

		default :
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return( 0L );
}