
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
HWND		hParentWnd;	// 최상위 Handle

char *strHelpString[] = { "F1", "Title", " ",
					   "F2", "Static Object", " ",
					   "F3", "Free Object", " ",
					   "F4", "House", " ",
					   "F5", "Area", " ",
					   "F6", "Hero", " ",
					   "F7", "Road", " ",
					   "F12", "도움말", "",
					   "", "", "",

					   "R", "건물 레이어 보기", "",
					   "T", "오브젝트 레이어 보기", "",
					   "Y", "유닛 레이어 보기", "",
					   "U", "영역 레이어 보기", "",
					   "I", "문 레이어 보기", "",
					   "", "", "",

					   "F", "못지나가는 영역 보기", "",
					   "G", "격자표시", "",
					   "H", "마우스화면스크롤 고정", "",
					   "J", "타일인덱스표시", "",
					   "K", "타일그리기금지", "",
					   "", "", "",

					   "ESC", "취소", "",
					   "C", "선택한 Static Object 복사", "스케일100%일때 제대로 동작한다.",
					   "X", "선택한 Static Object 잘라내기", "스케일100%일때 제대로 동작한다.",
					   "Del", "선택한 Object 삭제", "",
					   "휠버튼", "맵이동", "스크롤금지상태에서도 이동된다.",
					   "휠 Up", "맵확대", "",
					   "휠 Down", "맵축소", "",
					   "방향키", "맵이동", "스크롤금지상태에선 이동불가",
					   "Alt", "실제게임화면영역보기", "실제화면크기와는 차이가 있다.",
					   "Alt + M", "몬스터 편집", "",
					   "Alt + N", "NPC 편집", "",
					   "Ctrl+M", "미니맵보기/닫기", "",
					   "Ctrl + 마우스 선택", "선택한 오브젝트 선택취소", "",
					   "Ctrl + Z", "Undo", "",
					   "Ctrl + Shift + Z", "Redo", "",
					   "Crtl + S", "Map Save", "",
					   "Ctrl + A", "모든맵저장", "",
					   "Ctrl + S", "현재맵저장", "",
					   "Ctrl + D", "프로젝트저장", "",
					   "SHIFT + 방향키", "선택한 유닛오브젝트 이동", "",
					   "SHIFT + 마우스 선택", "오브젝트 추가선택", "",					        
					   "", "", "",					   
					   					   
					   "타일상태일때", "", "",
					   "1", "x1", "",
					   "2", "x2", "",
					   "3", "x3", "",
					   "4", "x4", "",
					   "5", "x5", "",
					   "E", "Expend", "",
					   "Tab", "다음타일", "",
					   "Q", "이전타일", "",
					   "", "", "",

					   "오브젝트상태일때", "", "",
					   "키패드숫자 + 엔터", "해당 오브젝트 선택", "",

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
		hFont = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, "굴림");
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