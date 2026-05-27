#include "cWINDOW.H"


/********************     Sonaki Foundation Class	************************************

	class cWND

****************************************************************************************/
cWND::~cWND()
{	Close();
}

BOOL
cWND::Init(DWORD dwExStyle,LPCTSTR lpClassName,LPCTSTR lpWindowName,DWORD dwStyle,
				int x,int y,int nWidth,int nHeight,
				HWND hWndParent,HMENU hMenu,HINSTANCE hInstance,
				LPVOID lpParam,int show,WNDPROC WndProc
				)
{	WNDCLASSEX wc;   // 윈도우 클래스

	lpszRegister	=	lpClassName;

	hMASTER	=	hWndParent;
	hINST	=	hInstance;
	wStyle	=	dwStyle;

   // Register the main application window class.
   //............................................
	wc.style		 = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;	//
	wc.lpfnWndProc   = (WNDPROC)WndProc;					// 윈도우 프로시져 지정..
	wc.cbClsExtra    = 0;									// 여분의 클래스와..
	wc.cbWndExtra    = 0;									// 여분의 윈도우..
	wc.hInstance     = hINST;								// 인스턴스 핸들..
	wc.hIcon		 = LoadIcon(hINST,lpClassName);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);         // 커서.. 
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);				// 클라이언트 영역의 바탕색깔..
	wc.lpszMenuName  = lpClassName;							// 메뉴 이름.. 리소스에서..
	wc.lpszClassName = lpClassName;							// 클래스 이름..
	wc.cbSize        = sizeof( WNDCLASSEX );                //
	wc.hIconSm       = (HICON)LoadImage( hINST, lpClassName,IMAGE_ICON, 16, 16,LR_COLOR);

														// 아이콘..
	if ( !RegisterClassEx( &wc ) )
	{	ERRMSG("Error in cWND::Init","class 등록 실패");
		return( FALSE );        // 윈도우 등록..
	}

	hWND	=	CreateWindowEx(	dwExStyle,lpClassName,lpWindowName,
								dwStyle,
								x,y,nWidth,nHeight,
								hWndParent,hMenu,hInstance,
								lpParam
								);
	
	if (!hWND)
	{	ERRMSG("Error in cWND::Init","window 생성 실패");
		return FALSE;
	}

	ShowWindow( hWND,show);
	UpdateWindow( hWND);

	client.Set(hWND);
	outFrame.OutFrame(hWND);

	return TRUE;
}

void
cWND::Close()
{	if (!hWND) return;
	DestroyWindow(hWND);
	if (lpszRegister) UnregisterClass((LPTSTR)lpszRegister,hINST);
	hWND=NULL;
	lpszRegister=NULL;
}
int
cWND::thickLeft()
{	RECT tmp;
	SetRect(&tmp,100,100,200,200);
	AdjustWindowRect(&tmp,wStyle,TRUE);

	return 100-tmp.left;
}

int
cWND::thickRight()
{	RECT tmp;
	SetRect(&tmp,100,100,200,200);
	AdjustWindowRect(&tmp,wStyle,TRUE);

	return tmp.right-200;
}

int
cWND::thickTop()
{	RECT tmp;
	SetRect(&tmp,100,100,200,200);
	AdjustWindowRect(&tmp,wStyle,TRUE);

	return 100-tmp.top;
}

int
cWND::thickBottom()
{	RECT tmp;
	SetRect(&tmp,100,100,200,200);
	AdjustWindowRect(&tmp,wStyle,TRUE);

	return 200-tmp.right;
}

BOOL
cWND::Resize(cRECT *rect,BOOL draw)
{	if (!hWND) return FALSE;
	MoveWindow(hWND,rect->x1,rect->y1,rect->x2,rect->y2,draw);

	client.Set(hWND);
	outFrame.OutFrame(hWND);

	return TRUE;
}

BOOL
cWND::Resize(int x1,int y1,int x2,int y2,BOOL draw)
{	if (!hWND) return FALSE;
	MoveWindow(hWND,x1,y1,x2,y2,draw);

	client.Set(hWND);
	outFrame.OutFrame(hWND);

	return TRUE;
}

BOOL
cWND::Resize(int xs,int ys,BOOL draw)
{	if (!hWND) return FALSE;

	client.Set(hWND);

	MoveWindow(hWND,client.x1,client.y1,xs,ys,draw);

	client.Set(hWND);
	outFrame.OutFrame(hWND);

	return TRUE;
}

void
cWND::setParent(HWND hwnd)
{	hMASTER=hwnd;
	SetParent(hWND,hwnd);
}

void
cWND::ENABLE()
{	EnableWindow(hWND,TRUE);
}

void
cWND::DISABLE()
{	EnableWindow(hWND,FALSE);
}

void
cWND::setFont(HGDIOBJ obj)
{	if(!hWND) return;
	SendMessage(hWND,WM_SETFONT,(WPARAM)obj, 0 );
}

void
cWND::setFont()
{	if(!hWND) return;
	SendMessage(hWND,WM_SETFONT,(WPARAM)GetStockObject( DEFAULT_GUI_FONT ), 0 );
}

void
cWND::SetProc(WNDPROC WndProc)
{	SetWindowLong(hWND,GWL_WNDPROC,(LONG)WndProc);
}

/********************     Sonaki Foundation Class	************************************

	class cDIBWND

****************************************************************************************/

cDIBWND::cDIBWND() : cWND() , cDIB()
{
}

cDIBWND::~cDIBWND()
{	Close();
}

BOOL
cDIBWND::Init(DWORD dwExStyle,LPCTSTR lpClassName,LPCTSTR lpWindowName,DWORD dwStyle,
				int x,int y,int nWidth,int nHeight,
				HWND hWndParent,HMENU hMenu,HINSTANCE hInstance,
				LPVOID lpParam,int show,WNDPROC WndProc
				)
{	Close();
	if (!cWND::Init(dwExStyle,lpClassName,lpWindowName,dwStyle,
					x,y,nWidth,nHeight,
					hWndParent,hMenu,hInstance,
					lpParam,show,WndProc
					)
		)
		return FALSE;

	return cDIB::Init(hWND);
}

BOOL
cDIBWND::Init(HWND hwnd,WNDPROC WndProc)
{	if (!IsWindow(hwnd)) return FALSE;

	hWND	=	hwnd;
	SetProc((WNDPROC)WndProc);

	return cDIB::Init(hWND);
}

BOOL
cDIBWND::Resize(cRECT *rect,BOOL repaint)
{	if (!cWND::hWND) return FALSE;
	cWND::Resize(rect,repaint);
	if (cDIB::hBM) DeleteObject(cDIB::hBM);
	return cDIB::Init(hWND);
}

BOOL
cDIBWND::Resize(int xs,int ys,BOOL repaint)
{	if (!IsWindow(hWND)) return FALSE;

	client.Set(hWND);
	outFrame.OutFrame(hWND);

	outFrame.x2	+=	xs	-	client.x2;
	outFrame.y2	+=	ys	-	client.y2;

	cWND::Resize(&outFrame,repaint);

	return cDIB::Init(hWND);
}

BOOL
cDIBWND::Active()
{	if (!IsWindow(hWND)) return FALSE;
	if (!cDIB::Active()) return FALSE;

	return TRUE;
}

void
cDIBWND::Close()
{	cDIB::Close();
	cWND::Close();
}