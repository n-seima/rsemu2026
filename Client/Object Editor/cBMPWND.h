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

	int						loadBmp(char *fname);
	int						loadShadow(char *fname);

	BOOL					OPEN();
	void					QUICKOPEN();
	void					OPENALL();
	BOOL					OPENSELECT();

	void					PopupMenu();

	void					CLOSEWND();
	void					Activate();
	BOOL					InitWindow();
	BOOL					Init(HINSTANCE hInst,HWND hWnd,cRECT *rect);
	void					VScroll(WORD wScroll );
	void					HScroll(WORD wScroll );

	void					Draw();

	BOOL					MakeImageRect(int x1,int y1,int x2,int y2,LPRECT rect);
	void					MakeImage(int x1,int y1,int x2,int y2);
	void					GetImage();
	void					SpoidColor();
	BOOL					Program(int image,int shadow,BOOL includeSub,char *src,char *dest);

	BOOL					Run();
	static LRESULT CALLBACK WNDProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	static UINT APIENTRY	hookOpen(HWND hdlg,UINT msg,WPARAM wParam,LPARAM lParam);

};

extern	cBMPWND _BMPWND;

#define	dLOAD_IMAGE_FILE_FAILED				0
#define	dLOAD_IMAGE_FILE_OK					1
#define	dLOAD_IMAGE_FILE_OPEN_ERROR			2
#define	dLOAD_IMAGE_FILE_NOT_8BIT_IMAGE		3
#define	dLOAD_IMAGE_FILE_NOT_SAME_BPP		4
#define	dLOAD_IMAGE_FILE_NOT_SAME_PALETTE	5

#endif