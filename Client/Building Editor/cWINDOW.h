/*
	윈도우 공통 컨트롤 클래스이다. 	comctl32.lib 를 링크 시켜야함
*/

#ifndef _classWINDOW_H
#define _classWINDOW_H

#include <windows.h>
#include <commctrl.h>
#include <richedit.h>
#include "SFC.H"
#include "cDIB.H"

#define	dSPLIT_VERT		0
#define	dSPLIT_HORZ		1
#pragma comment(lib,"comctl32.lib")   // Windows Multi Media Library

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

	int			thickTop();
	int			thickLeft();
	int			thickBottom();
	int			thickRight();
	void		ENABLE();
	void		DISABLE();
	void		setParent(HWND hwnd);

	void		Close();

	BOOL		Resize(cRECT rect,BOOL draw=TRUE);
	BOOL		Resize(int x1,int y1,int x2,int y2,BOOL draw=TRUE);
	BOOL		Resize(int xs,int ys,BOOL draw=TRUE);

	void		setFont(HGDIOBJ obj);
	void		setFont();
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

	BOOL		Resize(cRECT rect,BOOL repaint=TRUE);
	BOOL		Active();
	void		VScroll(WORD scroll);
	void		HScroll(WORD scroll);
	void		Draw()
				{	HDC	hdc	=	GetDC(hWND);
					cDIB::Flip(hdc);
					ReleaseDC(hWND,hdc);
				}
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
	cTOOLTIP	toolTIP;

				cTOOLBAR()	{	hWND=NULL;}
				~cTOOLBAR();

	BOOL		Init(HWND hwnd,HINSTANCE hInst,UINT wID,int nBitmaps,TBBUTTON lpButtons[],DWORD bmpId);

	void		Close();
	void		Check(UINT id,BOOL check);
	BOOL		isCheck(UINT id);
	void		Enable(UINT id);
	void		Disable(UINT id);
	void		SetTip(int index,char *tip);
	void		SetTip(char *tip[],int count);


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
				
	void		Init(HWND hwnd,HINSTANCE hinst,DWORD menu,int x,int y,int xs,int ys);
	void		Add(char *str);
	int			GetSelect();
	void		Clear();
	void		Select(int sel);
	static int	GetSelect(HWND hwnd);
	static void	Add(HWND hwnd,char *str);
	static void Clear(HWND hwnd);
	static void	Select(HWND hwnd,int sel);

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

	void			Init(HWND hwnd,HINSTANCE hinst,char *str,int x,int y,int xs,int ys,DWORD ws=0);
	static	HWND	init(HWND hwnd,HINSTANCE hinst,char *str,int x,int y,int xs,int ys,DWORD ws=0);

};


class cEDITBOX	: public cWND
{
public:
	static	char	tempStr[1024];

					cEDITBOX();
					~cEDITBOX();
				
	void		Init(HWND hwnd,HINSTANCE hinst,DWORD menu,char *str,int x,int y,int xs,int ys,DWORD ws=0);
	char*		getStr();

	static	DWORD CALLBACK ReadProc(DWORD str,LPBYTE pbBuff,LONG cb,LONG *pcb);

};

class cLISTBOX
{
public:

	static	void	Reset(HWND hwnd);
	static	void	Add(HWND hwnd,char *str);
	static	int		GetSelect(HWND hwnd,int *list);
	static	int		GetSelect(HWND hwnd);
	static	void	Del(HWND hwnd,int index);
	static	void	Select(HWND hwnd,int index);
	static	void	Select(HWND hwnd,char *str);
	static	BOOL	isSelect(HWND hwnd,int index);
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

#endif