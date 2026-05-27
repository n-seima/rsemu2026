#include	"license.h"
#include	"cText.h"
#include	"message.h"
#include	"installData.h"
#include	"welcome.h"
#include	"cDraw.h"

cLicense	g_license;

const	int	c_iLicenseTextWindowHeight	=	228;

enum
{
	eLI_title		=	6,
	eLI_index		=	20,

	eLI_pre_normal	=	23,
	eLI_pre_active	=	24,
	eLI_pre_pressed=	24,

	eLI_permit_normal	=	25,
	eLI_permit_active	=	26,
	eLI_permit_pressed	=	26,

	eLI_cancel_normal	=	27,
	eLI_cancel_active	=	28,
	eLI_cancel_pressed	=	28,

	eLI_sb_normal_up_button		=	58,
	eLI_sb_active_up_button		=	60,
	eLI_sb_pressed_up_button	=	59,

	eLI_sb_normal_down_button	=	61,
	eLI_sb_active_down_button	=	63,
	eLI_sb_pressed_down_button	=	62,

	eLI_sb_normal_body_top		=	52,
	eLI_sb_normal_body			=	53,
	eLI_sb_normal_body_bottom	=	54,

	eLI_sb_active_body_top		=	55,
	eLI_sb_active_body			=	56,
	eLI_sb_active_body_bottom	=	57,

	eLI_sb_place				=	64,

	eLI_read_license			=	11,
};

void
cLicense::readyToInit()
{
	init();
}

BOOL
cLicense::init()
{
	s_menuButton.reset();
	s_menuButton.addImageBar(eMenu_Pre,446,495,eLI_pre_normal,eLI_pre_active,eLI_pre_pressed);
	s_menuButton.addImageBar(eMenu_Permit,500,495,eLI_permit_normal,eLI_permit_active,eLI_permit_pressed);
	s_menuButton.addImageBar(eMenu_Cancel,610,495,eLI_cancel_normal,eLI_cancel_active,eLI_cancel_pressed);

	s_menuButton.setClickImageAllBarMargin(1,1);

	m_scrollBar.init(&g_sprPannel,dIMAGE_SCROLL_BAR_VERTICAL,
											eLI_sb_normal_up_button,	eLI_sb_active_up_button,	eLI_sb_pressed_up_button,0xffff,
											eLI_sb_normal_down_button,	eLI_sb_active_down_button,	eLI_sb_pressed_down_button,0xffff,
											eLI_sb_normal_body_top,		eLI_sb_normal_body,			eLI_sb_normal_body_bottom,
											eLI_sb_active_body_top,		eLI_sb_active_body,			eLI_sb_active_body_bottom);

	int	iTextLineCount	=	c_iLicenseTextWindowHeight/cTEXT::GetLineHeignt();
	int	iLineCount		=	cTEXT::GetLines(460,g_strLicence);

	m_scrollBar.set(670,195,232);
	m_scrollBar.setSize(iLineCount,iTextLineCount);

	m_iStatus	=	eStatus_Main;

	{
		int	iX	=	-10000;
		
		if	(g_iTestWindowButtonHeight)
			iX	=	0;
		
		m_preButton.Init("Pre"			,g_main.hWND,g_main.hINST	,0,eLicence_Menu_Pre	,iX		,g_iWindowHeight-g_iTestWindowButtonHeight,60,30);
		m_permitGameButton.Init("Permit",g_main.hWND,g_main.hINST	,0,eLicence_Menu_Permit	,iX+70	,g_iWindowHeight-g_iTestWindowButtonHeight,60,30);
		m_cancelGameButton.Init("Cancel",g_main.hWND,g_main.hINST	,0,eLicence_Menu_Cancel	,iX+140	,g_iWindowHeight-g_iTestWindowButtonHeight,60,30);
	}
	
	return	TRUE;
}

BOOL
cLicense::selectInstallFolder()
{
	char	strProgramFolder[512];
	char	strFolderName[64];

	GetProgrameFilesFolder( strProgramFolder,512);

	switch(g_iPortalType)
	{
		case	ePortal_PlayNet	:
			strProgramFolder[3]	=	NULL;
			strcat(strProgramFolder,"PlayNet");
			break;

		default	:
			break;
	}

	strcpy(strFolderName,g_strGameName);

#ifdef	_FOR_JAPAN
	SetCurrentDirectory(strProgramFolder);
	CreateDirectory("GameON",NULL);

	sprintf(strProgramFolder,"%s\\GameON",strProgramFolder);
#endif

#ifdef	_FOR_USA
	SetCurrentDirectory(strProgramFolder);
	CreateDirectory("Ogplanet",NULL);

	sprintf(strProgramFolder,"%s\\Ogplanet",strProgramFolder);
#endif

#ifdef	_FOR_JAPAN_HANGAME
	strcpy(strFolderName,"HGREDSTONE");
#endif

#ifdef	_FOR_WCG
	strcpy(strFolderName,"Red Stone for WCG");
#endif

	char	*strDestFolder;

	strDestFolder	=	cFOLDER::Tree(g_main.hWND,TRUE,FALSE,dMSG_SELECT_INSTALL_FOLDER,strProgramFolder,strFolderName);

	if	(!strDestFolder)
		return FALSE; 

	if	(cMSG::YESNO(dMSG_SETUP_GAME,dMSG_QUEST_INSTALL_FOLLOW_FOLDER,strDestFolder)==IDNO)
		return	FALSE;

	strcpy(g_strInstallFolder,strDestFolder);

	return	TRUE;
}

BOOL
cLicense::run()
{
	DWORD	dwSelectMenu	=	s_menuButton.update(MOUSE.x,MOUSE.y,KEY,TOUCHLBUTTON,LBUTTON);

	switch(dwSelectMenu)
	{
		case	eMenu_Permit	:
			if	(selectInstallFolder())
				g_main.changeFrame(&g_installData);
			break;

		case	eMenu_Pre	:
			g_main.changeFrame(&g_welcome);
			break;

		case	eMenu_Cancel	:
			if	(cMSG::YESNO(dMSG_END_SETUP,dMSG_CANCELED_PROCESS)	==	IDYES)
				return	FALSE;
			break;
	}

	m_scrollBar.update(MOUSE.x,MOUSE.y,KEY,TOUCHLBUTTON,LBUTTON);

	return	TRUE;
}

void
cLicense::readyToClose()
{
	m_iStatus	=	eStatus_Death;

	m_preButton.Close();
	m_permitGameButton.Close();
	m_cancelGameButton.Close();
}

void
cLicense::draw()
{
	if	(m_iStatus	!=	eStatus_Main)
		return;

	g_sprPannel.Put(176,60,eLI_title);
	g_sprPannel.Put(575,85,eLI_index);

	g_sprPannel.Put(176,105,eLI_read_license);

	cDRAW::FillHB(0xffff,190,195,685,427);
	
	s_menuButton.draw();

	cTEXT::PushShadow(FALSE);

	cTEXT::FitPut(192,162,450,400,RGB(89,54,0),dMSG_READ_LICENSE);
	cTEXT::FitPut(192,432,450,400,RGB(89,54,0),dMSG_LICENSE_QUESTION);

	cTEXT::FitPut(200,200,460,228,RGB(89,54,0),g_strLicence,m_scrollBar.m_iCurrentPos);

	cTEXT::PopShadow();

	m_scrollBar.draw();
}
