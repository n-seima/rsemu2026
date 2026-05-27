#ifndef _classWINDOW_H
#define _classWINDOW_H

#include "SFC.H"

class cWND
{
public:
	HWND		hWND,hMASTER;
	LPCTSTR		lpszRegister;
	HINSTANCE	hINST;
	DWORD		wStyle;
	CPos		pos;

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

	void		ENABLE();
	void		DISABLE();
	void		setParent(HWND hwnd);
	void		SetProc(WNDPROC WndProc);
	void		SetStyle(DWORD style);

	void		Close();

	BOOL		Resize(cRECT rect,BOOL draw=TRUE);
	BOOL		Resize(int x1,int y1,int x2,int y2,BOOL draw=TRUE);
	void		setFont(int xs,int ys,int thin,char *charSet);
	void		setFont();

	void		View(BOOL show);
	BOOL		IsView();

	static	LRESULT CALLBACK	WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
};

#endif