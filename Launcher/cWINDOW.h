/*
	윈도우 공통 컨트롤 클래스이다. 	comctl32.lib 를 링크 시켜야함
*/

#ifndef _classWINDOW_H
#define _classWINDOW_H

#include "SFC.H"
#include <commctrl.h>
#include <richedit.h>
#include "cDIB.H"

#define	dSPLIT_VERT		0
#define	dSPLIT_HORZ		1
#pragma comment(lib,"comctl32.lib")   // Windows Multi Media Library

class	cWND_BASE
{
public:
	virtual		LRESULT CALLBACK	Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){return TRUE;}
};

class cWND
{
public:
	cRECT		client,outFrame;
	HWND		hWND,hMASTER;
	LPCTSTR		lpszRegister;
	HINSTANCE	hINST;
	DWORD		wStyle;
	cPOINT		pos;

				cWND()	{hWND	=	NULL,lpszRegister=NULL;}
				~cWND();

	BOOL		Init(	DWORD dwExStyle,		// extended window style
						LPCTSTR lpClassName,	// pointer to registered class name
						LPCTSTR lpWindowName,	// pointer to window name
						DWORD dwStyle,			// window style
						int x,					// horizontal position of window
						int y,					// vertical position of window
						int nWidth,				// window width  
						int nHeight,			// window height
						HWND hWndParent,		// handle to parent or owner window
						HMENU hMenu,			// handle to menu or child-window identifier
						HINSTANCE hInstance,	// handle to application instance
						LPVOID lpParam,			// pointer to window-creation data
						int show,				//	윈도우 보이는 상태..
						WNDPROC WndProc
						);						//	CreateWindowEx 인자를 그대로 받는다.

	cWND		*GetThis()	{return this;}
	void		Copy(cWND wnd);

	int			width()		{client.Set(hWND);return client.width();}
	int			height()	{client.Set(hWND);return client.height();}
	int			left()		{client.Set(hWND);return client.left();}
	int			right()		{client.Set(hWND);return client.right();}
	int			top()		{client.Set(hWND);return client.top();}
	int			bottom()	{client.Set(hWND);return client.bottom();}

	int			wWidth()	{client.OutFrame(hWND);return client.width();}
	int			wHeight()	{client.OutFrame(hWND);return client.height();}
	int			wLeft()		{client.OutFrame(hWND);return client.left();}
	int			wRight()	{client.OutFrame(hWND);return client.right();}
	int			wTop()		{client.OutFrame(hWND);return client.top();}
	int			wBottom()	{client.OutFrame(hWND);return client.bottom();}
	inline	DWORD		getExStyle(){return	GetWindowLong(hWND,GWL_EXSTYLE);}
	inline	DWORD		getStyle()	{return	GetWindowLong(hWND,GWL_STYLE);}

	int			thickTop();
	int			thickLeft();
	int			thickBottom();
	int			thickRight();
	void		ENABLE();
	void		DISABLE();
	void		setParent(HWND hwnd);

	void		Close();

	BOOL		Resize(cRECT *rect,BOOL draw=TRUE);
	BOOL		Resize(int x1,int y1,int x2,int y2,BOOL draw=TRUE);
	BOOL		Resize(int xs,int ys,BOOL draw=TRUE);
	void		setFont(HGDIOBJ obj);
	void		setFont();
	void		SetStyle(DWORD style)	{SetWindowLong(hWND,GWL_STYLE,(LONG)style);}
	void		SetProc(WNDPROC WndProc);

};

class cSPLITBAR	:	public cWND
{	
protected:
	int			kind;
	HCURSOR		hCursor;
	cPOINT		oldPos;

public:
				cSPLITBAR();
				~cSPLITBAR();

	BOOL		bMoving;
	BOOL		Init(HWND hWndParent,HINSTANCE hInstance,int what,LPCTSTR lpClassName,int x,int y,int xs,int ys);
	void		DrawOutline();
	void		Move();

	static	LRESULT CALLBACK	splitProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
};

class cDIBWND : public cDIB , public cWND
{
public:
				cDIBWND();
				~cDIBWND();

	BOOL		Init(	DWORD dwExStyle,		// extended window style
						LPCTSTR lpClassName,	// pointer to registered class name
						LPCTSTR lpWindowName,	// pointer to window name
						DWORD dwStyle,			// window style
						int x,					// horizontal position of window
						int y,					// vertical position of window
						int nWidth,				// window width  
						int nHeight,			// window height
						HWND hWndParent,		// handle to parent or owner window
						HMENU hMenu,			// handle to menu or child-window identifier
						HINSTANCE hInstance,	// handle to application instance
						LPVOID lpParam,			// pointer to window-creation data
						int show,				//	윈도우 보이는 상태..
						WNDPROC WndProc
						);						//	CreateWindowEx 인자를 그대로 받는다.

	BOOL		Init(HWND hwnd,WNDPROC WndProc);


	BOOL		Resize(cRECT *rect,BOOL repaint=TRUE);
	BOOL		Resize(int xs,int ys,BOOL repaint=TRUE);
	BOOL		Active();
	void		VScroll(WORD scroll);
	void		HScroll(WORD scroll);
	void		Draw()	{cDIB::Flip();}
	void		Draw(int x1,int y1,int x2,int y2)	{cDIB::Flip( x1, y1, x2, y2);}
	void		Close();

};

class cTOOLTIP
{
public:
	HWND			hTOOLTIP,hOWNER;
	TOOLINFO		toolInfo;

					cTOOLTIP();
					~cTOOLTIP();


	BOOL			Init(HWND hwnd,HINSTANCE hinst);
	BOOL			set(HWND hwnd,HINSTANCE hinst,char *str);
	void			Close();

	void			addTip(int index,int x1,int y1,int x2,int y2,char *str);
	void			addTip(int index,cRECT rect,char *str);
	void			addTip(int index,HWND item,char *str);
	void			delTip(int index);
	void			delTip();
	void			setReshowDelay(int delay);
	void			setShowTime(int delay);

	void			setText(int index,char *str);
	void			setText(char *str);
};

class cTOOLBAR	: public cWND
{
public:
	cRECT		rect;
	HBITMAP     hBmp;
	cTOOLTIP	toolTIP;

				cTOOLBAR()	{	hWND=NULL;}
				~cTOOLBAR();

	BOOL		Init(	HWND hwnd,HINSTANCE hInst,
						UINT wID, int nBitmaps,
						TBBUTTON lpButtons[],
						int dxButton, int dyButton,
                        int dxBitmap, int dyBitmap,
						DWORD bmpId
						);

	void		Close();

	void		Check(UINT id,BOOL check);
	BOOL		isCheck(UINT id);
	void		Enable(UINT id);
	void		Disable(UINT id);
	void		SetTip(int index,char *tip);
	void		SetTip(char *tip[],int count=0);

};

class cTREEVIEW  : public cWND
{
public:

	HTREEITEM	selItem;
	HIMAGELIST	hIMAGE;

				cTREEVIEW();
				~cTREEVIEW();

	BOOL		Init(HWND hWnd,HINSTANCE hInst,DWORD ws,int x1,int y1,int xs,int ys,HIMAGELIST hImage);
	BOOL		Init(HWND hWnd,HINSTANCE hInst,HIMAGELIST hImage);
	HTREEITEM	Add(char *text,int imgIndex,int imgSelect=-1);
	HTREEITEM	Add(HTREEITEM hParent,char *text,int imgIndex,int imgSelect=-1);
	HTREEITEM	GetRoot(HTREEITEM item);
	void		Select(HTREEITEM item);
	char *		Select();
	int			getIndex();
	BOOL		Select(char *str);

	HTREEITEM	GetSelect();
	void		Clear();
	void		Clear(HTREEITEM	item);
	void		Expand(HTREEITEM	item);
	void		Close();

};


class cBUTTON : public cWND , public cTOOLTIP
{
public:
	HBITMAP		hBmp;

				cBUTTON();
				~cBUTTON();
	void		setImage(char *bmpId);
	void		setImage(DWORD bmpId);
	void		setTip(char *str);
	void		clearTip();

	void		Init(char *name,HWND master,HINSTANCE hinst,DWORD ws,DWORD menu,int x,int y,int xs,int ys);
	void		Init(char *name,HWND master,HINSTANCE hinst,DWORD ws,DWORD menu,int x,int y,int xs,int ys,char *bmpId);
	void		Init(char *name,HWND master,HINSTANCE hinst,DWORD ws,DWORD menu,int x,int y,int xs,int ys,DWORD bmpId);
	void		push();
	void		pop();
	BOOL		isPush();
	void		Close();

};

class cCOMBOBOX	: public cWND
{
public:
				cCOMBOBOX();
				~cCOMBOBOX();
				
	void		Init(HWND hwnd,HINSTANCE hinst,DWORD menu,int x,int y,int xs,int ys,DWORD style=0);
	void		Init(HWND hwnd);

	int			GetCount();
	int			Add(char *str);
	int			Add(char *str,DWORD data);
	int			Insert(int index,char *str);
	int			Insert(int index,char *str,DWORD data);
	int			Remove(int index);
	int			RemoveByData(int index);
	int			GetSelect();
	void		Reset();
	int			Select(int sel);
	int 		Select(char *str);
	int			SelectByData(int data);
	void		SetData(int index,DWORD data);
	int 		GetData(int index);
	int 		GetData();
	char		*GetString();
	char		*GetString(int index);

	static	int		GetSelect(HWND hwnd);
	static	int 	Add(HWND hwnd,char *str);
	static	int		Add(HWND hwnd,char *str,DWORD data);
	static	int		GetCount(HWND hwnd);
	static	void	Reset(HWND hwnd);
	static	int		Select(HWND hwnd,char *str);
	static	int		Select(HWND hwnd,int sel);
	static	int		SelectByData(HWND hwnd,int data);
	static	void	SetData(HWND hwnd,int index,DWORD data);
	static	int 	GetData(HWND hwnd,int index);
	static	int 	GetData(HWND hwnd);
	static	char	*GetString(HWND hwnd);
	static	char	*GetString(HWND hwnd,int index);
	static	int		Insert(HWND hwnd,int index,char *str);
	static	int		Insert(HWND hwnd,int index,char *str,DWORD data);
	static	int		Remove(HWND hwnd,int index);
	static	int		RemoveByData(HWND hwnd,int data);

};

class cSPIN : public cWND
{
public:
				cSPIN();
				~cSPIN();

	void		Init(HWND hwnd,HINSTANCE hinst,int nId,HWND nBuddy,int low,int upper,int pos);

	void		setBuddy(HWND buddy);
	void		setValue(int pos,BOOL withBuddy=TRUE);
	int			getValue();
	void		setRange(int low,int upper);
	HWND		getBuddy();
	void		setAccel(int num,int *sec,int *inc);

	static void	init(HWND hwnd,HINSTANCE hinst,int nId,HWND nBuddy,int low,int upper,int pos);
	static void	setBuddy(HWND hwnd,HWND buddy);
	static void	setValue(HWND hwnd,int pos,BOOL withBuddy=TRUE);
	static int	getValue(HWND hwnd);
	static void	setRange(HWND hwnd,int low,int upper);
	static HWND	getBuddy(HWND hwnd);
};

class cSTATUSBAR : public cWND
{
public:
	WORD	Parts;

			cSTATUSBAR();
			~cSTATUSBAR();

	void	Init(HWND master,char *text,DWORD id);
	void	Set(char *szText,int part);
	void	Resize(LPARAM lParam);
	void	SetPart(int parts,int *Size);

};

class cTRACKBAR : public cWND
{
public:
			cTRACKBAR();
			~cTRACKBAR();

	void	Init(HWND hwnd,HINSTANCE hinst,DWORD ws,int x1,int y1,int x2,int y2,DWORD menu);

	void	setRange(int low,int high);
	void	setTick(WORD value);
	void	setPos(int pos);
	int		getPos();
	void	setPageSize(int size);
	void	setThumbSize(int size);
	
};

class cSTATIC : public cWND
{
public:
					cSTATIC();
					~cSTATIC();

	int				Init(HWND hwnd,HINSTANCE hinst,char *str,int x,int y,int xs,int ys,DWORD ws=0);
	static	HWND	INIT(HWND hwnd,HINSTANCE hinst,char *str,int x,int y,int xs,int ys,DWORD ws=0);

};


class cEDITBOX	: public cWND
{
public:
	static	char	tempStr[1024];

					cEDITBOX();
					~cEDITBOX();
				
	void			Init(HWND hwnd,HINSTANCE hinst,DWORD menu,char *str,int x,int y,int xs,int ys,DWORD ws=0);
	char*			GetStr();
	int				GetInt();

	void			SetFocus(BOOL select=FALSE);

	static	char*	GetStr(HWND hwnd);
	static	int		GetInt(HWND hwnd);
	static	void	SetFocus(HWND hwnd,BOOL select=FALSE);
	static	DWORD CALLBACK ReadProc(DWORD str,LPBYTE pbBuff,LONG cb,LONG *pcb);

};

class cLISTBOX	:	public cWND
{
public:
					cLISTBOX();

	void			Init(HWND hwnd,HINSTANCE hinst,DWORD menu,int x,int y,int xs,int ys,DWORD ws=0,DWORD wsex=0);
	void			Init(HWND hwnd);
	void			Reset();
	int				Add(char *str);
	int				Add(char *str,int serial);
	int				GetSelect(int *list,int _iBufferSize=100);
	int				GetSelect();
	void			Del(int index);
	void			Select(int index);
	void			Select(char *str);
	int				SelectByData(int data);
	BOOL			isSelect(int index);
	void			SetData(int index,DWORD data);
	int 			GetData(int index);
	int 			GetData();
	char *			GetText(int index);
	int				GetCount();
	int				GetIndexByData(int data);

	static	void	Reset(HWND hwnd);
	static	int 	Add(HWND hwnd,char *str);
	static	int		Add(HWND hwnd,char *str,int serial);
	static	BOOL	RemoveByData(HWND hwnd,int _iData);
	static	int		GetSelect(HWND hwnd,int *list,int _iBufferSize=100);
	static	int		GetSelectData(HWND hwnd,int *list);
	static	int		GetSelect(HWND hwnd);
	static	void	Del(HWND hwnd,int index);
	static	void	Select(HWND hwnd,int index,BOOL _bReset=TRUE);
	static	void	Select(HWND hwnd,char *str);
	static	int		SelectByData(HWND hwnd,int data);
	static	BOOL	isSelect(HWND hwnd,int index);
	static	void	SetData(HWND hwnd,int index,DWORD data);
	static	int 	GetData(HWND hwnd,int index);
	static	int 	GetData(HWND hwnd);
	static	char *	GetText(HWND hwnd,int index);
	static	int		GetCount(HWND hwnd);
	static	int		GetIndexByData(HWND hwnd,int data);

};


class cLISTVIEW	: public cWND
{
public:
	HIMAGELIST	hImage, hSmall;

				cLISTVIEW();

	BOOL		Init(HWND hwnd,HINSTANCE hinst,DWORD menu,int x,int y,int xs,int ys,DWORD exStyle=0);
	BOOL		Init(HWND hwnd,HINSTANCE hinst,DWORD exStyle=0);

	void		Close();
	void		AddImage(char *iconName);
	void		AddColumn(int index,int width,char *str);
	void		AddItem(int index,int subItem,char *str,int image);
	void		AddItem(int index,int subItem,char *str);
	void		DelItem(int index);
	void		DelColumn(int index);
	void		SetItem(int index,int subItem,char *str,int image);
	void		SetItem(int index,int subItem,char *str);


	void		SetTextColor(int red,int green,int blue);
	void		SetTextColor(DWORD color);
	void		SetBkColor(int red,int green,int blue);
	void		SetBkColor(DWORD color);
	void		SetColor(DWORD text,DWORD bk);

	void		SetText(int subItem,int iItem,char *str);
};


class	cSCROLL_BAR
{
public:
	HWND		m_hWnd;
	int			m_iHeight			,m_iWidth;
	int			m_iHeightPageSize	,m_iWidthPageSize;
	int			m_iHeightPos		,m_iWidthPos;

				cSCROLL_BAR()
				{
					m_hWnd	=	NULL;
					memset(this,0,sizeof(cSCROLL_BAR));
				}

	void		Init(HWND hwnd);

	BOOL		VScroll(WORD wScroll);
	BOOL		HScroll(WORD wScroll);
	void		SetWidth(int width,int widthpage);
	void		SetHeight(int height,int heightpage);

	void		down();
	void		up();

	inline	int	GetWidthPos()	{return	m_iWidthPos;}
	inline	int	GetHeightPos()	{return	m_iHeightPos;}
	void		SetWidthPos(int pos);
	void		SetHeightPos(int pos);

	void		InitHeight();			//	세로 스크롤바를 생성하고 비활성화 시킨다.
	void		InitWidth();			//	가로 스크롤바를 생성하고 활성화 시킨다.
	void		correctWidthPossition(int _iX);	//	현재의 가로 위치가 화면에 보이게 보정
	void		correctHeightPossition(int _iY);//	현재의 세로 위치가 화면에 보이게 보정
	void		correctPossition(int _iX,int _iY){correctWidthPossition(_iX);correctHeightPossition(_iY);}

	void		Enable();
	void		Disable();
	void		EnableWidth();
	void		EnableHeight();
	void		DisableWidth();
	void		DisableHeight();
};

#endif