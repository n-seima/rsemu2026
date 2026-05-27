#ifndef _classCommand_h
#define	_classCommand_h

#include "cSET.H"

class	cCommandDlg	:	public cSET
{
public:
	static	HWND					s_hWnd;
	
	static	void					Init(HWND _hDlg);
	static	void					InitDlg(HWND _hDlg);
	static	void					Launch(HWND _hDlg);

	static	void					ExportItemData(int _iBegin,int _iEnd,char *_lpstrFN);
	static	void					ImportItemData(char *_lpstrFN);

	static	LRESULT CALLBACK		ProcDlg(HWND,UINT,WPARAM,LPARAM);
};

#endif