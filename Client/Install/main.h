#ifndef	_class_install_main_h
#define	_class_install_main_h

#include <winsock2.h>
#include "sfc.h"
#include "cWindow.h"
#include "cIMAGE.h"
#include "bar_menu.h"

class	cCommonFrame
{
public:
	enum
	{
		eStatus_Init,
		eStatus_Main,
		eStatus_Close,
		eStatus_Death,
	};

	int					m_iStatus;
	static	cBarMenu	s_menuButton;

	BOOL				isDeath()
	{
		if	(m_iStatus	==	eStatus_Death)
			return	TRUE;

		return	FALSE;
	}

	virtual	void	readyToInit(){};
	virtual	void	readyToClose(){};

	virtual	BOOL	run()	{return	TRUE;};

	virtual	void	draw()	{};
	virtual	BOOL	init()	{return	TRUE;};
	
};

class	cMain : public cDIBWND
{
public:
	BOOL			m_bIsCancel;

	BOOL			init(HINSTANCE _hInst);	//	√±≠
	BOOL			run();
	cCommonFrame	*m_lpActiveFrame;
	cCommonFrame	*m_lpNextFrame;
	void			changeFrame(cCommonFrame *_lpNextFrame);
	void			operateChangeFrameWork();

	void			draw();
	void			cancel()
	{
		m_bIsCancel	=	TRUE;
	}

	BOOL			exit();

	static	LRESULT CALLBACK	WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

};

extern	cMain	g_main;
extern	BOOL	g_bActive;
extern	cSPRITE	g_sprPannel;
extern	char	g_strInstallFolder[1024];
extern	char	g_strExcuteFileName[1024];

extern	char	*g_strLogFile;

extern	LPCTSTR			lpszRegistry;
extern	LPCTSTR			lpszTestRegistry;
extern	LPCTSTR			lpszBalanceRegistry;

extern	LPCTSTR			lpszUninstallRegistry;
extern	LPCTSTR			lpszUninstallTestRegistry;
extern	LPCTSTR			lpszUninstallBalanceRegistry;
extern	int				g_iVersion;
extern	BOOL			g_bIsChannelingService;

extern	char			g_strPortalRedstoneAddress[256];
extern	BOOL			g_bIsTestClient;
extern	int				g_iTestWindowButtonHeight;
extern	int				g_iWindowHeight;
extern	int				g_iPortalType;

enum
{
	ePortal_RedStone,
	ePortal_PlayNet,
};


enum
{
	eWelcome_Menu_Next,
	eWelcome_Menu_Cancel,

	eLicence_Menu_Pre,
	eLicence_Menu_Permit,
	eLicence_Menu_Cancel,

	eInstall_Menu_HomePage,
	eInstall_Menu_Cancel,

	eComplete_Menu_Finish,
	eComplete_Menu_LaunchGame,

};

#endif