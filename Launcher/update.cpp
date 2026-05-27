#include	"update.h"
#include	"cText.h"
#include	"message.h"
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

	eLI_sb_normal_up_button		=	56,
	eLI_sb_active_up_button		=	58,
	eLI_sb_pressed_up_button	=	57,

	eLI_sb_normal_down_button	=	59,
	eLI_sb_active_down_button	=	61,
	eLI_sb_pressed_down_button	=	60,

	eLI_sb_normal_body_top		=	50,
	eLI_sb_normal_body			=	51,
	eLI_sb_normal_body_bottom	=	52,

	eLI_sb_active_body_top		=	53,
	eLI_sb_active_body			=	54,
	eLI_sb_active_body_bottom	=	55,

	eLI_sb_place				=	61,

	eLI_read_license			=	11,
};

void
cLicense::readyToInit(void *_lpPara)
{
	init(_lpPara);
}

BOOL
cLicense::init(void *_lpPara)
{
	s_menuButton.reset();
	
	return	TRUE;
}

BOOL
cLicense::selectInstallFolder()
{

	return	TRUE;
}

BOOL
cLicense::run()
{

	return	TRUE;
}

void
cLicense::readyToClose()
{
	m_iStatus	=	eStatus_Death;
}

void
cLicense::draw()
{
}
