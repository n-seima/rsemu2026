#include	"complete.h"
#include	"stdafx.h"
#include	"message.h"
#include	"cText.h"

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
	
	if	(bIsUseLaunch && g_bIsYahoo	==	FALSE	&&	g_bIsParan	==	FALSE)
		s_menuButton.addImageBar(eMenu_LaunchGame,468,495,eCI_launch_normal,eCI_launch_active,eCI_launch_pressed);

	s_menuButton.addImageBar(eMenu_Finish,580,495,eCI_finish_normal,eCI_finish_active,eCI_finish_pressed);

	s_menuButton.setClickImageAllBarMargin(1,1);

	m_iStatus	=	eStatus_Main;

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
}

//
//	레지스트리 등록하고 상황봐서-_- 프로그램 실행도 시킨다.
void
cComplete::complete()
{
	if	(g_bIsTemplatePatch)
		return;

	CRegKey		reg;

	int			iResult		=	reg.Open( HKEY_CURRENT_USER, lpszRegistry);

	if	(iResult	!=	ERROR_SUCCESS)
		iResult		=	reg.Open( HKEY_LOCAL_MACHINE, lpszRegistry);

#ifdef	_FOR_JAPAN
	if	(iResult	!=	ERROR_SUCCESS)
		iResult		=	reg.Open( HKEY_CURRENT_USER, lpszRegistryOld);
	if	(iResult	!=	ERROR_SUCCESS)
		iResult		=	reg.Open( HKEY_LOCAL_MACHINE, lpszRegistryOld);
#endif

	if	(iResult == ERROR_SUCCESS)
		reg.SetValue(g_iVersion,"Version");	

	return;
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
			if	(g_bIsYahoo == FALSE	&&	g_bIsParan	==	FALSE)
			{
				SetCurrentDirectory(g_strInstallFolder);

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

	g_sprPannel.Put(176,105,eCI_complete);
	g_sprPannel.Put(480,185,eCI_complete_icon);

	s_menuButton.draw();

	cTEXT::PushShadow(FALSE);

	cTEXT::FitPut(205,174,450,400,RGB(89,54,0),dMSG_PATCH_COMPLETE);
	cTEXT::FitPut(205,240,450,400,RGB(89,54,0),dMSG_PRESSS_COMPLETE_BUTTON);

	{
		char	strInstallFolerText[1024];

		sprintf(strInstallFolerText,dMSG_EXTRACT_FOLDER,g_strInstallFolder);
		cTEXT::FitPut(205,398,450,400,RGB(89,54,0),strInstallFolerText);
	}

	cTEXT::PopShadow();
}