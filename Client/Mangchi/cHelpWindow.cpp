
// CHelpWindow.cpp: implementation of the CHelpWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "CHelpWindow.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LPCTSTR		CHelpWindow::m_lpszClass;	// Window Name
HWND		CHelpWindow::m_hWndMain;		// Main Handle
int			CHelpWindow::m_iVscrollPos;
HWND		hParentWnd;	// √÷ª¿ß Handle

char *strHelpString[] = { "F1", "Title", " ",
					   "F2", "Static Object", " ",
					   "F3", "Free Object", " ",
					   "F4", "House", " ",
					   "F5", "Area", " ",
					   "F6", "Hero", " ",
					   "F7", "Road", " ",
					   "F12", "Help", "",
						"", "", "",

						"R", "Show Building Layer", "",
						"T", "Show object layer", "",
						"Y", "Show unit layer", "",
						"U", "View area layer", "",
						"I", "Show Moon Layer", "",
						"", "", "",

						"F", "View unpassed area", "",
						"G", "Grid display", "",
						"H", "Fix mouse screen scrolling", "",
						"J", "tile index display", "",
						"K", "Do not draw tiles", "",
						"", "", "",

						"ESC", "Cancel", "",
						"C", "Copy the selected Static Object", "It works properly when the scale is 100%.",
						"X", "Cut the selected Static Object", "It works properly when the scale is 100%.",
						"Del", "Delete selected object", "",
						"Wheel button", "Map movement", "Moves even when scrolling is disabled.",
						"Wheel Up", "Map Zoom", "",
						"Wheel Down", "Map Zoom Out", "",
						"Direction keys", "Map movement", "No movement when scrolling is disabled",
						"Alt", "View the actual game screen area", "It is different from the actual screen size.",
						"Alt + M", "Edit Monster", "",
						"Alt + N", "Edit NPC", "",
						"Ctrl+M", "View/Close minimap", "",
						"Ctrl + mouse selection", "Deselect selected object", "",
						"Ctrl + Z", "Undo", "",
						"Ctrl + Shift + Z", "Redo", "",
						"Crtl + S", "Map Save", "",
						"Ctrl + A", "Save all maps", "",
						"Ctrl + S", "Save current map", "",
						"Ctrl + D", "Save Project", "",
						"SHIFT + arrow keys", "Move selected unit object", "",
						"SHIFT + select mouse", "select additional object", "",
						"", "", "",

						"When in tile state", "", "",
						"1", "x1", "",
						"2", "x2", "",
						"3", "x3", "",
						"4", "x4", "",
						"5", "x5", "",
						"E", "Expend", "",
						"Tab", "Next Tile", "",
						"Q", "Previous tile", "",
						"", "", "",

						"When in object state", "", "",
						"Keypad number + Enter", "Select the relevant object", "",

};

int iScrollY = (sizeof(strHelpString)/sizeof(*strHelpString))/3 * 5;




CHelpWindow::CHelpWindow()
{
	m_lpszClass = TEXT("Help Window");
	m_hWndMain	= NULL;
	m_iVscrollPos = 0;
	m_iWindowWidth = 600;
	m_iWindowHeight = 600;
}

CHelpWindow::~CHelpWindow()
{

}


int CHelpWindow::Init(HINSTANCE hInstance, HWND _hWnd)
{
	hParentWnd = _hWnd;
	HWND hWnd;
	WNDCLASS WndClass;

	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon =LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=m_lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW|CS_VREDRAW;
	RegisterClass(&WndClass);
	
	RECT rtRect;
	GetClientRect(GetDesktopWindow(), &rtRect);
	
	hWnd = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, m_lpszClass,"",WS_CAPTION| WS_POPUP | WS_THICKFRAME,
					rtRect.right/2 - m_iWindowWidth/2, rtRect.bottom-m_iWindowHeight, m_iWindowWidth, m_iWindowHeight, _hWnd, (HMENU)NULL, hInstance, NULL);
	

	ShowWindow(hWnd, SW_SHOW);
	
	return true;
}



LRESULT CALLBACK CHelpWindow::WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int ix, iy, iScroll;
	int i;
	HFONT hFont, hOldFont;
	HPEN hPen, hOldPen;
	HBRUSH hBrush, hOldBrush;
	
	switch(iMessage)
	{
	case WM_CREATE:
		m_hWndMain=hWnd;
		iScroll = 0;		
		SetScrollRange(hWnd, SB_VERT, 0, iScrollY, TRUE);
		SetScrollPos(hWnd, SB_VERT, m_iVscrollPos, TRUE);
		return 0;


	case 0x020A:	// mousewheel
		switch(HIWORD(wParam)){
		case 65416: // down
			m_iVscrollPos+=10;
			break;
		case 120:	// up
			m_iVscrollPos-=10;
			break;
		}
		if (m_iVscrollPos < 0)
			m_iVscrollPos = 0;
		else if (m_iVscrollPos > iScrollY)
			m_iVscrollPos = iScrollY;

		SetScrollPos(hWnd, SB_VERT, m_iVscrollPos, TRUE);
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_VSCROLL:
		switch(LOWORD(wParam)) {
		case SB_LINEUP:
			m_iVscrollPos-=10;
			break;
		case SB_LINEDOWN:
			m_iVscrollPos+=10;
			break;
		case SB_PAGEUP:
			m_iVscrollPos-=20;
			break;
		case SB_PAGEDOWN:
			m_iVscrollPos+=20;
			break;
		case SB_THUMBTRACK:
			m_iVscrollPos = HIWORD(wParam);
			break;
		}
		if(m_iVscrollPos < 0)
			m_iVscrollPos = 0;
		else if (m_iVscrollPos > iScrollY)
			m_iVscrollPos = iScrollY;
		SetScrollPos(hWnd, SB_VERT, m_iVscrollPos, TRUE);
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;


	case WM_KEYDOWN:
		switch(wParam){
		case VK_F11:
			DestroyWindow(hWnd);
			break;			
		}
		return 0;



	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		hFont = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, "±º∏≤");
		hOldFont = (HFONT)SelectObject(hdc, hFont);

		hPen = CreatePen(PS_SOLID, 1, RGB(0, 200, 0));
		hOldPen = (HPEN)SelectObject(hdc, hPen);

		hBrush = CreateSolidBrush(RGB(0, 200, 0));
		hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		
		SetBkMode(hdc, RGB(0, 200, 0));
		for(i=0 ; i<(sizeof(strHelpString)/sizeof(*strHelpString))/3 ; i++)
		{
			TextOut(hdc, 10, i*15-m_iVscrollPos , strHelpString[i*3+0], strlen(strHelpString[i*3+0]));
			TextOut(hdc, 150, i*15-m_iVscrollPos , strHelpString[i*3+1], strlen(strHelpString[i*3+1]));
			TextOut(hdc, 350, i*15-m_iVscrollPos , strHelpString[i*3+2], strlen(strHelpString[i*3+2]));
		}

		SelectObject(hdc, hOldFont);
		DeleteObject(hFont);
		SelectObject(hdc, hOldPen);
		DeleteObject(hPen);
		SelectObject(hdc, hOldBrush);
		DeleteObject(hBrush);

		EndPaint(hWnd,&ps);
		return 0;


	case WM_DESTROY:		
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}