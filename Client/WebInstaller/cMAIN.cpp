#include "cMAIN.H"
#include "stdafx.h"
#include <stdio.h>
#include <mmsystem.h>
#include "cDRAW.H"
#include "cMESSAGE.H"
//#include "dataSprite.cpp"

#define	dLOGIN_SERVER_PORT				9049

LPCTSTR			lpszAppName						=	"RedstoneWebInstaller";
LPCTSTR			lpszTitle						=	"Red stone Web Installer";
#ifdef	_FOR_INTER
LPCTSTR			lpszRegistry					=	"software\\L&K Logic Korea\\Red Stone for Inter";
char			*lpstrInstallVersionAddress		=	"http://redstone.globalgamecdn.com/redstone/setupRS/setupRS.exe";
#else
LPCTSTR			lpszRegistry					=	"software\\L&K Logic Korea\\Red Stone";
char			*lpstrInstallVersionAddress		=	"http://redstoneweb.nefficient.co.kr/redstoneweb/cl/setupRS.exe";
#endif
//char			*lpstrInstallVersionAddress		=	"http://redgem.yahoo.co.kr/service/download/SetupRSforYahoo.exe";
//char			*lpstrInstallVersionAddress		=	"ftp://redstone.nefficient.co.kr/pub/gamenjoy/redstone/regPatch.exe";
//char			strInstallVersionAddress[512]	=	"c:\\SpellMage102_setup_kor.exe";

char			g_strSetupOption[256];

//

cMAIN			MAIN;

cDOWNLOAD		cMAIN::s_setup;
cDIBWND			cMAIN::s_dibImageWnd;
cTHREAD			cMAIN::s_threadDownload;

BOOL			l_bIsCancelByUser	=	FALSE;

////////////////////////////////////////////////////////////////////////////////
//									초기화 클래스..
////////////////////////////////////////////////////////////////////////////////

cMAIN::cMAIN()
{
	strcpy(m_strGameFolder,"");
	strcpy(m_strInstallVersionDataAddress,lpstrInstallVersionAddress);
	strcpy(m_strDownLoadFileName,"");
	strcpy(m_strParam,"");

	m_dwLastUpdateTime		=	0;
	m_iCurrentVersion		=	0;
	m_isCompleteUpdate		=	FALSE;
	m_isOperationSuccess	=	FALSE;
	m_bIsServerClose		=	FALSE;
	m_bIsAutoRun			=	FALSE;
	m_bIsCannotFindServer	=	FALSE;
}

//
//	취소
cMAIN::~cMAIN()
{
}	//	cMAIN::~cMAIN()

BOOL
cMAIN::launchGame()
{
	BOOL		bIsInstalled=	TRUE;
	CRegKey		reg;
	DWORD		dwVersion	=	0;
	DWORD		dwAuto		=	0;
	int			iResult		=	reg.Open( HKEY_CURRENT_USER, lpszRegistry);

	if	(iResult	!=	ERROR_SUCCESS)
		iResult		=	reg.Open( HKEY_LOCAL_MACHINE, lpszRegistry);

	if	(iResult	==	ERROR_SUCCESS)
	{
		DWORD	count	=	sizeof(_logFolder);

		if	(reg.QueryValue((LPTSTR )_logFolder,"Path",&count) == ERROR_SUCCESS)
		{	
			if	(count	<=	0)
				bIsInstalled	=	FALSE;
		}
		else
			bIsInstalled	=	FALSE;

		if	(reg.QueryValue(dwVersion,"Version") != ERROR_SUCCESS)	
			bIsInstalled	=	FALSE;
	}
	else
		bIsInstalled	=	FALSE;

	if	(!SetLogFolder())
		return ERRMSG(dMSG_ERROR_AUTO_PATCH,dMSG_CAN_NOT_FIND_REGIST_FOLDER,_logFolder);

	if	(bIsInstalled)
	{
		ShellExecute(NULL,"open","Launcher.exe",m_strParam,NULL,SW_SHOWNORMAL);

		return	TRUE;
	}

	return	FALSE;
}

//
//	윈도우들 초기화
BOOL
cMAIN::Init(HINSTANCE hInst)
{
	char	strCommandLine[512];
	char	strPortalID[64];

	strcpy(strCommandLine,GetCommandLine());

	char	*lpstrParameter	=	strstr(strCommandLine,"begin_rs_parameter");
// 	char	*lpstrIsYahoo	=	strstr(strCommandLine,"isYahoo1");
// 	char	*lpstrIsParan	=	strstr(strCommandLine,"isParan");
// 	char	*lpstrIsInter	=	strstr(strCommandLine,"isInter");
	char	*lpstrIsAnycall	=	strstr(strCommandLine,"@s");
	char	*lpstrIsPlayNet	=	strstr(strCommandLine,"@n");

// 	if	(lpstrIsYahoo)
// 		strcpy(g_strSetupOption,"yahooInstall");
// 
// 	if	(lpstrIsParan)
// 		strcpy(g_strSetupOption,"paranInstall");
// 
 	if	(lpstrIsAnycall)
		strcpy(g_strSetupOption,"anycallInstall");

	if	(lpstrIsPlayNet)
		strcpy(g_strSetupOption,"playNetInstall");

	if	(!lpstrParameter)
		return	FALSE;

	{
		int		iBufferPoint=	strlen("begin_rs_parameter");
		char	strNumber[4];
		int		iLength;

//		if	(lpstrIsYahoo	||	lpstrIsParan	||	lpstrIsInter || lpstrIsAnycall || lpstrIsPlayNet)
		if	(lpstrIsAnycall || lpstrIsPlayNet)
		{
			strcpy(strPortalID,"portal_redstoneID");

			char	strId[32];
			char	strPass[32];

			//	계정
			{
				char	*lpstrId=	lpstrParameter+iBufferPoint;
				strncpy(strNumber,lpstrId,3);
				strNumber[3]	=	NULL;
				iLength			=	atoi(strNumber);

				iBufferPoint	+=	3;
				lpstrId			=	lpstrId+3;

				strncpy(strId,lpstrId,iLength);
				strId[iLength]	=	NULL;

				iBufferPoint	+=	iLength;
			}

			//	비번
			{
				char	*lpstrPass	=	lpstrParameter+iBufferPoint;
				strncpy(strNumber,lpstrPass,3);
				strNumber[3]	=	NULL;
				iLength			=	atoi(strNumber);

				iBufferPoint	+=	3;
				lpstrPass		=	lpstrPass+3;

				strncpy(strPass,lpstrPass,iLength);
				strPass[iLength]=	NULL;

				iBufferPoint	+=	iLength;
			}

			sprintf(m_strParam,"%s%03d%s%03d%s",strPortalID,strlen(strId),strId,strlen(strPass),strPass);
		}

		//	설치본 주소
		{
			char	*lpstrAddress=	lpstrParameter+iBufferPoint;

			strncpy(strNumber,lpstrAddress,3);
			strNumber[3]=	NULL;
			iLength		=	atoi(strNumber);
			lpstrAddress=	lpstrAddress+3;
			iBufferPoint+=	3;

			strncpy(m_strInstallVersionDataAddress,lpstrAddress,iLength);
			m_strInstallVersionDataAddress[iLength]	=	NULL;
		}
	}

//	레지스트리 읽기
	if	(launchGame())	
		return	FALSE;

	hINST		=	hInst;

	if (!cWND::Init(0	,lpszAppName,lpszTitle,WS_OVERLAPPEDWINDOW,
					-10000	,-10000	,10,10,
					NULL,NULL,hInst,NULL,
					SW_SHOW,
					(WNDPROC)WndProc
					)
		)
		return FALSE;

	s_threadDownload.Close();

#ifdef	_FOR_INTER
	DialogBox(hInst,"PROGRESS_BOX_I"	,hWND,(DLGPROC)procProgress	);
#else
	DialogBox(hInst,"PROGRESS_BOX"	,hWND,(DLGPROC)procProgress	);
#endif

	return	FALSE;
}

//
//	업데이트
//
BOOL
cMAIN::Run()
{

	return TRUE;
}	//	cMAIN::Run()

void
cMAIN::InitDlgBox(HWND hDlg)
{
	m_hProgressWnd				=	hDlg;
//	s_dibImageWnd.lpszRegister	=	"UPDATEIMAGEWND";
//	s_dibImageWnd.Init(GetDlgItem(hDlg,IDC_IMAGE),(WNDPROC)procImageWindow);
//	cDRAW::Init();
	s_threadDownload.Create(MAIN.m_hProgressWnd,DownLoad);

	s_setup.setWnd(hDlg);
}

void
cMAIN::Draw()
{
	return;

/*	if (!MAIN.s_dibImageWnd.Active()) return;

	PutSprite16[dPUT_NORMAL](0,0,g_aSprite,32);

	MAIN.s_dibImageWnd.Draw();
*/
}

LRESULT CALLBACK
cMAIN::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	switch( uMsg )
	{	case WM_CREATE		:
			break;

		default :
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return( 0L );
}

LRESULT CALLBACK
cMAIN::procProgress(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case	WM_INITDIALOG	:
			MAIN.InitDlgBox(hDlg);
			break;

		case	WM_COMMAND		:
			switch( LOWORD( wParam ) )
            {
				case	IDOK				:
				{
					MAIN.launchGame();
					EndDialog(hDlg,IDOK);
					break;
				}

				case	IDCANCEL			:
				{
					if (!MAIN.m_isCompleteUpdate)
					{
						s_setup.pause();

						if (cMSG::YESNO(dMSG_CANCEL_WORK,dMSG_ARE_YOU_REALLY_CANCEL_WORK)==IDYES)
						{
							l_bIsCancelByUser	=	TRUE;

							EndDialog(hDlg,IDCANCEL);
							break;
						}

						s_setup.resume();

						break;
					}
					EndDialog(hDlg,IDOK);
					break;
				}

				case	IDC_DOWNLOAD_FINISH	:
				{
					s_threadDownload.Close();
					EnableWindow(GetDlgItem(hDlg,IDOK),TRUE);
					SetFocus(GetDlgItem(hDlg,IDOK));
					break;
				}
			}
			break;
	}

	return (0L);
}


LRESULT CALLBACK 
cMAIN::procImageWindow(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	switch( uMsg )
	{	case WM_PAINT		:
		{
			MAIN.Draw();
			break;
		}

		default :
			return( DefWindowProc( hDlg, uMsg, wParam, lParam ) );
	}

	return( 0L );
}

void
cMAIN::Progress(cDOWNLOAD *_lpInfo)
{
	MAIN.progress(_lpInfo);
}

void
cMAIN::DownLoad()
{
	MAIN.downLoad();
}