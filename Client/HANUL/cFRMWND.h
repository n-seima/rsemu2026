#ifndef _classFRMWND_H
#define _classFRMWND_H

#include <windows.h>

#include "SFC.H"
#include "cDIB.H"
#include "cANM.H"
#include "cWINDOW.H"

class cFRMWND	: public cDIBWND
{
protected:
	int						curFrame;

public:
	HMENU					hMENU;

	BOOL					bActive;
	cPOINT					Mouse;
	int						ImgPerLine,XPos,Rate,imgXS,imgYS,imgXP,imgYP;

							cFRMWND();
							~cFRMWND();

	BOOL					Init(HINSTANCE hInst,HWND hWnd,cRECT *rect);

	void					CloseWND();
	void					HScroll(WORD wScroll);

	void					Draw();
	void					PopupMenu();

	void					Activate();
	BOOL					Run();
	int						GetFrame();
	
	void					CopyAnmData(int what);
	void					CopyFirstDirectPattern();
	void					IncFrameIndex();
	void					DecFrameIndex();
	void					AddFrameIndex();
	void					IncAllIndex();
	void					SyncAnm();
	void					SyncPreDirect();
	void					SyncAllDirect();

	void					ViewCurrent();
	void					ViewEnd();
	void					SetBarMenu();
	void					DrawBarMenu();
	void					UpdateBarMenu();

	void					SyncAddDataFrame();
	void					SaveAddData(HWND _hWnd);
	BOOL					IsActive() {return bActive;}

	static LRESULT CALLBACK WNDProc(HWND, UINT, WPARAM, LPARAM);
};

extern	cFRMWND	_FRMWND;
extern	BYTE	IsSelectedFrame[dMAX_FRAME];

#endif