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
	wc.hIcon		 = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);         // 커서.. 
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);				// 클라이언트 영역의 바탕색깔..
	wc.lpszMenuName  = lpClassName;							// 메뉴 이름.. 리소스에서..
	wc.lpszClassName = lpClassName;							// 클래스 이름..
	wc.cbSize        = sizeof( WNDCLASSEX );                //
	wc.hIconSm       = (HICON)LoadImage( hINST, lpClassName,IMAGE_ICON, 16, 16,LR_DEFAULTCOLOR );
														// 아이콘..
	if ( !RegisterClassEx( &wc ) )
	{	cMSG::Put("Error in cWND::Init","class 등록 실패");
		return( FALSE );        // 윈도우 등록..
	}

	hWND	=	CreateWindowEx(	dwExStyle,lpClassName,lpWindowName,
								dwStyle,
								x,y,nWidth,nHeight,
								hWndParent,hMenu,hInstance,
								lpParam
								);
	
	if (!IsWindow(hWND))
	{	cMSG::Put("Error in cWND::Init","window 생성 실패");
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
{	if (!IsWindow(hWND)) return;
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
cWND::Resize(cRECT rect,BOOL draw)
{	if (!IsWindow(hWND)) return FALSE;
	MoveWindow(hWND,rect.x1,rect.y1,rect.x2,rect.y2,draw);

	client.Set(hWND);
	outFrame.OutFrame(hWND);

	return TRUE;
}
BOOL
cWND::Resize(int x1,int y1,int x2,int y2,BOOL draw)
{	if (!IsWindow(hWND)) return FALSE;
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
{
	if (!IsWindow(hWND)) return;

	EnableWindow(hWND,TRUE);
}

void
cWND::DISABLE()
{
	if (!IsWindow(hWND))	return;

	EnableWindow(hWND,FALSE);
}

void
cWND::setFont(HGDIOBJ obj)
{
	if (!IsWindow(hWND))	return;

	SendMessage(hWND,WM_SETFONT,(WPARAM)obj, 0 );
}

void
cWND::setFont()
{
	if (!IsWindow(hWND))	return;

	SendMessage(hWND,WM_SETFONT,(WPARAM)GetStockObject( DEFAULT_GUI_FONT ), 0 );
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

	cWND::Init(	0,lpClassName,lpClassName,WS_CHILD,
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
{	Close();

	if (!cWND::Init(dwExStyle,lpClassName,lpWindowName,dwStyle,
					x,y,nWidth,nHeight,
					hWndParent,hMenu,hInstance,
					lpParam,show,WndProc
					)
		)
		return FALSE;

	cDIB::Init(cWND::hWND);

	return TRUE;
}

BOOL
cDIBWND::Resize(cRECT rect,BOOL repaint)
{	if (!cWND::hWND) return FALSE;
	cWND::Resize(rect,repaint);
	if (cDIB::hBM) DeleteObject(cDIB::hBM);
	return cDIB::Init(hWND);
}

BOOL
cDIBWND::Active()
{	if (!cWND::hWND) return FALSE;
	if (!cDIB::Active(TRUE)) return FALSE;
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
cTOOLBAR::Init(HWND hwnd,HINSTANCE hInst,UINT wID,int nBitmaps,TBBUTTON lpButtons[],DWORD bmpId)
{	hINST= hInst;
	hWND = CreateToolbarEx(	hwnd,WS_CHILD | WS_VISIBLE | WS_BORDER| TBSTYLE_FLAT,
							wID, nBitmaps,hInst,
                            bmpId,
							NULL,0,
							0,0,0,0, sizeof(TBBUTTON) 
							);

	SendMessage(hWND, TB_ADDBUTTONS,nBitmaps, (LONG)lpButtons);
	SendMessage(hWND, TB_AUTOSIZE,0,0);

	return TRUE;
}

void
cTOOLBAR::Check(UINT id,BOOL check)
{	if (hWND)	SendMessage(hWND,TB_CHECKBUTTON,id,check);
}

BOOL
cTOOLBAR::isCheck(UINT id)
{	if (!IsWindow(hWND))	return FALSE;

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
cTOOLBAR::Close()
{	cWND::Close();
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

	if (!IsWindow(hWND)) return FALSE;
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
{	if (!IsWindow(hWND)) return NULL;
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
{	if (!IsWindow(hWND)) return NULL;
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
{	if (!IsWindow(hWND)) return NULL;
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
{	if (!IsWindow(hWND)) return;
	TreeView_SelectItem(hWND,item);
}

char *
cTREEVIEW::Select()
{	if (!IsWindow(hWND)) return NULL;
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
{	if (!IsWindow(hWND)) return NULL;
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
{	if (!IsWindow(hWND)) return NULL;
	return TreeView_GetSelection(hWND);
}

void
cTREEVIEW::Clear()
{	if (!IsWindow(hWND)) return;
	TreeView_DeleteAllItems(hWND);
}

void
cTREEVIEW::Clear(HTREEITEM	item)
{	if (!IsWindow(hWND)) return;
	if (!item) return;
	TreeView_DeleteItem(hWND,item);
	item=NULL;
}

void
cTREEVIEW::Expand(HTREEITEM	item)
{	if (!IsWindow(hWND)) return;
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
{	if (!IsWindow(hWND)) return;

	if (hBmp) DeleteObject(hBmp);
	hBmp = LoadBitmap(hINST,bmpId);

	SendMessage(hWND,BM_SETIMAGE,0,(LPARAM)hBmp);

}

void
cBUTTON::setImage(DWORD bmpId)
{	if (!IsWindow(hWND)) return;

	if (hBmp) DeleteObject(hBmp);

	hBmp=CreateMappedBitmap(hINST,bmpId,0,NULL,0);

	SendMessage(hWND,BM_SETIMAGE,0,(LPARAM)hBmp);
}

void
cBUTTON::push()
{	if (!IsWindow(hWND)) return;
	SendMessage(hWND,BM_SETCHECK,BST_CHECKED,0);
}

void
cBUTTON::pop()
{	if (!IsWindow(hWND)) return;
	SendMessage(hWND,BM_SETCHECK,BST_UNCHECKED,0);
}

BOOL
cBUTTON::isPush()
{	if (!IsWindow(hWND)) return FALSE;
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
cCOMBOBOX::Init(HWND hwnd,HINSTANCE hinst,DWORD menu,int x,int y,int xs,int ys)
{	hWND=CreateWindowEx(WS_EX_TOPMOST,"COMBOBOX","",
						WS_CHILD|CBS_DROPDOWNLIST | WS_VISIBLE | CBS_HASSTRINGS |WS_VSCROLL,
						x,y,xs,ys,hwnd,(HMENU)menu,hinst,NULL);
	setFont();
	ShowWindow(hWND,SW_SHOW);
}

void
cCOMBOBOX::Add(char *str)
{	SendMessage(hWND, CB_ADDSTRING, 0,( LPARAM )str);
}

int
cCOMBOBOX::GetSelect()
{	return SendMessage(hWND,CB_GETCURSEL,0,0 );
}

void
cCOMBOBOX::Clear()
{	SendMessage(hWND,CB_RESETCONTENT,0,0);
}

void
cCOMBOBOX::Select(int sel)
{	SendMessage(hWND,CB_SETCURSEL,sel,0);
}

int
cCOMBOBOX::GetSelect(HWND hwnd)
{	return SendMessage(hwnd,CB_GETCURSEL,0,0 );
}

void
cCOMBOBOX::Add(HWND hwnd,char *str)
{	SendMessage(hwnd, CB_ADDSTRING, 0,( LPARAM )str);
}

void	
cCOMBOBOX::Clear(HWND hwnd)
{	SendMessage(hwnd,CB_RESETCONTENT,0,0);
}

void
cCOMBOBOX::Select(HWND hwnd,int sel)
{	SendMessage(hwnd,CB_SETCURSEL,sel,0);
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
{	if (!IsWindow(hWND)) return;
	SendMessage(hWND,UDM_SETBUDDY,(WPARAM) hwnd,0);
	SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)_ms("%d",getValue()));
}

void
cSPIN::setValue(int pos,BOOL withBuddy)
{	if (!IsWindow(hWND)) return;
	HWND buddy=getBuddy();
	if (!buddy) return;
	if (withBuddy) SendMessage(buddy,WM_SETTEXT,0,(LPARAM)_ms("%d",pos));
	SendMessage(hWND,UDM_SETPOS,0,pos);
}

int
cSPIN::getValue()
{	if (!IsWindow(hWND)) return 0;
	return	SendMessage(hWND,UDM_GETPOS,0,0);
}

void
cSPIN::setRange(int low,int upper)
{	if (!IsWindow(hWND)) return;
	SendMessage(hWND,UDM_SETRANGE,0,upper+(low<<16));
}

HWND
cSPIN::getBuddy()
{	if (!IsWindow(hWND)) return NULL;
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
{	if (!IsWindow(hWND)) return;
	SendMessage(hWND,TBM_SETSELSTART,FALSE,low);
	SendMessage(hWND,TBM_SETSELEND,FALSE,high);
	SendMessage(hWND,TBM_SETRANGEMIN,FALSE,low);
	SendMessage(hWND,TBM_SETRANGEMAX,TRUE,high);
}

void
cTRACKBAR::setTick(WORD value)
{	if (!IsWindow(hWND)) return;
	SendMessage(hWND,TBM_SETTICFREQ,value,0);
}

void
cTRACKBAR::setPos(int pos)
{	if (!IsWindow(hWND)) return;
	SendMessage(hWND,TBM_SETPOS,TRUE,pos);
}

int 
cTRACKBAR::getPos()
{	if (!IsWindow(hWND)) return 0;
	return SendMessage(hWND,TBM_GETPOS,0,0);
}

void
cTRACKBAR::setPageSize(int size)
{	if (!IsWindow(hWND)) return;
	SendMessage(hWND,TBM_SETPAGESIZE,0,size);
}

void
cTRACKBAR::setThumbSize(int size)
{	if (!IsWindow(hWND)) return;
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


void
cSTATIC::Init(HWND hwnd,HINSTANCE hinst,char *str,int x,int y,int xs,int ys,DWORD ws)
{	cWND::Close();
	hMASTER	=	hwnd;
	hINST	=	hinst;
	hWND	=	CreateWindowEx( 0, "STATIC",str, WS_CHILD | WS_VISIBLE|ws, x, y, xs, ys, hwnd,NULL,hinst, NULL );
	setFont();
}

HWND
cSTATIC::init(HWND hwnd,HINSTANCE hinst,char *str,int x,int y,int xs,int ys,DWORD ws)
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


DWORD CALLBACK 
cEDITBOX::ReadProc(DWORD str,LPBYTE pbBuff,LONG cb,LONG *pcb)
{	strcpy(tempStr,"1234");//(char *)pbBuff);
	return TRUE;
}

char *
cEDITBOX::getStr()
{	EDITSTREAM es;
	
	es.dwCookie		=	(DWORD)tempStr;
	es.dwError		=	0;
	es.pfnCallback	=	ReadProc;
	int a=SendMessage(hWND,EM_STREAMOUT,SF_TEXT,(LPARAM)&es);
	MessageBox(hWND,"asd",_ms("(%s)%d",tempStr,a),MB_OK);

	return tempStr;
}

/********************     Sonaki Foundation Class	************************************

	class cLISTBOX

****************************************************************************************/

void
cLISTBOX::Reset(HWND hwnd)
{	SendMessage(hwnd, LB_RESETCONTENT, 0,0);	//	리스트 리셋
}

void
cLISTBOX::Add(HWND hwnd,char *str)
{	SendMessage(hwnd, LB_ADDSTRING, 0,(LPARAM)str);
}

void
cLISTBOX::Del(HWND hwnd,int index)
{	SendMessage(hwnd, LB_DELETESTRING, index,0);
}

int
cLISTBOX::GetSelect(HWND hwnd,int *list)
{	return SendMessage(hwnd,LB_GETSELITEMS,100,(LPARAM)list);
}

int
cLISTBOX::GetSelect(HWND hwnd)
{	return SendMessage(hwnd,LB_GETCURSEL,0,0);
}

void
cLISTBOX::Select(HWND hwnd,int index)
{	SendMessage(hwnd,LB_SETCURSEL,index,0);
}

void
cLISTBOX::Select(HWND hwnd,char *str)
{	SendMessage(hwnd,LB_SELECTSTRING,-1,(LPARAM)str);
}

BOOL
cLISTBOX::isSelect(HWND hwnd,int index)
{	return SendMessage(hwnd,LB_GETSEL,index,0);
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
{	Close();
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
{	Close();
	hWND	=	hwnd;
	hINST	=	hinst;

	if (!IsWindow(hWND)) 
		return cMSG::Error("리스트뷰 초기화 에러!!","윈도우나 이미지 생성이 안됩니다.");

	if (exStyle)
		ListView_SetExtendedListViewStyle(hWND,exStyle);

	return TRUE;
}

int
cLISTVIEW::GetSelectItem()
{	return ListView_GetNextItem(hWND,-1,LVNI_SELECTED);
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
{	if (!IsWindow(hWND))	return;

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