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

	virtual	void	readyToInit(void *_lpPara){};
	virtual	void	readyToClose(){};

	virtual	BOOL	run()	{return	TRUE;};

	virtual	void	connectEvent(WPARAM wParam, LPARAM lParam)	{};
	virtual	void	draw()	{};
	virtual	BOOL	init(void *_lpPara)	{return	TRUE;};
	virtual	void	receiveNotice(void *_lpData){};
};

class	cMain : public cDIBWND
{
public:
	cBUTTON			m_startButton;

	BOOL			init(HINSTANCE _hInst);	//	√ ±‚»≠
	BOOL			run();
	cCommonFrame	*m_lpActiveFrame;
	cCommonFrame	*m_lpNextFrame;
	void			changeFrame(cCommonFrame *_lpNextFrame);
	void			operateChangeFrameWork();

	void			draw();
	void			receiveNotice(void *_lpData);

	BOOL			exit();

	static	LRESULT CALLBACK	WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

};

extern	cMain	g_main;
extern	BOOL	g_bActive;
extern	cSPRITE	g_sprPannel;
extern	char	g_strExcuteFileName[1024];

extern	LPCTSTR	lpszRegistry;
extern	LPCTSTR	lpszRegistryOld;
extern	LPCTSTR	lpszTestRegistry;
extern	LPCTSTR	lpszTestRegistryOld;
extern	LPCTSTR	lpszBalanceRegistry;

extern	LPCTSTR	lpszUninstallRegistry;
extern	LPCTSTR	lpszUninstallTestRegistry;
extern	LPCTSTR	lpszUninstallBalanceRegistry;
extern	char	*lpstrLoginServerAddressInfoFileAddress;
extern	char	g_strAddress[40];

extern	char	g_strCommandLine[1024];

extern	int		g_iServiceID;
extern	char	g_strUserID[20];
extern	char	g_strTokken[1024];

extern	BOOL	g_bIsForTest;

#ifdef	_FOR_CHINA
	#define dLOGIN_SERVER_PORT		50661	//	57425
#elif	_FOR_INTER
	#define	dLOGIN_SERVER_PORT		45661
#elif	_FOR_KOREA
	#define	dLOGIN_SERVER_PORT		9049
#elif	_FOR_JAPAN
	#define dLOGIN_SERVER_PORT		55661
#elif	_FOR_USA
	#define	dLOGIN_SERVER_PORT		55661
#elif	_FOR_THAI
	#define	dLOGIN_SERVER_PORT		55661
#endif
#include <string>

extern	std::string		GetMacAddressByAdapter();
extern	HKEY			g_hRegKey;

#endif