#ifndef	_class_install_licence_h
#define	_class_install_licence_h

#include "main.h"
#include "image_scroll_bar.h"

class	cLicense	:	public	cCommonFrame
{
public:
	enum
	{
		eMenu_Pre,
		eMenu_Permit,
		eMenu_Cancel,
	};

	CImageScrollBar	m_scrollBar;

	BOOL	run();
	void	draw();

	BOOL	init(void *_lpPara);

	BOOL	selectInstallFolder();

	void	readyToInit(void *_lpPara);
	void	readyToClose();
};

extern	cLicense	g_license;

#endif