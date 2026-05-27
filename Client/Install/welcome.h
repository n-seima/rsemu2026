#ifndef	_class_install_welcome_h
#define	_class_install_welcome_h

#include "main.h"
#include "bar_menu.h"

class	cWelcome	:	public	cCommonFrame
{
public:
	cBUTTON		m_nextButton,m_cancelButton;

	enum
	{
		eMenu_Next,
		eMenu_Cancel,
	};
	BOOL		run();
	BOOL		init();

	void		draw();

	void		readyToInit();
	void		readyToClose();
};

extern	cWelcome	g_welcome;

#endif