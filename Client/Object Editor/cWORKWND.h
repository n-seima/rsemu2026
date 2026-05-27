#ifndef _classWORKWND_H
#define _classWORKWND_H

#include <windows.h>

#include "SFC.H"
#include "cDIB.H"
#include "cWINDOW.H"

class cWORKWND	: public cDIBWND
{
public:
	HMENU					hMENU;

	BOOL					bActive;
	cPOINT					Zero,Mouse,oldMouse,movePoll,oldPos;
	int						Zoom,Alpha;

							cWORKWND();
							~cWORKWND();

	BOOL					Init(HINSTANCE hInst,HWND hWnd,cRECT *rect);
	void					SetBarMenu();
	void					DrawBarMenu();

	void					CloseWND();

	void					Draw(BOOL _bIsDrawActor=FALSE);
	void					PopupMenu();
	void					SelectAnm();

	void					Activate();
	BOOL					Run();
	void					GetMousePos();
	void					PointedCenter();
	void					ResetZero();
	void					ZoomIn();
	void					ZoomOut();

	BOOL					IsActive() {return bActive;}

	static LRESULT CALLBACK WNDProc(HWND, UINT, WPARAM, LPARAM);
};

extern	cWORKWND	_WORKWND;

extern	int	l_iGradeY;
extern	int	l_iGradeX;
extern	int	l_iGradeY2;
extern	int	l_iGradeX2;

#endif