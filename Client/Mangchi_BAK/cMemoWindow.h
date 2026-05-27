#ifndef _CMEMOWINDOWH_
#define _CMEMOWINDOWH_

#include "cMAPWND.h"

class cMemoWindow{
public:
	static	HWND		m_hMemo, m_hEdit;
	static	MEMO_INFO	*m_pMemoInfo;
	static	BOOL		m_bShowWindow;
	static	HFONT		m_hEditFont;
	// 10.01.25 ß°¡
	static	char		m_FileName[512];	
	static	void		CreateMemo(HINSTANCE _hInst, HWND _hWnd);
	static	void		SetMemo(MEMO_INFO *_pMemoInfo);
	static	void		Reset(MEMO_INFO	*_pMemoInfo);
	static	char*		ConversionFilename(char *_filename, char *_check);
	static	void		SaveText();

	static	LRESULT CALLBACK	WNDProc(HWND, UINT, WPARAM, LPARAM);
};

#endif
