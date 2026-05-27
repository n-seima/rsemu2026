#ifndef _classIMGWND_H
#define _classIMGWND_H

#include <windows.h>

#include "SFC.H"
#include "cDIB.H"
#include "cWINDOW.H"

class cIMGWND	: public cDIBWND
{
public:
	HMENU					hMENU;

	BOOL					bActive,bMoveImg;
	cPOINT					Mouse;
	int						ImgPerLine,ImgPerCol,minRow,minCol,YPos,Rate,imgXS,imgYS,imgXP,imgYP,moveImg;

							cIMGWND();
							~cIMGWND();

	BOOL					Init(HINSTANCE hInst,HWND hWnd,cRECT *rect);

	void					CloseWND();

	void					Draw();
	void					PopupMenu();

	void					Activate();
	BOOL					Run();
	void					GetMousePos();
	void					VScroll(WORD wScroll );

	BOOL					IsActive() {return bActive;}
	int						GetImage(BOOL mar=FALSE);
	void					Delete();
	void					DeleteShadow();

	void					Copy();
	void					Paste();
	void					Cut();

	void					ViewCurrent();
	void					ViewEnd();

	static LRESULT CALLBACK WNDProc(HWND, UINT, WPARAM, LPARAM);
};

extern	cIMGWND	_IMGWND;
extern	int		CurrentBlock;

#endif