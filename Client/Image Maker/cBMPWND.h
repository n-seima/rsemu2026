#ifndef _classBMPWND_H
#define	_classBMPWND_H

#include <windows.h>
#include "cWINDOW.H"
#include "SFC.H"
#include "cIMAGE.H"

#define	dMODE_NORMAL		0
#define	dMODE_RESIZEBOX		1
#define	dMODE_WAITBUTTON	2
#define	dMODE_MOVE			3
#define	dMAX_GET_IMAGE		3096

class cBMPWND	: public cDIBWND
{
public:
	HMENU					hMENU;
	BOOL					bACTIVE,bQUICKOPEN,bMOVE,bSPOID;
	RECT					ImageRect;
	char					BMPName[256*256-1];
	RECT					Images[dMAX_GET_IMAGE];
	cPOINT					Mouse;
	cIMAGE					*pBMP;
	int						rotateCOLOR,ImageCount,XPos,YPos;
	WORD					transColor,workMode;
	cBUTTON					buttonOpen,buttonClose;
	HWND					hPROGRESS,hSTATUS,hINFO;
	int						curExt;


public:
							cBMPWND();
							~cBMPWND();

	BOOL					loadBmp(char *fname);

	BOOL					OPEN();
	void					QUICKOPEN();
	void					OPENALL();
	BOOL					OPENSELECT();

	void					Activate();
	BOOL					InitWindow();

	BOOL					Run();
	static LRESULT CALLBACK WNDProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	static UINT APIENTRY	hookOpen(HWND hdlg,UINT msg,WPARAM wParam,LPARAM lParam);

};

extern	cBMPWND		_BMPWND;

extern	BOOL		bGETSHADOW;
extern	BOOL		g_isGetImageAndShadow;

#endif