#include	"welcome.h"
#include	"cText.h"
#include	"message.h"
#include	"installData.h"

cWelcome	g_welcome;

enum
{
	eWI_title		=	5,
	eWI_index		=	19,

	eWI_next_normal	=	12,
	eWI_next_active	=	13,
	eWI_next_pressed=	13,

	eWI_cancel_normal	=	17,
	eWI_cancel_active	=	18,
	eWI_cancel_pressed	=	18,

	eWI_install			=	48,

};

void
cWelcome::readyToInit()
{
	init();
}


BOOL
cWelcome::init()
{
	s_menuButton.reset();
	s_menuButton.addImageBar(eMenu_Next,500,495,eWI_next_normal,eWI_next_active,eWI_next_pressed);
	s_menuButton.addImageBar(eMenu_Cancel,610,495,eWI_cancel_normal,eWI_cancel_active,eWI_cancel_pressed);

	s_menuButton.setClickImageAllBarMargin(1,1);

	m_iStatus	=	eStatus_Main;
	
	return	TRUE;
}

BOOL
cWelcome::run()
{
	DWORD	dwSelectMenu	=	s_menuButton.update(MOUSE.x,MOUSE.y,KEY,TOUCHLBUTTON,LBUTTON);

	switch(dwSelectMenu)
	{
		case	eMenu_Next		:
			g_main.changeFrame(&g_installData);
			break;

		case	eMenu_Cancel	:
			if	(cMSG::YESNO(dMSG_END_SETUP,dMSG_CANCELED_PROCESS)	==	IDYES)
				return	FALSE;
			break;
	}

	return	TRUE;
}

void
cWelcome::readyToClose()
{
	m_iStatus	=	eStatus_Death;
}

void
cWelcome::draw()
{
	if	(m_iStatus	!=	eStatus_Main)
		return;

	g_sprPannel.Put(480,185,eWI_install);
	s_menuButton.draw();

	cTEXT::PushShadow(FALSE);
	cTEXT::SetLineGap(24);

	char	strPatchExplain[512],strGameFolder[512];

	if	(g_bIsTemplatePatch	)
		strcpy(strPatchExplain,"instant patch");
	else
		sprintf(strPatchExplain,dMSG_MANUAL_PATCH_EXPLAIN_FORM,g_iVersion/10000,g_iVersion%1000);

	sprintf(strGameFolder,dMSG_EXTRACT_FOLDER,_logFolder);

	cTEXT::FitPut(216,160,420,400,RGB(89,54,0),strPatchExplain);

	cTEXT::FitPut(216,300,420,400,RGB(89,54,0),dMSG_PLEASE_PRESS_NEXT_BUTTON_TO_CONTINUE_PATCH);

	cTEXT::FitPut(216,420,420,400,RGB(89,54,0),strGameFolder);

	cTEXT::RestoreLineGap();
	cTEXT::PopShadow();
}