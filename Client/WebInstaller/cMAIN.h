/*
*/
#ifndef _classMAIN_H
#define _classMAIN_H

#include "SFC.H"
#include <commctrl.h>
#include "cWINDOW.H"
#include "cDOWNLOAD.H"

#include "resource.h"

#define	dNOTICE_SIZE			4096
#define	dADRESS_SIZE			512
#define	dMAX_PATCH_FILE_COUNT	(1024*10)

class cMAIN	: public cWND
{
public:
	HWND				m_hProgressWnd;
	BOOL				m_bIsServerClose;
	char				m_strGameFolder[dADRESS_SIZE];
	int					m_iCurrentVersion,m_iLastestVersion,m_iServerStatus;
	char				m_strInstallVersionDataAddress[512],m_strDownLoadFileName[64],m_strParam[128];
	BOOL				m_isExtract;
	DWORD				m_dwLastUpdateTime;
	BOOL				m_isCompleteUpdate;
	BOOL				m_isOperationSuccess;

	BOOL				m_bIsAutoRun,m_bIsCannotFindServer;
	
						cMAIN();
						~cMAIN();

	BOOL				Init(HINSTANCE hInst);
	void				Draw();

	BOOL				Run();

	void				downLoad();
	void				install();
	BOOL				launchGame();
	void				InitDlgBox(HWND hDlg);
	BOOL				extractFile(char *_lpstrFilename);
	void				progress(cDOWNLOAD *lpInfo);

	static	cDOWNLOAD	s_setup;
	static	cDIBWND		s_dibImageWnd;
	static	cTHREAD		s_threadDownload;

	static	void		Progress(cDOWNLOAD *_lpInfo);
	static	void		DownLoad();

	static	LRESULT CALLBACK	WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	static	LRESULT CALLBACK	procProgress(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	procImageWindow(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	procNotice(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static	LRESULT CALLBACK	s_procEdit(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	static	LRESULT CALLBACK	s_procUpdateNotice(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
};

//	기타 전역 변수
extern	cMAIN				MAIN;						//	MAIN창이다.
extern	char				g_strSetupOption[256];

#endif