#include "cWINDOW.H"
#include "cMESSAGE.H"


/********************     Sonaki Foundation Class	************************************

	class cWND

****************************************************************************************/
cRECT_SIZE	cWND::Rect;

cWND::~cWND()
{	Close();
}

BOOL
cWND::Init(DWORD dwExStyle,LPCTSTR lpClassName,LPCTSTR lpWindowName,DWORD dwStyle,
				int x,int y,int nWidth,int nHeight,
				HWND hWndParent,HMENU hMenu,HINSTANCE hInstance,
				LPVOID lpParam,int show,WNDPROC WndProc,HBRUSH backColor
				)
{	WNDCLASSEX wc;   // 윈도우 클래스

	lpszRegister	=	lpClassName;

	hMASTER	=	hWndParent;
	hINST	=	hInstance;
	wStyle	=	dwStyle;

	if (backColor	==	NULL	)	backColor	=	(HBRUSH)(COLOR_WINDOW);

   // Register the main application window class.
   //............................................
	wc.style		 = CS_DBLCLKS |CS_HREDRAW | CS_VREDRAW;	//
	wc.lpfnWndProc   = (WNDPROC)WndProc;					// 윈도우 프로시져 지정..
	wc.cbClsExtra    = 0;									// 여분의 클래스와..
	wc.cbWndExtra    = 0;									// 여분의 윈도우..
	wc.hInstance     = hINST;								// 인스턴스 핸들..
	wc.hIcon		 = LoadIcon(hINST,lpClassName);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);         // 커서.. 
	wc.hbrBackground = backColor;					// 클라이언트 영역의 바탕색깔..
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

	if (!hWND)
	{	cMSG::Put("Error in cWND::Init","window 생성 실패");
		return FALSE;
	}

	ShowWindow( hWND,show);
	UpdateWindow( hWND);

	return TRUE;
}

WNDPROC
cWND::SetProc(WNDPROC WndProc)
{	return (WNDPROC)SetWindowLong(hWND,GWL_WNDPROC,(LONG)WndProc);
}

void
cWND::SetStyle(DWORD style)
{	SetWindowLong(hWND,GWL_STYLE,(LONG)style);
}

void
cWND::SetExStyle(DWORD style)
{	SetWindowLong(hWND,GWL_EXSTYLE,(LONG)style);
}

void
cWND::Close()
{	if (!hWND) return;
	DestroyWindow(hWND);
	if (lpszRegister) UnregisterClass((LPTSTR)lpszRegister,hINST);

	hWND		=	NULL;
	lpszRegister=	NULL;
}

int
cWND::ThickLeft()
{	RECT tmp;
	SetRect(&tmp,100,100,200,200);
	AdjustWindowRect(&tmp,wStyle,TRUE);

	return 100-tmp.left;
}

int
cWND::ThickRight()
{	RECT tmp;
	SetRect(&tmp,100,100,200,200);
	AdjustWindowRect(&tmp,wStyle,TRUE);

	return tmp.right-200;
}

int
cWND::ThickTop()
{	RECT tmp;
	SetRect(&tmp,100,100,200,200);
	AdjustWindowRect(&tmp,wStyle,TRUE);

	return 100-tmp.top;
}

int
cWND::ThickBottom()
{	RECT tmp;
	SetRect(&tmp,100,100,200,200);
	AdjustWindowRect(&tmp,wStyle,TRUE);

	return 200-tmp.right;
}

BOOL
cWND::Move(int x,int y,BOOL draw)
{	if (!hWND) return FALSE;
	Rect.OutFrame(hWND);
	MoveWindow(hWND,x,y,Rect.xs,Rect.ys,draw);

	return TRUE;
}

BOOL
cWND::Add(int x,int y,BOOL draw)
{	if (!hWND) return FALSE;
	Rect.OutFrame(hWND);
	MoveWindow(hWND,Rect.x+x,Rect.y+y,Rect.xs,Rect.ys,draw);

	return TRUE;
}

BOOL
cWND::Resize(cRECT_SIZE *rect,BOOL draw)
{	if (!hWND) return FALSE;
	MoveWindow(hWND,rect->x,rect->y,rect->xs,rect->ys,draw);

	return TRUE;
}

BOOL
cWND::Resize(int xs,int ys,BOOL draw)
{	if (!hWND) return FALSE;

	Rect.OutFrame(hWND);

	cPOINT	pos;

	pos.Set(Rect.x,Rect.y);
	pos.AdjustScreen(hMASTER);

	MoveWindow(hWND,Rect.x,Rect.y,xs,ys,draw);

	return TRUE;
}

BOOL
cWND::Resize(int x,int y,int xs,int ys,BOOL draw)
{	if (!hWND) return FALSE;

	MoveWindow(hWND,x,y,xs,ys,draw);

	return TRUE;
}

void
cWND::Parent(HWND hwnd)
{	hMASTER	=	hwnd;
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
cWND::SetFont(int xs,int ys,int thin,char *charSet)
			  //  12,     6,FW_NORMAL
{	if(!hWND) return;

	static HFONT font=CreateFont(ys,xs,0,0,thin,0,0,0,HANGUL_CHARSET,OUT_DEFAULT_PRECIS,
								CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FIXED_PITCH,charSet);

	SendMessage(hWND,WM_SETFONT,(WPARAM)font, 0 );
}

void
cWND::SetFont()
{	if(!hWND) return;

	static	HFONT font	=	NULL;

	if (!font)	font	=	CreateFont(12,6,0,0,FW_NORMAL,0,0,0,HANGUL_CHARSET,OUT_DEFAULT_PRECIS,
							CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FIXED_PITCH,"돋움체");

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
				LPVOID lpParam,int show,WNDPROC WndProc,HBRUSH back
				)
{	Close();
	if (!cWND::Init(dwExStyle,lpClassName,lpWindowName,dwStyle,
					x,y,nWidth,nHeight,
					hWndParent,hMenu,hInstance,
					lpParam,show,WndProc,back
					)
		)
		return FALSE;

	if (!cDIB::Init(hWND)) return FALSE;

	return TRUE;
}

BOOL
cDIBWND::Init(HWND hwnd,WNDPROC WndProc)
{	if (hwnd==NULL) return FALSE;

	hWND	=	hwnd;
	SetProc((WNDPROC)WndProc);

	return cDIB::Init(hWND);
}

BOOL
cDIBWND::Resize(cRECT_SIZE *rect,BOOL repaint)
{	if (!hWND) return FALSE;

	cWND::Resize(rect,repaint);

	return cDIB::Init(hWND);
}

BOOL
cDIBWND::Resize(int x,int y,int xs,int ys,BOOL repaint)
{	if (!hWND) return FALSE;

	cWND::Resize(x,y,xs,ys,repaint);

	return cDIB::Init(hWND);
}

BOOL
cDIBWND::Resize(int xs,int ys,BOOL repaint)
{	if (!hWND) return FALSE;

	cWND::Resize(xs,ys,repaint);

	return cDIB::Init(hWND);
}

BOOL
cDIBWND::Move(int x,int y,BOOL repaint)
{	if (!hWND) return FALSE;

	cWND::Move(x,y,repaint);

	return cDIB::Init(hWND);
}

BOOL
cDIBWND::Add(int x,int y,BOOL repaint)
{	if (!hWND) return FALSE;

	cWND::Add(x,y,repaint);

	return cDIB::Init(hWND);
}

BOOL
cDIBWND::Active()
{	if (!hWND) return FALSE;
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

	hOWNER	=	hwnd;
	
	hTOOLTIP	=	CreateWindowEx(	0,TOOLTIPS_CLASS, NULL, TTS_ALWAYSTIP,
									CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
									hwnd, NULL, hinst, NULL );

	ToolInfo.cbSize = sizeof( TOOLINFO );
    ToolInfo.hwnd   = hOWNER;
    ToolInfo.hinst  = NULL;
    ToolInfo.uFlags	= TTF_SUBCLASS;

	return TRUE;
}

BOOL
cTOOLTIP::Set(HWND hwnd,HINSTANCE hinst,char *str)
{	if (hTOOLTIP)
	{	SetText(str);
		return TRUE;
	}

	hOWNER		=	hwnd;
	hTOOLTIP	=	CreateWindowEx(	0,TOOLTIPS_CLASS, NULL, TTS_ALWAYSTIP,
								CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
								hwnd, NULL, hinst, NULL );

	ToolInfo.cbSize = sizeof( TOOLINFO );
    ToolInfo.hwnd   = hOWNER;
    ToolInfo.hinst  = NULL;
    ToolInfo.uFlags	= TTF_SUBCLASS;
	
	cRECT_SIZE	client;

	client.ClientFrame(hTOOLTIP);

	AddTip(0,&client,str);
	SetShowTime(10);

	return TRUE;
}

void
cTOOLTIP::AddTip(int index,int x,int y,int xs,int ys,char *str)
{	if (!hTOOLTIP ) return;

	ToolInfo.uId		= index;
	ToolInfo.lpszText	= str;
    ToolInfo.rect.left  = x;
    ToolInfo.rect.top   = y;
    ToolInfo.rect.right = xs;
    ToolInfo.rect.bottom= ys;

	SendMessage( hTOOLTIP, TTM_ADDTOOL, 0, (LPARAM)&ToolInfo);
}

void
cTOOLTIP::AddTip(int index,cRECT_SIZE *rect,char *str)
{	AddTip(index,rect->x,rect->y,rect->xs,rect->ys,str);
}

void
cTOOLTIP::AddTip(int index,HWND item,char *str)
{	if (!hTOOLTIP ) return;

	POINT pos;

	GetWindowRect(item,&ToolInfo.rect);

	pos.x=ToolInfo.rect.left,pos.y=ToolInfo.rect.top;
	ScreenToClient(hOWNER,&pos);

	ToolInfo.rect.right	-=	ToolInfo.rect.left-pos.x;
	ToolInfo.rect.bottom-=	ToolInfo.rect.top-pos.y;
	ToolInfo.rect.left	=	pos.x;
	ToolInfo.rect.top	=	pos.y;

	ToolInfo.uId		=	index;
	ToolInfo.lpszText	=	str;

	SendMessage( hTOOLTIP, TTM_ADDTOOL, 0, (LPARAM)&ToolInfo);
}

void
cTOOLTIP::DelTip(int index)
{	if (!hTOOLTIP ) return;
	ToolInfo.uId	=	index;
	SendMessage(hTOOLTIP,TTM_DELTOOL,0,(LPARAM)&ToolInfo);
}

void
cTOOLTIP::DelTip()
{	if (!hTOOLTIP ) return;
	ToolInfo.uId	=	0;
	SendMessage(hTOOLTIP,TTM_DELTOOL,0,(LPARAM)&ToolInfo);
}

void
cTOOLTIP::SetReshowDelay(int delay)
{	if (!hTOOLTIP ) return;
	SendMessage(hTOOLTIP,TTM_SETDELAYTIME,TTDT_RESHOW,delay);
}

void
cTOOLTIP::SetShowTime(int delay)
{	if (!hTOOLTIP ) return;
	SendMessage(hTOOLTIP,TTM_SETDELAYTIME,TTDT_INITIAL,delay);
}

void
cTOOLTIP::SetText(int index,char *str)
{	if (!hTOOLTIP ) return;

	ToolInfo.uId		=	index;
	ToolInfo.lpszText	=	str;

	SendMessage(hTOOLTIP,TTM_UPDATETIPTEXT,0,(LPARAM)&ToolInfo);
}

void
cTOOLTIP::SetText(char *str)
{	if (!hTOOLTIP ) return;
	ToolInfo.uId		=	0;
	ToolInfo.lpszText	=	str;
	SendMessage(hTOOLTIP,TTM_UPDATETIPTEXT,0,(LPARAM)&ToolInfo);
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
				UINT wID		,	int nBitmaps			,	HINSTANCE hBMInst		, 
				UINT wBMID		,	TBBUTTON lpButtons[]	,
				int iNumButtons	,	int dxButton, int dyButton,	int dxBitmap, int dyBitmap,
				DWORD bmpId
				)
{	hWND = CreateToolbarEx(	hwnd,WS_CHILD | CCS_ADJUSTABLE  | WS_VISIBLE | WS_BORDER | TBSTYLE_FLAT,
							wID, nBitmaps,(HINSTANCE)HINST_COMMCTRL, 
                            IDB_HIST_SMALL_COLOR, 
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
	hBmp=NULL;
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

	toolTIP.AddTip(index,rect.left,rect.top,rect.right,rect.bottom,tip);
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
{	if (IsWindow(hWND))
	{	cWND::View(TRUE);
		SetMenu(hWND,(HMENU)menu);
		Resize(x,y,xs,ys);
		return;
	}

//	Close();
	hINST	=	hinst;
	hWND=CreateWindow("BUTTON",name,WS_CHILD|BS_PUSHBUTTON|ws,x,y,xs,ys,master,(HMENU)menu,hinst,NULL);
	SetFont();
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
cBUTTON::Init(HWND hwnd,HINSTANCE hinst,char *bmpId)
{	hWND	=	hwnd;
	hINST	=	hinst;
	setImage(bmpId);
}

void
cBUTTON::setTip(char *str)
{	cTOOLTIP::Close();
	cTOOLTIP::Set(hWND,hINST,str);
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
{	cWND::Close();

	hWND=CreateWindowEx(WS_EX_TOPMOST,"COMBOBOX","",
						WS_CHILD|CBS_DROPDOWNLIST | WS_VISIBLE | CBS_HASSTRINGS | WS_BORDER|WS_VSCROLL|CBS_AUTOHSCROLL | style,
						x,y,xs,ys,hwnd,(HMENU)menu,hinst,NULL);
	SetFont();
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
cCOMBOBOX::GetSelect()
{	return SendMessage(hWND,CB_GETCURSEL,0,0 );
}

void
cCOMBOBOX::Clear()
{	SendMessage(hWND,CB_RESETCONTENT,0,0);
}

int
cCOMBOBOX::Select(int sel)
{	return SendMessage(hWND,CB_SETCURSEL,sel,0);
}

int
cCOMBOBOX::Select(char *str)
{	int value	=	SendMessage(hWND,CB_SELECTSTRING,0,(LPARAM)str),i;
	int	count	=	GetCount();

	if (strcmp(str,GetString())!=0)
	{	for (i=0;i<count;i++)
			if (strcmp(GetString(i),str)==0)
				Select(i);
	}

	return value;
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
{	return SendMessage(hWND,CB_GETITEMDATA,GetSelect(),0);
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
cCOMBOBOX::GetCount(HWND hwnd)
{	return SendMessage(hwnd,CB_GETCOUNT,0,0 );
}

int
cCOMBOBOX::GetSelect(HWND hwnd)
{	return SendMessage(hwnd,CB_GETCURSEL,0,0 );
}

void
cCOMBOBOX::Clear(HWND hwnd)
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
	minimum	=	low;
	maximum	=	upper;

	setRange(low,upper);
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

int
cSPIN::setValue(int pos,BOOL withBuddy)
{	if (!hWND) return 0;
	pos	=	max(pos,minimum);
	pos	=	min(pos,maximum);

	if (!withBuddy) return pos;

	HWND buddy=getBuddy();
	if (!buddy) return pos;
	if (withBuddy) SendMessage(buddy,WM_SETTEXT,0,(LPARAM)_ms("%d",pos));
	SendMessage(hWND,UDM_SETPOS,0,pos);

	return pos;
}

int
cSPIN::getValue()
{	if (!hWND) return 0;
	return	SendMessage(hWND,UDM_GETPOS,0,0);
}

void
cSPIN::setRange(int low,int upper)
{	if (!hWND) return;
	SendMessage(hWND,UDM_SETRANGE32,(WPARAM)(int)low,(LPARAM)(int)upper);
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

int
cSPIN::setValue(HWND hwnd,int pos,BOOL withBuddy)
{	if (withBuddy) SendMessage(getBuddy(hwnd),WM_SETTEXT,0,(LPARAM)_ms("%d",pos));
	SendMessage(hwnd,UDM_SETPOS,0,pos);

	return 0;
}

int
cSPIN::getValue(HWND hwnd)
{	return	SendMessage(hwnd,UDM_GETPOS,0,0);
}

void
cSPIN::setRange(HWND hwnd,int low,int upper)
{	SendMessage(hwnd,UDM_SETRANGE32,(WPARAM)(int)low,(LPARAM)(int)upper);
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
cTRACKBAR::Init(HWND hwnd)
{	hWND	=	hwnd;
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

void
cTRACKBAR::setRange(HWND hwnd,int low,int high)
{	SendMessage(hwnd,TBM_SETSELSTART,FALSE,low);
	SendMessage(hwnd,TBM_SETSELEND,FALSE,high);
	SendMessage(hwnd,TBM_SETRANGEMIN,FALSE,low);
	SendMessage(hwnd,TBM_SETRANGEMAX,TRUE,high);
}

void
cTRACKBAR::setTick(HWND hwnd,WORD value)
{	SendMessage(hwnd,TBM_SETTICFREQ,value,0);
}

void
cTRACKBAR::setPos(HWND hwnd,int pos)
{	SendMessage(hwnd,TBM_SETPOS,TRUE,pos);
}

int 
cTRACKBAR::getPos(HWND hwnd)
{	return SendMessage(hwnd,TBM_GETPOS,0,0);
}

void
cTRACKBAR::setPageSize(HWND hwnd,int size)
{	SendMessage(hwnd,TBM_SETPAGESIZE,0,size);
}

void
cTRACKBAR::setThumbSize(HWND hwnd,int size)
{	SendMessage(hwnd,TBM_SETTHUMBLENGTH,size,0);
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
	SetFont();
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
	SetFont();
}


DWORD CALLBACK 
cEDITBOX::ReadProc(DWORD str,LPBYTE pbBuff,LONG cb,LONG *pcb)
{	strcpy(tempStr,"1234");//(char *)pbBuff);
	return TRUE;
}

char *
cEDITBOX::GetStr()
{	if (!hWND) return NULL;

	SendMessage(hWND,WM_GETTEXT,1024,(LPARAM)tempStr);

	return tempStr;
}

int
cEDITBOX::GetInt()
{	return atoi(GetStr());	
}

/********************     Sonaki Foundation Class	************************************

	class cLISTVIEW

****************************************************************************************/

HWND	cLISTVIEW::hLIST	=	NULL;
int		cLISTVIEW::sortDir	=	ASC;
int		cLISTVIEW::sortKind	=	dSORT_TEXT;

cLISTVIEW::cLISTVIEW()
		 :cWND()
{	hImage	=	NULL;
	hSmall	=	NULL;
}

void
cLISTVIEW::Close()
{	Param	=	0;

	if (!hWND) return;

	if ( ListView_GetImageList( hWND, LVSIL_SMALL ) )
        ImageList_Destroy( ListView_GetImageList( hWND, LVSIL_SMALL ) );
	if ( ListView_GetImageList( hWND, LVSIL_NORMAL ) )
		ImageList_Destroy( ListView_GetImageList( hWND, LVSIL_NORMAL ) );

	hImage	=	NULL;
	hSmall	=	NULL;

	cWND::Close();
}

BOOL
cLISTVIEW::Init(HWND hwnd,HINSTANCE hinst,DWORD menu,int x,int y,int xs,int ys,DWORD exStyle,DWORD style)
{	Close();

	if (!style)	style = WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS | LVS_SINGLESEL | WS_BORDER|LVS_SHOWSELALWAYS;
	hWND	=	CreateWindowEx(WS_EX_CLIENTEDGE,WC_LISTVIEW,"",style,
								x,y,xs,ys,
								hwnd,(HMENU)menu,hinst,NULL);
	hINST	=	hinst;
	if (!hWND) 
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

	if (!hWND) 
		return cMSG::Error("리스트뷰 초기화 에러!!","윈도우나 이미지 생성이 안됩니다.");

	if (exStyle)
		ListView_SetExtendedListViewStyle(hWND,exStyle);

	return TRUE;
}

void
cLISTVIEW::Reset()
{	if (!hWND)	return;
	
	ListView_DeleteAllItems(hWND);
}

void
cLISTVIEW::Arrange()
{	if (!hWND)	return;
	ListView_Arrange(hWND,LVA_DEFAULT);
}

int CALLBACK pfnCompare(LPARAM lParam1,LPARAM lParam2,LPARAM lParamSort)
{	static	LV_FINDINFO	fi;
	static	int			nItem1,nItem2;
	static	char		szBuf1[512],szBuf2[512];

	if (cLISTVIEW::sortKind == dSORT_NUMERIC)
	{	if (cLISTVIEW::sortDir == 1)return (lParam1 < lParam2 ? -1 : lParam1 > lParam2 ? 1: 0);
		else						return (lParam1 > lParam2 ? -1 : lParam1 < lParam2 ? 1: 0);
	}
	else
	{	fi.flags	=	LVFI_PARAM;
		fi.lParam	=	lParam1;
		nItem1		=	ListView_FindItem(cLISTVIEW::hLIST,-1,&fi);

		fi.lParam	=	lParam2;
		nItem2		=	ListView_FindItem(cLISTVIEW::hLIST,-1,&fi);

		ListView_GetItemText(cLISTVIEW::hLIST,nItem1,1,szBuf1,sizeof(szBuf1));
		ListView_GetItemText(cLISTVIEW::hLIST,nItem2,1,szBuf2,sizeof(szBuf2));

		strcpy(szBuf1,cSTRING::LowCase(szBuf1));
		strcpy(szBuf2,cSTRING::LowCase(szBuf2));

		if (strcmp(szBuf1,szBuf2)!=0)
		{	if (cLISTVIEW::sortDir == 1)return (strcmp(szBuf1,szBuf2));
			else						return (strcmp(szBuf1,szBuf2)*-1);
		}

		if (lParam1 >= 0xffff || lParam2 >= 0xffff)
		if (lParam1 < 0xffff || lParam2 < 0xffff)
			return (lParam1 > lParam2 ? -1 : lParam1 < lParam2 ? 1: 0);

		ListView_GetItemText(cLISTVIEW::hLIST,nItem1,0,szBuf1,sizeof(szBuf1));
		ListView_GetItemText(cLISTVIEW::hLIST,nItem2,0,szBuf2,sizeof(szBuf2));

		strcpy(szBuf1,cSTRING::LowCase(szBuf1));
		strcpy(szBuf2,cSTRING::LowCase(szBuf2));

		if (cLISTVIEW::sortDir == 1)return (strcmp(szBuf1,szBuf2));
		else						return (strcmp(szBuf1,szBuf2)*-1);
	}
}

void
cLISTVIEW::Sort(int column,int sort,int asc)
{	if (asc==0)
	{	if (sortDir	==	ASC)	sortDir	=	DESC;
		else					sortDir	=	ASC;
	}
	else	sortDir	=	asc;

	sortKind=	sort;
	hLIST	=	hWND;

	ListView_SortItems(hWND,pfnCompare,sort);
}

int
cLISTVIEW::GetItemCount()
{	return ListView_GetItemCount(hWND);
}

int
cLISTVIEW::GetSelectedCount()
{	return ListView_GetSelectedCount(hWND);
}

int*
cLISTVIEW::GetSelectedItem()
{	if (GetSelectedCount()<1) return NULL;

	int count	= GetSelectedCount(),index	=	-1;

	static int *indexes	=	NULL;

	indexes		=	new int [count];
	count		=	0;

	while( (index = ListView_GetNextItem(hWND,index,LVNI_SELECTED)) > -1)
		indexes[count++]	=	index;

	return indexes;
}

int
cLISTVIEW::GetSelectItem()
{	return ListView_GetNextItem(hWND,-1,LVNI_SELECTED);
}

void
cLISTVIEW::AddImage(char *iconName)
{	if (!hWND) return;

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
{	if (!hWND)	return;

	LV_COLUMN		col;

    col.mask		= LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    col.fmt			= LVCFMT_LEFT;
    col.cx			= width;

	col.pszText		= str;
	col.iSubItem	= index;
	ListView_InsertColumn( hWND, index, &col );
}

int
cLISTVIEW::AddItem(int subItem,char *str,int image,int param)
{	if (!hWND)	return -1;

	LV_ITEM			item;

	item.mask		= LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;
	item.pszText	= str;
	item.iItem		= ListView_GetItemCount(hWND);
	item.lParam		= param+Param++;
	item.iSubItem	= subItem;
	item.iImage		= image;

//	if (image != -1)
//	{	item.mask	|= LVIF_IMAGE;
//		item.iImage	= image;
//	}

	return ListView_InsertItem( hWND, &item );
}


void
cLISTVIEW::SetItem(int index,int subItem,char *str,int image)
{	if (!hWND)	return;

	LV_ITEM			item;

	item.mask		= LVIF_TEXT;
	item.pszText	= str;
	item.iItem		= index;
	item.iSubItem	= subItem;

	if (image != -1)
	{	item.mask	|= LVIF_IMAGE;
		item.iImage	= image;
	}

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
{	if (!hWND) return;
	ListView_DeleteItem(hWND, index);
}

void
cLISTVIEW::DelColumn(int index)
{	if (!hWND) return;
	ListView_DeleteColumn(hWND, index);
}

void
cLISTVIEW::Select(int index)
{	if (!hWND) return;
	ListView_SetItemState(hWND, index,LVIS_SELECTED | LVIS_FOCUSED,LVIS_SELECTED| LVIS_FOCUSED);
}

void
cLISTVIEW::Update(int index)
{	if (!hWND) return;
	ListView_Update(hWND, index);
}

int
cLISTVIEW::GetHitItem(int x,int y)
{	cPOINT pos;

	if (x==-1)	pos.SetMousePos();
	else		pos.Set(x,y);

	LV_HITTESTINFO hi;

	hi.pt.x = pos.x;
	hi.pt.y = pos.y;

	ScreenToClient(hWND, &hi.pt );

	return ListView_HitTest( hWND, &hi );
}

char *
cLISTVIEW::GetText(int index,int subItem)
{	static char temp[128];

	if (index == -1)
	{	cPOINT pos;

		pos.SetMousePos();

		index = GetHitItem(pos.x,pos.y);
	}

	if (index == -1 ) return NULL;

	ListView_GetItemText(hWND,index,subItem,temp,sizeof(temp));

	return temp;
}

/********************     Sonaki Foundation Class	************************************

	class cLISTBOX

****************************************************************************************/

cLISTBOX::cLISTBOX()
{	hWND	=	NULL;
}

void
cLISTBOX::Init(HWND hwnd,HINSTANCE hinst,DWORD menu,int x,int y,int xs,int ys,DWORD ws,DWORD wsex)
{	cWND::Close();
	hINST	=	hinst;
	hMASTER	=	hwnd;
	hWND	=	CreateWindowEx(wsex, "LISTBOX",NULL, WS_CHILD | WS_VISIBLE | ws, x, y, xs, ys, hwnd,(HMENU)menu,hinst, NULL );

	SetFont();
}

void
cLISTBOX::Init(HWND hwnd)
{	hWND	=	hwnd;
}

void
cLISTBOX::Reset()
{	SendMessage(hWND, LB_RESETCONTENT, 0,0);	//	리스트 리셋
}

void
cLISTBOX::Add(char *str)
{	SendMessage(hWND, LB_ADDSTRING, 0,(LPARAM)str);
}

void
cLISTBOX::Del(int index)
{	SendMessage(hWND, LB_DELETESTRING, index,0);
}

int
cLISTBOX::GetSelect(int *list)
{	return SendMessage(hWND,LB_GETSELITEMS,100,(LPARAM)list);
}

int
cLISTBOX::GetSelect()
{	return SendMessage(hWND,LB_GETCURSEL,0,0);
}

void
cLISTBOX::Select(int index)
{	if (wStyle | LBS_EXTENDEDSEL)	SendMessage(hWND,LB_SETSEL,TRUE,index);
	else							SendMessage(hWND,LB_SETCURSEL,index,0);
	
}

void
cLISTBOX::Select(char *str)
{	SendMessage(hWND,LB_SELECTSTRING,-1,(LPARAM)str);
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
{	return SendMessage(hWND,LB_GETITEMDATA,GetSelect(),0);
}

//	정적 멤버들..
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
