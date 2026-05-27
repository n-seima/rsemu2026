#ifndef _classIMGWND_H
#define _classIMGWND_H

#include <windows.h>

#include "SFC.H"
#include "cDIB.H"
#include "cWINDOW.H"
#include "cANM.H"

class cIMGWND	: public cDIBWND
{
public:
	HMENU					hMENU;
	cIMG					*m_pClipImage;
	int						m_iClipImageCount,m_iPopPos;

	BOOL					bActive,bMoveImg;
	cPOINT					Mouse;
	int						ImgPerLine,ImgPerCol,minRow,minCol,YPos,Rate,imgXS,imgYS,imgXP,imgYP,moveImg;
	int						ImgCount,FirstImg,LastImg;

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

	void					DeleteShadow(BOOL all=FALSE);

	void					Delete(BOOL quick=FALSE);
	void					Copy();
	void					Paste();
	void					Cut();

	void					ViewCurrent();
	void					ViewEnd();

	static LRESULT CALLBACK WNDProc(HWND, UINT, WPARAM, LPARAM);
};

extern	cIMGWND	_IMGWND;

#endif