#ifndef	_class_install_complete_h
#define	_class_install_complete_h

#include "main.h"

class	cComplete	:	public	cCommonFrame
{
public:
	cBUTTON		m_finishButton,m_launchGameButton;

	enum
	{
		eMenu_Finish,
		eMenu_LaunchGame,
	};

	BOOL	run();
	void	draw();

	BOOL	init();

	void	readyToInit();
	void	readyToClose();

	void	complete();
};

extern	cComplete	g_complete;

#endif