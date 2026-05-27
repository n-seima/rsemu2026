#ifndef	_class_install_install_data_h
#define	_class_install_install_data_h

#include "main.h"
#include "cNux.h"

class	cInstallData	:	public	cCommonFrame
{
public:
	cBUTTON		m_homeButton,m_cancelGameButton;

	enum
	{
		eMenu_HomePage,
		eMenu_Cancel,
	};

	int			m_iExistFileCount;
	cSTRINGS	m_installFiles;

	BOOL		m_bIsFinishByComplete,m_bIsFinishByCancel,m_bIsRemoveInstalledFile;
	cNUX_INFO	*m_lpInfo;

	BOOL		run();
	void		draw();

	BOOL		init();

	BOOL		selectInstallFolder();

	void		readyToInit();
	void		readyToClose();

	BOOL		readyToInstall();

	cThread		m_thExtract;

	static	void	Extract();
	static	void	ProgressExtract(void *Info);
};

extern	cInstallData	g_installData;

#endif