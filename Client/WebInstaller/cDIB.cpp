#include "cDIB.H"
#include <commctrl.h>
#include "cDRAW.H"
#include "cTEXT.H"

cDIB::cDIB()
{	hDIBWND		=	NULL;
	hBM			=	NULL;
	isACTIVE	=	FALSE;
	WIDTH		=	0;
	HEIGHT		=	0;

	hDC			=	0;
}

cDIB::~cDIB()
{	Close();
}

cDIB::cDIB(HWND hwnd)
{	hBM		=	NULL;
	Init(hwnd);
}

BOOL
cDIB::Init(HWND hwnd)
{	Close();

	if (hBM)
	{	DeleteObject(hBM);
		hBM	=	NULL;
	}

	hDIBWND	=	hwnd;

	if (!hDIBWND) return FALSE;

	BITMAPINFO BMInfo;

	RECT rect;
	GetClientRect(hDIBWND,&rect);

	WIDTH	=	rect.right;
	HEIGHT	=	rect.bottom;

	if (WIDTH%4!=0) WIDTH=WIDTH/4*4+4;

	hDC		=	GetDC(hDIBWND);

	BMInfo.bmiHeader.biSize				= sizeof (BITMAPINFOHEADER);
	BMInfo.bmiHeader.biWidth			= WIDTH;
	BMInfo.bmiHeader.biHeight			= -HEIGHT;
	BMInfo.bmiHeader.biPlanes			= 1;
	BMInfo.bmiHeader.biBitCount			= 16;
	BMInfo.bmiHeader.biCompression		= BI_RGB;
	BMInfo.bmiHeader.biSizeImage		= 0;
	BMInfo.bmiHeader.biXPelsPerMeter	= 0;
	BMInfo.bmiHeader.biYPelsPerMeter	= 0;
	BMInfo.bmiHeader.biClrUsed			= 0;
	BMInfo.bmiHeader.biClrImportant		= 0;

	hBM = CreateDIBSection (hDC , (BITMAPINFO *)&BMInfo, DIB_RGB_COLORS, (VOID **)&p2SCREEN, NULL, 0);

	ReleaseDC(hDIBWND , hDC );

	return TRUE;
}

BOOL
cDIB::Flip()
{	if (!IsWindow(hDIBWND)	) return FALSE;
	if (!isACTIVE			) return FALSE;

	RECT		Dest;
	PAINTSTRUCT ps;

	BeginPaint(hDIBWND, &ps);

	HDC	destDC	= GetDC(hDIBWND);
	GetClientRect(hDIBWND, &Dest);

	BitBlt(destDC, 0, 0, Dest.right - Dest.left, Dest.bottom - Dest.top, hDC, 0, 0, SRCCOPY);

	ReleaseDC(hDIBWND, destDC);
	Active(FALSE);

	EndPaint(hDIBWND, &ps);

	return TRUE;
}

BOOL
cDIB::Flip(int x,int y,int xs,int ys)
{	if (!IsWindow(hDIBWND)	) return FALSE;
	if (!isACTIVE			) return FALSE;

	RECT		Dest;
	PAINTSTRUCT ps;

	BeginPaint(hDIBWND, &ps);

	HDC	destDC	= GetDC(hDIBWND);
	GetClientRect(hDIBWND, &Dest);

	BitBlt(destDC, x, y, xs, ys, hDC, x, y, SRCCOPY);

	ReleaseDC(hDIBWND, destDC);
	Active(FALSE);

	EndPaint(hDIBWND, &ps);

	return TRUE;
}

BOOL
cDIB::Active(BOOL active)
{	if (WIDTH == 0 || HEIGHT == 0) return FALSE;
	if (!hBM			  ) return FALSE;
	if (!IsWindow(hDIBWND)) return FALSE;
	if (isACTIVE == active) return FALSE;

	if (active)
	{	cDRAW::Active(p2SCREEN,WIDTH,HEIGHT);

		hDC				= CreateCompatibleDC(0);
		DefaultBitmap	= (HBITMAP)SelectObject(hDC, hBM);

		isACTIVE		= TRUE;
		cTEXT::Active(hDC);

		return TRUE;
	}

	SelectObject(hDC, DefaultBitmap);
	DeleteObject(DefaultBitmap);
	DeleteDC(hDC);

	hDC	=	0;

	isACTIVE	=	FALSE;

	return TRUE;
}

void
cDIB::Close()
{	Active(FALSE);
	if (hBM)
	{	DeleteObject(hBM);
		hBM	=	NULL;
	}
}