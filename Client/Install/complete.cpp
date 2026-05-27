#include	"complete.h"
#include	"stdafx.h"
#include	"message.h"
#include	"cText.h"
#include	"main.h"

cComplete	g_complete;

enum
{
	eCI_title		=	8,
	eCI_index		=	22,
	eCI_complete	=	9,

	eCI_finish_normal=	31,
	eCI_finish_active=	32,
	eCI_finish_pressed=	32,

	eCI_launch_normal=	29,
	eCI_launch_active=	30,
	eCI_launch_pressed=	30,
	eCI_complete_icon=	49,
};

BOOL
cComplete::init()
{
	complete();

	s_menuButton.reset();

	BOOL	bIsUseLaunch	=	TRUE;

#ifdef	_FOR_JAPAN_HANGAME
	bIsUseLaunch	=	FALSE;
#endif
#ifdef	_FOR_USA
	bIsUseLaunch	=	FALSE;
#endif

	if	(bIsUseLaunch)
		s_menuButton.addImageBar(eMenu_LaunchGame,468,495,eCI_launch_normal,eCI_launch_active,eCI_launch_pressed);

	s_menuButton.addImageBar(eMenu_Finish,580,495,eCI_finish_normal,eCI_finish_active,eCI_finish_pressed);

	s_menuButton.setClickImageAllBarMargin(1,1);

	m_iStatus	=	eStatus_Main;

	{
		int	iX	=	-10000;
		
		if	(g_iTestWindowButtonHeight)
			iX	=	0;
		
#ifdef	_FOR_USA
		m_finishButton		.Init("Finish"	,g_main.hWND,g_main.hINST	,0,eComplete_Menu_Finish	,iX+70	,g_iWindowHeight-g_iTestWindowButtonHeight,60,30);
#else
		m_finishButton		.Init("Finish"	,g_main.hWND,g_main.hINST	,0,eComplete_Menu_Finish	,iX		,g_iWindowHeight-g_iTestWindowButtonHeight,60,30);
		m_launchGameButton	.Init("Launch"	,g_main.hWND,g_main.hINST	,0,eComplete_Menu_LaunchGame,iX+70	,g_iWindowHeight-g_iTestWindowButtonHeight,60,30);
#endif
	}

	return	TRUE;
}

void
cComplete::readyToInit()
{
	init();
}

void
cComplete::readyToClose()
{
	m_finishButton.Close();
	m_launchGameButton.Close();
}

//
//	레지스트리 등록하고 상황봐서-_- 프로그램 실행도 시킨다.
void
cComplete::complete()
{
	CRegKey		reg;
	char		strGameReg[512];
	char		strUninstallReg[512];
	char		DesktopFolder[512];
	char		ProgramFolder[512];
	char		strLogFile[1024];
	char		strGameName[512];
	HKEY		hRegKey	=	HKEY_CURRENT_USER;

	strcpy(strGameName,g_strGameName);

#ifdef	_FOR_JAPAN_HANGAME
	sprintf(strGameName,"%s(for Hangame)",g_strGameName);
	hRegKey	=	HKEY_LOCAL_MACHINE;
#endif

	strcpy(strGameReg,lpszRegistry);
	strcpy(strUninstallReg,lpszUninstallRegistry);

	strcpy(strLogFile,_ms("\"%s\\%s\"",g_strInstallFolder,g_strLogFile));

	if	(reg.Create(hRegKey,strGameReg)==ERROR_SUCCESS)
	{
		char	strFolder[512];
		strcpy(strFolder,_exportName(g_strInstallFolder));
		reg.SetValue(strFolder,"Excute Folder");
		reg.SetValue(g_strInstallFolder,"path");
		reg.SetValue(g_iVersion,"Version");
		reg.SetValue(dMSG_GAME_NAME,"Title");

		reg.Close();
	}

	if	(reg.Create(HKEY_LOCAL_MACHINE,strUninstallReg)==ERROR_SUCCESS)
	{
		reg.SetValue(dMSG_GAME_NAME,"DisplayName");
		reg.SetValue(_ms("%s\\uninstall.exe %s"	,g_strInstallFolder,strLogFile),"UninstallString");

		if	(g_bIsTestClient)
			reg.SetValue(_ms("%s\\Launcher(forTest).exe"		,g_strInstallFolder),"DisplayIcon");
		else	
			reg.SetValue(_ms("%s\\Launcher.exe"		,g_strInstallFolder),"DisplayIcon");

		reg.Close();
	}

	reg.Open(HKEY_CURRENT_USER,"software\\microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders");

	DWORD size	=	512;

	reg.QueryValue(DesktopFolder,"Desktop",&size);
	size	=	512;
	reg.QueryValue(ProgramFolder,"Programs",&size);

	cFOLDER::Delete(_ms("%s\\%s",ProgramFolder,strGameName));

	strcpy(ProgramFolder,_ms("%s\\%s",ProgramFolder,strGameName));

	char	strUrl[512];
	char	strShotCut[1024];
	BOOL	bIsForcedWeb	=	FALSE;

#ifdef	_FOR_USA
	bIsForcedWeb	=	TRUE;
#endif

	if	(cFOLDER::Create(ProgramFolder,FALSE))
	{
		SetCurrentDirectory(ProgramFolder);

		BOOL	bIsEng	=	FALSE;
#ifdef	_FOR_USA
		bIsEng	=	TRUE;
#elif	_FOR_WCG
		bIsEng	=	TRUE;
#endif

		if	(bIsForcedWeb)
		{
			cFILE	File;

			strcpy(strUrl,"Red Stone webpage.url");

			File.Open(strUrl,"wb");

			fprintf(File.fp,"[InternetShortcut]\n");
			fprintf(File.fp,"URL=%s\n","http://redstone.ogplanet.com");
			fprintf(File.fp,"Hotkey=0\n");
			fprintf(File.fp,"IconIndex=0\n");
			fprintf(File.fp,_ms("IconFile=%s\\red stone.exe\n",g_strInstallFolder));
			fprintf(File.fp,"WorkingDirectory=\n");
			fprintf(File.fp,"ShowCommand=1\n");

			File.Close();

			CopyFile(strUrl,_ms("%s/%s",ProgramFolder,strUrl),FALSE);
			CopyFile(strUrl,_ms("%s/%s",DesktopFolder,strUrl),FALSE);

			sprintf(strShotCut,"%s\\%s",DesktopFolder,strUrl);
		}

		if	(g_bIsChannelingService)
		{
#ifndef	_FOR_JAPAN_HANGAME
			cFILE	File;

			sprintf(strUrl,"%s.url",strGameName);

			File.Open(strUrl,"wb");

			fprintf(File.fp,"[InternetShortcut]\n");
			fprintf(File.fp,"URL=%s\n",g_strPortalRedstoneAddress);
			fprintf(File.fp,"Hotkey=0\n");
			fprintf(File.fp,"IconIndex=0\n");
			fprintf(File.fp,_ms("IconFile=%s\\red stone.exe\n",g_strInstallFolder));
			fprintf(File.fp,"WorkingDirectory=\n");
			fprintf(File.fp,"ShowCommand=1\n");

			File.Close();

			CopyFile(strUrl,_ms("%s/%s",DesktopFolder,strUrl),FALSE);
#endif
		}
		else
		if	(bIsForcedWeb	==	FALSE)
		{
			if	(g_bIsTestClient)
			{
				_makeShotcut(_ms("%s\\Launcher(forTest).exe"		,g_strInstallFolder	),DesktopFolder,strGameName);
				_makeShotcut(_ms("%s\\Launcher(forTest).exe"		,g_strInstallFolder	),ProgramFolder,strGameName);
			}
			else
			{
				_makeShotcut(_ms("%s\\Launcher.exe"		,g_strInstallFolder	),DesktopFolder,strGameName);
				_makeShotcut(_ms("%s\\Launcher.exe"		,g_strInstallFolder	),ProgramFolder,strGameName);
			}

			strcpy(strShotCut,_ms("%s\\%s.LNK"	,DesktopFolder,strGameName));
		}

		_makeShotcut(_ms("%s\\setup.exe"		,g_strInstallFolder	),ProgramFolder,dMSG_SETTING);
		_makeShotcut(_ms("%s\\uninstall.exe"	,g_strInstallFolder	),ProgramFolder,dMSG_REMOVE_GAME,strLogFile);
	}

	SetCurrentDirectory(g_strInstallFolder);

	cFILE	logFile;

	if (logFile.Open((char *)g_strLogFile,"rb+"))
	{
		strcpy(strShotCut,_ms("%s\\%s.LNK"	,DesktopFolder,strGameName));

		logFile.Seek(0,SEEK_END);
		DWORD	locate	=	logFile.Length;	//	저장될 위치

		//	이게임의 레지스트리(이걸 참조로 이런거 저런거-_- 한다.)
		logFile.Write4(strlen(strGameReg)+1);					//	스트링 사이즈
		logFile.Write(strGameReg,strlen(strGameReg)+1);			//	스트링
#ifdef	_FOR_JAPAN_HANGAME
		logFile.Write4(4);										//	지워야 할 것들의 수
#else
		logFile.Write4(3);										//	지워야 할 것들의 수
#endif
		logFile.Write4(1);										//	폴더다.
		logFile.Write4(strlen(ProgramFolder)+1);				//	스트링 사이즈
		logFile.Write(ProgramFolder,strlen(ProgramFolder)+1);	//	스트링

		logFile.Write4(0);										//	화일이다.
		logFile.Write4(strlen(strShotCut)+1);					//	스트링 사이즈
		logFile.Write(strShotCut,strlen(strShotCut)+1);			//	스트링

		logFile.Write4(2);										//	레지스트리다.
		logFile.Write4(strlen(strUninstallReg)+1);				//	스트링 사이즈
		logFile.Write(strUninstallReg,strlen(strUninstallReg)+1);	//	스트링

#ifdef	_FOR_JAPAN_HANGAME
		char	strFileName[512];
		sprintf(strFileName,"%s/data/japanHanGame.dat",g_strInstallFolder);

		logFile.Write4(0);										//	화일이다.
		logFile.Write4(strlen(strFileName)+1);					//	스트링 사이즈
		logFile.Write(strFileName,strlen(strFileName)+1);			//	스트링
#endif

		logFile.Write(&locate,4);

		logFile.Close();
	}

	SetCurrentDirectory(g_strInstallFolder);

	if	(IsFile("ogpinst_us.exe"))
	{
		SHELLEXECUTEINFO	shi;

		memset(&shi,0,sizeof(shi));
		shi.cbSize	=	sizeof(shi);
		shi.fMask	=	SEE_MASK_NOCLOSEPROCESS;
		shi.lpFile	=	"ogpinst_us.exe";
		shi.lpParameters	=	"/S";
		shi.hwnd	=	g_main.hWND;

		ShellExecuteEx(&shi);

		if	(shi.hProcess) 
		{
			DWORD	dwExitCode	=	STILL_ACTIVE; 

			while(dwExitCode	==	STILL_ACTIVE) 
			{
				WaitForSingleObject( shi.hProcess, 1); 
				GetExitCodeProcess( shi.hProcess, &dwExitCode);
				Sleep(1);
			}
		}
	}
}

BOOL
cComplete::run()
{
	DWORD	dwSelectMenu	=	s_menuButton.update(MOUSE.x,MOUSE.y,KEY,TOUCHLBUTTON,LBUTTON);

	if	(dwSelectMenu	!=	0xffffffff)
		dwSelectMenu	=	dwSelectMenu;

	switch(dwSelectMenu)
	{
		case	eMenu_LaunchGame	:
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
			return	FALSE;

		case	eMenu_Finish	:
			return	FALSE;
	}

	return	TRUE;
}

void
cComplete::draw()
{
	if	(m_iStatus	!=	eStatus_Main)
		return;

	g_sprPannel.Put(176,60,eCI_title);
	g_sprPannel.Put(575,85,eCI_index);
	g_sprPannel.Put(176,105,eCI_complete);
	g_sprPannel.Put(480,185,eCI_complete_icon);

	s_menuButton.draw();

	cTEXT::PushShadow(FALSE);

	cTEXT::FitPut(205,174,450,400,RGB(89,54,0),dMSG_INSTALL_COMPLETE);
	cTEXT::FitPut(205,240,450,400,RGB(89,54,0),dMSG_PRESSS_COMPLETE_BUTTON);

	{
		char	strInstallFolerText[1024];
		
		sprintf(strInstallFolerText,dMSG_INSTALL_FOLDER_FORM,g_strInstallFolder);
		cTEXT::FitPut(205,398,450,400,RGB(89,54,0),strInstallFolerText);
	}

	cTEXT::PopShadow();
}

