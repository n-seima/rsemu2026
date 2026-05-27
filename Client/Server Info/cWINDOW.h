#ifndef _classWINDOW_H
#define _classWINDOW_H

#include <windows.h>
#include <commctrl.h>
#include <richedit.h>

#pragma comment(lib,"comctl32.lib")   // Windows Commom Control Library

#include "SFC.H"
#include "cDIB.H"


class cWND
{
public:
	static	cRECT_SIZE	Rect;

	HWND		hWND,hMASTER;
	LPCTSTR		lpszRegister;
	HINSTANCE	hINST;
	DWORD		wStyle;

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
						WNDPROC WndProc,
						HBRUSH back=NULL
						);						//	CreateWindowEx 인자를 그대로 받는다.

	void		ENABLE();
	void		DISABLE();
	void		Parent(HWND hwnd);
	WNDPROC		SetProc(WNDPROC WndProc);
	void		SetStyle(DWORD style);
	void		SetExStyle(DWORD style);

	void		Close();

	BOOL		Move(int x,int y,BOOL draw=TRUE);
	BOOL		Add(int x,int y,BOOL draw=TRUE);
	BOOL		Resize(cRECT_SIZE *rect,BOOL draw=TRUE);
	BOOL		Resize(int x,int y,int xs,int ys,BOOL draw=TRUE);
	BOOL		Resize(int xs,int ys,BOOL draw=TRUE);

	void		SetFont(int xs,int ys,int thin,char *charSet);
	void		SetFont();

	void		View(BOOL show);
	BOOL		IsView();

	int			ThickTop();
	int			ThickLeft();
	int			ThickBottom();
	int			ThickRight();

	int			ClientWidth()	{Rect.ClientFrame(hWND);return Rect.xs;}
	int			ClientHeight()	{Rect.ClientFrame(hWND);return Rect.ys;}
	int			ClientLeft()	{Rect.ClientFrame(hWND);return Rect.x;}
	int			ClientRight()	{Rect.ClientFrame(hWND);return Rect.Right();}
	int			ClientTop()		{Rect.ClientFrame(hWND);return Rect.y;}
	int			ClientNottom()	{Rect.ClientFrame(hWND);return Rect.Bottom();}

	int			OutWidth()		{Rect.OutFrame(hWND);return Rect.xs;}
	int			OutHeight()		{Rect.OutFrame(hWND);return Rect.ys;}
	int			OutLeft()		{Rect.OutFrame(hWND);return Rect.x;}
	int			OutRight()		{Rect.OutFrame(hWND);return Rect.Right();}
	int			OutTop()		{Rect.OutFrame(hWND);return Rect.y;}
	int			OutBottom()		{Rect.OutFrame(hWND);return Rect.Bottom();}

	static	LRESULT CALLBACK	WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
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
						WNDPROC WndProc,
						HBRUSH back=NULL
						);						//	CreateWindowEx 인자를 그대로 받는다.

	BOOL		Init(HWND hwnd,WNDPROC WndProc);

	BOOL		Add(int x,int y,BOOL draw=TRUE);
	BOOL		Move(int x,int y,BOOL repaint=TRUE);
	BOOL		Resize(cRECT_SIZE *rect,BOOL repaint=TRUE);
	BOOL		Resize(int xs,int ys,BOOL repaint=TRUE);
	BOOL		Resize(int x,int y,int xs,int ys,BOOL repaint=TRUE);
	BOOL		Active();
	void		VScroll(WORD scroll);
	void		HScroll(WORD scroll);
	void		Draw(cRECT *rect=NULL)	{cDIB::Flip(rect);}
	void		Close();

};

class cTOOLTIP
{
public:
	HWND			hTOOLTIP,hOWNER;
	TOOLINFO		ToolInfo;

					cTOOLTIP();
					~cTOOLTIP();


	BOOL			Init(HWND hwnd,HINSTANCE hinst);
	BOOL			Set(HWND hwnd,HINSTANCE hinst,char *str);
	void			Close();

	void			AddTip(int index,int x1,int y1,int x2,int y2,char *str);
	void			AddTip(int index,cRECT_SIZE *rect,char *str);
	void			AddTip(int index,HWND item,char *str);
	void			DelTip(int index);
	void			DelTip();
	void			SetReshowDelay(int delay);
	void			SetShowTime(int delay);

	void			SetText(int index,char *str);
	void			SetText(char *str);
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
                        HINSTANCE hBMInst, 
						UINT wBMID, 
						TBBUTTON lpButtons[],
                        int iNumButtons,
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
	void		Delete(HTREEITEM item);
	int			getIndex();
	BOOL		Select(char *str);
	void		SetText(HTREEITEM item,char *_strText);


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

	void		Init(HWND hwnd,HINSTANCE hinst,char *bmpId);
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
	int			GetSelect();
	void		Clear();
	int			Select(int sel);
	int 		Select(char *str);
	void		SetData(int index,DWORD data);
	int 		GetData(int index);
	int 		GetData();
	char		*GetString();
	char		*GetString(int index);

	static int	GetSelect(HWND hwnd);
	static int 	Add(HWND hwnd,char *str);
	static int	GetCount(HWND hwnd);
	static void	Clear(HWND hwnd);
	static int	Select(HWND hwnd,char *str);
	static int	Select(HWND hwnd,int sel);
	static void	SetData(HWND hwnd,int index,DWORD data);
	static int 	GetData(HWND hwnd,int index);
	static int 	GetData(HWND hwnd);
	static char	*GetString(HWND hwnd);
	static char	*GetString(HWND hwnd,int index);
};

class cSPIN : public cWND
{
public:
				cSPIN();
				~cSPIN();

	int			minimum,maximum;
	void		Init(HWND hwnd,HINSTANCE hinst,int nId,HWND nBuddy,int low,int upper,int pos);

	void		setBuddy(HWND buddy);
	int 		setValue(int pos,BOOL withBuddy=TRUE);
	int			getValue();
	void		setRange(int low,int upper);
	HWND		getBuddy();
	void		setAccel(int num,int *sec,int *inc);

	static void	init(HWND hwnd,HINSTANCE hinst,int nId,HWND nBuddy,int low,int upper,int pos);
	static void	setBuddy(HWND hwnd,HWND buddy);
	static int 	setValue(HWND hwnd,int pos,BOOL withBuddy=TRUE);
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

	void			Init(HWND hwnd,HINSTANCE hinst,DWORD ws,int x1,int y1,int x2,int y2,DWORD menu);
	void			Init(HWND hwnd);

	void			setRange(int low,int high);
	void			setTick(WORD value);
	void			setPos(int pos);
	int				getPos();
	void			setPageSize(int size);
	void			setThumbSize(int size);
	
	static	void	setRange(HWND hwnd,int low,int high);
	static	void	setTick(HWND hwnd,WORD value);
	static	void	setPos(HWND hwnd,int pos);
	static	int		getPos(HWND hwnd);
	static	void	setPageSize(HWND hwnd,int size);
	static	void	setThumbSize(HWND hwnd,int size);
	
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
	char*		GetStr();
	int			GetInt();

	static	DWORD CALLBACK ReadProc(DWORD str,LPBYTE pbBuff,LONG cb,LONG *pcb);

};

class cLISTBOX	:	public cWND
{
public:
					cLISTBOX();

	void			Init(HWND hwnd,HINSTANCE hinst,DWORD menu,int x,int y,int xs,int ys,DWORD ws=0,DWORD wsex=0);
	void			Init(HWND hwnd);
	void			Reset();
	void			Add(char *str);
	int				GetSelect(int *list);
	int				GetSelect();
	void			Del(int index);
	void			Select(int index);
	void			Select(char *str);
	BOOL			isSelect(int index);
	void			SetData(int index,DWORD data);
	int 			GetData(int index);
	int 			GetData();

	static	void	Reset(HWND hwnd);
	static	void	Add(HWND hwnd,char *str);
	static	int		GetSelect(HWND hwnd,int *list);
	static	int		GetSelect(HWND hwnd);
	static	void	Del(HWND hwnd,int index);
	static	void	Select(HWND hwnd,int index);
	static	void	Select(HWND hwnd,char *str);
	static	BOOL	isSelect(HWND hwnd,int index);
	static	void	SetData(HWND hwnd,int index,DWORD data);
	static	int 	GetData(HWND hwnd,int index);
	static	int 	GetData(HWND hwnd);

};

#define	dSORT_NUMERIC	0
#define	dSORT_TEXT		1

#define	ASC				1			//	소트 순서 순차적
#define	DESC			2			//	소트 순서 반대로


class cLISTVIEW	: public cWND
{
public:
	static		HWND	hLIST;
	static		int		sortDir,sortKind;

	HIMAGELIST	hImage, hSmall;
	DWORD		Param;						//	소트나 기타 거시기에 사용될 녀석..

				cLISTVIEW();

	BOOL		Init(HWND hwnd,HINSTANCE hinst,DWORD menu,int x,int y,int xs,int ys,DWORD exStyle=0,DWORD style=0);
	BOOL		Init(HWND hwnd,HINSTANCE hinst,DWORD exStyle=0);

	void		Close();
	void		Reset();

	void		AddImage(char *iconName);
	void		AddColumn(int index,int width,char *str);
	int			AddItem(int subItem,char *str,int image=-1,int param=0);
	void		SetItem(int index,int subItem,char *str,int image=-1);
	void		SetItem(int index,int subItem,int iValue);
	void		DelItem(int index);
	void		DelColumn(int index);

	int			GetItemCount();
	int			GetSelectedCount();
	int*		GetSelectedItem(int *_lpCount);
	int			GetSelectItem();
	int			getColumnWidth(int _iColumn);
	int			getData(int index,int subItem);

	void		SetTextColor(int red,int green,int blue);
	void		SetTextColor(DWORD color);
	void		SetBkColor(int red,int green,int blue);
	void		SetBkColor(DWORD color);
	void		SetColor(DWORD text,DWORD bk);

	void		Select(int index);
	void		Update(int index);

	void		SetText(int subItem,int iItem,char *str);
	int			GetHitItem(int x=-1,int y=-1);					//	선택한 아이템 인덱스
	char		*GetText(int index=-1,int subItem=0);
	void		Arrange();
	void		Sort(int column,int sort,int asc=1);
};

#endif