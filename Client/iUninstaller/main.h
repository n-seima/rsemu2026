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
	BOOL			init(HINSTANCE _hInst);	//	√±≠
	BOOL			run();

	char			m_strExcuteFileName[512];
	cCommonFrame	*m_lpActiveFrame;
	cCommonFrame	*m_lpNextFrame;

	void			changeFrame(cCommonFrame *_lpNextFrame);
	void			operateChangeFrameWork();

	void			draw();
	BOOL			readRegistry();

	BOOL			exit();

	static	LRESULT CALLBACK	WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

};

extern	cMain	g_main;
extern	BOOL	g_bActive;
extern	cSPRITE	g_sprPannel;
extern	char	*g_strLogFile;
// 
// extern	LPCTSTR			lpszRegistry;
// extern	LPCTSTR			lpszTestRegistry;
// extern	LPCTSTR			lpszBalanceRegistry;
// 
// extern	LPCTSTR			lpszUninstallRegistry;
// extern	LPCTSTR			lpszUninstallTestRegistry;
// extern	LPCTSTR			lpszUninstallBalanceRegistry;
extern	int				g_iVersion;
extern	BOOL			g_bIsYahoo;
extern	BOOL			g_bIsParan;

extern	char			g_strInfoFile[1024];
extern	char			g_strRegistry[1024];
extern	char			g_strInstallFolder[1024];
extern	char			g_strExcuteFileName[1024];
extern	char			g_strExcuteFolder[1024];
extern	char			g_strTitleName[1024];

#endif