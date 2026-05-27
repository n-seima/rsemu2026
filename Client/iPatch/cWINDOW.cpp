#include "cWINDOW.H"
#include <WINDOWSX.H>


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
	wc.hIcon		 = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);         // 커서.. 
	wc.hbrBackground = (HBRUSH)(GetStockObject( BLACK_BRUSH ));	// 클라이언트 영역의 바탕색깔..
	wc.lpszMenuName  = lpClassName;							// 메뉴 이름.. 리소스에서..
	wc.lpszClassName = lpClassName;							// 클래스 이름..
	wc.cbSize        = sizeof( WNDCLASSEX );                //
	wc.hIconSm       = (HICON)LoadImage( hINST, lpClassName,IMAGE_ICON, 16, 16,LR_COLOR);
														// 아이콘..
	if ( !RegisterClassEx( &wc ) )
	{
		ERRMSG("Error in cWND::Init","class 등록 실패");
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

	class cSPLITBAR

****************************************************************************************/
cSPLITBAR::cSPLITBAR() 
		  : cWND()
{	hCursor	=	NULL;
}

cSPLITBAR::~cSPLITBAR()
{	Close();
}

BOOL
cSPLITBAR::Init(HWND hWndParent,HINSTANCE hInstance,int what,LPCTSTR lpClassName,int x,int y,int xs,int ys)
{	Close();

	kind	=	what;

	cWND::Init(	0,lpClassName,lpClassName,WS_POPUP|WS_CHILD,
					x,y,xs,ys,
					hWndParent,NULL,hInstance,NULL,SW_SHOW,
					(WNDPROC)splitProc
				);

	if (!hCursor) hCursor=	LoadCursor(hINST, what==dSPLIT_VERT ? "CURSOR_VERT_SPLIT" : "CURSOR_HORZ_SPLIT");

	SetWindowLong(hWND, GWL_USERDATA, (long)this);
	bMoving	=	FALSE;

	return TRUE;
}

void 
cSPLITBAR::DrawOutline()
{	PAINTSTRUCT ps;
	HDC			hDC		=	GetDC(NULL);
	RECT		rect;
	cRECT		master;
	cPOINT		pos;

	pos.MousePos();

	master.toScreen(hMASTER);
	if (pos.x < master.x1+40) pos.x = master.x1+40;
	if (pos.x > master.x2-40) pos.x = master.x2-40;
	if (pos.y < master.y1+40) pos.y = master.y1+40;
	if (pos.y > master.y2-40) pos.y = master.y2-40;

	GetWindowRect(hWND, &rect);

	BeginPaint(NULL, &ps);

	if (oldPos.x!=0xffff)
	{	if (kind==dSPLIT_VERT)	rect.left=oldPos.x;else	rect.top=oldPos.y;
		rect.right	=	rect.left+width();
		rect.bottom	=	rect.top +height();

		DrawFocusRect(hDC,&rect);
	}

	if (bMoving)
	{	if (kind==dSPLIT_VERT)	rect.left=pos.x	;else	rect.top=pos.y;
		rect.right	=	rect.left+width();
		rect.bottom	=	rect.top +height();

		DrawFocusRect(hDC,&rect);
	}

	oldPos	=	pos;

	ReleaseDC(NULL,hDC);
	EndPaint(NULL, &ps);
}

void 
cSPLITBAR::Move()
{	DrawOutline();

	cPOINT	pos;
	pos.MousePos();

	cRECT	master;

	master.toScreen(hMASTER);
	if (pos.x < master.x1+40) pos.x = master.x1+40;
	if (pos.x > master.x2-40) pos.x = master.x2-40;
	if (pos.y < master.y1+40) pos.y = master.y1+40;
	if (pos.y > master.y2-40) pos.y = master.y2-40;


	if (kind==dSPLIT_VERT)	MoveWindow(hWND,pos.x,wTop(),width(),height(),TRUE);
	else					MoveWindow(hWND,wLeft(),pos.y,width(),height(),TRUE);
}

LRESULT CALLBACK
cSPLITBAR::splitProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{	cSPLITBAR *Split = (cSPLITBAR *)GetWindowLong(hWnd, GWL_USERDATA);
	
	switch( uMsg )
	{	case WM_SETCURSOR		:
			SetCursor(Split->hCursor);
			break;

		case WM_PAINT			:
			{	PAINTSTRUCT ps;
				HDC		hDC		=	GetDC(hWnd);
				RECT	rect;

				GetClientRect(hWnd, &rect);
				BeginPaint(hWnd, &ps);
				DrawEdge(hDC,&rect, EDGE_RAISED, BF_RECT);
				ReleaseDC(hWnd,hDC);

				EndPaint(hWnd, &ps);
			}
			return TRUE;
			break;

		case WM_LBUTTONDOWN		:
			if (Split->bMoving	==	FALSE)
			{	SetCursor(Split->hCursor);
				SetCapture(hWnd);
				Split->bMoving	= TRUE;
				Split->oldPos.Set(0xffff,0xffff);
			}
			break;

        case WM_MOUSEMOVE		:
			if(Split->bMoving == FALSE) break;
			Split->DrawOutline();
            break;

        case WM_LBUTTONUP		:
			if (Split->bMoving == FALSE) break;
			Split->bMoving = FALSE;
			Split->Move();
			ReleaseCapture();
			break;


		default :
			return( DefWindowProc( hWnd, uMsg, wParam, lParam ) );
	}

	return( 0L );
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
{
	Close();

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

	BOOL	isResult	=	cDIB::Init(hWND);

	return	isResult;
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
{
	if (!IsWindow(hWND))
		return FALSE;

	client.Set(hWND);
	outFrame.OutFrame(hWND);

	outFrame.x2	+=	xs	-	client.x2;
	outFrame.y2	+=	ys	-	client.y2;

	cWND::Resize(&outFrame,repaint);

	return cDIB::Init(hWND);
}

BOOL
cDIBWND::Active()
{
	if	(!IsWindow(hWND))
		return FALSE;
	if	(!cDIB::Active())
		return FALSE;

	return TRUE;
}

void
cDIBWND::Close()
{	cDIB::Close();
	cWND::Close();
}


/********************     Sonaki Foundation Class	************************************

	class cTOOLTIP

****************************************************************************************/

cTOOLTIP::cTOOLTIP()
{	hTOOLTIP	=	NULL;
	hOWNER	=	NULL;
}

cTOOLTIP::~cTOOLTIP()
{	Close();
}

void
cTOOLTIP::Close()
{	if (hTOOLTIP) DestroyWindow(hTOOLTIP);
	hTOOLTIP	=	NULL;
}

BOOL
cTOOLTIP::Init(HWND hwnd,HINSTANCE hinst)
{	Close();

	hOWNER			=	hwnd;
	
	hTOOLTIP		=	CreateWindowEx(	0,TOOLTIPS_CLASS, NULL, TTS_ALWAYSTIP,
										CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
										hwnd, NULL, hinst, NULL );

	toolInfo.cbSize = sizeof( TOOLINFO );
    toolInfo.hwnd   = hOWNER;
    toolInfo.hinst  = NULL;
    toolInfo.uFlags	= TTF_SUBCLASS;

	return TRUE;
}

BOOL
cTOOLTIP::set(HWND hwnd,HINSTANCE hinst,char *str)
{	if (hTOOLTIP)
	{	setText(str);
		return TRUE;
	}

	hOWNER		=	hwnd;
	hTOOLTIP	=	CreateWindowEx(	0,TOOLTIPS_CLASS, NULL, TTS_ALWAYSTIP,
								CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
								hwnd, NULL, hinst, NULL );

	toolInfo.cbSize = sizeof( TOOLINFO );
    toolInfo.hwnd   = hOWNER;
    toolInfo.hinst  = NULL;
    toolInfo.uFlags	= TTF_SUBCLASS;
	
	cRECT	client;
	client.Set(hTOOLTIP);

	addTip(0,client,str);
	setShowTime(10);
	return TRUE;
}

void
cTOOLTIP::addTip(int index,int x1,int y1,int x2,int y2,char *str)
{	if (!hTOOLTIP ) return;

	toolInfo.uId		= index;
	toolInfo.lpszText	= str;
    toolInfo.rect.left  = x1;
    toolInfo.rect.top   = y1;
    toolInfo.rect.right = x2;
    toolInfo.rect.bottom= y2;

	SendMessage( hTOOLTIP, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);
}

void
cTOOLTIP::addTip(int index,cRECT rect,char *str)
{	addTip(index,rect.left(),rect.top(),rect.right(),rect.bottom(),str);
}

void
cTOOLTIP::addTip(int index,HWND item,char *str)
{	if (!hTOOLTIP ) return;

	POINT pos;

	GetWindowRect(item,&toolInfo.rect);

	pos.x=toolInfo.rect.left,pos.y=toolInfo.rect.top;
	ScreenToClient(hOWNER,&pos);

	toolInfo.rect.right	-=	toolInfo.rect.left-pos.x;
	toolInfo.rect.bottom-=	toolInfo.rect.top-pos.y;
	toolInfo.rect.left	=	pos.x;
	toolInfo.rect.top	=	pos.y;

	toolInfo.uId		=	index;
	toolInfo.lpszText	=	str;

	SendMessage( hTOOLTIP, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);
}

void
cTOOLTIP::delTip(int index)
{	if (!hTOOLTIP ) return;
	toolInfo.uId	=	index;
	SendMessage(hTOOLTIP,TTM_DELTOOL,0,(LPARAM)&toolInfo);
}

void
cTOOLTIP::delTip()
{	if (!hTOOLTIP ) return;
	toolInfo.uId	=	0;
	SendMessage(hTOOLTIP,TTM_DELTOOL,0,(LPARAM)&toolInfo);
}

void
cTOOLTIP::setReshowDelay(int delay)
{	if (!hTOOLTIP ) return;
	SendMessage(hTOOLTIP,TTM_SETDELAYTIME,TTDT_RESHOW,delay);
}

void
cTOOLTIP::setShowTime(int delay)
{	if (!hTOOLTIP ) return;
	SendMessage(hTOOLTIP,TTM_SETDELAYTIME,TTDT_INITIAL,delay);
}

void
cTOOLTIP::setText(int index,char *str)
{	if (!hTOOLTIP ) return;
	toolInfo.uId		=	index;
	toolInfo.lpszText	=	str;
	SendMessage(hTOOLTIP,TTM_UPDATETIPTEXT,0,(LPARAM)&toolInfo);
}

void
cTOOLTIP::setText(char *str)
{	if (!hTOOLTIP ) return;
	toolInfo.uId		=	0;
	toolInfo.lpszText	=	str;
	SendMessage(hTOOLTIP,TTM_UPDATETIPTEXT,0,(LPARAM)&toolInfo);
}

/********************     Sonaki Foundation Class	************************************

	class cTOOLBAR

****************************************************************************************/

cTOOLBAR::~cTOOLBAR()
{	Close();
	cWND::Close();
}

BOOL
cTOOLBAR::Init(	HWND hwnd		,	HINSTANCE hInst			,
				UINT wID		,	int nBitmaps			,
				TBBUTTON lpButtons[]	,
				int dxButton, int dyButton,	int dxBitmap, int dyBitmap,
				DWORD bmpId
				)
{	hWND = CreateToolbarEx(	hwnd,WS_CHILD | CCS_ADJUSTABLE  | WS_VISIBLE | WS_BORDER ,//| TBSTYLE_FLAT,
							wID, nBitmaps,(HINSTANCE)HINST_COMMCTRL, 
                            IDB_STD_SMALL_COLOR, 
                            lpButtons, 0, 
							dxButton,dyButton,dxBitmap,dyBitmap, sizeof(TBBUTTON) );

	TBADDBITMAP tb;
	int index, stdidx;

	hBmp = CreateMappedBitmap( hInst, bmpId, 0, NULL, 0 );

	tb.hInst = NULL;
	tb.nID   = (UINT) hBmp;

	stdidx = SendMessage(hWND, TB_ADDBITMAP, nBitmaps, (LPARAM)&tb);

	for (index = 0; index < nBitmaps; index++) 	lpButtons[index].iBitmap += stdidx;

	SendMessage(hWND, TB_ADDBUTTONS,nBitmaps, (LONG)lpButtons);

	return TRUE;
}

void
cTOOLBAR::Close()
{	if (hBmp) DeleteObject(hBmp);
	hBmp	=	NULL;
	cWND::Close();
}

void
cTOOLBAR::Check(UINT id,BOOL check)
{	if (hWND)	SendMessage(hWND,TB_CHECKBUTTON,id,check);
}

BOOL
cTOOLBAR::isCheck(UINT id)
{	if (!hWND)	return FALSE;

	return SendMessage(hWND,TB_ISBUTTONCHECKED,id,0);
}

void
cTOOLBAR::Enable(UINT id)
{	if (hWND)	SendMessage(hWND,TB_ENABLEBUTTON,id,TRUE);
}

void
cTOOLBAR::Disable(UINT id)
{	if (hWND)	SendMessage(hWND,TB_ENABLEBUTTON,id,FALSE);
}

void
cTOOLBAR::SetTip(int index,char *tip)
{	RECT		rect;
	SendMessage(hWND,TB_GETITEMRECT,index,(LPARAM)&rect);

	toolTIP.addTip(index,rect.left,rect.top,rect.right,rect.bottom,tip);
}

void
cTOOLBAR::SetTip(char *tip[],int count)
{	toolTIP.Init(hWND,hINST);

//	for (int index = 0; index < count; index++)	
//		if (tip[index][0]!=0)	SetTip(index,tip[index]);

	int index=0;
	while(strcmp(tip[index],"end")!=0)
	{	if (tip[index][0]!=0)	SetTip(index,tip[index]);
		index++;
	}

	SendMessage(hWND , TB_SETTOOLTIPS, (WPARAM)toolTIP.hTOOLTIP, 0);
}

/********************     Sonaki Foundation Class	************************************

	class cTREEVIEW

****************************************************************************************/

cTREEVIEW::cTREEVIEW()
		  :cWND()
{	hIMAGE=NULL;
}

cTREEVIEW::~cTREEVIEW()
{	Close();
}

BOOL
cTREEVIEW::Init(HWND hWnd,HINSTANCE hInst,DWORD ws,int x,int y,int xs,int ys,HIMAGELIST hImage)
{	Close();
	hWND	= CreateWindowEx( WS_EX_CLIENTEDGE, WC_TREEVIEW, "",
							WS_CHILD | WS_BORDER | WS_VISIBLE | TVS_SINGLEEXPAND | TVS_TRACKSELECT | 
							TVS_NOSCROLL | TVS_NONEVENHEIGHT |TVS_DISABLEDRAGDROP | TVS_HASLINES | ws,
							x,y,xs,ys,hWnd,(HMENU)1,hInst,NULL);

	if (!hWND) return FALSE;
	TreeView_SetImageList( hWND, hImage, TVSIL_NORMAL );

	hIMAGE = hImage;

	selItem		=	NULL;

	return TRUE;
}

BOOL
cTREEVIEW::Init(HWND hWnd,HINSTANCE hInst,HIMAGELIST hImage)
{	hWND	=	hWnd;
	hINST	=	hInst;
	TreeView_SetImageList( hWND, hImage, TVSIL_NORMAL );

	selItem		=	NULL;

	return TRUE;
}

HTREEITEM
cTREEVIEW::Add(char *text,int imgIndex,int imgSelect)
{	if (!hWND) return NULL;
	TV_INSERTSTRUCT tv;

	if (imgSelect==-1) imgSelect=imgIndex;

	tv.hInsertAfter			= TVI_LAST;
	tv.hParent				= TVI_ROOT;

	tv.item.mask			= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tv.item.pszText			= text;
	tv.item.iImage			= imgIndex;
	tv.item.iSelectedImage	= imgSelect;
	tv.item.lParam			= 0;

	return	TreeView_InsertItem( hWND, &tv );
}

HTREEITEM
cTREEVIEW::Add(HTREEITEM hParent,char *text,int imgIndex,int imgSelect)
{	if (!hWND) return NULL;
	TV_INSERTSTRUCT tv;

	if (imgSelect==-1) imgSelect=imgIndex;

	tv.hInsertAfter			= TVI_LAST;
	tv.hParent				= hParent;

	tv.item.mask			= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tv.item.pszText			= text;
	tv.item.iImage			= imgIndex;
	tv.item.iSelectedImage	= imgSelect;
	tv.item.lParam			= 1;

	return	TreeView_InsertItem( hWND, &tv );
}

HTREEITEM
cTREEVIEW::GetRoot(HTREEITEM item)
{	if (!hWND) return NULL;
	HTREEITEM	temp,parent;

	parent	=	item;
	temp  	=	TreeView_GetParent(hWND,item);

	while(1)
	{	if (!temp) return parent;
		else	parent	=	temp;
		temp	=	TreeView_GetParent(hWND,temp);
	}
}

void
cTREEVIEW::Select(HTREEITEM item)
{	if (!hWND) return;
	TreeView_SelectItem(hWND,item);
}

char *
cTREEVIEW::Select()
{	if (!hWND) return NULL;
	HTREEITEM select,child=NULL;
	select	=	TreeView_GetSelection(hWND);
	child	=	TreeView_GetChild(hWND,select);

	if (child)
	{	while(child!=NULL)
		{	child=TreeView_GetChild(hWND,select);
			if (child) select=child;
		}
		Select(select);
	}

    TV_ITEM         tv;

    static char szName[256];

    tv.hItem      = select;
    tv.mask       = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
    tv.pszText    = szName;
    tv.cchTextMax = sizeof( szName );
    TreeView_GetItem( hWND, &tv);

	return szName;
}

BOOL
cTREEVIEW::Select(char *str)
{	return TreeView_GetISearchString(hWND,str);
}

int
cTREEVIEW::getIndex()
{	if (!hWND) return NULL;
	HTREEITEM select;
	select	=	TreeView_GetSelection(hWND);

    TV_ITEM         tv;

    tv.hItem      = select;
    tv.mask       = TVIF_SELECTEDIMAGE | TVIF_PARAM;
    TreeView_GetItem( hWND, &tv);

	return tv.iSelectedImage;
}

HTREEITEM
cTREEVIEW::GetSelect()
{	if (!hWND) return NULL;
	return TreeView_GetSelection(hWND);
}

void
cTREEVIEW::Clear()
{	if (!hWND) return;
	TreeView_DeleteAllItems(hWND);
}

void
cTREEVIEW::Clear(HTREEITEM	item)
{	if (!hWND) return;
	if (!item) return;
	TreeView_DeleteItem(hWND,item);
	item=NULL;
}

void
cTREEVIEW::Expand(HTREEITEM	item)
{	if (!hWND) return;
	if (!item) return;
	
	TreeView_Expand(hWND,item,TVE_EXPAND);
}

void
cTREEVIEW::Close()
{	if (hIMAGE) DeleteObject(hIMAGE);
	hIMAGE=NULL;
	cWND::Close();
}

/********************     Sonaki Foundation Class	************************************

	class cBUTTON

****************************************************************************************/
cBUTTON::cBUTTON()
		:cWND() , cTOOLTIP()
{	hBmp=NULL;
}

cBUTTON::~cBUTTON()
{	Close();
}

void
cBUTTON::Init(char *name,HWND master,HINSTANCE hinst,DWORD ws,DWORD menu,int x,int y,int xs,int ys)
{	Close();
	hINST	=	hinst;
	hWND=CreateWindow("BUTTON",name,WS_CHILD|BS_PUSHBUTTON|ws,x,y,xs,ys,master,(HMENU)menu,hinst,NULL);
	setFont();
	ShowWindow(hWND,SW_SHOW);
}

void
cBUTTON::Init(char *name,HWND master,HINSTANCE hinst,DWORD ws,DWORD menu,int x,int y,int xs,int ys,char *bmpId)
{	Init(name,master,hinst,ws|BS_BITMAP,menu,x,y,xs,ys);
	setImage(bmpId);
}

void
cBUTTON::Init(char *name,HWND master,HINSTANCE hinst,DWORD ws,DWORD menu,int x,int y,int xs,int ys,DWORD bmpId)
{	Init(name,master,hinst,ws|BS_BITMAP,menu,x,y,xs,ys);
	setImage(bmpId);
}

void
cBUTTON::setTip(char *str)
{	cTOOLTIP::Close();
	cTOOLTIP::set(hWND,hINST,str);
}

void
cBUTTON::clearTip()
{	cTOOLTIP::Close();
}

void
cBUTTON::setImage(char *bmpId)
{	if (!hWND) return;

	if (hBmp) DeleteObject(hBmp);
	hBmp = LoadBitmap(hINST,bmpId);

	SendMessage(hWND,BM_SETIMAGE,0,(LPARAM)hBmp);

}

void
cBUTTON::setImage(DWORD bmpId)
{	if (!hWND) return;

	if (hBmp) DeleteObject(hBmp);

	hBmp=CreateMappedBitmap(hINST,bmpId,0,NULL,0);

	SendMessage(hWND,BM_SETIMAGE,0,(LPARAM)hBmp);
}

void
cBUTTON::push()
{	if (!hWND) return;
	SendMessage(hWND,BM_SETCHECK,BST_CHECKED,0);
}

void
cBUTTON::pop()
{	if (!hWND) return;
	SendMessage(hWND,BM_SETCHECK,BST_UNCHECKED,0);
}

BOOL
cBUTTON::isPush()
{	if (!hWND) return FALSE;
	if (SendMessage(hWND,BM_GETSTATE,0,0)==BST_CHECKED) return TRUE;

	return FALSE;
}

void
cBUTTON::Close()
{	if (hBmp) DeleteObject(hBmp);
	hBmp=NULL;
	cWND::Close();
	cTOOLTIP::Close();
}

/********************     Sonaki Foundation Class	************************************

	class cCOMBOBOX

****************************************************************************************/

cCOMBOBOX::cCOMBOBOX()
		  : cWND()
{
}

cCOMBOBOX::~cCOMBOBOX()
{	cWND::Close();
}

void
cCOMBOBOX::Init(HWND hwnd,HINSTANCE hinst,DWORD menu,int x,int y,int xs,int ys,DWORD style)
{
	cWND::Close();

	hWND=CreateWindowEx(WS_EX_TOPMOST,"COMBOBOX","",
						WS_CHILD|WS_VISIBLE | WS_BORDER|WS_VSCROLL|CBS_AUTOHSCROLL | style,
						x,y,xs,ys,hwnd,(HMENU)menu,hinst,NULL);
	setFont();
	ShowWindow(hWND,SW_SHOW);
}

void
cCOMBOBOX::Init(HWND hwnd)
{	
	hWND	=	hwnd;

	setFont();
	ShowWindow(hWND,SW_SHOW);
}

int
cCOMBOBOX::GetCount()
{	return SendMessage(hWND,CB_GETCOUNT,0,0 );
}

int
cCOMBOBOX::Add(char *str)
{	return SendMessage(hWND, CB_ADDSTRING, 0,( LPARAM )str);
}

int
cCOMBOBOX::Add(char *str,DWORD data)
{
	int index	=	Add(str);
	SetData(index,data);

	return	index;
}

int
cCOMBOBOX::Insert(int index,char *str)
{
	return	SendMessage(hWND, CB_INSERTSTRING, index,( LPARAM )str);
}

int
cCOMBOBOX::Insert(int index,char *str,DWORD data)
{
	index	=	Insert(index,str);

	if (index	==	CB_ERR)	return	CB_ERR;

	SetData(index,data);

	return	index;
}

int
cCOMBOBOX::Remove(int index)
{
	return SendMessage(hWND,CB_DELETESTRING,index,0 );
}

int
cCOMBOBOX::RemoveByData(int data)
{
	int i;
	int	count	=	GetCount();

	for (i=0;i<count;i++)
		if (GetData(i)==data)
		{
			Remove(i);

			return	i;
		}

	return CB_ERR;
}

int
cCOMBOBOX::GetSelect()
{
	return SendMessage(hWND,CB_GETCURSEL,0,0 );
}

void
cCOMBOBOX::Reset()
{	SendMessage(hWND,CB_RESETCONTENT,0,0);
}

int
cCOMBOBOX::Select(int sel)
{	return SendMessage(hWND,CB_SETCURSEL,sel,0);
}

int
cCOMBOBOX::Select(char *str)
{	int value	=	SendMessage(hWND,CB_SELECTSTRING,0,(LPARAM)str);
	int	count	=	GetCount();

	return value;
}

int
cCOMBOBOX::SelectByData(int data)
{	int i;
	int	count	=	GetCount();

	for (i=0;i<count;i++)
		if (GetData(i)==data)
		{
			Select(i);

			return	i;
		}

	return CB_ERR;
}

void
cCOMBOBOX::SetData(int index,DWORD data)
{	SendMessage(hWND,CB_SETITEMDATA,index,(LPARAM) data);
}

int 
cCOMBOBOX::GetData(int index)
{	return SendMessage(hWND,CB_GETITEMDATA,index,0);
}

int 
cCOMBOBOX::GetData()
{
	int	select	=	GetSelect();

	select	=	SendMessage(hWND,CB_GETITEMDATA,select,0);

	return	select;
}

char*
cCOMBOBOX::GetString()
{	static char str[256];

	SendMessage(hWND,CB_GETLBTEXT,GetSelect(),(LPARAM)str);

	return str;
}

char*
cCOMBOBOX::GetString(int index)
{	static char str[256];

	SendMessage(hWND,CB_GETLBTEXT,index,(LPARAM)str);

	return str;
}

/********** 정적 멤버 **********/

int 
cCOMBOBOX::Add(HWND hwnd,char *str)
{	return SendMessage(hwnd, CB_ADDSTRING, 0,( LPARAM )str);
}

int 
cCOMBOBOX::Add(HWND hwnd,char *str,DWORD data)
{	int index	=	SendMessage(hwnd, CB_ADDSTRING, 0,( LPARAM )str);
	SetData(hwnd,index,data);

	return	index;
}

int
cCOMBOBOX::Insert(HWND hwnd,int index,char *str)
{
	return	SendMessage(hwnd, CB_INSERTSTRING, index,( LPARAM )str);
}

int
cCOMBOBOX::Insert(HWND hwnd,int index,char *str,DWORD data)
{
	index	=	Insert(hwnd,index,str);

	if (index	==	CB_ERR)	return	CB_ERR;

	SetData(hwnd,index,data);

	return	index;
}

int
cCOMBOBOX::Remove(HWND hwnd,int index)
{
	return SendMessage(hwnd,CB_DELETESTRING,index,0 );
}

int
cCOMBOBOX::RemoveByData(HWND hwnd,int data)
{
	int i;
	int	count	=	GetCount(hwnd);

	for (i=0;i<count;i++)
		if (GetData(hwnd,i)==data)
		{
			Remove(hwnd,i);

			return	i;
		}

	return CB_ERR;
}

int
cCOMBOBOX::GetCount(HWND hwnd)
{	return SendMessage(hwnd,CB_GETCOUNT,0,0 );
}

int
cCOMBOBOX::GetSelect(HWND hwnd)
{	return SendMessage(hwnd,CB_GETCURSEL,0,0 );
}

void
cCOMBOBOX::Reset(HWND hwnd)
{	SendMessage(hwnd,CB_RESETCONTENT,0,0);
}

int 
cCOMBOBOX::Select(HWND hwnd,int sel)
{	return SendMessage(hwnd,CB_SETCURSEL,sel,0);
}

int 
cCOMBOBOX::Select(HWND hwnd,char *str)
{	int value	=	SendMessage(hwnd,CB_SELECTSTRING,0,(LPARAM)str),i;
	int	count	=	cCOMBOBOX::GetCount(hwnd);

	if (strcmp(str,cCOMBOBOX::GetString(hwnd))!=0)
	{	for (i=0;i<count;i++)
			if (strcmp(cCOMBOBOX::GetString(hwnd,i),str)==0)
				cCOMBOBOX::Select(hwnd,i);
	}

	return value;
}

int
cCOMBOBOX::SelectByData(HWND hwnd,int data)
{
	int i;
	int	count	=	GetCount(hwnd);

	for (i=0;i<count;i++)
		if (GetData(hwnd,i)==data)
		{
			Select(hwnd,i);

			return	i;
		}

	return CB_ERR;
}

void
cCOMBOBOX::SetData(HWND hwnd,int index,DWORD data)
{	SendMessage(hwnd,CB_SETITEMDATA,index,(LPARAM) data);
}

int 
cCOMBOBOX::GetData(HWND hwnd,int index)
{	return SendMessage(hwnd,CB_GETITEMDATA,index,0);
}

int 
cCOMBOBOX::GetData(HWND hwnd)
{	return SendMessage(hwnd,CB_GETITEMDATA,GetSelect(hwnd),0);
}

char*
cCOMBOBOX::GetString(HWND hwnd)
{	static char str[256];

	SendMessage(hwnd,CB_GETLBTEXT,GetSelect(hwnd),(LPARAM)str);

	return str;
}

char*
cCOMBOBOX::GetString(HWND hwnd,int index)
{	static char str[256];

	SendMessage(hwnd,CB_GETLBTEXT,index,(LPARAM)str);

	return str;
}

/********************     Sonaki Foundation Class	************************************

	class cSTATUSBAR

****************************************************************************************/

cSTATUSBAR::cSTATUSBAR()
		   :cWND()
{
}

cSTATUSBAR::~cSTATUSBAR()
{	cWND::Close();
}


void
cSTATUSBAR::Init(HWND master,char *text,DWORD id)
{	cWND::Close();
	hWND	=	CreateStatusWindow(WS_CHILD | WS_VISIBLE | CCS_BOTTOM , text, master, id );
	hMASTER	=	master;
	Parts=1;
}

void
cSTATUSBAR::Resize(LPARAM lParam)
{	RECT clrect, strect;

	GetClientRect( hMASTER, &clrect );
    GetClientRect( hWND, &strect );

	MoveWindow( hWND, 0, clrect.bottom-strect.bottom, LOWORD( lParam ), HIWORD( lParam ), TRUE );
}

void
cSTATUSBAR::Set(char *szText,int part)
{	if (part>=Parts) return;
	SendMessage(hWND, SB_SETTEXT,part,(LPARAM)szText );
}

void
cSTATUSBAR::SetPart(int parts,int *Size)
{	Parts	=	parts;
	int size[2];
	size[0]=100;
	size[1]=200;
	SendMessage(hWND, SB_SETPARTS,parts,(LPARAM)&Size[0]);
}

/********************     Sonaki Foundation Class	************************************

	class cSPIN

****************************************************************************************/

cSPIN::cSPIN()
	  :cWND()
{
}

cSPIN::~cSPIN()
{	cWND::Close();
}

void
cSPIN::Init(HWND hwnd,HINSTANCE hinst,int nId,HWND buddy,int low,int upper,int pos)
{	cWND::Close();
	hINST	=	hinst;
	hWND	=	CreateUpDownControl(WS_CHILD | WS_BORDER | WS_VISIBLE |UDS_ALIGNRIGHT,
							 0, 0, 18, 25,hwnd, nId,hINST, buddy,upper,low,pos);
	if (pos < low || pos > upper)
	{	SendMessage(buddy, WM_SETTEXT, 0,(LPARAM) " - ");
		EnableWindow(buddy,FALSE);
		return;
	}
	SendMessage(buddy, WM_SETTEXT, 0, (LPARAM)_ms("%d",pos));
}

void
cSPIN::setBuddy(HWND hwnd)
{	if (!hWND) return;
	SendMessage(hWND,UDM_SETBUDDY,(WPARAM) hwnd,0);
	SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)_ms("%d",getValue()));
}

void
cSPIN::setValue(int pos,BOOL withBuddy)
{	if (!hWND) return;
	HWND buddy=getBuddy();
	if (!buddy) return;
	if (withBuddy) SendMessage(buddy,WM_SETTEXT,0,(LPARAM)_ms("%d",pos));
	SendMessage(hWND,UDM_SETPOS,0,pos);
}

int
cSPIN::getValue()
{	if (!hWND) return 0;
	return	SendMessage(hWND,UDM_GETPOS,0,0);
}

void
cSPIN::setRange(int low,int upper)
{	if (!hWND) return;
	SendMessage(hWND,UDM_SETRANGE,0,upper+(low<<16));
}

HWND
cSPIN::getBuddy()
{	if (!hWND) return NULL;
	return (HWND)SendMessage(hWND,UDM_GETBUDDY,0,0);
}

//	static 
void
cSPIN::init(HWND hwnd,HINSTANCE hinst,int nId,HWND buddy,int low,int upper,int pos)
{	CreateUpDownControl(WS_CHILD | WS_BORDER | WS_VISIBLE |UDS_ALIGNRIGHT,
							 0, 0, 18, 25,hwnd, nId,hinst, buddy,upper,low,pos);
	if (pos < low || pos > upper)
	{	SendMessage(buddy, WM_SETTEXT, 0,(LPARAM) " - ");
		EnableWindow(buddy,FALSE);
		return;
	}
	SendMessage(buddy, WM_SETTEXT, 0, (LPARAM)_ms("%d",pos));
}


void
cSPIN::setBuddy(HWND hwnd,HWND buddy)
{	SendMessage(hwnd,UDM_SETBUDDY,(WPARAM)buddy,0);
	SendMessage(buddy,WM_SETTEXT, 0, (LPARAM)_ms("%d",getValue(hwnd)));
}

void
cSPIN::setValue(HWND hwnd,int pos,BOOL withBuddy)
{	if (withBuddy) SendMessage(getBuddy(hwnd),WM_SETTEXT,0,(LPARAM)_ms("%d",pos));
	SendMessage(hwnd,UDM_SETPOS,0,pos);
}

int
cSPIN::getValue(HWND hwnd)
{	return	SendMessage(hwnd,UDM_GETPOS,0,0);
}

void
cSPIN::setRange(HWND hwnd,int low,int upper)
{	SendMessage(hwnd,UDM_SETRANGE,0,upper+(low<<16));
}

HWND
cSPIN::getBuddy(HWND hwnd)
{	return (HWND)SendMessage(hwnd,UDM_GETBUDDY,0,0);
}

void
cSPIN::setAccel(int num,int *sec,int *inc)
{	UDACCEL *accel;
	accel=new UDACCEL[num];

	for (int i=0;i<num;i++)
	{	accel[i].nSec=sec[i];
		accel[i].nInc=inc[i];
	}

	SendMessage(hWND,UDM_SETACCEL,num,(LPARAM)accel);
	delete [] accel;
}
/********************     Sonaki Foundation Class	************************************

	class cTRACK

****************************************************************************************/

cTRACKBAR::cTRACKBAR()
		  :cWND()
{
}
cTRACKBAR::~cTRACKBAR()
{	cWND::Close();
}


void
cTRACKBAR::Init(HWND hwnd,HINSTANCE hinst,DWORD ws,int x1,int y1,int x2,int y2,DWORD menu)
{	cWND::Close();
	hMASTER	=	hwnd;
	hINST	=	hinst;
	hWND	=	CreateWindowEx(	0, TRACKBAR_CLASS, "",WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | ws,
								x1,y1,x2,y2, hwnd,(HMENU)menu,hinst, NULL );
}

void
cTRACKBAR::setRange(int low,int high)
{	if (!hWND) return;
	SendMessage(hWND,TBM_SETSELSTART,FALSE,low);
	SendMessage(hWND,TBM_SETSELEND,FALSE,high);
	SendMessage(hWND,TBM_SETRANGEMIN,FALSE,low);
	SendMessage(hWND,TBM_SETRANGEMAX,TRUE,high);
}

void
cTRACKBAR::setTick(WORD value)
{	if (!hWND) return;
	SendMessage(hWND,TBM_SETTICFREQ,value,0);
}

void
cTRACKBAR::setPos(int pos)
{	if (!hWND) return;
	SendMessage(hWND,TBM_SETPOS,TRUE,pos);
}

int 
cTRACKBAR::getPos()
{	if (!hWND) return 0;
	return SendMessage(hWND,TBM_GETPOS,0,0);
}

void
cTRACKBAR::setPageSize(int size)
{	if (!hWND) return;
	SendMessage(hWND,TBM_SETPAGESIZE,0,size);
}

void
cTRACKBAR::setThumbSize(int size)
{	if (!hWND) return;
	SendMessage(hWND,TBM_SETTHUMBLENGTH,size,0);
}

/********************     Sonaki Foundation Class	************************************

	class cSTATIC

****************************************************************************************/
cSTATIC::cSTATIC()
		:cWND()
{
}

cSTATIC::~cSTATIC()
{	cWND::Close();
}


int
cSTATIC::Init(HWND hwnd,HINSTANCE hinst,char *str,int x,int y,int xs,int ys,DWORD ws)
{	
	int	width	=	strlen(str)*6;

	cWND::Close();
	hMASTER	=	hwnd;
	hINST	=	hinst;
	hWND	=	CreateWindowEx( 0, "STATIC",str, WS_CHILD | WS_VISIBLE|ws, x, y, width, ys, hwnd,NULL,hinst, NULL );
	setFont();
/*
	HDC	hDC	=	GetDC(hWND);

	SIZE	size;

	GetTextExtentPoint32(hDC,str,strlen(str),&size);

	ReleaseDC(hWND,hDC);
*/
	return	width;
}

HWND
cSTATIC::INIT(HWND hwnd,HINSTANCE hinst,char *str,int x,int y,int xs,int ys,DWORD ws)
{	return CreateWindowEx( 0, "STATIC",str, WS_CHILD | WS_VISIBLE |ws, x, y, xs, ys, hwnd,NULL,hinst, NULL );
}

/********************     Sonaki Foundation Class	************************************

	class cEDITBOX

****************************************************************************************/

char	cEDITBOX::tempStr[1024];

cEDITBOX::cEDITBOX()
		 :cWND()
{
}

cEDITBOX::~cEDITBOX()
{	cWND::Close();
}

void
cEDITBOX::Init(HWND hwnd,HINSTANCE hinst,DWORD menu,char *str,int x,int y,int xs,int ys,DWORD ws)
{	cWND::Close();
	hINST	=	hinst;
	hMASTER	=	hwnd;
	hWND	=	CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT",str, WS_CHILD | WS_VISIBLE | ws, x, y, xs, ys, hwnd,NULL,hinst, NULL );
	setFont();
}

void
cEDITBOX::SetFocus(BOOL select)
{
	if (!IsWindow(hWND))	return;

	SetFocus(hWND);

	if (select)	SendMessage(hWND,EM_SETSEL,0,-1);
}

DWORD CALLBACK 
cEDITBOX::ReadProc(DWORD str,LPBYTE pbBuff,LONG cb,LONG *pcb)
{	strcpy(tempStr,"1234");//(char *)pbBuff);
	return TRUE;
}

char *
cEDITBOX::GetStr()
{	if (!IsWindow(hWND)) return NULL;

	SendMessage(hWND,WM_GETTEXT,1024,(LPARAM)tempStr);

	return tempStr;
}

int
cEDITBOX::GetInt()
{	return atoi(GetStr());	
}

//	정적 멤버

char *
cEDITBOX::GetStr(HWND hwnd)
{	if (!IsWindow(hwnd)) return NULL;

	SendMessage(hwnd,WM_GETTEXT,1024,(LPARAM)tempStr);

	return tempStr;
}

int
cEDITBOX::GetInt(HWND hwnd)
{	return atoi(GetStr(hwnd));	
}

void
cEDITBOX::SetFocus(HWND hwnd,BOOL select)
{
	if (!IsWindow(hwnd))	return;

	SetFocus(hwnd);

	if (select)	SendMessage(hwnd,EM_SETSEL,0,-1);
}

/********************     Sonaki Foundation Class	************************************

	class cLISTBOX

****************************************************************************************/

cLISTBOX::cLISTBOX()
{
	hWND	=	NULL;
}

void
cLISTBOX::Init(HWND hwnd,HINSTANCE hinst,DWORD menu,int x,int y,int xs,int ys,DWORD ws,DWORD wsex)
{
	cWND::Close();

	hINST	=	hinst;
	hMASTER	=	hwnd;
	hWND	=	CreateWindowEx(wsex, "LISTBOX",NULL, WS_CHILD | WS_VISIBLE | ws, x, y, xs, ys, hwnd,(HMENU)menu,hinst, NULL );

	setFont();
}

void
cLISTBOX::Init(HWND hwnd)
{
	hWND	=	hwnd;
}

void
cLISTBOX::Reset()
{
	SendMessage(hWND, LB_RESETCONTENT, 0,0);	//	리스트 리셋
}

int
cLISTBOX::Add(char *str)
{	return	SendMessage(hWND, LB_ADDSTRING, 0,(LPARAM)str);
}

int
cLISTBOX::Add(char *str,int serial)
{	
	int	index	=	Add(str);
	SetData(index,serial);

	return	index;
}

void
cLISTBOX::Del(int index)
{	SendMessage(hWND, LB_DELETESTRING, index,0);
}

int
cLISTBOX::GetSelect(int *list,int _iBufferSize)
{
	return SendMessage(hWND,LB_GETSELITEMS,_iBufferSize,(LPARAM)list);
}

int
cLISTBOX::GetSelect()
{	return SendMessage(hWND,LB_GETCURSEL,0,0);
}

void
cLISTBOX::Select(int index)
{
	if	(wStyle | LBS_EXTENDEDSEL)
		SendMessage(hWND,LB_SETSEL,TRUE,index);
	else
		SendMessage(hWND,LB_SETCURSEL,index,0);
}

void
cLISTBOX::Select(char *str)
{
	SendMessage(hWND,LB_SELECTSTRING,-1,(LPARAM)str);
}

BOOL
cLISTBOX::isSelect(int index)
{	return SendMessage(hWND,LB_GETSEL,index,0);
}

void
cLISTBOX::SetData(int index,DWORD data)
{	SendMessage(hWND,LB_SETITEMDATA,index,(LPARAM)data);
}

int 
cLISTBOX::GetData(int index)
{	return SendMessage(hWND,LB_GETITEMDATA,index,0);
}

int 
cLISTBOX::GetData()
{	int	select	=	GetSelect();
	return SendMessage(hWND,LB_GETITEMDATA,select,0);
}

int
cLISTBOX::GetCount()
{	return SendMessage(hWND,LB_GETCOUNT,0,0);
}

int
cLISTBOX::GetIndexByData(int data)
{	int i;

	int	count	=	GetCount();

	for (i=0;i<count;i++)
		if (GetData(i)==data)
			return	i;

	return CB_ERR;
}

/////////////////////
//	정적 멤버들..
/////////////////////
void
cLISTBOX::Reset(HWND hwnd)
{	SendMessage(hwnd, LB_RESETCONTENT, 0,0);	//	리스트 리셋
}

int
cLISTBOX::Add(HWND hwnd,char *str)
{	return	SendMessage(hwnd, LB_ADDSTRING, 0,(LPARAM)str);
}

int
cLISTBOX::Add(HWND hwnd,char *str,int serial)
{	
	int	index	=	Add(hwnd,str);
	SetData(hwnd,index,serial);

	return	index;
}

void
cLISTBOX::Del(HWND hwnd,int index)
{
	SendMessage(hwnd, LB_DELETESTRING, index,0);
}

char *
cLISTBOX::GetText(HWND hwnd,int index)
{
	static	char	strText[1024];

	if (SendMessage(hwnd, LB_GETTEXT, index,(LPARAM)strText)	==	LB_ERR)	return	NULL;

	return	strText;
}


int
cLISTBOX::GetSelect(HWND hwnd,int *list,int _iBufferSize)
{	
	return SendMessage(hwnd,LB_GETSELITEMS,_iBufferSize,(LPARAM)list);
}

int
cLISTBOX::GetSelectData(HWND hwnd,int *list)
{	int iCount	=	SendMessage(hwnd,LB_GETSELITEMS,100,(LPARAM)list);

	for (int i=0;i<iCount;i++)
	{
		list[i]	=	GetData(hwnd,list[i]);
	}

	return	iCount;
}

int
cLISTBOX::GetSelect(HWND hwnd)
{	return SendMessage(hwnd,LB_GETCURSEL,0,0);
}

void
cLISTBOX::Select(HWND hwnd,int index,BOOL _bReset)
{
	DWORD	dwStyle	=	GetWindowStyle(hwnd);
	int		iResult;

	if	(dwStyle & LBS_EXTENDEDSEL)
	{
		if	(_bReset)
			SendMessage(hwnd,LB_SETSEL,FALSE,-1);

		iResult	=	SendMessage(hwnd,LB_SETSEL,TRUE,index);
	}
	else
		SendMessage(hwnd,LB_SETCURSEL,index,0);
}

void
cLISTBOX::Select(HWND hwnd,char *str)
{	SendMessage(hwnd,LB_SELECTSTRING,-1,(LPARAM)str);
}

BOOL
cLISTBOX::isSelect(HWND hwnd,int index)
{
	return SendMessage(hwnd,LB_GETSEL,index,0);
}

void
cLISTBOX::SetData(HWND hwnd,int index,DWORD data)
{	SendMessage(hwnd,LB_SETITEMDATA,index,(LPARAM)data);
}

int 
cLISTBOX::GetData(HWND hwnd,int index)
{	return SendMessage(hwnd,LB_GETITEMDATA,index,0);
}

int 
cLISTBOX::GetData(HWND hwnd)
{	return SendMessage(hwnd,LB_GETITEMDATA,GetSelect(hwnd),0);
}

int
cLISTBOX::GetCount(HWND hwnd)
{	return SendMessage(hwnd,LB_GETCOUNT,0,0);
}

int
cLISTBOX::GetIndexByData(HWND hwnd,int data)
{	int i;

	int	count	=	GetCount(hwnd);

	for (i=0;i<count;i++)
		if (GetData(hwnd,i)==data)
			return	i;

	return CB_ERR;
}


int
cLISTBOX::SelectByData(HWND hwnd,int data)
{	int i;
	int	count	=	GetCount(hwnd);

	for (i=0;i<count;i++)
		if (GetData(hwnd,i)==data)
		{
			Select(hwnd,i);

			return	i;
		}

	return LB_ERR;
}

BOOL
cLISTBOX::RemoveByData(HWND hwnd,int data)
{	int i;
	int	count	=	GetCount(hwnd);

	for (i=0;i<count;i++)
		if (GetData(hwnd,i)==data)
		{
			Del(hwnd,i);

			return	TRUE;
		}

	return FALSE;
}

/********************     Sonaki Foundation Class	************************************

	class cLISTVIEW

****************************************************************************************/

cLISTVIEW::cLISTVIEW()
		 :cWND()
{	hImage	=	NULL;
	hSmall	=	NULL;
}

void
cLISTVIEW::Close()
{	if (!IsWindow(hWND)) return;

	if ( ListView_GetImageList( hWND, LVSIL_SMALL ) )
        ImageList_Destroy( ListView_GetImageList( hWND, LVSIL_SMALL ) );
	if ( ListView_GetImageList( hWND, LVSIL_NORMAL ) )
		ImageList_Destroy( ListView_GetImageList( hWND, LVSIL_NORMAL ) );

	cWND::Close();
}

BOOL
cLISTVIEW::Init(HWND hwnd,HINSTANCE hinst,DWORD menu,int x,int y,int xs,int ys,DWORD exStyle)
{
	Close();
	hWND	=	CreateWindowEx(WS_EX_CLIENTEDGE,WC_LISTVIEW,"",
								WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS | LVS_SINGLESEL | WS_BORDER|LVS_SHOWSELALWAYS,
								x,y,xs,ys,
								hwnd,(HMENU)menu,hinst,NULL);
	if (!IsWindow(hWND)) 
		return cMSG::Error("리스트뷰 초기화 에러!!","윈도우나 이미지 생성이 안됩니다.");

	if (exStyle)
		ListView_SetExtendedListViewStyle(hWND,exStyle);

	return TRUE;
}

BOOL
cLISTVIEW::Init(HWND hwnd,HINSTANCE hinst,DWORD exStyle)
{	
	Close();

	hWND	=	hwnd;
	hINST	=	hinst;

	if (!IsWindow(hWND)) 
		return cMSG::Error("리스트뷰 초기화 에러!!","윈도우나 이미지 생성이 안됩니다.");

	if (exStyle)
		ListView_SetExtendedListViewStyle(hWND,exStyle);

	return TRUE;
}

void
cLISTVIEW::AddImage(char *iconName)
{	if (!IsWindow(hWND)) return;
	if (!hImage || !hSmall)
	{	hImage	=	ImageList_Create( 32, 32, ILC_COLOR8 | ILC_MASK, 4, 1 ); 
		hSmall	=	ImageList_Create( 16, 16, ILC_COLOR8 | ILC_MASK, 4, 1 );
		ListView_SetImageList( hWND, hImage, LVSIL_NORMAL );
		ListView_SetImageList( hWND, hSmall, LVSIL_SMALL );
	}

	ImageList_AddIcon( hImage, LoadIcon( hINST, iconName ) );
    ImageList_AddIcon( hSmall, LoadIcon( hINST, iconName ) );
}

void
cLISTVIEW::AddColumn(int index,int width,char *str)
{	
	if (!IsWindow(hWND))
		return;

	LV_COLUMN	col;

    col.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM ; 
    col.fmt		= LVCFMT_LEFT;
    col.cx		= width; 

	col.pszText = str; 
	col.iSubItem= index;
	ListView_InsertColumn( hWND, index, &col );
}

void
cLISTVIEW::AddItem(int index,int subItem,char *str,int image)
{	if (!IsWindow(hWND))	return;

	LV_ITEM   item;

	item.mask     = LVIF_TEXT | LVIF_IMAGE;
	item.pszText  = str;
	item.iItem    = index;
	item.iSubItem = subItem;
	item.iImage   = image;
	ListView_InsertItem( hWND, &item );
}

void
cLISTVIEW::AddItem(int index,int subItem,char *str)
{	if (!IsWindow(hWND))	return;

	LV_ITEM   item;

	item.mask		= LVIF_TEXT | LVIF_IMAGE;
	item.pszText	= str;
	item.iSubItem	= subItem;
	item.iImage		= 0;
	item.iItem		= index;
	ListView_InsertItem( hWND, &item );
}

void
cLISTVIEW::SetItem(int index,int subItem,char *str,int image)
{	if (!IsWindow(hWND))	return;

	LV_ITEM   item;

	item.mask     = LVIF_TEXT;
	item.pszText  = str;
	item.iItem    = image;
	item.iSubItem = index;
	item.iImage   = image;
	ListView_SetItem( hWND, &item );
}

void
cLISTVIEW::SetItem(int index,int subItem,char *str)
{	if (!IsWindow(hWND))	return;

	LV_ITEM   item;

	item.mask		= LVIF_TEXT;
	item.pszText	= str;
	item.iSubItem	= subItem;
	item.iItem		= index;
	ListView_SetItem( hWND, &item );
}

void
cLISTVIEW::SetTextColor(int red,int green,int blue)
{	ListView_SetTextColor(hWND,RGB(red,green,blue));
}

void
cLISTVIEW::SetTextColor(DWORD color)
{	ListView_SetTextColor(hWND,color);
}

void
cLISTVIEW::SetBkColor(int red,int green,int blue)
{	ListView_SetBkColor(hWND,RGB(red,green,blue));
}

void
cLISTVIEW::SetBkColor(DWORD color)
{	ListView_SetBkColor(hWND,color);
}

void
cLISTVIEW::SetColor(DWORD text,DWORD bk)
{	SetTextColor(text);
	SetBkColor(bk);
}

void
cLISTVIEW::SetText(int iItem,int subItem,char *str)
{	ListView_SetItemText(hWND,iItem,subItem,str);

}

void
cLISTVIEW::DelItem(int index)
{	if (!IsWindow(hWND)) return;
	ListView_DeleteItem(hWND, index);
}

void
cLISTVIEW::DelColumn(int index)
{	if (!IsWindow(hWND)) return;
	ListView_DeleteColumn(hWND, index);
}


/********************     Sonaki Foundation Class	************************************

	class cSCROLL_BAR

****************************************************************************************/

//
//
//
void
cSCROLL_BAR::Init(HWND hwnd)
{
	m_hWnd		=	hwnd;
}	//	cSCROLL_BAR::Init(HWND hwnd)

//
//
//
void
cSCROLL_BAR::SetWidth(int width,int widthpage)
{
	if (m_iWidth		==	width)
	{
		if (widthpage	==	0xffff				)	return;
		if (widthpage	==	m_iWidthPageSize	)	return;
	}

	m_iWidth			=	width;

	if (widthpage	!=	0xffff)	m_iWidthPageSize	=	widthpage;

	m_iWidth			=	width;
	m_iWidthPageSize	=	widthpage;

	if (m_iWidth	>	m_iWidthPageSize)
	{	SCROLLINFO	si;

		si.cbSize	=	sizeof(SCROLLINFO);
		si.fMask	=	SIF_POS | SIF_RANGE | SIF_PAGE ;
		si.nMin		=	0;
		si.nMax		=	m_iWidth-1;
		si.nPage	=	m_iWidthPageSize;
		si.nPos		=	m_iWidthPos;

		EnableScrollBar	(m_hWnd,SB_HORZ,ESB_ENABLE_BOTH);
		SetScrollInfo	(m_hWnd,SB_HORZ,&si,TRUE);
	}
	else
	{	EnableScrollBar	(m_hWnd,SB_HORZ,ESB_DISABLE_BOTH);
		m_iWidthPos		=	0;
	}

}	//	cSCROLL_BAR::SetWidth(int width,int widthpage)

//
//
void
cSCROLL_BAR::SetHeight(int height,int heightpage)
{
	if (m_iHeight		==	height)
	{
		if (heightpage	==	0xffff				)	return;
		if (heightpage	==	m_iHeightPageSize	)	return;
	}

	m_iHeight			=	height;

	if (heightpage	!=	0xffff)	m_iHeightPageSize	=	heightpage;

	if (m_iHeight	>	m_iHeightPageSize)
	{	SCROLLINFO	si;

		si.cbSize	=	sizeof(SCROLLINFO);
		si.fMask	=	SIF_POS | SIF_RANGE | SIF_PAGE ;
		si.nMin		=	0;
		si.nMax		=	m_iHeight-1;
		si.nPage	=	m_iHeightPageSize;
		si.nPos		=	m_iHeightPos;

		EnableScrollBar	(m_hWnd,SB_VERT,ESB_ENABLE_BOTH);
		SetScrollInfo	(m_hWnd,SB_VERT,&si,TRUE);
	}
	else
	{	EnableScrollBar	(m_hWnd,SB_VERT,ESB_DISABLE_BOTH);
		m_iHeightPos		=	0;
	}
}

//
//	밑으로..
void
cSCROLL_BAR::down()
{
	SCROLLINFO si;

	si.cbSize	=	sizeof( SCROLLINFO );
	si.fMask	=	SIF_ALL;

	GetScrollInfo( m_hWnd,SB_VERT, &si );

	m_iHeightPos ++;

	m_iHeightPos=	min(m_iHeightPos,(int)(si.nMax - si.nPage+1));
	m_iHeightPos=	max(m_iHeightPos,0);

	si.fMask	=	SIF_POS;
	si.nPos		=	m_iHeightPos;

	SetScrollInfo( m_hWnd, SB_VERT, &si, TRUE );
}

//
//	위로..
void
cSCROLL_BAR::up()
{
	SCROLLINFO si;

	si.cbSize	=	sizeof( SCROLLINFO );
	si.fMask	=	SIF_ALL;

	GetScrollInfo( m_hWnd,SB_VERT, &si );

	m_iHeightPos --;

	m_iHeightPos=	min(m_iHeightPos,(int)(si.nMax - si.nPage+1));
	m_iHeightPos=	max(m_iHeightPos,0);

	si.fMask	=	SIF_POS;
	si.nPos		=	m_iHeightPos;

	SetScrollInfo( m_hWnd, SB_VERT, &si, TRUE );
}	//	cSCROLL_BAR::up()

//
//
BOOL
cSCROLL_BAR::VScroll(WORD wScroll )
{	SCROLLINFO si;

	si.cbSize	=	sizeof( SCROLLINFO );
	si.fMask	=	SIF_ALL;

	GetScrollInfo( m_hWnd,SB_VERT, &si );

	switch( wScroll )
	{	case SB_LINEDOWN :
			if ( m_iHeightPos<=	(int)(si.nMax - si.nPage)	)	m_iHeightPos ++;
			break;

		case SB_LINEUP :
			if ( m_iHeightPos>	0							)	m_iHeightPos--;
			break;

		case SB_THUMBTRACK :  // 스크롤바를 잡구 움직였을때..
			GetScrollInfo( m_hWnd, SB_VERT, &si );
			m_iHeightPos	=	si.nTrackPos;
			break;

		case SB_PAGEUP		:
			m_iHeightPos	=	m_iHeightPos	-	si.nPage;
			if (m_iHeightPos < 0	)	m_iHeightPos =	0;
			break;

		case SB_PAGEDOWN	:
			m_iHeightPos	=	m_iHeightPos	+	si.nPage;
			if (m_iHeightPos >	(int)(si.nMax - si.nPage +1))	m_iHeightPos =	si.nMax - si.nPage +1;
			break;
	}

	if (si.nPos != m_iHeightPos )
	{	m_iHeightPos=	min(m_iHeightPos,(int)(si.nMax - si.nPage+1));
		m_iHeightPos=	max(m_iHeightPos,0);

		si.fMask	=	SIF_POS;
		si.nPos		=	m_iHeightPos;

		SetScrollInfo( m_hWnd, SB_VERT, &si, TRUE );

		return	TRUE;
	}

	return	FALSE;
}	//	cSCROLL_BAR::VScroll(HWND hWnd,WORD wScroll )


//
//
//
BOOL
cSCROLL_BAR::HScroll(WORD wScroll )
{	SCROLLINFO si;

	si.cbSize	=	sizeof( SCROLLINFO );
	si.fMask	=	SIF_ALL;

	GetScrollInfo( m_hWnd,SB_HORZ, &si );

	switch( wScroll )
	{	case SB_LINERIGHT	:
			if ( m_iWidthPos<=	(int)(si.nMax - si.nPage)	)	m_iWidthPos ++;
			break;

		case SB_LINELEFT	:
			if ( m_iWidthPos>	0							)	m_iWidthPos--;
			break;

		case SB_THUMBTRACK	:  // 스크롤바를 잡구 움직였을때..
			GetScrollInfo( m_hWnd, SB_HORZ, &si );
			m_iWidthPos	=	si.nTrackPos;
			break;

		case SB_PAGELEFT	:
			m_iWidthPos	=	m_iWidthPos	-	si.nPage;
			if (m_iWidthPos < 0	)	m_iWidthPos =	0;
			break;

		case SB_PAGERIGHT	:
			m_iWidthPos	=	m_iWidthPos	+	si.nPage;
			if (m_iWidthPos >	(int)(si.nMax - si.nPage +1))	m_iWidthPos =	si.nMax - si.nPage +1;
			break;
	}

	if (si.nPos != m_iWidthPos )
	{	m_iWidthPos=	min(m_iWidthPos,(int)(si.nMax - si.nPage+1));
		m_iWidthPos=	max(m_iWidthPos,0);

		si.fMask	=	SIF_POS;
		si.nPos		=	m_iWidthPos;
		
		SetScrollInfo( m_hWnd, SB_HORZ, &si, TRUE );

		return	TRUE;
	}

	return	FALSE;
}	//	cSCROLL_BAR::HScroll(HWND m_hWnd,WORD wScroll )

void
cSCROLL_BAR::SetWidthPos(int pos)
{
	pos			=	min(pos,m_iWidth-m_iWidthPageSize);
	pos			=	max(pos,0);

	if (m_iWidthPos	==	pos)	return;

	m_iWidthPos	=	pos;

	SCROLLINFO si;

	si.cbSize	=	sizeof( SCROLLINFO );
	si.fMask	=	SIF_ALL;

	GetScrollInfo( m_hWnd,SB_HORZ, &si );

	si.fMask	=	SIF_POS;
	si.nPos		=	m_iWidthPos;
	
	SetScrollInfo( m_hWnd, SB_HORZ, &si, TRUE );
}

void
cSCROLL_BAR::SetHeightPos(int pos)
{
	pos			=	min(pos,m_iHeight-m_iHeightPageSize);
	pos			=	max(pos,0);

	if (m_iHeightPos	==	pos)	return;

	m_iHeightPos=	pos;

	SCROLLINFO si;

	si.cbSize	=	sizeof( SCROLLINFO );
	si.fMask	=	SIF_ALL;

	GetScrollInfo( m_hWnd,SB_VERT, &si );

	si.fMask	=	SIF_POS;
	si.nPos		=	m_iHeightPos;
	
	SetScrollInfo( m_hWnd, SB_VERT, &si, TRUE );
}

void
cSCROLL_BAR::Enable()
{
	EnableScrollBar	(m_hWnd,SB_HORZ,ESB_ENABLE_BOTH);
	EnableScrollBar	(m_hWnd,SB_VERT,ESB_ENABLE_BOTH);
}

void
cSCROLL_BAR::Disable()
{
	EnableScrollBar	(m_hWnd,SB_HORZ,ESB_DISABLE_BOTH);
	EnableScrollBar	(m_hWnd,SB_VERT,ESB_DISABLE_BOTH);
}

void
cSCROLL_BAR::EnableWidth()
{
	EnableScrollBar	(m_hWnd,SB_HORZ,ESB_ENABLE_BOTH);
}

void
cSCROLL_BAR::EnableHeight()
{
	EnableScrollBar	(m_hWnd,SB_VERT,ESB_ENABLE_BOTH);
}

void
cSCROLL_BAR::DisableWidth()
{
	EnableScrollBar	(m_hWnd,SB_HORZ,ESB_DISABLE_BOTH);
}

void
cSCROLL_BAR::DisableHeight()
{
	EnableScrollBar	(m_hWnd,SB_VERT,ESB_DISABLE_BOTH);
}


//	세로 스크롤바를 생성하고 비활성화 시킨다.
//
void
cSCROLL_BAR::InitHeight()
{
	EnableHeight();
	SetHeight(2,1);
	DisableHeight();
}

//	가로 스크롤바를 생성하고 활성화 시킨다.
//
void
cSCROLL_BAR::InitWidth()
{
	EnableHeight();
	SetWidth(2,1);
	DisableWidth();
}

//
//	현재 위치가 보이게-_-? 한다.
void
cSCROLL_BAR::correctWidthPossition(int x)
{

	if (m_iWidth	==	0)	return;

	x	=	x-m_iWidthPageSize/2;
	x	=	max(0,x);

	if (x	>	m_iWidth	-	m_iWidthPageSize)	x	=	m_iWidth	-	m_iWidthPageSize;

	SetWidthPos(x);
}	//	cSCROLL_BAR::correctPossition(int x,int y)

//
//	현재 위치가 보이게-_-? 한다.
void
cSCROLL_BAR::correctHeightPossition(int y)
{
	if (m_iHeight	==	0)	return;

	y	=	y-m_iHeightPageSize/2;
	y	=	max(0,y);

	if (y>	m_iHeight	-	m_iHeightPageSize	)	y	=	m_iHeight	-	m_iHeightPageSize;

	SetHeightPos(y);
}	//	cSCROLL_BAR::correctPossition(int x,int y)