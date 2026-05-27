#include	"welcome.h"
#include	"uninstallData.h"
#include	"cText.h"
#include	"message.h"

cWelcome	g_welcome;

enum
{
	eWI_remove_normal	=	1,
	eWI_remove_active	=	2,
	eWI_remove_pressed	=	3,

	eWI_cancel_normal	=	4,
	eWI_cancel_active	=	5,
	eWI_cancel_pressed	=	6,

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
	s_menuButton.addImageBar(eMenu_Next,530,495,eWI_remove_normal,eWI_remove_active,eWI_remove_pressed);
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
			g_main.changeFrame(&g_uninstallData);
			break;

		case	eMenu_Cancel	:
			return	FALSE;
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

	s_menuButton.draw();

	cTEXT::PushShadow(FALSE);

	cTEXT::FitPut(190,200,450,400,0,_ms(dMSG_UNINSTALL_INFO1,dMSG_RED_STONE));
	cTEXT::FitPut(190,260,450,400,0,dMSG_UNINSTALL_INFO2);

	cTEXT::PopShadow();
}