#include	"installData.h"
#include	"message.h"
#include	"cTEXT.h"
#include	"complete.h"

cInstallData	g_installData;

enum
{
	eII_title		=	7,
	eII_index		=	21,
	eII_install_text=	10,

	eII_progress_total	=	2,
	eII_progress_current=	3,
	eII_progress_back	=	4,

	eII_cancel_normal	=	17,
	eII_cancel_active	=	18,
	eII_cancel_pressed	=	18,

	eII_current_file_icon	=	41,
};

void
cInstallData::readyToInit()
{
	init();
}

BOOL
cInstallData::init()
{
	m_iStatus				=	eStatus_Main;
	m_lpInfo				=	NULL;

	m_bIsFinishByComplete	=	FALSE;
	m_bIsFinishByCancel		=	FALSE;
	m_bIsRemoveInstalledFile=	FALSE;

	s_menuButton.reset();

//	s_menuButton.addImageBar(eMenu_HomePage,500,495,eII_next_normal,eII_next_active,eII_next_pressed);
	s_menuButton.addImageBar(eMenu_Cancel,610,495,eII_cancel_normal,eII_cancel_active,eII_cancel_pressed);

	s_menuButton.setClickImageAllBarMargin(1,1);
	
	m_thExtract.create(g_main.hWND,Extract);

	{
		int	iX	=	-10000;
		
		if	(g_iTestWindowButtonHeight)
			iX	=	0;
		
		m_homeButton		.Init("Home"	,g_main.hWND,g_main.hINST	,0,eInstall_Menu_HomePage	,iX		,g_iWindowHeight-g_iTestWindowButtonHeight,60,30);
		m_cancelGameButton	.Init("Cancel"	,g_main.hWND,g_main.hINST	,0,eInstall_Menu_Cancel		,iX+70	,g_iWindowHeight-g_iTestWindowButtonHeight,60,30);
	}

	return	TRUE;
}

void
cInstallData::readyToClose()
{
	m_iStatus	=	eStatus_Death;

	m_homeButton.Close();
	m_cancelGameButton.Close();
}

BOOL
cInstallData::readyToInstall()
{
	if	(!IsFolder(g_strInstallFolder))
		cFOLDER::Create(g_strInstallFolder,TRUE);

	if	(!IsFolder(g_strInstallFolder))
	{
		ERRMSG(dMSG_ERROR_INSTALL,dMSG_CAN_NOT_CREATE_FOLDER,g_strInstallFolder);

		return	FALSE;
	}

	return	TRUE;
}

BOOL
cInstallData::run()
{
	if	(m_bIsFinishByComplete)
	{
		g_main.changeFrame(&g_complete);

		return	TRUE;
	}

	if	(m_bIsFinishByCancel)
		return	FALSE;

	DWORD	dwSelectMenu	=	s_menuButton.update(MOUSE.x,MOUSE.y,KEY,TOUCHLBUTTON,LBUTTON);

	switch(dwSelectMenu)
	{
		case	eMenu_Cancel	:
			if	(cNUX::IsPaused())
				break;
			
			cNUX::Pause();
			if	(cMSG::YESNO(dMSG_END_SETUP,dMSG_CANCELED_PROCESS)	==	IDYES)
			{
				cNUX::Cancel();
				return	FALSE;
			}
			cNUX::Resume();
			break;
	}

	return	TRUE;
}

void
cInstallData::draw()
{
	if	(m_iStatus	!=	eStatus_Main)
		return;

	g_sprPannel.Put(176,60,eII_title);
	g_sprPannel.Put(575,85,eII_index);
	g_sprPannel.Put(176,105,eII_install_text);

	s_menuButton.draw();

	if	(!m_lpInfo)
		return;

	g_sprPannel.Put(190,465,eII_current_file_icon);

	cTEXT::PushShadow(FALSE);
	cTEXT::FitPut(204,463,450,400,0,m_lpInfo->m_strCurrentFile);

	{
		char	strInstallFolderText[1024];
		
		sprintf(strInstallFolderText,dMSG_INSTALL_FOLDER_FORM,g_strInstallFolder);
		cTEXT::FitPut(205,240,450,400,RGB(89,54,0),strInstallFolderText);
	}

	if	(m_bIsRemoveInstalledFile)
	{
		if	(timeGetTime()%1000 < 700)
			cTEXT::FitPut(205,300,450,400,RGB(89,54,0),dMSG_REMOVE_INSTALLED_FILES);
	}
	else
	{
		g_sprPannel.Put(190,427,eII_current_file_icon);

		if	(m_lpInfo->m_dwRemainTime	!=	0xffffffff)
			cTEXT::Put(204,425,RGB(89,54,0),_ms(dMSG_REMAIN_TIME_FORM,m_lpInfo->m_dwRemainTime/60,m_lpInfo->m_dwRemainTime%60));
		else
			cTEXT::Put(204,425,RGB(89,54,0),dMSG_CALC_REMAIN_TIME);
	}

	cTEXT::PopShadow();

	g_sprPannel.Put(191,439,eII_progress_back);
	g_sprPannel.Put(191,450,eII_progress_back);

	g_sprPannel.Put(191,439,eII_progress_total,m_lpInfo->m_iProgressRate/10);
	g_sprPannel.Put(191,450,eII_progress_current,m_lpInfo->m_iProgressFileRate/10);
}

void
cInstallData::Extract()
{
	cSTRINGS	*files	=	new cSTRINGS();	//	설치된 파일들 리스트

	cNUX::SetExtractFunc(ProgressExtract);	//	펑션 포인터

	SetCurrentDirectory(g_strInstallFolder);	//	인스톨될 폴더

	int	iFileCount	=	cFOLDER::GetFileCount("*.*",g_strInstallFolder,TRUE);	//	기존 폴더에 있는 파일 갯수 확인

	SetLogFolder();

//	if	(cNUX::MergeExtract("data.nux",g_strInstallFolder,g_strLogFile,files))
	if	(cNUX::MergeExtract(g_strExcuteFileName,g_strInstallFolder,g_strLogFile,files))	//	대상 파일 이름,설치될 폴더,로그가 남는 파일,설치된 파일리스트를 얻어올 포인터
	{
#ifdef	_FOR_JAPAN_HANGAME

		char	strTargetFile[512];

		sprintf(strTargetFile,"%s/data/japanHanGame.dat",g_strInstallFolder);
		
		cFILE	file;

		file.Open(strTargetFile,"wb");
		file.Write(&file,4);
		file.Close();
#endif

		Sleep(500);
		g_installData.m_bIsFinishByComplete	=	TRUE;
		
		g_installData.m_thExtract.close();

		return;
	};

	SetLogFolder();

	g_installData.m_bIsRemoveInstalledFile	=	TRUE;

	for (int i=0;i<files->Count;i++)
	{
		char	*lpstrDeleteFile	=	files->GetStr(i);

		if	(!DeleteFile(lpstrDeleteFile))
			cFOLDER::Delete(lpstrDeleteFile);
	}

	if	(iFileCount == 0)
		cFOLDER::Delete(g_strInstallFolder);

	KILL(files);

	g_installData.m_bIsFinishByCancel	=	TRUE;
	
	g_installData.m_thExtract.close();
}

void
cInstallData::ProgressExtract(void *_lpInfo)
{
	g_installData.m_lpInfo	=	(cNUX_INFO *)_lpInfo;
}
