/********************************************************************************

	class CANVAS DIB version	창모드 플밍을 위한것이다.
								범용적이지만 속도가 느리기 때문에 게임은 무리이고
								에디터 제작에 사용한다.

********************************************************************************/
#ifndef _classDIB_H_
#define _classDIB_H_

#include "SFC.H"

class cDIB
{
public:
	HWND		hDIBWND;
	HDC			hDC;
	WORD		*p2SCREEN;
	HBITMAP		hBM;
	HBITMAP		DefaultBitmap;
	BOOL		isACTIVE;			//	활성화 상태
	HFONT		hFONT;				//	폰트

	int			WIDTH,HEIGHT;

				cDIB();
				cDIB(HWND hwnd);
				~cDIB();

	BOOL		Init(HWND hwnd);
	BOOL		Flip();
	BOOL		Flip(int x,int y,int xs,int ys);
	BOOL		Active(BOOL active=TRUE);
	void		Close();
};

#endif