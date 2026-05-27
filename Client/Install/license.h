#ifndef	_class_install_licence_h
#define	_class_install_licence_h

#include "main.h"
#include "image_scroll_bar.h"

class	cLicense	:	public	cCommonFrame
{
public:
	cBUTTON		m_preButton,m_permitGameButton,m_cancelGameButton;

	enum
	{
		eMenu_Pre,
		eMenu_Permit,
		eMenu_Cancel,
	};

	CImageScrollBar	m_scrollBar;

	BOOL	run();
	void	draw();

	BOOL	init();

	BOOL	selectInstallFolder();

	void	readyToInit();
	void	readyToClose();
};

extern	cLicense	g_license;

#endif