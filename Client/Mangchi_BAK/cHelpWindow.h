#ifndef _CHELPWINDOWH_
#define _CHELPWINDOWH_


#include "cWINDOW.h"


class CHelpWindow
{
public:
	int		m_iWindowWidth;
	int		m_iWindowHeight;
	static LPCTSTR		m_lpszClass;	// Window Name
	static HWND		m_hWndMain;		// Main Handle
	static int		m_iVscrollPos;


	CHelpWindow();
	virtual ~CHelpWindow();

	int Init(HINSTANCE hInstance, HWND _hWnd);	// Create Window
	static LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam);	// Proc Function
};

#endif 