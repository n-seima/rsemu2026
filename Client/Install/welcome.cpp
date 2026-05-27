#include	"welcome.h"
#include	"license.h"
#include	"cText.h"
#include	"message.h"

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

	{
		int	iX	=	-10000;
		
		if	(g_iTestWindowButtonHeight)
			iX	=	0;
		
		m_nextButton.Init("Next",g_main.hWND,g_main.hINST		,0,eWelcome_Menu_Next	,iX		,g_iWindowHeight-g_iTestWindowButtonHeight,60,30);
		m_cancelButton.Init("Cancel",g_main.hWND,g_main.hINST	,0,eWelcome_Menu_Cancel	,iX+70	,g_iWindowHeight-g_iTestWindowButtonHeight,60,30);
	}

	return	TRUE;
}

BOOL
cWelcome::run()
{
	DWORD	dwSelectMenu	=	s_menuButton.update(MOUSE.x,MOUSE.y,KEY,TOUCHLBUTTON,LBUTTON);

	switch(dwSelectMenu)
	{
		case	eMenu_Next		:
			g_main.changeFrame(&g_license);
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

	m_nextButton.Close();
	m_cancelButton.Close();
}

void
cWelcome::draw()
{
	if	(m_iStatus	!=	eStatus_Main)
		return;

	g_sprPannel.Put(176,60,eWI_title);
	g_sprPannel.Put(575,85,eWI_index);
	g_sprPannel.Put(480,185,eWI_install);
	s_menuButton.draw();

	cTEXT::PushShadow(FALSE);
	cTEXT::SetLineGap(24);
	cTEXT::FitPut(216,160,450,400,RGB(89,54,0),g_strWelcome);
	cTEXT::RestoreLineGap();
	cTEXT::PopShadow();
}