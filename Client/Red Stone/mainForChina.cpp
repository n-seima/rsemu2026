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

static char	*lpszChinaAppName				=	"REDSTONE";
static char	*lpszChinaTitle					=	"Red Stone - The Lunatic Story";

BOOL
cMAIN::InitForChina(HINSTANCE hInst)
{	//	윈도우 초기화
	cFILE	fileConfig;
	char	strCommandLine[1024];

	strcpy(strCommandLine,GetCommandLine());

	if (fileConfig.Open("red stone.cfg","wb"))
	{
		fileConfig.Write(&g_dwClientVersion,4);
		fileConfig.Close();
	}

	if (!IsFile("launcher.exe"))
	{
		CRegKey		reg;

		if (reg.Open( HKEY_LOCAL_MACHINE, "software\\L&K Logic Korea\\Red Stone") == ERROR_SUCCESS)
		{	
			DWORD	count	=	sizeof(g_strLogFolder);

			if (reg.QueryValue((LPTSTR )g_strLogFolder,"Path",&count) == ERROR_SUCCESS)
			{
				if (count	<=	0)
					return FALSE;
			}
			else	return FALSE;
		}
		else	return FALSE;

		if (!SetLogFolder())
			return FALSE;

		DWORD	dwIsTestServer;

		if (reg.QueryValue(dwIsTestServer,"isTestServer") == ERROR_SUCCESS)
			g_bIsDevPC	=	dwIsTestServer;

		if (!IsFile("launcher.exe"))
			return	ERRMSG(dMSG_PROGRAM_BEGINING_ERROR,dMSG_CAN_NOT_FIND_REGIST_FOLDER,g_strLogFolder);
	}

	if (!CheckKeyLogger())
		return	FALSE;

	if (IsFile("Launcher.exe_"))
	{
		SetFileAttributes("Launcher.exe",FILE_ATTRIBUTE_ARCHIVE);
		DeleteFile("Launcher.exe");
		MoveFile("Launcher.exe_","Launcher.exe");

		char*	lpstrParam	=	strstr(strCommandLine,"redstoneID");

		ShellExecute(NULL,"open","Launcher.exe",lpstrParam,NULL,SW_SHOWNORMAL);

		return	FALSE;
	}

	if (!g_loaderText.init())
		return FALSE;

#ifdef _DEBUG
	s_isFullScreen	=	FALSE;
	ReadConfigScript();
#else

	s_isFullScreen	=	TRUE;

	//	패러미터 분석
	{
		char	*lpstrID	=	NULL;

		BOOL	bIsCorrect		=	FALSE;
		DWORD	dwTime			=	timeGetTime();
		char	*lpstrMongsran;

		lpstrMongsran			=	strstr(strCommandLine,"Mongsran");

		if (lpstrMongsran)
		{
			DWORD	dwReceiveTime=	atoi(lpstrMongsran+strlen("Mongsran"));

			int	iTimeGap	=	dwTime-dwReceiveTime;

			if (iTimeGap	<=	1000*10 && iTimeGap >= 0)	
				bIsCorrect	=	TRUE;
		}

		char*	lpstrParam		=	strstr(strCommandLine,"redstoneID");

		if (lpstrParam)
		{
			int		iBufferPoint=	strlen("redstoneID");
			char	strNumber[4];
			int		iLength;
			char	strId[32];

			//	계정
			{
				char	*lpstrId=	lpstrParam+iBufferPoint;
				strncpy(strNumber,lpstrId,3);
				strNumber[3]	=	NULL;
				iLength			=	atoi(strNumber);

				iBufferPoint	+=	3;
				lpstrId			=	lpstrId+3;

				if (iLength	>=	4)
				{
					strncpy(strId,lpstrId,iLength);
					strId[iLength]=	NULL;
				}

				strcpy(CGameInfo::s_strLastID,strId);
			}
		}

		if (!bIsCorrect)
		{
			ShellExecute(NULL,"open","Launcher.exe",lpstrParam,NULL,SW_SHOWNORMAL);

			return	FALSE;
		}
	}
#endif

	if	(!g_loaderText.init())
		return FALSE;

	s_pWnd		=	new cWND();

	int	width,height;

	if (g_iScreenWidth	==	0)
		g_iScreenWidth	=	800,g_iScreenHeight=	600;

	width	=	g_iScreenWidth;
	height	=	g_iScreenHeight;

	CGamePlay::LoadConfigData();

	width	=	g_iScreenWidth;
	height	=	g_iScreenHeight;

#ifndef	_DEBUG
	if	(g_config.m_bf1IsWindowMode)
		s_isFullScreen	=	FALSE;
	else
		s_isFullScreen	=	TRUE;
#endif

	if (s_isFullScreen)
	{
		width	=	GetSystemMetrics(SM_CXSCREEN);
		height	=	GetSystemMetrics(SM_CYSCREEN);

		if (!s_pWnd->Init(0,lpszChinaAppName,lpszChinaTitle,WS_POPUP|WS_VISIBLE|WS_SYSMENU,
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
		if (!s_pWnd->Init(WS_EX_APPWINDOW,lpszChinaAppName,lpszChinaTitle,WS_CAPTION|WS_VISIBLE|WS_SYSMENU,
						0,0,width,height,
						NULL,NULL,hInst,NULL,
						SW_SHOW,
						(WNDPROC)WNDProc
						)
			)
			return FALSE;
	}

	s_hWnd	=	s_pWnd->hWND;
	s_hInst	=	hInst;

	SetCursor(NULL);

	CGame::Init();		//	기본적인 게임 초기화 작업(프로그래스바 진행이 필요 없다.)

	return	SAFETY;
}
