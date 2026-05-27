#include <windows.h>
#include <commctrl.h>
#include "cDIB.H"
#include "cDRAW.H"

cDIB::cDIB()
{	hDIBWND		=	NULL;
	hBM			=	NULL;
	isACTIVE	=	FALSE;

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
cDIB::Flip(HDC destDC,int shadkeX,int shadkeY)
{	if (!IsWindow(hDIBWND)	) return FALSE;
	if (!isACTIVE			) return FALSE;

	RECT		Dest;
	PAINTSTRUCT ps;

	BeginPaint(hDIBWND, &ps);
	GetClientRect(hDIBWND, &Dest);

	BitBlt(destDC, shadkeX, shadkeY, Dest.right, Dest.bottom, hDC, 0, 0, SRCCOPY);

	Active(FALSE);

	EndPaint(hDIBWND, &ps);

	return TRUE;
}

BOOL
cDIB::Flip(HDC destDC,int x,int y,int xs,int ys)
{	if (!IsWindow(hDIBWND)	) return FALSE;
	if (!isACTIVE			) return FALSE;

	PAINTSTRUCT ps;

	BeginPaint(hDIBWND, &ps);

	BitBlt(destDC,x,y,xs,ys,hDC, 0, 0, SRCCOPY);

	Active(FALSE);

	EndPaint(hDIBWND, &ps);

	return TRUE;
}

BOOL
cDIB::Active(BOOL active)
{	if (!IsWindow(hDIBWND)) return FALSE;

	if (isACTIVE == active) return FALSE;

	if (active)
	{	cDRAW::Active(p2SCREEN,WIDTH,HEIGHT);

		hDC				= CreateCompatibleDC(0);
		DefaultBitmap	= (HBITMAP)SelectObject(hDC, hBM);

		isACTIVE		= TRUE;

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