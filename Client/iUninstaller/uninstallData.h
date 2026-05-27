#ifndef	_class_uninstall_data_h
#define	_class_uninstall_data_h

#include "main.h"
#include "cNux.h"

class	cUninstallData	:	public	cCommonFrame
{
public:
	enum
	{
		eMenu_HomePage,
		eMenu_Cancel,
	};

	int			m_iExistFileCount;
	cSTRINGS	m_installFiles;

	cNUX_INFO	*m_lpInfo;

	BOOL		run();
	void		draw();

	BOOL		init();

	BOOL		selectInstallFolder();

	void		readyToInit();
	void		readyToClose();

	BOOL		readyToUninstall();
	void		remove();

	cThread		m_thUninstall;

	static	void	Remove();
	static	void	ProgressExtract(void *Info);
};

extern	cUninstallData	g_uninstallData;

#endif