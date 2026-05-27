#include	"uninstallData.h"
#include	"message.h"
#include	"cTEXT.h"
#include	"stdafx.h"

cUninstallData	g_uninstallData;

enum
{
	eUI_progress_total	=	15,
	eUI_progress_current=	16,
	eUI_progress_back	=	17,

	eUI_cancel_normal	=	4,
	eUI_cancel_active	=	5,
	eUI_cancel_pressed	=	6,

	eUI_current_file_icon=	14,
};

int			l_iFileCount	=	0;
int			l_iCurrentCount	=	0;
char		l_strCurrentFile[256]	=	"";
BOOL		l_bPaused,l_bComplete;

void
cUninstallData::readyToInit()
{
	init();
}

BOOL
cUninstallData::init()
{
	m_iStatus				=	eStatus_Main;
	m_lpInfo				=	NULL;
	l_bPaused				=	FALSE;
	l_bComplete				=	FALSE;

	s_menuButton.reset();

//	s_menuButton.addImageBar(eMenu_HomePage,500,495,eII_next_normal,eII_next_active,eII_next_pressed);
	s_menuButton.addImageBar(eMenu_Cancel,610,495,eUI_cancel_normal,eUI_cancel_active,eUI_cancel_pressed);

	s_menuButton.setClickImageAllBarMargin(1,1);
	
	m_thUninstall.create(g_main.hWND,Remove);

	return	TRUE;
}

void
cUninstallData::readyToClose()
{
	m_iStatus	=	eStatus_Death;
}

BOOL
cUninstallData::readyToUninstall()
{
	if	(!IsFolder(g_strInstallFolder))
		cFOLDER::Create(g_strInstallFolder,TRUE);

	if	(!IsFolder(g_strInstallFolder))
		return	FALSE;

	return	TRUE;
}

BOOL
cUninstallData::run()
{
	if	(l_bComplete)
	{
		cMSG::Put(dMSG_COMPLETE,dMSG_UNINSTALL_SUCCESS);

		return	FALSE;
	}
	
	DWORD	dwSelectMenu	=	s_menuButton.update(MOUSE.x,MOUSE.y,KEY,TOUCHLBUTTON,LBUTTON);

	switch(dwSelectMenu)
	{
		case	eMenu_Cancel	:
			l_bPaused	=	TRUE;

			if	(cMSG::YESNO(dMSG_END_UNINSTALL,dMSG_CANCELED_PROCESS)==IDYES)
			{
				cMSG::Put(_ms(dMSG_UNINSTALL_PROGRAM,dMSG_RED_STONE),dMSG_REMOVE_MANUAL);

				return	FALSE;
			}

			l_bPaused	=	FALSE;
			break;
	}

	return	TRUE;
}

void
cUninstallData::draw()
{
	if	(m_iStatus	!=	eStatus_Main)
		return;

	s_menuButton.draw();

	cTEXT::PushShadow(FALSE);

	g_sprPannel.Put(190,412,eUI_current_file_icon);
	cTEXT::Put(204,410,0,_logFolder);

	g_sprPannel.Put(190,465,eUI_current_file_icon);
	cTEXT::Put(204,463,0,l_strCurrentFile);

	g_sprPannel.Put(191,439,eUI_progress_back);

	int	iRate		=	0;

	if	(l_iFileCount	>	0)
		iRate	=	l_iCurrentCount*100/l_iFileCount;

	g_sprPannel.Put(191,439,eUI_progress_current,iRate);

	cTEXT::PopShadow();	
}

void
cUninstallData::remove()
{
	char		strBuffer[1024];
	char		strRegistry[1024];
	int			iAttr;
	CRegKey		reg;

	int		count,i,size,iLocate;
	cFILE	logFile;

	if	(!logFile.Open(g_strInfoFile,"rb"))
	{
		m_thUninstall.close();
		return;
	}

	logFile.Seek(-4,SEEK_END);

	logFile.Read(&iLocate,4);			//	화일 외의 정보가 있는 위치
	logFile.Seek(iLocate,SEEK_SET);

	logFile.Read(&size,4);				//	레지스트리 위치
	logFile.Read(strRegistry,size);

	SetCurrentDirectory(g_strInstallFolder);

	if	(reg.Open( HKEY_LOCAL_MACHINE,"") == ERROR_SUCCESS)	
		reg.DeleteSubKey(strRegistry);

	if	(reg.Open( HKEY_CURRENT_USER,"") == ERROR_SUCCESS)	
		reg.DeleteSubKey(strRegistry);

	logFile.Read(&count,4);

	for (i=0;i<count;i++)
	{
		while(l_bPaused);

		logFile.Read(&iAttr,4);
		logFile.Read(&size,4);
		logFile.Read(strBuffer,size);

		if	(iAttr	==	0)
			DeleteFile(strBuffer);
		if	(iAttr	==	1)
			cFOLDER::Delete(strBuffer);
		if	(iAttr	==	2)
		{
			CRegKey		reg;

			if (reg.Open( HKEY_LOCAL_MACHINE,"") == ERROR_SUCCESS)
				reg.DeleteSubKey(strBuffer);
			if (reg.Open( HKEY_CURRENT_USER,"") == ERROR_SUCCESS)
				reg.DeleteSubKey(strBuffer);

			reg.Close();
		}
	}

	logFile.Rewind();

	logFile.Read(&count,4);

	l_iFileCount	=	count;
	l_iCurrentCount	=	0;
//	등록된 파일 모두 지우기
	for (i=0;i<count;i++)
	{
		while(l_bPaused);

		logFile.Read(&iAttr,4);
		logFile.Read(&size,4);
		logFile.Read(strBuffer,size);

		if	(iAttr	==	1)
		{
			SetCurrentDirectory(g_strInstallFolder);
			SetCurrentDirectory(strBuffer);
		}
		else
		{
			DeleteFile(strBuffer);
			strcpy(l_strCurrentFile,strBuffer);
			l_iCurrentCount++;
		}
	}

	logFile.Rewind();
	logFile.Read(&count,4);

//	등록된 폴더 모두 지우기
	for (i=0;i<count;i++)
	{
		while(l_bPaused);

		logFile.Read(&iAttr,4);
		logFile.Read(&size,4);
		logFile.Read(strBuffer,size);

		if	(iAttr	==	1)
		{
			SetCurrentDirectory(g_strInstallFolder);

			if	(IsFolder(strBuffer))
			{
				int	fc	=	cFOLDER::GetFileCount("*.*",strBuffer,FALSE,TRUE);

				if	(fc	== 0 )
				{
					cFOLDER::Delete(strBuffer);
					strcpy(l_strCurrentFile,strBuffer);
				}

				l_iCurrentCount++;
			}
		}
	}

	logFile.Close();
	DeleteFile(g_strInfoFile);

	SetCurrentDirectory(g_strInstallFolder);

	iAttr	=	cFOLDER::GetFileCount("*.*",g_strInstallFolder,FALSE,TRUE);

	if	(iAttr	==	0)
	{	
		SetCurrentDirectory("..");

		cFOLDER::Delete(g_strExcuteFolder);
	}

	l_bComplete	=	TRUE;
#ifdef	_FOR_ENG
	cMSG::Put("Uninstall","The uninstall process did not remove all the files. Please delete the remaining files from the Redstone folder. Thank you.");
#endif
	
	m_thUninstall.close();
}

void
cUninstallData::Remove()
{
	g_uninstallData.remove();
}

void
cUninstallData::ProgressExtract(void *_lpInfo)
{
	g_uninstallData.m_lpInfo	=	(cNUX_INFO *)_lpInfo;
}
