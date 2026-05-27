#ifndef _classMAIN_H
#define _classMAIN_H

#include "SFC.H"
#include "cWINDOW.H"
#include "resource.h"

#define	dIDM_LISTBOX	1000
#define	dIDM_REMOVE		2000
#define	dIDM_SAVE_IMAGE	2001

#define	dWORK_NOTHING	0
#define	dWORK_PRESS		1
#define	dWORK_EXTRACT	2

class cMAIN
{
public:
	static	cLISTVIEW			*hLIST;
	static	cSTRINGS			*pSTRINGS;
	static	cWND				*pWND;
	static	cDIBWND				*pDIBWND;
	static	HWND				hWND,hPROGRESS;
	static	HINSTANCE			hINST;
	static	int					WIDTH,HEIGHT;
	static	DWORD				TotalSize;
	static	char				ArcFile[512];
	static	char				ArcFolder[512];
	static	char				SmiFolder[512];
	
	static	char				ExtractFolder[512];
	static	int					Work;


	static	BOOL				Init(HINSTANCE hInst);
	static	BOOL				Init();
	static	BOOL				Exit();
	static	BOOL				Run();
	static	void				New();
	static	void				Resize();
	static	BOOL				Add(char *name);
	static	BOOL				Remove(int index);
	static	void				Remove();

	static	BOOL				Press(BOOL append=FALSE,BOOL merge=FALSE);
	static	BOOL				Merge(BOOL append=FALSE);
	static	BOOL				Extract();
	static	BOOL				SaveAsImage();

	static	void				ProcessPress();
	static	void				ProcessExtract();

	static	void				PopupMenu();
	static	void				GetDropFiles(HDROP handle);
	static	LRESULT CALLBACK	WNDProc(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK	PRESSProc(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK	EXTRACTProc(HWND,UINT,WPARAM,LPARAM);
	static	LRESULT CALLBACK	DRAWProc(HWND,UINT,WPARAM,LPARAM);

};

extern	BOOL	bRUNAPPLICATION,bACTIVEAPP;

#endif