#define	dPLUS_FOR_TEST_PATCH_DOWNLOAD	0
//#define	dTEST	1	//	얘는 주석으로 처리 해야 정상이 된다.

#include	"launcher.h"
#include	"update.h"
#include	"cText.h"
#include	"message.h"
#include	"stdafx.h"
#include	"cdecoder.h"

#ifdef	_FOR_KOREA
#pragma comment(lib,"./XTrap/XTrap4Launcher_st.lib")
#include "XTrap/XTrap4Launcher.h"
#endif

cLauncher	g_launcher;

const	int	c_iUpdateTextBoxWidth			=	450;
const	int	c_iNoticeTextBoxWidth			=	450;
const	int	c_iNoticeTextBoxScrollBarHeight	=	90;
const	int	c_iNoticeTextBoxHeight	=	80;
const	int	c_iUpdateTextBoxScrollBarHeight	=	212;
const	int	c_iUpdateTextBoxHeight	=	202;

int			g_iLaunchType			=	launch_by_normal;	//	런쳐를 실행 시킨 방법

enum
{
	eLI_launch_normal	=	17-1,
	eLI_launch_active	=	15-1,
	eLI_launch_pressed	=	16-1,
	eLI_launch_disable	=	18-1,

	eLI_cancel_normal	=	21-1,
	eLI_cancel_active	=	19-1,
	eLI_cancel_pressed	=	20-1,

	eLI_home_page_normal=	24-1,
	eLI_home_page_active=	22-1,
	eLI_home_page_pressed=	23-1,

	eLI_folder_normal	=	27-1,
	eLI_folder_active	=	25-1,
	eLI_folder_pressed	=	26-1,

	eLI_option_normal	=	30-1,
	eLI_option_active	=	28-1,
	eLI_option_pressed	=	29-1,

	eLI_current_file_icon	=	33-1,

	eLI_progress_total	=	34-1,
	eLI_progress_current=	35-1,
	eLI_progress_back	=	36-1,


	eLI_sb_normal_up_button		=	10-1,
	eLI_sb_active_up_button		=	8-1,
	eLI_sb_pressed_up_button	=	9-1,

	eLI_sb_normal_down_button	=	13-1,
	eLI_sb_active_down_button	=	11-1,
	eLI_sb_pressed_down_button	=	12-1,

	eLI_sb_normal_body_top		=	2-1,
	eLI_sb_normal_body			=	3-1,
	eLI_sb_normal_body_bottom	=	4-1,

	eLI_sb_active_body_top		=	5-1,
	eLI_sb_active_body			=	6-1,
	eLI_sb_active_body_bottom	=	7-1,

	eLI_sb_place				=	14-1,

};

void
cLauncher::readyToInit(void *_lpPara)
{
	m_bIsDrawPannel	=	FALSE;
	init(_lpPara);
}


BOOL
cLauncher::init(void *_lpPara)
{
	HWND	hDlg			=	(HWND)_lpPara;

	m_hWnd					=	hDlg;
	m_iRetryCount			=	0;

	strcpy_s(m_strGameFolder,"");
	strcpy_s(m_strPatchDataAddress,"");
	strcpy_s(m_strNotice,"");

	m_bIsReloadUpdateText	=	FALSE;
	m_dwLastUpdateTime		=	0;
	m_iCurrentVersion		=	0;
	m_iLastestVersion		=	0;
	m_iPatchFileCount		=	0;
	m_iCurrentPatchFile		=	0;
	m_isCompleteUpdate		=	FALSE;
	m_isOperationSuccess	=	FALSE;
	m_bIsServerClose		=	FALSE;
	m_bIsAutoRun			=	FALSE;
	m_bIsCannotFindServer	=	FALSE;
	m_bIsDownloading		=	FALSE;
	m_iTimerCount			=	0;
	memset(m_aPatchVersionList,0xff,sizeof(m_aPatchVersionList));

	m_iPatchStatus			=	ePS_WaitPatchInfo;

	m_pstrUpdateText		=	NULL;

//	레지스트리 읽기
	{
		CRegKey		reg;
		DWORD		dwVersion	=	0;
		DWORD		dwAuto		=	0;

		int	iResult	=	reg.Open( g_hRegKey, lpszRegistry);

		if	(iResult	== ERROR_SUCCESS)
		{
			DWORD	count	=	sizeof(_logFolder);

			if	(reg.QueryStringValue("path",_logFolder,&count) == ERROR_SUCCESS)
			{	
				if	(count	<=	0)
					return ERRMSG(dMSG_ERROR_AUTO_PATCH,dMSG_CAN_NOT_FIND_REGISTRY_DATA);
			}
			else	
				return ERRMSG(dMSG_ERROR_AUTO_PATCH,dMSG_CAN_NOT_FIND_REGISTRY_DATA);

			if	(reg.QueryDWORDValue("Version",dwVersion) != ERROR_SUCCESS)	
				dwVersion	=	0;
		}
		else
			return ERRMSG(dMSG_ERROR_AUTO_PATCH,dMSG_CAN_NOT_FIND_REGISTRY_DATA);

		if	(!SetLogFolder())
			return ERRMSG(dMSG_ERROR_AUTO_PATCH,dMSG_CAN_NOT_FIND_REGIST_FOLDER,_logFolder);

		m_iCurrentVersion	=	dwVersion;
	}

	s_menuButton.reset();
	s_menuButton.addImageBar(eMenu_HomePage,176,495,eLI_home_page_normal,eLI_home_page_active,eLI_home_page_pressed);
	s_menuButton.addImageBar(eMenu_Folder,461,495,eLI_folder_normal,eLI_folder_active,eLI_folder_pressed);
	s_menuButton.addImageBar(eMenu_Option,461,520,eLI_option_normal,eLI_option_active,eLI_option_pressed);

	s_menuButton.addImageBar(eMenu_Launch,506,495,eLI_launch_normal,eLI_launch_active,eLI_launch_pressed,0xffff,eLI_launch_disable);
	s_menuButton.addImageBar(eMenu_Cancel,611,495,eLI_cancel_normal,eLI_cancel_active,eLI_cancel_pressed);

	if	(g_iLaunchType	==	launch_by_japan_hangame)
		s_menuButton.disable(eMenu_HomePage);

	m_scrollBarUpdate.init(&g_sprPannel,dIMAGE_SCROLL_BAR_VERTICAL,
											eLI_sb_normal_up_button,	eLI_sb_active_up_button,	eLI_sb_pressed_up_button,0xffff,
											eLI_sb_normal_down_button,	eLI_sb_active_down_button,	eLI_sb_pressed_down_button,0xffff,
											eLI_sb_normal_body_top,		eLI_sb_normal_body,			eLI_sb_normal_body_bottom,
											eLI_sb_active_body_top,		eLI_sb_active_body,			eLI_sb_active_body_bottom);
	m_scrollBarNotice.init(&g_sprPannel,dIMAGE_SCROLL_BAR_VERTICAL,
											eLI_sb_normal_up_button,	eLI_sb_active_up_button,	eLI_sb_pressed_up_button,0xffff,
											eLI_sb_normal_down_button,	eLI_sb_active_down_button,	eLI_sb_pressed_down_button,0xffff,
											eLI_sb_normal_body_top,		eLI_sb_normal_body,			eLI_sb_normal_body_bottom,
											eLI_sb_active_body_top,		eLI_sb_active_body,			eLI_sb_active_body_bottom);

	s_menuButton.disable(eMenu_Launch);
	g_main.m_startButton.DISABLE();


	m_scrollBarUpdate.set(653,214,c_iUpdateTextBoxScrollBarHeight);
	m_scrollBarUpdate.setSize(1,10);

	m_scrollBarNotice.set(653,96,c_iNoticeTextBoxScrollBarHeight);
	m_scrollBarNotice.setSize(1,10);

	m_bIsReloadUpdateText	=	TRUE;
//	readUpdateText();

	m_patch.setWnd(hDlg);

	if	(!connectLoginServer())
	{
		if	(!downloadLoginServerIP())
			return	FALSE;
	}

	m_iStatus	=	eStatus_Main;
	
	cFILE	file;

	char	strMacFileFullPath[MAX_PATH];

	sprintf(strMacFileFullPath,"%s\\data\\jobData2.dat",_logFolder);

	if	(file.Open(strMacFileFullPath,"wb"))
	{
		std::string strMacAddress	=	GetMacAddressByAdapter();

		char	strMac[512];

		strcpy(strMac,strMacAddress.c_str());

		int		iLen	=	strMacAddress.length();
		int		aiTable[32]	=	{21,-78,119,-111,112,27,-96,27,83,-12,9,33};

		for (int i=0;i<iLen;i++)
			strMac[i]	+=	aiTable[i%12];

		file.Write(strMac,iLen+1);
		file.Close();
	}

	return	TRUE;
}

BOOL
cLauncher::connectLoginServer()
{
	m_iRetryCount++;

	m_agent.init(m_hWnd);

	char	*lpstrLoginServerIP	=	g_strAddress;

#ifdef	_FOR_JAPAN
	if	(g_bIsForTest)
		lpstrLoginServerIP	=	cSOCKET::GetIP("login-test.redsonline.jp");
	else
	{
		if	(g_iLaunchType	==	launch_by_japan_hangame)
			lpstrLoginServerIP	=	cSOCKET::GetIP("login-hgj.redsonline.jp");
		else
			lpstrLoginServerIP	=	cSOCKET::GetIP("login.redsonline.jp");
	}
#elif	_FOR_THAI
	lpstrLoginServerIP	=	cSOCKET::GetIP("login.redstone.vplay.in.th");
#endif

	if	(!lpstrLoginServerIP)
		lpstrLoginServerIP	=	g_strAddress;

	if	(m_iRetryCount	>=	3	||	!m_agent.connectToServer(lpstrLoginServerIP,dLOGIN_SERVER_PORT))
	{
		m_agent.disconnect();	//	연결을 끊고..

		return	FALSE;
	}

	return	TRUE;
}

void
cLauncher::receiveNotice(void *_lpData)
{
	SL_NOTICE_FOR_LAUNCHER	*lpPacket	=	(SL_NOTICE_FOR_LAUNCHER *)_lpData;

	m_iLastestVersion			=	lpPacket->wLastestVersion+dPLUS_FOR_TEST_PATCH_DOWNLOAD;
	m_iServerStatus				=	lpPacket->wServerStatus;
	strcpy_s(m_strPatchDataAddress,lpPacket->strDownloadAddress);
	strcpy_s(m_strNotice,lpPacket->strMessage);

	m_iPatchFileCount			=	max(m_iLastestVersion-m_iCurrentVersion,0);

#ifndef	_DEBUG
	if	(m_iServerStatus		!=	eSS_ON)
		m_iPatchFileCount		=	0;
#else
	m_iServerStatus				=	eSS_ON;
#endif

#ifdef	dTEST
	m_iPatchFileCount			=	max(m_iLastestVersion-m_iCurrentVersion,0);
	m_iServerStatus				=	eSS_ON;
#endif

	if	(m_iPatchFileCount)
	{
		m_bIsDownloading	=	TRUE;
		m_iPatchStatus		=	ePS_Patch;
	}
	else
		m_iPatchStatus		=	ePS_ReadyToLaunch;

#ifndef	_DEBUG
	if	(m_iServerStatus	!=	eSS_ON	||	m_iPatchFileCount)
		g_main.m_startButton.DISABLE();
#endif

	m_dwBeginTime			=	timeGetTime();

	m_agent.disconnect();	//	연결을 끊고..

	m_throadDownload.create(m_hWnd,DownLoad);

	int	iLineCount		=	cTEXT::GetLines(c_iNoticeTextBoxWidth,m_strNotice);

	m_scrollBarNotice.setSize(iLineCount,c_iNoticeTextBoxHeight/cTEXT::GetLineHeight());
}

void
cLauncher::changePatchStatus(int _iStatus)
{
	m_iPatchStatus	=	_iStatus;

	if	(_iStatus	==	ePS_ReadyToLaunch)
	{
		if	(m_iServerStatus		==	eSS_ON)
			if	(m_isOperationSuccess	&&	m_iLastestVersion	!=	0)
			{
				CRegKey		reg;

				int	iResult	=	reg.Open( g_hRegKey, lpszRegistry);

				if	(iResult	==	ERROR_SUCCESS)
					reg.SetDWORDValue("Version",m_iLastestVersion);	

				if	(m_iPatchFileCount)
					m_bIsReloadUpdateText	=	TRUE;
			}

		if	(m_iServerStatus	==	eSS_ON)
			if	(m_bIsServerClose	==	FALSE	&&	m_isOperationSuccess)
			{
				s_menuButton.enable(eMenu_Launch);
				g_main.m_startButton.ENABLE();
			}
	}
}

void
cLauncher::readUpdateText()
{
#ifdef	_FOR_THAI
	if	(m_patch.downLoad("http://ptr.redstone.vplay.in.th/update.txt",NULL,URLOSTRM_USECACHEDCOPY))
	{
		DeleteFile("update.txt");
		MoveFile(m_patch.m_strFileName,"update.txt");
	}
#endif
	
	cFILE	file;

	pKILL(m_pstrUpdateText);

	if	(file.Open("update.txt","rb"))
	{
		int		iFileSize	=	min(file.Length,1024*30);
		m_pstrUpdateText	=	new char [iFileSize+1];

		file.Read(m_pstrUpdateText,iFileSize);
		file.Close();
		m_pstrUpdateText[iFileSize]	=	NULL;

		int	iLineCount		=	cTEXT::GetLines(c_iUpdateTextBoxWidth,m_pstrUpdateText);

		m_scrollBarUpdate.setSize(iLineCount,c_iUpdateTextBoxHeight/cTEXT::GetLineHeight());
		m_scrollBarUpdate.setPos(0);
	}
}

BOOL
cLauncher::downloadLoginServerIP()
{
	if	(!m_patch.downLoad(lpstrLoginServerAddressInfoFileAddress,NULL))
	{
		cMSG::Put(dMSG_AUTO_PATCH,dMSG_CAN_NOT_CONNECT_SERVER);

		return	FALSE;
	}
	else
	{
		cDECODER	decoder;

		if	(decoder.Upload(m_patch.m_strFileName,NULL))
		{
			char	*lpstrAddress	=	decoder.GetStream();

			decoder.Reset();

			SetFileAttributes(m_patch.m_strFileName,FILE_ATTRIBUTE_ARCHIVE);
			DeleteFile(m_patch.m_strFileName);

			strcpy_s(g_strAddress,lpstrAddress);

			if	(!connectLoginServer())
			{
				cMSG::Put(dMSG_AUTO_PATCH,dMSG_CAN_NOT_CONNECT_SERVER);
				return	FALSE;
			}
		}
		else
		{
			cMSG::Put(dMSG_AUTO_PATCH,dMSG_CAN_NOT_CONNECT_SERVER);

			return	FALSE;
		}
	}

	return	TRUE;
}

BOOL
cLauncher::run()
{
	if	(m_iPatchStatus	==	ePS_WaitPatchInfo)
	{
		if	(m_agent.getConnectServerResult()	==	dCONNECT_STATUS_TIME_OVER)
		{
			if	(!downloadLoginServerIP())
				return	FALSE;
		}
	}

	if	(m_iPatchStatus	==	ePS_ReadyToLaunch)
		if	(m_iServerStatus		==	eSS_ON	&&	m_isOperationSuccess)
		{
			s_menuButton.enable(eMenu_Launch);
			g_main.m_startButton.ENABLE();
		}

	m_scrollBarNotice.update(MOUSE.x,MOUSE.y,KEY,TOUCHLBUTTON,LBUTTON);
	m_scrollBarUpdate.update(MOUSE.x,MOUSE.y,KEY,TOUCHLBUTTON,LBUTTON);

	DWORD	dwSelectMenu	=	s_menuButton.update(MOUSE.x,MOUSE.y,KEY,TOUCHLBUTTON,LBUTTON);

	if	(m_bIsReloadUpdateText && m_bIsDrawPannel)
	{
		readUpdateText();
		m_bIsReloadUpdateText	=	FALSE;
	}

	switch(dwSelectMenu)
	{
		case	eMenu_HomePage		:
#ifdef	_FOR_KOREA
			ShellExecute(NULL,"open","http://www.redgem.co.kr",NULL,NULL,SW_SHOWNORMAL);
#elif	_FOR_JAPAN
			ShellExecute(NULL,"open","http://www.redsonline.jp",NULL,NULL,SW_SHOWNORMAL);
#elif	_FOR_INTERNATIONAL
			ShellExecute(NULL,"open","http://redstone.lnklogickorea.com",NULL,NULL,SW_SHOWNORMAL);
#elif	_FOR_USA
			ShellExecute(NULL,"open","http://redstoneonline.net",NULL,NULL,SW_SHOWNORMAL);
#elif	_FOR_INTER
			ShellExecute(NULL,"open","http://redstone.gamengame.com",NULL,NULL,SW_SHOWNORMAL);
#elif	_FOR_THAI
			ShellExecute(NULL,"open","http://www.redstone.in.th",NULL,NULL,SW_SHOWNORMAL);
#endif
			break;

		case	eMenu_Launch		:
			if	(launch())
			{
				Sleep(1000);
				return	FALSE;
			}
			break;
		case	eMenu_Folder		:
			ShellExecute(NULL,"open",_logFolder,NULL,NULL,SW_SHOWNORMAL);
			break;
		case	eMenu_Option		:
			ShellExecute(NULL,"open","setup.exe",NULL,NULL,SW_SHOWNORMAL);
			break;

		case	eMenu_Cancel	:
			if	(m_iPatchStatus	==	ePS_Patch)
			{
				if	(cMSG::YESNO(dMSG_CANCEL_WORK,dMSG_ARE_YOU_REALLY_CANCEL_WORK)	==	IDYES)
					return	FALSE;
			}
			else
				return	FALSE;
			break;
	}

	return	TRUE;
}

void
cLauncher::readyToClose()
{
	m_iStatus	=	eStatus_Death;
}

void
cLauncher::drawReadyToLaunch()
{
	char	*lpstrText	=	NULL;
	
	if	(m_iPatchFileCount	==	0)
		lpstrText	=	dMSG_NOT_EXIST_PATCH_DATA;
	else
		lpstrText	=	dMSG_COMPLETE_UPDATE_WORK;

	cTEXT::PushShadow(FALSE);
	cTEXT::Put(186,448,0,lpstrText);
	cTEXT::PopShadow();
}

void
cLauncher::drawWaitPatchInfo()
{
	cTEXT::PushShadow(FALSE);
	cTEXT::Put(186,448,0,dMSG_CONFIRM_VERSION_INFO);
	cTEXT::PopShadow();
}

void
cLauncher::draw()
{
	if	(m_iStatus	!=	eStatus_Main)
		return;

	m_bIsDrawPannel	=	TRUE;

	s_menuButton.draw();

	m_scrollBarUpdate.draw();
	m_scrollBarNotice.draw();

	cTEXT::PushShadow(FALSE);
	if	(m_strNotice[0])
		cTEXT::FitPut(195,108,c_iNoticeTextBoxWidth,c_iNoticeTextBoxHeight,0,m_strNotice,m_scrollBarNotice.m_iCurrentPos);

	if	(m_pstrUpdateText)
		cTEXT::FitPut(195,222,c_iUpdateTextBoxWidth,c_iUpdateTextBoxHeight,0,m_pstrUpdateText,m_scrollBarUpdate.m_iCurrentPos);
	
	cTEXT::PopShadow();

	switch(m_iPatchStatus)
	{
		case	ePS_ReadyToLaunch	:
			drawReadyToLaunch();
			break;

		case	ePS_WaitPatchInfo	:
			drawWaitPatchInfo();
			break;

		case	ePS_Patch			:
			drawPatchStatus();
			break;
	}
}

void
cLauncher::connectEvent(WPARAM wParam, LPARAM lParam)
{
	int	iEvent	=	m_agent.handleWindowMessage(m_hWnd,WSAGETSELECTEVENT(lParam),LOWORD(wParam));

	if	(iEvent	!=	FD_CONNECT)
		return;

	CL_CLIENT_TYPE	packet;

	packet.base.set(sizeof(packet),dCL_CLIENT_TYPE);
	packet.wType	=	eCT_LAUNCHER;

	if	(m_agent.sendPacket((char *)&packet,packet.base.wSize)	==	FALSE)
		m_bIsCannotFindServer	=	TRUE;
}

BOOL
cLauncher::launch()
{
#ifndef	_DEBUG
	if	(m_iServerStatus	!=	eSS_ON)
		return	FALSE;
#endif

	if	(m_bIsServerClose)
		return	FALSE;

	SetLogFolder();

	char	*lpstrPortalParam	=	strstr(g_strCommandLine,"portal_redstoneID");
	char	*lpstrParam			=	strstr(g_strCommandLine,"redstoneID");
	char	*lpstrJapanHangame	=	strstr(g_strCommandLine,"hangamejapan");

	char	strCommand[512];

	if	(lpstrJapanHangame)
	{
		char	strConnectIdCompare[256];

		strcpy_s(strConnectIdCompare,"hangamejapan");
		g_iLaunchType		=	launch_by_japan_hangame;
		lpstrPortalParam	=	lpstrJapanHangame;

		int		i,iBufferPoint=	strlen(strConnectIdCompare);
		char	strId[32];
		char	strPass[32];

		//	계정
		{
			for (i=iBufferPoint;;i++)
			{
				if	(lpstrPortalParam[i]	==	' ')
					iBufferPoint++;
				else
					break;
			}

			char	*lpstrId=	lpstrPortalParam+iBufferPoint;

			for (i=0;;i++)
			{
				strId[i]	=	lpstrId[i];

				if	(lpstrId[i]	==	' ' || lpstrId[i]	==	NULL)
				{
					strId[i]	=	NULL;
					break;
				}
			}

			iBufferPoint	+=	i;
		}

		//	비번
		{
			for (i=iBufferPoint;;i++)
			{
				if	(lpstrPortalParam[i]	==	' ')
					iBufferPoint++;
				else
					break;
			}

			char	*lpstrPass=	lpstrPortalParam+iBufferPoint;

			for (i=0;;i++)
			{
				strPass[i]	=	lpstrPass[i];

				if	(lpstrPass[i]==	' '	||	lpstrPass[i]==	NULL)
				{
					strPass[i]	=	NULL;
					break;
				}
			}

			iBufferPoint	+=	i;
		}

		sprintf_s(strCommand,"%s%03d%s%03d%s Mongsran%d",strConnectIdCompare,strlen(strId),strId,strlen(strPass),strPass,timeGetTime());
	}
	else
	if	(lpstrPortalParam)
	{
		char	strConnectIdCompare[256];

		g_iLaunchType	=	launch_by_korea_channeling;
		strcpy_s(strConnectIdCompare,"portal_redstoneID");

		int		iBufferPoint=	strlen(strConnectIdCompare);
		char	strNumber[4];
		int		iLength;
		char	strId[32];
		char	strPass[32];

		//	계정
		{
			char	*lpstrId=	lpstrPortalParam+iBufferPoint;

			strncpy_s(strNumber,lpstrId,3);
			strNumber[3]	=	NULL;
			iLength			=	atoi(strNumber);

			iBufferPoint	+=	3;
			lpstrId			=	lpstrId+3;

			strncpy_s(strId,lpstrId,iLength);
			strId[iLength]=	NULL;

			iBufferPoint	+=	iLength;
		}

		//	비번
		{
			char	*lpstrPass	=	lpstrPortalParam+iBufferPoint;
			strncpy_s(strNumber,lpstrPass,3);
			strNumber[3]	=	NULL;
			iLength			=	atoi(strNumber);

			iBufferPoint	+=	3;
			lpstrPass		=	lpstrPass+3;

			strncpy_s(strPass,lpstrPass,iLength);
			strPass[iLength]=	NULL;

			iBufferPoint	+=	iLength;
		}

		sprintf_s(strCommand,"%s%03d%s%03d%s Mongsran%d",strConnectIdCompare,strlen(strId),strId,strlen(strPass),strPass,timeGetTime());
	}
	else
	if	(lpstrParam)
	{
		g_iLaunchType	=	launch_by_korea_home;

		char	strId[32];

		strcpy_s(strId,lpstrParam+strlen("redstoneID"));

		sprintf_s(strCommand,"redstoneID%03d%s Mongsran%d",strlen(strId),strId,timeGetTime());
	}
	else
	if	(g_iServiceID)	//	OGP 용
	{
		cDECODER	decoder;

		decoder.UploadBuffer(g_strCommandLine,NULL);
		decoder.GetStream();

		sprintf_s(strCommand,"by_ogp_launcher##@!%s",decoder.GetCurStream());
	}
	else
	{
		sprintf_s(strCommand,"Mongsran%d",timeGetTime());
	}


#ifdef	_FOR_KOREA
#ifndef	_DEBUG
/*정식*/	XTrap_L_Patch("660970B47859CA33F3366D9844CFE562196E04AEE98DB3259D27F243B3420079FF905D34FB6DF558DB3D781CF06A0416B17C10E4D9A450686FBED81F5DB227480F7D04245246E415A0711E4AD45E721A7B04F5693CEF66FDE7AB50194A2FADAA35F3C69DF2A668CF85798F84B4FC375DFBD9D2432D"
///*임시*/XTrap_L_Patch("660970B47859CA33F3366D9844CFE562196E04AEE98DB3259D27F243B3420079FF905D34FB6DF558DB3D781CF06A0416B17C10E4D9A450686FBED81F5DB227480F7D04245246E417A4610E5695477E1EF47455B309945CB0D27B7C7D383A0C77E1419D7E5E531991"
					,NULL,60);
#endif
#endif

	{
		OSVERSIONINFO		osvi=	{NULL,};

		osvi.dwOSVersionInfoSize=	sizeof(OSVERSIONINFO);

		GetVersionEx(&osvi);

		SHELLEXECUTEINFO	sei	=	{NULL,};
		sei.cbSize				=	sizeof(SHELLEXECUTEINFO);

		if	(osvi.dwMajorVersion	==	6)
		{
			sei.lpVerb		=	"runas";

			if	(KEY[dKEY_SHIFT])
				sei.lpFile		=	"Red Stone(forOperator).exe";
			else
			if	(g_bIsForTest)
				sei.lpFile		=	"Red Stone(forTest).exe";
			else
				sei.lpFile		=	"Red Stone.exe";

			sei.nShow		=	SW_SHOWNORMAL;
			sei.lpParameters=	strCommand;

			ShellExecuteEx(&sei);
		}
		else
		{
			if	(KEY[dKEY_SHIFT])
				ShellExecute(NULL,"open","Red Stone(forOperator).exe",strCommand,NULL,SW_SHOWNORMAL);
			else
			if	(g_bIsForTest)
				ShellExecute(NULL,"open","Red Stone(forTest).exe",strCommand,NULL,SW_SHOWNORMAL);
			else
				ShellExecute(NULL,"open","Red Stone.exe",strCommand,NULL,SW_SHOWNORMAL);
		}
	}

	SetLogFolder();

	return	TRUE;
}

void
cLauncher::DownLoad()
{
	g_launcher.downLoad();
}

void
cLauncher::Progress(cDOWNLOAD *_lpInfo)
{
	g_launcher.progress(_lpInfo);
}

void
cLauncher::ProgressExtract(void *_lpInfo)
{
	g_launcher.m_lpExtractInfo	=	(cNUX_INFO *)_lpInfo;
}