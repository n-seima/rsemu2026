#ifndef __MINIMAPWINDOW_H__
#define __MINIMAPWINDOW_H__

#include "cMAP.h"
#include "cWINDOW.h"
#include "cMAPWND.h"
class MinimapWindow : public cDIBWND
{

private:

	int			m_iScale;
	BOOL		bLbuttondown;
	POINT		m_posMinimap;
	DWORD		m_dwCreateTime;
public:

	static POINT			m_posMouse;
	static cMAPWND*			m_pMapWnd;
	static MinimapWindow*	m_pMinimapWnd;

public:
	MinimapWindow();
	virtual ~MinimapWindow();
	
	BOOL		Init(HINSTANCE _hInst, HWND _hWnd, int _xP, int _yP, int _xWidth, int _yHeight,int _iScale);
	BOOL		Init(HINSTANCE _hInst, HWND _hWnd,cMAPWND* _mapWnd, int _xP, int _yP, int _xWidth, int _yHeight,int _iScale);
	void		Draw();
	void		Run();
	int			getScale(){ return m_iScale;}
	static LRESULT CALLBACK	WNDProc(HWND, UINT, WPARAM, LPARAM);

};

#endif
