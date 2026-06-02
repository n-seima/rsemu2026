/*
*/
#ifndef _classMAIN_H
#define _classMAIN_H

#include "SFC.H"
#include <windows.h>
#include <commctrl.h>

#include "resource.h"
#include "../red stone/CGameSaveOption.h"

enum
{
	eCD_DIRECTDRAW,
	eCD_AGP,
	eCD_DIB,
};

#define	dMODE_WELCOME		1
#define	dMODE_LICENCE		2
#define	dMODE_CDKEY			3
#define	dMODE_INSTALL		4
#define	dMODE_COMPLETE		5

#define	WM_COMPLETE			(WM_USER	+2)
#define	WM_PROCESS_FAILED	(WM_USER	+3)

class cMain
{
public:
	BOOL			m_bIsVirgin;
	int				m_iOutputDevice;
	CGameOption		m_option;
	char			*m_pFileData;
	DWORD			m_dwFileSize;

	char						m_strFolder[1024];
	int							m_iWidth,m_iHeight;

								cMain();
								~cMain();

	BOOL						init(HINSTANCE hInst);
	BOOL						initDlg(HWND hDlg);
	void						complete(HWND _hDlg);

	inline	void	lostVirgin()	{m_bIsVirgin	=	FALSE;}
	inline	void	setVirgin()		{m_bIsVirgin	=	TRUE;}
	inline	BOOL	isVirgin()		{return m_bIsVirgin;}
	inline	BOOL	isLostVirgin()	{return 1-m_bIsVirgin;}

	static	LRESULT CALLBACK	s_wndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	static	LRESULT CALLBACK	s_procDialog(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};


//	기타 전역 변수
extern	cMain					MAIN;						//	MAIN창이다.

#ifdef	_FOR_CHINA
#define	dCONFIG_RESOURCE	"CONFIG_C"
#else
#define	dCONFIG_RESOURCE	"CONFIG"
#endif

#endif
