#include "cWND.H"


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
	wc.style		 = CS_DBLCLKS;	//
	wc.lpfnWndProc   = (WNDPROC)WndProc;					// 윈도우 프로시져 지정..
	wc.cbClsExtra    = 0;									// 여분의 클래스와..
	wc.cbWndExtra    = 0;									// 여분의 윈도우..
	wc.hInstance     = hINST;								// 인스턴스 핸들..
	wc.hIcon		 = LoadIcon(hINST,lpClassName);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);         // 커서.. 
	wc.hbrBackground = (HBRUSH)(GetStockObject( BLACK_BRUSH ));	// 클라이언트 영역의 바탕색깔..
	wc.lpszMenuName  = lpClassName;							// 메뉴 이름.. 리소스에서..
	wc.lpszClassName = lpClassName;							// 클래스 이름..
	wc.cbSize        = sizeof( WNDCLASSEX );                //
	wc.hIconSm       = (HICON)LoadImage( hINST, lpClassName,IMAGE_ICON, 16, 16,LR_DEFAULTCOLOR );
														// 아이콘..
	if ( !RegisterClassEx( &wc ) )
	{
		cMSG::Put("Error in cWND::Init","Regist class failed");
		return( FALSE );        // 윈도우 등록..
	}

	hWND	=	CreateWindowEx(	dwExStyle,lpClassName,lpWindowName,
								dwStyle,
								x,y,nWidth,nHeight,
								hWndParent,hMenu,hInstance,
								lpParam
								);

	if (!hWND)	
	{	ERRMSG("asd","%d",GetLastError());
		return	cMSG::Error("Error in cWND::Init","Create window failed");
	}


	RECT tmp;
	SetRect(&tmp,100,100,200,200);

	AdjustWindowRectEx( &tmp,dwStyle,FALSE,dwExStyle);

	nWidth	+=	(100-tmp.left)*2;
	nHeight	+=	(100-tmp.top)+(tmp.bottom-200);

	MoveWindow(hWND,x,y,nWidth,nHeight,TRUE);


	ShowWindow( hWND,show);
	UpdateWindow( hWND);

	return TRUE;
}

void
cWND::SetProc(WNDPROC WndProc)
{	
	SetWindowLong(hWND,GWL_WNDPROC,(LONG)WndProc);
}

void
cWND::SetStyle(DWORD style)
{	
	SetWindowLong(hWND,GWL_STYLE,(LONG)style);
}

void
cWND::Close()
{	
	if (!hWND) 
		return;
	DestroyWindow(hWND);

	if (lpszRegister)
		UnregisterClass((LPTSTR)lpszRegister,hINST);

	hWND		=	NULL;
	lpszRegister=	NULL;
}

BOOL
cWND::Resize(cRECT rect,BOOL draw)
{
	if (!hWND)
		return FALSE;

	MoveWindow(hWND,rect.x1,rect.y1,rect.x2,rect.y2,draw);

	return TRUE;
}
BOOL
cWND::Resize(int x1,int y1,int x2,int y2,BOOL draw)
{	
	if (!hWND) 
		return FALSE;

	MoveWindow(hWND,x1,y1,x2,y2,draw);

	return TRUE;
}

void
cWND::setParent(HWND hwnd)
{	
	hMASTER	=	hwnd;

	SetParent(hWND,hwnd);
}

void
cWND::ENABLE()
{	EnableWindow(hWND,TRUE);
}

void
cWND::DISABLE()
{	
	EnableWindow(hWND,FALSE);
}

void
cWND::setFont(int xs,int ys,int thin,char *charSet)
			  //  12,     6,FW_NORMAL
{	if(!hWND) return;

	static HFONT font=NULL;

	if (!font)	font	=	CreateFont(ys,xs,0,0,thin,0,0,0,HANGUL_CHARSET,OUT_DEFAULT_PRECIS,
							CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FIXED_PITCH,charSet);
	

	SendMessage(hWND,WM_SETFONT,(WPARAM)font, 0 );
}

void
cWND::setFont()
{	if(!hWND) return;

	static	HFONT font	=	NULL;

	if (!font)	font	=	CreateFont(12,6,0,0,FW_NORMAL,0,0,0,HANGUL_CHARSET,OUT_DEFAULT_PRECIS,
							CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FIXED_PITCH,"GulimChe");

	SendMessage(hWND,WM_SETFONT,(WPARAM)font, 0 );
}

void
cWND::View(BOOL show)
{	if (show)	show	=	SW_SHOW;
	else		show	=	SW_HIDE;

	ShowWindow(hWND,show);
}

BOOL
cWND::IsView()
{	return IsWindowVisible(hWND);
}
