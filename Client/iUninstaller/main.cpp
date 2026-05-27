#include "main.h"
#include "cWindow.h"
#include "message.h"
#include "_resource.sd.h"
#include "cWindow.h"
#include "cDraw.h"
#include "stdafx.h"

#include "uninstallData.h"
#include "welcome.h"


LPCTSTR		lpszAppName		=	"RedStoneUninstall";
char		*g_strLogFile	=	"uninstall.dat";

cMain		g_main;
BOOL		g_bActive		=	TRUE;
char		g_strInstallFolder[1024];
char		g_strInfoFile[1024];
char		g_strRegistry[1024];
char		g_strExcuteFolder[1024];
char		g_strTitleName[1024];

cSPRITE		g_sprPannel;

cBarMenu	cCommonFrame::s_menuButton;

int				g_iVersion					=	341;
BOOL			g_bIsYahoo					=	FALSE;
BOOL			g_bIsTestClient				=	FALSE;
char			g_strExcuteFileName[1024];


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

BOOL
cMain::readRegistry()
{
	cFILE		logFile;
	int			iLocate,size;
	DWORD		count;

	if	(!logFile.Open(g_strInfoFile,"rb"))
		return	ERRMSG(dMSG_ERROR_UNINSTALL,dMSG_CAN_NOT_FIND_UNINSTALL_INFO,g_strInfoFile);

	logFile.Seek(-4,SEEK_END);

	logFile.Read(&iLocate,4);			//	화일의 정보가 있는 위치
	logFile.Seek(iLocate,SEEK_SET);

	logFile.Read(&size,4);				//	레지스트리 위치
	logFile.Read(g_strRegistry,size);

	logFile.Close();

	CRegKey		reg;

	int			iResult		=	reg.Open( HKEY_CURRENT_USER, g_strRegistry);

	if	(iResult	!=	ERROR_SUCCESS)
		iResult		=	reg.Open( HKEY_LOCAL_MACHINE, g_strRegistry);

	if	(iResult	!=	ERROR_SUCCESS)
	{
		strcpy(g_strRegistry,lpszRegistry);
		iResult		=	reg.Open( HKEY_LOCAL_MACHINE, g_strRegistry);
	}

	if	(iResult	!=	ERROR_SUCCESS)
	{
		strcpy(g_strRegistry,lpszTestRegistry);
		iResult		=	reg.Open( HKEY_LOCAL_MACHINE, g_strRegistry);
	}

#ifdef	_FOR_JAPAN
	if	(iResult	!=	ERROR_SUCCESS)
	{
		strcpy(g_strRegistry,lpszRegistryOld);
		iResult		=	reg.Open( HKEY_LOCAL_MACHINE, g_strRegistry);
	}
	if	(iResult	!=	ERROR_SUCCESS)
	{
		strcpy(g_strRegistry,lpszTestRegistryOld);
		iResult		=	reg.Open( HKEY_LOCAL_MACHINE, g_strRegistry);
	}
#endif


	if	(iResult == ERROR_SUCCESS)
	{
		count	=	sizeof(g_strInstallFolder);
		if	(reg.QueryValue((LPTSTR )g_strInstallFolder,"Path",&count) == ERROR_SUCCESS)
		{	
			if	(count	<=	0)
				return ERRMSG(dMSG_ERROR_UNINSTALL,dMSG_REGISTRY_DATA_BROKEN);
		}
		else
			return ERRMSG(dMSG_ERROR_UNINSTALL,dMSG_REGISTRY_DATA_BROKEN);
	}
	else
		return ERRMSG(dMSG_ERROR_UNINSTALL,dMSG_CAN_NOT_FIND_REGISTRY);

	if	(!SetCurrentDirectory(g_strInstallFolder))	
		return ERRMSG(dMSG_ERROR_UNINSTALL,dMSG_CAN_NOT_FIND_FOLDER,g_strInstallFolder);

	count	=	sizeof(g_strExcuteFolder);
	if	(reg.QueryValue((LPTSTR )g_strExcuteFolder,"Excute Folder",&count) != ERROR_SUCCESS)	
		return ERRMSG(dMSG_ERROR_UNINSTALL,dMSG_REGISTRY_DATA_BROKEN);
	if	(count	<=	0)	
		return ERRMSG(dMSG_ERROR_UNINSTALL,dMSG_REGISTRY_DATA_BROKEN);

	count	=	sizeof(g_strTitleName);
	if	(reg.QueryValue((LPTSTR )g_strTitleName,"Title",&count) != ERROR_SUCCESS)	
		return ERRMSG(dMSG_ERROR_UNINSTALL,dMSG_REGISTRY_DATA_BROKEN);
	if	(count	<=	0)	
		return ERRMSG(dMSG_ERROR_UNINSTALL,dMSG_REGISTRY_DATA_BROKEN);

	reg.Close();

	return	TRUE;
}


//	초기화
BOOL
cMain::init(HINSTANCE _hInst)
{
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

	if	(!cDIBWND::Init(0,lpszAppName,_ms(dMSG_UNINSTALL_PROGRAM,dMSG_RED_STONE),WS_POPUPWINDOW,
					(width-lpSprite[0])/2,(height-lpSprite[1])/2,lpSprite[0],lpSprite[1],
					NULL,NULL,_hInst,NULL,
					SW_SHOW,
					(WNDPROC)WndProc
					)
		)
		return FALSE;

	cINPDEV::Init(hWND);

	strcpy(m_strExcuteFileName,GetExcuteFileName());
	strcpy(g_strInfoFile,GetCommandLine(1));

	if	(strlen(g_strInfoFile)==0)
	{
		if	(IsFile("uninstall.dat"))
			strcpy(g_strInfoFile,_ms("%s\\uninstall.dat",_logFolder));
		else
			return	cMSG::Error(dMSG_UNINSTALL_ERROR,dMSG_CAN_NOT_FIND_UNINSTALL_DATA);
	}

#ifndef	_DEBUG
	if	(strlen(GetCommandLine(2))==0)
	{
		char	tempFile[1024];
		strcpy(g_strInfoFile,_ms("\"%s\" sonaki",g_strInfoFile));

		GetTempPath(1024,tempFile);

		strcpy(tempFile,_ms("%s\\%s",tempFile,_exportFileName(m_strExcuteFileName)));

		CopyFile(m_strExcuteFileName,tempFile,FALSE);

		ShellExecute(NULL,"open",tempFile,g_strInfoFile,NULL,SW_SHOWNORMAL);

		return	FALSE;
	}
#endif

	if (!readRegistry())
		return	FALSE;

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